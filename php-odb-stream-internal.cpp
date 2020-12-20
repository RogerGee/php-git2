/*
 * php-odb-stream-internal.cpp
 *
 * Copyright (C) Roger P. Gee
 */

#include "php-object.h"
using namespace php_git2;

// Class entry

static PHP_METHOD(GitODBStream_Internal,read);
static PHP_METHOD(GitODBStream_Internal,write);
static PHP_METHOD(GitODBStream_Internal,finalize_write);

zend_function_entry php_git2::odb_stream_internal_methods[] = {
    PHP_ME(GitODBStream_Internal,read,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBStream_Internal,write,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBStream_Internal,finalize_write,NULL,ZEND_ACC_PUBLIC)
    PHP_FE_END
};

// Make function implementation

void php_git2::php_git2_make_odb_stream(zval* zp,git_odb_stream* stream,php_git_odb* owner)
{
    php_odb_stream_internal_object* obj;

    // Create object zval.
    object_init_ex(zp,php_git2::class_entry[php_git2_odb_stream_internal_obj]);
    obj = php_zend_object<php_odb_stream_internal_object>::get_storage(Z_OBJ_P(zp));

    // Assign the stream handle.
    obj->stream = stream;

    // Assign owner. (This increments the owner refcount if set to prevent the
    // ODB from freeing while the stream is in use.)
    obj->assign_owner(owner);

    // Assign the kind based on whether the owner was set.
    if (obj->owner != nullptr) {
        obj->kind = php_odb_stream_object::conventional; // stream freed via git_odb_stream_free()
    }
    else {
        obj->kind = php_odb_stream_object::user; // stream freed via $stream->free()
    }
}

// php_zend_object init function

template<>
void php_zend_object<php_odb_stream_internal_object>::init(zend_class_entry* ce)
{
    handlers.get_constructor = php_git2::not_allowed_get_constructor;

    UNUSED(ce);
}

// Implementation of php_odb_stream_internal_object

php_odb_stream_internal_object::php_odb_stream_internal_object()
{
}

// Implementation of class methods

PHP_METHOD(GitODBStream_Internal,read)
{
    /* NOTE: it isn't actually clear how useful this method (and its wrapper,
     * git_odb_stream_read) really are. The documentation seems to indicate that
     * they are not to be used. Plus it is not obvious how to obtain the number
     * of bytes read. For now these functions will return a string zval with the
     * length of the allocated buffer.
     */

    php_bailer bailer;
    int retval;
    char* buffer;
    zend_long bufsz;
    php_odb_stream_internal_object* object;

    object = php_zend_object<php_odb_stream_internal_object>::get_storage(getThis());

    // Stream must be created and the function must be implemented.
    if (object->stream == nullptr || object->stream->read == nullptr) {
        zend_throw_error(nullptr,"GitODBStream_Internal::read(): method is not available");
        return;
    }

    // Parse parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS(),"l",&bufsz) == FAILURE) {
        return;
    }
    if (bufsz <= 0) {
        bufsz = 4096;
    }

    // Call underlying function.
    try {
        buffer = (char*)emalloc(bufsz);
        retval = object->stream->read(object->stream,buffer,bufsz);
        if (retval < 0) {
            efree(buffer);
            php_git2::git_error(retval);
        }
        else {
            RETVAL_STRINGL(buffer,bufsz);
            efree(buffer);
        }

    } catch (php_git2::php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle();
        }
    }
}

PHP_METHOD(GitODBStream_Internal,write)
{
    php_bailer bailer;
    int retval;
    char* buffer;
    size_t bufsz;
    php_odb_stream_internal_object* object;

    object = php_zend_object<php_odb_stream_internal_object>::get_storage(getThis());

    // Stream must be created and the function must be implemented.
    if (object->stream == nullptr || object->stream->write == nullptr) {
        zend_throw_error(nullptr,"GitODBStream_Internal::write(): method is not available");
        return;
    }

    // Parse parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS(),"s",&buffer,&bufsz) == FAILURE) {
        return;
    }

    // Call underlying function.
    try {
        retval = object->stream->write(object->stream,buffer,bufsz);
        if (retval < 0) {
            php_git2::git_error(retval);
        }

    } catch (php_git2::php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle();
        }
    }
}

PHP_METHOD(GitODBStream_Internal,finalize_write)
{
    php_bailer bailer;
    int retval;
    char* input;
    size_t input_len;
    git_oid oid;
    php_odb_stream_internal_object* object;

    object = php_zend_object<php_odb_stream_internal_object>::get_storage(getThis());

    // Stream must be created and the function must be implemented.
    if (object->stream == nullptr || object->stream->finalize_write == nullptr) {
        zend_throw_error(nullptr,"GitODBStream_Internal::finalize_write(): method is not available");
        return;
    }

    // Read OID parameter from function arguments.
    if (zend_parse_parameters(ZEND_NUM_ARGS(),"s",&input,&input_len) == FAILURE) {
        return;
    }

    // Convert OID string to binary structure.
    convert_oid_fromstr(&oid,input,input_len);

    // Call underlying function.
    try {
        retval = object->stream->finalize_write(object->stream,&oid);
        if (retval < 0) {
            php_git2::git_error(retval);
        }

    } catch (php_git2::php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle();
        }
    }
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
