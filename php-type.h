/*
 * php-type.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_TYPE_H
#define PHPGIT2_TYPE_H
#include "php-git2.h"
#include "git2-resource.h"
#include <new>
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

        zval** byref_php(unsigned argno = std::numeric_limits<unsigned>::max())
        { return &value; }

        zval* byval_php(unsigned argno = std::numeric_limits<unsigned>::max()) const
        { return value; }

        static inline void error(const char* typeName,unsigned argno)
        {
            if (argno != std::numeric_limits<unsigned>::max()) {
                php_error(E_ERROR,"expected '%s' for argument position %d",
                    typeName,argno);
            }
            php_error(E_ERROR,"expected '%s' for argument",typeName);
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
        ZTS_CONSTRUCTOR(php_value)

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
        ZTS_CONSTRUCTOR(php_value)

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
        ZTS_CONSTRUCTOR(php_value)

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
        ZTS_CONSTRUCTOR(php_value)

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
        ZTS_CONSTRUCTOR_WITH_BASE(php_nullable_string,php_string)

        char* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max()) const
        {
            if (Z_TYPE_P(value) == IS_NULL) {
                return nullptr;
            }
            return php_string::byval_git2(argno);
        }
    };

    // Provide a string connector that returns the string length.

    template<typename IntType,typename StringType = php_string>
    class php_string_length_connector
    {
    public:
        using connect_t = StringType;
        typedef IntType target_t;

        php_string_length_connector(connect_t&& obj TSRMLS_DC):
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

    // Provide a connector for an arbitrary string buffer that can be returned
    // to PHP userspace. The connector connects to a php_long which represents
    // the desired buffer length.

    class php_string_buffer_connector
    {
    public:
        using connect_t = php_long;
        using target_t = char*;

        php_string_buffer_connector(connect_t&& obj TSRMLS_DC):
            conn(std::forward<connect_t>(obj))
        {
            bufsz = (size_t)conn.byval_git2();
            buffer = (char*)emalloc(bufsz);
        }

        target_t byval_git2(unsigned p)
        {
            return buffer;
        }

        void ret(zval* return_value)
        {
            // Return a string. We'll pass the buffer along to the return_value
            // zval.
            RETVAL_STRINGL(buffer,bufsz,0);
        }
    private:
        char* buffer;
        size_t bufsz;
        connect_t&& conn;
    };

    // Provide a type that casts a php_long to any arbitrary integer type.

    template<typename IntType>
    class php_long_cast:
        public php_long
    {
    public:
        ZTS_CONSTRUCTOR_WITH_BASE(php_long_cast,php_long)

        IntType byval_git2(unsigned argno = std::numeric_limits<unsigned>::max()) const
        {
            return (IntType)php_long::byval_git2(argno);
        }
    };

    // Provide a type that accepts a numeric value from an underlying call.

    template<typename IntType>
    class php_long_indirect
    {
    public:
        ZTS_CONSTRUCTOR(php_long_indirect)

        IntType* byval_git2(unsigned p)
        {
            return &n;
        }

        void ret(zval* return_value)
        {
            RETVAL_LONG((long)n);
        }
    private:
        IntType n;
    };

    // Provide a type for returning a numeric value using an out parameter.

    template<typename IntType>
    class php_long_out:
        public php_value_base
    {
    public:
        ZTS_CONSTRUCTOR(php_long_out)

        ~php_long_out()
        {
            ZVAL_LONG(value,(long)n);
        }

        IntType* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            return &n;
        }
    private:
        IntType n;
    };

    // Provide a class to manage passing copies of ZTS handles around.

    class php_zts_base
    {
#ifdef ZTS
    protected:
        php_zts_base(TSRMLS_D):
            TSRMLS_C(TSRMLS_C)
        {
        }

    public:
        TSRMLS_D;
#endif
    };

    class php_zts_member:
        public php_zts_base
    {
#ifdef ZTS
    public:
        php_zts_member(void*** zts):
            php_zts_base(std::forward<void***>(zts))
        {
        }
#endif
    };

    // Provide generic resource types for libgit2 objects. The parameter should
    // be instantiated with some instantiation of 'git2_resource<>' (or some
    // derived class thereof). We provide one type for when a resource is used
    // as a value (for a created resource) and another when it is used by
    // reference (for an uncreated resource).

    template<typename GitResource>
    class php_resource:
        public php_value_base,
        protected php_zts_base
    {
    public:
        typedef GitResource resource_t;

        php_resource(TSRMLS_D):
            php_zts_base(TSRMLS_C), rsrc(nullptr)
        {
        }

        typename GitResource::git2_type
        byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            lookup(argno);
            return rsrc->get_handle();
        }

        // This member function is used to retrieve the resource object. We must
        // make sure it has been fetched from the resource value.
        GitResource* get_object(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            lookup(argno);
            return rsrc;
        }
    private:
        void lookup(unsigned argno)
        {
            if (rsrc == nullptr) {
                // Make sure this is a resource zval.
                if (Z_TYPE_P(value) != IS_RESOURCE) {
                    error("resource",argno);
                }

                // Fetch the resource handle. Zend will perform error checking
                // on the resource type.
                rsrc = (GitResource*)zend_fetch_resource(&value TSRMLS_CC,-1,
                    GitResource::resource_name(),NULL,1,GitResource::resource_le());
                if (rsrc == nullptr) {
                    throw php_git2_exception("resource is invalid");
                }
            }
        }

        // Store the resource object.
        GitResource* rsrc;
    };

    template<typename GitResource>
    class php_resource_owned:
        public php_resource<GitResource>
    {
    public:
        php_resource_owned(TSRMLS_D):
            php_resource<GitResource>(TSRMLS_C)
        {
        }

        typename GitResource::git2_type
        byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            GitResource* res = php_resource<GitResource>::get_object(argno);

            // Ensure that the underlying handle is owned by the resource before
            // returning it.
            if (!res->is_owned()) {
                throw php_git2_exception("cannot execute git2 call on non-owner resource");
            }

            return res->get_handle();
        }
    };

    template<typename GitResource>
    class php_resource_ref:
        private php_zts_base
    {
    public:
        php_resource_ref(TSRMLS_D):
            php_zts_base(TSRMLS_C), rsrc(nullptr)
        {
        }

        typename GitResource::git2_type*
        byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            // Create a resource backing instance if it does not already exist.
            if (rsrc == nullptr) {
                rsrc = php_git2_create_resource<GitResource>();
            }

            return rsrc->get_handle_byref();
            (void)argno;
        }

        typename GitResource::const_git2_type*
        byval_git2(unsigned argno = std::numeric_limits<unsigned>::max()) const
        {
            // Create a resource backing instance if it does not already exist.
            if (rsrc == nullptr) {
                rsrc = php_git2_create_resource<GitResource>();
            }

            // Cast the resource object to have the same kind of constness to
            // force the compiler to call the correct overloaded member
            // function. We have to do this since it's mutable.
            return const_cast<const GitResource*>(rsrc)->get_handle_byref();
            (void)argno;
        }

        void ret(zval* return_value) const
        {
            // Create a resource zval that uses the GitResource backing.
            zend_register_resource(return_value,rsrc,GitResource::resource_le() TSRMLS_CC);
        }

        GitResource* get_object(unsigned argno = std::numeric_limits<unsigned>::max()) const
        {
            // Retrieve the resource object, creating it if it does not exist.
            if (rsrc == nullptr) {
                rsrc = php_git2_create_resource<GitResource>();
            }

            return rsrc;
        }
    private:
        mutable GitResource* rsrc;
    };

    // Provide a type that represents an optional resource value (one that could
    // be null).

    template<typename GitResource>
    class php_resource_null:
        public php_value_base,
        private php_zts_base
    {
    public:
        php_resource_null(TSRMLS_D):
            php_zts_base(TSRMLS_C)
        {
        }

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
            rsrc = (GitResource*)zend_fetch_resource(&value TSRMLS_CC,-1,
                GitResource::resource_name(),NULL,1,GitResource::resource_le());
            if (rsrc == nullptr) {
                throw php_git2_exception("resource is invalid");
            }
            return rsrc->get_handle();
        }

        GitResource* get_object()
        {
            return nullptr;
        }
    };

    // Provide a type that cleans up the resource before it returns the
    // underlying git2 handle type.

    template<typename GitResource>
    class php_resource_cleanup:
        public php_resource<GitResource>
    {
    public:
        php_resource_cleanup(TSRMLS_D):
            php_resource<GitResource>(TSRMLS_C)
        {
        }

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
            rsrc = (GitResource*)zend_fetch_resource(&value TSRMLS_CC,-1,
                GitResource::resource_name(),NULL,1,GitResource::resource_le());
            if (rsrc == nullptr) {
                throw php_git2_exception("resource is invalid");
            }

            // Delete the PHP resource. This will cause the resource to be
            // invalidated across any zvals that reference it, and the
            // underlying handle will be destroyed (if it has no more
            // references).
            zend_hash_index_del(&EG(regular_list),Z_RESVAL_P(value));
            value = nullptr;

            // The return value should not be used. We do not attempt frees
            // directly from user space.
            return nullptr;
        }
    protected:
        using php_resource<GitResource>::value;
#ifdef ZTS
        using php_resource<GitResource>::TSRMLS_C;
#endif
    };

    class php_git_oid
    {
    public:
        ZTS_CONSTRUCTOR(php_git_oid)

        git_oid* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            return &oid;
        }

        void ret(zval* return_value)
        {
            char buf[GIT_OID_HEXSZ + 1];
            git_oid_tostr(buf,sizeof(buf),&oid);
            RETVAL_STRING(buf,1);
        }
    private:
        git_oid oid;
    };

    class php_git_oid_fromstr:
        public php_value_base
    {
    public:
        ZTS_CONSTRUCTOR(php_git_oid_fromstr)

        git_oid* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            if (Z_TYPE_P(value) != IS_STRING) {
                error("string",argno);
            }

            // Convert PHP string to git_oid.
            convert_oid_fromstr(&oid,Z_STRVAL_P(value),Z_STRLEN_P(value));
            return &oid;
        }
    private:
        git_oid oid;
    };

    // Provide a type for returning an OID value using an out parameter.

    class php_git_oid_out:
        public php_git_oid,
        public php_value_base
    {
    public:
        ZTS_CONSTRUCTOR(php_git_oid_out)

        ~php_git_oid_out()
        {
            ret(value);
        }
    };

    // Wrap 'git_strarray' and provide conversions to PHP userspace array. Note
    // that we never accept this type as an argument from userspace. The
    // strarray structure itself is also created on the stack.

    class php_strarray
    {
    public:
        ZTS_CONSTRUCTOR(php_strarray)

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
        ZTS_CONSTRUCTOR(php_git_buf)

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
            // Convert the git_buf into a PHP string. Make sure to copy the
            // buffer since the destructor will free the git_buf.
            RETVAL_STRINGL(buf.ptr,buf.size,1);
        }
    private:
        git_buf buf;
    };

    // Provide a type for returning a git_buf value using an out parameter.

    class php_git_buf_out:
        public php_git_buf,
        public php_value_base
    {
    public:
        ZTS_CONSTRUCTOR(php_git_buf_out)

        ~php_git_buf_out()
        {
            ret(value);
        }
    };

    // Provide a type that converts PHP arrays into arrays of git2 objects. The
    // git2 array is allocated using the PHP allocator and should only be used
    // in read-only contexts. The memory is designed to persist for the duration
    // of a function call.

    template<typename SourceType,typename ConvertType>
    class php_array:
        public php_value_base,
        private php_zts_base
    {
    public:
        typedef SourceType source_t;
        typedef ConvertType convert_t;

        php_array(TSRMLS_D):
            php_zts_base(TSRMLS_C), data(nullptr)
        {
        }

        ~php_array()
        {
            if (data != nullptr) {
                efree(data);

                // Call the destructor on each source object before freeing the
                // memory.
                for (long i = 0;i < cnt;++i) {
                    sources[i].~SourceType();
                }
                efree(sources);
            }
        }

        ConvertType* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            long i;
            HashPosition pos;
            zval** element;

            // Make sure the zval is an array.
            if (Z_TYPE_P(value) != IS_ARRAY) {
                error("array",argno);
            }
            cnt = zend_hash_num_elements(Z_ARRVAL_P(value));

            // Allocate an array to hold the source objects. We need to call the
            // constructor on each object.
            sources = reinterpret_cast<SourceType*>(emalloc(sizeof(SourceType) * cnt));
            for (i = 0;i < cnt;++i) {
                new(sources + i) SourceType(TSRMLS_C);
            }

            // Create an array to hold the converted values.
            data = reinterpret_cast<ConvertType*>(emalloc(sizeof(ConvertType) * cnt));

            // Walk the array, using the corresponding SourceType object to
            // convert each element.
            i = 0;
            for (zend_hash_internal_pointer_reset_ex(Z_ARRVAL_P(value),&pos);
                 zend_hash_get_current_data_ex(Z_ARRVAL_P(value),(void**)&element,&pos) == SUCCESS;
                 zend_hash_move_forward_ex(Z_ARRVAL_P(value),&pos))
            {
                *sources[i].byref_php() = *element;
                data[i] = sources[i].byval_git2();
                i += 1;
            }

            return data;
        }
    private:
        long cnt;
        SourceType* sources;
        ConvertType* data;
    };

    // Provide an array connector that returns the array length.

    template<typename IntType,typename ArrayType>
    class php_array_length_connector
    {
    public:
        using connect_t = php_array<typename ArrayType::source_t,typename ArrayType::convert_t>;
        typedef IntType target_t;

        php_array_length_connector(connect_t&& obj TSRMLS_DC):
            conn(std::forward<connect_t>(obj))
        {
        }

        target_t byval_git2(unsigned p)
        {
            return zend_hash_num_elements(Z_ARRVAL_P(conn.byval_php(p)));
        }
    private:
        connect_t&& conn;
    };

    // Enumerate common array types.

    template<typename WrapperType>
    using php_resource_array = php_array<
        php_resource<WrapperType>,
        typename WrapperType::const_git2_type>;

    using php_oid_array = php_array<
        php_git_oid_fromstr,
        const git_oid*>;

    // Enumerate all resource types that we'll care about.

    using php_git_repository = git2_resource<git_repository>;
    using php_git_reference = git2_resource<git_reference>;
    using php_git_object = git2_resource<git_object>;
    using php_git_revwalk = git2_resource<git_revwalk>;
    using php_git_packbuilder = git2_resource<git_packbuilder>;
    using php_git_indexer = git2_resource<git_indexer>;
    using php_git_odb = git2_resource<git_odb>;
    using php_git_odb_object = git2_resource<git_odb_object>;
    using php_git_commit = git2_resource<git_commit>;
    using php_git_blob = git2_resource<git_blob>;
    using php_git_tree = git2_resource<git_tree>;
    using php_git_tree_entry = git2_resource<git_tree_entry>;
    using php_git_signature = git2_resource<git_signature>;
    using php_git_treebuilder = git2_resource<git_treebuilder>;
    using php_git_blame = git2_resource<git_blame>;

    // Enumerate nofree versions of certain resource types.

    using php_git_repository_nofree = git2_resource_nofree<git_repository>;
    using php_git_tree_entry_nofree = git2_resource_nofree<git_tree_entry>;
    using php_git_signature_nofree = git2_resource_nofree<git_signature>;

} // namespace php_git2

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
