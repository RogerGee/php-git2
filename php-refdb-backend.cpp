/*
 * php-refdb-backend.cpp
 *
 * This file is a part of php-git2.
 */

#include "php-object.h"
using namespace php_git2;

// Helper macros.

#define EXTRACT_BACKEND(backend) \
    reinterpret_cast<php_refdb_backend_object::git_refdb_backend_php*>(backend)

#define EXTRACT_THISOBJ(backend) \
    EXTRACT_BACKEND(backend)->thisobj

// Custom class handlers

static zval* refdb_backend_read_property(zval* obj,zval* prop,int type,const zend_literal* key TSRMLS_DC);
static void refdb_backend_write_property(zval* obj,zval* prop,zval* value,const zend_literal* key TSRMLS_DC);
static int refdb_backend_has_property(zval* obj,zval* prop,int chk_type,const zend_literal* key TSRMLS_DC);

// Class entry

zend_function_entry php_git2::refdb_backend_methods[] = {
    PHP_ABSTRACT_ME(GitRefDBBackend,exists,NULL)
    PHP_ABSTRACT_ME(GitRefDBBackend,lookup,NULL)
    PHP_ABSTRACT_ME(GitRefDBBackend,iterator,NULL)
    PHP_ABSTRACT_ME(GitRefDBBackend,write,NULL)
    PHP_ABSTRACT_ME(GitRefDBBackend,rename,NULL)
    PHP_ABSTRACT_ME(GitRefDBBackend,del,NULL)
    PHP_ABSTRACT_ME(GitRefDBBackend,compress,NULL)
    PHP_ABSTRACT_ME(GitRefDBBackend,has_log,NULL)
    PHP_ABSTRACT_ME(GitRefDBBackend,ensure_log,NULL)
    PHP_ABSTRACT_ME(GitRefDBBackend,reflog_read,NULL)
    PHP_ABSTRACT_ME(GitRefDBBackend,reflog_write,NULL)
    PHP_ABSTRACT_ME(GitRefDBBackend,reflog_rename,NULL)
    PHP_ABSTRACT_ME(GitRefDBBackend,reflog_delete,NULL)
    PHP_ABSTRACT_ME(GitRefDBBackend,lock,NULL)
    PHP_ABSTRACT_ME(GitRefDBBackend,unlock,NULL)
    PHP_ABSTRACT_ME(GitRefDBBackend,free,NULL)
    PHP_FE_END
};

// Implementation of custom class handlers

zval* refdb_backend_read_property(zval* obj,zval* prop,int type,const zend_literal* key TSRMLS_DC)
{
    return (*std_object_handlers.read_property)(obj,prop,type,key TSRMLS_CC);
}

void refdb_backend_write_property(zval* obj,zval* prop,zval* value,const zend_literal* key TSRMLS_DC)
{
    (*std_object_handlers.write_property)(obj,prop,value,key TSRMLS_CC);
}

int refdb_backend_has_property(zval* obj,zval* prop,int chk_type,const zend_literal* key TSRMLS_DC)
{
    return (*std_object_handlers.has_property)(obj,prop,chk_type,key TSRMLS_CC);
}

// Implementation of php_refdb_backend_object

php_refdb_backend_object::php_refdb_backend_object(zend_class_entry* ce TSRMLS_DC):
    backend(nullptr), zts(TSRMLS_C)
{
    zend_object_std_init(this,ce TSRMLS_CC);
    object_properties_init(this,ce);
}

php_refdb_backend_object::~php_refdb_backend_object()
{
    zend_object_std_dtor(this ZTS_MEMBER_CC(this->zts));
}

/*static*/ zend_object_handlers php_refdb_backend_object::handlers;
/*static*/ void php_refdb_backend_object::init(zend_class_entry* ce)
{
    handlers.read_property = refdb_backend_read_property;
    handlers.write_property = refdb_backend_write_property;
    handlers.has_property = refdb_backend_has_property;
    (void)ce;
}

/*static*/ int php_refdb_backend_object::exists(
    int *exists,
    git_refdb_backend *backend,
    const char *ref_name)
{
    return GIT_ERROR;
}

/*static*/ int php_refdb_backend_object::lookup(
    git_reference **out,
    git_refdb_backend *backend,
    const char *ref_name)
{
    return GIT_ERROR;
}

/*static*/ int php_refdb_backend_object::iterator(
    git_reference_iterator **iter,
    struct git_refdb_backend *backend,
    const char *glob)
{
    return GIT_ERROR;
}

/*static*/ int php_refdb_backend_object::write(git_refdb_backend *backend,
    const git_reference *ref,int force,
    const git_signature *who,const char *message,
    const git_oid *old,const char *old_target)
{
    return GIT_ERROR;
}

/*static*/ int php_refdb_backend_object::rename(
    git_reference **out,git_refdb_backend *backend,
    const char *old_name,const char *new_name,int force,
    const git_signature *who,const char *message)
{
    return GIT_ERROR;
}

/*static*/ int php_refdb_backend_object::del(
    git_refdb_backend *backend,
    const char *ref_name,
    const git_oid *old_id,
    const char *old_target)
{
    return GIT_ERROR;
}

/*static*/ int php_refdb_backend_object::compress(git_refdb_backend *backend)
{
    return GIT_ERROR;
}

/*static*/ int php_refdb_backend_object::has_log(git_refdb_backend *backend,const char *refname)
{
    return GIT_ERROR;
}

/*static*/ int php_refdb_backend_object::ensure_log(git_refdb_backend *backend,const char *refname)
{
    return GIT_ERROR;
}

/*static*/ void php_refdb_backend_object::free(git_refdb_backend *backend)
{
}

/*static*/ int php_refdb_backend_object::reflog_read(git_reflog **out,
    git_refdb_backend *backend,
    const char *name)
{
    return GIT_ERROR;
}

/*static*/ int php_refdb_backend_object::reflog_write(git_refdb_backend *backend,git_reflog *reflog)
{
    return GIT_ERROR;
}

/*static*/ int php_refdb_backend_object::reflog_rename(git_refdb_backend *_backend,
    const char *old_name,
    const char *new_name)
{
    return GIT_ERROR;
}

/*static*/ int php_refdb_backend_object::reflog_delete(git_refdb_backend *backend,
    const char *name)
{
    return GIT_ERROR;
}

/*static*/ int php_refdb_backend_object::lock(void **payload_out,
    git_refdb_backend *backend,
    const char *refname)
{
    return GIT_ERROR;
}

/*static*/ int php_refdb_backend_object::unlock(git_refdb_backend *backend,
    void *payload,
    int success,
    int update_reflog,
    const git_reference *ref,
    const git_signature *sig,
    const char *message)
{
    return GIT_ERROR;
}

// Implementation of php_refdb_backend_object::git_refdb_backend_php

php_refdb_backend_object::git_refdb_backend_php::git_refdb_backend_php(zval* zv)
{
}

php_refdb_backend_object::git_refdb_backend_php::~git_refdb_backend_php()
{
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
