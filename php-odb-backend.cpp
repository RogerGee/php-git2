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

struct transfer_progress_callback_info
{
    git_transfer_progress_cb callback;
    void* payload;
};

static ZEND_FUNCTION(backend_foreach_callback);
static ZEND_FUNCTION(backend_transfer_progress_callback);

// Class method entries

static PHP_EMPTY_METHOD(GitODBBackend,read);
static PHP_EMPTY_METHOD(GitODBBackend,read_prefix);
static PHP_EMPTY_METHOD(GitODBBackend,read_header);
static PHP_EMPTY_METHOD(GitODBBackend,write);
static PHP_EMPTY_METHOD(GitODBBackend,writestream);
static PHP_EMPTY_METHOD(GitODBBackend,readstream);
static PHP_EMPTY_METHOD(GitODBBackend,exists);
static PHP_EMPTY_METHOD(GitODBBackend,exists_prefix);
static PHP_EMPTY_METHOD(GitODBBackend,refresh);
static PHP_EMPTY_METHOD(GitODBBackend,for_each);
static PHP_EMPTY_METHOD(GitODBBackend,writepack);

zend_function_entry php_git2::odb_backend_methods[] = {
    PHP_ME(GitODBBackend,read,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend,read_prefix,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend,read_header,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend,write,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend,writestream,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend,readstream,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend,exists,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend,exists_prefix,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend,refresh,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend,for_each,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBBackend,writepack,NULL,ZEND_ACC_PUBLIC)
    PHP_FE_END
};

// Implementation of php_odb_backend_object

/*static*/ zend_object_handlers php_odb_backend_object::handlers;
php_odb_backend_object::php_odb_backend_object(zend_class_entry* ce TSRMLS_DC):
    backend(nullptr), kind(unset), owner(nullptr), zts(TSRMLS_C)
{
    zend_object_std_init(this,ce TSRMLS_CC);
    object_properties_init(this,ce);
}

php_odb_backend_object::~php_odb_backend_object()
{
    // AFAIK we never call the free() function on git_odb_backend objects that
    // were returned from a call that referenced a git_odb NOR those applied to
    // a git_odb's list of backends. This is the case when kind is either
    // 'conventional' or 'custom'.

    if (kind == user) {
        backend->free(backend);
    }

    // Free owner ODB in case it is referenced.
    if (owner != nullptr) {
        git2_resource_base::free_recursive(owner);
    }

    zend_object_std_dtor(this ZTS_MEMBER_CC(this->zts));
}

void php_odb_backend_object::create_custom_backend(zval* zobj)
{
    // NOTE: the zval should be any zval that points to an object with 'this' as
    // its backing (i.e. result of zend_objects_get_address()). This is used by
    // the implementation to obtain class entry info for the class that was used
    // to create the object AND to keep the PHP object alive while it is being
    // used by git2.

    // Make sure object doesn't already have a backing. This would imply it is
    // in use already in an ODB.
    if (backend != nullptr) {
        throw php_git2_fatal_exception("Cannot create custom ODB backend: object already in use");
    }

    // Set kind to 'custom'.
    kind = custom;

    // Create custom backend. Custom backends are always passed off to git2, so
    // we are not responsible for calling its free function.
    backend = new (emalloc(sizeof(git_odb_backend_php))) git_odb_backend_php(zobj ZTS_MEMBER_CC(zts));

    // Custom ODB backends never have a direct owner. We always assume the
    // would-be owner is kept alive circularly since the custom backen stores a
    // reference to this object.
    assign_owner(nullptr);
}

void php_odb_backend_object::assign_owner(php_git_odb* newOwner)
{
    // Free existing owner (if any).
    if (owner != nullptr) {
        git2_resource_base::free_recursive(owner);
    }

    owner = newOwner;

    // Increment owner refcount to prevent the ODB from freeing while the
    // backend object is in use.
    if (owner != nullptr) {
        owner->up_ref();
    }
}

/*static*/ void php_odb_backend_object::init(zend_class_entry* ce)
{
    handlers.read_property = odb_backend_read_property;
    handlers.write_property = odb_backend_write_property;
    handlers.has_property = odb_backend_has_property;

    UNUSED(ce);
}

/*static*/ int php_odb_backend_object::read(void** datap,size_t* sizep,
    git_otype* typep,git_odb_backend* backend,const git_oid* oid)
{
    int result;
    zval* zoid;
    zval* ztype;
    method_wrapper method("read",backend);

    // Prepare parameters.

    ALLOC_INIT_ZVAL(ztype);
    ALLOC_INIT_ZVAL(zoid);

    convert_oid(zoid,oid);

    // Call userspace method implementation corresponding to odb operation.

    zval* params[] = { ztype, zoid };

    result = method.call(2,params);
    if (result == GIT_OK) {
        void* data;
        zval* retval = method.retval();

        // Copy the returned data to a persistent memory buffer that git2 can
        // free later on.
        convert_to_string(retval);
        data = pemalloc(Z_STRLEN_P(retval),1);
        memcpy(data,Z_STRVAL_P(retval),Z_STRLEN_P(retval));

        // Return values to caller.
        *datap = data;
        *sizep = Z_STRLEN_P(retval);
        convert_to_long(ztype);
        *typep = (git_otype)Z_LVAL_P(ztype);
    }

    // Cleanup zvals.

    zval_ptr_dtor(&zoid);
    zval_ptr_dtor(&ztype);

    return result;
}

/*static*/ int php_odb_backend_object::read_prefix(git_oid* oidp,void** datap,
    size_t* sizep,git_otype* typep,git_odb_backend* backend,const git_oid* prefix,
    size_t len)
{
    int result;
    zval* zfull;
    zval* ztype;
    zval* zprefix;
    method_wrapper method("read_prefix",backend);

    // Allocate/set zvals.

    ALLOC_INIT_ZVAL(zfull);
    ALLOC_INIT_ZVAL(ztype);
    ALLOC_INIT_ZVAL(zprefix);

    convert_oid_prefix(zprefix,prefix,len);

    // Call userspace method implementation corresponding to odb operation.

    zval* params[] = { zfull, ztype, zprefix };

    result = method.call(3,params);
    if (result == GIT_OK) {
        void* data;
        zval* retval = method.retval();

        // Copy the returned data to a persistent memory buffer that git2 can
        // free later on.
        convert_to_string(retval);
        data = pemalloc(Z_STRLEN_P(retval),1);
        memcpy(data,Z_STRVAL_P(retval),Z_STRLEN_P(retval));

        // Return values to caller.
        *datap = data;
        *sizep = Z_STRLEN_P(retval);
        convert_to_long(ztype);
        *typep = (git_otype)Z_LVAL_P(ztype);
        convert_to_string(zfull);
        convert_oid_fromstr(oidp,Z_STRVAL_P(zfull),Z_STRLEN_P(zfull));
    }

    // Cleanup zvals.

    zval_ptr_dtor(&zfull);
    zval_ptr_dtor(&ztype);
    zval_ptr_dtor(&zprefix);

    return result;
}

/*static*/ int php_odb_backend_object::read_header(size_t* sizep,git_otype* typep,
    git_odb_backend* backend,const git_oid* oid)
{
    int result;
    zval* zsize;
    zval* ztype;
    zval* zoid;
    method_wrapper method("read_header",backend);

    // Allocate/set zvals.

    ALLOC_INIT_ZVAL(zsize);
    ALLOC_INIT_ZVAL(ztype);
    ALLOC_INIT_ZVAL(zoid);

    convert_oid(zoid,oid);

    // Call userspace method implementation corresponding to odb operation.

    zval* params[] = { zsize, ztype, zoid };

    result = method.call(3,params);
    if (result == GIT_OK) {
        // Return values to caller.
        convert_to_long(zsize);
        convert_to_long(ztype);
        *sizep = Z_LVAL_P(zsize);
        *typep = (git_otype)Z_LVAL_P(ztype);
    }

    // Cleanup zvals.

    zval_ptr_dtor(&zsize);
    zval_ptr_dtor(&ztype);
    zval_ptr_dtor(&zoid);

    return result;
}

/*static*/ int php_odb_backend_object::write(git_odb_backend* backend,
    const git_oid* oid,const void* data,size_t size,git_otype type)
{
    int result;
    zval* zoid;
    zval* zpayload;
    zval* ztype;
    method_wrapper method("write",backend);

    // Allocate and set zvals.

    MAKE_STD_ZVAL(zoid);
    MAKE_STD_ZVAL(zpayload);
    MAKE_STD_ZVAL(ztype);

    convert_oid(zoid,oid);
    ZVAL_STRINGL(zpayload,(const char*)data,size,1);
    ZVAL_LONG(ztype,type);

    // Call userspace method implementation corresponding to odb operation.

    zval* params[] = { zoid, zpayload, ztype };

    result = method.call(3,params);

    // Cleanup zvals.

    zval_ptr_dtor(&zoid);
    zval_ptr_dtor(&zpayload);
    zval_ptr_dtor(&ztype);

    return result;
}

/*static*/ int php_odb_backend_object::writestream(git_odb_stream** streamp,
    git_odb_backend* backend,git_off_t size,git_otype type)
{
    int result;
    zval* zsize;
    zval* ztype;
    method_wrapper method("writestream",backend);

    ZTS_MEMBER_EXTRACT(method.backing()->zts);

    // Allocate/set zvals.

    MAKE_STD_ZVAL(zsize);
    MAKE_STD_ZVAL(ztype);

    ZVAL_LONG(zsize,size);
    ZVAL_LONG(ztype,type);

    // Call userspace method implementation corresponding to odb operation.

    zval* params[] = { zsize, ztype };

    result = method.call(2,params);
    if (result == GIT_OK) {
        zval* retval = method.retval();

        // Make sure returned zval is an object derived from GitODBStream.
        if (Z_TYPE_P(retval) != IS_OBJECT
            || !is_subclass_of(Z_OBJCE_P(retval),class_entry[php_git2_odb_stream_obj]))
        {
            php_git2_giterr_set(
                GITERR_ODB,
                "GitODBBackend::writestream(): return value must be an object "
                "derived from GitODBStream");

            result = GIT_EPHPFATAL;
        }
        else {
            // Copy the return value zval.
            zval* zobj;
            MAKE_STD_ZVAL(zobj);
            ZVAL_ZVAL(zobj,retval,1,0);

            try {
                // Now create the custom stream backing. The stream tracks this
                // object to keep it alive and provide access to the
                // GitODBBackend derivation.
                php_odb_stream_object* sobj = LOOKUP_OBJECT(php_odb_stream_object,retval);
                sobj->create_custom_stream(zobj,method.thisobj(),GIT_STREAM_WRONLY);

                // The libgit2 implementation expects the custom backing to
                // provide a reference to the ODB backend.
                sobj->stream->backend = backend;

                // Assign the git_odb_stream to the out parameter. This is safe
                // since the instance holds its own zval that references the
                // object backing, meaning the returned object will survive
                // until the free() function is called on the git_odb_stream.
                *streamp = sobj->stream;
            } catch (php_git2_fatal_exception& ex) {
                const char* msg = ex.what();
                if (msg == nullptr) {
                    msg = "GitODBBackend::writestream(): failed to generate the writestream";
                }
                php_git2_giterr_set(GITERR_ODB,msg);

                result = GIT_EPHPFATAL;
            }

            zval_ptr_dtor(&zobj);
        }
    }

    // Cleanup zvals.

    zval_ptr_dtor(&zsize);
    zval_ptr_dtor(&ztype);

    return result;
}

/*static*/ int php_odb_backend_object::readstream(git_odb_stream** streamp,
    git_odb_backend* backend,const git_oid* oid)
{
    int result;
    zval* zbuf;
    method_wrapper method("readstream",backend);

    ZTS_MEMBER_EXTRACT(method.backing()->zts);

    // Allocate/set zvals.

    MAKE_STD_ZVAL(zbuf);

    convert_oid(zbuf,oid);

    // Call userspace method implementation corresponding to odb operation.

    zval* params[] = { zbuf };

    result = method.call(1,params);
    if (result == GIT_OK) {
        zval* retval = method.retval();

        // Make sure returned zval is an object derived from GitODBStream.
        if (Z_TYPE_P(retval) != IS_OBJECT
            || !is_subclass_of(Z_OBJCE_P(retval),class_entry[php_git2_odb_stream_obj]))
        {
            php_git2_giterr_set(
                GITERR_ODB,
                "GitODBBackend::readstream(): return value must be an object "
                "derived from GitODBStream");

            result = GIT_EPHPFATAL;
        }
        else {
            // Copy the return value zval.
            zval* zobj;
            MAKE_STD_ZVAL(zobj);
            ZVAL_ZVAL(zobj,retval,1,0);

            try {
                // Now create the custom stream backing. The stream tracks this
                // object to keep it alive and provide access to the
                // GitODBBackend derivation.
                php_odb_stream_object* sobj = LOOKUP_OBJECT(php_odb_stream_object,retval);
                sobj->create_custom_stream(zobj,method.thisobj(),GIT_STREAM_RDONLY);

                // Help out the implementation by providing a reference to the
                // ODB backend.
                sobj->stream->backend = backend;

                // Assign the git_odb_stream to the out parameter. This is safe
                // since the instance holds its own zval that references the
                // object backing, meaning the returned object will survive
                // until the free() function is called on the git_odb_stream.
                *streamp = sobj->stream;
            } catch (php_git2_fatal_exception& ex) {
                const char* msg = ex.what();
                if (msg == nullptr) {
                    msg = "GitODBBackend::readstream(): failed to generate the readstream";
                }
                php_git2_giterr_set(GITERR_ODB,msg);

                result = GIT_EPHPFATAL;
            }

            zval_ptr_dtor(&zobj);
        }
    }

    // Cleanup zvals.

    zval_ptr_dtor(&zbuf);

    return result;
}

/*static*/ int php_odb_backend_object::exists(git_odb_backend* backend,
    const git_oid* oid)
{
    int result;
    zval* zoid;
    method_wrapper method("exists",backend);

    // Allocate/set zvals.

    MAKE_STD_ZVAL(zoid);

    convert_oid(zoid,oid);

    // Call userspace method implementation corresponding to odb operation.

    zval* params[] = { zoid };

    result = method.call(1,params);
    if (result == GIT_OK) {
        zval* retval = method.retval();
        convert_to_boolean(retval);

        // NOTE: exists() returns 1 for TRUE and 0 for FALSE.

        result = Z_BVAL_P(retval) ? 1 : 0;
    }

    // Cleanup zvals.

    zval_ptr_dtor(&zoid);

    return result;
}

/*static*/ int php_odb_backend_object::exists_prefix(git_oid* oidp,
    git_odb_backend* backend,const git_oid* prefix,size_t len)
{
    int result;
    zval* zfull;
    zval* zoid;
    method_wrapper method("exists_prefix",backend);

    // Allocate/set zvals.

    ALLOC_INIT_ZVAL(zfull);
    MAKE_STD_ZVAL(zoid);

    convert_oid_prefix(zoid,prefix,len);

    // Call userspace method implementation corresponding to odb operation.

    zval* params[] = { zfull, zoid };

    result = method.call(2,params);
    if (result == GIT_OK) {
        zval* retval = method.retval();

        // Return values to caller. NOTE: exists_prefix() returns 0 on success
        // (i.e. found) and the GIT_ENOTFOUND error when the element is not
        // found.

        convert_to_string(zfull);
        convert_oid_fromstr(oidp,Z_STRVAL_P(zfull),Z_STRLEN_P(zfull));
        convert_to_boolean(retval);
        result = Z_BVAL_P(retval) ? 0 : GIT_ENOTFOUND;
    }

    // Cleanup zvals.

    zval_ptr_dtor(&zfull);
    zval_ptr_dtor(&zoid);

    return result;
}

/*static*/ int php_odb_backend_object::refresh(git_odb_backend* backend)
{
    int result;
    method_wrapper method("refresh",backend);

    // Call userspace method implementation corresponding to odb operation.

    result = method.call();

    return result;
}

/*static*/ int php_odb_backend_object::foreach(git_odb_backend* backend,
    git_odb_foreach_cb cb,void* payload)
{
    int result;
    zval* zclosure;
    zval* zpayload;
    method_wrapper method("for_each",backend);

    // Create zvals for method call. The payload argument is always a NULL zval
    // from this context. However this fact should be mostly transparent to the
    // user since they will just pass it along in their implementation none the
    // wiser. The payload gets set on the closure object so that it is available
    // to the actual callback. NOTE: the only time the payload zval parameter is
    // directly useful is if the PHP engine calls the "for_each()" method, not
    // this function.

    MAKE_STD_ZVAL(zclosure);
    ALLOC_INIT_ZVAL(zpayload);

    php_closure_object* closureInfo;
    foreach_callback_info callbackInfo = { cb, payload };

    // Make sure ZTS stuff is in-scope for call to object_init_ex() and
    // zend_objects_get_address().
    ZTS_MEMBER_EXTRACT(method.backing()->zts);

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

    // Call userspace method implementation corresponding to odb operation.

    zval* params[] = { zclosure, zpayload };

    result = method.call(2,params);

    zval_ptr_dtor(&zclosure);
    zval_ptr_dtor(&zpayload);

    return result;
}

/*static*/ int php_odb_backend_object::writepack(git_odb_writepack** writepackp,
    git_odb_backend* backend,git_odb* odb,git_transfer_progress_cb progress_cb,
    void* progress_payload)
{
    int result = GIT_OK;
    zval* zodb;
    zval* zclosure;
    zval* zpayload;
    method_wrapper method("writepack",backend);

    ZTS_MEMBER_EXTRACT(method.backing()->zts);

    // Create zvals. The payload zval is unused and will remain null. (Note that
    // the payload will be passed to the underlying callback but not converted
    // for use in userspace.)

    MAKE_STD_ZVAL(zodb);
    MAKE_STD_ZVAL(zclosure);
    ALLOC_INIT_ZVAL(zpayload);

    {
        php_resource_ref<php_git_odb_nofree> resourceWrapper;
        resourceWrapper.set_object(odb);
        resourceWrapper.ret(zodb);
    }

    // Set up the closure to call an internal function that will handle calling
    // the function and passing the payload.

    php_closure_object* closureInfo;
    transfer_progress_callback_info callbackInfo = { progress_cb, progress_payload };

    object_init_ex(zclosure,php_git2::class_entry[php_git2_closure_obj]);
    closureInfo = reinterpret_cast<php_closure_object*>
        (zend_objects_get_address(zclosure TSRMLS_CC));
    closureInfo->func.type = ZEND_INTERNAL_FUNCTION;
    closureInfo->func.common.function_name = "php-git2 transfer progress internal callback";
    closureInfo->func.common.fn_flags |= ZEND_ACC_CLOSURE;
    closureInfo->func.common.num_args = 2;
    closureInfo->func.common.required_num_args = 2;
    closureInfo->func.common.scope = php_git2::class_entry[php_git2_closure_obj];
    closureInfo->func.internal_function.handler = ZEND_FN(backend_transfer_progress_callback);
    closureInfo->hasPayload = true;
    closureInfo->payload = &callbackInfo;

    // Invoke method on odb_backend.

    zval* params[] = { zodb, zclosure, zpayload };

    result = method.call(3,params);
    if (result == GIT_OK) {
        zval* retval = method.retval();

        // Make sure returned zval is an object derived from GitODBWritepack.
        if (Z_TYPE_P(retval) != IS_OBJECT
            || !is_subclass_of(Z_OBJCE_P(retval),class_entry[php_git2_odb_writepack_obj]))
        {
            php_git2_giterr_set(
                GITERR_ODB,
                "GitODBBackend::readstream(): return value must be an object "
                "derived from GitODBWritepack");

            result = GIT_EPHPFATAL;
        }
        else {
            // Copy the return value zval.
            zval* zobj;
            MAKE_STD_ZVAL(zobj);
            ZVAL_ZVAL(zobj,retval,1,0);

            try {
                // Now create the custom writepack backing. The writepack tracks
                // this object (via the backendWrapper's owner) and will keep it
                // alive.
                php_odb_writepack_object* sobj;
                php_odb_backend_object* backendWrapper;

                sobj = LOOKUP_OBJECT(php_odb_writepack_object,retval);
                backendWrapper = LOOKUP_OBJECT(php_odb_backend_object,method.object()->thisobj);
                sobj->create_custom_writepack(zobj,method.object()->thisobj);

                // Help out the implementation by providing a reference to the
                // ODB backend.
                sobj->writepack->backend = backend;

                // Assign the git_odb_stream to the out parameter. This is safe
                // since the instance holds its own zval that references the
                // object backing, meaning the returned object will survive
                // until the free() function is called on the git_odb_stream.
                *writepackp = sobj->writepack;
            } catch (php_git2_fatal_exception& ex) {
                const char* msg = ex.what();
                if (msg == nullptr) {
                    msg = "GitODBBackend::readstream(): failed to generate the readstream";
                }
                php_git2_giterr_set(GITERR_ODB,msg);

                result = GIT_EPHPFATAL;
            }

            zval_ptr_dtor(&zobj);
        }
    }

    // Do cleanup.
    zval_ptr_dtor(&zodb);
    zval_ptr_dtor(&zclosure);
    zval_ptr_dtor(&zpayload);

    return result;
}

/*static*/ void php_odb_backend_object::free(git_odb_backend* backend)
{
    method_wrapper::object_wrapper object(backend);

    ZTS_MEMBER_EXTRACT(object.backing()->zts);

    // Make sure object buckets still exist to lookup object (in case the
    // destructor was already called). This happens when free() is called after
    // PHP has finished cleaning up all objects (but before all variables, which
    // may have references to git2 objects that reference this PHP object).

    if (EG(objects_store).object_buckets != nullptr) {
        // Unassign backend from the object since it is about to get destroyed.
        object.backing()->backend = nullptr;
    }

    // Explicitly call the destructor on the custom backend. Then free the block
    // of memory that holds the custom backend.

    object.object()->~git_odb_backend_php();
    efree(backend);
}

// php_odb_backend_object::git_odb_backend_php

php_odb_backend_object::
git_odb_backend_php::git_odb_backend_php(zval* zv TSRMLS_DC)
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
                    // resource remains valid for the user. (The resource
                    // destructor will handle the decrement.)
                    rsrc = backendWrapper->owner;
                    rsrc->up_ref();
                }
                zend_register_resource(ret,rsrc,php_git_odb::resource_le() TSRMLS_CC);

                Z_ADDREF_P(ret);
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
        php_error(E_ERROR,"Property '%s' of GitODBBackend cannot be updated",str);
    }
    else {
        (*std_object_handlers.write_property)(obj,prop,value,key TSRMLS_CC);
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

// Implementation of internal function for custom backend's foreach callback.

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

ZEND_FUNCTION(backend_transfer_progress_callback)
{
    int retval;
    zval* zstats = nullptr;
    git_transfer_progress stats;
    zval* zdummyPayload = nullptr;
    php_closure_object* object = LOOKUP_OBJECT(php_closure_object,getThis());
    const transfer_progress_callback_info* info
        = reinterpret_cast<const transfer_progress_callback_info*>(object->payload);

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"a|z",&zstats,&zdummyPayload) == FAILURE) {
        return;
    }

    // Convert parameters to libgit2 values.
    retval = convert_transfer_progress(stats,zstats);
    if (retval == GIT_ERROR) {
        // Generate exception. The returned stats array was malformed.

        return;
    }

    // Invoke the payload and return the result to user space. The payload
    // passed here is the payload from the libgit2 calling context (not
    // userspace).
    retval = (*info->callback)(&stats,info->payload);
    RETVAL_LONG(static_cast<long>(retval));
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
