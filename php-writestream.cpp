/**
 * php-writestream.cpp
 *
 * This file is a part of php-git2.
 */

#include "php-object.h"
using namespace php_git2;

// Class method entries
static PHP_METHOD(GitWritestream,write);
static PHP_METHOD(GitWritestream,close);
static PHP_METHOD(GitWritestream,free);
zend_function_entry php_git2::writestream_methods[] = {
    PHP_ME(GitWritestream,write,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitWritestream,close,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitWritestream,free,NULL,ZEND_ACC_PUBLIC)
    PHP_FE_END
};

// Make function implementation

void php_git2::php_git2_make_writestream(zval* zp,git_writestream* ws TSRMLS_DC)
{
    php_writestream_object* obj;
    zend_class_entry* ce = php_git2::class_entry[php_git2_writestream_obj];

    // Create object zval.
    object_init_ex(zp,ce);
    obj = reinterpret_cast<php_writestream_object*>(zend_objects_get_address(zp TSRMLS_CC));

    obj->ws = ws;
}

// Implementation of php_writestream_object

/*static*/ zend_object_handlers php_writestream_object::handlers;
php_writestream_object::php_writestream_object(zend_class_entry* ce TSRMLS_DC):
    ws(nullptr), zts(TSRMLS_C)
{
    zend_object_std_init(&base,ce TSRMLS_CC);
    object_properties_init(&base,ce);
}

php_writestream_object::~php_writestream_object()
{
    if (ws != nullptr) {
        ws->free(ws);
    }

    zend_object_std_dtor(&base ZTS_MEMBER_CC(zts));
}

/*static*/ void php_writestream_object::init(zend_class_entry* ce)
{
    // Nothing to do...
}

// Userspace method implementations

PHP_METHOD(GitWritestream,write)
{
    int retval;
    char* buffer;
    int bufsz;
    php_writestream_object* object = LOOKUP_OBJECT(php_writestream_object,getThis());

    // The writestream must exist and the function must be implemented.
    if (object->ws == nullptr || object->ws->write == nullptr) {
        php_error(E_ERROR,"GitWritestream::write(): method is not available");
        return;
    }

    // Parse parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s",&buffer,&bufsz) == FAILURE) {
        return;
    }

    // Call the underlying function on the writestream.
    try {
        retval = object->ws->write(object->ws,buffer,bufsz);
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

PHP_METHOD(GitWritestream,close)
{
    int retval;
    char* buffer;
    int bufsz;
    php_writestream_object* object = LOOKUP_OBJECT(php_writestream_object,getThis());

    // The writestream must exist and the function must be implemented.
    if (object->ws == nullptr || object->ws->close == nullptr) {
        php_error(E_ERROR,"GitWritestream::close(): method is not available");
        return;
    }

    // Parse parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s",&buffer,&bufsz) == FAILURE) {
        return;
    }

    // Call the underlying function on the writestream.
    try {
        retval = object->ws->close(object->ws);
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

PHP_METHOD(GitWritestream,free)
{
    // NOOP - we don't want the user explicitly controlling the freeing of this
    // object.
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
