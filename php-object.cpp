/*
 * php-object.cpp
 *
 * This file is a part of php-git2.
 *
 * This unit provides the out-of-line implementations for the various custom
 * object types used by php-git2. These classes should never be instantiated
 * directly by userspace. They are only used to help us conform to the git2 API
 * as much as possible.
 */

#include "php-object.h"
#include "php-type.h"
#include <new>
#include <cstring>
using namespace php_git2;

// Helper macros

#define LOOKUP_OBJECT(type,val)                         \
    (type*)zend_object_store_get_object(val TSRMLS_CC)

// Forward declare all class methods and provide lists of function entries for
// each class.

// odb_writepack
static PHP_METHOD(ODB_WRITEPACK,append);
static PHP_METHOD(ODB_WRITEPACK,commit);
static PHP_METHOD(ODB_WRITEPACK,free);
static zend_function_entry odb_writepack_methods[] = {
    PHP_ME(ODB_WRITEPACK,append,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(ODB_WRITEPACK,commit,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(ODB_WRITEPACK,free,NULL,ZEND_ACC_PUBLIC)
    PHP_FE_END
};

// Provide generic version of the custom zend_object derivation create/free
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

    object = new(emalloc(sizeof(T))) T;

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

static zend_class_entry* g_Classes[_php_git2_obj_top_];

// Implementation of custom zend_object derivations.

// php_odb_writepack_object

/*static*/ zend_object_handlers php_odb_writepack_object::handlers;
php_odb_writepack_object::php_odb_writepack_object():
    writepack(nullptr), cb(nullptr)
{
    zend_class_entry* ce = g_Classes[php_git2_odb_writepack_obj];
    zend_object_std_init(&base,ce TSRMLS_CC);
    object_properties_init(&base,ce);
    memset(&prog,0,sizeof(git_transfer_progress));
}

php_odb_writepack_object::~php_odb_writepack_object()
{
    if (writepack != nullptr) {
        writepack->free(writepack);
    }
    if (cb != nullptr) {
        cb->~php_callback_sync();
        efree(cb);
    }
    zend_object_std_dtor(&base TSRMLS_CC);
}

// This function registers all classes. It should be called by the MINIT startup
// function.

void php_git2::php_git2_register_classes()
{
    zend_object_handlers* stdhandlers = zend_get_std_object_handlers();
    zend_class_entry ce, *pce;

    // ODB_WRITEPACK
    INIT_CLASS_ENTRY(ce,"_php_git2_odb_writepack_",odb_writepack_methods);
    pce = zend_register_internal_class(&ce TSRMLS_CC);
    pce->create_object = php_create_object_handler<php_odb_writepack_object>;
    g_Classes[php_git2_odb_writepack_obj] = pce;
    memcpy(&php_odb_writepack_object::handlers,stdhandlers,sizeof(zend_object_handlers));
}

// Provide implementations for the creation functions.

void php_git2::php_git2_make_object(zval* zp,php_git2_object_t type)
{
    object_init_ex(zp,g_Classes[type]);
}

void php_git2::php_git2_make_odb_writepack(zval* zp,git_odb_writepack* writepack,
    php_callback_sync* cb)
{
    zval* zbackend;
    php_resource_ref<php_git_odb_backend> backend;
    php_odb_writepack_object* obj;

    object_init_ex(zp,g_Classes[php_git2_odb_writepack_obj]);
    obj = reinterpret_cast<php_odb_writepack_object*>(zend_objects_get_address(zp));

    // Assign the writepack and callback. The callback is merely along for the
    // ride so it can be destroyed at the proper time.
    obj->writepack = writepack;
    obj->cb = cb;

    // Create 'backend' property for object. We have to create a resource zval
    // for this purpose.
    *backend.byval_git2() = writepack->backend;
    zbackend = backend.byval_php();
    add_property_resource_ex(zp,"backend",sizeof("backend"),
        Z_RESVAL_P(zbackend));
}

// Implementation of object methods

// odb_writepack

PHP_METHOD(ODB_WRITEPACK,append)
{
    int result;
    char* buf;
    int amt;
    php_odb_writepack_object* object;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s",&buf,&amt) != SUCCESS) {
        return;
    }

    object = LOOKUP_OBJECT(php_odb_writepack_object,getThis());
    if (object->writepack != nullptr) {
        result = object->writepack->append(object->writepack,buf,amt,&object->prog);
        RETVAL_LONG(result);
    }
}

PHP_METHOD(ODB_WRITEPACK,commit)
{
    int result;
    php_odb_writepack_object* object;

    object = LOOKUP_OBJECT(php_odb_writepack_object,getThis());
    if (object->writepack != nullptr) {
        result = object->writepack->commit(object->writepack,&object->prog);
        RETVAL_LONG(result);
    }
}

PHP_METHOD(ODB_WRITEPACK,free)
{
    php_odb_writepack_object* object;

    object = LOOKUP_OBJECT(php_odb_writepack_object,getThis());
    if (object->writepack != nullptr) {
        object->writepack->free(object->writepack);
        object->writepack = nullptr;
    }    
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
