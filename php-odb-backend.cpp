/*
 * php-odb-backend.cpp
 *
 * This file is a part of php-git2.
 *
 * This unit provides the out-of-line implementation for the gitODBBackend
 * class.
 */

#include "php-object.h"
using namespace php_git2;

// Class method entries

// string GitODBBackend::read(&$size,&$type,$oid);
static PHP_METHOD(GitODBBackend,read);
ZEND_BEGIN_ARG_INFO_EX(GitODBBackend_read_arginfo,0,0,3)
    ZEND_ARG_PASS_INFO(1)
    ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

static PHP_METHOD(GitODBBackend,read_prefix);
ZEND_BEGIN_ARG_INFO_EX(GitODBBackend_read_prefix_arginfo,0,0,4)
    ZEND_ARG_PASS_INFO(1)
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
    PHP_ME(GitODBBackend,exists_prefix,NULL,ZEND_ACC_PUBLIC)
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
    zval* zsize;
    zval* ztype;
    char* strOid;
    int strOidLen;
    php_odb_backend_object* object;

    // Grab parameters. The first two parameters are passed by reference so we
    // have to extract their zvals.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"zzs",&zsize,&ztype,&strOid,
            &strOidLen) == FAILURE)
    {
        return;
    }

    // Grab object backing.
    object = LOOKUP_OBJECT(php_odb_backend_object,getThis());

    // If the odb_backend exists, then we can actually call the underlying
    // function. The backend only exists if we retrieved the object from an
    // existing ODB or if we passed the custom backend to an ODB setter which
    // prompted it to be created.
    if (object->backend != nullptr) {
        void* data = NULL;
        size_t size = 0;
        git_otype type = GIT_OBJ_ANY;
        git_oid oid;

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
        ZVAL_LONG(zsize,size);
        ZVAL_LONG(ztype,type);
        free(data);
        return;
    }

    RETURN_FALSE;
}

PHP_METHOD(GitODBBackend,read_prefix)
{
    zval* zoid;
    zval* zsize;
    zval* ztype;
    char* strOid;
    int strOidLen;
    php_odb_backend_object* object;

    // Grab parameters. The first three parameters are passed by reference so we
    // have to extract their zvals.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"zzzs",&zoid,&zsize,&ztype,
            &strOid,&strOidLen) == FAILURE)
    {
        return;
    }

    // Grab object backing.
    object = LOOKUP_OBJECT(php_odb_backend_object,getThis());

    // If the odb_backend exists, then we can actually call the underlying
    // function. The backend only exists if we retrieved the object from an
    // existing ODB or if we passed the custom backend to an ODB setter which
    // prompted it to be created.
    if (object->backend != nullptr) {
        void* data = NULL;
        size_t size = 0;
        git_otype type = GIT_OBJ_ANY;
        git_oid prefix;
        git_oid full;
        char buf[GIT_OID_HEXSZ + 1];

        // Interpret the string parameter as a human-readable OID. Convert it
        // and then call read_prefix().
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
        // parameters. Finally we have to free the buffer allocated by the call
        // to read().
        RETVAL_STRINGL((const char*)data,size,1);
        git_oid_tostr(buf,sizeof(buf),&full);
        ZVAL_STRING(zoid,buf,1);
        ZVAL_LONG(zsize,size);
        ZVAL_LONG(ztype,type);
        free(data);
        return;
    }

    RETURN_FALSE;
}

PHP_METHOD(GitODBBackend,read_header)
{
    zval* zsize;
    zval* ztype;
    char* strOid;
    int strOidLen;
    php_odb_backend_object* object;

    // Grab parameters. The first two parameters are passed by reference so we
    // have to extract their zvals.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"zzs",&zsize,&ztype,&strOid,
            &strOidLen) == FAILURE)
    {
        return;
    }

    // Grab object backing.
    object = LOOKUP_OBJECT(php_odb_backend_object,getThis());

    // If the odb_backend exists, then we can actually call the underlying
    // function. The backend only exists if we retrieved the object from an
    // existing ODB or if we passed the custom backend to an ODB setter which
    // prompted it to be created.
    if (object->backend != nullptr) {
        size_t size = 0;
        git_otype type = GIT_OBJ_ANY;
        git_oid oid;

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

    RETURN_FALSE;
}

PHP_METHOD(GitODBBackend,write)
{
}

PHP_METHOD(GitODBBackend,writestream)
{
}

PHP_METHOD(GitODBBackend,readstream)
{
}

PHP_METHOD(GitODBBackend,exists)
{
}

PHP_METHOD(GitODBBackend,exists_prefix)
{
}

PHP_METHOD(GitODBBackend,refresh)
{
}

PHP_METHOD(GitODBBackend,foreach)
{
}

PHP_METHOD(GitODBBackend,writepack)
{
}

PHP_METHOD(GitODBBackend,free)
{
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
