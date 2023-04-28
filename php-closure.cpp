/*
 * php-closure.cpp
 *
 * Copyright (C) Roger P. Gee
 */

#include "php-object.h"
using namespace php_git2;

// Custom class handlers

#if PHP_API_VERSION >= 20220829
static zend_result closure_get_closure(
    zend_object* obj,
    zend_class_entry** ce_ptr,
    zend_function** fptr_ptr,
    zend_object** obj_ptr,
    zend_bool check_only);
#else
static int closure_get_closure(
    zend_object* obj,
    zend_class_entry** ce_ptr,
    zend_function** fptr_ptr,
    zend_object** obj_ptr,
    zend_bool check_only);
#endif

// Class method entries

zend_function_entry php_git2::closure_methods[] = {
    PHP_FE_END
};

// php_zend_object init function

template<>
zend_object_handlers php_git2::php_zend_object<php_closure_object>::handlers;

template<>
void php_zend_object<php_closure_object>::init(zend_class_entry* ce)
{
    handlers.get_closure = closure_get_closure;
    handlers.get_constructor = php_git2::not_allowed_get_constructor;

    handlers.offset = offset();

    UNUSED(ce);
}

// Implementation of php_closure_object

php_closure_object::php_closure_object():
    payload(nullptr), hasPayload(false), payloadDestructor(nullptr)
{
    memset(&func,0,sizeof(zend_function));
}

php_closure_object::~php_closure_object()
{
    // NOTE: Currently, the only allocated item from 'func' is the function name
    // string.
    if (func.common.function_name) {
        zend_string_release(func.common.function_name);
    }

    if (payloadDestructor != nullptr) {
        (*payloadDestructor)(payload);
    }
}

// Implementation of custom class handlers

#if PHP_API_VERSION >= 20220829
zend_result closure_get_closure(
    zend_object* obj,
    zend_class_entry** ce_ptr,
    zend_function** fptr_ptr,
    zend_object** obj_ptr,
    zend_bool check_only)
#else
int closure_get_closure(
    zend_object* obj,
    zend_class_entry** ce_ptr,
    zend_function** fptr_ptr,
    zend_object** obj_ptr,
    zend_bool check_only)
#endif
{
    php_closure_object* closure;
    closure = php_zend_object<php_closure_object>::get_storage(obj);

    *fptr_ptr = &closure->func;
    *ce_ptr = obj->ce;
    if (obj_ptr) {
        *obj_ptr = obj;
    }

    return SUCCESS;
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
