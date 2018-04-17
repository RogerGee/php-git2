/*
 * php-closure.cpp
 *
 * This file is a part of php-git2.
 */

#include "php-object.h"
using namespace php_git2;

// Custom class handlers

static int closure_get_closure(zval* obj,zend_class_entry** ce_ptr,zend_function** fptr_ptr,
    zval** zobj_ptr TSRMLS_DC);

// Class method entries

zend_function_entry php_git2::closure_methods[] = {
    PHP_FE_END
};

// Implementation of php_closure_object

/*static*/ zend_object_handlers php_closure_object::handlers;
php_closure_object::php_closure_object(zend_class_entry* ce TSRMLS_DC):
    payload(nullptr), hasPayload(false), payloadDestructor(nullptr), zts(TSRMLS_C)
{
    zend_object_std_init(this,ce TSRMLS_CC);
    object_properties_init(this,ce);
    memset(&func,0,sizeof(zend_function));
}

php_closure_object::~php_closure_object()
{
    if (payloadDestructor != nullptr) {
        (*payloadDestructor)(payload);
    }

    zend_object_std_dtor(this ZTS_MEMBER_CC(this->zts));
}

/*static*/ void php_closure_object::init(zend_class_entry* ce)
{
    handlers.get_closure = closure_get_closure;
    handlers.get_constructor = php_git2::not_allowed_get_constructor;
    (void)ce;
}

// Implementation of custom class handlers

int closure_get_closure(zval* obj,zend_class_entry** ce_ptr,zend_function** fptr_ptr,
    zval** zobj_ptr TSRMLS_DC)
{
    if (Z_TYPE_P(obj) != IS_OBJECT) {
        return FAILURE;
    }

    php_closure_object* closure = LOOKUP_OBJECT(php_closure_object,obj);

    *fptr_ptr = &closure->func;
    *ce_ptr = Z_OBJCE_P(obj); // i.e. php_git2::class_entry[php_git2_closure_obj]
    if (zobj_ptr) {
        Z_DELREF_P(obj);
        *zobj_ptr = obj;
    }

    return SUCCESS;
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
