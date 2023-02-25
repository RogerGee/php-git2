/*
 * php-refdb-backend.cpp
 *
 * Copyright (C) Roger P. Gee
 */

#include "php-object.h"
#include "stubs/GitRefDBBackend_arginfo.h"
#include <cstring>
extern "C" {
#include <git2/sys/refs.h>
#include <git2/sys/reflog.h>
}
using namespace php_git2;

// Helper functions.

static int reference_from_string(git_reference** out,zval* zstr,const char* ref_name)
{
    // If the string starts with "ref: " then assume it is a symbolic
    // reference. Otherwise try to decode an OID hex string.

    const char* target = Z_STRVAL_P(zstr);

    if (strstr(target,"ref: ") == target) {
        *out = git_reference__alloc_symbolic(ref_name,target);
    }
    else if (strlen(target) == GIT_OID_HEXSZ) {
        git_oid oid;
        if (convert_oid_fromstr(&oid,target,GIT_OID_HEXSZ) != 0) {
            giterr_set_str(GIT_EINVALID,"Invalid OID reference target value");
            return GIT_ERROR;
        }

        *out = git_reference__alloc(ref_name,&oid,NULL);
    }
    else {
        giterr_set_str(GIT_EINVALID,"Invalid reference target value");
        return GIT_ERROR;
    }

    return GIT_OK;
}

static int reflog_entry_from_array(git_reflog_entry** out,zval* zarr)
{
    const char* oid_old;
    int oid_old_len;
    const char* oid_cur;
    int oid_cur_len;
    git_signature* committer;
    const char* msg;

    git_reflog_entry* ent;

    if (Z_TYPE_P(zarr) != IS_ARRAY) {
        giterr_set_str(GIT_EINVALID,"Reflog entry must be an array");
        return GIT_ERROR;
    }

    array_wrapper arr(zarr);

    if (!arr.query("oid_old",sizeof("oid_old")-1)) {
        giterr_set_str(GIT_EINVALID,"Reflog entry must have 'oid_old' member");
        return GIT_ERROR;
    }
    oid_old = arr.get_string();
    oid_old_len = arr.get_string_length();

    if (!arr.query("oid_cur",sizeof("oid_cur")-1)) {
        giterr_set_str(GIT_EINVALID,"Reflog entry must have 'oid_cur' member");
        return GIT_ERROR;
    }
    oid_cur = arr.get_string();
    oid_cur_len = arr.get_string_length();

    if (!arr.query("committer",sizeof("committer")-1)) {
        giterr_set_str(GIT_EINVALID,"Reflog entry must have 'committer' member");
        return GIT_ERROR;
    }
    committer = convert_signature(arr.get_value());
    if (committer == nullptr) {
        giterr_set_str(GIT_EINVALID,"Failed to extract 'committer' from reflog entry");
        return GIT_ERROR;
    }

    if (!arr.query("msg",sizeof("msg")-1)) {
        giterr_set_str(GIT_EINVALID,"Reflog entry must have 'msg' member");
        return GIT_ERROR;
    }
    msg = arr.get_string();

    ent = git_reflog_entry__alloc();
    if (ent == nullptr) {
        return GIT_ERROR;
    }

    // TODO Uh, how do I set the fields on the opaque struct?

    UNUSED(oid_old);
    UNUSED(oid_old_len);
    UNUSED(oid_cur);
    UNUSED(oid_cur_len);
    UNUSED(committer);
    UNUSED(msg);

    *out = ent;

    return GIT_OK;
}

struct custom_backend_iterator : git_reference_iterator
{
    typedef git_reference_iterator base_class;

    using method_wrapper = php_method_wrapper<
        custom_backend_iterator,
        php_refdb_backend_object
        >;

    // Store reference to GitRefDBBackend object.
    zval thisobj;

    // Store last returned name string.
    zval zlast;
};

static int custom_backend_iterator_next(git_reference** ref,custom_backend_iterator* iter)
{
    int result;
    zval_array<1> params;
    custom_backend_iterator::method_wrapper method("iterator_next",iter);

    ZVAL_MAKE_REF(params[0]);

    // Call userspace method implementation corresponding to refdb operation.

    result = method.call(params);
    if (result == GIT_OK) {
        zval* retval = method.retval();

        // If userspace returned false, then iteration is over.
        if (Z_TYPE_P(retval) == IS_FALSE) {
            result = GIT_ITEROVER;
        }
        else {
            if (Z_TYPE_P(retval) == IS_RESOURCE) {
                // Verify that the returned resource is a git_reference.
                void* result = zend_fetch_resource(
                    Z_RES_P(retval),
                    php_git_reference::resource_name(),
                    php_git_reference::resource_le()
                    );
                if (result == nullptr) {
                    throw php_git2_propagated_exception();
                }

                // Extract reference, duplicate and return.
                php_git_reference* extract = reinterpret_cast<php_git_reference*>(
                    Z_RES_VAL_P(retval)
                    );
                git_reference_dup(ref,extract->get_handle());
            }
            else {
                // If a non-resource is returned, then we build the result
                // git_reference from two distinct values:
                //  1) a string OID or symbolic name (returned from method call)
                //  2) a string reference name (returned via the first method
                //     parameter)

                // Deep copy the reference name from the method out parameter
                // into a zval that is kept alive by the iterator.
                zval_ptr_dtor(&iter->zlast);
                ZVAL_COPY(&iter->zlast,Z_REFVAL_P(params[0]));
                convert_to_string(&iter->zlast);

                // Convert return value to string (in case it needs a cast) and
                // build the git_reference result value.
                convert_to_string(retval);
                result = reference_from_string(ref,retval,Z_STRVAL(iter->zlast));
            }
        }
    }

    return result;
}

static int custom_backend_iterator_next_name(const char** ref_name,custom_backend_iterator* iter)
{
    int result;
    zval_array<1> params;
    custom_backend_iterator::method_wrapper method("iterator_next",iter);

    // Call userspace method implementation corresponding to refdb operation.

    result = method.call(params);
    if (result == GIT_OK) {
        zval* retval = method.retval();

        // If userspace returned false, then iteration is over.
        if (Z_TYPE_P(retval) == IS_FALSE) {
            result = GIT_ITEROVER;
        }
        else {
            // Deep copy the name into a zval that is kept alive by the
            // iterator.
            zval_ptr_dtor(&iter->zlast);
            ZVAL_COPY(&iter->zlast,params[0]);
            convert_to_string(&iter->zlast);

            *ref_name = Z_STRVAL(iter->zlast);
        }
    }

    return result;
}

static void custom_backend_iterator_free(custom_backend_iterator* iter)
{
    zval_ptr_dtor(&iter->thisobj);
    zval_ptr_dtor(&iter->zlast);
    efree(iter);
}

// Custom class handlers

static zval* refdb_backend_read_property(
    zend_object* object,
    zend_string* member,
    int type,
    void** cache_slot,
    zval* rv);
static zval* refdb_backend_write_property(
    zend_object* object,
    zend_string* member,
    zval* value,
    void** cache_slot);
static int refdb_backend_has_property(
    zend_object* object,
    zend_string* member,
    int has_set_exists,
    void** cache_slot);

// Class entries

// The following functions are optional so this base class provides an empty
// implementation.

static PHP_EMPTY_METHOD(GitRefDBBackend,iterator_new);
static PHP_EMPTY_METHOD(GitRefDBBackend,iterator_next);
static PHP_EMPTY_METHOD(GitRefDBBackend,compress);
static PHP_EMPTY_METHOD(GitRefDBBackend,lock);
static PHP_EMPTY_METHOD(GitRefDBBackend,unlock);

zend_function_entry php_git2::refdb_backend_methods[] = {
    PHP_ABSTRACT_ME(
        GitRefDBBackend,
        exists,
        arginfo_class_GitRefDBBackend_exists
        )
    PHP_ABSTRACT_ME(
        GitRefDBBackend,
        lookup,
        arginfo_class_GitRefDBBackend_lookup
        )
    PHP_ME(
        GitRefDBBackend,
        iterator_new,
        arginfo_class_GitRefDBBackend_iterator_new,
        ZEND_ACC_PUBLIC
        )
    PHP_ME(
        GitRefDBBackend,
        iterator_next,
        arginfo_class_GitRefDBBackend_iterator_next,
        ZEND_ACC_PUBLIC
        )
    PHP_ABSTRACT_ME(
        GitRefDBBackend,
        write,
        arginfo_class_GitRefDBBackend_write
        )
    PHP_ABSTRACT_ME(
        GitRefDBBackend,
        rename,
        arginfo_class_GitRefDBBackend_rename
        )
    PHP_ABSTRACT_ME(
        GitRefDBBackend,
        del,
        arginfo_class_GitRefDBBackend_del
        )
    PHP_ME(
        GitRefDBBackend,
        compress,
        arginfo_class_GitRefDBBackend_compress,
        ZEND_ACC_PUBLIC
        )
    PHP_ABSTRACT_ME(
        GitRefDBBackend,
        has_log,
        arginfo_class_GitRefDBBackend_has_log
        )
    PHP_ABSTRACT_ME(
        GitRefDBBackend,
        ensure_log,
        arginfo_class_GitRefDBBackend_ensure_log
        )
    PHP_ABSTRACT_ME(
        GitRefDBBackend,
        reflog_read,
        arginfo_class_GitRefDBBackend_reflog_read
        )
    PHP_ABSTRACT_ME(
        GitRefDBBackend,
        reflog_write,
        arginfo_class_GitRefDBBackend_reflog_write
        )
    PHP_ABSTRACT_ME(
        GitRefDBBackend,
        reflog_rename,
        arginfo_class_GitRefDBBackend_reflog_delete
        )
    PHP_ABSTRACT_ME(
        GitRefDBBackend,
        reflog_delete,
        arginfo_class_GitRefDBBackend_reflog_delete
        )
    PHP_ME(
        GitRefDBBackend,
        lock,
        arginfo_class_GitRefDBBackend_lock,
        ZEND_ACC_PUBLIC
        )
    PHP_ME(
        GitRefDBBackend,
        unlock,
        arginfo_class_GitRefDBBackend_unlock,
        ZEND_ACC_PUBLIC
        )
    PHP_FE_END
};

// php_zend_object init function

template<>
zend_object_handlers php_git2::php_zend_object<php_refdb_backend_object>::handlers;

template<>
void php_zend_object<php_refdb_backend_object>::init(zend_class_entry* ce)
{
    handlers.read_property = refdb_backend_read_property;
    handlers.write_property = refdb_backend_write_property;
    handlers.has_property = refdb_backend_has_property;

    handlers.offset = offset();

    UNUSED(ce);
}

// Implementation of custom class handlers

zval* refdb_backend_read_property(
    zend_object* object,
    zend_string* member,
    int type,
    void** cache_slot,
    zval* rv)
{
    const zend_object_handlers* std = zend_get_std_object_handlers();
    return std->read_property(object,member,type,cache_slot,rv);
}

zval* refdb_backend_write_property(
    zend_object* object,
    zend_string* member,
    zval* value,
    void** cache_slot)
{
    const zend_object_handlers* std = zend_get_std_object_handlers();
    return std->write_property(object,member,value,cache_slot);
}

int refdb_backend_has_property(
    zend_object* object,
    zend_string* member,
    int has_set_exists,
    void** cache_slot)
{
    const zend_object_handlers* std = zend_get_std_object_handlers();
    return std->has_property(object,member,has_set_exists,cache_slot);
}

// Implementation of php_refdb_backend_object

php_refdb_backend_object::php_refdb_backend_object():
    backend(nullptr), kind(unset), owner(nullptr)
{
}

php_refdb_backend_object::~php_refdb_backend_object()
{
    // Free the object if we do not have an owner.
    if (kind == user) {
        backend->free(backend);
    }

    if (owner != nullptr) {
        git2_resource_base::free_recursive(owner);
    }
}

void php_refdb_backend_object::create_custom_backend(zval* zobj)
{
    // NOTE: 'zobj' is the zval to the outer object that wraps this custom
    // zend_object derivation.

    // Make sure object doesn't already have a backing. This would imply it is
    // in use already in an refdb.
    if (backend != nullptr) {
        throw php_git2_error_exception("Cannot create custom refdb backend: object already in use");
    }

    // Set kind to 'custom'.
    kind = custom;

    // Create custom backend. Custom backends are always passed off to git2, so
    // we are not responsible for calling its free function.
    backend = new (emalloc(sizeof(git_refdb_backend_php)))
        git_refdb_backend_php(Z_OBJ_P(zobj));

    // Set backend owner to NULL. Custom ref backends never have a direct
    // owner. We always assume the would-be owner is kept alive circularly since
    // the custom backend stores a reference to this object.
    assign_owner(nullptr);
}

void php_refdb_backend_object::assign_owner(php_git_refdb* newOwner)
{
    // Free existing owner (if any).
    if (owner != nullptr) {
        git2_resource_base::free_recursive(owner);
    }

    owner = newOwner;

    // Increment owner refcount to prevent the ODB from freeing while the
    // backend object is in use.
    if (owner != nullptr) {
        owner->up_ref();
    }
}

/*static*/ int php_refdb_backend_object::exists(
    int *exists,
    git_refdb_backend *backend,
    const char *ref_name)
{
    int result;
    zval_array<1> params;
    method_wrapper method("exists",backend);

    ZVAL_STRING(params[0],ref_name);

    // Call userspace method implementation corresponding to refdb operation.

    result = method.call(params);
    if (result == GIT_OK) {
        zval* retval = method.retval();

        // Return method call return value.

        convert_to_boolean(retval);
        *exists = Z_TYPE_P(retval) == IS_TRUE;
    }

    return result;
}

/*static*/ int php_refdb_backend_object::lookup(
    git_reference **out,
    git_refdb_backend *backend,
    const char *ref_name)
{
    int result;
    zval_array<1> params;
    method_wrapper method("lookup",backend);

    ZVAL_STRING(params[0],ref_name);

    // Call userspace method implementation corresponding to refdb operation.

    result = method.call(params);
    if (result == GIT_OK) {
        zval* retval = method.retval();

        // Return reference from method call return value.

        if (Z_TYPE_P(retval) == IS_RESOURCE) {
            // Verify that the returned resource is a git_reference.
            void* result = zend_fetch_resource(
                Z_RES_P(retval),
                php_git_reference::resource_name(),
                php_git_reference::resource_le()
                );
            if (result == nullptr) {
                throw php_git2_propagated_exception();
            }

            // Extract reference, duplicate and return.
            php_git_reference* ref = reinterpret_cast<php_git_reference*>(
                Z_RES_VAL_P(retval)
                );
            git_reference_dup(out,ref->get_handle());
        }
        else {
            // Otherwise, interpret the return value as a string and convert to
            // reference.
            convert_to_string(retval);
            result = reference_from_string(out,retval,ref_name);
        }
    }

    return result;
}

/*static*/ int php_refdb_backend_object::iterator(
    git_reference_iterator **iter,
    struct git_refdb_backend *backend,
    const char *glob)
{
    int result;
    zval_array<1> params;
    method_wrapper method("iterator_new",backend);

    if (glob != nullptr) {
        ZVAL_STRING(params[0],glob);
    }

    // Call userspace method implementation corresponding to refdb operation.

    result = method.call(params);
    if (result == GIT_OK) {
        zval* thisobj = method.thisobj();
        custom_backend_iterator* custom;

        custom = reinterpret_cast<custom_backend_iterator*>(
            emalloc(sizeof(custom_backend_iterator))
            );
        memset(custom,0,sizeof(custom_backend_iterator));

        custom->next = (int(*)(git_reference**,git_reference_iterator*))custom_backend_iterator_next;
        custom->next_name =
            (int(*)(const char**,git_reference_iterator*))custom_backend_iterator_next_name;
        custom->free = (void(*)(git_reference_iterator*))custom_backend_iterator_free;

        ZVAL_COPY(&custom->thisobj,thisobj);
        ZVAL_NULL(&custom->zlast);

        *iter = custom;
    }

    return result;
}

/*static*/ int php_refdb_backend_object::write(
    git_refdb_backend *backend,
    const git_reference *ref,
    int force,
    const git_signature *who,
    const char *message,
    const git_oid *old,
    const char *old_target)
{
    int result;
    zval_array<6> params;
    method_wrapper method("write",backend);

    try {
        const php_resource_ref<php_git_reference_nofree> res;

        *res.byval_git2() = ref;
        res.ret(params[0]);

    } catch (php_git2_exception_base& ex) {
        php_git2_giterr_set(
            GITERR_INVALID,
            "GitRefDBBackend::write(): Failed to convert git_reference resource: %s",
            ex.what());

        return GIT_EPHP_ERROR;
    }

    ZVAL_BOOL(params[1],force);
    convert_signature(params[2],who);
    if (message != NULL) {
        ZVAL_STRING(params[3],message);
    }
    if (old != nullptr) {
        convert_oid(params[4],old);
    }
    if (old_target != nullptr) {
        ZVAL_STRING(params[5],old_target);
    }

    // Call userspace method implementation corresponding to refdb operation.

    result = method.call(params);

    return result;
}

/*static*/ int php_refdb_backend_object::rename(
    git_reference **out,
    git_refdb_backend *backend,
    const char *old_name,
    const char *new_name,
    int force,
    const git_signature *who,
    const char *message)
{
    int result;
    zval_array<5> params;
    method_wrapper method("rename",backend);

    ZVAL_STRING(params[0],old_name);
    ZVAL_STRING(params[1],new_name);
    ZVAL_BOOL(params[2],1);
    convert_signature(params[3],who);
    if (message != nullptr) {
        ZVAL_STRING(params[4],message);
    }

    // Call userspace method implementation corresponding to refdb operation.

    result = method.call(params);
    if (result == GIT_OK) {
        zval* retval = method.retval();

        convert_to_string(retval);
        result = reference_from_string(out,retval,new_name);
    }

    return result;
}

/*static*/ int php_refdb_backend_object::del(
    git_refdb_backend *backend,
    const char *ref_name,
    const git_oid *old_id,
    const char *old_target)
{
    int result;
    zval_array<3> params;
    method_wrapper method("del",backend);

    ZVAL_STRING(params[0],ref_name);
    if (old_id != nullptr) {
        convert_oid(params[1],old_id);
    }
    if (old_target != nullptr) {
        ZVAL_STRING(params[3],old_target);
    }

    // Call userspace method implementation corresponding to refdb operation.

    result = method.call(params);

    return result;
}

/*static*/ int php_refdb_backend_object::compress(git_refdb_backend *backend)
{
    int result;
    method_wrapper method("compress",backend);

    // Call userspace method implementation corresponding to refdb operation.

    result = method.call();

    return result;
}

/*static*/ int php_refdb_backend_object::has_log(
    git_refdb_backend *backend,
    const char *refname)
{
    int result;
    zval_array<1> params;
    method_wrapper method("has_log",backend);

    ZVAL_STRING(params[0],refname);

    // Call userspace method implementation corresponding to refdb operation.

    result = method.call(params);
    if (result == GIT_OK) {
        zval* retval = method.retval();

        convert_to_boolean(retval);
        result = Z_TYPE_P(retval) == IS_TRUE;
    }

    return result;
}

/*static*/ int php_refdb_backend_object::ensure_log(
    git_refdb_backend *backend,
    const char *refname)
{
    int result;
    zval_array<1> params;
    method_wrapper method("ensure_log",backend);

    ZVAL_STRING(params[0],refname);

    // Call userspace method implementation corresponding to refdb operation.

    result = method.call(params);

    return result;
}

/*static*/ int php_refdb_backend_object::reflog_read(
    git_reflog **out,
    git_refdb_backend *backend,
    const char *name)
{
    // TODO Until we can manipulate git_reflog and git_reflog_entry
    // appropriately, we cannot implement this function.

    giterr_set_str(
        GIT_ERROR,
        "GitRefDBBackend::reflog_read(): function is not supported at this time");

    return GIT_ERROR;

    int result;
    zval_array<1> params;
    method_wrapper method("reflog_read",backend);

    ZVAL_STRING(params[0],name);

    // Call userspace method implementation corresponding to refdb operation.

    result = method.call(params);
    if (result == GIT_OK) {
        zval* retval = method.retval();

        if (Z_TYPE_P(retval) != IS_ARRAY) {
            php_git2_giterr_set(
                GITERR_INVALID,
                "GitRefDBBackend::reflog_read(): return value must be array");

            result = GIT_ERROR;
        }
        else {
            zval* zlookup;
            HashTable* ht;
            HashPosition pos;
            git_reflog* log;

            // TODO Allocate reflog instance.
            log = nullptr;

            ht = Z_ARRVAL_P(retval);
            for (zend_hash_internal_pointer_reset_ex(ht,&pos);
                 (zlookup = zend_hash_get_current_data_ex(ht,&pos)) != nullptr;
                 zend_hash_move_forward_ex(ht,&pos))
            {
                git_reflog_entry* ent;

                if (reflog_entry_from_array(&ent,zlookup) == GIT_ERROR) {
                    result = GIT_ERROR;
                    break;
                }

                // TODO Append raw reflog entry to reflog.

            }

            if (result == GIT_ERROR) {
                git_reflog_free(log);
            }
            else {
                *out = log;
            }
        }
    }

    return result;
}

/*static*/ int php_refdb_backend_object::reflog_write(
    git_refdb_backend *backend,
    git_reflog *reflog)
{
    int result;
    zval_array<1> params;
    method_wrapper method("reflog_write",backend);

    try {
        php_resource_ref<php_git_reflog_nofree> reflogResource;

        *reflogResource.byval_git2() = reflog;
        reflogResource.ret(params[0]);

    } catch (php_git2_exception_base& ex) {
        php_git2_giterr_set(
            GITERR_INVALID,
            "GitRefDBBackend::reflog_write(): Failed to convert git_reflog resource: %s",
            ex.what());

        return GIT_EPHP_ERROR;
    }

    // Call userspace method implementation corresponding to refdb operation.

    result = method.call(params);

    return result;
}

/*static*/ int php_refdb_backend_object::reflog_rename(
    git_refdb_backend *backend,
    const char *old_name,
    const char *new_name)
{
    int result;
    zval_array<2> params;
    method_wrapper method("reflog_rename",backend);

    ZVAL_STRING(params[0],old_name);
    ZVAL_STRING(params[1],new_name);

    // Call userspace method implementation corresponding to refdb operation.

    result = method.call(params);

    return result;
}

/*static*/ int php_refdb_backend_object::reflog_delete(git_refdb_backend *backend,
    const char *name)
{
    int result;
    zval_array<1> params;
    method_wrapper method("reflog_delete",backend);

    ZVAL_STRING(params[0],name);

    // Call userspace method implementation corresponding to refdb operation.

    result = method.call(params);

    return result;
}

/*static*/ int php_refdb_backend_object::lock(void **payload_out,
    git_refdb_backend *backend,
    const char *refname)
{
    int result;
    zval_array<1> params;
    method_wrapper method("lock",backend);

    ZVAL_STRING(params[0],refname);

    // Call userspace method implementation corresponding to refdb operation.

    result = method.call(params);
    if (result == GIT_OK) {
        zval* lockobj = &method.object()->lockobj;

        // Copy the return value zval to the lock object zval.
        zval_ptr_dtor(lockobj);
        ZVAL_COPY(lockobj,method.retval());
        *payload_out = lockobj;
    }

    return result;
}

/*static*/ int php_refdb_backend_object::unlock(git_refdb_backend *backend,
    void *payload,
    int success,
    int update_reflog,
    const git_reference *ref,
    const git_signature *sig,
    const char *message)
{
    int result;
    zval* lockobj;
    zval_array<6> params;
    method_wrapper method("unlock",backend);

    lockobj = &method.object()->lockobj;
    if (payload == lockobj) {
        params.assign<0>(lockobj);
    }
    ZVAL_BOOL(params[1],success);
    ZVAL_BOOL(params[2],update_reflog);

    try {
        const php_resource_ref<php_git_reference_nofree> res;

        *res.byval_git2() = ref;
        res.ret(params[3]);

    } catch (php_git2_exception_base& ex) {
        php_git2_giterr_set(
            GITERR_INVALID,
            "GitRefDBBackend::unlock(): Failed to convert git_reference resource: %s",
            ex.what());

        return GIT_EPHP_ERROR;
    }

    convert_signature(params[4],sig);
    if (message != nullptr) {
        ZVAL_STRING(params[5],message);
    }

    // Call userspace method implementation corresponding to refdb operation.

    result = method.call(params);

    zval_ptr_dtor(lockobj);
    ZVAL_UNDEF(lockobj);

    return result;
}

/*static*/ void php_refdb_backend_object::free(git_refdb_backend *backend)
{
    method_wrapper::object_wrapper object(backend);

    // Make sure object buckets still exist to lookup object (in case the
    // destructor was already called). This happens when free() is called after
    // PHP has finished cleaning up all objects (but before all variables, which
    // may have references to git2 objects that reference this PHP object).
    if (EG(objects_store).object_buckets != nullptr) {
        // Set backend to null in internal storage.

        object.backing()->backend = nullptr;
    }

    // Call the destructor on the backend and free the structure.

    object.object()->~git_refdb_backend_php();
    efree(backend);
}

// Implementation of php_refdb_backend_object::git_refdb_backend_php

php_refdb_backend_object::git_refdb_backend_php::git_refdb_backend_php(zend_object* obj)
{
    // Blank out the base class (which is the structure defined in the git2
    // headers).
    memset(this,0,sizeof(struct git_odb_backend));

    // Assign zval to keep object alive while the backend is in use in the
    // library.
    ZVAL_OBJ(&thisobj,obj);
    Z_ADDREF(thisobj);

    ZVAL_UNDEF(&lockobj);
    version = GIT_REFDB_BACKEND_VERSION;

    // Every custom backend gets the free function (whether it is overloaded in
    // userspace or not).
    free = php_refdb_backend_object::free;

    // We now must select which functions we are going to include in the
    // backend. We do this by determining which ones were overloaded. PHP will
    // ensure that the abstract methods are always overloaded so we don't have
    // to check those.
    exists = php_refdb_backend_object::exists;
    lookup = php_refdb_backend_object::lookup;
    write = php_refdb_backend_object::write;
    rename = php_refdb_backend_object::rename;
    del = php_refdb_backend_object::del;
    has_log = php_refdb_backend_object::has_log;
    ensure_log = php_refdb_backend_object::ensure_log;
    reflog_read = php_refdb_backend_object::reflog_read;
    reflog_write = php_refdb_backend_object::reflog_write;
    reflog_rename = php_refdb_backend_object::reflog_rename;
    reflog_delete = php_refdb_backend_object::reflog_delete;
    if (is_method_overridden(obj->ce,"iterator_new",sizeof("iterator_new")-1)) {
        if (!is_method_overridden(obj->ce,"iterator_next",sizeof("iterator_next")-1)) {
            throw php_git2_error_exception(
                "Cannot create custom refdb backend: "
                "must implement iterator_next() with iterator_new()");
        }

        iterator = php_refdb_backend_object::iterator;
    }
    if (is_method_overridden(obj->ce,"compress",sizeof("compress")-1)) {
        compress = php_refdb_backend_object::compress;
    }
    if (is_method_overridden(obj->ce,"lock",sizeof("lock")-1)) {
        lock = php_refdb_backend_object::lock;

        // Require unlock if lock is provided. This seems to be in line with
        // what happens in git2 source code: unlock is not checked but only
        // called when lock is present.
        if (!is_method_overridden(obj->ce,"unlock",sizeof("unlock")-1)) {
            throw php_git2_error_exception(
                "Cannot create custom refdb backend: unlock() must be "
                "implemented with lock()");
        }

        unlock = php_refdb_backend_object::unlock;
    }
}

php_refdb_backend_object::git_refdb_backend_php::~git_refdb_backend_php()
{
    zval_ptr_dtor(&thisobj);
    zval_ptr_dtor(&lockobj);
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
