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
    const git_oid* oid;
    git_reference* ref = nullptr;

    php_refdb_backend_internal_object* object =
        LOOKUP_OBJECT(php_refdb_backend_internal_object,getThis());

    zval* znameout;

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->iterator == nullptr) {
        php_error(E_ERROR,"GitRefDBBackend_Internal::iterator_next(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"z",&znameout) == FAILURE) {
        return;
    }

    if (object->iter == nullptr) {
        RETVAL_FALSE;
    }

    try {
        int err;

        err = object->iter->next(&ref,object->iter);
        if (err < 0) {
            if (err == GIT_ITEROVER) {
                object->iter->free(object->iter);
                object->iter = nullptr;
                RETVAL_FALSE;
            }

            php_git2::git_error(err);
        }

        // Write reference name to out parameter.
        ZVAL_STRING(znameout,git_reference_name(ref),1);

        // Write reference value to the return value.
        oid = git_reference_target(ref);
        if (oid == nullptr) {
            const char* target = git_reference_symbolic_target(ref);
            if (target == nullptr) {
                throw php_git2_exception("GitRefDBBackend_Internal::iterator_next(): next reference is invalid");
            }

            RETVAL_STRING(target,1);
        }
        else {
            convert_oid(return_value,oid);
        }
    } catch (php_git2::php_git2_exception_base& ex) {
        if (ref != nullptr) {
            git_reference_free(ref);
        }

        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }

        return;
    }

    git_reference_free(ref);
}

PHP_METHOD(GitRefDBBackend_Internal,write)
{
    php_refdb_backend_object* object = LOOKUP_OBJECT(php_refdb_backend_object,getThis());

    zval* zref;
    zend_bool force;
    zval* zwho;
    char* message = nullptr;
    int message_len = 0;
    char* old;
    int old_len;
    char* oldtarget;
    int oldtarget_len;

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->write == nullptr) {
        php_error(E_ERROR,"GitRefDBBackend_Internal::write(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"rba!sss",
            &zref,
            &force,
            &zwho,
            &message,
            &message_len,
            &old,
            &old_len,
            &oldtarget,
            &oldtarget_len) == FAILURE)
    {
        return;
    }

    // Call underlying backend function.
    try {
        int retval;
        git_oid oid;
        git_signature* sig;
        php_resource<php_git_reference> ref;

        ref.set_zval(zref);
        sig = convert_signature(zwho);
        if (sig == nullptr) {
            throw php_git2_exception("GitRefDBBackend_Internal::write(): signature array is incorrect");
        }
        convert_oid_fromstr(&oid,old,old_len);

        retval = object->backend->write(
            object->backend,
            ref.byval_git2(),
            force,
            sig,
            message,
            &oid,
            oldtarget);

        git_signature_free(sig);

        if (retval < 0) {
            php_git2::git_error(retval);
        }
    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }

        return;
    }
}

PHP_METHOD(GitRefDBBackend_Internal,rename)
{
    php_resource_ref<php_git_reference> ref;
    php_refdb_backend_object* object = LOOKUP_OBJECT(php_refdb_backend_object,getThis());

    char* oldname;
    int oldname_len;
    char* newname;
    int newname_len;
    zend_bool force;
    zval* zwho;
    char* message = nullptr;
    int message_len = 0;

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->rename == nullptr) {
        php_error(E_ERROR,"GitRefDBBackend_Internal::rename(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"ssba!s",
            &oldname,
            &oldname_len,
            &newname,
            &newname_len,
            &force,
            &zwho,
            &message,
            &message_len) == FAILURE)
    {
        return;
    }

    // Call underlying backend function.
    try {
        int retval;
        git_signature* sig;

        sig = convert_signature(zwho);
        if (sig == nullptr) {
            throw php_git2_exception("GitRefDBBackend_Internal::rename(): signature array is incorrect");
        }

        retval = object->backend->rename(
            ref.byval_git2(),
            object->backend,
            oldname,
            newname,
            force,
            sig,
            message);

        git_signature_free(sig);

        if (retval < 0) {
            php_git2::git_error(retval);
        }
    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }

        return;
    }

    ref.ret(return_value);
}

PHP_METHOD(GitRefDBBackend_Internal,del)
{
    php_refdb_backend_object* object = LOOKUP_OBJECT(php_refdb_backend_object,getThis());

    char* refname;
    int refname_len;
    char* oldid;
    int oldid_len;
    char* oldtarget;
    int oldtarget_len;

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->del == nullptr) {
        php_error(E_ERROR,"GitRefDBBackend_Internal::del(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"sss",
            &refname,
            &refname_len,
            &oldid,
            &oldid_len,
            &oldtarget,
            &oldtarget_len) == FAILURE)
    {
        return;
    }

    // Call underlying backend function.
    try {
        int retval;
        git_oid oid;

        if (convert_oid_fromstr(&oid,oldid,oldid_len) != 0) {
            throw php_git2_exception("GitRefDBBackend_Internal::del(): the provided OID is invalid");
        }

        retval = object->backend->del(object->backend,refname,&oid,oldtarget);
        if (retval < 0) {
            php_git2::git_error(retval);
        }
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

    // Call underlying backend function.
    try {
        int retval;

        retval = object->backend->compress(object->backend);
        if (retval < 0) {
            php_git2::git_error(retval);
        }
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

    char* refname;
    int refname_len;

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s",&refname,&refname_len) == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {
        int retval;

        retval = object->backend->has_log(object->backend,refname);
        if (retval < 0) {
            php_git2::git_error(retval);
        }

        RETVAL_BOOL(retval);
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

    char* refname;
    int refname_len;

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s",&refname,&refname_len) == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {
        int retval;

        retval = object->backend->ensure_log(object->backend,refname);
        if (retval < 0) {
            php_git2::git_error(retval);
        }
    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }

        return;
    }
}

PHP_METHOD(GitRefDBBackend_Internal,reflog_read)
{
    php_git_reflog* reflog;
    php_resource_ref<php_git_reflog> reflogResource;

    php_refdb_backend_object* object = LOOKUP_OBJECT(php_refdb_backend_object,getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->reflog_read == nullptr) {
        php_error(E_ERROR,"GitRefDBBackend_Internal::reflog_read(): method is not available");
        return;
    }

    char* name;
    int name_len;

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s",&name,&name_len) == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {
        int retval;

        retval = object->backend->reflog_read(
            reflogResource.byval_git2(),
            object->backend,
            name);
        if (retval < 0) {
            php_git2::git_error(retval);
        }
    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }

        return;
    }

    reflog = reflogResource.get_object();
    convert_reflog(return_value,reflog->get_handle());
    git2_resource_base::free_recursive(reflog);
}

PHP_METHOD(GitRefDBBackend_Internal,reflog_write)
{
    php_refdb_backend_object* object = LOOKUP_OBJECT(php_refdb_backend_object,getThis());

    zval* zreflog;

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->reflog_write == nullptr) {
        php_error(E_ERROR,"GitRefDBBackend_Internal::reflog_write(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"r",&zreflog) == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {
        int retval;
        php_resource<php_git_reflog> reflog;

        reflog.set_zval(zreflog);

        retval = object->backend->reflog_write(object->backend,reflog.byval_git2());
        if (retval < 0) {
            php_git2::git_error(retval);
        }
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

    char* oldname;
    int oldname_len;
    char* newname;
    int newname_len;

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->reflog_rename == nullptr) {
        php_error(E_ERROR,"GitRefDBBackend_Internal::reflog_rename(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"ss",
            &oldname,
            &oldname_len,
            &newname,
            &newname_len) == FAILURE)
    {
        return;
    }

    // Call underlying backend function.
    try {
        int retval;

        retval = object->backend->reflog_rename(object->backend,oldname,newname);
        if (retval < 0) {
            php_git2::git_error(retval);
        }
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

    char* name;
    int name_len;

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->reflog_delete == nullptr) {
        php_error(E_ERROR,"GitRefDBBackend_Internal::reflog_delete(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s",&name,&name_len) == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {
        int retval;

        retval = object->backend->reflog_delete(object->backend,name);
        if (retval < 0) {
            php_git2::git_error(retval);
        }
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
