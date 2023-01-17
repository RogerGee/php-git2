/*
 * php-object.cpp
 *
 * Copyright (C) Roger P. Gee
 *
 * This unit provides the generic class registration for classes we wish to
 * provide to PHP userspace. The classes' methods and handlers are defined in
 * their own separate compilation units.
 */

#include "php-object.h"
#include "php-type.h"
using namespace php_git2;

// Provide generic versions of the custom zend_object derivation create/free
// handlers.

template<typename StorageType>
static zend_object* php_create_object_handler(zend_class_entry* ce)
{
    using php_object_t = php_zend_object<StorageType>;
    php_object_t* object;

    // TODO Update to use zend_object_alloc().

    const size_t nbytes = sizeof(php_object_t) + zend_object_properties_size(ce);

    object = new(emalloc(nbytes)) php_object_t(ce);

    return &object->std;
}

template<typename StorageType>
static zend_object* php_clone_object_handler(zval* zv)
{
    zend_object* newObject;
    zend_object* oldObject;

    oldObject = Z_OBJ_P(zv);
    newObject = php_create_object_handler<StorageType>(oldObject->ce);

	zend_objects_clone_members(newObject,oldObject);

    return newObject;
}

template<typename StorageType>
static void php_free_object(zend_object* zo)
{
    using php_object_t = php_zend_object<StorageType>;
    php_object_t::get_wrapper(zo)->~php_object_t();
}

template<typename StorageType>
static void php_init_object_handlers(zend_class_entry* ce)
{
    using php_object_t = php_zend_object<StorageType>;
    const zend_object_handlers* stdhandlers = zend_get_std_object_handlers();

    // Store class entry for later global lookup and set create_object function.
    php_git2::class_entry[StorageType::get_type()] = ce;
    ce->create_object = &php_create_object_handler<StorageType>;

    // Initialize handlers.
    memcpy(&php_object_t::handlers,stdhandlers,sizeof(zend_object_handlers));
    php_object_t::handlers.offset = php_object_t::offset();
    php_object_t::handlers.clone_obj = php_clone_object_handler<StorageType>;
    php_object_t::handlers.free_obj = &php_free_object<StorageType>;
    php_object_t::init(ce);
}

// Define the global list of class entry structure references.

zend_class_entry* php_git2::class_entry[_php_git2_obj_top_];

// Define handlers for any type.

template<typename StorageType>
zend_object_handlers php_git2::php_zend_object<StorageType>::handlers;

// This function registers all classes. It should be called by the MINIT startup
// function.

void php_git2::php_git2_register_classes()
{
    zend_class_entry ce, *pce;

    // abstract class GitODBBackend
    INIT_CLASS_ENTRY(ce,"GitODBBackend",odb_backend_methods);
    pce = zend_register_internal_class(&ce);
    pce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
    php_init_object_handlers<php_odb_backend_object>(pce);

    // final class GitODBBackend_Internal extends GitODBBackend
    INIT_CLASS_ENTRY(ce,"GitODBBackend_Internal",odb_backend_internal_methods);
    pce = zend_register_internal_class_ex(&ce,
        php_git2::class_entry[php_git2_odb_backend_obj]);
    pce->ce_flags |= ZEND_ACC_FINAL;
    php_init_object_handlers<php_odb_backend_internal_object>(pce);

    // abstract class GitODBWritepack
    INIT_CLASS_ENTRY(ce,"GitODBWritepack",odb_writepack_methods);
    pce = zend_register_internal_class(&ce);
    pce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
    php_init_object_handlers<php_odb_writepack_object>(pce);

    // final class GitODBWritepack_Internal extends GitODBWritepack
    INIT_CLASS_ENTRY(ce,"GitODBWritepack_Internal",odb_writepack_internal_methods);
    pce = zend_register_internal_class_ex(&ce,
        php_git2::class_entry[php_git2_odb_writepack_obj]);
    pce->ce_flags |= ZEND_ACC_FINAL;
    php_init_object_handlers<php_odb_writepack_internal_object>(pce);

    // abstract class GitODBStream
    INIT_CLASS_ENTRY(ce,"GitODBStream",odb_stream_methods);
    pce = zend_register_internal_class(&ce);
    pce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
    php_init_object_handlers<php_odb_stream_object>(pce);

    // final class GitODBStream_Internal extends GitODBStream
    INIT_CLASS_ENTRY(ce,"GitODBStream_Internal",odb_stream_internal_methods);
    pce = zend_register_internal_class_ex(&ce,
        php_git2::class_entry[php_git2_odb_stream_obj]);
    pce->ce_flags |= ZEND_ACC_FINAL;
    php_init_object_handlers<php_odb_stream_internal_object>(pce);

    // final class GitWritestream
    INIT_CLASS_ENTRY(ce,"GitWritestream",writestream_methods);
    pce = zend_register_internal_class(&ce);
    pce->ce_flags |= ZEND_ACC_FINAL;
    php_init_object_handlers<php_writestream_object>(pce);

    // abstract class GitConfigBackend
    INIT_CLASS_ENTRY(ce,"GitConfigBackend",config_backend_methods);
    pce = zend_register_internal_class(&ce);
    pce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
    php_init_object_handlers<php_config_backend_object>(pce);

    // abstract class GitRefDBBackend
    INIT_CLASS_ENTRY(ce,"GitRefDBBackend",refdb_backend_methods);
    pce = zend_register_internal_class(&ce);
    pce->ce_flags |= ZEND_ACC_EXPLICIT_ABSTRACT_CLASS;
    php_init_object_handlers<php_refdb_backend_object>(pce);

    // final class GitRefDBBackend_Internal extends GitRefDBBackend
    INIT_CLASS_ENTRY(ce,"GitRefDBBackend_Internal",refdb_backend_internal_methods);
    pce = zend_register_internal_class_ex(&ce,
        php_git2::class_entry[php_git2_refdb_backend_obj]);
    pce->ce_flags |= ZEND_ACC_FINAL;
    php_init_object_handlers<php_refdb_backend_internal_object>(pce);

    // final class GitClosure
    INIT_CLASS_ENTRY(ce,"GitClosure",closure_methods);
    pce = zend_register_internal_class(&ce);
    pce->ce_flags |= ZEND_ACC_FINAL;
    php_init_object_handlers<php_closure_object>(pce);
}

// Provide implementations for generic make function.

void php_git2::php_git2_make_object(zval* zp,php_git2_object_t type)
{
    object_init_ex(zp,php_git2::class_entry[type]);
}

// Helpers

bool php_git2::is_method_overridden(zend_class_entry* ce,const char* method,int len)
{
    zval* func;
    zend_function* fbc;

    func = zend_hash_str_find(&ce->function_table,method,len);
    if (func == nullptr) {
        return false;
    }

    fbc = Z_FUNC_P(func);

    // The method is overridden if the prototype is defined.
    return (fbc->common.prototype != NULL);
}

zend_function* php_git2::not_allowed_get_constructor(zend_object* object)
{
    zend_throw_error(
        nullptr, 
        "Instantiation of '%s' is not allowed",
        ZSTR_VAL(object->ce->name));
    return nullptr;
}

zend_function* php_git2::disallow_base_get_constructor(zend_object* object)
{
    if (!object->ce->parent) {
        return not_allowed_get_constructor(object);
    }

    return object->ce->constructor;
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
