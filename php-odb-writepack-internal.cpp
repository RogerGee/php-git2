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

static zval* odb_writepack_internal_read_property(
    zval* object,
    zval* member,
    int type,
    void** cache_slot,
    zval* rv);
static zval* odb_writepack_internal_write_property(
    zval* object,
    zval* member,
    zval* value,
    void** cache_slot);
static int odb_writepack_internal_has_property(
    zval* object,
    zval* member,
    int has_set_exists,
    void** cache_slot);

// Make function implementation

void php_git2::php_git2_make_odb_writepack(
    zval* zp,
    git_odb_writepack* writepack,
    php_callback_sync* cb,
    zval* backend,
    php_git_odb* owner)
{
    php_odb_writepack_internal_object* obj;

    object_init_ex(zp,php_git2::class_entry[php_git2_odb_writepack_internal_obj]);
    obj = php_zend_object<php_odb_writepack_internal_object>::get_storage(Z_OBJ_P(zp));

    // Assign the writepack and callback. The callback is merely along for the
    // ride so it can be destroyed at the proper time.
    obj->writepack = writepack;
    obj->cb = cb;

    // Assign owner. If set, this will prevent the parent ODB from freeing
    // during the lifetime of the writepack.
    obj->assign_owner(owner);

    // Assign backend if we have a backend specified. This will prevent the
    // parent backend object from freeing while the writepack is in use. We keep
    // this reference so we can refer to the same backend used to create the
    // writepack instead of creating a new one via the ODB.
    if (backend != nullptr) {
        ZVAL_COPY(&obj->backend,backend);
    }
}

// php_zend_object init function

template<>
zend_object_handlers php_git2::php_zend_object<php_odb_writepack_object>::handlers;

template<>
zend_object_handlers php_git2::php_zend_object<php_odb_writepack_internal_object>::handlers;

template<>
void php_zend_object<php_odb_writepack_internal_object>::init(zend_class_entry* ce)
{
    handlers.read_property = odb_writepack_internal_read_property;
    handlers.write_property = odb_writepack_internal_write_property;
    handlers.has_property = odb_writepack_internal_has_property;
    handlers.get_constructor = php_git2::not_allowed_get_constructor;

    handlers.offset = offset();

    UNUSED(ce);
}

php_odb_writepack_internal_object::php_odb_writepack_internal_object():
    php_odb_writepack_object(), cb(nullptr)
{
    memset(&prog,0,sizeof(git_transfer_progress));
    ZVAL_UNDEF(&backend);
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

    zval_ptr_dtor(&backend);
}

// Implementation of custom class handlers.

zval* odb_writepack_internal_read_property(
    zval* object,
    zval* member,
    int type,
    void** cache_slot,
    zval* rv)
{
    zval* retval = rv;
    zval tmp_member;
    php_odb_writepack_internal_object* storage;

    // Ensure deep copy of member zval.
    if (Z_TYPE_P(member) != IS_STRING) {
        ZVAL_STR(&tmp_member,zval_get_string(member));
        member = &tmp_member;
        cache_slot = nullptr;
    }

    // Handle special properties of the git_odb_writepack.

    using zend_object_t = php_zend_object<php_odb_writepack_internal_object>;
    storage = zend_object_t::get_storage(Z_OBJ_P(object));

    if (strcmp(Z_STRVAL_P(member),"progress") == 0) {
        php_git2::convert_transfer_progress(retval,&storage->prog);
    }
    else if (strcmp(Z_STRVAL_P(member),"backend") == 0
        && Z_TYPE(storage->backend) != IS_UNDEF)
    {
        ZVAL_COPY_VALUE(retval,&storage->backend);
    }
    else {
        // Invoke base class handler.
        auto handler = php_zend_object<php_odb_writepack_object>::handlers.read_property;
        retval = (*handler)(object,member,type,cache_slot,rv);
    }

    if (member == &tmp_member) {
        zval_dtor(member);
    }

    return retval;
}

zval* odb_writepack_internal_write_property(
    zval* object,
    zval* member,
    zval* value,
    void** cache_slot)
{
    zval* result = value;
    zval tmp_member;

    // Ensure deep copy of member zval.
    if (Z_TYPE_P(member) != IS_STRING) {
        ZVAL_STR(&tmp_member,zval_get_string(member));
        member = &tmp_member;
        cache_slot = nullptr;
    }

    if (strcmp(Z_STRVAL_P(member),"progress") == 0) {
        zend_throw_error(
            nullptr,
            "Property '%s' of GitODBWritepack_Internal cannot be updated",
            Z_STRVAL_P(member));
    }
    else {
        // Invoke base class handler.
        auto handler = php_zend_object<php_odb_writepack_object>::handlers.write_property;
        result = (*handler)(object,member,value,cache_slot);
    }

    if (member == &tmp_member) {
        zval_dtor(member);
    }

    return result;
}

int odb_writepack_internal_has_property(
    zval* object,
    zval* member,
    int has_set_exists,
    void** cache_slot)
{
    int result;
    zval tmp_member;

    // Ensure deep copy of member zval.
    if (Z_TYPE_P(member) != IS_STRING) {
        ZVAL_STR(&tmp_member,zval_get_string(member));
        member = &tmp_member;
        cache_slot = nullptr;
    }

    if (strcmp(Z_STRVAL_P(member),"progress") == 0) {
        result = true;
    }
    else {
        // Invoke base class handler.
        auto handler = php_zend_object<php_odb_writepack_object>::handlers.has_property;
        result = (*handler)(object,member,has_set_exists,cache_slot);
    }

    if (member == &tmp_member) {
        zval_dtor(member);
    }

    return result;
}

// Implementation of object methods

PHP_METHOD(GitODBWritepack_Internal,append)
{
    php_bailer bailer;
    int result;
    char* buf;
    size_t amt;
    php_odb_writepack_internal_object* object;

    object = php_zend_object<php_odb_writepack_internal_object>::get_storage(getThis());

    assert(object->writepack != nullptr);

    if (zend_parse_parameters(ZEND_NUM_ARGS(),"s",&buf,&amt) != SUCCESS) {
        return;
    }

    try {
        result = object->writepack->append(object->writepack,buf,amt,&object->prog);
        if (result < 0) {
            php_git2::git_error(result);
        }

    } catch (php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle();
        }
    }
}

PHP_METHOD(GitODBWritepack_Internal,commit)
{
    php_bailer bailer;
    int result;
    php_odb_writepack_internal_object* object;

    object = php_zend_object<php_odb_writepack_internal_object>::get_storage(getThis());

    assert(object->writepack != nullptr);

    try {
        result = object->writepack->commit(object->writepack,&object->prog);
        if (result < 0) {
            php_git2::git_error(result);
        }

    } catch (php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle();
        }
    }
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
