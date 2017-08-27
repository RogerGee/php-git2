/*
 * php-config-backend.cpp
 *
 * This file is a part of php-git2.
 *
 * This unit provides the out-of-line implementation for the GitConfigBackend
 * class.
 */

#include "php-object.h"
#include <new>
using namespace php_git2;

// Helper macros

#define EXTRACT_BACKEND(backend) \
    reinterpret_cast<php_config_backend_object::git_config_backend_php*>(backend)

#define EXTRACT_THISOBJ(backend) \
    EXTRACT_BACKEND(backend)->thisobj

// Class method entries

zend_function_entry php_git2::config_backend_methods[] = {

    PHP_FE_END
};

// Implementation of php_config_backend_object

/*static*/ void php_config_backend_object::init(zend_class_entry* ce)
{

}

/*static*/ zend_object_handlers php_config_backend_object::handlers;
php_config_backend_object::php_config_backend_object(zend_class_entry* ce TSRMLS_DC):
    backend(nullptr), zts(TSRMLS_C)
{
    zend_object_std_init(&base,ce TSRMLS_CC);
    object_properties_init(&base,ce);
}

php_config_backend_object::~php_config_backend_object()
{

    zend_object_std_dtor(&base ZTS_MEMBER_CC(this->zts));
}

// Implementation of class methods
