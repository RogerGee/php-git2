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

// Class method implementation.

PHP_METHOD(GitRefDBBackend_Internal,exists)
{
}

PHP_METHOD(GitRefDBBackend_Internal,lookup)
{
}

PHP_METHOD(GitRefDBBackend_Internal,iterator_new)
{
}

PHP_METHOD(GitRefDBBackend_Internal,iterator_next)
{
}

PHP_METHOD(GitRefDBBackend_Internal,write)
{
}

PHP_METHOD(GitRefDBBackend_Internal,rename)
{
}

PHP_METHOD(GitRefDBBackend_Internal,del)
{
}

PHP_METHOD(GitRefDBBackend_Internal,compress)
{
}

PHP_METHOD(GitRefDBBackend_Internal,has_log)
{
}

PHP_METHOD(GitRefDBBackend_Internal,ensure_log)
{
}

PHP_METHOD(GitRefDBBackend_Internal,reflog_read)
{
}

PHP_METHOD(GitRefDBBackend_Internal,reflog_write)
{
}

PHP_METHOD(GitRefDBBackend_Internal,reflog_rename)
{
}

PHP_METHOD(GitRefDBBackend_Internal,reflog_delete)
{
}

PHP_METHOD(GitRefDBBackend_Internal,lock)
{
}

PHP_METHOD(GitRefDBBackend_Internal,unlock)
{
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
