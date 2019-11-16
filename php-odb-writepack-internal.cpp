/**
 * php-odb-writepack-internal.cpp
 *
 * This file is a part of php-git2.
 */

#include "php-object.h"
using namespace php_git2;

// Class method entries

static PHP_METHOD(GitODBWritepack_Internal,append);
static PHP_METHOD(GitODBWritepack_Internal,commit);

zend_function_entry php_git2::odb_writepack_internal_methods[] = {
    PHP_ME(GitODBWritepack_Internal,append,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBWritepack_Internal,commit,NULL,ZEND_ACC_PUBLIC)
    PHP_FE_END
};

// Make function implementation

void php_git2::php_git2_make_odb_writepack(zval* zp,git_odb_writepack* writepack,
    php_callback_sync* cb,zval* zbackend,php_git_odb* owner TSRMLS_DC)
{
    php_odb_writepack_object* obj;
    zend_class_entry* ce = php_git2::class_entry[php_git2_odb_writepack_internal_obj];

    object_init_ex(zp,ce);
    obj = reinterpret_cast<php_odb_writepack_object*>(zend_objects_get_address(zp TSRMLS_CC));

    // Assign the writepack and callback. The callback is merely along for the
    // ride so it can be destroyed at the proper time.
    obj->writepack = writepack;
    obj->cb = cb;

    // Assign owner. If set, this will prevent the parent ODB from freeing
    // during the lifetime of the writepack.
    obj->assign_owner(owner);

    // Assign backend property if we have a backend specified. This will prevent
    // the parent backend from freeing while the writepack is in use.
    if (zbackend != nullptr) {
        Z_ADDREF_P(zbackend);
        zend_hash_add(Z_OBJPROP_P(zp),"backend",sizeof("backend"),&zbackend,sizeof(zval*),NULL);
    }
}

/*static*/ zend_object_handlers php_odb_writepack_internal_object::handlers;
php_odb_writepack_internal_object::php_odb_writepack_internal_object(zend_class_entry* ce TSRMLS_DC):
    php_odb_writepack_object(ce TSRMLS_CC)
{
}

/*static*/ void php_odb_writepack_internal_object::init(zend_class_entry* ce)
{
    handlers.get_constructor = php_git2::not_allowed_get_constructor;

    UNUSED(ce);
}

// Implementation of object methods

PHP_METHOD(GitODBWritepack_Internal,append)
{
    php_bailer bailer ZTS_CTOR;
    int result;
    char* buf;
    int amt;
    php_odb_writepack_object* object = LOOKUP_OBJECT(php_odb_writepack_object,getThis());

    assert(object->writepack != nullptr);

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s",&buf,&amt) != SUCCESS) {
        return;
    }

    try {
        result = object->writepack->append(object->writepack,buf,amt,&object->prog);
        if (result < 0) {
            php_git2::git_error(result);
        }
    } catch (php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer TSRMLS_CC);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle(TSRMLS_C);
        }
    }
}

PHP_METHOD(GitODBWritepack_Internal,commit)
{
    php_bailer bailer ZTS_CTOR;
    int result;
    php_odb_writepack_object* object = LOOKUP_OBJECT(php_odb_writepack_object,getThis());

    assert(object->writepack != nullptr);

    try {
        result = object->writepack->commit(object->writepack,&object->prog);
        if (result < 0) {
            php_git2::git_error(result);
        }
    } catch (php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer TSRMLS_CC);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle(TSRMLS_C);
        }
    }
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
