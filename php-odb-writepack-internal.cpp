/**
 * php-odb-writepack-internal.cpp
 *
 * Copyright (C) Roger P. Gee
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

// Custom class handlers.

static zval* odb_writepack_internal_read_property(zval* obj,zval* prop,int type,const zend_literal* key TSRMLS_DC);
static void odb_writepack_internal_write_property(zval* obj,zval* prop,zval* value,const zend_literal* key TSRMLS_DC);
static int odb_writepack_internal_has_property(zval* obj,zval* prop,int chk_type,const zend_literal* key TSRMLS_DC);

// Make function implementation

void php_git2::php_git2_make_odb_writepack(zval* zp,git_odb_writepack* writepack,
    php_callback_sync* cb,zval* zbackend,php_git_odb* owner TSRMLS_DC)
{
    php_odb_writepack_internal_object* obj;
    zend_class_entry* ce = php_git2::class_entry[php_git2_odb_writepack_internal_obj];

    object_init_ex(zp,ce);
    obj = reinterpret_cast<php_odb_writepack_internal_object*>(zend_objects_get_address(zp TSRMLS_CC));

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
        zend_hash_add(Z_OBJPROP_P(zp),"backend",sizeof("backend"),&zbackend,sizeof(zval*),nullptr);
    }
}

/*static*/ zend_object_handlers php_odb_writepack_internal_object::handlers;
php_odb_writepack_internal_object::php_odb_writepack_internal_object(zend_class_entry* ce TSRMLS_DC):
    php_odb_writepack_object(ce TSRMLS_CC), cb(nullptr)
{
    memset(&prog,0,sizeof(git_transfer_progress));
}

php_odb_writepack_internal_object::~php_odb_writepack_internal_object()
{
    // Free the writepack. Note: we always are responsible for freeing the
    // writepack from an internal object.
    if (writepack != nullptr) {
        writepack->free(writepack);
    }

    // Destroy any associated callback.
    if (cb != nullptr) {
        cb->~php_callback_sync();
        efree(cb);
    }
}

/*static*/ void php_odb_writepack_internal_object::init(zend_class_entry* ce)
{
    handlers.read_property = odb_writepack_internal_read_property;
    handlers.write_property = odb_writepack_internal_write_property;
    handlers.has_property = odb_writepack_internal_has_property;
    handlers.get_constructor = php_git2::not_allowed_get_constructor;

    UNUSED(ce);
}

// Implementation of custom class handlers.

zval* odb_writepack_internal_read_property(zval* obj,zval* prop,int type,const zend_literal* key TSRMLS_DC)
{
    zval* ret;
    zval* tmp_prop = nullptr;
    const char* str;
    php_odb_writepack_internal_object* wrapper;

    wrapper = LOOKUP_OBJECT(php_odb_writepack_internal_object,obj);

    // Ensure deep copy of member zval.
    if (Z_TYPE_P(prop) != IS_STRING) {
        MAKE_STD_ZVAL(tmp_prop);
        *tmp_prop = *prop;
        INIT_PZVAL(tmp_prop);
        zval_copy_ctor(tmp_prop);
        convert_to_string(tmp_prop);
        prop = tmp_prop;
        key = NULL;
    }

    // Handle special properties of git_odb_writepack.

    str = Z_STRVAL_P(prop);
    if (strcmp(str,"progress") == 0) {
        // NOTE: We create a new zval every time since this property is readonly
        // and the value may change.
        MAKE_STD_ZVAL(ret);
        php_git2::convert_transfer_progress(ret,&wrapper->prog);
    }
    else {
        // Invoke base class handler.
        ret = (*php_odb_writepack_object::handlers.read_property)(obj,prop,type,key TSRMLS_CC);
    }

    if (tmp_prop != nullptr) {
        Z_ADDREF_P(ret);
        zval_ptr_dtor(&tmp_prop);
        Z_DELREF_P(ret);
    }

    return ret;
}

void odb_writepack_internal_write_property(zval* obj,zval* prop,zval* value,const zend_literal* key TSRMLS_DC)
{
    zval* tmp_prop = nullptr;
    const char* str;

    // Ensure deep copy of member zval.
    if (Z_TYPE_P(prop) != IS_STRING) {
        MAKE_STD_ZVAL(tmp_prop);
        *tmp_prop = *prop;
        INIT_PZVAL(tmp_prop);
        zval_copy_ctor(tmp_prop);
        convert_to_string(tmp_prop);
        prop = tmp_prop;
        key = NULL;
    }

    str = Z_STRVAL_P(prop);
    if (strcmp(str,"progress") == 0) {
        php_error(E_ERROR,"Property '%s' of GitODBWritepack_Internal cannot be updated",str);
    }
    else {
        // Invoke base class handler.
        (*php_odb_writepack_object::handlers.write_property)(obj,prop,value,key TSRMLS_CC);
    }

    if (tmp_prop != nullptr) {
        zval_ptr_dtor(&tmp_prop);
    }
}

int odb_writepack_internal_has_property(zval* obj,zval* prop,int chk_type,const zend_literal* key TSRMLS_DC)
{
    int result;
    zval* tmp_prop = nullptr;
    const char* src;

    // Ensure deep copy of member zval.
    if (Z_TYPE_P(prop) != IS_STRING) {
        MAKE_STD_ZVAL(tmp_prop);
        *tmp_prop = *prop;
        INIT_PZVAL(tmp_prop);
        zval_copy_ctor(tmp_prop);
        convert_to_string(tmp_prop);
        prop = tmp_prop;
        key = NULL;
    }

    src = Z_STRVAL_P(prop);
    if (strcmp(src,"progress") == 0) {
        result = true;
    }
    else {
        // Invoke base class handler.
        result = (*php_odb_writepack_object::handlers.has_property)
            (obj,prop,chk_type,key TSRMLS_CC);
    }

    if (tmp_prop != nullptr) {
        zval_ptr_dtor(&tmp_prop);
    }

    return result;
}

// Implementation of object methods

PHP_METHOD(GitODBWritepack_Internal,append)
{
    php_bailer bailer ZTS_CTOR;
    int result;
    char* buf;
    int amt;
    php_odb_writepack_internal_object* object;

    object = LOOKUP_OBJECT(php_odb_writepack_internal_object,getThis());

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
    php_odb_writepack_internal_object* object;

    object = LOOKUP_OBJECT(php_odb_writepack_internal_object,getThis());

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
