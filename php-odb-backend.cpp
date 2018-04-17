/*
 * php-odb-backend.cpp
 *
 * This file is a part of php-git2.
 *
 * This unit provides the out-of-line implementation for the GitODBBackend
 * class.
 */

#include "php-object.h"
#include "php-callback.h"
#include <cstring>
#include <new>
using namespace php_git2;

// Helper macros

#define EXTRACT_BACKEND(backend) \
    reinterpret_cast<php_odb_backend_object::git_odb_backend_php*>(backend)

#define EXTRACT_THISOBJ(backend) \
    EXTRACT_BACKEND(backend)->thisobj

// Custom class handlers

static zval* odb_backend_read_property(zval* obj,zval* prop,int type,const zend_literal* key TSRMLS_DC);
static void odb_backend_write_property(zval* obj,zval* prop,zval* value,const zend_literal* key TSRMLS_DC);
static int odb_backend_has_property(zval* obj,zval* prop,int chk_type,const zend_literal* key TSRMLS_DC);

// Closure callbacks

struct foreach_callback_info
{
    git_odb_foreach_cb callback;
    void* payload;
};

static ZEND_FUNCTION(backend_foreach_callback);

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
static PHP_METHOD(GitODBBackend,for_each);
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
    PHP_ME(GitODBBackend,for_each,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend,writepack,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend,free,NULL,ZEND_ACC_PUBLIC)
    PHP_FE_END
};

// Make function implementation

void php_git2::php_git2_make_odb_backend(zval* zp,git_odb_backend* backend,php_git_odb* owner TSRMLS_DC)
{
    php_odb_backend_object* obj;
    zend_class_entry* ce = php_git2::class_entry[php_git2_odb_backend_obj];

    // Create object zval.
    object_init_ex(zp,ce);
    obj = reinterpret_cast<php_odb_backend_object*>(zend_objects_get_address(zp TSRMLS_CC));

    // Assign the git2 odb_backend object and owner. We assume the owner manages
    // the lifetime of the backend.
    obj->backend = backend;
    obj->owner = owner;
}

// Implementation of php_odb_backend_object

/*static*/ zend_object_handlers php_odb_backend_object::handlers;
php_odb_backend_object::php_odb_backend_object(zend_class_entry* ce TSRMLS_DC):
    backend(nullptr), owner(nullptr), derivedCall(false), zts(TSRMLS_C)
{
    zend_object_std_init(this,ce TSRMLS_CC);
    object_properties_init(this,ce);
}

php_odb_backend_object::~php_odb_backend_object()
{
    // AFAIK we never call the free() function on git_odb_backend objects that
    // were returned from a call that referenced a git_odb. This is the case if
    // 'owner' is set.

    if (owner == nullptr && backend != nullptr) {
        backend->free(backend);
    }

    zend_object_std_dtor(this ZTS_MEMBER_CC(this->zts));
}

void php_odb_backend_object::create_custom_backend(zval* zobj,php_git_odb* newOwner)
{
    // NOTE: the zval should be any zval that points to an object with 'this' as
    // its backing (i.e. result of zend_objects_get_address()). This is used by
    // the implementation to obtain class entry info for the class that was used
    // to create the object AND to keep the PHP object alive while it is being
    // used by git2.

    // Make sure object doesn't already have a backing. This would imply it is
    // in use already in an ODB.
    if (backend != nullptr) {
        php_error(E_ERROR,"Cannot create custom ODB backend: object already in use");
    }

    // Create custom backend. Custom backends are always passed off to git2, so
    // we are not responsible for calling its free function.
    backend = new (emalloc(sizeof(git_odb_backend_php))) git_odb_backend_php(zobj);

    // Assume new owner. It should be a non-null ODB resource pointer.
    owner = newOwner;
}

/*static*/ void php_odb_backend_object::init(zend_class_entry* ce)
{
    handlers.read_property = odb_backend_read_property;
    handlers.write_property = odb_backend_write_property;
    handlers.has_property = odb_backend_has_property;
    handlers.get_constructor = php_git2::disallow_base_get_constructor;
    (void)ce;
}

/*static*/ int php_odb_backend_object::read(void** datap,size_t* sizep,
    git_otype* typep,git_odb_backend* backend,const git_oid* oid)
{
    zval fname;
    zval retval;
    zval* zoid;
    zval* ztype;
    zval* params[2];
    zval* thisobj = EXTRACT_THISOBJ(backend);
    char buffer[GIT_OID_HEXSZ+1];
    int result = GIT_OK;
    void* data;

    // Initialize zvals to default.
    INIT_ZVAL(fname);
    ALLOC_INIT_ZVAL(ztype);
    ALLOC_INIT_ZVAL(zoid);
    INIT_ZVAL(retval);
    params[0] = ztype;
    params[1] = zoid;

    // Assign method call name and search OID string to respective zvals.
    ZVAL_STRING(&fname,"read",1);
    git_oid_tostr(buffer,sizeof(buffer),oid);
    ZVAL_STRINGL(zoid,buffer,GIT_OID_HEXSZ,1);

    // Call userspace method implementation of odb operation. The user hopefully
    // has overridden the method in a derived class.
    {
        derived_context<php_odb_backend_object> ctx(LOOKUP_OBJECT(php_odb_backend_object,thisobj));

        if (call_user_function(NULL,&thisobj,&fname,&retval,2,params
                ZTS_MEMBER_CC(ctx.object()->zts)) == FAILURE)
        {
            result = GIT_ERROR;
        }
        else {
            // Return values to caller. We have to copy the data to use a persistent
            // memory buffer that git2 can free.
            convert_to_string(&retval);
            data = pemalloc(Z_STRLEN(retval),1);
            memcpy(data,Z_STRVAL(retval),Z_STRLEN(retval));
            *datap = data;
            *sizep = Z_STRLEN(retval);
            convert_to_long(ztype);
            *typep = (git_otype)Z_LVAL_P(ztype);
        }
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zoid);
    zval_ptr_dtor(&ztype);
    return result;
}

/*static*/ int php_odb_backend_object::read_prefix(git_oid* oidp,void** datap,
    size_t* sizep,git_otype* typep,git_odb_backend* backend,const git_oid* prefix,
    size_t nbits)
{
    zval fname;
    zval retval;
    zval* zfull;
    zval* ztype;
    zval* zprefix;
    zval* thisobj = EXTRACT_THISOBJ(backend);
    char buffer[GIT_OID_HEXSZ+1];
    int result = GIT_OK;
    void* data;

    // Allocate/initialize zvals.
    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zfull);
    ALLOC_INIT_ZVAL(ztype);
    ALLOC_INIT_ZVAL(zprefix);

    // Assign values to function name and by-value arguments.
    ZVAL_STRING(&fname,"read_prefix",1);
    git_oid_tostr(buffer,sizeof(buffer),prefix);
    ZVAL_STRING(zprefix,buffer,1);

    zval* params[] = { zfull, ztype, zprefix };

    // Call userspace method implementation of odb operation. The user hopefully
    // has overridden the method in a derived class.
    {
        derived_context<php_odb_backend_object> ctx(LOOKUP_OBJECT(php_odb_backend_object,thisobj));

        if (call_user_function(NULL,&thisobj,&fname,&retval,3,params
                ZTS_MEMBER_CC(ctx.object()->zts)) == FAILURE)
        {
            result = GIT_ERROR;
        }
        else {
            // Return values to caller. We have to copy the data to use a persistent
            // memory buffer that git2 can free.
            convert_to_string(&retval);
            data = pemalloc(Z_STRLEN(retval),1);
            memcpy(data,Z_STRVAL(retval),Z_STRLEN(retval));
            *datap = data;
            *sizep = Z_STRLEN(retval);
            convert_to_long(ztype);
            *typep = (git_otype)Z_LVAL_P(ztype);
            convert_to_string(zfull);
            convert_oid_fromstr(oidp,Z_STRVAL_P(zfull),Z_STRLEN_P(zfull));
        }
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zfull);
    zval_ptr_dtor(&ztype);
    zval_ptr_dtor(&zprefix);
    return result;
}

/*static*/ int php_odb_backend_object::read_header(size_t* sizep,git_otype* typep,
    git_odb_backend* backend,const git_oid* oid)
{
    zval fname;
    zval retval;
    zval* zsize;
    zval* ztype;
    zval* zoid;
    zval* thisobj = EXTRACT_THISOBJ(backend);
    char buffer[GIT_OID_HEXSZ+1];
    int result = GIT_OK;

    // Allocate/initialize zvals.
    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zsize);
    ALLOC_INIT_ZVAL(ztype);
    ALLOC_INIT_ZVAL(zoid);

    // Assign value to function name.
    ZVAL_STRING(&fname,"read_header",1);
    git_oid_tostr(buffer,sizeof(buffer),oid);
    ZVAL_STRING(zoid,buffer,1);

    zval* params[] = { zsize, ztype, zoid };

    // Call userspace method implementation of odb operation. The user hopefully
    // has overridden the method in a derived class.
    {
        derived_context<php_odb_backend_object> ctx(LOOKUP_OBJECT(php_odb_backend_object,thisobj));

        if (call_user_function(NULL,&thisobj,&fname,&retval,3,params
                ZTS_MEMBER_CC(ctx.object()->zts)) == FAILURE)
        {
            result = GIT_ERROR;
        }
        else {
            // Return values to caller.
            convert_to_long(zsize);
            convert_to_long(ztype);
            *sizep = Z_LVAL_P(zsize);
            *typep = (git_otype)Z_LVAL_P(ztype);
        }
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zsize);
    zval_ptr_dtor(&ztype);
    zval_ptr_dtor(&zoid);
    return result;
}

/*static*/ int php_odb_backend_object::write(git_odb_backend* backend,
    const git_oid* oid,const void* data,size_t size,git_otype type)
{
    zval fname;
    zval retval;
    zval* zoid;
    zval* zpayload;
    zval* ztype;
    zval* thisobj = EXTRACT_THISOBJ(backend);
    char buffer[GIT_OID_HEXSZ+1];
    int result = GIT_OK;

    // Allocate/initialize zvals.
    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    MAKE_STD_ZVAL(zoid);
    MAKE_STD_ZVAL(zpayload);
    MAKE_STD_ZVAL(ztype);

    // Assign values to function name and by-value arguments.
    ZVAL_STRING(&fname,"write",1);
    git_oid_tostr(buffer,sizeof(buffer),oid);
    ZVAL_STRING(zoid,buffer,1);
    ZVAL_STRINGL(zpayload,(const char*)data,size,1);
    ZVAL_LONG(ztype,type);

    zval* params[] = { zoid, zpayload, ztype };

    // Call userspace method implementation of odb operation. The user hopefully
    // has overridden the method in a derived class.
    {
        derived_context<php_odb_backend_object> ctx(LOOKUP_OBJECT(php_odb_backend_object,thisobj));

        if (call_user_function(NULL,&thisobj,&fname,&retval,3,params
                ZTS_MEMBER_CC(ctx.object()->zts)) == FAILURE)
        {
            result = GIT_ERROR;
        }
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zoid);
    zval_ptr_dtor(&zpayload);
    zval_ptr_dtor(&ztype);
    return result;
}

/*static*/ int php_odb_backend_object::writestream(git_odb_stream** streamp,
    git_odb_backend* backend,git_off_t size,git_otype type)
{
    zval fname;
    zval retval;
    zval* zsize;
    zval* ztype;
    zval* thisobj = EXTRACT_THISOBJ(backend);
    int result = GIT_OK;

    // Allocate/initialize zvals.
    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    MAKE_STD_ZVAL(zsize);
    MAKE_STD_ZVAL(ztype);

    // Assign values to function name and by-value arguments.
    ZVAL_STRING(&fname,"writestream",1);
    ZVAL_LONG(zsize,size);
    ZVAL_LONG(ztype,type);

    zval* params[] = { zsize, ztype };

    // Call userspace method implementation of odb operation. The user hopefully
    // has overridden the method in a derived class.
    {
        derived_context<php_odb_backend_object> ctx(LOOKUP_OBJECT(php_odb_backend_object,thisobj));

        if (call_user_function(NULL,&thisobj,&fname,&retval,2,params
                ZTS_MEMBER_CC(ctx.object()->zts)) == FAILURE)
        {
            result = GIT_ERROR;
        }
        else {
            // Make sure returned zval is an object derived from GitODBStream.
            if (Z_TYPE(retval) != IS_OBJECT
                || !is_subclass_of(Z_OBJCE(retval),class_entry[php_git2_odb_stream_obj]))
            {
                php_error(
                    E_ERROR,
                    "GitODBBackend::writestream(): return value must be an object "
                    "derived from GitODBStream");
                return GIT_ERROR;
            }

            // Now create the custom stream backing. The stream tracks this object
            // to keep it alive and provide access to the GitODBBackend derivation.
            php_odb_stream_object* sobj = LOOKUP_OBJECT(php_odb_stream_object,&retval);
            sobj->create_custom_stream(&retval,GIT_STREAM_WRONLY,thisobj);

            // The libgit2 implementation expects the custom backing to provide a
            // reference to the ODB backend.
            sobj->stream->backend = backend;

            // Assign the git_odb_stream to the out parameter. This is safe since
            // the instance holds its own zval that references the object backing,
            // meaning the git_odb_stream will survive until its free() function is
            // called.
            *streamp = sobj->stream;
        }
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zsize);
    zval_ptr_dtor(&ztype);
    return result;
}

/*static*/ int php_odb_backend_object::readstream(git_odb_stream** streamp,
    git_odb_backend* backend,const git_oid* oid)
{
    zval fname;
    zval retval;
    zval* zbuf;
    zval* thisobj = EXTRACT_THISOBJ(backend);
    char buffer[GIT_OID_HEXSZ+1];
    int result = GIT_OK;

    // Allocate/initialize zvals.
    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    MAKE_STD_ZVAL(zbuf);

    // Assign values to function name and by-value arguments.
    ZVAL_STRING(&fname,"readstream",1);
    git_oid_tostr(buffer,sizeof(buffer),oid);
    ZVAL_STRINGL(zbuf,buffer,GIT_OID_HEXSZ,1);

    zval* params[] = { zbuf };

    // Call userspace method implementation of odb operation. The user hopefully
    // has overridden the method in a derived class.
    {
        derived_context<php_odb_backend_object> ctx(LOOKUP_OBJECT(php_odb_backend_object,thisobj));

        if (call_user_function(NULL,&thisobj,&fname,&retval,1,params
                ZTS_MEMBER_CC(ctx.object()->zts)) == FAILURE)
        {
            result = GIT_ERROR;
        }
        else {
            // Make sure returned zval is an object derived from GitODBStream.
            if (Z_TYPE(retval) != IS_OBJECT
                || !is_subclass_of(Z_OBJCE(retval),class_entry[php_git2_odb_stream_obj]))
            {
                php_error(
                    E_ERROR,
                    "GitODBBackend::readstream(): return value must be an object "
                    "derived from GitODBStream");
                return GIT_ERROR;
            }

            // Now create the custom stream backing.
            php_odb_stream_object* sobj = LOOKUP_OBJECT(php_odb_stream_object,&retval);
            sobj->create_custom_stream(&retval,GIT_STREAM_RDONLY,thisobj);

            // Help out the implementation by providing a reference to the ODB
            // backend.
            sobj->stream->backend = backend;

            // Assign the git_odb_stream to the out parameter. This is safe
            // since the instance holds its own zval that references the object
            // backing, meaning the git_odb_stream will survive until its free()
            // function is called.
            *streamp = sobj->stream;
        }
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zbuf);
    return result;
}

/*static*/ int php_odb_backend_object::exists(git_odb_backend* backend,
    const git_oid* oid)
{
    zval fname;
    zval retval;
    zval* zoid;
    zval* thisobj = EXTRACT_THISOBJ(backend);
    char buffer[GIT_OID_HEXSZ+1];
    int result = GIT_OK;

    // Allocate/initialize zvals.
    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    MAKE_STD_ZVAL(zoid);

    // Assign values to function name and by-value arguments.
    ZVAL_STRING(&fname,"exists",1);
    git_oid_tostr(buffer,sizeof(buffer),oid);
    ZVAL_STRING(zoid,buffer,1);

    zval* params[] = { zoid };

    // Call userspace method implementation of odb operation. The user hopefully
    // has overridden the method in a derived class.
    {
        derived_context<php_odb_backend_object> ctx(LOOKUP_OBJECT(php_odb_backend_object,thisobj));

        if (call_user_function(NULL,&thisobj,&fname,&retval,1,params
                ZTS_MEMBER_CC(ctx.object()->zts)) == FAILURE)
        {
            result = GIT_ERROR;
        }
        else {
            // Exists() must return a traditional Boolean value.
            result = Z_BVAL(retval) ? 1 : 0;
        }
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zoid);
    return result;
}

/*static*/ int php_odb_backend_object::exists_prefix(git_oid* oidp,
    git_odb_backend* backend,const git_oid* prefix,size_t nbits)
{
    zval fname;
    zval retval;
    zval* zfull;
    zval* zoid;
    zval* thisobj = EXTRACT_THISOBJ(backend);
    char buffer[GIT_OID_HEXSZ+1];
    int result = GIT_OK;

    // Allocate/initialize zvals.
    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zfull);
    MAKE_STD_ZVAL(zoid);

    // Assign values to function name and by-value arguments.
    ZVAL_STRING(&fname,"exists",1);
    git_oid_tostr(buffer,sizeof(buffer),prefix);
    ZVAL_STRING(zoid,buffer,1);

    zval* params[] = { zfull, zoid };

    // Call userspace method implementation of odb operation. The user hopefully
    // has overridden the method in a derived class.
    {
        derived_context<php_odb_backend_object> ctx(LOOKUP_OBJECT(php_odb_backend_object,thisobj));

        if (call_user_function(NULL,&thisobj,&fname,&retval,2,params
                ZTS_MEMBER_CC(ctx.object()->zts)) == FAILURE)
        {
            result = GIT_ERROR;
        }
        else {
            // Return values to caller.
            convert_to_string(zfull);
            convert_oid_fromstr(oidp,Z_STRVAL_P(zfull),Z_STRLEN_P(zfull));
            convert_to_boolean(&retval);
            if (Z_BVAL(retval)) {
                // Function needs to return zero when the element is found.
                result = 0;
            }
            else {
                result = GIT_ENOTFOUND;
            }
        }
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zfull);
    zval_ptr_dtor(&zoid);
    return result;
}

/*static*/ int php_odb_backend_object::refresh(git_odb_backend* backend)
{
    zval fname;
    zval retval;
    zval* thisobj = EXTRACT_THISOBJ(backend);
    int result = GIT_OK;

    // Allocate/initialize zvals.
    INIT_ZVAL(fname);
    INIT_ZVAL(retval);

    // Assign values to function name and by-value arguments.
    ZVAL_STRING(&fname,"refresh",1);

    // Call userspace method implementation of odb operation. The user hopefully
    // has overridden the method in a derived class.
    {
        derived_context<php_odb_backend_object> ctx(LOOKUP_OBJECT(php_odb_backend_object,thisobj));

        if (call_user_function(NULL,&thisobj,&fname,&retval,0,NULL
                ZTS_MEMBER_CC(ctx.object()->zts)) == FAILURE)
        {
            result = GIT_ERROR;
        }
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    return result;
}

/*static*/ int php_odb_backend_object::foreach(git_odb_backend* backend,
    git_odb_foreach_cb cb,void* payload)
{
    int result = GIT_OK;
    zval* thisobj = EXTRACT_THISOBJ(backend);
    zval fname;
    zval retval;
    zval* zclosure;
    zval* zpayload;

    // Create zvals for method call. The payload argument is always a NULL zval
    // from this context. However this fact should be mostly transparent to the
    // user since they will just pass it along in their implementation none the
    // wiser. NOTE: the only time the payload zval is directly useful is if the
    // PHP engine calls the "for_each()" method, not this function.

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ZVAL_STRING(&fname,"for_each",1);
    MAKE_STD_ZVAL(zclosure);
    ALLOC_INIT_ZVAL(zpayload);

    {
        php_closure_object* closureInfo;
        zval* params[] = { zclosure, zpayload };
        foreach_callback_info callbackInfo = { cb, payload };
        derived_context<php_odb_backend_object> ctx(LOOKUP_OBJECT(php_odb_backend_object,thisobj));
#ifdef ZTS
        TSRMLS_D = ZTS_MEMBER_CC(ctx.object()->zts); // Make in-scope for call to object_init_ex().
#endif

        // Set up the closure to call an internal function that will handle
        // calling the function and passing the payload.
        object_init_ex(zclosure,php_git2::class_entry[php_git2_closure_obj]);
        closureInfo = reinterpret_cast<php_closure_object*>
            (zend_objects_get_address(zclosure TSRMLS_CC));
        closureInfo->func.type = ZEND_INTERNAL_FUNCTION;
        closureInfo->func.common.function_name = "php-git2 foreach internal callback";
        closureInfo->func.common.fn_flags |= ZEND_ACC_CLOSURE;
        closureInfo->func.common.num_args = 2;
        closureInfo->func.common.required_num_args = 2;
        closureInfo->func.common.scope = php_git2::class_entry[php_git2_closure_obj];
        closureInfo->func.internal_function.handler = ZEND_FN(backend_foreach_callback);
        closureInfo->hasPayload = true;
        closureInfo->payload = &callbackInfo;

        // Call PHP object instance method.
        if (call_user_function(NULL,&thisobj,&fname,&retval,2,params TSRMLS_CC) == FAILURE) {
            result = GIT_ERROR;
        }
    }

    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zclosure);
    zval_ptr_dtor(&zpayload);

    return result;
}

/*static*/ int php_odb_backend_object::writepack(git_odb_writepack** writepackp,
    git_odb_backend* backend,git_odb* odb,git_transfer_progress_cb progress_cb,
    void* progress_payload)
{
    int result = GIT_OK;

    return result;
}

/*static*/ void php_odb_backend_object::free(git_odb_backend* backend)
{
    // Set backend to null in internal storage (just in case). Then explicitly
    // call the destructor on the custom backend. Finally free the block of
    // memory that holds the custom backend.

    git_odb_backend_php* wrapper = EXTRACT_BACKEND(backend);
    php_odb_backend_object* obj;

    // Make sure object buckets still exist to lookup object (in case the
    // destructor was already called). This happens when free() is called after
    // PHP has finished cleaning up all objects (but before all variables, which
    // may have references to git2 objects that reference this PHP object).
    if (EG(objects_store).object_buckets != nullptr) {
        // Call userspace free() method on the object. It may not necessarily be
        // implemented since we didn't check for it when the git_odb_backend_php
        // object was created. If not, we just let the callback fail.

        zval fname;
        zval retval;

        obj = LOOKUP_OBJECT(php_odb_backend_object,wrapper->thisobj);

        INIT_ZVAL(fname);
        INIT_ZVAL(retval);
        ZVAL_STRING(&fname,"free",1);
        {
            derived_context<php_odb_backend_object> ctx(obj);

            call_user_function(NULL,&wrapper->thisobj,&fname,&retval,0,NULL
                ZTS_MEMBER_CC(obj->zts));
        }
        zval_dtor(&fname);
        zval_dtor(&retval);

        // Unassign backend from the object since it is about to get destroyed.
        obj->backend = nullptr;
    }

    wrapper->~git_odb_backend_php();
    efree(backend);
}

// php_odb_backend_object::git_odb_backend_php

php_odb_backend_object::
git_odb_backend_php::git_odb_backend_php(zval* zv)
{
    // Blank out the base class (which is the structure defined in the git2
    // headers).
    memset(this,0,sizeof(struct git_odb_backend));

    // Assign zval to keep object alive while backend is in use in the library.
    Z_ADDREF_P(zv);
    thisobj = zv;

    zend_class_entry* ce = Z_OBJCE_P(thisobj);
    version = GIT_ODB_BACKEND_VERSION;

    // Every custom backend gets the free function (whether it is overloaded in
    // userspace or not).
    free = php_odb_backend_object::free;

    // We now must select which functions we are going to include in the
    // backend. We do this by determining which ones were overloaded.
    if (is_method_overridden(ce,"read",sizeof("read"))) {
        read = php_odb_backend_object::read;
    }
    if (is_method_overridden(ce,"read_prefix",sizeof("read_prefix"))) {
        read_prefix = php_odb_backend_object::read_prefix;
    }
    if (is_method_overridden(ce,"read_header",sizeof("read_header"))) {
        read_header = php_odb_backend_object::read_header;
    }
    if (is_method_overridden(ce,"write",sizeof("write"))) {
        write = php_odb_backend_object::write;
    }
    if (is_method_overridden(ce,"writestream",sizeof("writestream"))) {
        writestream = php_odb_backend_object::writestream;
    }
    if (is_method_overridden(ce,"readstream",sizeof("readstream"))) {
        readstream = php_odb_backend_object::readstream;
    }
    if (is_method_overridden(ce,"exists",sizeof("exists"))) {
        exists = php_odb_backend_object::exists;
    }
    if (is_method_overridden(ce,"exists_prefix",sizeof("exists_prefix"))) {
        exists_prefix = php_odb_backend_object::exists_prefix;
    }
    if (is_method_overridden(ce,"refresh",sizeof("refresh"))) {
        refresh = php_odb_backend_object::refresh;
    }
    if (is_method_overridden(ce,"for_each",sizeof("for_each"))) {
        foreach = php_odb_backend_object::foreach;
    }
    if (is_method_overridden(ce,"writepack",sizeof("writepack"))) {
        writepack = php_odb_backend_object::writepack;
    }
}

php_odb_backend_object::
git_odb_backend_php::~git_odb_backend_php()
{
    // Free object zval.
    zval_ptr_dtor(&thisobj);
}

// Implementation of custom class handlers

zval* odb_backend_read_property(zval* obj,zval* prop,int type,const zend_literal* key TSRMLS_DC)
{
    zval* ret;
    zval** zfind;
    zval* tmp_prop = nullptr;
    const char* str;
    php_odb_backend_object* backendWrapper = LOOKUP_OBJECT(php_odb_backend_object,obj);
    git_odb_backend* backend = backendWrapper->backend;

    // Ensure deep copy of member zval.
    if (Z_TYPE_P(prop) != IS_STRING) {
        MAKE_STD_ZVAL(tmp_prop);
        *tmp_prop = *prop;
        INIT_PZVAL(tmp_prop);
        zval_copy_ctor(tmp_prop);
        convert_to_string(tmp_prop);
        prop = tmp_prop;
        key = NULL;
    }

    // Handle special properties of the git_odb_backend.

    str = Z_STRVAL_P(prop);
    if (strcmp(str,"version") == 0 && backend != nullptr) {
        ALLOC_INIT_ZVAL(ret);
        ZVAL_LONG(ret,backend->version);
        Z_DELREF_P(ret);
    }
    else if (strcmp(str,"odb") == 0 && backend != nullptr) {
        if (key != nullptr) {
            ret = zend_hash_quick_find(Z_OBJPROP_P(obj),"odb",sizeof("odb"),key->hash_value,(void**)&zfind) != FAILURE
                ? *zfind : nullptr;
        }
        else {
            ret = zend_hash_find(Z_OBJPROP_P(obj),"odb",sizeof("odb"),(void**)&zfind) != FAILURE
                ? *zfind : nullptr;
        }

        if (ret == nullptr) {
            ALLOC_INIT_ZVAL(ret);
            if (backend->odb != nullptr) {
                php_git_odb* rsrc;
                if (backendWrapper->owner == nullptr) {
                    // Create new resource that will not free the git_odb.
                    rsrc = php_git2_create_resource<php_git_odb_nofree>();
                    rsrc->set_handle(backend->odb);
                }
                else {
                    // Use owner resource. We must increment the refcount so the
                    // resource remains valid for the user.
                    rsrc = backendWrapper->owner;
                    rsrc->up_ref();
                }
                zend_register_resource(ret,rsrc,php_git_odb::resource_le() TSRMLS_CC);

                if (key != nullptr) {
                    zend_hash_quick_add(Z_OBJPROP_P(obj),"odb",sizeof("odb"),key->hash_value,
                        &ret,sizeof(zval*),NULL);
                }
                else {
                    zend_hash_add(Z_OBJPROP_P(obj),"odb",sizeof("odb"),&ret,sizeof(zval*),NULL);
                }
            }
            Z_DELREF_P(ret);
        }
    }
    else {
        ret = (*std_object_handlers.read_property)(obj,prop,type,key TSRMLS_CC);
    }

    if (tmp_prop != nullptr) {
        Z_ADDREF_P(ret);
        zval_ptr_dtor(&tmp_prop);
        Z_DELREF_P(ret);
    }

    return ret;
}

void odb_backend_write_property(zval* obj,zval* prop,zval* value,const zend_literal* key TSRMLS_DC)
{
    zval* tmp_prop = nullptr;
    const char* str;

    // Ensure deep copy of member zval.
    if (Z_TYPE_P(prop) != IS_STRING) {
        MAKE_STD_ZVAL(tmp_prop);
        *tmp_prop = *prop;
        INIT_PZVAL(tmp_prop);
        zval_copy_ctor(tmp_prop);
        convert_to_string(tmp_prop);
        prop = tmp_prop;
        key = NULL;
    }

    str = Z_STRVAL_P(prop);
    if (strcmp(str,"version") == 0 || strcmp(str,"odb") == 0) {
        php_error(E_ERROR,"GitODBBackend::__set: the '%s' property is read-only",str);
    }
    else {
        (*std_object_handlers.write_property)(obj,prop,value,key);
    }

    if (tmp_prop != nullptr) {
        zval_ptr_dtor(&tmp_prop);
    }
}

int odb_backend_has_property(zval* obj,zval* prop,int chk_type,const zend_literal* key TSRMLS_DC)
{
    int result;
    zval* tmp_prop = nullptr;
    const char* src;
    git_odb_backend* backend = LOOKUP_OBJECT(php_odb_backend_object,obj)->backend;

    // Ensure deep copy of member zval.
    if (Z_TYPE_P(prop) != IS_STRING) {
        MAKE_STD_ZVAL(tmp_prop);
        *tmp_prop = *prop;
        INIT_PZVAL(tmp_prop);
        zval_copy_ctor(tmp_prop);
        convert_to_string(tmp_prop);
        prop = tmp_prop;
        key = NULL;
    }

    src = Z_STRVAL_P(prop);
    if (strcmp(src,"version") == 0) {
        result = (backend != nullptr);
    }
    else if (strcmp(src,"odb") == 0) {
        if (chk_type == 2) {
            result = (backend != nullptr);
        }
        else {
            result = (backend != nullptr && backend->odb != nullptr);
        }
    }
    else {
        result = (*std_object_handlers.has_property)(obj,prop,chk_type,key TSRMLS_CC);
    }

    if (tmp_prop != nullptr) {
        zval_ptr_dtor(&tmp_prop);
    }

    return result;
}

// Implementation of class methods

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
    if (object->backend == nullptr || object->backend->read == nullptr) {
        php_error(E_ERROR,"GitODBBackend::read(): method is not available");
        return;
    }

    // Disallow calling this method from a derived context as this would create
    // an infinite loop.
    if (object->derivedCall) {
        php_error(E_ERROR,"GitODBBackend::read(): do not call parent method from derived context");
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
    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }
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

    // Disallow calling this method from a derived context as this would create
    // an infinite loop.
    if (object->derivedCall) {
        php_error(E_ERROR,"GitODBBackend::read_prefix(): do not call parent method from derived context");
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
        int retval;
        convert_oid_fromstr(&prefix,strOid,strOidLen);
        retval = object->backend->read_prefix(&full,&data,&size,&type,object->backend,&prefix,strOidLen);
        if (retval < 0) {
            php_git2::git_error(retval);
        }
    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }
        return;
    }

    // Copy the result into the return zval. Then set the out parameters.
    // Finally we have to free the buffer allocated by the call to read().
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

    // Disallow calling this method from a derived context as this would create
    // an infinite loop.
    if (object->derivedCall) {
        php_error(E_ERROR,"GitODBBackend::read_header(): do not call parent method from derived context");
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
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }
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

    // Disallow calling this method from a derived context as this would create
    // an infinite loop.
    if (object->derivedCall) {
        php_error(E_ERROR,"GitODBBackend::write(): do not call parent method from derived context");
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
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }
        return;
    }

    RETURN_TRUE;
}

PHP_METHOD(GitODBBackend,writestream)
{
    long offset;
    long objectType;
    git_odb_stream* outstream = nullptr;
    php_odb_backend_object* object = LOOKUP_OBJECT(php_odb_backend_object,getThis());

    // Backend must be created and write function must be available.
    if (object->backend == nullptr || object->backend->writestream == nullptr) {
        php_error(E_ERROR,"GitODBBackend::writestream(): method is not available");
        return;
    }

    // Disallow calling this method from a derived context as this would create
    // an infinite loop.
    if (object->derivedCall) {
        php_error(E_ERROR,"GitODBBackend::writestream(): do not call parent method from derived context");
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
    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }
        return;
    }

    if (outstream != nullptr) {
        // Create GitODBStream object to wrap git_odb_stream.
        php_git2_make_odb_stream(return_value,outstream,nullptr TSRMLS_CC);
    }
}

PHP_METHOD(GitODBBackend,readstream)
{
    char* oidstr;
    int oidstr_len;
    git_oid oid;
    git_odb_stream* outstream = nullptr;
    php_odb_backend_object* object = LOOKUP_OBJECT(php_odb_backend_object,getThis());

    // Backend must be created and write function must be available.
    if (object->backend == nullptr || object->backend->readstream == nullptr) {
        php_error(E_ERROR,"GitODBBackend::readstream(): method is not available");
        return;
    }

    // Disallow calling this method from a derived context as this would create
    // an infinite loop.
    if (object->derivedCall) {
        php_error(E_ERROR,"GitODBBackend::readstream(): do not call parent method from derived context");
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
    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }
        return;
    }

    if (outstream != nullptr) {
        // Create GitODBStream object to wrap git_odb_stream.
        php_git2_make_odb_stream(return_value,outstream,nullptr TSRMLS_CC);
    }
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

    // Disallow calling this method from a derived context as this would create
    // an infinite loop.
    if (object->derivedCall) {
        php_error(E_ERROR,"GitODBBackend::exists(): do not call parent method from derived context");
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

    // Disallow calling this method from a derived context as this would create
    // an infinite loop.
    if (object->derivedCall) {
        php_error(E_ERROR,"GitODBBackend::exists_prefix(): do not call parent method from derived context");
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

    // Disallow calling this method from a derived context as this would create
    // an infinite loop.
    if (object->derivedCall) {
        php_error(E_ERROR,"GitODBBackend::refresh(): do not call parent method from derived context");
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
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }
        return;
    }
}

PHP_METHOD(GitODBBackend,for_each)
{
    php_odb_backend_object* object = LOOKUP_OBJECT(php_odb_backend_object,getThis());

    // Backend must be created and the function must be implemented.
    if (object->backend == nullptr || object->backend->foreach == nullptr) {
        php_error(E_ERROR,"GitODBBackend::for_each(): method is not available");
        return;
    }

    // Disallow calling this method from a derived context as this would create
    // an infinite loop.
    if (object->derivedCall) {
        php_error(E_ERROR,"GitODBBackend::for_each(): do not call parent method from derived context");
        return;
    }

    int retval;
    php_callback_sync callback;
    php_callback_handler<odb_foreach_callback> handler;

    if (zend_get_parameters(0,2,callback.byref_php(1),callback.byref_php(2)) == FAILURE) {
        php_error(E_ERROR,"GitODBBackend::for_each(): method expects 2 arguments");
        return;
    }

    // Call the underlying function.
    try {
        retval = object->backend->foreach(object->backend,
            handler.byval_git2(),callback.byval_git2());
    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }
        return;
    }

    RETURN_LONG(static_cast<long>(retval));
}

PHP_METHOD(GitODBBackend,writepack)
{
    zval* thisobj = getThis();
    php_odb_backend_object* object = LOOKUP_OBJECT(php_odb_backend_object,thisobj);

    // Backend must be created and the function must be implemented.
    if (object->backend == nullptr || object->backend->writepack == nullptr) {
        php_error(E_ERROR,"GitODBBackend::writepack(): method is not available");
        return;
    }

    // Disallow calling this method from a derived context as this would create
    // an infinite loop.
    if (object->derivedCall) {
        php_error(E_ERROR,"GitODBBackend::writepack(): do not call parent method from derived context");
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
        int retval;
        retval = object->backend->writepack(&wp,object->backend,object->backend->odb,
            handler.byval_git2(),callback->byval_git2());
        if (retval < 0) {
            php_git2::git_error(retval);
        }

        // Create return value out of the writepack and callback. The callback
        // will be managed by the GitODBWritepack object.
        php_git2_make_odb_writepack(return_value,wp,callback,thisobj,object->owner TSRMLS_CC);
    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }
        return;
    }
}

PHP_METHOD(GitODBBackend,free)
{
    php_odb_backend_object* object = LOOKUP_OBJECT(php_odb_backend_object,getThis());

    // Backend must be created and the function must be implemented.
    if (object->backend == nullptr || object->backend->free == nullptr) {
        php_error(E_ERROR,"GitODBBackend::free(): method is not available");
        return;
    }

    // Disallow calling this method from a derived context as this would create
    // an infinite loop.
    if (object->derivedCall) {
        php_error(E_ERROR,"GitODBBackend::free(): do not call parent method from derived context");
        return;
    }

    // We really don't want people to be able to shoot themselves in the foot
    // with this one (after all, crashing the Web server is generally not
    // considered a good thing to do). We only call free if the owner is not
    // set. If the owner is set then we silently do nothing (the user really has
    // no business calling this anyway since our destructor will get it).

    if (object->owner == nullptr) {
        object->backend->free(object->backend);

        // Avoid double free's later on.
        object->backend = nullptr;
    }
}

ZEND_FUNCTION(backend_foreach_callback)
{
    int retval;
    char* strOID;
    int strOIDSize;
    git_oid oid;
    zval* zdummyPayload = nullptr;
    php_closure_object* object = LOOKUP_OBJECT(php_closure_object,getThis());
    const foreach_callback_info* info = reinterpret_cast<const foreach_callback_info*>(object->payload);

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s|z",&strOID,&strOIDSize,&zdummyPayload) == FAILURE) {
        return;
    }

    // Convert string representation to git2 OID struct.
    convert_oid_fromstr(&oid,strOID,strOIDSize);

    // Invoke callback. The return value is important so we'll pass it along.
    retval = (*info->callback)(&oid,info->payload);

    RETVAL_LONG(static_cast<long>(retval));
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
