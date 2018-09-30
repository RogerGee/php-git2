/*
 * php-git2.cpp
 *
 * This file is a part of php-git2.
 */

#include "php-git2.h"
#include "php-git2-fe.h"
#include <cstdio>
#include <cstdarg>
using namespace std;
using namespace php_git2;

ZEND_DECLARE_MODULE_GLOBALS(git2)

// Extension setup functions defined in this unit.
static PHP_GINIT_FUNCTION(git2);
static PHP_GSHUTDOWN_FUNCTION(git2);
static PHP_MINIT_FUNCTION(git2);
static PHP_RINIT_FUNCTION(git2);
static PHP_MINFO_FUNCTION(git2);
static PHP_MSHUTDOWN_FUNCTION(git2);
static PHP_RSHUTDOWN_FUNCTION(git2);

// Exported extension functions defined in this unit.
PHP_FUNCTION(git_libgit2_version);
PHP_FUNCTION(git2_version);

// Functions exported by this extension into PHP.
static zend_function_entry php_git2_functions[] = {
    // Functions that do not directly wrap libgit2 exports:
    PHP_FE(git2_version,NULL)

    // General libgit2 functions:
    PHP_FE(git_libgit2_version,NULL)
    PHP_GIT2_FE(git_libgit2_features,
        (zif_php_git2_function<
            func_wrapper<int>::func<git_libgit2_features>,
            local_pack<>,
            0 >),
        NULL)

    // Include template specializations for the different library wrappers. The
    // compiler will instantiate these into this compilation unit.
    GIT_REPOSITORY_FE
    GIT_REFERENCE_FE
    GIT_OBJECT_FE
    GIT_REVWALK_FE
    GIT_PACKBUILDER_FE
    GIT_INDEXER_FE
    GIT_ODB_FE
    GIT_COMMIT_FE
    GIT_BLOB_FE
    GIT_TREE_FE
    GIT_SIGNATURE_FE
    GIT_TREEBUILDER_FE
    GIT_BLAME_FE
    GIT_REVPARSE_FE
    GIT_ANNOTATED_FE
    GIT_BRANCH_FE
    GIT_CONFIG_FE
    GIT_CLONE_FE
    GIT_CHECKOUT_FE
    GIT_TAG_FE
    GIT_DIFF_FE
    GIT_INDEX_FE
    GIT_TRACE_FE
    GIT_IGNORE_FE
    GIT_ATTR_FE
    GIT_STATUS_FE
    GIT_CHERRYPICK_FE
    GIT_MERGE_FE
    GIT_NOTE_FE
    GIT_REFLOG_FE
    PHP_FE_END
};

// Module entry table.
zend_module_entry git2_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_GIT2_EXTNAME,
    php_git2_functions,
    PHP_MINIT(git2),
    PHP_MSHUTDOWN(git2),
    PHP_RINIT(git2),
    PHP_RSHUTDOWN(git2),
    PHP_MINFO(git2),
#if ZEND_MODULE_API_NO >= 20010901
    PHP_GIT2_EXTVER,
#endif
    PHP_MODULE_GLOBALS(git2),
    PHP_GINIT(git2),
    PHP_GSHUTDOWN(git2),
    NULL,
    STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_GIT2
ZEND_GET_MODULE(git2)
#endif

// Implementation of internal functions.

PHP_GINIT_FUNCTION(git2)
{
}

PHP_GSHUTDOWN_FUNCTION(git2)
{
}

PHP_MINIT_FUNCTION(git2)
{
    // Initialize git2 library.
    git_libgit2_init();

    // Call the function to register all resource types. Whenever a resource
    // type is added, the libgit2 data type name should be added to the list of
    // template parameters.
    php_git2_define_resource_types<
        git_repository,
        git_reference,
        git_object,
        git_revwalk,
        git_packbuilder,
        git_indexer,
        git_odb,
        git_odb_object,
        git_commit,
        git_blob,
        git_tree,
        git_tree_entry,
        git_signature,
        git_treebuilder,
        git_blame,
        git_annotated_commit,
        git_branch_iterator,
        git_reference_iterator,
        git_config,
        git_config_iterator,
        git_tag,
        git_diff,
        git_diff_stats,
        git_index,
        git_index_conflict_iterator,
        git_status_list,
        git_note,
        git_note_iterator,
        git_reflog >(module_number);

    // Register all classes provided by this extension.
    php_git2_register_classes(TSRMLS_C);

    // Register all libgit2 constants.
    php_git2_register_constants(module_number TSRMLS_CC);

    return SUCCESS;
}

PHP_RINIT_FUNCTION(git2)
{
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

PHP_MSHUTDOWN_FUNCTION(git2)
{
    // Deinitialize libgit2. At this point, all resources should have been
    // freed. This means they would call their destructors and all libgit2
    // memory should be freed.
    git_libgit2_shutdown();

    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(git2)
{
    return SUCCESS;
}

PHP_FUNCTION(git_libgit2_version)
{
    char buf[128];
    int major, minor, rev;

    git_libgit2_version(&major,&minor,&rev);
    snprintf(buf,sizeof(buf),"%d.%d.%d",major,minor,rev);

    RETURN_STRING(buf,1);
}

PHP_FUNCTION(git2_version)
{
    char buf[128];
    int major, minor, rev;

    git_libgit2_version(&major,&minor,&rev);
    snprintf(buf,sizeof(buf),"%s %s (libgit2 %d.%d.%d)",PHP_GIT2_EXTNAME,PHP_GIT2_EXTVER,
        major,minor,rev);

    RETURN_STRING(buf,1);
}

// php_git2_exception

php_git2_exception::php_git2_exception(const char* format, ...)
    : message(4096,0)
{
    va_list args;
    va_start(args,format);
    vsnprintf(&message[0],message.size(),format,args);
    va_end(args);
}

// php_git2::git_error<int>()

template<>
void php_git2::git_error(int code)
{
    const ::git_error* err = giterr_last();
    if (err == nullptr) {
        throw php_git2_exception("libgit2 no error?");
    }

    php_git2_exception ex("libgit2 error: (%d): %s",err->klass,err->message);
    ex.code = code;
    throw ex;
}

// Helpers

int php_git2::convert_oid_fromstr(git_oid* dest,const char* src,int srclen)
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
    ZVAL_STRINGL(zv,buf,GIT_OID_HEXSZ,1);
}

void php_git2::convert_oid_prefix(zval* zv,const git_oid* prefix,size_t len)
{
    char buf[GIT_OID_HEXSZ + 1];

    if (len > GIT_OID_HEXSZ) {
        len = GIT_OID_HEXSZ;
    }

    git_oid_tostr(buf,len,prefix);
    ZVAL_STRING(zv,buf,1);
}

void php_git2::convert_transfer_progress(zval* zv,const git_transfer_progress* stats)
{
    if (stats == nullptr) {
        ZVAL_NULL(zv);
        return;
    }

    array_init(zv);
    add_assoc_long_ex(zv,"total_objects",sizeof("total_objects"),stats->total_objects);
    add_assoc_long_ex(zv,"indexed_objects",sizeof("indexed_objects"),stats->indexed_objects);
    add_assoc_long_ex(zv,"received_objects",sizeof("received_objects"),stats->received_objects);
    add_assoc_long_ex(zv,"local_objects",sizeof("local_objects"),stats->local_objects);
    add_assoc_long_ex(zv,"total_objects",sizeof("total_objects"),stats->total_objects);
    add_assoc_long_ex(zv,"indexed_deltas",sizeof("indexed_deltas"),stats->indexed_deltas);
    add_assoc_long_ex(zv,"received_bytes",sizeof("received_bytes"),stats->received_bytes);
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
    add_assoc_string(zv,"final_commit_id",buf,1);
    add_assoc_long(zv,"final_start_line_number",hunk->final_start_line_number);
    add_assoc_string(zv,"final_signature.name",hunk->final_signature->name,1);
    add_assoc_string(zv,"final_signature.email",hunk->final_signature->email,1);
    add_assoc_long(zv,"final_signature.when.time",hunk->final_signature->when.time);
    add_assoc_long(zv,"final_signature.when.offset",hunk->final_signature->when.offset);
    git_oid_tostr(buf,sizeof(buf),&hunk->orig_commit_id);
    add_assoc_string(zv,"orig_commit_id",buf,1);
    add_assoc_string(zv,"orig_path",const_cast<char*>(hunk->orig_path),1);
    add_assoc_long(zv,"orig_start_line_number",hunk->orig_start_line_number);
    add_assoc_string(zv,"orig_signature.name",hunk->orig_signature->name,1);
    add_assoc_string(zv,"orig_signature.email",hunk->orig_signature->email,1);
    add_assoc_long(zv,"orig_signature.when.time",hunk->orig_signature->when.time);
    add_assoc_long(zv,"orig_signature.when.offset",hunk->orig_signature->when.offset);
    buf[0] = hunk->boundary;
    buf[1] = 0;
    add_assoc_string(zv,"boundary",buf,1);
}

void php_git2::convert_diff_delta(zval* zv,const git_diff_delta* delta)
{
    zval* zfile;

    if (delta == nullptr) {
        ZVAL_NULL(zv);
        return;
    }

    array_init(zv);
    add_assoc_long_ex(zv,"status",sizeof("status"),delta->status);
    add_assoc_long_ex(zv,"flags",sizeof("flags"),delta->flags);
    add_assoc_long_ex(zv,"similarity",sizeof("similarity"),delta->similarity);
    add_assoc_long_ex(zv,"nfiles",sizeof("nfiles"),delta->nfiles);

    MAKE_STD_ZVAL(zfile);
    convert_diff_file(zfile,&delta->old_file);
    add_assoc_zval_ex(zv,"old_file",sizeof("old_file"),zfile);

    MAKE_STD_ZVAL(zfile);
    convert_diff_file(zfile,&delta->new_file);
    add_assoc_zval_ex(zv,"new_file",sizeof("new_file"),zfile);
}

void php_git2::convert_diff_file(zval* zv,const git_diff_file* file)
{
    if (file == nullptr) {
        ZVAL_NULL(zv);
        return;
    }

    uint16_t idlen = file->id_abbrev;
    char buf[GIT_OID_HEXSZ + 1];

    if (idlen > GIT_OID_HEXSZ) {
        idlen = GIT_OID_HEXSZ;
    }

    array_init(zv);
    git_oid_tostr(buf,sizeof(buf),&file->id);
    buf[idlen] = 0;
    add_assoc_stringl(zv,"id",buf,GIT_OID_HEXSZ,1);
    add_assoc_const_string(zv,"path",file->path);
    add_assoc_long_ex(zv,"size",sizeof("size"),file->size);
    add_assoc_long_ex(zv,"flags",sizeof("flags"),file->flags);
    add_assoc_long_ex(zv,"mode",sizeof("mode"),file->mode);
}

void php_git2::convert_diff_binary(zval* zv,const git_diff_binary* binary)
{
    zval* zold;
    zval* znew;

    if (binary == nullptr) {
        ZVAL_NULL(zv);
        return;
    }

    array_init(zv);
    add_assoc_bool_ex(zv,"contains_data",sizeof("contains_data"),binary->contains_data);

    MAKE_STD_ZVAL(zold);
    MAKE_STD_ZVAL(znew);
    convert_diff_binary_file(zold,&binary->old_file);
    convert_diff_binary_file(znew,&binary->new_file);
    add_assoc_zval_ex(zv,"old_file",sizeof("old_file"),zold);
    add_assoc_zval_ex(zv,"new_file",sizeof("new_file"),znew);
}

void php_git2::convert_diff_binary_file(zval* zv,const git_diff_binary_file* file)
{
    if (file == nullptr) {
        ZVAL_NULL(zv);
        return;
    }

    array_init(zv);
    add_assoc_long_ex(zv,"type",sizeof("type"),file->type);
    if (file->data == nullptr || file->datalen == 0) {
        add_assoc_null_ex(zv,"data",sizeof("data"));
    }
    else {
        add_assoc_stringl_ex(zv,"data",sizeof("data"),const_cast<char*>(file->data),file->datalen,1);
    }
    add_assoc_long_ex(zv,"inflatedlen",sizeof("inflatedlen"),file->inflatedlen);
}

void php_git2::convert_diff_hunk(zval* zv,const git_diff_hunk* hunk)
{
    if (hunk == nullptr) {
        ZVAL_NULL(zv);
        return;
    }

    array_init(zv);
    add_assoc_long_ex(zv,"old_start",sizeof("old_start"),hunk->old_start);
    add_assoc_long_ex(zv,"old_lines",sizeof("old_lines"),hunk->old_lines);
    add_assoc_long_ex(zv,"new_start",sizeof("new_start"),hunk->new_start);
    add_assoc_long_ex(zv,"new_lines",sizeof("new_lines"),hunk->new_lines);

    // The header is NUL terminated so we just let PHP copy it over.
    add_assoc_string_ex(zv,"header",sizeof("header"),const_cast<char*>(hunk->header),1);
}

void php_git2::convert_diff_line(zval* zv,const git_diff_line* line)
{
    if (line == nullptr) {
        ZVAL_NULL(zv);
        return;
    }

    array_init(zv);
    add_assoc_long_ex(zv,"origin",sizeof("origin"),line->origin);
    add_assoc_long_ex(zv,"old_lineno",sizeof("old_lineno"),line->old_lineno);
    add_assoc_long_ex(zv,"new_lineno",sizeof("new_lineno"),line->new_lineno);
    add_assoc_long_ex(zv,"num_lines",sizeof("num_lines"),line->num_lines);
    add_assoc_long_ex(zv,"content_offset",sizeof("content_offset"),line->content_offset);
    add_assoc_stringl_ex(zv,"content",sizeof("content"),const_cast<char*>(line->content),line->content_len,1);
}

void php_git2::convert_diff_perfdata(zval* zv,const git_diff_perfdata* perfdata)
{
    array_init(zv);
    add_assoc_long_ex(zv,"stat_calls",sizeof("stat_calls"),perfdata->stat_calls);
    add_assoc_long_ex(zv,"oid_calculations",sizeof("oid_calculations"),perfdata->oid_calculations);
}

void php_git2::convert_signature(zval* zv,const git_signature* sig)
{
    array_init(zv);
    add_assoc_string(zv,"name",sig->name,1);
    add_assoc_string(zv,"email",sig->email,1);
    add_assoc_long(zv,"when.time",sig->when.time);
    add_assoc_long(zv,"when.offset",sig->when.offset);
}

void php_git2::convert_index_entry(zval* zv,const git_index_entry* ent)
{
    zval* zctime;
    zval* zmtime;
    char buf[GIT_OID_HEXSZ + 1];

    MAKE_STD_ZVAL(zctime);
    MAKE_STD_ZVAL(zmtime);

    array_init(zv);
    convert_index_time(zctime,&ent->ctime);
    convert_index_time(zmtime,&ent->mtime);
    add_assoc_long_ex(zv,"dev",sizeof("dev"),ent->dev);
    add_assoc_long_ex(zv,"ino",sizeof("ino"),ent->ino);
    add_assoc_long_ex(zv,"mode",sizeof("mode"),ent->mode);
    add_assoc_long_ex(zv,"uid",sizeof("uid"),ent->uid);
    add_assoc_long_ex(zv,"gid",sizeof("gid"),ent->gid);
    add_assoc_long_ex(zv,"file_size",sizeof("file_size"),ent->file_size);
    git_oid_tostr(buf,sizeof(buf),&ent->id);
    add_assoc_string_ex(zv,"id",sizeof("id"),buf,1);
    add_assoc_long_ex(zv,"flags",sizeof("flags"),ent->flags);
    add_assoc_long_ex(zv,"flags_extended",sizeof("flags_extended"),ent->flags_extended);
    if (ent->path != nullptr) {
        add_assoc_string_ex(zv,"path",sizeof("path"),const_cast<char*>(ent->path),1);
    }
    else {
        add_assoc_null_ex(zv,"path",sizeof("path"));
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
        zval* zheadToIndex;

        MAKE_STD_ZVAL(zheadToIndex);
        convert_diff_delta(zheadToIndex,ent->head_to_index);

        add_assoc_zval_ex(zv,"head_to_index",sizeof("head_to_index"),zheadToIndex);
    }
    if (ent->index_to_workdir) {
        zval* zindexToWorkdir;

        MAKE_STD_ZVAL(zindexToWorkdir);
        convert_diff_delta(zindexToWorkdir,ent->index_to_workdir);

        add_assoc_zval_ex(zv,"index_to_workdir",sizeof("index_to_workdir"),zindexToWorkdir);
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
        add_assoc_string(zv,"path",const_cast<char*>(res->path),1);
    }
    add_assoc_long(zv,"mode",res->mode);
    add_assoc_stringl(zv,"ptr",const_cast<char*>(res->ptr),res->len,1);
}

void php_git2::convert_reflog_entry(zval* zv,const git_reflog_entry* ent)
{
    const char* msg;
    zval* znewid;
    zval* zoldid;
    zval* zsig;

    array_init(zv);
    MAKE_STD_ZVAL(znewid);
    MAKE_STD_ZVAL(zoldid);
    MAKE_STD_ZVAL(zsig);

    convert_signature(zsig,git_reflog_entry_committer(ent));
    add_assoc_zval_ex(zv,"committer",sizeof("committer"),zsig);

    convert_oid(znewid,git_reflog_entry_id_new(ent));
    add_assoc_zval_ex(zv,"id_new",sizeof("id_new"),znewid);

    convert_oid(zoldid,git_reflog_entry_id_old(ent));
    add_assoc_zval_ex(zv,"id_old",sizeof("id_old"),zoldid);

    msg = git_reflog_entry_message(ent);
    if (msg == nullptr) {
        add_assoc_null(zv,"message");
    }
    else {
        add_assoc_string(zv,"message",const_cast<char*>(msg),1);
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
        zval* zentry;
        const git_reflog_entry* ent = git_reflog_entry_byindex(log,i);

        MAKE_STD_ZVAL(zentry);
        convert_reflog_entry(zentry,ent);

        add_next_index_zval(zv,zentry);
    }
}

git_signature* php_git2::convert_signature(zval* zv)
{
    // NOTE: This functions returns nullptr if the PHP array was not formatted
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

    if (!arr.query("name",sizeof("name"))) {
        return nullptr;
    }
    name = arr.get_string();
    if (!arr.query("email",sizeof("email"))) {
        return nullptr;
    }
    email = arr.get_string();
    if (!arr.query("when.time",sizeof("when.time"))) {
        return nullptr;
    }
    time = arr.get_long();
    if (!arr.query("when.offset",sizeof("when.offset"))) {
        return nullptr;
    }
    offset = arr.get_long();

    if (git_signature_new(&sig,name,email,time,offset) == -1) {
        return nullptr;
    }

    return sig;
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
