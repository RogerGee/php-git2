/*
 * php-odb-stream-internal.cpp
 *
 * This file is a part of php-git2.
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

void php_git2::php_git2_make_odb_stream(zval* zp,git_odb_stream* stream,php_git_odb* owner TSRMLS_DC)
{
    php_odb_stream_internal_object* obj;
    zend_class_entry* ce = php_git2::class_entry[php_git2_odb_stream_internal_obj];

    // Create object zval.
    object_init_ex(zp,ce);
    obj = reinterpret_cast<php_odb_stream_internal_object*>(zend_objects_get_address(zp TSRMLS_CC));

    // Assign the stream handle.
    obj->stream = stream;
    obj->owner = owner;

    // Increment owner refcount if set. This will prevent the ODB from freeing
    // while the writestream is in use.
    if (obj->owner != nullptr) {
        obj->owner->up_ref();
    }
}

// Implementation of php_odb_stream_internal_object

/*static*/ zend_object_handlers php_odb_stream_internal_object::handlers;
php_odb_stream_internal_object::php_odb_stream_internal_object(zend_class_entry* ce TSRMLS_DC):
    php_odb_stream_object(ce TSRMLS_CC)
{
}

/*static*/ void php_odb_stream_internal_object::init(zend_class_entry* ce)
{
    handlers.get_constructor = php_git2::not_allowed_get_constructor;

    UNUSED(ce);
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
    long bufsz;
    php_odb_stream_object* object = LOOKUP_OBJECT(php_odb_stream_object,getThis());

    // Stream must be created and the function must be implemented.
    if (object->stream == nullptr || object->stream->read == nullptr) {
        php_error(E_ERROR,"GitODBStream_Internal::read(): method is not available");
        return;
    }

    // Parse parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"l",&bufsz) == FAILURE) {
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
            php_git2::git_error(retval);
        }
        else {
            RETVAL_STRINGL(buffer,bufsz,0);
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
    int bufsz;
    php_odb_stream_object* object = LOOKUP_OBJECT(php_odb_stream_object,getThis());

    // Stream must be created and the function must be implemented.
    if (object->stream == nullptr || object->stream->write == nullptr) {
        php_error(E_ERROR,"GitODBStream_Internal::write(): method is not available");
        return;
    }

    // Parse parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s",&buffer,&bufsz) == FAILURE) {
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
    int input_len;
    git_oid oid;
    php_odb_stream_object* object = LOOKUP_OBJECT(php_odb_stream_object,getThis());

    // Stream must be created and the function must be implemented.
    if (object->stream == nullptr || object->stream->finalize_write == nullptr) {
        php_error(E_ERROR,"GitODBStream_Internal::finalize_write(): method is not available");
        return;
    }

    // Read OID parameter from function arguments.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s",&input,&input_len) == FAILURE) {
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
