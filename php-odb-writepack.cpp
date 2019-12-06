/*
 * php-odb-writepack.cpp
 *
 * This file is a part of php-git2.
 *
 * This unit provides the out-of-line implementation for the GitODBWritepack
 * class.
 */

#include "php-object.h"
using namespace php_git2;

// Custom class handlers

static zval* odb_writepack_read_property(zval* obj,zval* prop,int type,const zend_literal* key TSRMLS_DC);
static void odb_writepack_write_property(zval* obj,zval* prop,zval* value,const zend_literal* key TSRMLS_DC);
static int odb_writepack_has_property(zval* obj,zval* prop,int chk_type,const zend_literal* key TSRMLS_DC);

// Class method entries

zend_function_entry php_git2::odb_writepack_methods[] = {
    PHP_ABSTRACT_ME(GitODBWritepack,append,NULL)
    PHP_ABSTRACT_ME(GitODBWritepack,commit,NULL)
    PHP_FE_END
};

// Implementation of php_odb_writepack_object

/*static*/ zend_object_handlers php_odb_writepack_object::handlers;
php_odb_writepack_object::php_odb_writepack_object(zend_class_entry* ce TSRMLS_DC):
    writepack(nullptr), owner(nullptr), zts(TSRMLS_C)
{
    zend_object_std_init(this,ce TSRMLS_CC);
    object_properties_init(this,ce);
}

php_odb_writepack_object::~php_odb_writepack_object()
{
    // Attempt to free the owner resource. This only really frees the owner if
    // its refcount reaches zero.
    if (owner != nullptr) {
        git2_resource_base::free_recursive(owner);
    }

    zend_object_std_dtor(this ZTS_MEMBER_CC(this->zts));
}

void php_odb_writepack_object::create_custom_writepack(zval* zobj,zval* zbackend)
{
    // NOTE: this member function should be called under the php-git2 standard
    // exception handler.

    // Make sure the writepack does not already exist.
    if (writepack != nullptr) {
        throw php_git2_fatal_exception("cannot create custom ODB writepack - object already in use");
    }

    ZTS_MEMBER_EXTRACT(this->zts);

    // Unset any previous 'zbackend' property.
    zend_unset_property(Z_OBJCE_P(zobj),zobj,"backend",sizeof("backend")-1 TSRMLS_CC);

    // Assign 'zbackend' property. This will prevent the parent backend from
    // freeing while the writepack is in use.
    Z_ADDREF_P(zbackend);
    zend_hash_add(Z_OBJPROP_P(zobj),"backend",sizeof("backend"),&zbackend,sizeof(zval*),nullptr);

    // Create new custom writepack.
    writepack = new (emalloc(sizeof(git_odb_writepack_php))) git_odb_writepack_php(zobj TSRMLS_CC);

    // Reset owner (in case it is set). We do not need to maintain a direct
    // owner reference since we are maintaining a reference to the backend in
    // the object's properties hashtable.
    assign_owner(nullptr);
}

void php_odb_writepack_object::assign_owner(php_git_odb* newOwner)
{
    // Free previous owner if any.
    if (owner != nullptr) {
        git2_resource_base::free_recursive(owner);
    }

    owner = newOwner;

    // Increment reference count (if set). This keeps the ODB alive while the
    // writepack is in use.
    if (owner != nullptr) {
        owner->up_ref();
    }
}

/*static*/ void php_odb_writepack_object::init(zend_class_entry* ce)
{
    handlers.read_property = odb_writepack_read_property;
    handlers.write_property = odb_writepack_write_property;
    handlers.has_property = odb_writepack_has_property;

    UNUSED(ce);
}

/*static*/ int php_odb_writepack_object::append(
    git_odb_writepack* writepack,
    const void* data,
    size_t length,
    git_transfer_progress* prog)
{
    int result;
    method_wrapper method("append",writepack);

    zval* zdata;
    zval* zprogress;

    MAKE_STD_ZVAL(zdata);
    MAKE_STD_ZVAL(zprogress);

    ZVAL_STRINGL(zdata,reinterpret_cast<const char*>(data),length,1);
    convert_transfer_progress(zprogress,prog);

    zval* params[] = { zdata, zprogress };

    result = method.call(2,params);
    if (result == GIT_OK) {
        // Write back transfer progress to library structure.
        convert_transfer_progress(*prog,zprogress);
    }

    zval_ptr_dtor(&zdata);
    zval_ptr_dtor(&zprogress);

    return result;
}

/*static*/ int php_odb_writepack_object::commit(
    git_odb_writepack* writepack,
    git_transfer_progress* prog)
{
    int result;
    method_wrapper method("commit",writepack);

    zval* zprogress;

    MAKE_STD_ZVAL(zprogress);

    convert_transfer_progress(zprogress,prog);

    zval* params[] = { zprogress };

    result = method.call(1,params);
    if (result == GIT_OK) {
        // Write back transfer progress to library structure.
        convert_transfer_progress(*prog,zprogress);
    }

    zval_ptr_dtor(&zprogress);

    return result;
}

/*static*/ void php_odb_writepack_object::free(git_odb_writepack* writepack)
{
    method_wrapper::object_wrapper object(writepack);

    // Explicitly call the destructor on the custom writepack. Then free the block
    // of memory that holds the custom writepack.

    object.object()->~git_odb_writepack_php();
    efree(writepack);
}

// Implementation of custom git_odb_writepack

php_odb_writepack_object::
git_odb_writepack_php::git_odb_writepack_php(zval* zv TSRMLS_DC)
{
    // Blank out the base class (which is the structure defined in the git2
    // headers).
    memset(this,0,sizeof(struct git_odb_writepack));

    // Assign zval to keep object alive while backend is in use in the library.
    Z_ADDREF_P(zv);
    thisobj = zv;

    // Assign functions that will invoke the PHP methods in the userspace
    // class. Note that the methods are necessarily overridden since they are
    // abstract in the base class.
    append = php_odb_writepack_object::append;
    commit = php_odb_writepack_object::commit;
    free = php_odb_writepack_object::free;
}

php_odb_writepack_object::
git_odb_writepack_php::~git_odb_writepack_php()
{
    // Free object zval.
    zval_ptr_dtor(&thisobj);
}

// Implementation of custom object handlers

zval* odb_writepack_read_property(zval* obj,zval* prop,int type,const zend_literal* key TSRMLS_DC)
{
    zval* ret;
    zval** zfind;
    zval* tmp_prop = nullptr;
    const char* str;
    php_odb_writepack_object* wrapper = LOOKUP_OBJECT(php_odb_writepack_object,obj);
    git_odb_writepack* writepack = wrapper->writepack;

    // Ensure deep copy of member zval.
    if (Z_TYPE_P(prop) != IS_STRING) {
        MAKE_STD_ZVAL(tmp_prop);
        *tmp_prop = *prop;
        INIT_PZVAL(tmp_prop);
        zval_copy_ctor(tmp_prop);
        convert_to_string(tmp_prop);
        prop = tmp_prop;
        key = NULL;
    }

    // Handle special properties of git_odb_writepack.

    str = Z_STRVAL_P(prop);
    if (strcmp(str,"backend") == 0 && writepack != NULL) {
        if (key != nullptr) {
            ret = zend_hash_quick_find(Z_OBJPROP_P(obj),"backend",sizeof("backend"),key->hash_value,(void**)&zfind) != FAILURE
                ? *zfind : nullptr;
        }
        else {
            ret = zend_hash_find(Z_OBJPROP_P(obj),"backend",sizeof("backend"),(void**)&zfind) != FAILURE
                ? *zfind : nullptr;
        }

        if (ret == nullptr) {
            ALLOC_INIT_ZVAL(ret);

            // NOTE: We can only safely create a backend object if an owner is
            // set on the writepack.

            if (writepack->backend != nullptr && wrapper->owner != nullptr) {
                // Create GitODBBackend object instance to represent the
                // git_odb_backend attached to the writepack. The object does
                // not own the underlying backend object since we pass an owner.
                php_git2_make_odb_backend(ret,writepack->backend,wrapper->owner TSRMLS_CC);

                Z_ADDREF_P(ret);
                if (key != nullptr) {
                    zend_hash_quick_add(Z_OBJPROP_P(obj),"backend",sizeof("backend"),key->hash_value,
                        &ret,sizeof(zval*),NULL);
                }
                else {
                    zend_hash_add(Z_OBJPROP_P(obj),"backend",sizeof("backend"),&ret,sizeof(zval*),NULL);
                }
            }
            Z_DELREF_P(ret);
        }
    }
    else {
        ret = (*std_object_handlers.read_property)(obj,prop,type,key TSRMLS_CC);
    }

    if (tmp_prop != nullptr) {
        Z_ADDREF_P(ret);
        zval_ptr_dtor(&tmp_prop);
        Z_DELREF_P(ret);
    }

    return ret;
}

void odb_writepack_write_property(zval* obj,zval* prop,zval* value,const zend_literal* key TSRMLS_DC)
{
    zval* tmp_prop = nullptr;
    const char* str;

    // Ensure deep copy of member zval.
    if (Z_TYPE_P(prop) != IS_STRING) {
        MAKE_STD_ZVAL(tmp_prop);
        *tmp_prop = *prop;
        INIT_PZVAL(tmp_prop);
        zval_copy_ctor(tmp_prop);
        convert_to_string(tmp_prop);
        prop = tmp_prop;
        key = NULL;
    }

    str = Z_STRVAL_P(prop);
    if (strcmp(str,"backend") == 0) {
        php_error(E_ERROR,"Property '%s' of GitODBWritepack cannot be updated",str);
    }
    else {
        (*std_object_handlers.write_property)(obj,prop,value,key TSRMLS_CC);
    }

    if (tmp_prop != nullptr) {
        zval_ptr_dtor(&tmp_prop);
    }
}

int odb_writepack_has_property(zval* obj,zval* prop,int chk_type,const zend_literal* key TSRMLS_DC)
{
    int result;
    zval* tmp_prop = nullptr;
    const char* src;
    git_odb_writepack* writepack = LOOKUP_OBJECT(php_odb_writepack_object,obj)->writepack;

    // Ensure deep copy of member zval.
    if (Z_TYPE_P(prop) != IS_STRING) {
        MAKE_STD_ZVAL(tmp_prop);
        *tmp_prop = *prop;
        INIT_PZVAL(tmp_prop);
        zval_copy_ctor(tmp_prop);
        convert_to_string(tmp_prop);
        prop = tmp_prop;
        key = NULL;
    }

    src = Z_STRVAL_P(prop);
    if (strcmp(src,"backend") == 0) {
        if (chk_type == 2) {
            result = (writepack != nullptr);
        }
        else {
            result = (writepack != nullptr && writepack->backend != nullptr);
        }
    }
    else {
        result = (*std_object_handlers.has_property)(obj,prop,chk_type,key TSRMLS_CC);
    }

    if (tmp_prop != nullptr) {
        zval_ptr_dtor(&tmp_prop);
    }

    return result;
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
