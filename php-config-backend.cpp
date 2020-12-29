/*
 * php-config-backend.cpp
 *
 * Copyright (C) Roger P. Gee
 *
 * This unit provides the out-of-line implementation for the GitConfigBackend
 * class.
 */

#include "php-object.h"
#include <new>
using namespace php_git2;

// Helper functions

static void custom_backend_entry_free(git_config_entry* ent)
{
    efree(ent->payload);
    efree(ent);
}

static int set_custom_backend_entry(zval* arr,git_config_entry* ent,const char** err)
{
    char* buffer;
    zval* zname;
    zval* zvalue;
    zval* zlevel;
    zend_string* name;
    zend_string* value;
    git_config_level_t level;
    zend_string* sname = nullptr;
    zend_string* svalue = nullptr;
    HashTable* ht = Z_ARRVAL_P(arr);

    zname = zend_hash_str_find(ht,"name",sizeof("name")-1);
    if (zname == nullptr) {
        *err = "expected array element 'name'";
        return FAILURE;
    }

    zvalue = zend_hash_str_find(ht,"value",sizeof("value")-1);
    if (zvalue == nullptr) {
        *err = "expected array element 'value'";
        return FAILURE;
    }

    // NOTE: level is optional and will default to GIT_CONFIG_LEVEL_APP.
    zlevel = zend_hash_str_find(ht,"level",sizeof("level")-1);
    if (zlevel == nullptr) {
        level = GIT_CONFIG_LEVEL_APP;
    }
    else if (Z_TYPE_P(zlevel) == IS_LONG) {
        level = (git_config_level_t)Z_LVAL_P(zlevel);
    }
    else {
        level = (git_config_level_t)zval_get_long(zlevel);
    }

    if (Z_TYPE_P(zname) != IS_STRING) {
        sname = zval_get_string(zname);
        name = sname;
    }
    else {
        name = Z_STR_P(zname);
    }

    if (Z_TYPE_P(zvalue) != IS_STRING) {
        svalue = zval_get_string(zvalue);
        value = svalue;
    }
    else {
        value = Z_STR_P(zvalue);
    }

    // Copy name and length into buffer and assign sections to fields.
    buffer = (char*)emalloc(ZSTR_LEN(name) + ZSTR_LEN(value) + 2);
    ent->name = buffer;
    ent->value = buffer + ZSTR_LEN(name) + 1;
    memcpy(buffer,ZSTR_VAL(name),ZSTR_LEN(name)+1);
    memcpy(buffer + ZSTR_LEN(name) + 1,ZSTR_VAL(value),ZSTR_LEN(value)+1);

    ent->level = level;
    ent->free = custom_backend_entry_free;
    ent->payload = buffer;

    if (sname != nullptr) {
        zend_string_release(sname);
    }
    if (svalue != nullptr) {
        zend_string_release(svalue);
    }

    return SUCCESS;
}

struct custom_backend_iterator : git_config_iterator
{
    typedef git_config_iterator base_class;

    using method_wrapper = php_method_wrapper<
        custom_backend_iterator,
        php_config_backend_object
        >;

    // Custom backend object that handles iteration. This field needs to be
    // named 'thisobj' so the method_wrapper can access it.
    zval thisobj;

    // Context parameter for method call.
    zval context;
};

static void custom_backend_iterator_free(custom_backend_iterator* iter)
{
    zval_ptr_dtor(&iter->thisobj);
    if (Z_TYPE(iter->context) != IS_UNDEF) {
        zval_ptr_dtor(&iter->context);
    }
    efree(iter);
}

static int custom_backend_iterator_next(git_config_entry** out,custom_backend_iterator* iter)
{
    int result;
    custom_backend_iterator::method_wrapper method("iterator_next",iter);

    if (Z_TYPE(iter->context) != IS_UNDEF) {
        zval_array<1> params;
        params.assign<0>(&iter->context);
        result = method.call(params);
    }
    else {
        result = method.call();
    }

    if (result == GIT_OK) {
        zval* retval = method.retval();

        if (Z_TYPE_P(retval) == IS_ARRAY) {
            const char* err;
            git_config_entry* entry;

            entry = (git_config_entry*)emalloc(sizeof(git_config_entry));
            memset(entry,0,sizeof(git_config_entry));

            if (set_custom_backend_entry(retval,entry,&err) == FAILURE) {
                custom_backend_entry_free(entry);
                php_git2_giterr_set(
                    GITERR_CONFIG,
                    "GitConfigBackend::iterator_next(): bad return value: %s",
                    err);

                result = GIT_EPHP_ERROR;
            }
            else {
                *out = entry;
            }
        }
        else {
            convert_to_boolean(retval);

            if (!Z_LVAL_P(retval)) {
                result = GIT_ITEROVER;
            }
            else {
                php_git2_giterr_set(
                    GITERR_CONFIG,
                    "GitConfigBackend::iterator_next(): return value must be an array");

                result = GIT_EPHP_ERROR;
            }
        }
    }

    return result;
}

// Class method entries

zend_function_entry php_git2::config_backend_methods[] = {
    PHP_ABSTRACT_ME(GitConfigBackend,open,NULL)
    PHP_ABSTRACT_ME(GitConfigBackend,get,NULL)
    PHP_ABSTRACT_ME(GitConfigBackend,set,NULL)
    PHP_ABSTRACT_ME(GitConfigBackend,set_multivar,NULL)
    PHP_ABSTRACT_ME(GitConfigBackend,del,NULL)
    PHP_ABSTRACT_ME(GitConfigBackend,del_multivar,NULL)
    PHP_ABSTRACT_ME(GitConfigBackend,iterator_new,NULL)
    PHP_ABSTRACT_ME(GitConfigBackend,iterator_next,NULL)
    PHP_ABSTRACT_ME(GitConfigBackend,snapshot,NULL)
    PHP_ABSTRACT_ME(GitConfigBackend,lock,NULL)
    PHP_ABSTRACT_ME(GitConfigBackend,unlock,NULL)
    PHP_FE_END
};

// php_zend_object init function

template<>
void php_zend_object<php_config_backend_object>::init(zend_class_entry* ce)
{
    UNUSED(ce);
}

// Implementation of php_config_backend_object

php_config_backend_object::php_config_backend_object():
    backend(nullptr), owner(nullptr)
{
}

void php_config_backend_object::create_custom_backend(zval* zobj,php_git_config* newOwner)
{
    // NOTE: this member function should be called under a php-git2 exception
    // handler since it will throw.

    // NOTE: 'zobj' is the zval to the outer object that wraps this custom
    // zend_object derivation.

    // Make sure this config backend object is not in use anywhere else
    // (i.e. does not yet have a backing).
    if (backend != nullptr) {
        throw php_git2_error_exception(
            "Cannot create custom config backend: object already in use");
    }

    // Create custom backend. Custom backends are always passed off to git2, so
    // we are not responsible for calling its free function.
    backend = new (emalloc(sizeof(git_config_backend_php)))
        git_config_backend_php(Z_OBJ_P(zobj));

    // Assume new owner. This should be a non-null value.
    owner = newOwner;
}

/*static*/ int php_config_backend_object::open(git_config_backend* cfg,git_config_level_t level)
{
    int result;
    zval_array<1> params;
    method_wrapper method("open",cfg);

    // Allocate/set zvals.

    ZVAL_LONG(params[0],level);

    // Call userspace method implementation corresponding to config backend
    // operation.

    result = method.call(params);

    return result;
}

/*static*/ int php_config_backend_object::get(
    git_config_backend* cfg,
    const char* key,
    git_config_entry** out)
{
    int result;
    zval_array<1> params;
    method_wrapper method("get",cfg);

    // Allocate/set zvals.

    ZVAL_STRING(params[0],key);

    // Call userspace method implementation corresponding to config backend
    // operation.

    result = method.call(params);
    if (result == GIT_OK) {
        zval* retval = method.retval();

        if (Z_TYPE_P(retval) == IS_ARRAY) {
            const char* err;
            git_config_entry* entry;

            entry = (git_config_entry*)emalloc(sizeof(git_config_entry));
            memset(entry,0,sizeof(git_config_entry));

            if (set_custom_backend_entry(retval,entry,&err) == FAILURE) {
                custom_backend_entry_free(entry);

                php_git2_giterr_set(
                    GITERR_CONFIG,
                    "GitConfigBackend::get(): bad return value: %s",
                    err);

                result = GIT_EPHP_ERROR;
            }
            else {
                *out = entry;
            }
        }
        else {
            convert_to_boolean(retval);

            if (!Z_LVAL_P(retval)) {
                result = GIT_ENOTFOUND;
            }
            else {
                php_git2_giterr_set(
                    GITERR_CONFIG,
                    "GitConfigBackend::get(): return value must be an array");

                result = GIT_EPHP_ERROR;
            }
        }
    }

    return result;
}

/*static*/ int php_config_backend_object::set(
    git_config_backend* cfg,
    const char* name,
    const char* value)
{
    int result;
    zval_array<2> params;
    method_wrapper method("set",cfg);

    // Allocate/set zvals.

    ZVAL_STRING(params[0],name);
    ZVAL_STRING(params[1],value);

    // Call userspace method implementation corresponding to config backend
    // operation.

    result = method.call(params);

    return result;
}

/*static*/ int php_config_backend_object::set_multivar(
    git_config_backend* cfg,
    const char* name,
    const char* regexp,
    const char* value)
{
    int result;
    zval_array<3> params;
    method_wrapper method("set_multivar",cfg);

    // Allocate/set zvals.

    ZVAL_STRING(params[0],name);
    ZVAL_STRING(params[1],regexp);
    ZVAL_STRING(params[2],value);

    // Call userspace method implementation corresponding to config backend
    // operation.

    result = method.call(params);

    return result;
}

/*static*/ int php_config_backend_object::del(git_config_backend* cfg,const char* name)
{
    int result;
    zval_array<1> params;
    method_wrapper method("del",cfg);

    // Allocate/set zvals.

    ZVAL_STRING(params[0],name);

    // Call userspace method implementation corresponding to config backend
    // operation.

    result = method.call(params);

    return result;
}

/*static*/ int php_config_backend_object::del_multivar(
    git_config_backend* cfg,
    const char* name,
    const char* regexp)
{
    int result;
    zval_array<2> params;
    method_wrapper method("del_multivar",cfg);

    // Allocate/set zvals.

    ZVAL_STRING(params[0],name);
    ZVAL_STRING(params[1],regexp);

    // Call userspace method implementation corresponding to config backend
    // operation.

    result = method.call(params);

    return result;
}

/*static*/ int php_config_backend_object::iterator(
    git_config_iterator** out,
    git_config_backend* cfg)
{
    int result;
    method_wrapper method("iterator_new",cfg);

    // Call userspace method implementation corresponding to config backend
    // operation.

    result = method.call();
    if (result == GIT_OK) {
        custom_backend_iterator* iter;
        zval* retval = method.retval();
        zval* thisobj = method.thisobj();

        iter = (custom_backend_iterator*)emalloc(sizeof(custom_backend_iterator));
        iter->backend = cfg;
        iter->flags = 0;
        iter->next = (int(*)(git_config_entry**,git_config_iterator*))custom_backend_iterator_next;
        iter->free = (void(*)(git_config_iterator*))custom_backend_iterator_free;

        ZVAL_COPY(&iter->thisobj,thisobj);

        // Copy return value context into the iterator. Otherwise leave null.
        if (Z_TYPE_P(retval) != IS_NULL) {
            ZVAL_COPY(&iter->context,retval);
        }
        else {
            ZVAL_UNDEF(&iter->context);
        }

        *out = iter;
    }

    return result;
}

/*static*/ int php_config_backend_object::snapshot(
    git_config_backend** out,
    git_config_backend* cfg)
{
    int result;
    method_wrapper method("snapshot",cfg);

    // Call userspace method implementation corresponding to config backend
    // operation.

    result = method.call();
    if (result != GIT_OK) {
        return result;
    }

    try {
        php_config_backend_object* internal;
        php_object<php_config_backend_object> obj;

        // Extract backend object and create the custom backend.
        obj.parse_with_context(method.retval(),"GitConfigBackend::snapshot()");
        internal = obj.get_storage();
        internal->create_custom_backend(obj.get_value(),method.backing()->owner);
        *out = internal->backend;

    } catch (php_git2_exception_base& ex) {
        const char* msg = ex.what();
        if (msg == nullptr) {
            msg = "GitConfigBackend::snapshot(): failed to create snapshot";
        }
        php_git2_giterr_set(GITERR_CONFIG,msg);
        result = GIT_EPHP_ERROR;
    }

    return result;
}

/*static*/ int php_config_backend_object::lock(git_config_backend* cfg)
{
    int result;
    method_wrapper method("lock",cfg);

    // Call userspace method implementation corresponding to config backend
    // operation.

    result = method.call();

    return result;
}

/*static*/ int php_config_backend_object::unlock(git_config_backend* cfg,int success)
{
    int result;
    zval_array<1> params;
    method_wrapper method("unlock",cfg);

    // Allocate/set zvals.

    ZVAL_BOOL(params[0],success);

    // Call userspace method implementation corresponding to config backend
    // operation.

    result = method.call(params);

    return result;
}

/*static*/ void php_config_backend_object::free(git_config_backend* cfg)
{
    // Set backend to null in internal storage (just in case). Then explicitly
    // call the destructor on the custom backend. Finally free the block of
    // memory that holds the custom backend.

    method_wrapper::object_wrapper object(cfg);

    // Make sure object buckets still exist to lookup object (in case the
    // destructor was already called).
    if (EG(objects_store).object_buckets != nullptr) {
        object.backing()->backend = nullptr;
    }

    object.object()->~git_config_backend_php();
    efree(cfg);
}

// Implementation of php_config_backend_object::git_config_backend_php

php_config_backend_object::
git_config_backend_php::git_config_backend_php(zend_object* obj)
{
    // Blank out the base object to initialize.
    memset(this,0,sizeof(struct git_config_backend));

    // Assign zend_object to local zval to keep object alive while backend is in
    // use in the library.
    ZVAL_OBJ(&thisobj,obj);

    version = GIT_CONFIG_BACKEND_VERSION;

    // Every custom backend gets a free function (whether overloaded from
    // userspace or not).
    free = php_config_backend_object::free;

    // Assign functions to the backend. Every function is required and must
    // exist since the class has all abstract methods.
    open = php_config_backend_object::open;
    get = php_config_backend_object::get;
    set = php_config_backend_object::set;
    set_multivar = php_config_backend_object::set_multivar;
    del = php_config_backend_object::del;
    del_multivar = php_config_backend_object::del_multivar;
    iterator = php_config_backend_object::iterator;
    snapshot = php_config_backend_object::snapshot;
    lock = php_config_backend_object::lock;
    unlock = php_config_backend_object::unlock;
}

php_config_backend_object::
git_config_backend_php::~git_config_backend_php()
{
    // Free object zval.
    zval_ptr_dtor(&thisobj);
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
