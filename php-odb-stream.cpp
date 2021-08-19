/*
 * php-odb-stream.cpp
 *
 * Copyright (C) Roger P. Gee
 */

#include "php-object.h"
using namespace php_git2;

// Custom object handlers

static zval* odb_stream_read_property(zval* object,
    zval* member,
    int type,
    void** cache_slot,
    zval* rv);
static void odb_stream_write_property(zval* object,
    zval* member,
    zval* value,
    void** cache_slot);
static int odb_stream_has_property(zval* object,
    zval* member,
    int has_set_exists,
    void** cache_slot);

// Class method entries

static PHP_EMPTY_METHOD(GitODBStream,read);
static PHP_EMPTY_METHOD(GitODBStream,write);
static PHP_EMPTY_METHOD(GitODBStream,finalize_write);

zend_function_entry php_git2::odb_stream_methods[] = {
    PHP_ME(GitODBStream,read,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBStream,write,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBStream,finalize_write,NULL,ZEND_ACC_PUBLIC)
    PHP_FE_END
};

// php_zend_object init function

template<>
zend_object_handlers php_git2::php_zend_object<php_odb_stream_object>::handlers;

template<>
void php_zend_object<php_odb_stream_object>::init(zend_class_entry* ce)
{
    handlers.read_property = odb_stream_read_property;
    handlers.write_property = odb_stream_write_property;
    handlers.has_property = odb_stream_has_property;

    handlers.offset = offset();

    UNUSED(ce);
}

// Implementation of php_odb_stream_object

php_odb_stream_object::php_odb_stream_object():
    stream(nullptr), kind(unset), owner(nullptr)
{
    ZVAL_UNDEF(&backend);
}

php_odb_stream_object::~php_odb_stream_object()
{
    if (stream != nullptr) {
        if (kind == conventional) {
            // If the stream was created conventionally using the higher-level
            // functions, free via git_odb_stream_free(). To prevent memory
            // leaks we must call this function. Note: this does call
            // stream->free() at some point.

            git_odb_stream_free(stream);
        }
        else if (kind == user) {
            // If the stream was created in PHP userspace (e.g. via the
            // GitODBBackend's stream methods), then free directly using the
            // stream's bound free() function.

            stream->free(stream);
        }

        // NOTE: When kind == custom, the libgit2 library is responsible for
        // freeing the stream, and we do not free it here.
    }

    // Free owner ODB if set. This is typically set when kind == conventional.
    if (owner != nullptr) {
        git2_resource_base::free_recursive(owner);
    }

    // Free backend zval.
    zval_ptr_dtor(&backend);
}

void php_odb_stream_object::create_custom_stream(
    zval* zobj,
    zval* backendObject,
    unsigned int mode)
{
    // NOTE: this member function should be called under the php-git2 standard
    // exception handler.

    // Make sure the stream does not already exist.
    if (stream != nullptr) {
        throw php_git2_error_exception(
            "cannot create custom ODB stream - object already in use");
    }

    // The stream wrapper is now considered custom.
    kind = custom;

    // Free existing zbackend (just in case).
    zval_ptr_dtor(&backend);

    // Assign backend and increment reference count. This keeps the
    // GitODBBackend alive while our GitODBStream is alive.
    ZVAL_COPY(&backend,backendObject);

    // Create new custom stream.
    stream = new (emalloc(sizeof(git_odb_stream_php)))
        git_odb_stream_php(Z_OBJ_P(zobj),mode);

    // Custom ODB streams never require an ODB owner. (The backend is the owner
    // in this case.)
    assign_owner(nullptr);
}

void php_odb_stream_object::assign_owner(php_git_odb* newOwner)
{
    // Free existing owner (just in case).
    if (owner != nullptr) {
        git2_resource_base::free_recursive(owner);
    }

    owner = newOwner;

    // Increment owner refcount to prevent the ODB from freeing while the stream
    // object is in use.
    if (owner != nullptr) {
        owner->up_ref();
    }
}

/*static*/ int php_odb_stream_object::read(git_odb_stream *stream,char *buffer,size_t len)
{
    int result;
    zval_array<1> params;
    method_wrapper method("read",stream);

    ZVAL_LONG(params[0],len);

    // Call userspace method implementation of corresponding stream operation.

    result = method.call(params);
    if (result == GIT_OK) {
        size_t n;
        zval* retval = method.retval();

        // Return values to caller. We copy the data into the git2-provided
        // buffer, making sure not to overflow.

        convert_to_string(retval);

        n = Z_STRLEN_P(retval);
        if (n > len) {
            n = len;
        }

        memcpy(buffer,Z_STRVAL_P(retval),n);
    }

    return result;
}

/*static*/ int php_odb_stream_object::write(git_odb_stream *stream,const char *buffer,size_t len)
{
    int result;
    zval_array<1> params;
    method_wrapper method("write",stream);

    ZVAL_STRINGL(params[0],buffer,len);

    // Call userspace method implementation of corresponding stream operation.

    result = method.call(params);

    return result;
}

/*static*/ int php_odb_stream_object::finalize_write(git_odb_stream *stream,const git_oid *oid)
{
    int result;
    zval_array<1> params;
    method_wrapper method("finalize_write",stream);

    // Initialize/set zvals.

    convert_oid(params[0],oid);

    // Call userspace method implementation of corresponding stream operation.

    result = method.call(params);

    return result;
}

/*static*/ void php_odb_stream_object::free(git_odb_stream *stream)
{
    method_wrapper::object_wrapper object(stream);

    // Explicitly call the destructor on the custom stream. Then free the block
    // of memory that holds the custom stream.

    object.object()->~git_odb_stream_php();
    efree(stream);
}

// php_odb_stream::git_odb_stream_php

php_odb_stream_object::
git_odb_stream_php::git_odb_stream_php(zend_object* obj,unsigned int newMode)
{
    // Blank out the base class (which is the structure defined in the git2
    // headers).
    memset(this,0,sizeof(struct git_odb_stream));

    // Create object zval to track and keep PHP object storage alive.
    ZVAL_OBJ(&thisobj,obj);
    Z_ADDREF(thisobj);

    // Make sure the class provided overridden methods needed for the requested
    // mode. If not we raise an error.
    if (mode == GIT_STREAM_RDONLY) {
        if (!is_method_overridden(obj->ce,"read",sizeof("read")-1)) {
            throw php_git2_error_exception("Custom GitODBStream must override read()");
        }
    }
    else if (mode == GIT_STREAM_WRONLY) {
        if (!is_method_overridden(obj->ce,"write",sizeof("write")-1)) {
            throw php_git2_error_exception("Custom GitODBStream must override write()");
        }
        if (!is_method_overridden(obj->ce,"finalize_write",sizeof("finalize_write")-1)) {
            throw php_git2_error_exception(
                "Custom GitODBStream must override finalize_write()");
        }
    }

    // Every custom stream must provide all the functions even if they are not
    // all implemented.
    read = php_odb_stream_object::read;
    write = php_odb_stream_object::write;
    finalize_write = php_odb_stream_object::finalize_write;
    free = php_odb_stream_object::free;

    mode = newMode;
}

php_odb_stream_object::
git_odb_stream_php::~git_odb_stream_php()
{
    zval_ptr_dtor(&thisobj);
}

// Implementation of object handlers

zval* odb_stream_read_property(
    zval* object,
    zval* member,
    int type,
    void** cache_slot,
    zval* rv)
{
    zval* retval = rv;
    zval tmp_member;
    php_odb_stream_object* storage;
    git_odb_stream* stream;

    // Ensure deep copy of member zval.
    if (Z_TYPE_P(member) != IS_STRING) {
        ZVAL_STR(&tmp_member,zval_get_string(member));
        member = &tmp_member;
        cache_slot = nullptr;
    }

    storage = php_zend_object<php_odb_stream_object>::get_storage(Z_OBJ_P(object));
    stream = storage->stream;

    // Handle special properties of the git_odb_stream.

    if (strcmp(Z_STRVAL_P(member),"mode") == 0 && stream != nullptr) {
        ZVAL_LONG(rv,stream->mode);
    }
    else if (strcmp(Z_STRVAL_P(member),"declared_size") == 0 && stream != nullptr) {
        ZVAL_LONG(rv,stream->declared_size);
    }
    else if (strcmp(Z_STRVAL_P(member),"received_bytes") == 0 && stream != nullptr) {
        ZVAL_LONG(rv,stream->received_bytes);
    }
    else if (strcmp(Z_STRVAL_P(member),"backend") == 0 && stream != nullptr) {
        if (Z_TYPE(storage->backend) != IS_UNDEF) {
            ZVAL_COPY(rv,&storage->backend);
        }
        else {
            // NOTE: In practice, this case should never get executed for custom
            // backends overriding the write/readpack methods since
            // create_custom_stream() should be called in every use case to set
            // the backend in internal storage. However it will get called for
            // internal backends or backends that employ the default "fake" ODB
            // streams.

            // NOTE: We can only safely create a backend if an owner is set on
            // the stream.

            if (stream->backend != nullptr && storage->owner != nullptr) {
                // Create backend object to return to userspace.
                php_git2_make_odb_backend(rv,stream->backend,storage->owner);

                // Cache the backend to internal storage for possible lookup
                // later on. Increment refcount so we can hold a reference in
                // the object storage.
                ZVAL_COPY(&storage->backend,rv);
            }
        }
    }
    else {
        const zend_object_handlers* std = zend_get_std_object_handlers();
        retval = std->read_property(object,member,type,cache_slot,rv);
    }

    if (member == &tmp_member) {
        zval_dtor(member);
    }

    return retval;
}

void odb_stream_write_property(
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

    if (strcmp(Z_STRVAL_P(member),"mode") == 0
        || strcmp(Z_STRVAL_P(member),"declared_size") == 0
        || strcmp(Z_STRVAL_P(member),"received_bytes") == 0
        || strcmp(Z_STRVAL_P(member),"backend") == 0)
    {
        zend_throw_error(
            nullptr,
            "Property '%s' of GitODBStream cannot be updated",
            Z_STRVAL_P(member));
    }
    else {
        const zend_object_handlers* std = zend_get_std_object_handlers();
        std->write_property(object,member,value,cache_slot);
    }

    if (member == &tmp_member) {
        zval_dtor(member);
    }
}

int odb_stream_has_property(
    zval* object,
    zval* member,
    int has_set_exists,
    void** cache_slot)
{
    int result;
    zval tmp_member;
    git_odb_stream* stream;

    // Ensure deep copy of member zval.
    if (Z_TYPE_P(member) != IS_STRING) {
        ZVAL_STR(&tmp_member,zval_get_string(member));
        member = &tmp_member;
        cache_slot = nullptr;
    }

    stream = php_zend_object<php_odb_stream_object>::get_storage(Z_OBJ_P(object))->stream;

    if (strcmp(Z_STRVAL_P(member),"mode") == 0
        || strcmp(Z_STRVAL_P(member),"declared_size") == 0
        || strcmp(Z_STRVAL_P(member),"received_bytes") == 0)
    {
        result = (stream != nullptr);
    }
    else if (strcmp(Z_STRVAL_P(member),"backend") == 0) {
        if (has_set_exists == 2) {
            result = (stream != nullptr);
        }
        else {
            result = (stream != nullptr && stream->backend != nullptr);
        }
    }
    else {
        const zend_object_handlers* std = zend_get_std_object_handlers();
        result = std->has_property(object,member,has_set_exists,cache_slot);
    }

    if (member == &tmp_member) {
        zval_dtor(member);
    }

    return result;
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
