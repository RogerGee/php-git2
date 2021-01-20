/*
 * php-closure.cpp
 *
 * Copyright (C) Roger P. Gee
 */

#include "php-object.h"
using namespace php_git2;

// Custom class handlers

static int closure_get_closure(zval* obj,
    zend_class_entry** ce_ptr,
    zend_function** fptr_ptr,
    zend_object** obj_ptr);

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
    if (func.type != 0) {
        destroy_zend_function(&func);
    }
    if (payloadDestructor != nullptr) {
        (*payloadDestructor)(payload);
    }
}

// Implementation of custom class handlers

int closure_get_closure(zval* obj,
    zend_class_entry** ce_ptr,
    zend_function** fptr_ptr,
    zend_object** obj_ptr)
{
    if (Z_TYPE_P(obj) != IS_OBJECT) {
        return FAILURE;
    }

    php_closure_object* closure;
    closure = php_zend_object<php_closure_object>::get_storage(obj);

    *fptr_ptr = &closure->func;
    *ce_ptr = Z_OBJCE_P(obj);
    if (obj_ptr) {
        *obj_ptr = Z_OBJ_P(obj);
    }

    return SUCCESS;
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
