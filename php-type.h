/*
 * php-type.h
 *
 * This file is a part of php-git.
 */

#ifndef PHPGIT2_TYPE_H
#define PHPGIT2_TYPE_H
#include "php-git2.h"

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

        T* from_php()
        { return &value; }

        const T& to_git2() const
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

        char** from_php()
        { return &value; }
        int* from_php_length()
        { return &length; }

        const char* to_git2() const
        { return value; }
    private:
        char* value;
        int length;
    };

    // Provide basic type definitions for the core types.
    using php_long = php_value<long>;
    using php_double = php_value<double>;
    using php_string = php_value<char*>;

    // Provide a generic resource type for libgit2 objects.
    template<typename GitResource>
    class php_git2_resource
    {
    public:
        php_git2_resource():
            resource(nullptr)
        {
        }

        zval** from_php()
        { return &resource; }

        typename GitResource::git2_type to_git2() const
        {
            GitResource* rsrc;

            // Fetch the resource handle.
            rsrc = (GitResource*)zend_fetch_resource(&resource,-1,
                GitResource::resource_name(),NULL,1,GitResource::resource_le());
            if (rsrc == nullptr) {
                throw php_git2_exception("resource is invalid");
            }
            return rsrc->get_handle();
        }
    private:
        zval* resource;
    };

} // namespace php_git2

#endif
