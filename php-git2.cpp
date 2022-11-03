/*
 * php-git2.cpp
 *
 * Copyright (C) Roger P. Gee
 */

#include "php-git2.h"
#include "php-resource.h"
#include "php-object.h"
using namespace std;
using namespace php_git2;

ZEND_DECLARE_MODULE_GLOBALS(git2)

// Extension setup functions defined in this unit.
static PHP_GINIT_FUNCTION(git2);
static PHP_GSHUTDOWN_FUNCTION(git2);
static PHP_MINIT_FUNCTION(git2);
static PHP_MSHUTDOWN_FUNCTION(git2);
static PHP_MINFO_FUNCTION(git2);
static PHP_RINIT_FUNCTION(git2);
static PHP_RSHUTDOWN_FUNCTION(git2);
static int php_git2_post_deactivate();

// Module entry table.
zend_module_entry git2_module_entry = {
    STANDARD_MODULE_HEADER,
    PHP_GIT2_EXTNAME,
    php_git2::functions,
    PHP_MINIT(git2),
    PHP_MSHUTDOWN(git2),
    PHP_RINIT(git2),
    PHP_RSHUTDOWN(git2),
    PHP_MINFO(git2),
    PHP_GIT2_EXTVER,
    PHP_MODULE_GLOBALS(git2),
    PHP_GINIT(git2),
    PHP_GSHUTDOWN(git2),
    php_git2_post_deactivate,
    STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_GIT2
ZEND_GET_MODULE(git2)
#endif

// Create php.ini settings.
PHP_INI_BEGIN()
PHP_INI_END()

// Implementation of internal functions.

PHP_GINIT_FUNCTION(git2)
{
}

PHP_GSHUTDOWN_FUNCTION(git2)
{
}

PHP_MINIT_FUNCTION(git2)
{
    php_git2_globals_init();
    REGISTER_INI_ENTRIES();

    // Call the function to register all resource types. Whenever a resource
    // type is added, the libgit2 data type name should be added to the list of
    // template parameters.
    php_git2_define_resource_types<
        git_annotated_commit,
        git_blame,
        git_blob,
        git_branch_iterator,
        git_commit,
        git_config,
        git_config_iterator,
        git_cred,
        git_describe_result,
        git_diff,
        git_diff_stats,
        git_index,
        git_index_conflict_iterator,
        git_indexer,
        git_note,
        git_note_iterator,
        git_object,
        git_odb,
        git_odb_object,
        git_packbuilder,
        git_patch,
        git_rebase,
        git_refdb,
        git_reference,
        git_reference_iterator,
        git_reflog,
        const git_refspec,
        git_remote,
        git_repository,
        git_revwalk,
        git_signature,
        git_status_list,
        git_submodule,
        git_tag,
        git_tree,
        git_tree_entry,
        git_treebuilder,
        git_worktree
        >(module_number);

    // Register all classes provided by this extension.
    php_git2_register_classes();

    // Register all libgit2 constants.
    php_git2_register_constants(module_number);

    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(git2)
{
#ifndef ZTS
    php_git2_globals_dtor(&git2_globals);
#endif

    return SUCCESS;
}

PHP_MINFO_FUNCTION(git2)
{
    char buf[128];
    int major, minor, rev;

    git_libgit2_version(&major,&minor,&rev);
    snprintf(buf,sizeof(buf),"%d.%d.%d",major,minor,rev);

    php_info_print_table_start();
    php_info_print_table_row(2,PHP_GIT2_EXTNAME,"enabled");
    php_info_print_table_row(2,"extension version",PHP_GIT2_EXTVER);
    php_info_print_table_row(2,"libgit2 version",buf);
    php_info_print_table_end();

    DISPLAY_INI_ENTRIES();
}

PHP_RINIT_FUNCTION(git2)
{
    php_git2_globals_request_init();

    // Initialize git2 library.
    git_libgit2_init();

    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(git2)
{
    php_git2_globals_request_shutdown();

    return SUCCESS;
}

int php_git2_post_deactivate()
{
    // Deinitialize libgit2. At this point, all resources should have been
    // freed. This means they would call their destructors and all libgit2
    // memory should be freed.
    git_libgit2_shutdown();

    return SUCCESS;
}

// php_git2_exception__with_message

php_git2_exception__with_message::php_git2_exception__with_message()
{
    buf = new (emalloc(sizeof(buffer))) buffer;
}

php_git2_exception__with_message::
php_git2_exception__with_message(const php_git2_exception__with_message& inst):
    php_git2_exception_base(inst)
{
    buf = inst.buf;
    buf->ref += 1;
}

php_git2_exception__with_message::~php_git2_exception__with_message()
{
    if (--buf->ref <= 0) {
        buf->~buffer();
        efree(buf);
    }
}

php_git2_exception__with_message&
php_git2_exception__with_message::operator =(const php_git2_exception__with_message& inst)
{
    if (this != &inst) {
        buf = inst.buf;
        buf->ref += 1;
    }

    return *this;
}

// php_git2_exception

php_git2_exception::php_git2_exception(const char* format, ...)
{
    va_list args;
    va_start(args,format);
    set_message(format,args);
    va_end(args);
}

// php_git2_error_exception

php_git2_error_exception::php_git2_error_exception(const char* format, ...)
{
    va_list args;
    va_start(args,format);
    set_message(format,args);
    va_end(args);
}

// php_exception_wrapper

const char* php_exception_wrapper::get_message() const
{
    zval rv;
    zval* msg;

    msg = zend_read_property(Z_OBJCE(zex),
        const_cast<zval*>(&zex),
        "message",
        sizeof("message")-1,
        1,
        &rv);

    return msg == nullptr ? nullptr : Z_STRVAL_P(msg);
}

void php_exception_wrapper::set_giterr() const
{
    const char* msg = get_message();

    if (msg != nullptr) {
        php_git2_giterr_set(GITERR_INVALID,msg);
    }
    else {
        php_git2_giterr_set(GITERR_INVALID,"An exception occurred during the operation");
    }
}

void php_exception_wrapper::throw_error() const
{
    const char* msg = get_message();

    if (msg != nullptr) {
        throw php_git2_error_exception(msg);
    }

    throw php_git2_error_exception("An exception occurred during the operation");
}

// php_git2::git_error<int>()

template<>
void php_git2::git_error(int code)
{
    // Treat EUSER as handled in user space. This will not generate an
    // exception.
    if (code == GIT_EUSER) {
        return;
    }

    // Pull libgit2 exception.
    const ::git_error* err = giterr_last();
    if (err == nullptr) {
        throw php_git2_exception("libgit2 no error");
    }

    if (code == GIT_EPHP) {
        php_git2_exception ex("%s",err->message);
        ex.code = code;
        giterr_clear();
        throw ex;
    }

    if (code == GIT_EPHP_ERROR) {
        php_git2_error_exception ex(err->message);
        ex.code = code;
        giterr_clear();
        throw ex;
    }

    if (code == GIT_EPHP_PROP) {
        php_git2_propagated_exception ex;
        ex.code = code;
        giterr_clear();
        throw ex;
    }

    if (code == GIT_EPHP_PROP_BAILOUT) {
        php_git2_propagated_bailout_exception ex;
        ex.code = code;
        giterr_clear();
        throw ex;
    }

    // Library errors are passed off as normal exceptions that we annotate as
    // "libgit2 error: (code): message".
    php_git2_exception ex("libgit2 error: (%d): %s",err->klass,err->message);
    ex.code = code;
    giterr_clear();
    throw ex;
}

// php_git2::php_git2_giterr_set()

void php_git2::php_git2_giterr_set(int errorClass,const char* format, ...)
{
    char buf[4096];
    va_list args;
    va_start(args,format);
    vsnprintf(buf,sizeof(buf),format,args);
    va_end(args);

    giterr_set_str(errorClass,buf);
}

// Globals functions

void php_git2::php_git2_globals_ctor(zend_git2_globals* gbls)
{
    gbls->propagateError = false;
}

void php_git2::php_git2_globals_dtor(zend_git2_globals* gbls)
{
    UNUSED(gbls);
}

void php_git2::php_git2_globals_init()
{
#ifdef ZTS
    ts_allocate_id(&git2_globals_id,
        sizeof(zend_git2_globals),
        (ts_allocate_ctor)php_git2_globals_ctor,
        (ts_allocate_dtor)php_git2_globals_dtor);
#else
    php_git2_globals_ctor(&git2_globals);
#endif
}

void php_git2::php_git2_globals_request_init()
{
    GIT2_G(propagateError) = false;
    GIT2_G(requestActive) = true;
}

void php_git2::php_git2_globals_request_shutdown()
{
    // NOTE: this function does not touch all globals since the state of some
    // globals MAY be important at this stage.

    // Toggle request active flag. This allows code (typically destructors) to
    // understand their execution context.
    GIT2_G(requestActive) = false;
}

// Helpers

int php_git2::convert_oid_fromstr(git_oid* dest,const char* src,size_t srclen)
{
    // Use a temporary buffer to hold the OID hex string. We make sure it can at
    // most contain a 40 character string. A prefix OID string may have less
    // than 40 characters.

    char buf[GIT_OID_HEXSZ + 1];

    if (srclen > GIT_OID_HEXSZ) {
        srclen = GIT_OID_HEXSZ;
    }

    memset(buf,'0',GIT_OID_HEXSZ);
    buf[GIT_OID_HEXSZ] = 0;
    strncpy(buf,src,srclen);

    return git_oid_fromstr(dest,buf);
}

void php_git2::convert_oid(zval* zv,const git_oid* oid)
{
    char buf[GIT_OID_HEXSZ + 1];

    git_oid_tostr(buf,sizeof(buf),oid);
    ZVAL_STRINGL(zv,buf,GIT_OID_HEXSZ);
}

void php_git2::convert_oid_prefix(zval* zv,const git_oid* prefix,size_t len)
{
    char buf[GIT_OID_HEXSZ + 1];

    if (len > GIT_OID_HEXSZ) {
        len = GIT_OID_HEXSZ;
    }

    git_oid_tostr(buf,len,prefix);
    ZVAL_STRING(zv,buf);
}

void php_git2::convert_transfer_progress(zval* zv,const git_transfer_progress* stats)
{
    if (stats == nullptr) {
        ZVAL_NULL(zv);
        return;
    }

    array_init(zv);
    add_assoc_long_ex(zv,"total_objects",sizeof("total_objects")-1,stats->total_objects);
    add_assoc_long_ex(zv,"indexed_objects",sizeof("indexed_objects")-1,stats->indexed_objects);
    add_assoc_long_ex(zv,"received_objects",sizeof("received_objects")-1,stats->received_objects);
    add_assoc_long_ex(zv,"local_objects",sizeof("local_objects")-1,stats->local_objects);
    add_assoc_long_ex(zv,"total_deltas",sizeof("total_deltas")-1,stats->total_deltas);
    add_assoc_long_ex(zv,"indexed_deltas",sizeof("indexed_deltas")-1,stats->indexed_deltas);
    add_assoc_long_ex(zv,"received_bytes",sizeof("received_bytes")-1,stats->received_bytes);
}

void php_git2::convert_blame_hunk(zval* zv,const git_blame_hunk* hunk)
{
    char buf[GIT_OID_HEXSZ + 1];

    if (hunk == nullptr) {
        ZVAL_NULL(zv);
        return;
    }

    // Convert the git_blame_hunk into a single-dimensional PHP array
    // that contains only primative types (i.e. no objects, resources or
    // arrays). The assumption is here that userspace is more interested
    // in reading the data then having the values in the "proper" format
    // (e.g. git_signature resource type, ETC.).

    array_init(zv);
    add_assoc_long(zv,"lines_in_hunk",hunk->lines_in_hunk);
    git_oid_tostr(buf,sizeof(buf),&hunk->final_commit_id);
    add_assoc_string(zv,"final_commit_id",buf);
    add_assoc_long(zv,"final_start_line_number",hunk->final_start_line_number);
    add_assoc_string(zv,"final_signature.name",hunk->final_signature->name);
    add_assoc_string(zv,"final_signature.email",hunk->final_signature->email);
    add_assoc_long(zv,"final_signature.when.time",hunk->final_signature->when.time);
    add_assoc_long(zv,"final_signature.when.offset",hunk->final_signature->when.offset);
    git_oid_tostr(buf,sizeof(buf),&hunk->orig_commit_id);
    add_assoc_string(zv,"orig_commit_id",buf);
    add_assoc_string(zv,"orig_path",const_cast<char*>(hunk->orig_path));
    add_assoc_long(zv,"orig_start_line_number",hunk->orig_start_line_number);
    add_assoc_string(zv,"orig_signature.name",hunk->orig_signature->name);
    add_assoc_string(zv,"orig_signature.email",hunk->orig_signature->email);
    add_assoc_long(zv,"orig_signature.when.time",hunk->orig_signature->when.time);
    add_assoc_long(zv,"orig_signature.when.offset",hunk->orig_signature->when.offset);
    buf[0] = hunk->boundary;
    buf[1] = 0;
    add_assoc_string(zv,"boundary",buf);
}

void php_git2::convert_diff_delta(zval* zv,const git_diff_delta* delta)
{
    zval zfile;

    if (delta == nullptr) {
        ZVAL_NULL(zv);
        return;
    }

    array_init(zv);
    add_assoc_long_ex(zv,"status",sizeof("status")-1,delta->status);
    add_assoc_long_ex(zv,"flags",sizeof("flags")-1,delta->flags);
    add_assoc_long_ex(zv,"similarity",sizeof("similarity")-1,delta->similarity);
    add_assoc_long_ex(zv,"nfiles",sizeof("nfiles")-1,delta->nfiles);

    convert_diff_file(&zfile,&delta->old_file);
    add_assoc_zval_ex(zv,"old_file",sizeof("old_file")-1,&zfile);

    convert_diff_file(&zfile,&delta->new_file);
    add_assoc_zval_ex(zv,"new_file",sizeof("new_file")-1,&zfile);
}

void php_git2::convert_diff_file(zval* zv,const git_diff_file* file)
{
    if (file == nullptr) {
        ZVAL_NULL(zv);
        return;
    }

    uint16_t idlen = file->id_abbrev;
    char buf[GIT_OID_HEXSZ + 1];

    if (idlen == 0 || idlen > GIT_OID_HEXSZ) {
        idlen = GIT_OID_HEXSZ;
    }
    git_oid_tostr(buf,sizeof(buf),&file->id);
    buf[idlen] = 0;

    array_init(zv);
    add_assoc_stringl(zv,"id",buf,idlen);
    add_assoc_string(zv,"path",const_cast<char*>(file->path));
    add_assoc_long_ex(zv,"size",sizeof("size")-1,file->size);
    add_assoc_long_ex(zv,"flags",sizeof("flags")-1,file->flags);
    add_assoc_long_ex(zv,"mode",sizeof("mode")-1,file->mode);
}

void php_git2::convert_diff_binary(zval* zv,const git_diff_binary* binary)
{
    zval zold;
    zval znew;

    if (binary == nullptr) {
        ZVAL_NULL(zv);
        return;
    }

    array_init(zv);
    add_assoc_bool_ex(zv,"contains_data",sizeof("contains_data")-1,binary->contains_data);

    convert_diff_binary_file(&zold,&binary->old_file);
    convert_diff_binary_file(&znew,&binary->new_file);
    add_assoc_zval_ex(zv,"old_file",sizeof("old_file")-1,&zold);
    add_assoc_zval_ex(zv,"new_file",sizeof("new_file")-1,&znew);
}

void php_git2::convert_diff_binary_file(zval* zv,const git_diff_binary_file* file)
{
    if (file == nullptr) {
        ZVAL_NULL(zv);
        return;
    }

    array_init(zv);
    add_assoc_long_ex(zv,"type",sizeof("type")-1,file->type);
    if (file->data == nullptr || file->datalen == 0) {
        add_assoc_null_ex(zv,"data",sizeof("data")-1);
    }
    else {
        add_assoc_stringl_ex(zv,
            "data",
            sizeof("data")-1,
            const_cast<char*>(file->data),
            file->datalen);
    }
    add_assoc_long_ex(zv,"inflatedlen",sizeof("inflatedlen")-1,file->inflatedlen);
}

void php_git2::convert_diff_hunk(zval* zv,const git_diff_hunk* hunk)
{
    if (hunk == nullptr) {
        ZVAL_NULL(zv);
        return;
    }

    array_init(zv);
    add_assoc_long_ex(zv,"old_start",sizeof("old_start")-1,hunk->old_start);
    add_assoc_long_ex(zv,"old_lines",sizeof("old_lines")-1,hunk->old_lines);
    add_assoc_long_ex(zv,"new_start",sizeof("new_start")-1,hunk->new_start);
    add_assoc_long_ex(zv,"new_lines",sizeof("new_lines")-1,hunk->new_lines);

    // The header is NUL terminated so we just let PHP copy it over.
    add_assoc_string_ex(zv,"header",sizeof("header")-1,const_cast<char*>(hunk->header));
}

void php_git2::convert_diff_line(zval* zv,const git_diff_line* line)
{
    if (line == nullptr) {
        ZVAL_NULL(zv);
        return;
    }

    array_init(zv);
    add_assoc_long_ex(zv,"origin",sizeof("origin")-1,line->origin);
    add_assoc_long_ex(zv,"old_lineno",sizeof("old_lineno")-1,line->old_lineno);
    add_assoc_long_ex(zv,"new_lineno",sizeof("new_lineno")-1,line->new_lineno);
    add_assoc_long_ex(zv,"num_lines",sizeof("num_lines")-1,line->num_lines);
    add_assoc_long_ex(zv,"content_offset",sizeof("content_offset")-1,line->content_offset);
    add_assoc_stringl_ex(zv,
        "content",
        sizeof("content")-1,
        const_cast<char*>(line->content),
        line->content_len);
}

void php_git2::convert_diff_perfdata(zval* zv,const git_diff_perfdata* perfdata)
{
    array_init(zv);
    add_assoc_long_ex(zv,"stat_calls",sizeof("stat_calls")-1,perfdata->stat_calls);
    add_assoc_long_ex(zv,"oid_calculations",sizeof("oid_calculations")-1,perfdata->oid_calculations);
}

void php_git2::convert_signature(zval* zv,const git_signature* sig)
{
    array_init(zv);
    add_assoc_string(zv,"name",sig->name);
    add_assoc_string(zv,"email",sig->email);
    add_assoc_long(zv,"when.time",sig->when.time);
    add_assoc_long(zv,"when.offset",sig->when.offset);
}

void php_git2::convert_index_entry(zval* zv,const git_index_entry* ent)
{
    zval zctime;
    zval zmtime;
    char buf[GIT_OID_HEXSZ + 1];

    array_init(zv);
    convert_index_time(&zctime,&ent->ctime);
    convert_index_time(&zmtime,&ent->mtime);
    add_assoc_zval(zv,"ctime",&zctime);
    add_assoc_zval(zv,"mtime",&zmtime);
    add_assoc_long_ex(zv,"dev",sizeof("dev")-1,ent->dev);
    add_assoc_long_ex(zv,"ino",sizeof("ino")-1,ent->ino);
    add_assoc_long_ex(zv,"mode",sizeof("mode")-1,ent->mode);
    add_assoc_long_ex(zv,"uid",sizeof("uid")-1,ent->uid);
    add_assoc_long_ex(zv,"gid",sizeof("gid")-1,ent->gid);
    add_assoc_long_ex(zv,"file_size",sizeof("file_size")-1,ent->file_size);
    git_oid_tostr(buf,sizeof(buf),&ent->id);
    add_assoc_string_ex(zv,"id",sizeof("id")-1,buf);
    add_assoc_long_ex(zv,"flags",sizeof("flags")-1,ent->flags);
    add_assoc_long_ex(zv,"flags_extended",sizeof("flags_extended")-1,ent->flags_extended);
    if (ent->path != nullptr) {
        add_assoc_string_ex(zv,"path",sizeof("path")-1,const_cast<char*>(ent->path));
    }
    else {
        add_assoc_null_ex(zv,"path",sizeof("path")-1);
    }
}

void php_git2::convert_index_time(zval* zv,const git_index_time* tv)
{
    array_init(zv);
    add_assoc_long(zv,"seconds",tv->seconds);
    add_assoc_long(zv,"nanoseconds",tv->nanoseconds);
}

void php_git2::convert_status_entry(zval* zv,const git_status_entry* ent)
{
    array_init(zv);
    add_assoc_long(zv,"status",ent->status);

    if (ent->head_to_index) {
        zval zheadToIndex;

        convert_diff_delta(&zheadToIndex,ent->head_to_index);
        add_assoc_zval_ex(zv,"head_to_index",sizeof("head_to_index")-1,&zheadToIndex);
    }
    if (ent->index_to_workdir) {
        zval zindexToWorkdir;

        convert_diff_delta(&zindexToWorkdir,ent->index_to_workdir);
        add_assoc_zval_ex(zv,
            "index_to_workdir",
            sizeof("index_to_workdir")-1,
            &zindexToWorkdir);
    }
}

void php_git2::convert_merge_file_result(zval* zv,const git_merge_file_result* res)
{
    array_init(zv);
    add_assoc_bool(zv,"automergeable",res->automergeable);
    if (res->path == nullptr) {
        add_assoc_null(zv,"path");
    }
    else {
        add_assoc_string(zv,"path",const_cast<char*>(res->path));
    }
    add_assoc_long(zv,"mode",res->mode);
    add_assoc_stringl(zv,"ptr",const_cast<char*>(res->ptr),res->len);
}

void php_git2::convert_reflog_entry(zval* zv,const git_reflog_entry* ent)
{
    const char* msg;
    zval znewid;
    zval zoldid;
    zval zsig;

    array_init(zv);

    convert_signature(&zsig,git_reflog_entry_committer(ent));
    add_assoc_zval_ex(zv,"committer",sizeof("committer")-1,&zsig);

    convert_oid(&znewid,git_reflog_entry_id_new(ent));
    add_assoc_zval_ex(zv,"id_new",sizeof("id_new")-1,&znewid);

    convert_oid(&zoldid,git_reflog_entry_id_old(ent));
    add_assoc_zval_ex(zv,"id_old",sizeof("id_old")-1,&zoldid);

    msg = git_reflog_entry_message(ent);
    if (msg == nullptr) {
        add_assoc_null(zv,"message");
    }
    else {
        add_assoc_string(zv,"message",const_cast<char*>(msg));
    }
}

void php_git2::convert_reflog(zval* zv,const git_reflog* log)
{
    size_t count;

    array_init(zv);

    // NOTE: For some reason, libgit2 has git_reflog_entrycount take a non-const
    // reflog pointer. According to the source code, the function is read-only,
    // so a const-cast is acceptable here to get around the inconvenience of the
    // function's signature.
    count = git_reflog_entrycount(const_cast<git_reflog*>(log));
    for (size_t i = 0;i < count;++i) {
        zval zentry;
        const git_reflog_entry* ent = git_reflog_entry_byindex(log,i);

        convert_reflog_entry(&zentry,ent);
        add_next_index_zval(zv,&zentry);
    }
}

void php_git2::convert_rebase_operation(zval* zv,const git_rebase_operation* oper)
{
    zval zid;

    array_init(zv);

    add_assoc_long(zv,"type",oper->type);

    convert_oid(&zid,&oper->id);
    add_assoc_zval_ex(zv,"id",sizeof("id")-1,&zid);

    if (oper->exec != nullptr) {
        add_assoc_string(zv,"exec",const_cast<char*>(oper->exec));
    }
    else {
        add_assoc_null(zv,"exec");
    }
}

void php_git2::convert_cert(zval* zv,const git_cert* cert)
{
    array_init(zv);

    add_assoc_long(zv,"cert_type",cert->cert_type);
}

void php_git2::convert_push_update(zval* zv,const git_push_update* up)
{
    zval zoidsrc;
    zval zoiddst;

    array_init(zv);

    add_assoc_string(zv,"src_refname",up->src_refname);
    add_assoc_string(zv,"dst_refname",up->dst_refname);

    convert_oid(&zoidsrc,&up->src);
    convert_oid(&zoiddst,&up->dst);

    add_assoc_zval(zv,"src",&zoidsrc);
    add_assoc_zval(zv,"dst",&zoiddst);
}

void php_git2::convert_remote_head(zval* zv,const git_remote_head* head)
{
    zval zoid;
    zval zloid;

    array_init(zv);

    add_assoc_bool(zv,"local",head->local);

    convert_oid(&zoid,&head->oid);
    convert_oid(&zloid,&head->loid);

    add_assoc_zval(zv,"oid",&zoid);
    add_assoc_zval(zv,"loid",&zloid);

    add_assoc_string(zv,"name",head->name);
    if (head->symref_target != nullptr) {
        add_assoc_string(zv,"symref_target",head->symref_target);
    }
    else {
        add_assoc_null(zv,"symref_target");
    }
}

git_signature* php_git2::convert_signature(zval* zv)
{
    // NOTE: This function returns nullptr if the PHP array was not formatted
    // correctly.

    const char* name;
    const char* email;
    git_time_t time;
    int offset;
    git_signature* sig;

    if (Z_TYPE_P(zv) != IS_ARRAY) {
        return nullptr;
    }

    array_wrapper arr(zv);

    if (!arr.query("name",sizeof("name")-1)) {
        return nullptr;
    }
    name = arr.get_string();
    if (!arr.query("email",sizeof("email")-1)) {
        return nullptr;
    }
    email = arr.get_string();
    if (!arr.query("when.time",sizeof("when.time")-1)) {
        return nullptr;
    }
    time = arr.get_long();
    if (!arr.query("when.offset",sizeof("when.offset")-1)) {
        return nullptr;
    }
    offset = arr.get_long();

    if (git_signature_new(&sig,name,email,time,offset) == -1) {
        return nullptr;
    }

    return sig;
}

int php_git2::convert_transfer_progress(git_transfer_progress& stats,zval* zv)
{
    bool failure = false;
    array_wrapper arr(zv);

    if (arr.query("total_objects",sizeof("total_objects")-1)) {
        stats.total_objects = static_cast<int>(arr.get_long());
    }
    else {
        failure = true;
    }

    if (arr.query("indexed_objects",sizeof("indexed_objects")-1)) {
        stats.indexed_objects = static_cast<int>(arr.get_long());
    }
    else {
        failure = true;
    }

    if (arr.query("received_objects",sizeof("received_objects")-1)) {
        stats.received_objects = static_cast<int>(arr.get_long());
    }
    else {
        failure = true;
    }

    if (arr.query("local_objects",sizeof("local_objects")-1)) {
        stats.local_objects = static_cast<int>(arr.get_long());
    }
    else {
        failure = true;
    }

    if (arr.query("total_deltas",sizeof("total_deltas")-1)) {
        stats.total_deltas = static_cast<int>(arr.get_long());
    }
    else {
        failure = true;
    }

    if (arr.query("indexed_deltas",sizeof("indexed_deltas")-1)) {
        stats.indexed_deltas = static_cast<int>(arr.get_long());
    }
    else {
        failure = true;
    }

    if (arr.query("received_bytes",sizeof("received_bytes")-1)) {
        stats.received_bytes = static_cast<int>(arr.get_long());
    }
    else {
        failure = true;
    }

    if (failure) {
        php_git2::php_git2_giterr_set(GITERR_INVALID,"Transfer progress array is malformed");
        return GIT_ERROR;
    }

    return GIT_OK;
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
