/*
 * php-odb-stream.cpp
 *
 * This file is a part of php-git2.
 */

#include "php-object.h"
using namespace php_git2;

// Custom object handlers

static zval* odb_stream_read_property(zval* obj,zval* prop,int type,const zend_literal* key TSRMLS_DC);
static void odb_stream_write_property(zval* obj,zval* prop,zval* value,const zend_literal* key TSRMLS_DC);
static int odb_stream_has_property(zval* obj,zval* prop,int chk_type,const zend_literal* key TSRMLS_DC);

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

// Implementation of php_odb_stream_object

/*static*/ zend_object_handlers php_odb_stream_object::handlers;
php_odb_stream_object::php_odb_stream_object(zend_class_entry* ce TSRMLS_DC):
    stream(nullptr), kind(unset), owner(nullptr), zbackend(nullptr), zts(TSRMLS_C)
{
    zend_object_std_init(this,ce TSRMLS_CC);
    object_properties_init(this,ce);
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

    // Free backend zval (if any). This is typically set when kind == custom.
    if (zbackend != nullptr) {
        zval_ptr_dtor(&zbackend);
    }

    zend_object_std_dtor(this ZTS_MEMBER_CC(zts));
}

void php_odb_stream_object::create_custom_stream(zval* zobj,zval* zbackendObject,unsigned int mode)
{
    // NOTE: this member function should be called under the php-git2 standard
    // exception handler.

    // NOTE: 'zobj' should be any zval that points to an object with 'this' as
    // its backing (i.e. result of zend_objects_get_address()). This is really
    // only used by the implementation to obtain class entry info for the class
    // that was used to create the object AND to keep the PHP object alive while
    // it is being used by git2.

    // Make sure the stream does not already exist.
    if (stream != nullptr) {
        throw php_git2_fatal_exception("cannot create custom ODB stream - object already in use");
    }

    // The stream wrapper is now considered custom.
    kind = custom;

    // Free existing zbackend (just in case).
    if (zbackend != nullptr) {
        zval_ptr_dtor(&zbackend);
    }

    // Assign backend and increment reference count. This keeps the
    // GitODBBackend alive while our GitODBStream is alive.
    zbackend = zbackendObject;
    Z_ADDREF_P(zbackend);

    // Create new custom stream.
    stream = new (emalloc(sizeof(git_odb_stream_php))) git_odb_stream_php(zobj,mode ZTS_MEMBER_CC(zts));

    // Custom ODB streams never require an ODB owner. (The zbackend is the owner
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

/*static*/ void php_odb_stream_object::init(zend_class_entry* ce)
{
    handlers.read_property = odb_stream_read_property;
    handlers.write_property = odb_stream_write_property;
    handlers.has_property = odb_stream_has_property;

    UNUSED(ce);
}

/*static*/ int php_odb_stream_object::read(git_odb_stream *stream,char *buffer,size_t len)
{
    int result;
    zval* zlen;
    method_wrapper method("read",stream);

    MAKE_STD_ZVAL(zlen);
    ZVAL_LONG(zlen,len);

    // Call userspace method implementation of corresponding stream operation.

    zval* params[] = { zlen };

    result = method.call(1,params);
    if (result == GIT_OK) {
        zval* retval = method.retval();
        size_t n;

        // Return values to caller. We copy the data into the git2-provided
        // buffer, making sure not to overflow.

        convert_to_string(retval);

        n = Z_STRLEN_P(retval);
        if (n > len) {
            n = len;
        }

        memcpy(buffer,Z_STRVAL_P(retval),n);
    }

    // Cleanup zvals.

    zval_ptr_dtor(&zlen);

    return result;
}

/*static*/ int php_odb_stream_object::write(git_odb_stream *stream,const char *buffer,size_t len)
{
    int result;
    zval* zbuf;
    method_wrapper method("write",stream);

    MAKE_STD_ZVAL(zbuf);
    ZVAL_STRINGL(zbuf,buffer,len,1);

    // Call userspace method implementation of corresponding stream operation.

    zval* params[] = { zbuf };

    result = method.call(1,params);

    // Cleanup zvals.

    zval_ptr_dtor(&zbuf);

    return result;
}

/*static*/ int php_odb_stream_object::finalize_write(git_odb_stream *stream,const git_oid *oid)
{
    int result;
    zval* zbuf;
    method_wrapper method("finalize_write",stream);

    // Initialize/set zvals.

    MAKE_STD_ZVAL(zbuf);
    convert_oid(zbuf,oid);

    // Call userspace method implementation of corresponding stream operation.

    zval* params[] = { zbuf };

    result = method.call(1,params);

    // Cleanup zvals.

    zval_ptr_dtor(&zbuf);

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
git_odb_stream_php::git_odb_stream_php(zval* zv,unsigned int newMode TSRMLS_DC)
{
    // Blank out the base class (which is the structure defined in the git2
    // headers).
    memset(this,0,sizeof(struct git_odb_stream));

    // Copy the object zval so that we have a new zval we can track that refers
    // to the same specified object. We keep this zval alive as long as the
    // git_odb_stream is alive.
    Z_ADDREF_P(zv);
    thisobj = zv;

    // Get the class entry for the (hopefully) derived class type.
    zend_class_entry* ce = Z_OBJCE_P(thisobj);

    // Make sure the class provided overridden methods needed for the requested
    // mode. If not we raise a fatal error.
    if (mode == GIT_STREAM_RDONLY) {
        if (!is_method_overridden(ce,"read",sizeof("read"))) {
            throw php_git2_fatal_exception("Custom GitODBStream must override read()");
        }
    }
    else if (mode == GIT_STREAM_WRONLY) {
        if (!is_method_overridden(ce,"write",sizeof("write"))) {
            throw php_git2_fatal_exception("Custom GitODBStream must override write()");
        }
        if (!is_method_overridden(ce,"finalize_write",sizeof("finalize_write"))) {
            throw php_git2_fatal_exception("Custom GitODBStream must override finalize_write()");
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
    // Free object zval.
    zval_ptr_dtor(&thisobj);
}

// Implementation of object handlers

zval* odb_stream_read_property(zval* obj,zval* prop,int type,const zend_literal* key TSRMLS_DC)
{
    zval* ret;
    zval* tmp_prop = nullptr;
    const char* str;
    php_odb_stream_object* streamWrapper = LOOKUP_OBJECT(php_odb_stream_object,obj);
    git_odb_stream* stream = streamWrapper->stream;

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

    // Handle special properties of the git_odb_stream.

    str = Z_STRVAL_P(prop);
    if (strcmp(str,"mode") == 0 && stream != nullptr) {
        ALLOC_INIT_ZVAL(ret);
        ZVAL_LONG(ret,stream->mode);
        Z_DELREF_P(ret);
    }
    else if (strcmp(str,"declared_size") == 0 && stream != nullptr) {
        ALLOC_INIT_ZVAL(ret);
        ZVAL_LONG(ret,stream->declared_size);
        Z_DELREF_P(ret);
    }
    else if (strcmp(str,"received_bytes") == 0 && stream != nullptr) {
        ALLOC_INIT_ZVAL(ret);
        ZVAL_LONG(ret,stream->received_bytes);
        Z_DELREF_P(ret);
    }
    else if (strcmp(str,"backend") == 0 && stream != nullptr) {
        if (streamWrapper->zbackend != NULL) {
            ret = streamWrapper->zbackend;
        }
        else {
            // NOTE: In practice, this case should never get executed for custom
            // backends overriding the write/readpack methods since
            // create_custom_stream() should be called in every use case to set
            // the zbackend in internal storage. However it will get called for
            // internal backends or backends that employ the default "fake" ODB
            // streams.

            // NOTE: We can only safely create a backend if an owner is set on
            // the stream.

            ALLOC_INIT_ZVAL(ret);
            if (stream->backend != nullptr && streamWrapper->owner != nullptr) {
                // Create backend object to return to userspace.
                php_git2_make_odb_backend(ret,stream->backend,streamWrapper->owner TSRMLS_CC);

                // Cache the property zval to internal storage for possible
                // lookup later on. Leave the refcount at 1 so we can hold a
                // reference in our 'streamWrapper'. PHP will grab its own
                // reference later on for userspace.
                streamWrapper->zbackend = ret;
            }
            else {
                Z_DELREF_P(ret);
            }
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

void odb_stream_write_property(zval* obj,zval* prop,zval* value,const zend_literal* key TSRMLS_DC)
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
    if (strcmp(str,"mode") == 0
        || strcmp(str,"declared_size") == 0
        || strcmp(str,"received_bytes") == 0
        || strcmp(str,"backend") == 0)
    {
        php_error(E_ERROR,"Property '%s' of GitODBStream cannot be updated",str);
    }
    else {
        (*std_object_handlers.write_property)(obj,prop,value,key TSRMLS_CC);
    }

    if (tmp_prop != nullptr) {
        zval_ptr_dtor(&tmp_prop);
    }
}

int odb_stream_has_property(zval* obj,zval* prop,int chk_type,const zend_literal* key TSRMLS_DC)
{
    int result;
    zval* tmp_prop = nullptr;
    const char* src;
    git_odb_stream* stream = LOOKUP_OBJECT(php_odb_stream_object,obj)->stream;

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
    if (strcmp(src,"mode") == 0 || strcmp(src,"declared_size") == 0
        || strcmp(src,"received_bytes") == 0)
    {
        result = (stream != nullptr);
    }
    else if (strcmp(src,"backend") == 0) {
        if (chk_type == 2) {
            result = (stream != nullptr);
        }
        else {
            result = (stream != nullptr && stream->backend != nullptr);
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

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */