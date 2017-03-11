/*
 * php-odb-stream.cpp
 *
 * This file is a part of php-git2.
 */

#include "php-object.h"
using namespace php_git2;

// Class method entries
static PHP_METHOD(GitODBStream,read);
static PHP_METHOD(GitODBStream,write);
static PHP_METHOD(GitODBStream,finalized_write);
static PHP_METHOD(GitODBStream,free);
zend_function_entry php_git2::odb_stream_methods[] = {
    PHP_ME(GitODBStream,read,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBStream,write,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBStream,finalized_write,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBStream,free,NULL,ZEND_ACC_PUBLIC)
    PHP_FE_END
};

// Make function implementation

void php_git2::php_git2_make_odb_stream(zval* zp,git_odb_stream* stream TSRMLS_DC)
{
    php_odb_stream_object* obj;
    zend_class_entry* ce = php_git2::class_entry[php_git2_odb_stream_obj];

    // Create object zval.
    object_init_ex(zp,ce);
    obj = reinterpret_cast<php_odb_stream_object*>(zend_objects_get_address(zp TSRMLS_CC));

    // Assign the stream handle.
    obj->stream = stream;
}

// Implementation of php_odb_stream_object

/*static*/ zend_object_handlers php_odb_stream_object::handlers;
php_odb_stream_object::php_odb_stream_object(TSRMLS_D):
    stream(nullptr), zts(TSRMLS_C)
{
    zend_class_entry* ce = php_git2::class_entry[php_git2_odb_stream_obj];
    zend_object_std_init(&base,ce TSRMLS_CC);
    object_properties_init(&base,ce);
}

php_odb_stream_object::~php_odb_stream_object()
{
    if (stream != nullptr) {
        stream->free(stream);
    }

    zend_object_std_dtor(&base ZTS_MEMBER_CC(zts));
}

/*static*/ void php_odb_stream_object::init(zend_class_entry* ce)
{
    // Register properties.
    zend_declare_property_long(ce,"mode",sizeof("mode")-1,0,ZEND_ACC_PUBLIC);
    zend_declare_property_long(ce,"declared_size",sizeof("declared_size")-1,0,
        ZEND_ACC_PUBLIC);
    zend_declare_property_long(ce,"received_bytes",sizeof("received_bytes")-1,0,
        ZEND_ACC_PUBLIC);
}

// Implementation of class methods

PHP_METHOD(GitODBStream,read)
{
}

PHP_METHOD(GitODBStream,write)
{
}

PHP_METHOD(GitODBStream,finalized_write)
{
}

PHP_METHOD(GitODBStream,free)
{
}
