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

        zval** byref_php(unsigned)
        { return &value; }

        zval* byval_php(unsigned) const
        { return value; }

        static inline void error(const char* typeName,unsigned argno)
        {
            if (argno != std::numeric_limits<unsigned>::max()) {
                throw php_git2_exception(
                    "expected '%s' for argument position %d",
                    typeName,
                    argno);
            }
            throw php_git2_exception(
                "expected '%s' for argument",
                typeName);
        }
    protected:
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
            if (Z_TYPE_P(value) != IS_LONG) {
                error("long",argno);
            }
            return Z_LVAL_P(value);
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
            if (Z_TYPE_P(value) != IS_BOOL) {
                error("bool",argno);
            }
            return Z_BVAL_P(value);
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
            if (Z_TYPE_P(value) != IS_DOUBLE) {
                error("double",argno);
            }
            return Z_DVAL_P(value);
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
            if (Z_TYPE_P(value) != IS_STRING) {
                error("string",argno);
            }
            return Z_STRVAL_P(value);
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

    // Provide a nullable string type.
    class php_nullable_string:
        public php_string
    {
    public:
        char* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max()) const
        {
            if (Z_TYPE_P(value) == IS_NULL) {
                return nullptr;
            }
            return php_string::byval_git2(argno);
        }
    };

    // Provide a string connector that returns the string length.
    template<typename IntType>
    class php_string_length_connector
    {
    public:
        using connect_t = php_string;
        typedef IntType target_t;

        php_string_length_connector(connect_t&& obj):
            conn(std::forward<connect_t>(obj))
        {
        }

        target_t byval_git2(unsigned p)
        {
            return Z_STRLEN_P(conn.byval_php(p));
        }
    private:
        connect_t&& conn;
    };

    // Provide a type that casts a php_long to any arbitrary integer type
    template<typename IntType>
    class php_long_cast:
        public php_long
    {
    public:
        IntType byval_git2(unsigned argno = std::numeric_limits<unsigned>::max()) const
        {
            return (IntType)php_long::byval_git2(argno);
        }
    };

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
        php_resource():
            rsrc(nullptr)
        {
        }

        typename GitResource::git2_type
        byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            lookup(argno);
            return rsrc->get_handle();
        }

        void ret(zval* return_value) const
        {
            RETVAL_RESOURCE(Z_RESVAL_P(value));
        }

        // This member function is used to retrieve the resource object. We must
        // make sure it has been fetched from the resource value.
        GitResource* get_object(unsigned argno)
        {
            lookup(argno);
            return rsrc;
        }
    private:
        // Store the resource object.
        GitResource* rsrc;

        void lookup(unsigned argno)
        {
            if (rsrc == nullptr) {
                // Make sure this is a resource zval.
                if (Z_TYPE_P(value) != IS_RESOURCE) {
                    error("resource",argno);
                }

                // Fetch the resource handle. Zend will perform error checking
                // on the resource type.
                rsrc = (GitResource*)zend_fetch_resource(&value,-1,
                    GitResource::resource_name(),NULL,1,GitResource::resource_le());
                if (rsrc == nullptr) {
                    throw php_git2_exception("resource is invalid");
                }
            }
        }
    };

    template<typename GitResource>
    class php_resource_ref:
        public php_value_base
    {
    public:
        php_resource_ref():
            rsrc(nullptr)
        {
        }

        typename GitResource::git2_type*
        byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            lookup(argno);
            return rsrc->get_handle_byref();
            (void)argno;
        }

        void ret(zval* return_value) const
        {
            RETVAL_RESOURCE(Z_RESVAL_P(value));
        }

        // This member function is used to retrieve the resource object. We must
        // make sure it has been created.
        GitResource* get_object(unsigned argno)
        {
            lookup(argno);
            return rsrc;
        }
    private:
        GitResource* rsrc;

        void lookup(unsigned argno)
        {
            int r;
            if (rsrc != nullptr) {
                return;
            }

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
                // NOTE: in practice this shouldn't happen, since this type is
                // used to create new resources.

                // Make sure this is a resource zval.
                if (Z_TYPE_P(value) != IS_RESOURCE) {
                    error("resource",argno);
                }

                // Fetch the resource handle. Zend will perform error checking
                // on the resource type.
                rsrc = (GitResource*)zend_fetch_resource(&value,-1,
                    GitResource::resource_name(),NULL,1,GitResource::resource_le());
                if (rsrc == nullptr) {
                    throw php_git2_exception("resource is invalid");
                }
            }

        }
    };

    // Provide a type that represents an optional resource value (one that could
    // be null).

    template<typename GitResource>
    class php_resource_null:
        public php_value_base
    {
    public:
        typename GitResource::git2_type
        byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            GitResource* rsrc;

            // Resource may be null.
            if (Z_TYPE_P(value) == IS_NULL) {
                return nullptr;
            }

            // Make sure this is a resource zval.
            if (Z_TYPE_P(value) != IS_RESOURCE) {
                error("resource or null",argno);
            }

            // Fetch the resource handle. Zend will perform error checking on
            // the resource type.
            rsrc = (GitResource*)zend_fetch_resource(&value,-1,
                GitResource::resource_name(),NULL,1,GitResource::resource_le());
            if (rsrc == nullptr) {
                throw php_git2_exception("resource is invalid");
            }
            return rsrc->get_handle();
        }

        void ret(zval* return_value) const
        {
            if (Z_TYPE_P(value) == IS_NULL) {
                RETVAL_NULL();
            }
            else {
                RETVAL_RESOURCE(Z_RESVAL_P(value));
            }
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

    class php_git_oid
    {
    public:
        git_oid* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            return &oid;
        }

        void ret(zval* return_value)
        {
            char buf[GIT_OID_HEXSZ + 1];
            git_oid_tostr(buf,sizeof(buf),&oid);
            RETVAL_STRING(buf,strlen(buf));
        }
    private:
        git_oid oid;
    };

    class php_git_oid_fromstr:
        public php_value_base
    {
    public:
        git_oid* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            if (Z_TYPE_P(value) != IS_STRING) {
                error("string",argno);
            }
            // Convert PHP string to git_oid.
            git_oid_fromstr(&oid,Z_STRVAL_P(value));
            return &oid;
        }
    private:
        git_oid oid;
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

    // Wrap 'git_buf' and make it to convert to a PHP string.

    class php_git_buf
    {
    public:
        ~php_git_buf()
        {
            git_buf_free(&buf);
        }

        git_buf* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            return &buf;
        }

        void ret(zval* return_value) const
        {
            // Convert the git_buf into a PHP string.
            RETVAL_STRINGL(buf.ptr,buf.size,1);
        }
    private:
        git_buf buf;
    };

    // Enumerate all resource types that we'll care about.
    using php_git_repository = git2_resource<git_repository>;
    using php_git_reference = git2_resource<git_reference>;
    using php_git_object = git2_resource<git_object>;
    using php_git_revwalk = git2_resource<git_revwalk>;
    using php_git_packbuilder = git2_resource<git_packbuilder>;
    using php_git_indexer = git2_resource<git_indexer>;
    using php_git_odb = git2_resource<git_odb>;

} // namespace php_git2

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
