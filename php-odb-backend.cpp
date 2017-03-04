/*
 * php-odb-backend.cpp
 *
 * This file is a part of php-git2.
 *
 * This unit provides the out-of-line implementation for the gitODBBackend
 * class.
 */

#include "php-object.h"
#include "php-callback.h"
using namespace php_git2;

// Helper macros

#define EXTRACT_THISOBJ(backend) \
    reinterpret_cast<php_odb_backend_object::php_git_odb_backend*>(backend)->thisobj;

// Class method entries

// string GitODBBackend::read(&$type,$oid);
static PHP_METHOD(GitODBBackend,read);
ZEND_BEGIN_ARG_INFO_EX(GitODBBackend_read_arginfo,0,0,2)
    ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

static PHP_METHOD(GitODBBackend,read_prefix);
ZEND_BEGIN_ARG_INFO_EX(GitODBBackend_read_prefix_arginfo,0,0,3)
    ZEND_ARG_PASS_INFO(1)
    ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

static PHP_METHOD(GitODBBackend,read_header);
ZEND_BEGIN_ARG_INFO_EX(GitODBBackend_read_header_arginfo,0,0,3)
    ZEND_ARG_PASS_INFO(1)
    ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

static PHP_METHOD(GitODBBackend,write);
static PHP_METHOD(GitODBBackend,writestream);
static PHP_METHOD(GitODBBackend,readstream);
static PHP_METHOD(GitODBBackend,exists);

static PHP_METHOD(GitODBBackend,exists_prefix);
ZEND_BEGIN_ARG_INFO_EX(GitODBBackend_exists_prefix_arginfo,0,0,2)
    ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

static PHP_METHOD(GitODBBackend,refresh);
static PHP_METHOD(GitODBBackend,foreach);
static PHP_METHOD(GitODBBackend,writepack);
static PHP_METHOD(GitODBBackend,free);
zend_function_entry php_git2::odb_backend_methods[] = {
    PHP_ME(GitODBBackend,read,GitODBBackend_read_arginfo,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend,read_prefix,GitODBBackend_read_prefix_arginfo,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend,read_header,GitODBBackend_read_header_arginfo,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend,write,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend,writestream,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend,readstream,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend,exists,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend,exists_prefix,GitODBBackend_exists_prefix_arginfo,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend,refresh,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend,foreach,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend,writepack,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend,free,NULL,ZEND_ACC_PUBLIC)
    PHP_FE_END
};

// Make function implementation

void php_git2::php_git2_make_odb_backend(zval* zp,git_odb_backend* backend,bool owner TSRMLS_DC)
{
    php_odb_backend_object* obj;
    zend_class_entry* ce = php_git2::class_entry[php_git2_odb_backend_obj];

    object_init_ex(zp,ce);
    obj = reinterpret_cast<php_odb_backend_object*>(zend_objects_get_address(zp TSRMLS_CC));

    // Assign the git2 odb_backend object.
    obj->backend = backend;
    obj->isowner = owner;

    // Update the version property to the version specified by the backend.
    zend_update_property_long(ce,zp,"version",sizeof("version")-1,backend->version TSRMLS_CC);
}

// Implementation of php_odb_backend_object

/*static*/ zend_object_handlers php_odb_backend_object::handlers;
php_odb_backend_object::php_odb_backend_object(TSRMLS_D):
    php_zts_base(TSRMLS_C), backend(nullptr), isowner(false)
{
    zend_class_entry* ce = php_git2::class_entry[php_git2_odb_backend_obj];
    zend_object_std_init(&base,ce TSRMLS_CC);
    object_properties_init(&base,ce);
}

php_odb_backend_object::~php_odb_backend_object()
{
    // AFAIK we never call the free() function on git_odb_backend objects unless
    // they were returned directly from a library call. We've marked this using
    // the 'isowner' flag.
    if (isowner && backend != nullptr) {
        backend->free(backend);
    }

    zend_object_std_dtor(&base TSRMLS_CC);
}

/*static*/ void php_odb_backend_object::init(zend_class_entry* ce)
{
    zend_declare_property_long(ce,"version",sizeof("version")-1,0,ZEND_ACC_PUBLIC);
}

/*static*/ int php_odb_backend_object::read(void** datap,size_t* sizep,
    git_otype* typep,git_odb_backend* backend,const git_oid* oid)
{

    return GIT_OK;
}

/*static*/ int php_odb_backend_object::read_prefix(git_oid* oidp,void** datap,
    size_t* sizep,git_otype* typep,git_odb_backend* backend,const git_oid* prefix,
    size_t nbits)
{

    return GIT_OK;
}

/*static*/ int php_odb_backend_object::read_header(size_t* sizep,git_otype* typep,
    git_odb_backend* backend,const git_oid* oid)
{

    return GIT_OK;
}

/*static*/ int php_odb_backend_object::write(git_odb_backend* backend,
    const git_oid* oid,const void* data,size_t size,git_otype type)
{

    return GIT_OK;
}

/*static*/ int php_odb_backend_object::writestream(git_odb_stream** streamp,
    git_odb_backend* backend,git_off_t off,git_otype type)
{

    return GIT_OK;
}

/*static*/ int php_odb_backend_object::readstream(git_odb_stream** streamp,
    git_odb_backend* backend,const git_oid* oid)
{

    return GIT_OK;
}

/*static*/ int php_odb_backend_object::exists(git_odb_backend* backend,
    const git_oid* oid)
{

    return GIT_OK;
}

/*static*/ int php_odb_backend_object::exists_prefix(git_oid* oidp,
    git_odb_backend* backend,const git_oid* prefix,size_t nbits)
{

    return GIT_OK;
}

/*static*/ int php_odb_backend_object::refresh(git_odb_backend* backend)
{

    return GIT_OK;
}

/*static*/ int php_odb_backend_object::foreach(git_odb_backend* backend,
    git_odb_foreach_cb cb,void* payload)
{
    return GIT_OK;
}

/*static*/ int php_odb_backend_object::writepack(git_odb_writepack** writepackp,
    git_odb_backend* backend,git_odb* odb,git_transfer_progress_cb progress_cb,
    void* progress_payload)
{
    return GIT_OK;
}

/*static*/ void php_odb_backend_object::free(git_odb_backend* backend)
{
}

static void convert_oid_fromstr(git_oid* dest,const char* src,int srclen)
{
    // Use a temporary buffer to hold the OID hex string.
    char buf[GIT_OID_HEXSZ + 1];
    memset(buf,'0',GIT_OID_HEXSZ);
    buf[GIT_OID_HEXSZ] = 0;
    if (srclen > GIT_OID_HEXSZ) {
        srclen = GIT_OID_HEXSZ;
    }
    strncpy(buf,src,srclen);
    git_oid_fromstr(dest,buf);
}

// Implementation of object methods

PHP_METHOD(GitODBBackend,read)
{
    zval* ztype;
    char* strOid;
    int strOidLen;
    php_odb_backend_object* object;
    void* data = NULL;
    size_t size = 0;
    git_otype type = GIT_OBJ_ANY;
    git_oid oid;

    // Grab object backing. Verify that the backend exists and the function is
    // implemented.
    object = LOOKUP_OBJECT(php_odb_backend_object,getThis());
    if (object->backend == nullptr || object->backend->read_prefix == nullptr) {
        php_error(E_ERROR,"GitODBBackend::read(): method is not available");
        return;
    }

    // Grab parameters. The first two parameters are passed by reference so we
    // have to extract their zvals.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"zs",&ztype,&strOid,
            &strOidLen) == FAILURE)
    {
        return;
    }

    // Interpret the string parameter as a human-readable OID. Convert it
    // and then call read().
    try {
        convert_oid_fromstr(&oid,strOid,strOidLen);
        if (object->backend->read(&data,&size,&type,object->backend,&oid) < 0) {
            git_error();
        }
    } catch (php_git2_exception ex) {
        zend_throw_exception(nullptr,ex.what(),0 TSRMLS_CC);
        return;
    }

    // Copy the result into the return zval. Then set the out
    // parameters. Finally we have to free the buffer allocated by the call
    // to read().
    RETVAL_STRINGL((const char*)data,size,1);
    ZVAL_LONG(ztype,type);
    free(data);
}

PHP_METHOD(GitODBBackend,read_prefix)
{
    zval* zoid;
    zval* ztype;
    char* strOid;
    int strOidLen;
    php_odb_backend_object* object;

    // Grab object backing. Verify that the backend exists and the function is
    // implemented.
    object = LOOKUP_OBJECT(php_odb_backend_object,getThis());
    if (object->backend == nullptr || object->backend->read_prefix == nullptr) {
        php_error(E_ERROR,"GitODBBackend::read_prefix(): method is not available");
        return;
    }

    // Grab parameters. The first three parameters are passed by reference so we
    // have to extract their zvals.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"zzs",&zoid,&ztype,
            &strOid,&strOidLen) == FAILURE)
    {
        return;
    }

    void* data = NULL;
    size_t size = 0;
    git_otype type = GIT_OBJ_ANY;
    git_oid prefix;
    git_oid full;
    char buf[GIT_OID_HEXSZ + 1];

    // Interpret the string parameter as a human-readable OID. Convert it and
    // then call read_prefix().
    try {
        convert_oid_fromstr(&prefix,strOid,strOidLen);
        if (object->backend->read_prefix(&full,&data,&size,&type,object->backend,&prefix,strOidLen) < 0) {
            git_error();
        }
    } catch (php_git2_exception ex) {
        zend_throw_exception(nullptr,ex.what(),0 TSRMLS_CC);
        return;
    }

    // Copy the result into the return zval. Then set the out
    // parameters. Finally we have to free the buffer allocated by the call to
    // read().
    RETVAL_STRINGL((const char*)data,size,1);
    git_oid_tostr(buf,sizeof(buf),&full);
    ZVAL_STRING(zoid,buf,1);
    ZVAL_LONG(ztype,type);
    free(data);
}

PHP_METHOD(GitODBBackend,read_header)
{
    zval* zsize;
    zval* ztype;
    char* strOid;
    int strOidLen;
    php_odb_backend_object* object;
    size_t size = 0;
    git_otype type = GIT_OBJ_ANY;
    git_oid oid;

    // Grab object backing. Verify that the backend exists and the function is
    // implemented.
    object = LOOKUP_OBJECT(php_odb_backend_object,getThis());
    if (object->backend == nullptr || object->backend->read_header == nullptr) {
        php_error(E_ERROR,"GitODBBackend::read_header(): method is not available");
        return;
    }

    // Grab parameters. The first two parameters are passed by reference so we
    // have to extract their zvals.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"zzs",&zsize,&ztype,&strOid,
            &strOidLen) == FAILURE)
    {
        return;
    }

    // Interpret the string parameter as a human-readable OID. Convert it
    // and then call read().
    try {
        convert_oid_fromstr(&oid,strOid,strOidLen);
        if (object->backend->read_header(&size,&type,object->backend,&oid) < 0) {
            git_error();
        }
    } catch (php_git2_exception ex) {
        zend_throw_exception(nullptr,ex.what(),0 TSRMLS_CC);
        return;
    }

    // This varient just returns the header as output parameters and returns
    // true.
    ZVAL_LONG(zsize,size);
    ZVAL_LONG(ztype,type);

    RETURN_TRUE;
}

PHP_METHOD(GitODBBackend,write)
{
    char* uoid;
    int uoidSize;
    char* data;
    int dataSize;
    long type;
    git_oid oid;
    php_odb_backend_object* object = LOOKUP_OBJECT(php_odb_backend_object,getThis());

    // Backend must be created and write function must be available.
    if (object->backend == nullptr || object->backend->write == nullptr) {
        php_error(E_ERROR,"GitODBBackend::write(): method is not available");
        return;
    }

    // Grab parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"ssl",&uoid,&uoidSize,
            &data,&dataSize,&type) == FAILURE)
    {
        return;
    }

    // Convert OID hex string to oid structure and call underlying function.
    try {
        convert_oid_fromstr(&oid,uoid,uoidSize);
        if (object->backend->write(object->backend,&oid,data,dataSize,(git_otype)type) < 0) {
            git_error();
        }
    } catch (php_git2_exception ex) {
        zend_throw_exception(nullptr,ex.what(),0 TSRMLS_CC);
        return;
    }

    RETURN_TRUE;
}

PHP_METHOD(GitODBBackend,writestream)
{
}

PHP_METHOD(GitODBBackend,readstream)
{
}

PHP_METHOD(GitODBBackend,exists)
{
    int retval;
    char* uoid;
    int uoidSize;
    git_oid oid;
    php_odb_backend_object* object = LOOKUP_OBJECT(php_odb_backend_object,getThis());

    // Backend must be created.
    if (object->backend == nullptr || object->backend->exists == nullptr) {
        php_error(E_ERROR,"GitODBBackend::exists(): method is not available");
        return;
    }

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s",&uoid,&uoidSize) == FAILURE) {
        return;
    }

    // Convert OID hex string to oid structure and call underlying function.
    convert_oid_fromstr(&oid,uoid,uoidSize);
    retval = object->backend->exists(object->backend,&oid);

    // The exists() function should return 1 if found.
    if (retval) {
        RETURN_TRUE;
    }
    RETURN_FALSE;
}

PHP_METHOD(GitODBBackend,exists_prefix)
{
    int retval;
    zval* zoid;
    char* uoid;
    int uoidSize;
    git_oid oid;
    git_oid fullOid;
    php_odb_backend_object* object = LOOKUP_OBJECT(php_odb_backend_object,getThis());

    // Backend must be created.
    if (object->backend == nullptr || object->backend->exists_prefix == nullptr) {
        php_error(E_ERROR,"GitODBBackend::exists_prefix(): method is not available");
        return;
    }

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"zs",&zoid,&uoid,&uoidSize) == FAILURE) {
        return;
    }

    // Convert OID hex string to oid structure and call underlying function.
    convert_oid_fromstr(&oid,uoid,uoidSize);
    retval = object->backend->exists_prefix(&fullOid,object->backend,&oid,uoidSize);

    // Confusingly, the exists_prefix() function returns 0 if found.
    if (retval == 0) {
        char buf[GIT_OID_HEXSZ + 1];

        // Set the out variable to the full OID string.
        git_oid_tostr(buf,sizeof(buf),&fullOid);
        ZVAL_STRING(zoid,buf,1);

        RETURN_TRUE;
    }

    // No need to set the out variable. PHP already did this if it was a new
    // variable. Otherwise it will just keep its old value.
    RETURN_FALSE;
}

PHP_METHOD(GitODBBackend,refresh)
{
    php_odb_backend_object* object = LOOKUP_OBJECT(php_odb_backend_object,getThis());

    // Backend must be created and the function must be implemented.
    if (object->backend == nullptr || object->backend->refresh == nullptr) {
        php_error(E_ERROR,"GitODBBackend::refresh(): method is not available");
        return;
    }

    // Perform function call.
    try {
        if (object->backend->refresh(object->backend) < 0) {
            git_error();
        }
    } catch (php_git2_exception ex) {
        zend_throw_exception(nullptr,ex.what(),0 TSRMLS_CC);
        return;
    }
}

PHP_METHOD(GitODBBackend,foreach)
{
    php_odb_backend_object* object = LOOKUP_OBJECT(php_odb_backend_object,getThis());

    // Backend must be created and the function must be implemented.
    if (object->backend == nullptr || object->backend->foreach == nullptr) {
        php_error(E_ERROR,"GitODBBackend::foreach(): method is not available");
        return;
    }

    php_callback_sync callback;
    php_callback_handler<odb_foreach_callback> handler;

    if (zend_get_parameters(0,2,callback.byref_php(1),callback.byref_php(2)) == FAILURE) {
        php_error(E_ERROR,"GitODBBackend::foreach(): method expects 2 arguments");
        return;
    }

    // Call the underlying function.
    try {
        object->backend->foreach(object->backend,
            handler.byval_git2(),callback.byval_git2());
    } catch (php_git2_exception ex) {
        zend_throw_exception(nullptr,ex.what(),0 TSRMLS_CC);
        return;
    }
}

PHP_METHOD(GitODBBackend,writepack)
{
    php_odb_backend_object* object = LOOKUP_OBJECT(php_odb_backend_object,getThis());

    // Backend must be created and the function must be implemented.
    if (object->backend == nullptr || object->backend->writepack == nullptr) {
        php_error(E_ERROR,"GitODBBackend::writepack(): method is not available");
        return;
    }

    git_odb_writepack* wp;
    php_callback_sync* callback;
    php_callback_handler<transfer_progress_callback> handler;

    // Allocate callback.
    callback = new (emalloc(sizeof(php_callback_sync))) php_callback_sync(TSRMLS_C);

    if (zend_get_parameters(0,2,callback->byref_php(1),callback->byref_php(2)) == FAILURE) {
        callback->~php_callback_sync();
        efree(callback);
        php_error(E_ERROR,"GitODBBackend::writepack(): method expects 2 arguments");
        return;
    }

    try {
        // Call the underlying function.
        object->backend->writepack(&wp,object->backend,object->backend->odb,
            handler.byval_git2(),callback->byval_git2());

        // Create return value out of the writepack and callback. The callback
        // will be managed by the GitODBWritepack object.
        php_git2_make_odb_writepack(return_value,wp,callback TSRMLS_CC);
    } catch (php_git2_exception ex) {
        zend_throw_exception(nullptr,ex.what(),0 TSRMLS_CC);
        return;
    }
}

PHP_METHOD(GitODBBackend,free)
{
    php_odb_backend_object* object = LOOKUP_OBJECT(php_odb_backend_object,getThis());

    // Backend must be created and the function must be implemented.
    if (object->backend == nullptr || object->backend->foreach == nullptr) {
        php_error(E_ERROR,"GitODBBackend::foreach(): method is not available");
        return;
    }

    // We really don't want people to be able to shoot themselves in the foot
    // with this one (after all, crashing the Web server is generally not
    // considered a good thing to do). We only call free if the owner flag is
    // set to true. If owner is false we silently do nothing (the user really
    // has no business calling this anyway since our destructor will get it).

    if (object->isowner) {
        object->backend->free(object->backend);

        // Avoid double free's later on.
        object->backend = nullptr;
    }
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
