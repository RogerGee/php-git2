/*
 * php-odb-writepack.cpp
 *
 * This file is a part of php-git2.
 *
 * This unit provides the out-of-line implementation for the GitODBWritepack
 * class.
 */

#include "php-object.h"
using namespace php_git2;

// Custom class handlers

static zval* odb_writepack_read_property(zval* obj,zval* prop,int type,const zend_literal* key TSRMLS_DC);
static void odb_writepack_write_property(zval* obj,zval* prop,zval* value,const zend_literal* key TSRMLS_DC);
static int odb_writepack_has_property(zval* obj,zval* prop,int chk_type,const zend_literal* key TSRMLS_DC);

// Class method entries

zend_function_entry php_git2::odb_writepack_methods[] = {
    PHP_ABSTRACT_ME(GitODBWritepack,append,NULL)
    PHP_ABSTRACT_ME(GitODBWritepack,commit,NULL)
    PHP_FE_END
};

// Implementation of php_odb_writepack_object

/*static*/ zend_object_handlers php_odb_writepack_object::handlers;
php_odb_writepack_object::php_odb_writepack_object(zend_class_entry* ce TSRMLS_DC):
    writepack(nullptr), cb(nullptr), owner(nullptr), zts(TSRMLS_C)
{
    zend_object_std_init(this,ce TSRMLS_CC);
    object_properties_init(this,ce);
    memset(&prog,0,sizeof(git_transfer_progress));
}

php_odb_writepack_object::~php_odb_writepack_object()
{
    // Writepack could have become null if user freed it.
    if (writepack != nullptr) {
        writepack->free(writepack);
    }

    if (cb != nullptr) {
        cb->~php_callback_sync();
        efree(cb);
    }

    // Attempt to free the owner resource. This only really frees the owner if
    // its refcount reaches zero.
    if (owner != nullptr) {
        git2_resource_base::free_recursive(owner);
    }

    zend_object_std_dtor(this ZTS_MEMBER_CC(this->zts));
}

void php_odb_writepack_object::create_custom_writepack(zval* zobj,zval* zbackendObject)
{
    // NOTE: this member function should be called under the php-git2 standard
    // exception handler.

    // Make sure the writepack does not already exist.
    if (writepack != nullptr) {
        throw php_git2_fatal_exception("cannot create custom ODB writepack - object already in use");
    }

    // TODO
}

/*static*/ void php_odb_writepack_object::init(zend_class_entry* ce)
{
    handlers.read_property = odb_writepack_read_property;
    handlers.write_property = odb_writepack_write_property;
    handlers.has_property = odb_writepack_has_property;
    handlers.get_constructor = php_git2::not_allowed_get_constructor;

    UNUSED(ce);
}

// Implementation of custom object handlers

zval* odb_writepack_read_property(zval* obj,zval* prop,int type,const zend_literal* key TSRMLS_DC)
{
    zval* ret;
    zval** zfind;
    zval* tmp_prop = nullptr;
    const char* str;
    php_odb_writepack_object* wrapper = LOOKUP_OBJECT(php_odb_writepack_object,obj);
    git_odb_writepack* writepack = wrapper->writepack;

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
    if (strcmp(str,"backend") == 0 && writepack != NULL) {
        if (key != nullptr) {
            ret = zend_hash_quick_find(Z_OBJPROP_P(obj),"backend",sizeof("backend"),key->hash_value,(void**)&zfind) != FAILURE
                ? *zfind : nullptr;
        }
        else {
            ret = zend_hash_find(Z_OBJPROP_P(obj),"backend",sizeof("backend"),(void**)&zfind) != FAILURE
                ? *zfind : nullptr;
        }

        if (ret == nullptr) {
            ALLOC_INIT_ZVAL(ret);

            // NOTE: We can only safely create a backend object if an owner is
            // set on the writepack.

            if (writepack->backend != nullptr && wrapper->owner != nullptr) {
                // Create GitODBBackend object instance to represent the
                // git_odb_backend attached to the writepack. The object does
                // not own the underlying backend object since we pass an owner.
                php_git2_make_odb_backend(ret,writepack->backend,wrapper->owner TSRMLS_CC);

                Z_ADDREF_P(ret);
                if (key != nullptr) {
                    zend_hash_quick_add(Z_OBJPROP_P(obj),"backend",sizeof("backend"),key->hash_value,
                        &ret,sizeof(zval*),NULL);
                }
                else {
                    zend_hash_add(Z_OBJPROP_P(obj),"backend",sizeof("backend"),&ret,sizeof(zval*),NULL);
                }
            }
            else {
                Z_DELREF_P(ret);
            }
        }
    }
    else if (strcmp(str,"progress") == 0) {
        // NOTE: We create a new zval every time since this property is readonly
        // and the value may change.
        MAKE_STD_ZVAL(ret);
        php_git2::convert_transfer_progress(ret,&wrapper->prog);
    }
    else {
        ret = (*std_object_handlers.read_property)(obj,prop,type,key TSRMLS_CC);
    }

    if (tmp_prop != nullptr) {
        Z_ADDREF_P(ret);
        zval_ptr_dtor(&tmp_prop);
        Z_DELREF_P(ret);
    }

    return ret;
}

void odb_writepack_write_property(zval* obj,zval* prop,zval* value,const zend_literal* key TSRMLS_DC)
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
    if (strcmp(str,"backend") == 0 || strcmp(str,"progress") == 0) {
        php_error(E_ERROR,"Property '%s' of GitODBWritepack cannot be updated",str);
    }
    else {
        (*std_object_handlers.write_property)(obj,prop,value,key TSRMLS_CC);
    }

    if (tmp_prop != nullptr) {
        zval_ptr_dtor(&tmp_prop);
    }
}

int odb_writepack_has_property(zval* obj,zval* prop,int chk_type,const zend_literal* key TSRMLS_DC)
{
    int result;
    zval* tmp_prop = nullptr;
    const char* src;
    git_odb_writepack* writepack = LOOKUP_OBJECT(php_odb_writepack_object,obj)->writepack;

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
    if (strcmp(src,"backend") == 0) {
        if (chk_type == 2) {
            result = (writepack != nullptr);
        }
        else {
            result = (writepack != nullptr && writepack->backend != nullptr);
        }
    }
    else {
        result = (*std_object_handlers.has_property)(obj,prop,chk_type,key TSRMLS_CC);
    }

    if (tmp_prop != nullptr) {
        zval_ptr_dtor(&tmp_prop);
    }

    return result;
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
