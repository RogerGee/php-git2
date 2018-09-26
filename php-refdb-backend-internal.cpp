/*
 * php-refdb-backend-internal.cpp
 *
 * This file is a part of php-git2.
 */

#include "php-object.h"
using namespace php_git2;

// Class entry

static PHP_METHOD(GitRefDBBackend_Internal,exists);
static PHP_METHOD(GitRefDBBackend_Internal,lookup);
static PHP_METHOD(GitRefDBBackend_Internal,iterator_new);
static PHP_METHOD(GitRefDBBackend_Internal,iterator_next);
ZEND_BEGIN_ARG_INFO_EX(GitRefDBBackend_Internal_iterator_next_arginfo,0,0,1)
    ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()
static PHP_METHOD(GitRefDBBackend_Internal,write);
static PHP_METHOD(GitRefDBBackend_Internal,rename);
static PHP_METHOD(GitRefDBBackend_Internal,del);
static PHP_METHOD(GitRefDBBackend_Internal,compress);
static PHP_METHOD(GitRefDBBackend_Internal,has_log);
static PHP_METHOD(GitRefDBBackend_Internal,ensure_log);
static PHP_METHOD(GitRefDBBackend_Internal,reflog_read);
static PHP_METHOD(GitRefDBBackend_Internal,reflog_write);
static PHP_METHOD(GitRefDBBackend_Internal,reflog_rename);
static PHP_METHOD(GitRefDBBackend_Internal,reflog_delete);
static PHP_METHOD(GitRefDBBackend_Internal,lock);
static PHP_METHOD(GitRefDBBackend_Internal,unlock);

zend_function_entry php_git2::refdb_backend_internal_methods[] = {
    PHP_ME(GitRefDBBackend_Internal,exists,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitRefDBBackend_Internal,lookup,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitRefDBBackend_Internal,iterator_new,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitRefDBBackend_Internal,iterator_next,
        GitRefDBBackend_Internal_iterator_next_arginfo,
        ZEND_ACC_PUBLIC)
    PHP_ME(GitRefDBBackend_Internal,write,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitRefDBBackend_Internal,rename,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitRefDBBackend_Internal,del,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitRefDBBackend_Internal,compress,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitRefDBBackend_Internal,has_log,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitRefDBBackend_Internal,ensure_log,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitRefDBBackend_Internal,reflog_read,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitRefDBBackend_Internal,reflog_write,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitRefDBBackend_Internal,reflog_rename,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitRefDBBackend_Internal,reflog_delete,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitRefDBBackend_Internal,lock,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitRefDBBackend_Internal,unlock,NULL,ZEND_ACC_PUBLIC)
    PHP_FE_END
};

// Implementation of php_refdb_backend_internal_object

/*static*/ zend_object_handlers php_refdb_backend_internal_object::handlers;
php_refdb_backend_internal_object::php_refdb_backend_internal_object(zend_class_entry* ce TSRMLS_DC):
    php_refdb_backend_object(ce TSRMLS_CC), iter(nullptr)
{
}

php_refdb_backend_internal_object::~php_refdb_backend_internal_object()
{
    if (iter != nullptr) {
        iter->free(iter);
    }
}

/*static*/ void php_refdb_backend_internal_object::init(zend_class_entry* ce)
{
    handlers.get_constructor = php_git2::not_allowed_get_constructor;
    (void)ce;
}

// Class method implementation.

PHP_METHOD(GitRefDBBackend_Internal,exists)
{
    int result;
    char* refname;
    int refname_len;
    php_refdb_backend_object* object = LOOKUP_OBJECT(php_refdb_backend_object,getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->exists == nullptr) {
        php_error(E_ERROR,"GitRefDBBackend_Internal::exists(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s",&refname,&refname_len) == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {
        int retval;

        retval = object->backend->exists(&result,object->backend,refname);
        if (retval < 0) {
            php_git2::git_error(retval);
        }
    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }

        return;
    }

    RETVAL_BOOL(result);
}

PHP_METHOD(GitRefDBBackend_Internal,lookup)
{
    char* refname;
    int refname_len;
    php_resource_ref<php_git_reference> res;
    php_refdb_backend_object* object = LOOKUP_OBJECT(php_refdb_backend_object,getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->lookup == nullptr) {
        php_error(E_ERROR,"GitRefDBBackend_Internal::lookup(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s",&refname,&refname_len) == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {
        int retval;

        retval = object->backend->lookup(res.byval_git2(),object->backend,refname);
        if (retval < 0) {
            php_git2::git_error(retval);
        }
    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }

        return;
    }

    res.ret(return_value);
}

PHP_METHOD(GitRefDBBackend_Internal,iterator_new)
{
    char* glob;
    int glob_len;
    git_reference_iterator* iter;
    php_refdb_backend_internal_object* object =
        LOOKUP_OBJECT(php_refdb_backend_internal_object,getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->iterator == nullptr) {
        php_error(E_ERROR,"GitRefDBBackend_Internal::iterator_new(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"!s",&glob,&glob_len) == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {
        int retval;

        retval = object->backend->iterator(&iter,object->backend,glob);
        if (retval < 0) {
            php_git2::git_error(retval);
        }
    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }

        return;
    }

    // Free any existing iterator before applying the new one.
    if (object->iter != nullptr) {
        object->iter->free(object->iter);
    }

    object->iter = iter;
}

PHP_METHOD(GitRefDBBackend_Internal,iterator_next)
{
    php_refdb_backend_internal_object* object =
        LOOKUP_OBJECT(php_refdb_backend_internal_object,getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->iterator == nullptr) {
        php_error(E_ERROR,"GitRefDBBackend_Internal::iterator_next(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"") == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {

    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }

        return;
    }

}

PHP_METHOD(GitRefDBBackend_Internal,write)
{
    php_refdb_backend_object* object = LOOKUP_OBJECT(php_refdb_backend_object,getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->write == nullptr) {
        php_error(E_ERROR,"GitRefDBBackend_Internal::write(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"") == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {

    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }

        return;
    }

}

PHP_METHOD(GitRefDBBackend_Internal,rename)
{
    php_refdb_backend_object* object = LOOKUP_OBJECT(php_refdb_backend_object,getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->rename == nullptr) {
        php_error(E_ERROR,"GitRefDBBackend_Internal::rename(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"") == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {

    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }

        return;
    }

}

PHP_METHOD(GitRefDBBackend_Internal,del)
{
    php_refdb_backend_object* object = LOOKUP_OBJECT(php_refdb_backend_object,getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->del == nullptr) {
        php_error(E_ERROR,"GitRefDBBackend_Internal::del(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"") == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {

    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }

        return;
    }

}

PHP_METHOD(GitRefDBBackend_Internal,compress)
{
    php_refdb_backend_object* object = LOOKUP_OBJECT(php_refdb_backend_object,getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->compress == nullptr) {
        php_error(E_ERROR,"GitRefDBBackend_Internal::compress(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"") == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {

    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }

        return;
    }

}

PHP_METHOD(GitRefDBBackend_Internal,has_log)
{
    php_refdb_backend_object* object = LOOKUP_OBJECT(php_refdb_backend_object,getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->has_log == nullptr) {
        php_error(E_ERROR,"GitRefDBBackend_Internal::has_log(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"") == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {

    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }

        return;
    }

}

PHP_METHOD(GitRefDBBackend_Internal,ensure_log)
{
    php_refdb_backend_object* object = LOOKUP_OBJECT(php_refdb_backend_object,getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->ensure_log == nullptr) {
        php_error(E_ERROR,"GitRefDBBackend_Internal::ensure_log(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"") == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {

    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }

        return;
    }

}

PHP_METHOD(GitRefDBBackend_Internal,reflog_read)
{
    php_refdb_backend_object* object = LOOKUP_OBJECT(php_refdb_backend_object,getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->reflog_read == nullptr) {
        php_error(E_ERROR,"GitRefDBBackend_Internal::reflog_read(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"") == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {

    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }

        return;
    }

}

PHP_METHOD(GitRefDBBackend_Internal,reflog_write)
{
    php_refdb_backend_object* object = LOOKUP_OBJECT(php_refdb_backend_object,getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->reflog_write == nullptr) {
        php_error(E_ERROR,"GitRefDBBackend_Internal::reflog_write(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"") == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {

    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }

        return;
    }

}

PHP_METHOD(GitRefDBBackend_Internal,reflog_rename)
{
    php_refdb_backend_object* object = LOOKUP_OBJECT(php_refdb_backend_object,getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->reflog_rename == nullptr) {
        php_error(E_ERROR,"GitRefDBBackend_Internal::reflog_rename(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"") == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {

    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }

        return;
    }

}

PHP_METHOD(GitRefDBBackend_Internal,reflog_delete)
{
    php_refdb_backend_object* object = LOOKUP_OBJECT(php_refdb_backend_object,getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->reflog_delete == nullptr) {
        php_error(E_ERROR,"GitRefDBBackend_Internal::reflog_delete(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"") == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {

    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }

        return;
    }

}

PHP_METHOD(GitRefDBBackend_Internal,lock)
{
    php_error(E_ERROR,"GitRefDBBackend_Internal::lock(): method is inaccessible");
}

PHP_METHOD(GitRefDBBackend_Internal,unlock)
{
    php_error(E_ERROR,"GitRefDBBackend_Internal::unlock(): method is inaccessible");
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
