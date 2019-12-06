/*
 * php-odb-backend-internal.cpp
 *
 * This file is a part of php-git2.
 */

#include "php-object.h"
using namespace php_git2;

// Class method entries

// string GitODBBackend_Internal::read(&$type,$oid);
static PHP_METHOD(GitODBBackend_Internal,read);
ZEND_BEGIN_ARG_INFO_EX(GitODBBackend_Internal_read_arginfo,0,0,2)
    ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

static PHP_METHOD(GitODBBackend_Internal,read_prefix);
ZEND_BEGIN_ARG_INFO_EX(GitODBBackend_Internal_read_prefix_arginfo,0,0,3)
    ZEND_ARG_PASS_INFO(1)
    ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

static PHP_METHOD(GitODBBackend_Internal,read_header);
ZEND_BEGIN_ARG_INFO_EX(GitODBBackend_Internal_read_header_arginfo,0,0,3)
    ZEND_ARG_PASS_INFO(1)
    ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

static PHP_METHOD(GitODBBackend_Internal,write);
static PHP_METHOD(GitODBBackend_Internal,writestream);
static PHP_METHOD(GitODBBackend_Internal,readstream);
static PHP_METHOD(GitODBBackend_Internal,exists);

static PHP_METHOD(GitODBBackend_Internal,exists_prefix);
ZEND_BEGIN_ARG_INFO_EX(GitODBBackend_Internal_exists_prefix_arginfo,0,0,2)
    ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

static PHP_METHOD(GitODBBackend_Internal,refresh);
static PHP_METHOD(GitODBBackend_Internal,for_each);
static PHP_METHOD(GitODBBackend_Internal,writepack);
zend_function_entry php_git2::odb_backend_internal_methods[] = {
    PHP_ME(GitODBBackend_Internal,read,GitODBBackend_Internal_read_arginfo,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend_Internal,read_prefix,GitODBBackend_Internal_read_prefix_arginfo,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend_Internal,read_header,GitODBBackend_Internal_read_header_arginfo,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend_Internal,write,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend_Internal,writestream,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend_Internal,readstream,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend_Internal,exists,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend_Internal,exists_prefix,GitODBBackend_Internal_exists_prefix_arginfo,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend_Internal,refresh,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend_Internal,for_each,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend_Internal,writepack,NULL,ZEND_ACC_PUBLIC)
    PHP_FE_END
};

// Make function implementation

void php_git2::php_git2_make_odb_backend(zval* zp,git_odb_backend* backend,php_git_odb* owner TSRMLS_DC)
{
    php_odb_backend_object* obj;
    zend_class_entry* ce = php_git2::class_entry[php_git2_odb_backend_internal_obj];

    // Create object zval.
    object_init_ex(zp,ce);
    obj = reinterpret_cast<php_odb_backend_object*>(zend_objects_get_address(zp TSRMLS_CC));

    // Assign the git2 odb_backend object.
    obj->backend = backend;

    // Assign owner. (If set, this increments owner refcount to prevent the ODB
    // from freeing while the backend object is in use.)
    obj->assign_owner(owner);

    // Assign kind based on if owner set.
    if (owner != nullptr) {
        obj->kind = php_odb_backend_object::conventional;
    }
    else {
        obj->kind = php_odb_backend_object::user;
    }
}

// Implementation of php_odb_backend_internal_object

/*static*/ zend_object_handlers php_odb_backend_internal_object::handlers;
php_odb_backend_internal_object::php_odb_backend_internal_object(zend_class_entry* ce TSRMLS_DC):
    php_odb_backend_object(ce TSRMLS_CC)
{
}

/*static*/ void php_odb_backend_internal_object::init(zend_class_entry* ce)
{
    handlers.get_constructor = php_git2::not_allowed_get_constructor;

    UNUSED(ce);
}

// Implementation of class methods

PHP_METHOD(GitODBBackend_Internal,read)
{
    php_bailer bailer ZTS_CTOR;
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
    if (object->backend == nullptr || object->backend->read == nullptr) {
        php_error(E_ERROR,"GitODBBackend_Internal::read(): method is not available");
        return;
    }

    // Grab parameters. The first parameter is passed by reference so we have to
    // extract its zval.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"zs",&ztype,&strOid,
            &strOidLen) == FAILURE)
    {
        return;
    }

    // Interpret the string parameter as a human-readable OID. Convert it
    // and then call read().
    try {
        int retval;
        convert_oid_fromstr(&oid,strOid,strOidLen);
        retval = object->backend->read(&data,&size,&type,object->backend,&oid);
        if (retval < 0) {
            php_git2::git_error(retval);
        }
        else {
            // Copy the result into the return zval. Then set the out
            // parameters. Finally we have to free the buffer allocated by the
            // call to read().
            RETVAL_STRINGL((const char*)data,size,1);
            ZVAL_LONG(ztype,type);
            free(data);
        }
    } catch (php_git2::php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer TSRMLS_CC);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle(TSRMLS_C);
        }
    }
}

PHP_METHOD(GitODBBackend_Internal,read_prefix)
{
    php_bailer bailer ZTS_CTOR;
    zval* zoid;
    zval* ztype;
    char* strOid;
    int strOidLen;
    php_odb_backend_object* object;

    // Grab object backing. Verify that the backend exists and the function is
    // implemented.
    object = LOOKUP_OBJECT(php_odb_backend_object,getThis());
    if (object->backend == nullptr || object->backend->read_prefix == nullptr) {
        php_error(E_ERROR,"GitODBBackend_Internal::read_prefix(): method is not available");
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

    // Interpret the string parameter as a human-readable OID. Convert it and
    // then call read_prefix().
    try {
        int retval;
        convert_oid_fromstr(&prefix,strOid,strOidLen);
        retval = object->backend->read_prefix(&full,&data,&size,&type,object->backend,&prefix,strOidLen);
        if (retval < 0) {
            php_git2::git_error(retval);
        }
        else {
            // Copy the result into the return zval. Then set the out
            // parameters.  Finally we have to free the buffer allocated by the
            // call to read_prefix().
            RETVAL_STRINGL((const char*)data,size,1);
            convert_oid(zoid,&full);
            ZVAL_LONG(ztype,type);
            free(data);
        }
    } catch (php_git2::php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer TSRMLS_CC);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle(TSRMLS_C);
        }
    }
}

PHP_METHOD(GitODBBackend_Internal,read_header)
{
    php_bailer bailer ZTS_CTOR;
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
        php_error(E_ERROR,"GitODBBackend_Internal::read_header(): method is not available");
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
        int retval;
        convert_oid_fromstr(&oid,strOid,strOidLen);
        retval = object->backend->read_header(&size,&type,object->backend,&oid);
        if (retval < 0) {
            php_git2::git_error(retval);
        }
    } catch (php_git2::php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer TSRMLS_CC);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle(TSRMLS_C);
        }

        return;
    }

    // This varient just returns the header as output parameters and returns
    // true.
    ZVAL_LONG(zsize,size);
    ZVAL_LONG(ztype,type);
}

PHP_METHOD(GitODBBackend_Internal,write)
{
    php_bailer bailer ZTS_CTOR;
    char* uoid;
    int uoidSize;
    char* data;
    int dataSize;
    long type;
    git_oid oid;
    php_odb_backend_object* object = LOOKUP_OBJECT(php_odb_backend_object,getThis());

    // Backend must be created and write function must be available.
    if (object->backend == nullptr || object->backend->write == nullptr) {
        php_error(E_ERROR,"GitODBBackend_Internal::write(): method is not available");
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
        int retval;
        convert_oid_fromstr(&oid,uoid,uoidSize);
        retval = object->backend->write(object->backend,&oid,data,dataSize,(git_otype)type);
        if (retval < 0) {
            php_git2::git_error(retval);
        }
    } catch (php_git2::php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer TSRMLS_CC);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle(TSRMLS_C);
        }
    }
}

PHP_METHOD(GitODBBackend_Internal,writestream)
{
    php_bailer bailer ZTS_CTOR;
    long offset;
    long objectType;
    git_odb_stream* outstream = nullptr;
    php_odb_backend_object* object = LOOKUP_OBJECT(php_odb_backend_object,getThis());

    // Backend must be created and write function must be available.
    if (object->backend == nullptr || object->backend->writestream == nullptr) {
        php_error(E_ERROR,"GitODBBackend_Internal::writestream(): method is not available");
        return;
    }

    // Grab parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"ll",&offset,&objectType) == FAILURE) {
        return;
    }

    // Call the underlying function.
    try {
        int retval;
        retval = object->backend->writestream(&outstream,object->backend,
            (git_off_t)offset,(git_otype)objectType);
        if (retval < 0) {
            php_git2::git_error(retval);
        }
        else if (outstream != nullptr) {
            // Create GitODBStream object to wrap git_odb_stream. Do NOT set an
            // owner since we want the GitODBStream object to free the stream
            // using its free() function.
            php_git2_make_odb_stream(return_value,outstream,nullptr TSRMLS_CC);
        }
    } catch (php_git2::php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer TSRMLS_CC);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle(TSRMLS_C);
        }
    }
}

PHP_METHOD(GitODBBackend_Internal,readstream)
{
    php_bailer bailer ZTS_CTOR;
    char* oidstr;
    int oidstr_len;
    git_oid oid;
    git_odb_stream* outstream = nullptr;
    php_odb_backend_object* object = LOOKUP_OBJECT(php_odb_backend_object,getThis());

    // Backend must be created and write function must be available.
    if (object->backend == nullptr || object->backend->readstream == nullptr) {
        php_error(E_ERROR,"GitODBBackend_Internal::readstream(): method is not available");
        return;
    }

    // Grab parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s",&oidstr,&oidstr_len) == FAILURE) {
        return;
    }

    // Call the underlying function.
    try {
        int retval;
        convert_oid_fromstr(&oid,oidstr,oidstr_len);
        retval = object->backend->readstream(&outstream,object->backend,&oid);
        if (retval < 0) {
            php_git2::git_error(retval);
        }
        else if (outstream != nullptr) {
            // Create GitODBStream object to wrap git_odb_stream. Do NOT set an
            // owner since we want the GitODBStream object to free the stream
            // using its free() function.
            php_git2_make_odb_stream(return_value,outstream,nullptr TSRMLS_CC);
        }
    } catch (php_git2::php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer TSRMLS_CC);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle(TSRMLS_C);
        }
    }
}

PHP_METHOD(GitODBBackend_Internal,exists)
{
    php_bailer bailer ZTS_CTOR;
    int retval;
    char* uoid;
    int uoidSize;
    git_oid oid;
    php_odb_backend_object* object = LOOKUP_OBJECT(php_odb_backend_object,getThis());

    // Backend must be created.
    if (object->backend == nullptr || object->backend->exists == nullptr) {
        php_error(E_ERROR,"GitODBBackend_Internal::exists(): method is not available");
        return;
    }

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s",&uoid,&uoidSize) == FAILURE) {
        return;
    }

    // Convert OID hex string to oid structure.
    convert_oid_fromstr(&oid,uoid,uoidSize);

    // Call underlying function.
    try {
        retval = object->backend->exists(object->backend,&oid);
        if (retval < 0) {
            php_git2::git_error(retval);
        }
    } catch (php_git2::php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer TSRMLS_CC);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle(TSRMLS_C);
        }

        return;
    }

    // The exists() function should return 1 if found.
    if (retval == 1) {
        RETURN_TRUE;
    }

    RETURN_FALSE;
}

PHP_METHOD(GitODBBackend_Internal,exists_prefix)
{
    php_bailer bailer ZTS_CTOR;
    int retval;
    zval* zoid;
    char* uoid;
    int uoidSize;
    git_oid oid;
    git_oid fullOid;
    php_odb_backend_object* object = LOOKUP_OBJECT(php_odb_backend_object,getThis());

    // Backend must be created.
    if (object->backend == nullptr || object->backend->exists_prefix == nullptr) {
        php_error(E_ERROR,"GitODBBackend_Internal::exists_prefix(): method is not available");
        return;
    }

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"zs",&zoid,&uoid,&uoidSize) == FAILURE) {
        return;
    }

    // Convert OID hex string to oid structure.
    convert_oid_fromstr(&oid,uoid,uoidSize);

    // Call underlying function.
    try {
        retval = object->backend->exists_prefix(&fullOid,object->backend,&oid,uoidSize);
        if (retval < 0) {
            php_git2::git_error(retval);
        }
    } catch (php_git2::php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer TSRMLS_CC);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle(TSRMLS_C);
        }

        return;
    }

    // Confusingly, the exists_prefix() function returns 0 if found.
    if (retval == 0) {
        // Set the out variable to the full OID string.
        convert_oid(zoid,&fullOid);

        RETURN_TRUE;
    }

    // No need to set the out variable. PHP already did this if it was a new
    // variable. Otherwise it will just keep its old value.
    RETURN_FALSE;
}

PHP_METHOD(GitODBBackend_Internal,refresh)
{
    php_bailer bailer ZTS_CTOR;
    php_odb_backend_object* object = LOOKUP_OBJECT(php_odb_backend_object,getThis());

    // Backend must be created and the function must be implemented.
    if (object->backend == nullptr || object->backend->refresh == nullptr) {
        php_error(E_ERROR,"GitODBBackend_Internal::refresh(): method is not available");
        return;
    }

    // Perform function call.
    try {
        int retval;
        retval = object->backend->refresh(object->backend);
        if (retval < 0) {
            php_git2::git_error(retval);
        }
    } catch (php_git2::php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer TSRMLS_CC);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle(TSRMLS_C);
        }
    }
}

PHP_METHOD(GitODBBackend_Internal,for_each)
{
    php_bailer bailer ZTS_CTOR;
    php_odb_backend_object* object = LOOKUP_OBJECT(php_odb_backend_object,getThis());

    // Backend must be created and the function must be implemented.
    if (object->backend == nullptr || object->backend->foreach == nullptr) {
        php_error(E_ERROR,"GitODBBackend_Internal::for_each(): method is not available");
        return;
    }

    int retval;
    php_callback_sync callback ZTS_CTOR;

    if (zend_get_parameters(0,2,callback.byref_php(1),callback.byref_php(2)) == FAILURE) {
        php_error(E_ERROR,"GitODBBackend_Internal::for_each(): method expects 2 arguments");
        return;
    }

    // Call the underlying function.
    try {
        php_callback_handler<odb_foreach_callback> handler ZTS_CTOR;

        retval = object->backend->foreach(object->backend,
            handler.byval_git2(),callback.byval_git2());

        if (retval < 0) {
            php_git2::git_error(retval);
        }
    } catch (php_git2::php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer TSRMLS_CC);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle(TSRMLS_C);
        }
    }
}

PHP_METHOD(GitODBBackend_Internal,writepack)
{
    php_bailer bailer ZTS_CTOR;
    zval* thisobj = getThis();
    php_odb_backend_object* object = LOOKUP_OBJECT(php_odb_backend_object,thisobj);

    // Backend must be created and the function must be implemented.
    if (object->backend == nullptr || object->backend->writepack == nullptr) {
        php_error(E_ERROR,"GitODBBackend_Internal::writepack(): method is not available");
        return;
    }

    git_odb_writepack* wp;
    php_callback_sync_nullable* callback;

    // Allocate callback.
    callback = new (emalloc(sizeof(php_callback_sync_nullable))) php_callback_sync_nullable(TSRMLS_C);

    if (zend_get_parameters(0,2,callback->byref_php(1),callback->byref_php(2)) == FAILURE) {
        callback->~php_callback_sync_nullable();
        efree(callback);
        php_error(E_ERROR,"GitODBBackend_Internal::writepack(): method expects 2 arguments");
        return;
    }

    try {
        int retval;
        php_callback_handler_nullable_connector<transfer_progress_callback> handler(*callback TSRMLS_CC);

        // Call the underlying function.
        retval = object->backend->writepack(
            &wp,
            object->backend,
            object->backend->odb,
            handler.byval_git2(),
            callback->byval_git2());

        if (retval < 0) {
            php_git2::git_error(retval);
        }
        else {
            // Create return value out of the writepack and callback. The
            // callback will be managed by the GitODBWritepack_Internal object.
            php_git2_make_odb_writepack(return_value,wp,callback,thisobj,object->owner TSRMLS_CC);
        }
    } catch (php_git2::php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer TSRMLS_CC);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle(TSRMLS_C);
        }

        callback->~php_callback_sync_nullable();
        efree(callback);
    }
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
