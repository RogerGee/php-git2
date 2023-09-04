/**
 * php-writestream.cpp
 *
 * Copyright (C) Roger P. Gee
 */

#include "php-object.h"
#include "stubs/GitWritestream_arginfo.h"
using namespace php_git2;

// Class method entries

static PHP_METHOD(GitWritestream,write);
static PHP_METHOD(GitWritestream,close);

zend_function_entry php_git2::writestream_methods[] = {
    PHP_ME(
        GitWritestream,
        write,
        arginfo_class_GitWritestream_write,
        ZEND_ACC_PUBLIC
        )

    PHP_ME(
        GitWritestream,
        close,
        arginfo_class_GitWritestream_close,
        ZEND_ACC_PUBLIC
        )

    PHP_FE_END
};

// Make function implementation

void php_git2::php_git2_make_writestream(zval* zp,git_writestream* ws)
{
    php_writestream_object* obj;

    // Create object zval.
    object_init_ex(zp,php_git2::class_entry[php_git2_writestream_obj]);
    obj = php_zend_object<php_writestream_object>::get_storage(zp);
    obj->ws = ws;
}

// php_zend_object init function

template<>
zend_object_handlers php_git2::php_zend_object<php_writestream_object>::handlers;

template<>
void php_zend_object<php_writestream_object>::init(zend_class_entry* ce)
{
    handlers.get_constructor = php_git2::not_allowed_get_constructor;

    handlers.offset = offset();
}

// Implementation of php_writestream_object

php_writestream_object::php_writestream_object():
    ws(nullptr)
{
}

php_writestream_object::~php_writestream_object()
{
    if (ws != nullptr) {
        ws->free(ws);
    }
}

// Userspace method implementations

PHP_METHOD(GitWritestream,write)
{
    php_bailer bailer;
    int retval;
    char* buffer;
    size_t bufsz;
    php_writestream_object* object;

    object = php_zend_object<php_writestream_object>::get_storage(getThis());

    // The writestream must exist and the function must be implemented.
    if (object->ws == nullptr || object->ws->write == nullptr) {
        zend_throw_error(nullptr,"GitWritestream::write(): method is not available");
        return;
    }

    // Parse parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS(),"s",&buffer,&bufsz) == FAILURE) {
        return;
    }

    // Call the underlying function on the writestream.
    try {
        retval = object->ws->write(object->ws,buffer,bufsz);
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

PHP_METHOD(GitWritestream,close)
{
    php_bailer bailer;
    int retval;
    char* buffer;
    size_t bufsz;
    php_writestream_object* object;

    object = php_zend_object<php_writestream_object>::get_storage(getThis());

    // The writestream must exist and the function must be implemented.
    if (object->ws == nullptr || object->ws->close == nullptr) {
        zend_throw_error(nullptr,"GitWritestream::close(): method is not available");
        return;
    }

    // Parse parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS(),"s",&buffer,&bufsz) == FAILURE) {
        return;
    }

    // Call the underlying function on the writestream.
    try {
        retval = object->ws->close(object->ws);
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
