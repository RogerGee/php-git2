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

static PHP_METHOD(GitODBWritepack,append);
static PHP_METHOD(GitODBWritepack,commit);
static PHP_METHOD(GitODBWritepack,free);
zend_function_entry php_git2::odb_writepack_methods[] = {
    PHP_ME(GitODBWritepack,append,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBWritepack,commit,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBWritepack,free,NULL,ZEND_ACC_PUBLIC)
    PHP_FE_END
};

// Make function implementation

void php_git2::php_git2_make_odb_writepack(zval* zp,git_odb_writepack* writepack,
    php_callback_sync* cb,zval* zbackend,php_git_odb* owner TSRMLS_DC)
{
    php_odb_writepack_object* obj;
    zend_class_entry* ce = php_git2::class_entry[php_git2_odb_writepack_obj];

    object_init_ex(zp,ce);
    obj = reinterpret_cast<php_odb_writepack_object*>(zend_objects_get_address(zp TSRMLS_CC));

    // Assign the writepack and callback. The callback is merely along for the
    // ride so it can be destroyed at the proper time.
    obj->writepack = writepack;
    obj->cb = cb;
    obj->owner = owner;

    // Increment owner refcount if set. This will prevent the ODB from freeing
    // while the backend is in use.
    if (obj->owner != nullptr) {
        obj->owner->up_ref();
    }

    // Assign backend property if we have a backend specified.
    if (zbackend != nullptr) {
        Z_ADDREF_P(zbackend);
        zend_hash_add(Z_OBJPROP_P(zp),"backend",sizeof("backend"),&zbackend,sizeof(zval*),NULL);
    }
}

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

/*static*/ void php_odb_writepack_object::init(zend_class_entry* ce)
{
    handlers.read_property = odb_writepack_read_property;
    handlers.write_property = odb_writepack_write_property;
    handlers.has_property = odb_writepack_has_property;
    handlers.get_constructor = php_git2::not_allowed_get_constructor;
    (void)ce;
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

            if (writepack->backend != nullptr) {
                // Create GitODBBackend object instance to represent the
                // git_odb_backend attached to the writepack. The object does not
                // own the underlying backend object.
                php_git2_make_odb_backend(ret,writepack->backend,wrapper->owner);

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
    if (strcmp(str,"backend") == 0) {
        php_error(E_ERROR,"GitODBWritepack: the %s property is read-only",str);
    }
    else {
        (*std_object_handlers.write_property)(obj,prop,value,key);
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

// Implementation of object methods

PHP_METHOD(GitODBWritepack,append)
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
        return;
    }

    RETURN_FALSE;
}

PHP_METHOD(GitODBWritepack,commit)
{
    int result;
    php_odb_writepack_object* object;

    object = LOOKUP_OBJECT(php_odb_writepack_object,getThis());
    if (object->writepack != nullptr) {
        result = object->writepack->commit(object->writepack,&object->prog);
        RETVAL_LONG(result);
        return;
    }

    RETURN_FALSE;
}

PHP_METHOD(GitODBWritepack,free)
{
    php_odb_writepack_object* object;

    object = LOOKUP_OBJECT(php_odb_writepack_object,getThis());
    if (object->writepack != nullptr) {
        object->writepack->free(object->writepack);
        object->writepack = nullptr;
        RETURN_TRUE;
    }

    RETURN_FALSE;
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
