/*
 * php-object.cpp
 *
 * This file is a part of php-git2.
 *
 * This unit provides the generic class registration for classes we wish to
 * provide to PHP userspace. The classes' methods and handlers are defined in
 * their own separate compilation units.
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

    object = new(emalloc(sizeof(T))) T(ce TSRMLS_CC);

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

    // final class GitODBWritepack
    INIT_CLASS_ENTRY(ce,"GitODBWritepack",odb_writepack_methods);
    pce = zend_register_internal_class(&ce TSRMLS_CC);
    php_git2::class_entry[php_git2_odb_writepack_obj] = pce;
    pce->ce_flags |= ZEND_ACC_FINAL_CLASS;
    memcpy(&php_odb_writepack_object::handlers,stdhandlers,sizeof(zend_object_handlers));
    pce->create_object = php_create_object_handler<php_odb_writepack_object>;
    php_odb_writepack_object::init(pce);

    // abstract class GitODBBackend
    INIT_CLASS_ENTRY(ce,"GitODBBackend",odb_backend_methods);
    pce = zend_register_internal_class(&ce TSRMLS_CC);
    php_git2::class_entry[php_git2_odb_backend_obj] = pce;
    pce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
    memcpy(&php_odb_backend_object::handlers,stdhandlers,sizeof(zend_object_handlers));
    pce->create_object = php_create_object_handler<php_odb_backend_object>;
    php_odb_backend_object::init(pce);

    // final class GitODBBackend_Internal extends GitODBBackend
    INIT_CLASS_ENTRY(ce,"GitODBBackend_Internal",odb_backend_internal_methods);
    pce = zend_register_internal_class_ex(
        &ce,
        php_git2::class_entry[php_git2_odb_backend_obj],
        nullptr TSRMLS_CC);
    php_git2::class_entry[php_git2_odb_backend_internal_obj] = pce;
    pce->ce_flags |= ZEND_ACC_FINAL_CLASS;
    memcpy(&php_odb_backend_internal_object::handlers,
        &php_odb_backend_object::handlers,
        sizeof(zend_object_handlers));
    pce->create_object = php_create_object_handler<php_odb_backend_internal_object>;
    php_odb_backend_internal_object::init(pce);

    // abstract class GitODBStream
    INIT_CLASS_ENTRY(ce,"GitODBStream",odb_stream_methods);
    pce = zend_register_internal_class(&ce TSRMLS_CC);
    php_git2::class_entry[php_git2_odb_stream_obj] = pce;
    pce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
    memcpy(&php_odb_stream_object::handlers,stdhandlers,sizeof(zend_object_handlers));
    pce->create_object = php_create_object_handler<php_odb_stream_object>;
    php_odb_stream_object::init(pce);

    // final class GitODBStream_Internal extends GitODBStream
    INIT_CLASS_ENTRY(ce,"GitODBStream_Internal",odb_stream_internal_methods);
    pce = zend_register_internal_class_ex(
        &ce,
        php_git2::class_entry[php_git2_odb_stream_obj],
        nullptr TSRMLS_CC);
    php_git2::class_entry[php_git2_odb_stream_internal_obj] = pce;
    pce->ce_flags |= ZEND_ACC_FINAL_CLASS;
    memcpy(&php_odb_stream_internal_object::handlers,
        &php_odb_stream_object::handlers,
        sizeof(zend_object_handlers));
    pce->create_object = php_create_object_handler<php_odb_stream_internal_object>;
    php_odb_stream_internal_object::init(pce);

    // final class GitWritestream
    INIT_CLASS_ENTRY(ce,"GitWritestream",writestream_methods);
    pce = zend_register_internal_class(&ce TSRMLS_CC);
    php_git2::class_entry[php_git2_writestream_obj] = pce;
    pce->ce_flags |= ZEND_ACC_FINAL_CLASS;
    memcpy(&php_writestream_object::handlers,stdhandlers,sizeof(zend_object_handlers));
    pce->create_object = php_create_object_handler<php_writestream_object>;
    php_writestream_object::init(pce);

    // abstract class GitConfigBackend
    INIT_CLASS_ENTRY(ce,"GitConfigBackend",config_backend_methods);
    pce = zend_register_internal_class(&ce TSRMLS_CC);
    php_git2::class_entry[php_git2_config_backend_obj] = pce;
    pce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
    memcpy(&php_config_backend_object::handlers,stdhandlers,sizeof(zend_object_handlers));
    pce->create_object = php_create_object_handler<php_config_backend_object>;
    php_config_backend_object::init(pce);

    // abstract class GitRefDBBackend
    INIT_CLASS_ENTRY(ce,"GitRefDBBackend",refdb_backend_methods);
    pce = zend_register_internal_class(&ce TSRMLS_CC);
    php_git2::class_entry[php_git2_refdb_backend_obj] = pce;
    pce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
    memcpy(&php_refdb_backend_object::handlers,stdhandlers,sizeof(zend_object_handlers));
    pce->create_object = php_create_object_handler<php_refdb_backend_object>;
    php_refdb_backend_object::init(pce);

    // final class GitRefDBBackend_Internal extends GitRefDBBackend
    INIT_CLASS_ENTRY(ce,"GitRefDBBackend_Internal",refdb_backend_internal_methods);
    pce = zend_register_internal_class_ex(
        &ce,
        php_git2::class_entry[php_git2_refdb_backend_obj],
        nullptr TSRMLS_CC);
    php_git2::class_entry[php_git2_refdb_backend_internal_obj] = pce;
    pce->ce_flags |= ZEND_ACC_FINAL_CLASS;
    memcpy(&php_refdb_backend_internal_object::handlers,
        &php_refdb_backend_object::handlers,
        sizeof(zend_object_handlers));
    pce->create_object = php_create_object_handler<php_refdb_backend_internal_object>;
    php_refdb_backend_internal_object::init(pce);

    // final class GitClosure
    INIT_CLASS_ENTRY(ce,"GitClosure",closure_methods);
    pce = zend_register_internal_class(&ce TSRMLS_CC);
    php_git2::class_entry[php_git2_closure_obj] = pce;
    pce->ce_flags |= ZEND_ACC_FINAL_CLASS;
    memcpy(&php_closure_object::handlers,stdhandlers,sizeof(zend_object_handlers));
    pce->create_object = php_create_object_handler<php_closure_object>;
    php_closure_object::init(pce);
}

// Provide implementations for generic make function.

void php_git2::php_git2_make_object(zval* zp,php_git2_object_t type TSRMLS_DC)
{
    object_init_ex(zp,php_git2::class_entry[type]);
}

// Helpers

bool php_git2::is_method_overridden(zend_class_entry* ce,const char* method,int len)
{
    zend_function* func;

    // Look up the function entry in the class entry's function table.
    if (zend_hash_find(&ce->function_table,method,len,(void**)&func) == FAILURE) {
        return false;
    }

    // The method is overloaded if the prototype is defined.
    return (func->common.prototype != NULL);
}

zend_function* php_git2::not_allowed_get_constructor(zval* object TSRMLS_DC)
{
    zend_class_entry* ce = Z_OBJCE_P(object);
    php_error(E_RECOVERABLE_ERROR, "Instantiation of '%s' is not allowed",ce->name);
    return nullptr;
}

zend_function* php_git2::disallow_base_get_constructor(zval* object TSRMLS_DC)
{
    zend_class_entry* ce = Z_OBJCE_P(object);
    if (!ce->parent) {
        return not_allowed_get_constructor(object);
    }
    return ce->constructor;
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
