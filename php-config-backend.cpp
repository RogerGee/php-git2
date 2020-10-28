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
    zval** zvpName;
    zval** zvpValue;
    zval** zvpLevel;
    HashTable* ht = Z_ARRVAL_P(arr);
    const char* name, *value;
    zval cpyName;
    zval cpyValue;
    int name_len;
    int value_len;
    git_config_level_t level;

    if (zend_hash_find(ht,"name",sizeof("name"),(void**)&zvpName) == FAILURE) {
        *err = "expected array element 'name'";
        return FAILURE;
    }

    if (zend_hash_find(ht,"value",sizeof("value"),(void**)&zvpValue) == FAILURE) {
        *err = "expected array element 'value'";
        return FAILURE;
    }

    // NOTE: level is optional and will default to GIT_CONFIG_LEVEL_APP.
    if (zend_hash_find(ht,"level",sizeof("level"),(void**)&zvpLevel) == FAILURE) {
        level = GIT_CONFIG_LEVEL_APP;
    }
    else if (Z_TYPE_PP(zvpLevel) == IS_LONG) {
        level = (git_config_level_t)Z_LVAL_PP(zvpLevel);
    }
    else {
        zval cpyLevel;
        ZVAL_COPY_VALUE(&cpyLevel,*zvpLevel);
        zval_copy_ctor(&cpyLevel);
        convert_to_long(&cpyLevel);
        level = (git_config_level_t)Z_LVAL(cpyLevel);
    }

    if (Z_TYPE_PP(zvpName) != IS_STRING) {
        ZVAL_COPY_VALUE(&cpyName,*zvpName);
        zval_copy_ctor(&cpyName);
        convert_to_string(&cpyName);
        name = Z_STRVAL(cpyName);
    }
    else {
        INIT_ZVAL(cpyName);
        name = Z_STRVAL_P(*zvpName);
    }

    if (Z_TYPE_PP(zvpValue) != IS_STRING) {
        ZVAL_COPY_VALUE(&cpyValue,*zvpValue);
        zval_copy_ctor(&cpyValue);
        convert_to_string(&cpyValue);
        value = Z_STRVAL(cpyValue);
    }
    else {
        INIT_ZVAL(cpyValue);
        value = Z_STRVAL_P(*zvpValue);
    }

    // Copy name and length into buffer and assign sections to fields.
    name_len = strlen(name);
    value_len = strlen(value);
    buffer = (char*)emalloc(name_len + value_len + 2);
    ent->name = buffer;
    ent->value = buffer + name_len + 1;
    memcpy(buffer,name,name_len+1);
    memcpy(buffer + name_len + 1,value,value_len+1);

    ent->level = level;
    ent->free = custom_backend_entry_free;
    ent->payload = buffer;

    zval_dtor(&cpyName);
    zval_dtor(&cpyValue);

    return SUCCESS;
}

struct custom_backend_iterator : git_config_iterator
{
    typedef git_config_iterator base_class;

    using method_wrapper = php_method_wrapper<
        custom_backend_iterator,
        php_config_backend_object
        >;

    // Custom backend object handling iteration. This field needs to be named
    // 'thisobj' so the method_wrapper can access it.
    zval* thisobj;

    // Context parameter for method call.
    zval* zcontext;
};

static void custom_backend_iterator_free(custom_backend_iterator* iter)
{
    zval_ptr_dtor(&iter->thisobj);
    if (iter->zcontext != nullptr) {
        zval_ptr_dtor(&iter->zcontext);
    }
    efree(iter);
}

static int custom_backend_iterator_next(git_config_entry** out,custom_backend_iterator* iter)
{
    int result;
    int nparams;
    zval** params;
    custom_backend_iterator::method_wrapper method("iterator_next",iter);

    if (iter->zcontext) {
        nparams = 1;
        params = &iter->zcontext;
    }
    else {
        nparams = 0;
        params = NULL;
    }

    result = method.call(nparams,params);
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

                result = GIT_EPHPFATAL;
            }
            else {
                *out = entry;
            }
        }
        else {
            convert_to_boolean(retval);

            if (!Z_BVAL_P(retval)) {
                result = GIT_ITEROVER;
            }
            else {
                php_git2_giterr_set(
                    GITERR_CONFIG,
                    "GitConfigBackend::iterator_next(): return value must be an array");

                result = GIT_EPHPFATAL;
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

// Implementation of php_config_backend_object

/*static*/ void php_config_backend_object::init(zend_class_entry* ce)
{

}

/*static*/ zend_object_handlers php_config_backend_object::handlers;
php_config_backend_object::php_config_backend_object(zend_class_entry* ce TSRMLS_DC):
    backend(nullptr), owner(nullptr), zts(TSRMLS_C)
{
    zend_object_std_init(this,ce TSRMLS_CC);
    object_properties_init(this,ce);
}

php_config_backend_object::~php_config_backend_object()
{
    zend_object_std_dtor(this ZTS_MEMBER_CC(this->zts));
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
        throw php_git2_fatal_exception("Cannot create custom config backend: object already in use");
    }

    // Create custom backend. Custom backends are always passed off to git2, so
    // we are not responsible for calling its free function.
    backend = new (emalloc(sizeof(git_config_backend_php))) git_config_backend_php(zobj);

    // Assume new owner. This should be a non-null value.
    owner = newOwner;
}

/*static*/ int php_config_backend_object::open(git_config_backend* cfg,git_config_level_t level)
{
    int result;
    zval* zlevel;
    method_wrapper method("open",cfg);

    // Allocate/set zvals.

    MAKE_STD_ZVAL(zlevel);
    ZVAL_LONG(zlevel,level);

    // Call userspace method implementation corresponding to config backend
    // operation.

    zval* params[] = { zlevel };

    result = method.call(1,params);

    // Cleanup zvals.

    zval_ptr_dtor(&zlevel);

    return result;
}

/*static*/ int php_config_backend_object::get(git_config_backend* cfg,const char* key,
    git_config_entry** out)
{
    int result;
    zval* zkey;
    method_wrapper method("get",cfg);

    // Allocate/set zvals.

    MAKE_STD_ZVAL(zkey);
    ZVAL_STRING(zkey,key,1);

    // Call userspace method implementation corresponding to config backend
    // operation.

    zval* params[] = { zkey };

    result = method.call(1,params);
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

                result = GIT_EPHPFATAL;
            }
            else {
                *out = entry;
            }
        }
        else {
            convert_to_boolean(retval);

            if (!Z_BVAL_P(retval)) {
                result = GIT_ENOTFOUND;
            }
            else {
                php_git2_giterr_set(
                    GITERR_CONFIG,
                    "GitConfigBackend::get(): return value must be an array");

                result = GIT_EPHPFATAL;
            }
        }
    }

    // Cleanup zvals.

    zval_ptr_dtor(&zkey);

    return result;
}

/*static*/ int php_config_backend_object::set(git_config_backend* cfg,const char* name,
    const char* value)
{
    int result;
    zval* zname;
    zval* zvalue;
    method_wrapper method("set",cfg);

    // Allocate/set zvals.

    MAKE_STD_ZVAL(zname);
    MAKE_STD_ZVAL(zvalue);
    ZVAL_STRING(zname,name,1);
    ZVAL_STRING(zvalue,value,1);

    // Call userspace method implementation corresponding to config backend
    // operation.

    zval* params[] = { zname, zvalue };

    result = method.call(2,params);

    // Cleanup zvals.

    zval_ptr_dtor(&zname);
    zval_ptr_dtor(&zvalue);

    return result;
}

/*static*/ int php_config_backend_object::set_multivar(git_config_backend* cfg,
    const char* name,const char* regexp,const char* value)
{
    int result;
    zval* zname;
    zval* zregexp;
    zval* zvalue;
    method_wrapper method("set_multivar",cfg);

    // Allocate/set zvals.

    MAKE_STD_ZVAL(zname);
    MAKE_STD_ZVAL(zregexp);
    MAKE_STD_ZVAL(zvalue);
    ZVAL_STRING(zname,name,1);
    ZVAL_STRING(zregexp,regexp,1);
    ZVAL_STRING(zvalue,value,1);

    // Call userspace method implementation corresponding to config backend
    // operation.

    zval* params[] = { zname, zregexp, zvalue };

    result = method.call(3,params);

    // Cleanup zvals.

    zval_ptr_dtor(&zname);
    zval_ptr_dtor(&zregexp);
    zval_ptr_dtor(&zvalue);

    return result;
}

/*static*/ int php_config_backend_object::del(git_config_backend* cfg,const char* name)
{
    int result;
    zval* zname;
    method_wrapper method("del",cfg);

    // Allocate/set zvals.

    MAKE_STD_ZVAL(zname);
    ZVAL_STRING(zname,name,1);

    // Call userspace method implementation corresponding to config backend
    // operation.

    zval* params[] = { zname };

    result = method.call(1,params);

    // Cleanup zvals.

    zval_ptr_dtor(&zname);

    return result;
}

/*static*/ int php_config_backend_object::del_multivar(git_config_backend* cfg,
    const char* name,const char* regexp)
{
    int result;
    zval* zname;
    zval* zregexp;
    method_wrapper method("del_multivar",cfg);

    // Allocate/set zvals.

    MAKE_STD_ZVAL(zname);
    MAKE_STD_ZVAL(zregexp);
    ZVAL_STRING(zname,name,1);
    ZVAL_STRING(zregexp,regexp,1);

    // Call userspace method implementation corresponding to config backend
    // operation.

    zval* params[] = { zname, zregexp };

    result = method.call(1,params);

    // Cleanup zvals.

    zval_ptr_dtor(&zname);
    zval_ptr_dtor(&zregexp);

    return result;
}

/*static*/ int php_config_backend_object::iterator(git_config_iterator** out,
    git_config_backend* cfg)
{
    int result;
    method_wrapper method("iterator_new",cfg);

    // Call userspace method implementation corresponding to config backend
    // operation.

    result = method.call();
    if (result == GIT_OK) {
        zval* retval = method.retval();
        zval* thisobj = method.thisobj();
        custom_backend_iterator* iter;

        iter = (custom_backend_iterator*)emalloc(sizeof(custom_backend_iterator));
        iter->backend = cfg;
        iter->flags = 0;
        iter->next = (int(*)(git_config_entry**,git_config_iterator*))custom_backend_iterator_next;
        iter->free = (void(*)(git_config_iterator*))custom_backend_iterator_free;

        Z_ADDREF_P(thisobj);
        iter->thisobj = thisobj;

        // Copy return value context into the iterator. Otherwise leave null.
        if (Z_TYPE_P(retval) != IS_NULL) {
            MAKE_STD_ZVAL(iter->zcontext);
            ZVAL_COPY_VALUE(iter->zcontext,retval);
            zval_copy_ctor(iter->zcontext);
        }
        else {
            iter->zcontext = nullptr;
        }

        *out = iter;
    }

    return result;
}

/*static*/ int php_config_backend_object::snapshot(git_config_backend** out,
    git_config_backend* cfg)
{
    int result;
    method_wrapper method("snapshot",cfg);

    ZTS_MEMBER_EXTRACT(method.backing()->zts);

    // Call userspace method implementation corresponding to config backend
    // operation.

    result = method.call();
    if (result == GIT_OK) {
        zval* retval = method.retval();

        // Make sure the function returned an object that extends
        // GitConfigBackend.
        if (Z_TYPE_P(retval) != IS_OBJECT) {
            php_git2_giterr_set(
                GITERR_CONFIG,
                "GitConfigBackend::snapshot(): return value must be object");

            result = GIT_EPHPFATAL;
        }
        else if (!is_subclass_of(Z_OBJCE_P(retval),class_entry[php_git2_config_backend_obj])) {
            php_git2_giterr_set(
                GITERR_CONFIG,
                "GitConfigBackend::snapshot(): return value object type must extend GitConfigBackend");

            result = GIT_EPHPFATAL;
        }
        else {
            zval* zobj;

            // Create git_config_backend that wraps the object. It will point to
            // the same set of functions. It is up to userspace to enforce any
            // readonly behavior as desired.

            // Copy return value object into new zval.
            MAKE_STD_ZVAL(zobj);
            ZVAL_COPY_VALUE(zobj,retval);
            zval_copy_ctor(zobj);

            // Extract backend object and create the custom backend. We must do
            // this in an exception handler since create_custom_backend() is
            // designed to be called from userspace code.
            try {
                php_config_backend_object* internal;

                internal = LOOKUP_OBJECT(php_config_backend_object,zobj);
                internal->create_custom_backend(zobj,method.backing()->owner);
                *out = internal->backend;

            } catch (php_git2_exception_base& ex) {
                const char* msg = ex.what();
                if (msg == nullptr) {
                    msg = "GitConfigBackend::snapshot(): failed to create snapshot";
                }
                php_git2_giterr_set(GITERR_CONFIG,msg);

                result = GIT_EPHPFATAL;
                zval_ptr_dtor(&zobj);
            }
        }
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
    zval* zsuccess;
    method_wrapper method("unlock",cfg);

    // Allocate/set zvals.

    MAKE_STD_ZVAL(zsuccess);
    ZVAL_BOOL(zsuccess,success);

    // Call userspace method implementation corresponding to config backend
    // operation.

    zval* params[] = { zsuccess };

    result = method.call(1,params);

    // Cleanup zvals.

    zval_ptr_dtor(&zsuccess);

    return result;
}

/*static*/ void php_config_backend_object::free(git_config_backend* cfg)
{
    // Set backend to null in internal storage (just in case). Then explicitly
    // call the destructor on the custom backend. Finally free the block of
    // memory that holds the custom backend.

    method_wrapper::object_wrapper object(cfg);

    ZTS_MEMBER_EXTRACT(object.backing()->zts);

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
git_config_backend_php::git_config_backend_php(zval* zv)
{
    // Blank out the base object to initialize.
    memset(this,0,sizeof(struct git_config_backend));

    // Assign zval to keep object alive while backend is in use in the library.
    Z_ADDREF_P(zv);
    thisobj = zv;

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
