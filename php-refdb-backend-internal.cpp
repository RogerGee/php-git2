/*
 * php-refdb-backend-internal.cpp
 *
 * Copyright (C) Roger P. Gee
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

// Make function implementation

void php_git2::php_git2_make_refdb_backend(zval* zp,git_refdb_backend* backend,php_git_refdb* owner)
{
    php_refdb_backend_internal_object* obj;

    // Create object zval.
    object_init_ex(zp,php_git2::class_entry[php_git2_refdb_backend_internal_obj]);
    obj = php_zend_object<php_refdb_backend_internal_object>::get_storage(zp);

    // Assign the backend.
    obj->backend = backend;

    // Assign owner. (If set, this increments owner refcount to prevent the
    // refdb from freeing while the backend object is in use.)
    obj->assign_owner(owner);

    // Set kind depending on if owner was provided.
    if (owner != nullptr) {
        obj->kind = php_refdb_backend_object::conventional;
    }
    else {
        obj->kind = php_refdb_backend_object::user;
    }
}

// php_zend_object init function

template<>
void php_zend_object<php_refdb_backend_internal_object>::init(zend_class_entry* ce)
{
    auto& parentHandlers = php_zend_object<php_refdb_backend_object>::handlers;

    handlers.read_property = parentHandlers.read_property;
    handlers.write_property = parentHandlers.write_property;
    handlers.has_property = parentHandlers.has_property;
    handlers.get_constructor = php_git2::not_allowed_get_constructor;

    handlers.offset = offset();

    UNUSED(ce);
}

// Implementation of php_refdb_backend_internal_object

php_refdb_backend_internal_object::php_refdb_backend_internal_object():
    iter(nullptr)
{
}

php_refdb_backend_internal_object::~php_refdb_backend_internal_object()
{
    if (iter != nullptr) {
        iter->free(iter);
    }
}

// Class method implementation.

PHP_METHOD(GitRefDBBackend_Internal,exists)
{
    php_bailer bailer;
    php_refdb_backend_object* object;

    char* refname;
    size_t refname_len;

    object = php_zend_object<php_refdb_backend_internal_object>::get_storage(getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->exists == nullptr) {
        zend_throw_error(nullptr,"GitRefDBBackend_Internal::exists(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS(),"s",&refname,&refname_len) == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {
        int retval;
        int result;

        retval = object->backend->exists(&result,object->backend,refname);
        if (retval < 0) {
            php_git2::git_error(retval);
        }
        else {
            RETVAL_BOOL(result);
        }

    } catch (php_git2::php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle();
        }
    }
}

PHP_METHOD(GitRefDBBackend_Internal,lookup)
{
    php_bailer bailer;
    php_refdb_backend_internal_object* object;

    char* refname;
    size_t refname_len;

    object = php_zend_object<php_refdb_backend_internal_object>::get_storage(getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->lookup == nullptr) {
        zend_throw_error(nullptr,"GitRefDBBackend_Internal::lookup(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS(),"s",&refname,&refname_len) == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {
        int retval;
        php_resource_ref<php_git_reference> res;

        retval = object->backend->lookup(res.byval_git2(),object->backend,refname);
        if (retval < 0) {
            php_git2::git_error(retval);
        }
        else {
            res.ret(return_value);
        }

    } catch (php_git2::php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle();
        }
    }
}

PHP_METHOD(GitRefDBBackend_Internal,iterator_new)
{
    php_bailer bailer;
    php_refdb_backend_internal_object* object;

    char* glob;
    size_t glob_len;

    object = php_zend_object<php_refdb_backend_internal_object>::get_storage(getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->iterator == nullptr) {
        zend_throw_error(
            nullptr,
            "GitRefDBBackend_Internal::iterator_new(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS(),"!s",&glob,&glob_len) == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {
        int retval;
        git_reference_iterator* iter;

        retval = object->backend->iterator(&iter,object->backend,glob);
        if (retval < 0) {
            php_git2::git_error(retval);
        }
        else {
            // Free any existing iterator before applying the new one.
            if (object->iter != nullptr) {
                object->iter->free(object->iter);
            }

            object->iter = iter;
        }

    } catch (php_git2::php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle();
        }
    }
}

PHP_METHOD(GitRefDBBackend_Internal,iterator_next)
{
    php_bailer bailer;
    php_refdb_backend_internal_object* object;

    zval* znameout;

    object = php_zend_object<php_refdb_backend_internal_object>::get_storage(getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->iterator == nullptr) {
        zend_throw_error(
            nullptr,
            "GitRefDBBackend_Internal::iterator_next(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS(),"z/",&znameout) == FAILURE) {
        return;
    }

    if (object->iter == nullptr) {
        RETVAL_FALSE;
    }

    try {
        int err;
        const git_oid* oid;
        git_reference* ref = nullptr;

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
        ZVAL_STRING(znameout,git_reference_name(ref));

        // Write reference value to the return value.
        oid = git_reference_target(ref);
        if (oid == nullptr) {
            const char* target = git_reference_symbolic_target(ref);
            if (target == nullptr) {
                git_reference_free(ref);
                throw php_git2_error_exception(
                    "GitRefDBBackend_Internal::iterator_next(): next reference is invalid");
            }

            RETVAL_STRING(target);
        }
        else {
            convert_oid(return_value,oid);
        }

        git_reference_free(ref);

    } catch (php_git2::php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle();
        }
    }
}

PHP_METHOD(GitRefDBBackend_Internal,write)
{
    php_bailer bailer;
    php_refdb_backend_object* object;

    zval* zref;
    zend_bool force;
    zval* zwho;
    char* message = nullptr;
    size_t message_len = 0;
    char* old;
    size_t old_len;
    char* oldtarget;
    size_t oldtarget_len;

    object = php_zend_object<php_refdb_backend_internal_object>::get_storage(getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->write == nullptr) {
        zend_throw_error(nullptr,"GitRefDBBackend_Internal::write(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS(),
            "rba!sss",
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

        ref.set_value(zref);
        sig = convert_signature(zwho);
        if (sig == nullptr) {
            throw php_git2_error_exception(
                "GitRefDBBackend_Internal::write(): signature array is incorrect");
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
        php_bailout_context ctx(bailer);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle();
        }
    }
}

PHP_METHOD(GitRefDBBackend_Internal,rename)
{
    php_bailer bailer;
    php_refdb_backend_object* object;

    char* oldname;
    size_t oldname_len;
    char* newname;
    size_t newname_len;
    zend_bool force;
    zval* zwho;
    char* message = nullptr;
    size_t message_len = 0;

    object = php_zend_object<php_refdb_backend_internal_object>::get_storage(getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->rename == nullptr) {
        zend_throw_error(nullptr,"GitRefDBBackend_Internal::rename(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS(),
            "ssba!s",
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
        php_resource_ref<php_git_reference> ref;

        sig = convert_signature(zwho);
        if (sig == nullptr) {
            throw php_git2_exception(
                "GitRefDBBackend_Internal::rename(): signature array is incorrect");
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
        else {
            ref.ret(return_value);
        }

    } catch (php_git2::php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle();
        }
    }
}

PHP_METHOD(GitRefDBBackend_Internal,del)
{
    php_bailer bailer;
    php_refdb_backend_object* object;

    char* refname;
    size_t refname_len;
    char* oldid;
    size_t oldid_len;
    char* oldtarget;
    size_t oldtarget_len;

    object = php_zend_object<php_refdb_backend_internal_object>::get_storage(getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->del == nullptr) {
        zend_throw_error(nullptr,"GitRefDBBackend_Internal::del(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS(),
            "sss",
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
        php_bailout_context ctx(bailer);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle();
        }
    }
}

PHP_METHOD(GitRefDBBackend_Internal,compress)
{
    php_bailer bailer;
    php_refdb_backend_object* object;

    object = php_zend_object<php_refdb_backend_internal_object>::get_storage(getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->compress == nullptr) {
        zend_throw_error(
            nullptr,
            "GitRefDBBackend_Internal::compress(): method is not available");
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
        php_bailout_context ctx(bailer);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle();
        }
    }
}

PHP_METHOD(GitRefDBBackend_Internal,has_log)
{
    php_bailer bailer;
    php_refdb_backend_object* object;

    object = php_zend_object<php_refdb_backend_internal_object>::get_storage(getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->has_log == nullptr) {
        zend_throw_error(
            nullptr,
            "GitRefDBBackend_Internal::has_log(): method is not available");
        return;
    }

    char* refname;
    size_t refname_len;

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS(),"s",&refname,&refname_len) == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {
        int retval;

        retval = object->backend->has_log(object->backend,refname);
        if (retval < 0) {
            php_git2::git_error(retval);
        }
        else {
            RETVAL_BOOL(retval);
        }

    } catch (php_git2::php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle();
        }
    }
}

PHP_METHOD(GitRefDBBackend_Internal,ensure_log)
{
    php_bailer bailer;
    php_refdb_backend_object* object;

    object = php_zend_object<php_refdb_backend_internal_object>::get_storage(getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->ensure_log == nullptr) {
        zend_throw_error(
            nullptr,
            "GitRefDBBackend_Internal::ensure_log(): method is not available");
        return;
    }

    char* refname;
    size_t refname_len;

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS(),"s",&refname,&refname_len) == FAILURE) {
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
        php_bailout_context ctx(bailer);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle();
        }
    }
}

PHP_METHOD(GitRefDBBackend_Internal,reflog_read)
{
    php_bailer bailer;
    php_refdb_backend_object* object;

    object = php_zend_object<php_refdb_backend_internal_object>::get_storage(getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->reflog_read == nullptr) {
        zend_throw_error(
            nullptr,
            "GitRefDBBackend_Internal::reflog_read(): method is not available");
        return;
    }

    char* name;
    size_t name_len;

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS(),"s",&name,&name_len) == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {
        int retval;
        php_git_reflog* reflog;
        php_resource_ref<php_git_reflog> reflogResource;

        retval = object->backend->reflog_read(
            reflogResource.byval_git2(),
            object->backend,
            name);
        if (retval < 0) {
            php_git2::git_error(retval);
        }
        else {
            reflog = reflogResource.get_object();
            convert_reflog(return_value,reflog->get_handle());
            git2_resource_base::free_recursive(reflog);
        }

    } catch (php_git2::php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle();
        }
    }
}

PHP_METHOD(GitRefDBBackend_Internal,reflog_write)
{
    php_bailer bailer;
    php_refdb_backend_object* object;

    zval* zreflog;

    object = php_zend_object<php_refdb_backend_internal_object>::get_storage(getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->reflog_write == nullptr) {
        zend_throw_error(
            nullptr,
            "GitRefDBBackend_Internal::reflog_write(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS(),"r",&zreflog) == FAILURE) {
        return;
    }

    // Call underlying backend function.
    try {
        int retval;
        php_resource<php_git_reflog> reflog;

        reflog.set_value(zreflog);

        retval = object->backend->reflog_write(object->backend,reflog.byval_git2());
        if (retval < 0) {
            php_git2::git_error(retval);
        }

    } catch (php_git2::php_git2_exception_base& ex) {
        php_bailout_context ctx(bailer);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle();
        }
    }
}

PHP_METHOD(GitRefDBBackend_Internal,reflog_rename)
{
    php_bailer bailer;
    php_refdb_backend_object* object;

    char* oldname;
    size_t oldname_len;
    char* newname;
    size_t newname_len;

    object = php_zend_object<php_refdb_backend_internal_object>::get_storage(getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->reflog_rename == nullptr) {
        zend_throw_error(
            nullptr,
            "GitRefDBBackend_Internal::reflog_rename(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS(),
            "ss",
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
        php_bailout_context ctx(bailer);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle();
        }
    }
}

PHP_METHOD(GitRefDBBackend_Internal,reflog_delete)
{
    php_bailer bailer;
    php_refdb_backend_object* object;

    char* name;
    size_t name_len;

    object = php_zend_object<php_refdb_backend_internal_object>::get_storage(getThis());

    // Verfiy the backend exists and the function is implemented.
    if (object->backend == nullptr || object->backend->reflog_delete == nullptr) {
        zend_throw_error(
            nullptr,
            "GitRefDBBackend_Internal::reflog_delete(): method is not available");
        return;
    }

    // Parse method parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS(),"s",&name,&name_len) == FAILURE) {
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
        php_bailout_context ctx(bailer);

        if (BAILOUT_ENTER_REGION(ctx)) {
            ex.handle();
        }
    }
}

PHP_METHOD(GitRefDBBackend_Internal,lock)
{
    zend_throw_error(nullptr,"GitRefDBBackend_Internal::lock(): method is inaccessible");
}

PHP_METHOD(GitRefDBBackend_Internal,unlock)
{
    zend_throw_error(nullptr,"GitRefDBBackend_Internal::unlock(): method is inaccessible");
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
