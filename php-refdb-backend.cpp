/*
 * php-refdb-backend.cpp
 *
 * This file is a part of php-git2.
 */

#include "php-object.h"
#include <cstring>
extern "C" {
#include <git2/sys/refs.h>
#include <git2/sys/reflog.h>
}
using namespace php_git2;

// Helper macros and functions

#define EXTRACT_BACKEND(backend) \
    reinterpret_cast<php_refdb_backend_object::git_refdb_backend_php*>(backend)

#define EXTRACT_THISOBJ(backend) \
    EXTRACT_BACKEND(backend)->thisobj

static int reference_from_string(git_reference** out,zval* zstr,const char* ref_name)
{
    // If the string starts with "ref: " then assume it is a symbolic
    // reference. Otherwise try to decode an OID hex string.

    const char* target = Z_STRVAL_P(zstr);

    if (strstr(target,"ref: ") == target) {
        *out = git_reference__alloc_symbolic(ref_name,target);
    }
    else if (strlen(target) == GIT_OID_HEXSZ) {
        git_oid oid;
        if (convert_oid_fromstr(&oid,target,GIT_OID_HEXSZ) != 0) {
            php_error(E_ERROR,"GitRefDBBackend::lookup(): invalid OID return value");
            return GIT_ERROR;
        }

        *out = git_reference__alloc(ref_name,&oid,NULL);
    }
    else {
        php_error(E_ERROR,"GitRefDBBackend::lookup(): invalid return value");
        return GIT_ERROR;
    }

    return GIT_OK;
}

static int reflog_entry_from_array(git_reflog_entry** out,zval* zarr)
{
    const char* oid_old;
    int oid_old_len;
    const char* oid_cur;
    int oid_cur_len;
    git_signature* committer;
    const char* msg;

    git_reflog_entry* ent;

    if (Z_TYPE_P(zarr) != IS_ARRAY) {
        giterr_set_str(GIT_EINVALID,"Reflog entry must be an array");
        return GIT_ERROR;
    }

    array_wrapper arr(zarr);

    if (!arr.query("oid_old",sizeof("oid_old"))) {
        giterr_set_str(GIT_EINVALID,"Reflog entry must have 'oid_old' member");
        return GIT_ERROR;
    }
    oid_old = arr.get_string();
    oid_old_len = arr.get_string_length();

    if (!arr.query("oid_cur",sizeof("oid_cur"))) {
        giterr_set_str(GIT_EINVALID,"Reflog entry must have 'oid_cur' member");
        return GIT_ERROR;
    }
    oid_cur = arr.get_string();
    oid_cur_len = arr.get_string_length();

    if (!arr.query("committer",sizeof("committer"))) {
        giterr_set_str(GIT_EINVALID,"Reflog entry must have 'committer' member");
        return GIT_ERROR;
    }
    committer = convert_signature(arr.get_zval());
    if (committer == nullptr) {
        giterr_set_str(GIT_EINVALID,"Failed to extract 'committer' from reflog entry");
        return GIT_ERROR;
    }

    if (!arr.query("msg",sizeof("msg"))) {
        giterr_set_str(GIT_EINVALID,"Reflog entry must have 'msg' member");
        return GIT_ERROR;
    }
    msg = arr.get_string();

    ent = git_reflog_entry__alloc();
    if (ent == nullptr) {
        return GIT_ERROR;
    }

    // TODO Uh, how do I set the fields on the opaque struct?

    (void)oid_old;
    (void)oid_old_len;
    (void)oid_cur;
    (void)oid_cur_len;
    (void)committer;
    (void)msg;

    *out = ent;

    return GIT_OK;
}

struct custom_backend_iterator : git_reference_iterator
{
    // Store reference to GitRefDBBackend object.
    zval* zobject;

    // Store last returned name string.
    zval zlast;
};

static int custom_backend_iterator_next(git_reference** ref,custom_backend_iterator* iter)
{
    const int N = 1;
    int result = GIT_OK;

    zval fname;
    zval retval;
    zval* params[N];
    zval* zname;

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zname);

    ZVAL_STRING(&fname,"iterator_next",1);

    params[0] = zname;

    // Call userspace method implementation corresponding to refdb operation.
    if (call_user_function(NULL,&iter->zobject,&fname,&retval,N,params
            ZTS_MEMBER_CC(wrapper.object()->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }
    else {
        zval_dtor(&iter->zlast);
        ZVAL_COPY_VALUE(&iter->zlast,zname);
        zval_copy_ctor(&iter->zlast);
        convert_to_string(&iter->zlast);

        // Return values.
        convert_to_string(&retval);
        result = reference_from_string(ref,&retval,Z_STRVAL(iter->zlast));
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zname);

    return result;
}

static int custom_backend_iterator_next_name(const char** ref_name,custom_backend_iterator* iter)
{
    const int N = 1;
    int result = GIT_OK;

    zval fname;
    zval retval;
    zval* params[N];
    zval* zname;

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zname);

    ZVAL_STRING(&fname,"iterator_next",1);

    params[0] = zname;

    // Call userspace method implementation corresponding to refdb operation.
    if (call_user_function(NULL,&iter->zobject,&fname,&retval,N,params
            ZTS_MEMBER_CC(wrapper.object()->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }
    else {
        zval_dtor(&iter->zlast);
        ZVAL_COPY_VALUE(&iter->zlast,zname);
        zval_copy_ctor(&iter->zlast);
        convert_to_string(&iter->zlast);
        *ref_name = Z_STRVAL(iter->zlast);
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zname);

    return result;
}

static void custom_backend_iterator_free(custom_backend_iterator* iter)
{
    zval_ptr_dtor(&iter->zobject);
    zval_dtor(&iter->zlast);
    efree(iter);
}

// Custom class handlers

static zval* refdb_backend_read_property(zval* obj,zval* prop,int type,const zend_literal* key TSRMLS_DC);
static void refdb_backend_write_property(zval* obj,zval* prop,zval* value,const zend_literal* key TSRMLS_DC);
static int refdb_backend_has_property(zval* obj,zval* prop,int chk_type,const zend_literal* key TSRMLS_DC);

// Class entries

// The following functions are optional so this base class provides an empty
// implementation.
static PHP_EMPTY_METHOD(GitRefDBBackend,iterator_new);
static PHP_EMPTY_METHOD(GitRefDBBackend,iterator_next);
static PHP_EMPTY_METHOD(GitRefDBBackend,compress);
static PHP_EMPTY_METHOD(GitRefDBBackend,lock);
static PHP_EMPTY_METHOD(GitRefDBBackend,unlock);

zend_function_entry php_git2::refdb_backend_methods[] = {
    PHP_ABSTRACT_ME(GitRefDBBackend,exists,NULL)
    PHP_ABSTRACT_ME(GitRefDBBackend,lookup,NULL)
    PHP_ME(GitRefDBBackend,iterator_new,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitRefDBBackend,iterator_next,NULL,ZEND_ACC_PUBLIC)
    PHP_ABSTRACT_ME(GitRefDBBackend,write,NULL)
    PHP_ABSTRACT_ME(GitRefDBBackend,rename,NULL)
    PHP_ABSTRACT_ME(GitRefDBBackend,del,NULL)
    PHP_ME(GitRefDBBackend,compress,NULL,ZEND_ACC_PUBLIC)
    PHP_ABSTRACT_ME(GitRefDBBackend,has_log,NULL)
    PHP_ABSTRACT_ME(GitRefDBBackend,ensure_log,NULL)
    PHP_ABSTRACT_ME(GitRefDBBackend,reflog_read,NULL)
    PHP_ABSTRACT_ME(GitRefDBBackend,reflog_write,NULL)
    PHP_ABSTRACT_ME(GitRefDBBackend,reflog_rename,NULL)
    PHP_ABSTRACT_ME(GitRefDBBackend,reflog_delete,NULL)
    PHP_ME(GitRefDBBackend,lock,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitRefDBBackend,unlock,NULL,ZEND_ACC_PUBLIC)
    PHP_FE_END
};

// Implementation of custom class handlers

zval* refdb_backend_read_property(zval* obj,zval* prop,int type,const zend_literal* key TSRMLS_DC)
{
    return (*std_object_handlers.read_property)(obj,prop,type,key TSRMLS_CC);
}

void refdb_backend_write_property(zval* obj,zval* prop,zval* value,const zend_literal* key TSRMLS_DC)
{
    (*std_object_handlers.write_property)(obj,prop,value,key TSRMLS_CC);
}

int refdb_backend_has_property(zval* obj,zval* prop,int chk_type,const zend_literal* key TSRMLS_DC)
{
    return (*std_object_handlers.has_property)(obj,prop,chk_type,key TSRMLS_CC);
}

// Implementation of php_refdb_backend_object

php_refdb_backend_object::php_refdb_backend_object(zend_class_entry* ce TSRMLS_DC):
    backend(nullptr), zts(TSRMLS_C)
{
    zend_object_std_init(this,ce TSRMLS_CC);
    object_properties_init(this,ce);
}

php_refdb_backend_object::~php_refdb_backend_object()
{
    zend_object_std_dtor(this ZTS_MEMBER_CC(this->zts));
}

void php_refdb_backend_object::create_custom_backend(zval* zobj)
{
    // NOTE: 'zobj' is the zval to the outer object that wraps this custom
    // zend_object derivation.

    // Make sure object doesn't already have a backing. This would imply it is
    // in use already in an refdb.
    if (backend != nullptr) {
        php_error(E_ERROR,"Cannot create custom refdb backend: object already in use");
    }

    // Create custom backend. Custom backends are always passed off to git2, so
    // we are not responsible for calling its free function.
    backend = new (emalloc(sizeof(git_refdb_backend_php))) git_refdb_backend_php(zobj);
}

/*static*/ zend_object_handlers php_refdb_backend_object::handlers;
/*static*/ void php_refdb_backend_object::init(zend_class_entry* ce)
{
    handlers.read_property = refdb_backend_read_property;
    handlers.write_property = refdb_backend_write_property;
    handlers.has_property = refdb_backend_has_property;
    (void)ce;
}

/*static*/ int php_refdb_backend_object::exists(
    int *exists,
    git_refdb_backend *backend,
    const char *ref_name)
{
    const int N = 1;
    int result = GIT_OK;

    zval fname;
    zval retval;
    zval* thisobj = EXTRACT_THISOBJ(backend);
    zval* params[N];
    zval* zrefname;

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zrefname);

    ZVAL_STRING(&fname,"exists",1);
    ZVAL_STRING(zrefname,ref_name,1);

    params[0] = zrefname;

    // Call userspace method implementation corresponding to refdb operation.
    if (call_user_function(NULL,&thisobj,&fname,&retval,N,params
            ZTS_MEMBER_CC(wrapper.object()->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }
    else {
        // Return values.
        convert_to_boolean(&retval);
        *exists = Z_BVAL(retval);
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zrefname);

    return result;
}

/*static*/ int php_refdb_backend_object::lookup(
    git_reference **out,
    git_refdb_backend *backend,
    const char *ref_name)
{
    const int N = 1;
    int result = GIT_OK;

    zval fname;
    zval retval;
    zval* thisobj = EXTRACT_THISOBJ(backend);
    zval* params[N];
    zval* zrefname;

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zrefname);

    ZVAL_STRING(&fname,"lookup",1);
    ZVAL_STRING(zrefname,ref_name,1);

    // Call userspace method implementation corresponding to refdb operation.
    if (call_user_function(NULL,&thisobj,&fname,&retval,N,params
            ZTS_MEMBER_CC(wrapper.object()->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }
    else {
        // Return values.
        convert_to_string(&retval);
        result = reference_from_string(out,&retval,ref_name);
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zrefname);

    return result;
}

/*static*/ int php_refdb_backend_object::iterator(
    git_reference_iterator **iter,
    struct git_refdb_backend *backend,
    const char *glob)
{
    const int N = 1;
    int result = GIT_OK;

    zval fname;
    zval retval;
    zval* thisobj = EXTRACT_THISOBJ(backend);
    zval* params[N];
    zval* zglob;

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zglob);

    ZVAL_STRING(&fname,"iterator_new",1);
    if (glob != nullptr) {
        ZVAL_STRING(zglob,glob,1);
    }

    params[0] = zglob;

    // Call userspace method implementation corresponding to refdb operation.
    if (call_user_function(NULL,&thisobj,&fname,&retval,N,params
            ZTS_MEMBER_CC(wrapper.object()->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }
    else {
        // Create and return custom git_reference_iterator.

        custom_backend_iterator* custom;

        custom = reinterpret_cast<custom_backend_iterator*>(emalloc(sizeof(custom_backend_iterator)));
        memset(custom,0,sizeof(custom_backend_iterator));
        custom->next = (int(*)(git_reference**,git_reference_iterator*))custom_backend_iterator_next;
        custom->next_name = (int(*)(const char**,git_reference_iterator*))custom_backend_iterator_next_name;
        custom->free = (void(*)(git_reference_iterator*))custom_backend_iterator_free;
        Z_ADDREF_P(thisobj);
        custom->zobject = thisobj;
        INIT_ZVAL(custom->zlast);

        *iter = custom;
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zglob);

    return result;
}

/*static*/ int php_refdb_backend_object::write(
    git_refdb_backend *backend,
    const git_reference *ref,
    int force,
    const git_signature *who,
    const char *message,
    const git_oid *old,
    const char *old_target)
{
    const int N = 6;
    int result = GIT_OK;

    zval fname;
    zval retval;
    zval* thisobj = EXTRACT_THISOBJ(backend);
    zval* params[N];
    zval* zref;
    zval* zforce;
    zval* zwho;
    zval* zmessage;
    zval* zold;
    zval* zoldtarget;
    const php_resource_ref<php_git_reference> res;

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zref);
    ALLOC_INIT_ZVAL(zforce);
    ALLOC_INIT_ZVAL(zwho);
    ALLOC_INIT_ZVAL(zmessage);
    ALLOC_INIT_ZVAL(zold);
    ALLOC_INIT_ZVAL(zoldtarget);

    ZVAL_STRING(&fname,"write",1);
    *res.byval_git2() = ref;
    res.ret(zref);
    ZVAL_BOOL(zforce,force);
    convert_signature(zwho,who);
    if (message != NULL) {
        ZVAL_STRING(zmessage,message,1);
    }
    convert_oid(zold,old);
    ZVAL_STRING(zoldtarget,old_target,1);

    params[0] = zref;
    params[1] = zforce;
    params[2] = zwho;
    params[3] = zmessage;
    params[4] = zold;
    params[5] = zoldtarget;

    // Call userspace method implementation corresponding to refdb operation.
    if (call_user_function(NULL,&thisobj,&fname,&retval,N,params
            ZTS_MEMBER_CC(wrapper.object()->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }
    else {
        // TODO Check for exception.
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zref);
    zval_ptr_dtor(&zforce);
    zval_ptr_dtor(&zwho);
    zval_ptr_dtor(&zmessage);
    zval_ptr_dtor(&zold);
    zval_ptr_dtor(&zoldtarget);

    return result;
}

/*static*/ int php_refdb_backend_object::rename(
    git_reference **out,
    git_refdb_backend *backend,
    const char *old_name,
    const char *new_name,
    int force,
    const git_signature *who,
    const char *message)
{
    const int N = 5;
    int result = GIT_OK;

    zval fname;
    zval retval;
    zval* thisobj = EXTRACT_THISOBJ(backend);
    zval* params[N];
    zval* zoldname;
    zval* znewname;
    zval* zforce;
    zval* zwho;
    zval* zmessage;

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zoldname);
    ALLOC_INIT_ZVAL(znewname);
    ALLOC_INIT_ZVAL(zforce);
    ALLOC_INIT_ZVAL(zwho);
    ALLOC_INIT_ZVAL(zmessage);

    ZVAL_STRING(&fname,"rename",1);
    ZVAL_STRING(zoldname,old_name,1);
    ZVAL_STRING(znewname,new_name,1);
    ZVAL_BOOL(zforce,1);
    convert_signature(zwho,who);
    ZVAL_STRING(zmessage,message,1);

    params[0] = zoldname;
    params[1] = znewname;
    params[2] = zforce;
    params[3] = zwho;
    params[4] = zmessage;

    // Call userspace method implementation corresponding to refdb operation.
    if (call_user_function(NULL,&thisobj,&fname,&retval,N,params
            ZTS_MEMBER_CC(wrapper.object()->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }
    else {
        // Return values.
        convert_to_string(&retval);
        result = reference_from_string(out,&retval,new_name);
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zoldname);
    zval_ptr_dtor(&znewname);
    zval_ptr_dtor(&zforce);
    zval_ptr_dtor(&zwho);
    zval_ptr_dtor(&zmessage);

    return result;
}

/*static*/ int php_refdb_backend_object::del(
    git_refdb_backend *backend,
    const char *ref_name,
    const git_oid *old_id,
    const char *old_target)
{
    const int N = 3;
    int result = GIT_OK;

    zval fname;
    zval retval;
    zval* thisobj = EXTRACT_THISOBJ(backend);
    zval* params[N];
    zval* zrefname;
    zval* zoldid;
    zval* zoldtarget;

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zrefname);
    ALLOC_INIT_ZVAL(zoldid);
    ALLOC_INIT_ZVAL(zoldtarget);

    ZVAL_STRING(&fname,"del",1);
    ZVAL_STRING(zrefname,ref_name,1);
    convert_oid(zoldid,old_id);
    ZVAL_STRING(zoldtarget,old_target,1);

    params[0] = zrefname;
    params[1] = zoldid;
    params[2] = zoldtarget;

    // Call userspace method implementation corresponding to refdb operation.
    if (call_user_function(NULL,&thisobj,&fname,&retval,N,params
            ZTS_MEMBER_CC(wrapper.object()->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }
    else {
        // Interpret boolean return value. Assume reference not found on
        // failure.
        convert_to_boolean(&retval);
        if (!Z_BVAL(retval)) {
            giterr_set_str(GIT_ENOTFOUND,"php-refdb-backend: del: no such reference");
            result = GIT_ERROR;
        }
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zrefname);
    zval_ptr_dtor(&zoldid);
    zval_ptr_dtor(&zoldtarget);

    return result;
}

/*static*/ int php_refdb_backend_object::compress(git_refdb_backend *backend)
{
    const int N = 0;
    int result = GIT_OK;

    zval fname;
    zval retval;
    zval* thisobj = EXTRACT_THISOBJ(backend);

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);

    ZVAL_STRING(&fname,"compress",1);

    // Call userspace method implementation corresponding to refdb operation.
    if (call_user_function(NULL,&thisobj,&fname,&retval,N,nullptr
            ZTS_MEMBER_CC(wrapper.object()->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);

    return result;
}

/*static*/ int php_refdb_backend_object::has_log(
    git_refdb_backend *backend,
    const char *refname)
{
    const int N = 1;
    int result = GIT_OK;

    zval fname;
    zval retval;
    zval* thisobj = EXTRACT_THISOBJ(backend);
    zval* params[N];
    zval* zrefname;

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zrefname);

    ZVAL_STRING(&fname,"has_log",1);
    ZVAL_STRING(zrefname,refname,1);

    params[0] = zrefname;

    // Call userspace method implementation corresponding to refdb operation.
    if (call_user_function(NULL,&thisobj,&fname,&retval,N,params
            ZTS_MEMBER_CC(wrapper.object()->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }
    else {
        convert_to_boolean(&retval);
        result = Z_BVAL(retval);
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zrefname);

    return result;
}

/*static*/ int php_refdb_backend_object::ensure_log(
    git_refdb_backend *backend,
    const char *refname)
{
    const int N = 1;
    int result = GIT_OK;

    zval fname;
    zval retval;
    zval* thisobj = EXTRACT_THISOBJ(backend);
    zval* params[N];
    zval* zrefname;

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zrefname);

    ZVAL_STRING(&fname,"ensure_log",1);
    ZVAL_STRING(zrefname,refname,1);

    params[0] = zrefname;

    // Call userspace method implementation corresponding to refdb operation.
    if (call_user_function(NULL,&thisobj,&fname,&retval,N,params
            ZTS_MEMBER_CC(wrapper.object()->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zrefname);

    return result;
}

/*static*/ int php_refdb_backend_object::reflog_read(
    git_reflog **out,
    git_refdb_backend *backend,
    const char *name)
{
    const int N = 1;
    int result = GIT_OK;

    // TODO Until we can manipulate git_reflog and git_reflog_entry
    // appropriately, we cannot implement this function.
    giterr_set_str(GIT_ERROR,"GitRefDBBackend::reflog_read(): function is not supported at this time");
    return GIT_ERROR;

    zval fname;
    zval retval;
    zval* thisobj = EXTRACT_THISOBJ(backend);
    zval* params[N];
    zval* zname;

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zname);

    ZVAL_STRING(&fname,"reflog_read",1);
    ZVAL_STRING(zname,name,1);

    params[0] = zname;

    // Call userspace method implementation corresponding to refdb operation.
    if (call_user_function(NULL,&thisobj,&fname,&retval,N,params
            ZTS_MEMBER_CC(wrapper.object()->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }
    else {
        if (Z_TYPE(retval) != IS_ARRAY) {
            php_error(E_ERROR,"GitRefDBBackend::reflog_read(): return value must be array");
            return GIT_ERROR;
        }

        zval** zlookup;
        HashPosition pos;
        git_reflog* log;

        // TODO Allocate reflog instance.
        log = nullptr;

        for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL(retval),&pos);
             zend_hash_get_current_data_ex(Z_ARRVAL(retval),(void**)&zlookup,&pos) == SUCCESS;
             zend_hash_move_forward_ex(Z_ARRVAL(retval),&pos))
        {
            git_reflog_entry* ent;

            if (reflog_entry_from_array(&ent,*zlookup) == GIT_ERROR) {
                result = GIT_ERROR;
                break;
            }

            // TODO Append raw reflog entry to reflog.

        }

        if (result == GIT_ERROR) {
            git_reflog_free(log);
        }
        else {
            *out = log;
        }
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zname);

    return result;
}

/*static*/ int php_refdb_backend_object::reflog_write(
    git_refdb_backend *backend,
    git_reflog *reflog)
{
    const int N = 1;
    int result = GIT_OK;

    zval fname;
    zval retval;
    zval* thisobj = EXTRACT_THISOBJ(backend);
    zval* params[N];
    zval* zreflog;
    php_resource_ref<php_git_reflog> reflogResource;

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zreflog);

    ZVAL_STRING(&fname,"reflog_write",1);
    *reflogResource.byval_git2() = reflog;
    reflogResource.ret(zreflog);

    params[0] = zreflog;

    // Call userspace method implementation corresponding to refdb operation.
    if (call_user_function(NULL,&thisobj,&fname,&retval,N,params
            ZTS_MEMBER_CC(wrapper.object()->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }
    else {
        // Handle the return value.
        convert_to_boolean(&retval);
        if (!Z_BVAL(retval)) {
            giterr_set_str(GIT_EINVALID,"php-refdb-backend: fail reflog_write()");
            result = GIT_ERROR;
        }
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zreflog);

    return result;
}

/*static*/ int php_refdb_backend_object::reflog_rename(
    git_refdb_backend *backend,
    const char *old_name,
    const char *new_name)
{
    const int N = 2;
    int result = GIT_OK;

    zval fname;
    zval retval;
    zval* thisobj = EXTRACT_THISOBJ(backend);
    zval* params[N];
    zval* zoldname;
    zval* znewname;

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zoldname);
    ALLOC_INIT_ZVAL(znewname);

    ZVAL_STRING(&fname,"reflog_rename",1);
    ZVAL_STRING(zoldname,old_name,1);
    ZVAL_STRING(znewname,new_name,1);

    // Call userspace method implementation corresponding to refdb operation.
    if (call_user_function(NULL,&thisobj,&fname,&retval,N,params
            ZTS_MEMBER_CC(wrapper.object()->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }
    else {
        // TODO Check for exception.
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zoldname);
    zval_ptr_dtor(&znewname);

    return result;
}

/*static*/ int php_refdb_backend_object::reflog_delete(git_refdb_backend *backend,
    const char *name)
{
    const int N = 1;
    int result = GIT_OK;

    zval fname;
    zval retval;
    zval* thisobj = EXTRACT_THISOBJ(backend);
    zval* params[N];
    zval* zname;

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zname);

    ZVAL_STRING(&fname,"reflog_delete",1);
    ZVAL_STRING(zname,name,1);

    params[0] = zname;

    // Call userspace method implementation corresponding to refdb operation.
    if (call_user_function(NULL,&thisobj,&fname,&retval,N,params
            ZTS_MEMBER_CC(wrapper.object()->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }
    else {
        // TODO Check for exception.
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zname);

    return result;
}

/*static*/ int php_refdb_backend_object::lock(void **payload_out,
    git_refdb_backend *backend,
    const char *refname)
{
    const int N = 1;
    int result = GIT_OK;

    zval fname;
    zval retval;
    zval* thisobj = EXTRACT_THISOBJ(backend);
    zval* params[N];
    zval* zrefname;

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zrefname);

    ZVAL_STRING(&fname,"lock",1);
    ZVAL_STRING(zrefname,refname,1);

    params[0] = zrefname;

    // Call userspace method implementation corresponding to refdb operation.
    if (call_user_function(NULL,&thisobj,&fname,&retval,N,params
            ZTS_MEMBER_CC(wrapper.object()->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }
    else {
        zval* zcpy;

        // Copy the return value zval.
        MAKE_STD_ZVAL(zcpy);
        ZVAL_COPY_VALUE(zcpy,&retval);
        zval_copy_ctor(zcpy);

        *payload_out = zcpy;
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zrefname);

    return result;
}

/*static*/ int php_refdb_backend_object::unlock(git_refdb_backend *backend,
    void *payload,
    int success,
    int update_reflog,
    const git_reference *ref,
    const git_signature *sig,
    const char *message)
{
    const int N = 6;
    int result = GIT_OK;

    zval fname;
    zval retval;
    zval* thisobj = EXTRACT_THISOBJ(backend);
    zval* params[N];
    zval* zpayload;
    zval* zsuccess;
    zval* zupdatereflog;
    zval* zref;
    zval* zsig;
    zval* zmessage;

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zsuccess);
    ALLOC_INIT_ZVAL(zupdatereflog);
    ALLOC_INIT_ZVAL(zref);
    ALLOC_INIT_ZVAL(zsig);
    ALLOC_INIT_ZVAL(zmessage);
    const php_resource_ref<php_git_reference> res;

    ZVAL_STRING(&fname,"unlock",1);
    if (payload != nullptr) {
        zpayload = reinterpret_cast<zval*>(payload);
    }
    else {
        ALLOC_INIT_ZVAL(zpayload);
    }
    ZVAL_BOOL(zsuccess,success);
    ZVAL_BOOL(zupdatereflog,update_reflog);
    *res.byval_git2() = ref;
    res.ret(zref);
    convert_signature(zsig,sig);
    if (message != nullptr) {
        ZVAL_STRING(zmessage,message,1);
    }

    params[0] = zpayload;
    params[1] = zsuccess;
    params[2] = zupdatereflog;
    params[3] = zref;
    params[4] = zsig;
    params[5] = zmessage;

    // Call userspace method implementation corresponding to refdb operation.
    if (call_user_function(NULL,&thisobj,&fname,&retval,N,params
            ZTS_MEMBER_CC(wrapper.object()->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }
    else {
        // TODO Check for exception.

    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zpayload);
    zval_ptr_dtor(&zsuccess);
    zval_ptr_dtor(&zupdatereflog);
    zval_ptr_dtor(&zref);
    zval_ptr_dtor(&zsig);
    zval_ptr_dtor(&zmessage);

    return result;
}

/*static*/ void php_refdb_backend_object::free(git_refdb_backend *backend)
{
    git_refdb_backend_php* wrapper = EXTRACT_BACKEND(backend);

    // Make sure object buckets still exist to lookup object (in case the
    // destructor was already called). This happens when free() is called after
    // PHP has finished cleaning up all objects (but before all variables, which
    // may have references to git2 objects that reference this PHP object).
    if (EG(objects_store).object_buckets != nullptr) {
        object_wrapper<php_odb_backend_object> obj(wrapper->thisobj);
        obj.object()->backend = nullptr;
    }

    wrapper->~git_refdb_backend_php();
    efree(backend);
}

// Implementation of php_refdb_backend_object::git_refdb_backend_php

php_refdb_backend_object::git_refdb_backend_php::git_refdb_backend_php(zval* zv)
{
    // Blank out the base class (which is the structure defined in the git2
    // headers).
    memset(this,0,sizeof(struct git_odb_backend));

    // Assign zval to keep object alive while the backend is in use in the
    // library.
    Z_ADDREF_P(zv);
    thisobj = zv;

    zend_class_entry* ce = Z_OBJCE_P(thisobj);
    version = GIT_REFDB_BACKEND_VERSION;

    // Every custom backend gets the free function (whether it is overloaded in
    // userspace or not).
    free = php_refdb_backend_object::free;

    // We now must select which functions we are going to include in the
    // backend. We do this by determining which ones were overloaded. PHP will
    // ensure that the abstract methods are always overloaded so we don't have
    // to check those.
    exists = php_refdb_backend_object::exists;
    lookup = php_refdb_backend_object::lookup;
    write = php_refdb_backend_object::write;
    rename = php_refdb_backend_object::rename;
    del = php_refdb_backend_object::del;
    has_log = php_refdb_backend_object::has_log;
    ensure_log = php_refdb_backend_object::ensure_log;
    reflog_read = php_refdb_backend_object::reflog_read;
    reflog_write = php_refdb_backend_object::reflog_write;
    reflog_rename = php_refdb_backend_object::reflog_rename;
    reflog_delete = php_refdb_backend_object::reflog_delete;
    if (is_method_overridden(ce,"iterator",sizeof("iterator"))) {
        iterator = php_refdb_backend_object::iterator;
    }
    if (is_method_overridden(ce,"compress",sizeof("compress"))) {
        compress = php_refdb_backend_object::compress;
    }
    if (is_method_overridden(ce,"lock",sizeof("lock"))) {
        lock = php_refdb_backend_object::lock;

        // Require unlock if lock is provided. This seems to be in line with
        // what happens in git2 source code: unlock is not checked but only
        // called when lock is present.
        if (!is_method_overridden(ce,"unlock",sizeof("unlock"))) {
            php_error(E_ERROR,"Cannot create custom refdb baceknd: unlock() must be implemented with lock()");
            // Control no longer in this function.
        }
        unlock = php_refdb_backend_object::unlock;
    }
}

php_refdb_backend_object::git_refdb_backend_php::~git_refdb_backend_php()
{
    // Free object zval.
    zval_ptr_dtor(&thisobj);
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
