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
    php_callback_sync* cb TSRMLS_DC)
{
    zval* zbackend;
    php_odb_writepack_object* obj;
    zend_class_entry* ce = php_git2::class_entry[php_git2_odb_writepack_obj];

    object_init_ex(zp,php_git2::class_entry[php_git2_odb_writepack_obj]);
    obj = reinterpret_cast<php_odb_writepack_object*>(zend_objects_get_address(zp TSRMLS_CC));

    // Assign the writepack and callback. The callback is merely along for the
    // ride so it can be destroyed at the proper time.
    obj->writepack = writepack;
    obj->cb = cb;

    // Update the 'backend' property for object. We have to create an object
    // zval for this. The object does *not* own the backend since it will be
    // freed by the writepack (assumably, all I know is it crashes if I free it
    // myself).
    MAKE_STD_ZVAL(zbackend);
    php_git2_make_odb_backend(zbackend,writepack->backend,false);
    zend_update_property(ce,zp,"backend",sizeof("backend")-1,zbackend TSRMLS_CC);
}

// Implementation of php_odb_writepack_object

/*static*/ zend_object_handlers php_odb_writepack_object::handlers;
php_odb_writepack_object::php_odb_writepack_object(zend_class_entry* ce TSRMLS_DC):
    writepack(nullptr), cb(nullptr), zts(TSRMLS_C)
{
    zend_object_std_init(&base,ce TSRMLS_CC);
    object_properties_init(&base,ce);
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

    zend_object_std_dtor(&base ZTS_MEMBER_CC(this->zts));
}

/*static*/ void php_odb_writepack_object::init(zend_class_entry* ce)
{
    zend_declare_property_null(ce,"backend",sizeof("backend")-1,ZEND_ACC_PUBLIC);
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
