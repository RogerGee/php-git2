/*
 * php-type.h
 *
 * This file is a part of php-git.
 */

#ifndef PHPGIT2_TYPE_H
#define PHPGIT2_TYPE_H
#include "php-git2.h"
#include "git2-resource.h"

namespace php_git2
{

    // Provide a template for a generic data type.
    template<typename T>
    class php_value
    {
    public:
        php_value():
            value(T())
        {
        }

        // Provide byref() and byval() getters. We have one pair for PHP values
        // and another for libgit2 values. By default these operations are the
        // same.

        T* byref_php()
        { return &value; }
        T* byref_git2()
        { return &value; }

        const T& byval_php()
        { return value; }
        const T& byval_git2() const
        { return value; }
    private:
        T value;
    };

    // Provide a specialization for strings that includes a length.
    template<>
    class php_value<char*>
    {
    public:
        php_value():
            value(nullptr), length(0)
        {
        }

        char** byref_php()
        { return &value; }
        char** byref_git2()
        { return &value; }
        int* byref_length()
        { return &length; }

        const char* byval_php() const
        { return value; }
        const char* byval_git2() const
        { return value; }
    private:
        char* value;
        int length;
    };

    // Provide basic type definitions for the core types.
    using php_long = php_value<long>;
    using php_double = php_value<double>;
    using php_string = php_value<char*>;

    // Provide a generic resource type for libgit2 objects. The parameter should
    // be instantiated with some instantiation of 'git2_resource<>' (or some
    // derived class thereof).
    template<typename GitResource>
    class php_git2_resource
    {
    public:
        php_git2_resource():
            zresource(nullptr)
        {
        }

        zval** byref_php()
        { return &zresource; }
        typename GitResource::git2_type* byref_git2()
        {
            GitResource* rsrc;

            // Fetch the resource handle.
            rsrc = (GitResource*)zend_fetch_resource(&zresource,-1,
                GitResource::resource_name(),NULL,1,GitResource::resource_le());
            if (rsrc == nullptr) {
                throw php_git2_exception("resource is invalid");
            }
            return rsrc->get_handle_byref();
        }

        zval* byval_php() const
        { return zresource; }
        typename GitResource::git2_type byval_git2() const
        {
            GitResource* rsrc;

            // Fetch the resource handle.
            rsrc = (GitResource*)zend_fetch_resource(&zresource,-1,
                GitResource::resource_name(),NULL,1,GitResource::resource_le());
            if (rsrc == nullptr) {
                throw php_git2_exception("resource is invalid");
            }
            return rsrc->get_handle();
        }

        void create()
        {
            int r;

            if (zresource != nullptr) {
                throw php_git2_exception("resource zval was already allocated");
            }
            MAKE_STD_ZVAL(zresource);
            r = zend_register_resource(zresource,
                php_git2_create_resource<GitResource>(),
                GitResource::resource_le());
        }

        void destroy()
        {
            // Force the resource to be cleaned up by Zend.

            zend_hash_index_del(&EG(regular_list),Z_RESVAL_P(zresource));
        }
    private:
        zval* zresource;
    };

} // namespace php_git2

#endif
