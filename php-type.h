/*
 * php-type.h
 *
 * This file is a part of php-git.
 */

#ifndef PHPGIT2_TYPE_H
#define PHPGIT2_TYPE_H
#include "php-git2.h"
#include "git2-resource.h"
#include <limits>

namespace php_git2
{

    // Provide a specializations of a template type for all PHP primative data
    // types. We always wrap a pointer to a zval. Each specialization will
    // extract the correct value and do type checking.

    class php_value_base
    {
    public:
        php_value_base():
            value(nullptr)
        {
        }

        zval** byref_php()
        { return &value; }

        zval* byval_php() const
        { return value; }
    protected:
        static void error(const char* typeName,unsigned argno)
        {
            if (argno != std::numeric_limits<unsigned>::max()) {
                throw php_git2_exception(
                    "expected '%s' for argument '%d'",
                    typeName,
                    argno);
            }
            throw php_git2_exception(
                "expected '%s' for argument",
                typeName);
        }

        zval* value;
    };

    template<typename T>
    class php_value;

    template<>
    class php_value<long>:
        public php_value_base
    {
    public:
        long byval_git2(unsigned argno = std::numeric_limits<unsigned>::max()) const
        {
            if (Z_TYPE_P(value) == IS_LONG) {
                return Z_LVAL_P(value);
            }
            error("long",argno);
            return long();
        }

        void ret(zval* return_value) const
        {
            RETVAL_LONG(Z_LVAL_P(value));
        }
    };

    template<>
    class php_value<bool>:
        public php_value_base
    {
    public:
        bool byval_git2(unsigned argno = std::numeric_limits<unsigned>::max()) const
        {
            if (Z_TYPE_P(value) == IS_BOOL) {
                return Z_BVAL_P(value);
            }
            error("bool",argno);
            return bool();
        }

        void ret(zval* return_value) const
        {
            RETVAL_LONG(Z_BVAL_P(value));
        }
    };

    template<>
    class php_value<double>:
        public php_value_base
    {
    public:
        double byval_git2(unsigned argno = std::numeric_limits<unsigned>::max()) const
        {
            if (Z_TYPE_P(value) == IS_DOUBLE) {
                return Z_DVAL_P(value);
            }
            error("double",argno);
            return double();
        }

        void ret(zval* return_value) const
        {
            RETVAL_DOUBLE(Z_DVAL_P(value));
        }
    };

    template<>
    class php_value<char*>:
        public php_value_base
    {
    public:
        char* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max()) const
        {
            if (Z_TYPE_P(value) == IS_STRING) {
                return Z_STRVAL_P(value);
            }
            error("string",argno);
            return nullptr;
        }

        void ret(zval* return_value) const
        {
            // This will copy the string buffer (i.e. duplicate it).
            RETVAL_STRING(Z_STRVAL_P(value),1);
        }
    };

    // Provide basic type definitions for the core types.
    using php_bool = php_value<bool>;
    using php_long = php_value<long>;
    using php_double = php_value<double>;
    using php_string = php_value<char*>;

    // Provide generic resource types for libgit2 objects. The parameter should
    // be instantiated with some instantiation of 'git2_resource<>' (or some
    // derived class thereof). We provide one type for when a resource is used
    // as a value (for a created resource) and another when it is used by
    // reference (for an uncreated resource).

    template<typename GitResource>
    class php_resource:
        public php_value_base
    {
    public:
        typename GitResource::git2_type
        byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            GitResource* rsrc;

            // Make sure this is a resource zval.
            if (Z_TYPE_P(value) != IS_RESOURCE) {
                error("resource",argno);
            }

            // Fetch the resource handle.
            rsrc = (GitResource*)zend_fetch_resource(&value,-1,
                GitResource::resource_name(),NULL,1,GitResource::resource_le());
            if (rsrc == nullptr) {
                throw php_git2_exception("resource is invalid");
            }
            return rsrc->get_handle();
        }

        void ret(zval* return_value) const
        {
            RETVAL_RESOURCE(Z_RESVAL_P(value));
        }
    };

    template<typename GitResource>
    class php_resource_ref:
        public php_value_base
    {
    public:
        typename GitResource::git2_type*
        byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            int r;
            GitResource* rsrc;

            // Create the resource if it doesn't exist (which should most likely
            // be the case). Our assumption is that since this value is being
            // used by reference, then it is being assigned. This means we will
            // need to create and register the resource for later assignment and
            // lookup.
            if (value == nullptr) {
                MAKE_STD_ZVAL(value);
                rsrc = php_git2_create_resource<GitResource>();
                r = zend_register_resource(value,
                    rsrc,
                    GitResource::resource_le());
                (void)r;
            }
            else {
                // Fetch the resource handle.
                rsrc = (GitResource*)zend_fetch_resource(&value,-1,
                    GitResource::resource_name(),NULL,1,GitResource::resource_le());
                if (rsrc == nullptr) {
                    throw php_git2_exception("resource is invalid");
                }
            }

            return rsrc->get_handle_byref();
            (void)argno;
        }

        void ret(zval* return_value) const
        {
            RETVAL_RESOURCE(Z_RESVAL_P(value));
        }
    };

    // Provide a type that cleans up the resource before it returns the
    // underlying git2 handle type.

    template<typename GitResource>
    class php_resource_cleanup:
        public php_resource<GitResource>
    {
    public:
        typename GitResource::git2_type
        byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            GitResource* rsrc;
            typename GitResource::git2_type handle;

            // Make sure this is a resource zval.
            if (Z_TYPE_P(value) != IS_RESOURCE) {
                php_value_base::error("resource",argno);
            }

            // Fetch the resource handle.
            rsrc = (GitResource*)zend_fetch_resource(&value,-1,
                GitResource::resource_name(),NULL,1,GitResource::resource_le());
            if (rsrc == nullptr) {
                throw php_git2_exception("resource is invalid");
            }

            // Release the git2 handle from the resource handle; then we cause
            // PHP to destroy the resource zval.
            handle = rsrc->get_handle();
            rsrc->release();
            zend_hash_index_del(&EG(regular_list),Z_RESVAL_P(value));
            value = nullptr;

            return handle;
        }
    protected:
        using php_resource<GitResource>::value;
    };

    // Wrap 'git_strarray' and provide conversions to PHP userspace array. Note
    // that we never accept this type as an argument from userspace. The
    // strarray structure itself is also created on the stack.

    class php_strarray
    {
    public:
        ~php_strarray()
        {
            git_strarray_free(&arr);
        }

        git_strarray* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            return &arr;
        }

        void ret(zval* return_value) const
        {
            // Convert the strarray to a PHP array.
            array_init(return_value);
            for (size_t i = 0;i < arr.count;++i) {
                add_next_index_string(return_value,arr.strings[i],1);
            }
        }
    private:
        git_strarray arr;
    };

    // Enumerate all resource types that we'll care about.
    using php_git_repository = git2_resource<git_repository>;
    using php_git_reference = git2_resource<git_reference>;

} // namespace php_git2

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
