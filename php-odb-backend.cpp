/*
 * php-odb-backend.cpp
 *
 * Copyright (C) Roger P. Gee
 *
 * This unit provides the out-of-line implementation for the GitODBBackend
 * class.
 */

#include "php-object.h"
#include "php-callback.h"
#include <cstring>
#include <new>
using namespace php_git2;

// Wrapper for efree since it's a macro.

static void efree_wrapper(void* d)
{
    efree(d);
}

// Custom class handlers

static zval* odb_backend_read_property(zval* object,
    zval* member,
    int type,
    void** cache_slot,
    zval* rv);
static void odb_backend_write_property(zval* object,
    zval* member,
    zval* value,
    void** cache_slot);
static int odb_backend_has_property(zval* object,
    zval* member,
    int has_set_exists,
    void** cache_slot);

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

// php_zend_object init function

template<>
void php_zend_object<php_odb_backend_object>::init(zend_class_entry* ce)
{
    handlers.read_property = odb_backend_read_property;
    handlers.write_property = odb_backend_write_property;
    handlers.has_property = odb_backend_has_property;

    UNUSED(ce);
}

// Implementation of php_odb_backend_object

php_odb_backend_object::php_odb_backend_object():
    backend(nullptr), kind(unset), owner(nullptr)
{
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
}

void php_odb_backend_object::create_custom_backend(zval* obj)
{
    // Make sure object doesn't already have a backing. This would imply it is
    // already in use in an ODB.
    if (backend != nullptr) {
        throw php_git2_fatal_exception(
            "Cannot create custom ODB backend: object already in use"
            );
    }

    // Set kind to 'custom'.
    kind = custom;

    // Create custom backend. Custom backends are always passed off to git2, so
    // we are not responsible for calling its free function.
    backend = new (emalloc(sizeof(git_odb_backend_php))) git_odb_backend_php(Z_OBJ_P(obj));

    // Custom ODB backends never have a direct owner. We always assume the
    // would-be owner is kept alive circularly since the custom backend stores a
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

void php_odb_backend_object::unset_backend(zval* obj)
{
    // Unset 'odb' property.
    zend_unset_property(Z_OBJCE_P(obj),obj,"odb",sizeof("odb")-1);

    // Unset backend reference.
    backend = nullptr;
    kind = unset;
}

/*static*/ int php_odb_backend_object::read(
    void** datap,
    size_t* sizep,
    git_otype* typep,
    git_odb_backend* backend,
    const git_oid* oid)
{
    int result;
    zval_array<2> params;
    method_wrapper method("read",backend);

    // Prepare parameters.

    convert_oid(params[1],oid);

    // Call userspace method.

    result = method.call(params);
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
        convert_to_long(params[0]);
        *typep = (git_otype)Z_LVAL_P(params[0]);
    }

    return result;
}

/*static*/ int php_odb_backend_object::read_prefix(
    git_oid* oidp,
    void** datap,
    size_t* sizep,
    git_otype* typep,
    git_odb_backend* backend,
    const git_oid* prefix,
    size_t len)
{
    int result;
    zval_array<3> params;
    method_wrapper method("read_prefix",backend);

    // Prepare parameters.

    convert_oid_prefix(params[2],prefix,len);

    // Call userspace method.

    result = method.call(params);
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
        convert_to_long(params[1]);
        *typep = (git_otype)Z_LVAL_P(params[1]);
        convert_to_string(params[0]);
        convert_oid_fromstr(oidp,Z_STRVAL_P(params[0]),Z_STRLEN_P(params[0]));
    }

    return result;
}

/*static*/ int php_odb_backend_object::read_header(
    size_t* sizep,
    git_otype* typep,
    git_odb_backend* backend,
    const git_oid* oid)
{
    int result;
    zval_array<3> params;
    method_wrapper method("read_header",backend);

    // Prepare parameters.

    convert_oid(params[2],oid);

    // Call userspace method.

    result = method.call(params);
    if (result == GIT_OK) {
        // Return values to caller.
        convert_to_long(params[0]);
        convert_to_long(params[1]);
        *sizep = Z_LVAL_P(params[0]);
        *typep = (git_otype)Z_LVAL_P(params[1]);
    }

    return result;
}

/*static*/ int php_odb_backend_object::write(
    git_odb_backend* backend,
    const git_oid* oid,
    const void* data,
    size_t size,
    git_otype type)
{
    int result;
    zval_array<3> params;
    method_wrapper method("write",backend);

    // Prepare parameters.

    convert_oid(params[0],oid);
    ZVAL_STRINGL(params[1],(const char*)data,size);
    ZVAL_LONG(params[2],type);

    // Call userspace method.

    result = method.call(params);

    return result;
}

/*static*/ int php_odb_backend_object::writestream(
    git_odb_stream** streamp,
    git_odb_backend* backend,
    git_off_t size,
    git_otype type)
{
    int result;
    zval_array<2> params;
    method_wrapper method("writestream",backend);

    // Prepare parameters.

    ZVAL_LONG(params[0],size);
    ZVAL_LONG(params[1],type);

    // Call userspace method.

    result = method.call(params);
    if (result != GIT_OK) {
        return result;
    }

    try {
        php_object<php_odb_stream_object> obj;
        obj.parse_with_context(
            method.retval(),
            "GitODBBackend::writestream(): return value"
            );

        php_odb_stream_object* stream = obj.get_storage();

        // Now create the custom stream backing. The stream tracks this object
        // to keep it alive and provide access to the GitODBBackend derivation.
        stream->create_custom_stream(
            obj.get_value(),
            method.thisobj(),
            GIT_STREAM_WRONLY);

        // The libgit2 implementation expects the custom backing to provide a
        // reference to the ODB backend.
        stream->stream->backend = backend;

        // Assign the git_odb_stream to the out parameter. This is safe since
        // the instance holds its own reference to the object backing, meaning
        // the returned object will survive until the free() function is called
        // on the git_odb_stream.
        *streamp = stream->stream;

    } catch (php_git2_exception_base& ex) {
        php_git2_giterr_set(GITERR_ODB,ex.what());
        result = GIT_EPHPFATAL;
    }

    return result;
}

/*static*/ int php_odb_backend_object::readstream(
    git_odb_stream** streamp,
    git_odb_backend* backend,
    const git_oid* oid)
{
    int result;
    zval_array<1> params;
    method_wrapper method("readstream",backend);

    // Prepare parameters.

    convert_oid(params[0],oid);

    // Call userspace method.

    result = method.call(params);
    if (result != GIT_OK) {
        return result;
    }

    try {
        php_object<php_odb_stream_object> obj;
        obj.parse_with_context(
            method.retval(),
            "GitODBBackend::readstream(): return value"
            );

        php_odb_stream_object* stream = obj.get_storage();

        // Now create the custom stream backing. The stream tracks this object
        // to keep it alive and provide access to the GitODBBackend derivation.
        stream->create_custom_stream(
            obj.get_value(),
            method.thisobj(),
            GIT_STREAM_RDONLY);

        // The libgit2 implementation expects the custom backing to provide a
        // reference to the ODB backend.
        stream->stream->backend = backend;

        // Assign the git_odb_stream to the out parameter. This is safe since
        // the instance holds its own reference to the object backing, meaning
        // the returned object will survive until the free() function is called
        // on the git_odb_stream.
        *streamp = stream->stream;

    } catch (php_git2_exception_base& ex) {
        php_git2_giterr_set(GITERR_ODB,ex.what());
        result = GIT_EPHPFATAL;
    }

    return result;
}

/*static*/ int php_odb_backend_object::exists(
    git_odb_backend* backend,
    const git_oid* oid)
{
    int result;
    zval_array<1> params;
    method_wrapper method("exists",backend);

    // Prepare parameters.

    convert_oid(params[0],oid);

    // Call userspace method.

    result = method.call(params);
    if (result == GIT_OK) {
        zval* retval = method.retval();
        convert_to_boolean(retval);

        // NOTE: exists() returns 1 for TRUE and 0 for FALSE.

        result = Z_TYPE_P(retval) == IS_TRUE ? 1 : 0;
    }

    return result;
}

/*static*/ int php_odb_backend_object::exists_prefix(
    git_oid* oidp,
    git_odb_backend* backend,
    const git_oid* prefix,
    size_t len)
{
    int result;
    zval_array<2> params;
    method_wrapper method("exists_prefix",backend);

    // Prepare parameters.

    convert_oid_prefix(params[1],prefix,len);

    // Call userspace method.

    result = method.call(params);
    if (result == GIT_OK) {
        zval* retval = method.retval();

        // Return values to caller. NOTE: exists_prefix() returns 0 on success
        // (i.e. found) and the GIT_ENOTFOUND error when the element is not
        // found.

        convert_to_string(params[0]);
        convert_oid_fromstr(oidp,Z_STRVAL_P(params[0]),Z_STRLEN_P(params[0]));

        convert_to_boolean(retval);
        result = Z_TYPE_P(retval) == IS_TRUE ? 0 : GIT_ENOTFOUND;
    }

    return result;
}

/*static*/ int php_odb_backend_object::refresh(git_odb_backend* backend)
{
    int result;
    method_wrapper method("refresh",backend);

    // Call userspace method.

    result = method.call();

    return result;
}

/*static*/ int php_odb_backend_object::foreach(
    git_odb_backend* backend,
    git_odb_foreach_cb cb,
    void* payload)
{
    int result;
    zval_array<2> params;
    php_closure_object* closure;
    method_wrapper method("for_each",backend);
    foreach_callback_info callbackInfo = { cb, payload };

    // Set up the closure to call an internal function that will handle calling
    // the function and passing the payload.

    object_init_ex(params[0],php_git2::class_entry[php_git2_closure_obj]);

    closure = php_zend_object<php_closure_object>::get_storage(Z_OBJ_P(params[0]));
    closure->func.type = ZEND_INTERNAL_FUNCTION;
    closure->func.common.function_name = zend_string_init(
        "php_odb_backend_object_foreach_internal_callback",
        sizeof("php_odb_backend_object_foreach_internal_callback")-1,
        0
        );
    closure->func.common.fn_flags |= ZEND_ACC_CLOSURE;
    closure->func.common.num_args = 2;
    closure->func.common.required_num_args = 2;
    closure->func.common.scope = php_git2::class_entry[php_git2_closure_obj];
    closure->func.internal_function.handler = ZEND_FN(backend_foreach_callback);
    closure->hasPayload = true;
    closure->payload = &callbackInfo;

    // Call userspace method. NOTE: The payload (i.e. params[1]) will be NULL.

    result = method.call(params);
    zend_string_release(closure->func.common.function_name);

    return result;
}

/*static*/ int php_odb_backend_object::writepack(
    git_odb_writepack** writepackp,
    git_odb_backend* backend,
    git_odb* odb,
    git_transfer_progress_cb progress_cb,
    void* progress_payload)
{
    int result = GIT_OK;
    zval_array<3> params;
    method_wrapper method("writepack",backend);

    // Prepare parameters The payload zval (params[2]) is unused and will remain
    // NULL. (Note that the payload will be passed to the underlying callback
    // internally but not converted for use in userspace.)

    {
        // Create non-freeable resource value for ODB.
        php_resource_ref<php_git_odb_nofree> resourceWrapper;
        resourceWrapper.set_object(odb);
        resourceWrapper.ret(params[0]);
    }

    // Set up the closure to call an internal function that will handle calling
    // the function and passing the payload.

    php_closure_object* closure;
    transfer_progress_callback_info* callbackInfo;

    callbackInfo = reinterpret_cast<transfer_progress_callback_info*>(
        emalloc(sizeof(transfer_progress_callback_info))
        );
    callbackInfo->callback = progress_cb;
    callbackInfo->payload = progress_payload;

    object_init_ex(params[1],php_git2::class_entry[php_git2_closure_obj]);
    closure = php_zend_object<php_closure_object>::get_storage(Z_OBJ_P(params[1]));
    closure->func.type = ZEND_INTERNAL_FUNCTION;
    closure->func.common.function_name = zend_string_init(
        "php_odb_backend_object_writepack_internal_callback",
        sizeof("php_odb_backend_object_writepack_internal_callback")-1,
        0
        );
    closure->func.common.fn_flags |= ZEND_ACC_CLOSURE;
    closure->func.common.num_args = 2;
    closure->func.common.required_num_args = 2;
    closure->func.common.scope = php_git2::class_entry[php_git2_closure_obj];
    closure->func.internal_function.handler = ZEND_FN(backend_transfer_progress_callback);
    closure->hasPayload = true;
    closure->payload = callbackInfo;
    closure->payloadDestructor = efree_wrapper;

    // Invoke method on odb_backend.

    result = method.call(params);
    zend_string_release(closure->func.common.function_name);
    if (result != GIT_OK) {
        return result;
    }

    try {
        php_object<php_odb_writepack_object> obj;
        obj.parse_with_context(
            method.retval(),
            "GitODBBackend::writepack(): return value"
            );

        php_odb_writepack_object* writepack = obj.get_storage();

        // Now create the custom writepack backing. The writepack tracks this
        // object (via the backendWrapper's owner) and will keep it alive.

        writepack->create_custom_writepack(obj.get_value(),method.thisobj());

        // Help out the implementation by providing a reference to the ODB
        // backend.
        writepack->writepack->backend = backend;

        // Assign the git_odb_stream to the out parameter. This is safe since
        // the instance holds its own reference to the object backing, meaning
        // the returned object will survive until the free() function is called
        // on the git_odb_writepack.
        *writepackp = writepack->writepack;

    } catch (php_git2_exception_base& ex) {
        php_git2_giterr_set(GITERR_ODB,ex.what());
        result = GIT_EPHPFATAL;
    }

    return result;
}

/*static*/ void php_odb_backend_object::free(git_odb_backend* backend)
{
    method_wrapper::object_wrapper object(backend);

    // Make sure object buckets still exist to lookup object (in case the
    // destructor was already called). This happens when free() is called after
    // PHP has finished cleaning up all objects (but before all variables, which
    // may have references to git2 objects that reference this PHP object).

    if (EG(objects_store).object_buckets != nullptr) {
        // Unassign backend from the object since it is about to get
        // destroyed. This also ensures no references remain to any ODB since
        // the ODB is (presumably) in the process of freeing right now.
        object.backing()->unset_backend(object.thisobj());
    }

    // Explicitly call the destructor on the custom backend. Then free the block
    // of memory that holds the custom backend.

    object.object()->~git_odb_backend_php();
    efree(backend);
}

// php_odb_backend_object::git_odb_backend_php

php_odb_backend_object::
git_odb_backend_php::git_odb_backend_php(zend_object* obj)
{
    // Blank out the base class (which is the structure defined in the git2
    // headers).
    memset(this,0,sizeof(struct git_odb_backend));

    // Assign zend_object to local zval so it is kept alive for duration of
    // custom backend.
    ZVAL_OBJ(&thisobj,obj);
    Z_ADDREF(thisobj);

    version = GIT_ODB_BACKEND_VERSION;

    // Every custom backend gets the free function (whether it is overloaded in
    // userspace or not).
    free = php_odb_backend_object::free;

    // We now must select which functions we are going to include in the
    // backend. We do this by determining which ones were overloaded.
    if (is_method_overridden(obj->ce,"read",sizeof("read"))) {
        read = php_odb_backend_object::read;
    }
    if (is_method_overridden(obj->ce,"read_prefix",sizeof("read_prefix"))) {
        read_prefix = php_odb_backend_object::read_prefix;
    }
    if (is_method_overridden(obj->ce,"read_header",sizeof("read_header"))) {
        read_header = php_odb_backend_object::read_header;
    }
    if (is_method_overridden(obj->ce,"write",sizeof("write"))) {
        write = php_odb_backend_object::write;
    }
    if (is_method_overridden(obj->ce,"writestream",sizeof("writestream"))) {
        writestream = php_odb_backend_object::writestream;
    }
    if (is_method_overridden(obj->ce,"readstream",sizeof("readstream"))) {
        readstream = php_odb_backend_object::readstream;
    }
    if (is_method_overridden(obj->ce,"exists",sizeof("exists"))) {
        exists = php_odb_backend_object::exists;
    }
    if (is_method_overridden(obj->ce,"exists_prefix",sizeof("exists_prefix"))) {
        exists_prefix = php_odb_backend_object::exists_prefix;
    }
    if (is_method_overridden(obj->ce,"refresh",sizeof("refresh"))) {
        refresh = php_odb_backend_object::refresh;
    }
    if (is_method_overridden(obj->ce,"for_each",sizeof("for_each"))) {
        foreach = php_odb_backend_object::foreach;
    }
    if (is_method_overridden(obj->ce,"writepack",sizeof("writepack"))) {
        writepack = php_odb_backend_object::writepack;
    }
}

php_odb_backend_object::
git_odb_backend_php::~git_odb_backend_php()
{
    zval_ptr_dtor(&thisobj);
}

// Implementation of custom class handlers

zval* odb_backend_read_property(
    zval* object,
    zval* member,
    int type,
    void** cache_slot,
    zval* rv)
{
    zval* retval;
    zval tmp_member;
    php_odb_backend_object* storage;

    // Ensure deep copy of member zval.
    if (Z_TYPE_P(member) != IS_STRING) {
        ZVAL_STR(&tmp_member,zval_get_string(member));
        member = &tmp_member;
        cache_slot = nullptr;
    }

    // Handle special properties of the git_odb_backend.

    storage = php_zend_object<php_odb_backend_object>::get_storage(Z_OBJ_P(object));

    if (strcmp(Z_STRVAL_P(member),"version") == 0) {
        if (storage->backend != nullptr) {
            ZVAL_LONG(rv,storage->backend->version);
        }
        retval = rv;
    }
    else if (strcmp(Z_STRVAL_P(member),"odb") == 0) {
        retval = zend_hash_find(Z_OBJPROP_P(object),Z_STR_P(member));

        if (retval == nullptr) {
            retval = rv;

            if (storage->backend != nullptr && storage->backend->odb != nullptr) {
                zend_resource* zr;
                php_git_odb* rsrc;

                if (storage->owner == nullptr) {
                    // Create new resource that will not free the git_odb.
                    rsrc = php_git2_create_resource<php_git_odb_nofree>();
                    rsrc->set_handle(storage->backend->odb);
                }
                else {
                    // Use owner resource. We must increment the refcount so the
                    // resource remains valid for the user. (The resource
                    // destructor will handle the decrement.)
                    rsrc = storage->owner;
                    rsrc->up_ref();
                }
                zr = zend_register_resource(rsrc,php_git_odb::resource_le());
                ZVAL_RES(retval,zr);

                Z_ADDREF_P(retval);
                zend_hash_add(Z_OBJPROP_P(object),Z_STR_P(member),retval);
            }
        }
    }
    else {
        zend_object_handlers* std = zend_get_std_object_handlers();
        retval = std->read_property(object,member,type,cache_slot,rv);
    }

    if (member == &tmp_member) {
        zval_dtor(member);
    }

    return retval;
}

void odb_backend_write_property(
    zval* object,
    zval* member,
    zval* value,
    void** cache_slot)
{
    zval tmp_member;

    // Ensure deep copy of member zval.
    if (Z_TYPE_P(member) != IS_STRING) {
        ZVAL_STR(&tmp_member,zval_get_string(member));
        member = &tmp_member;
        cache_slot = nullptr;
    }

    if (strcmp(Z_STRVAL_P(member),"version") == 0
        || strcmp(Z_STRVAL_P(member),"odb") == 0)
    {
        zend_throw_error(
            nullptr,
            "Property '%s' of GitODBBackend cannot be updated",
            Z_STRVAL_P(member)
            );
    }
    else {
        zend_object_handlers* std = zend_get_std_object_handlers();
        std->write_property(object,member,value,cache_slot);
    }

    if (member == &tmp_member) {
        zval_dtor(member);
    }
}

int odb_backend_has_property(
    zval* object,
    zval* member,
    int has_set_exists,
    void** cache_slot)
{
    int result;
    zval tmp_member;
    git_odb_backend* backend;

    // Ensure deep copy of member zval.
    if (Z_TYPE_P(member) != IS_STRING) {
        ZVAL_STR(&tmp_member,zval_get_string(member));
        member = &tmp_member;
        cache_slot = nullptr;
    }

    backend = php_zend_object<php_odb_backend_object>::get_storage(Z_OBJ_P(object))->backend;

    if (strcmp(Z_STRVAL_P(member),"version") == 0) {
        result = (backend != nullptr);
    }
    else if (strcmp(Z_STRVAL_P(member),"odb") == 0) {
        if (has_set_exists == 2) {
            result = (backend != nullptr);
        }
        else {
            result = (backend != nullptr && backend->odb != nullptr);
        }
    }
    else {
        zend_object_handlers* std = zend_get_std_object_handlers();
        result = std->has_property(object,member,has_set_exists,cache_slot);
    }

    if (member == &tmp_member) {
        zval_dtor(member);
    }

    return result;
}

// Implementation of internal function for custom backend's foreach callback.

ZEND_FUNCTION(backend_foreach_callback)
{
    php_bailer bailer;
    char* oidS;
    size_t oidL;
    zval* payload = nullptr;

    if (zend_parse_parameters(ZEND_NUM_ARGS(),"s|z",&oidS,&oidL,&payload) == FAILURE) {
        return;
    }

    try {
        int result;
        git_oid oid;
        php_closure_object* object;
        const foreach_callback_info* info;

        object = php_zend_object<php_closure_object>::get_storage(Z_OBJ_P(getThis()));
        info = reinterpret_cast<const foreach_callback_info*>(object->payload);

        // Convert string representation to git2 OID struct.
        php_git2::convert_oid_fromstr(&oid,oidS,oidL);

        // Invoke callback.
        result = (*info->callback)(&oid,info->payload);
        if (result != GIT_OK) {
            php_git2::git_error(result);
        }

    } catch (php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle(TSRMLS_C);
        }
    }
}

ZEND_FUNCTION(backend_transfer_progress_callback)
{
    php_bailer bailer;
    zval* zstats;
    zval* payload = nullptr;

    if (zend_parse_parameters(ZEND_NUM_ARGS(),"a|z",&zstats,&payload) == FAILURE) {
        return;
    }

    try {
        int result;
        php_closure_object* object;
        git_transfer_progress stats;
        const transfer_progress_callback_info* info;

        // Convert parameters to libgit2 values.
        result = php_git2::convert_transfer_progress(stats,zstats);
        if (result == GIT_ERROR) {
            php_git2::git_error(result);
        }

        object = php_zend_object<php_closure_object>::get_storage(Z_OBJ_P(getThis()));
        info = reinterpret_cast<const transfer_progress_callback_info*>(object->payload);

        // Invoke the payload and return the result to user space. The payload
        // passed here is the payload from the libgit2 calling context (not
        // userspace).
        result = (*info->callback)(&stats,info->payload);
        if (result != GIT_OK) {
            php_git2::git_error(result);
        }

    } catch (php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle(TSRMLS_C);
        }
    }
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
