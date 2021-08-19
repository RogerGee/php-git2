/*
 * php-odb-writepack.cpp
 *
 * Copyright (C) Roger P. Gee
 *
 * This unit provides the out-of-line implementation for the GitODBWritepack
 * class.
 */

#include "php-object.h"
using namespace php_git2;

// Custom class handlers

static zval* odb_writepack_read_property(
    zval* object,
    zval* member,
    int type,
    void** cache_slot,
    zval* rv);
static void odb_writepack_write_property(
    zval* object,
    zval* member,
    zval* value,
    void** cache_slot);
static int odb_writepack_has_property(
    zval* object,
    zval* member,
    int has_set_exists,
    void** cache_slot);

// Class method entries

zend_function_entry php_git2::odb_writepack_methods[] = {
    PHP_ABSTRACT_ME(GitODBWritepack,append,NULL)
    PHP_ABSTRACT_ME(GitODBWritepack,commit,NULL)
    PHP_FE_END
};

// php_zend_object init function

template<>
zend_object_handlers php_git2::php_zend_object<php_odb_writepack_object>::handlers;

template<>
void php_zend_object<php_odb_writepack_object>::init(zend_class_entry* ce)
{
    handlers.read_property = odb_writepack_read_property;
    handlers.write_property = odb_writepack_write_property;
    handlers.has_property = odb_writepack_has_property;

    handlers.offset = offset();

    UNUSED(ce);
}

// Implementation of php_odb_writepack_object

php_odb_writepack_object::php_odb_writepack_object():
    writepack(nullptr), owner(nullptr)
{
}

php_odb_writepack_object::~php_odb_writepack_object()
{
    // Attempt to free the owner resource. This only really frees the owner if
    // its refcount reaches zero.
    if (owner != nullptr) {
        git2_resource_base::free_recursive(owner);
    }
}

void php_odb_writepack_object::create_custom_writepack(zval* zobj,zval* zbackend)
{
    // NOTE: this member function should be called under the php-git2 standard
    // exception handler.

    // Make sure the writepack does not already exist.
    if (writepack != nullptr) {
        throw php_git2_error_exception(
            "cannot create custom ODB writepack - object already in use"
            );
    }

    // Unset any previous 'zbackend' property.
    zend_unset_property(Z_OBJCE_P(zobj),zobj,"backend",sizeof("backend")-1);

    // Assign 'zbackend' property. This will prevent the parent backend from
    // freeing while the writepack is in use.
    Z_ADDREF_P(zbackend);
    zend_hash_str_add(Z_OBJPROP_P(zobj),"backend",sizeof("backend")-1,zbackend);

    // Create new custom writepack.
    writepack = new (emalloc(sizeof(git_odb_writepack_php)))
        git_odb_writepack_php(Z_OBJ_P(zobj));

    // Reset owner (in case it is set). We do not need to maintain a direct
    // owner reference since we are maintaining a reference to the backend in
    // the object's properties hashtable.
    assign_owner(nullptr);
}

void php_odb_writepack_object::assign_owner(php_git_odb* newOwner)
{
    // Free previous owner if any.
    if (owner != nullptr) {
        git2_resource_base::free_recursive(owner);
    }

    owner = newOwner;

    // Increment reference count (if set). This keeps the ODB alive while the
    // writepack is in use.
    if (owner != nullptr) {
        owner->up_ref();
    }
}

/*static*/ int php_odb_writepack_object::append(
    git_odb_writepack* writepack,
    const void* data,
    size_t length,
    git_transfer_progress* prog)
{
    int result;
    zval_array<2> params;
    method_wrapper method("append",writepack);

    ZVAL_STRINGL(params[0],reinterpret_cast<const char*>(data),length);
    ZVAL_MAKE_REF(params[1]);
    convert_transfer_progress(Z_REFVAL_P(params[1]),prog);

    result = method.call(params);
    if (result == GIT_OK) {
        // Write back transfer progress to library structure.
        convert_transfer_progress(*prog,Z_REFVAL_P(params[1]));
    }

    return result;
}

/*static*/ int php_odb_writepack_object::commit(
    git_odb_writepack* writepack,
    git_transfer_progress* prog)
{
    int result;
    zval_array<1> params;
    method_wrapper method("commit",writepack);

    ZVAL_MAKE_REF(params[0]);
    convert_transfer_progress(Z_REFVAL_P(params[0]),prog);

    result = method.call(params);
    if (result == GIT_OK) {
        // Write back transfer progress to library structure.
        convert_transfer_progress(*prog,Z_REFVAL_P(params[0]));
    }

    return result;
}

/*static*/ void php_odb_writepack_object::free(git_odb_writepack* writepack)
{
    method_wrapper::object_wrapper object(writepack);

    // Explicitly call the destructor on the custom writepack. Then free the
    // block of memory that holds the custom writepack.

    object.object()->~git_odb_writepack_php();
    efree(writepack);
}

// Implementation of custom git_odb_writepack

php_odb_writepack_object::
git_odb_writepack_php::git_odb_writepack_php(zend_object* obj)
{
    // Blank out the base class (which is the structure defined in the git2
    // headers).
    memset(this,0,sizeof(struct git_odb_writepack));

    // Assign zend_object to local zval so it is kept alive for duration of
    // custom backend.
    ZVAL_OBJ(&thisobj,obj);
    Z_ADDREF(thisobj);

    // Assign functions that will invoke the PHP methods in the userspace
    // class. Note that the methods are necessarily overridden since they are
    // abstract in the base class.
    append = php_odb_writepack_object::append;
    commit = php_odb_writepack_object::commit;
    free = php_odb_writepack_object::free;
}

php_odb_writepack_object::
git_odb_writepack_php::~git_odb_writepack_php()
{
    // Free object zval.
    zval_ptr_dtor(&thisobj);
}

// Implementation of custom object handlers

zval* odb_writepack_read_property(
    zval* object,
    zval* member,
    int type,
    void** cache_slot,
    zval* rv)
{
    zval* retval = rv;
    zval tmp_member;
    php_odb_writepack_object* storage;

    // Ensure deep copy of member zval.
    if (Z_TYPE_P(member) != IS_STRING) {
        ZVAL_STR(&tmp_member,zval_get_string(member));
        member = &tmp_member;
        cache_slot = nullptr;
    }

    // Handle special properties of the git_odb_writepack.

    storage = php_zend_object<php_odb_writepack_object>::get_storage(Z_OBJ_P(object));

    if (strcmp(Z_STRVAL_P(member),"backend") == 0) {
        zval* lookup;
        lookup = zend_hash_find(Z_OBJPROP_P(object),Z_STR_P(member));

        if (lookup == nullptr) {
            // NOTE: We can only safely create a backend object if an owner is
            // set on the writepack.

            if (storage->writepack != nullptr && storage->owner != nullptr) {
                // Create GitODBBackend object instance to represent the
                // git_odb_backend attached to the writepack. The object does
                // not own the underlying backend object since we pass an owner.
                php_git2_make_odb_backend(
                    retval,
                    storage->writepack->backend,
                    storage->owner);

                Z_ADDREF_P(retval);
                zend_hash_add(Z_OBJPROP_P(object),Z_STR_P(member),retval);
            }
        }
        else {
            retval = lookup;
        }
    }
    else {
        const zend_object_handlers* std = zend_get_std_object_handlers();
        retval = std->read_property(object,member,type,cache_slot,rv);
    }

    if (member == &tmp_member) {
        zval_dtor(member);
    }

    return retval;
}

void odb_writepack_write_property(
    zval* object,
    zval* member,
    zval* value,
    void** cache_slot)
{
    zval tmp_member;

    // Ensure deep copy of member zval.
    if (Z_TYPE_P(member) != IS_STRING) {
        ZVAL_STR(&tmp_member,zval_get_string(member));
        member = &tmp_member;
        cache_slot = nullptr;
    }

    if (strcmp(Z_STRVAL_P(member),"backend") == 0) {
        zend_throw_error(
            nullptr,
            "Property '%s' of GitODBWritepack cannot be updated",
            Z_STRVAL_P(member));
    }
    else {
        const zend_object_handlers* std = zend_get_std_object_handlers();
        std->write_property(object,member,value,cache_slot);
    }

    if (member == &tmp_member) {
        zval_dtor(member);
    }
}

int odb_writepack_has_property(
    zval* object,
    zval* member,
    int has_set_exists,
    void** cache_slot)
{
    int result;
    zval tmp_member;
    git_odb_writepack* writepack;

    // Ensure deep copy of member zval.
    if (Z_TYPE_P(member) != IS_STRING) {
        ZVAL_STR(&tmp_member,zval_get_string(member));
        member = &tmp_member;
        cache_slot = nullptr;
    }

    using zend_object_t = php_zend_object<php_odb_writepack_object>;
    writepack = zend_object_t::get_storage(Z_OBJ_P(object))->writepack;

    if (strcmp(Z_STRVAL_P(member),"backend") == 0) {
        if (has_set_exists == 2) {
            result = (writepack != nullptr);
        }
        else {
            result = (writepack != nullptr && writepack->backend != nullptr);
        }
    }
    else {
        const zend_object_handlers* std = zend_get_std_object_handlers();
        result = std->has_property(object,member,has_set_exists,cache_slot);
    }

    if (member == &tmp_member) {
        zval_dtor(member);
    }

    return result;
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
