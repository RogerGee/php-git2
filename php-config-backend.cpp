/*
 * php-config-backend.cpp
 *
 * This file is a part of php-git2.
 *
 * This unit provides the out-of-line implementation for the GitConfigBackend
 * class.
 */

#include "php-object.h"
#include <new>
using namespace php_git2;

// Helper macros and functions

#define EXTRACT_BACKEND(backend) \
    reinterpret_cast<php_config_backend_object::git_config_backend_php*>(backend)

#define EXTRACT_THISOBJ(backend) \
    EXTRACT_BACKEND(backend)->thisobj

static void custom_backend_entry_free(git_config_entry* ent)
{
    pefree(ent->payload,1);
    pefree(ent,1);
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
    git_config_level_t level = GIT_CONFIG_LEVEL_APP;

    if (zend_hash_find(ht,"name",sizeof("name"),(void**)&zvpName) == FAILURE) {
        *err = "expected array element 'name'";
        return FAILURE;
    }
    if (zend_hash_find(ht,"value",sizeof("value"),(void**)&zvpValue) == FAILURE) {
        *err = "expected array element 'value'";
        return FAILURE;
    }
    if (zend_hash_find(ht,"level",sizeof("level"),(void**)&zvpLevel) == FAILURE) {
        // NOTE: level is optional and will default to GIT_CONFIG_LEVEL_APP.
        if (Z_TYPE_PP(zvpLevel) == IS_LONG) {
            level = (git_config_level_t)Z_LVAL_PP(zvpLevel);
        }
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
    buffer = (char*)pemalloc(name_len + value_len + 2,1);
    ent->name = buffer;
    ent->value = buffer + name_len + 1;
    memcpy(buffer,name,name_len+1);
    memcpy(buffer + name_len + 1,value,value_len+1);

    ent->level = level;
    ent->free = custom_backend_entry_free;
    ent->payload = buffer;

    return SUCCESS;
}

// Class method entries

zend_function_entry php_git2::config_backend_methods[] = {
    PHP_ABSTRACT_ME(GitConfigBackend,open,NULL)
    PHP_ABSTRACT_ME(GitConfigBackend,get,NULL)
    PHP_ABSTRACT_ME(GitConfigBackend,set,NULL)
    PHP_ABSTRACT_ME(GitConfigBackend,set_multivar,NULL)
    PHP_ABSTRACT_ME(GitConfigBackend,del,NULL)
    PHP_ABSTRACT_ME(GitConfigBackend,del_multivar,NULL)
    PHP_ABSTRACT_ME(GitConfigBackend,iterator,NULL)
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
    // Attempt to free the owner resource. This only really frees the owner if
    // its refcount reaches zero.
    if (owner != nullptr) {
        git2_resource_base::free_recursive(owner);
    }

    zend_object_std_dtor(this ZTS_MEMBER_CC(this->zts));
}

void php_config_backend_object::create_custom_backend(zval* zobj,php_git_config* newOwner)
{
    // NOTE: 'zobj' is the zval to the outer object that wraps this custom
    // zend_object derivation.

    // Make sure this config backend object is not in use anywhere else
    // (i.e. does not yet have a backing).
    if (backend != nullptr) {
        php_error(E_ERROR,"cannot create custom config backend - object already in use");
    }

    // Create custom backend. Custom backends are always passed off to git2, so
    // we are not responsible for calling its free function.
    backend = new (emalloc(sizeof(git_config_backend_php))) git_config_backend_php(zobj);

    // Assume new owner, increment refcount if set.
    owner = newOwner;
    if (owner != nullptr) {
        owner->up_ref();
    }
}

/*static*/ int php_config_backend_object::open(git_config_backend* cfg,git_config_level_t level)
{
    int result = GIT_OK;
    zval fname;
    zval retval;
    zval* zlevel;
    zval* thisobj = EXTRACT_THISOBJ(cfg);

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zlevel);

    // Assign method name and parameters.
    ZVAL_STRING(&fname,"open",1);
    ZVAL_LONG(zlevel,level);

    // Call userspace method.
    if (call_user_function(NULL,&thisobj,&fname,&retval,1,&zlevel
        ZTS_MEMBER_CC(object->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }
    else {
        // Allow userspace to indicate failure by returning false. If NULL is
        // returned, the function assumes success.
        if (Z_TYPE(retval) != IS_NULL) {
            convert_to_boolean(&retval);
            if (!Z_BVAL(retval)) {
                giterr_set_str(GITERR_CONFIG,"php-config-backend: fail open()");
                result = GIT_ERROR;
            }
        }
    }

    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zlevel);

    return result;
}

/*static*/ int php_config_backend_object::get(git_config_backend* cfg,const char* key,
    git_config_entry** out)
{
    int result = GIT_OK;
    zval fname;
    zval retval;
    zval* zkey;
    zval* thisobj = EXTRACT_THISOBJ(cfg);
    git_config_entry* entry;

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zkey);

    ZVAL_STRING(&fname,"get",1);
    ZVAL_STRING(zkey,key,1);

    if (call_user_function(NULL,&thisobj,&fname,&retval,1,&zkey
        ZTS_MEMBER_CC(object->zts)) == FAILURE)
    {
    }
    else {
        if (Z_TYPE(retval) == IS_ARRAY) {
            const char* err;
            entry = (git_config_entry*)pemalloc(sizeof(git_config_entry),1);
            memset(entry,0,sizeof(git_config_entry));
            if (set_custom_backend_entry(&retval,entry,&err) == FAILURE) {
                custom_backend_entry_free(entry);
                php_error(E_ERROR,"GitConfigBackend: get(): bad return value: %s",err);
                return GIT_ERROR;
            }
            *out = entry;
        }
        else {
            convert_to_boolean(&retval);
            if (!Z_BVAL(retval)) {
                result = GIT_ENOTFOUND;
            }
            else {
                php_error(E_ERROR,"GitConfigBackend: get(): return value must be an array");
                return GIT_ERROR;
            }
        }
    }

    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zkey);

    return result;
}

/*static*/ int php_config_backend_object::set(git_config_backend* cfg,const char* name,
    const char* value)
{
    int result = GIT_OK;

    return result;
}

/*static*/ int php_config_backend_object::set_multivar(git_config_backend* cfg,
    const char* name,const char* regexp,const char* value)
{
    int result = GIT_OK;

    return result;
}

/*static*/ int php_config_backend_object::del(git_config_backend* cfg,const char* name)
{
    int result = GIT_OK;

    return result;
}

/*static*/ int php_config_backend_object::del_multivar(git_config_backend* cfg,
    const char* name,const char* regexp)
{
    int result = GIT_OK;

    return result;
}

/*static*/ int php_config_backend_object::iterator(git_config_iterator** iter,
    git_config_backend* cfg)
{
    int result = GIT_OK;

    return result;
}

/*static*/ int php_config_backend_object::snapshot(git_config_backend** out,
    git_config_backend* cfg)
{
    int result = GIT_OK;

    return result;
}

/*static*/ int php_config_backend_object::lock(git_config_backend* cfg)
{
    int result = GIT_OK;

    return result;
}

/*static*/ int php_config_backend_object::unlock(git_config_backend* cfg,int success)
{
    int result = GIT_OK;

    return result;
}

/*static*/ void php_config_backend_object::free(git_config_backend* cfg)
{
}

// Implementation of php_config_backend_object::git_config_backend_php

php_config_backend_object::
git_config_backend_php::git_config_backend_php(zval* zv)
{
    // Blank out the base object to initialize.
    memset(this,0,sizeof(struct git_odb_backend));

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
