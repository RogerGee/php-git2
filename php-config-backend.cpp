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
    // Custom backend object handling iteration.
    zval* zobject;

    // Context parameter for method call.
    zval* zcontext;
};

static void custom_backend_iterator_free(custom_backend_iterator* iter)
{
    zval_ptr_dtor(&iter->zobject);
    if (iter->zcontext != NULL) {
        zval_ptr_dtor(&iter->zcontext);
    }
    efree(iter);
}

static int custom_backend_iterator_next(git_config_entry** out,custom_backend_iterator* iter)
{
    int result = GIT_OK;
    int nparams;
    zval** params;
    zval fname;
    zval retval;

    // We only need to lookup the object backing to get at the ZTS globals.
#ifdef ZTS
    php_config_backend_object* object;
    object = LOOKUP_OBJECT(php_config_backend_object,iter->zobject);
#endif

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ZVAL_STRING(&fname,"iterator_next",1);

    if (iter->zcontext) {
        nparams = 1;
        params = &iter->zcontext;
    }
    else {
        nparams = 0;
        params = NULL;
    }

    if (call_user_function(NULL,&iter->zobject,&fname,&retval,nparams,params
            ZTS_MEMBER_CC(object->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }
    else {
        if (Z_TYPE(retval) == IS_ARRAY) {
            const char* err;
            git_config_entry* entry;
            entry = (git_config_entry*)emalloc(sizeof(git_config_entry));
            memset(entry,0,sizeof(git_config_entry));
            if (set_custom_backend_entry(&retval,entry,&err) == FAILURE) {
                custom_backend_entry_free(entry);
                php_git2_giterr_set(
                    GITERR_CONFIG,
                    "GitConfigBackend::iterator_next(): bad return value: %s",
                    err);

                return GIT_EPHPFATAL;
            }
            *out = entry;
        }
        else {
            convert_to_boolean(&retval);
            if (!Z_BVAL(retval)) {
                result = GIT_ITEROVER;
            }
            else {
                php_git2_giterr_set(
                    GITERR_CONFIG,
                    "GitConfigBackend::iterator_next(): return value must be an array");

                return GIT_EPHPFATAL;
            }
        }
    }

    zval_dtor(&fname);
    zval_dtor(&retval);

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
    int result = GIT_OK;
    zval fname;
    zval retval;
    zval* zlevel;
    zval* thisobj = EXTRACT_THISOBJ(cfg);

    // We only need to lookup the object backing to get at the ZTS globals.
#ifdef ZTS
    php_config_backend_object* object;
    object = LOOKUP_OBJECT(php_config_backend_object,thisobj);
#endif

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

    // We only need to lookup the object backing to get at the ZTS globals.
#ifdef ZTS
    php_config_backend_object* object;
    object = LOOKUP_OBJECT(php_config_backend_object,thisobj);
#endif

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zkey);

    ZVAL_STRING(&fname,"get",1);
    ZVAL_STRING(zkey,key,1);

    if (call_user_function(NULL,&thisobj,&fname,&retval,1,&zkey
        ZTS_MEMBER_CC(object->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }
    else {
        if (Z_TYPE(retval) == IS_ARRAY) {
            const char* err;
            entry = (git_config_entry*)emalloc(sizeof(git_config_entry));
            memset(entry,0,sizeof(git_config_entry));
            if (set_custom_backend_entry(&retval,entry,&err) == FAILURE) {
                custom_backend_entry_free(entry);

                php_git2_giterr_set(
                    GITERR_CONFIG,
                    "GitConfigBackend::get(): bad return value: %s",
                    err);

                return GIT_EPHPFATAL;
            }
            *out = entry;
        }
        else {
            convert_to_boolean(&retval);
            if (!Z_BVAL(retval)) {
                result = GIT_ENOTFOUND;
            }
            else {
                php_git2_giterr_set(
                    GITERR_CONFIG,
                    "GitConfigBackend::get(): return value must be an array");

                return GIT_EPHPFATAL;
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
    zval fname;
    zval retval;
    zval* zname;
    zval* zvalue;
    zval* thisobj = EXTRACT_THISOBJ(cfg);
    zval* params[2];

    // We only need to lookup the object backing to get at the ZTS globals.
#ifdef ZTS
    php_config_backend_object* object;
    object = LOOKUP_OBJECT(php_config_backend_object,thisobj);
#endif

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zname);
    ALLOC_INIT_ZVAL(zvalue);
    ZVAL_STRING(&fname,"set",1);
    ZVAL_STRING(zname,name,1);
    ZVAL_STRING(zvalue,value,1);
    params[0] = zname;
    params[1] = zvalue;

    if (call_user_function(NULL,&thisobj,&fname,&retval,2,params
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
                giterr_set_str(GITERR_CONFIG,"php-config-backend: fail set()");
                result = GIT_ERROR;
            }
        }
    }

    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zname);
    zval_ptr_dtor(&zvalue);

    return result;
}

/*static*/ int php_config_backend_object::set_multivar(git_config_backend* cfg,
    const char* name,const char* regexp,const char* value)
{
    int result = GIT_OK;
    zval fname;
    zval retval;
    zval* zname;
    zval* zregexp;
    zval* zvalue;
    zval* thisobj = EXTRACT_THISOBJ(cfg);
    zval* params[3];

    // We only need to lookup the object backing to get at the ZTS globals.
#ifdef ZTS
    php_config_backend_object* object;
    object = LOOKUP_OBJECT(php_config_backend_object,thisobj);
#endif

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zname);
    ALLOC_INIT_ZVAL(zregexp);
    ALLOC_INIT_ZVAL(zvalue);
    ZVAL_STRING(&fname,"set_multivar",1);
    ZVAL_STRING(zname,name,1);
    ZVAL_STRING(zregexp,regexp,1);
    ZVAL_STRING(zvalue,value,1);
    params[0] = zname;
    params[1] = zregexp;
    params[2] = zvalue;

    if (call_user_function(NULL,&thisobj,&fname,&retval,3,params
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
                giterr_set_str(GITERR_CONFIG,"php-config-backend: fail set_multivar()");
                result = GIT_ERROR;
            }
        }
    }

    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zname);
    zval_ptr_dtor(&zregexp);
    zval_ptr_dtor(&zvalue);

    return result;
}

/*static*/ int php_config_backend_object::del(git_config_backend* cfg,const char* name)
{
    int result = GIT_OK;
    zval fname;
    zval retval;
    zval* zname;
    zval* thisobj = EXTRACT_THISOBJ(cfg);

    // We only need to lookup the object backing to get at the ZTS globals.
#ifdef ZTS
    php_config_backend_object* object;
    object = LOOKUP_OBJECT(php_config_backend_object,thisobj);
#endif

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zname);
    ZVAL_STRING(&fname,"del",1);
    ZVAL_STRING(zname,name,1);

    if (call_user_function(NULL,&thisobj,&fname,&retval,1,&zname
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
                giterr_set_str(GITERR_CONFIG,"php-config-backend: fail del()");
                result = GIT_ERROR;
            }
        }
    }

    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zname);

    return result;
}

/*static*/ int php_config_backend_object::del_multivar(git_config_backend* cfg,
    const char* name,const char* regexp)
{
    int result = GIT_OK;
    zval fname;
    zval retval;
    zval* zname;
    zval* zregexp;
    zval* thisobj = EXTRACT_THISOBJ(cfg);
    zval* params[2];

    // We only need to lookup the object backing to get at the ZTS globals.
#ifdef ZTS
    php_config_backend_object* object;
    object = LOOKUP_OBJECT(php_config_backend_object,thisobj);
#endif

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zname);
    ALLOC_INIT_ZVAL(zregexp);
    ZVAL_STRING(&fname,"del_multivar",1);
    ZVAL_STRING(zname,name,1);
    ZVAL_STRING(zregexp,regexp,1);
    params[0] = zname;
    params[1] = zregexp;

    if (call_user_function(NULL,&thisobj,&fname,&retval,2,params
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
                giterr_set_str(GITERR_CONFIG,"php-config-backend: fail del_multivar()");
                result = GIT_ERROR;
            }
        }
    }

    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zname);
    zval_ptr_dtor(&zregexp);

    return result;
}

/*static*/ int php_config_backend_object::iterator(git_config_iterator** out,
    git_config_backend* cfg)
{
    int result = GIT_OK;
    zval fname;
    zval retval;
    zval* thisobj = EXTRACT_THISOBJ(cfg);

    // We only need to lookup the object backing to get at the ZTS globals.
#ifdef ZTS
    php_config_backend_object* object;
    object = LOOKUP_OBJECT(php_config_backend_object,thisobj);
#endif

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ZVAL_STRING(&fname,"iterator_new",1);

    if (call_user_function(NULL,&thisobj,&fname,&retval,0,NULL
            ZTS_MEMBER_CC(object->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }
    else if (Z_TYPE(retval) == IS_BOOL && !Z_BVAL(retval)) {
        // Let false generate an error. Anything else is the context parameter
        // (including NULL).
        giterr_set_str(GITERR_CONFIG,"php-config-backend: fail iterator()");
        result = GIT_ERROR;
    }
    else {
        custom_backend_iterator* iter;
        iter = (custom_backend_iterator*)emalloc(sizeof(custom_backend_iterator));
        iter->backend = cfg;
        iter->flags = 0;
        iter->next = (int(*)(git_config_entry**,git_config_iterator*))custom_backend_iterator_next;
        iter->free = (void(*)(git_config_iterator*))custom_backend_iterator_free;
        Z_ADDREF_P(thisobj);
        iter->zobject = thisobj;
        if (Z_TYPE(retval) != IS_NULL) {
            MAKE_STD_ZVAL(iter->zcontext);
            ZVAL_COPY_VALUE(iter->zcontext,&retval);
            zval_copy_ctor(iter->zcontext);
        }
        else {
            iter->zcontext = NULL;
        }

        *out = iter;
    }

    return result;
}

/*static*/ int php_config_backend_object::snapshot(git_config_backend** out,
    git_config_backend* cfg)
{
    int result = GIT_OK;
    zval fname;
    zval retval;
    zval* thisobj = EXTRACT_THISOBJ(cfg);
    php_config_backend_object* object = LOOKUP_OBJECT(php_config_backend_object,thisobj);

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ZVAL_STRING(&fname,"snapshot",1);

    // Call userspace method.
    if (call_user_function(NULL,&thisobj,&fname,&retval,0,NULL
            ZTS_MEMBER_CC(object->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }
    else {
        zval* zobj;
        php_config_backend_object* internal;

        // Make sure the function returned an object that extends
        // GitConfigBackend.
        if (Z_TYPE(retval) != IS_OBJECT) {
            convert_to_boolean(&retval);
            if (!Z_BVAL(retval)) {
                giterr_set_str(GITERR_CONFIG,"php-config-backend: fail snapshot()");

                result = GIT_ERROR;
            }
            else {
                php_git2_giterr_set(
                    GITERR_CONFIG,
                    "GitConfigBackend::snapshot(): return value must be object");

                result = GIT_EPHPFATAL;
            }
        }
        else if (!is_subclass_of(Z_OBJCE(retval),class_entry[php_git2_config_backend_obj])) {
            php_git2_giterr_set(
                GITERR_CONFIG,
                "GitConfigBackend::snapshot(): return value object type must extend GitConfigBackend");

            result = GIT_EPHPFATAL;
        }
        else {
            // Create git_config_backend that wraps the object. It will point to
            // the same set of functions. It is up to userspace to enforce any
            // readonly behavior as desired.

            // Copy return value object into new zval.
            ALLOC_ZVAL(zobj);
            ZVAL_COPY_VALUE(zobj,&retval);
            zval_copy_ctor(zobj);

            // Extract backend object and create the custom backend. We must do
            // this in an exception handler since create_custom_backend() is
            // designed to be called from userspace code.
            try {
                internal = LOOKUP_OBJECT(php_config_backend_object,zobj);
                internal->create_custom_backend(zobj,object->owner);
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

    zval_dtor(&fname);
    zval_dtor(&retval);

    return result;
}

/*static*/ int php_config_backend_object::lock(git_config_backend* cfg)
{
    int result = GIT_OK;
    zval fname;
    zval retval;
    zval* thisobj = EXTRACT_THISOBJ(cfg);

    // We only need to lookup the object backing to get at the ZTS globals.
#ifdef ZTS
    php_config_backend_object* object;
    object = LOOKUP_OBJECT(php_config_backend_object,thisobj);
#endif

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ZVAL_STRING(&fname,"lock",1);

    // Call userspace method.
    if (call_user_function(NULL,&thisobj,&fname,&retval,0,NULL
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
                giterr_set_str(GITERR_CONFIG,"php-config-backend: fail lock()");
                result = GIT_ERROR;
            }
        }
    }

    zval_dtor(&fname);
    zval_dtor(&retval);

    return result;
}

/*static*/ int php_config_backend_object::unlock(git_config_backend* cfg,int success)
{
    int result = GIT_OK;
    zval fname;
    zval retval;
    zval* zsuccess;
    zval* thisobj = EXTRACT_THISOBJ(cfg);

    // We only need to lookup the object backing to get at the ZTS globals.
#ifdef ZTS
    php_config_backend_object* object;
    object = LOOKUP_OBJECT(php_config_backend_object,thisobj);
#endif

    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    ALLOC_INIT_ZVAL(zsuccess);
    ZVAL_STRING(&fname,"unlock",1);
    ZVAL_BOOL(zsuccess,success);

    // Call userspace method.
    if (call_user_function(NULL,&thisobj,&fname,&retval,1,&zsuccess
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
                giterr_set_str(GITERR_CONFIG,"php-config-backend: fail unlock()");
                result = GIT_ERROR;
            }
        }
    }

    zval_dtor(&fname);
    zval_dtor(&retval);

    return result;
}

/*static*/ void php_config_backend_object::free(git_config_backend* cfg)
{
    // Set backend to null in internal storage (just in case). Then explicitly
    // call the destructor on the custom backend. Finally free the block of
    // memory that holds the custom backend.

    git_config_backend_php* wrapper = EXTRACT_BACKEND(cfg);
    php_config_backend_object* obj;

    // Make sure object buckets still exist to lookup object (in case the
    // destructor was already called).
    if (EG(objects_store).object_buckets != nullptr) {
        obj = LOOKUP_OBJECT(php_config_backend_object,wrapper->thisobj);
        obj->backend = nullptr;
    }

    wrapper->~git_config_backend_php();
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
