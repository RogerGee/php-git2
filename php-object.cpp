/*
 * php-object.cpp
 *
 * This file is a part of php-git2.
 *
 * This unit provides the generic class registration for classes we which to
 * provide to PHP userspace. Each class's methods and handlers are defined in
 * their own compilation unit.
 */

#include "php-object.h"
#include "php-type.h"
#include <new>
#include <cstring>
using namespace php_git2;

// Provide generic versions of the custom zend_object derivation create/free
// handlers.

template<typename T>
static void php_free_object(T* object)
{
    object->~T();
    efree(object);
}

template<typename T>
static zend_object_value php_create_object_handler(zend_class_entry* ce TSRMLS_DC)
{
    T* object;
    zend_object_value val;

    object = new(emalloc(sizeof(T))) T(TSRMLS_C);

    memset(&val,0,sizeof(zend_object_value));
    val.handle = zend_objects_store_put(object,
        (zend_objects_store_dtor_t)zend_objects_destroy_object,
        (zend_objects_free_object_storage_t)php_free_object<T>,
        nullptr TSRMLS_CC);
    val.handlers = &T::handlers;

    return val;
    (void)ce;
}

// Define the global list of class entry structure references.

zend_class_entry* php_git2::class_entry[_php_git2_obj_top_];

// This function registers all classes. It should be called by the MINIT startup
// function.

void php_git2::php_git2_register_classes(TSRMLS_D)
{
    zend_object_handlers* stdhandlers = zend_get_std_object_handlers();
    zend_class_entry ce, *pce;

    // ODB_WRITEPACK
    INIT_CLASS_ENTRY(ce,"GitODBWritepack",odb_writepack_methods);
    pce = zend_register_internal_class(&ce TSRMLS_CC);
    pce->create_object = php_create_object_handler<php_odb_writepack_object>;
    php_git2::class_entry[php_git2_odb_writepack_obj] = pce;
    memcpy(&php_odb_writepack_object::handlers,stdhandlers,sizeof(zend_object_handlers));
    php_odb_writepack_object::init(pce);

    // ODB_BACKEND
    INIT_CLASS_ENTRY(ce,"GitODBBackend",odb_backend_methods);
    pce = zend_register_internal_class(&ce TSRMLS_CC);
    pce->create_object = php_create_object_handler<php_odb_backend_object>;
    php_git2::class_entry[php_git2_odb_backend_obj] = pce;
    memcpy(&php_odb_backend_object::handlers,stdhandlers,sizeof(zend_object_handlers));
    php_odb_backend_object::init(pce);
}

// Provide implementations for generic make function.

void php_git2::php_git2_make_object(zval* zp,php_git2_object_t type TSRMLS_DC)
{
    object_init_ex(zp,php_git2::class_entry[type]);
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
