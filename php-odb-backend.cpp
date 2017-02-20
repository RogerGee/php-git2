/*
 * php-odb-backend.cpp
 *
 * This file is a part of php-git2.
 *
 * This unit provides the out-of-line implementation for the gitODBBackend
 * class.
 */

#include "php-object.h"
using namespace php_git2;

// Class method entries

zend_function_entry php_git2::odb_backend_methods[] = {
    PHP_FE_END
};

// Make function implementation

void php_git2::php_git2_make_odb_backend(zval* zp,git_odb_backend* backend)
{
    php_odb_backend_object* obj;
    zend_class_entry* ce = php_git2::class_entry[php_git2_odb_backend_obj];

    object_init_ex(zp,ce);
    obj = reinterpret_cast<php_odb_backend_object*>(zend_objects_get_address(zp TSRMLS_CC));

    // Assign the git2 odb_backend object.
    obj->backend = backend;

    // Update the version property to the version specified by the backend.
    zend_update_property_long(ce,zp,"version",sizeof("version"),backend->version);
}

// Implementation of php_odb_backend_object

/*static*/ zend_object_handlers php_odb_backend_object::handlers;
php_odb_backend_object::php_odb_backend_object(TSRMLS_D):
    php_zts_base(TSRMLS_C), backend(nullptr), iscustom(false)
{
    zend_class_entry* ce = php_git2::class_entry[php_git2_odb_backend_obj];
    zend_object_std_init(&base,ce TSRMLS_CC);
    object_properties_init(&base,ce);
}

php_odb_backend_object::~php_odb_backend_object()
{
    // AFAIK we never call the free() function on git_odb_backend objects
    // directly.

    zend_object_std_dtor(&base TSRMLS_CC);
}

/* static */ void php_odb_backend_object::init(zend_class_entry* ce)
{
    zend_declare_property_long(ce,"version",sizeof("version"),0,ZEND_ACC_PUBLIC);
}

/*static*/ int php_odb_backend_object::read(void** datap,size_t* sizep,
    git_otype* typep,git_odb_backend* backend,const git_oid* oid)
{

    return GIT_OK;
}

/*static*/ int php_odb_backend_object::read_prefix(git_oid* oidp,void** datap,
    size_t* sizep,git_otype* typep,git_odb_backend* backend,const git_oid* prefix,
    size_t nbits)
{

    return GIT_OK;
}

/*static*/ int php_odb_backend_object::read_header(size_t* sizep,git_otype* typep,
    git_odb_backend* backend,const git_oid* oid)
{

    return GIT_OK;
}

/*static*/ int php_odb_backend_object::write(git_odb_backend* backend,
    const git_oid* oid,const void* data,size_t size,git_otype type)
{

    return GIT_OK;
}

/*static*/ int php_odb_backend_object::writestream(git_odb_stream** streamp,
    git_odb_backend* backend,git_off_t off,git_otype type)
{

    return GIT_OK;
}

/*static*/ int php_odb_backend_object::readstream(git_odb_stream** streamp,
    git_odb_backend* backend,const git_oid* oid)
{

    return GIT_OK;
}

/*static*/ int php_odb_backend_object::exists(git_odb_backend* backend,
    const git_oid* oid)
{

    return GIT_OK;
}

/*static*/ int php_odb_backend_object::exists_prefix(git_oid* oidp,
    git_odb_backend* backend,const git_oid* prefix,size_t nbits)
{

    return GIT_OK;
}

/*static*/ int php_odb_backend_object::refresh(git_odb_backend* backend)
{

    return GIT_OK;
}

/*static*/ int php_odb_backend_object::foreach(git_odb_backend* backend,
    git_odb_foreach_cb cb,void* payload)
{
    return GIT_OK;
}

/*static*/ int php_odb_backend_object::writepack(git_odb_writepack** writepackp,
    git_odb_backend* backend,git_odb* odb,git_transfer_progress_cb progress_cb,
    void* progress_payload)
{
    return GIT_OK;
}

/*static*/ int php_odb_backend_object::freshen(git_odb_backend* backend,
    const git_oid* oid)
{
    return GIT_OK;
}

/*static*/ void php_odb_backend_object::free(git_odb_backend* backend)
{
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
