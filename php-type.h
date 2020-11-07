/*
 * php-type.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_TYPE_H
#define PHPGIT2_TYPE_H
#include "php-array.h"
#include "php-resource.h"
#include <new>

namespace php_git2
{
    // Provide abstract base class for value that can be parsed from a parameter
    // zval.

    class php_parameter
    {
    public:
        void parse(zval* zvp,int argno)
        {
            // NOTE: Parse assumes 'value' is empty.
            parse_impl(zvp,argno);
        }

        void parse_with_context(zval* zvp,const char* ctx);

    private:
        virtual void parse_impl(zval* zvp,int argno) = 0;
    };

    // Provide abstract base class for managing PHP values. The purpose of this
    // class is to provide a read-only view of the zval. The class does NOT
    // manage the lifecycle of the wrapped zval.

    class php_value_base:
        public php_parameter
    {
    public:
        php_value_base()
        {
            ZVAL_UNDEF(&value);
        }

        zval* get_value()
        {
            return &value;
        }

        void set_value(zval* zvp)
        {
            ZVAL_COPY_VALUE(&value,zvp);
        }

        bool is_null() const
        {
            return Z_TYPE(value) == IS_NULL;
        }

    protected:
        zval value;
    };

    class php_value_generic:
        public php_value_base
    {
    private:
        virtual void parse_impl(zval* zvp,int argno);
    };

    template<typename T>
    class php_value;

    template<>
    class php_value<long>:
        public php_value_base
    {
    public:
        long byval_git2() const
        {
            return static_cast<long>(Z_LVAL(value));
        }

        void ret(zval* return_value) const
        {
            RETVAL_LONG(Z_LVAL(value));
        }

    private:
        virtual void parse_impl(zval* zvp,int argno)
        {
            int result;
            zend_long dummy;

            result = zend_parse_parameter(ZEND_PARSE_PARAMS_THROW,argno,zvp,"l",&dummy);
            if (result == FAILURE) {
                throw php_git2_propagated_exception();
            }

            ZVAL_COPY_VALUE(&value,zvp);
        }
    };

    template<>
    class php_value<bool>:
        public php_value_base
    {
    public:
        bool byval_git2() const
        {
            return Z_LVAL(value) ? true : false;
        }

        void ret(zval* return_value) const
        {
            if (Z_LVAL(value)) {
                RETURN_TRUE
            }

            RETURN_FALSE
        }

    private:
        virtual void parse_impl(zval* zvp,int argno)
        {
            int result;
            zend_bool dummy;

            result = zend_parse_parameter(ZEND_PARSE_PARAMS_THROW,argno,zvp,"b",&dummy);
            if (result == FAILURE) {
                throw php_git2_propagated_exception();
            }

            ZVAL_COPY_VALUE(&value,zvp);
        }
    };

    template<>
    class php_value<double>:
        public php_value_base
    {
    public:
        double byval_git2() const
        {
            return Z_DVAL(value);
        }

        void ret(zval* return_value) const
        {
            RETVAL_DOUBLE(Z_DVAL(value));
        }

    private:
        virtual void parse_impl(zval* zvp,int argno)
        {
            int result;
            double dummy;

            result = zend_parse_parameter(ZEND_PARSE_PARAMS_THROW,argno,zvp,"d",&dummy);
            if (result == FAILURE) {
                throw php_git2_propagated_exception();
            }

            ZVAL_COPY_VALUE(&value,zvp);
        }
    };

    template<>
    class php_value<const char*>:
        virtual public php_value_base
    {
    public:
        const char* byval_git2() const
        {
            return Z_STRVAL(value);
        }

        void ret(zval* return_value) const
        {
            // This will copy the string buffer (i.e. duplicate it).
            RETVAL_STRING(Z_STRVAL(value));
        }

    protected:
        virtual void parse_impl(zval* zvp,int argno)
        {
            char* s;
            size_t l;
            int result;

            result = zend_parse_parameter(ZEND_PARSE_PARAMS_THROW,argno,zvp,"s",&s,&l);
            if (result == FAILURE) {
                throw php_git2_propagated_exception();
            }

            ZVAL_COPY_VALUE(&value,zvp);
        }
    };

    // Provide basic type definitions for the core types.

    using php_bool = php_value<bool>;
    using php_long = php_value<long>;
    using php_double = php_value<double>;
    using php_string = php_value<const char*>;

    // Provide a nullable string type.

    class php_string_nullable:
        virtual public php_string
    {
    public:
        ZTS_CONSTRUCTOR_WITH_BASE(php_string_nullable,php_string)

        const char* byval_git2() const
        {
            if (is_null()) {
                return nullptr;
            }

            return php_string::byval_git2();
        }

    protected:
        virtual void parse_impl(zval* zvp,int argno)
        {
            if (Z_TYPE_P(zvp) == IS_NULL) {
                ZVAL_NULL(&value);
                return;
            }

            php_string::parse_impl(zvp,argno);
        }
    };

    // Provide a returnable string type that is set by reference.

    class php_string_ref
    {
    public:
        php_string_ref(TSRMLS_D):
            ptr(nullptr)
        {
        }

        const char** byval_git2()
        {
            return &ptr;
        }

        void ret(zval* return_value)
        {
            if (ptr != NULL) {
                RETVAL_STRING(ptr);
                return;
            }

            ZVAL_NULL(return_value);
        }

    private:
        const char* ptr;
    };

    // Provide a string type that can be returned through an out parameter.

    class php_string_out:
        public php_string
    {
    public:
        php_string_out(TSRMLS_D):
            ptr(nullptr)
        {
        }

        ~php_string_out()
        {
            if (ptr == nullptr) {
                ZVAL_NULL(&value);
            }
            else {
                ZVAL_STRING(&value,ptr);
            }
        }

        const char** byval_git2()
        {
            return &ptr;
        }

    private:
        const char* ptr;
    };

    // Provide a string connector that returns the string length.

    template<typename IntType,typename StringType = php_string>
    class php_string_length_connector
    {
    public:
        using connect_t = StringType;
        typedef IntType target_t;

        php_string_length_connector(connect_t& obj TSRMLS_DC):
            conn(obj)
        {
        }

        target_t byval_git2()
        {
            return static_cast<IntType>(Z_STRLEN_P(conn.get_value()));
        }

    protected:
        connect_t& conn;
    };

    template<typename IntType,typename StringType = php_string_nullable>
    class php_string_length_connector_null:
        public php_string_length_connector<IntType,StringType>
    {
        using base_type = php_string_length_connector<IntType,StringType>;
    public:
        using typename base_type::connect_t;
        using typename base_type::target_t;

        php_string_length_connector_null(connect_t& obj TSRMLS_DC):
            base_type(obj TSRMLS_CC)
        {
        }

        target_t byval_git2()
        {
            zval* zvp = base_type::conn.get_value();

            if (Z_TYPE_P(zvp) != IS_STRING) {
                return 0;
            }

            return static_cast<IntType>(Z_STRLEN_P(zvp));
        }
    };

    // Provide a connector for an arbitrary string buffer that can be returned
    // to PHP userspace. The connector connects to a php_long which represents
    // the desired buffer length.

    class php_string_buffer_connector
    {
    public:
        using connect_t = php_long;
        using target_t = char*;

        php_string_buffer_connector(connect_t& obj TSRMLS_DC)
        {
            bufsz = static_cast<size_t>(obj.byval_git2());
            buffer = static_cast<char*>(emalloc(bufsz));
        }

        ~php_string_buffer_connector()
        {
            efree(buffer);
        }

        target_t byval_git2()
        {
            return buffer;
        }

        void ret(zval* return_value)
        {
            RETVAL_STRINGL(buffer,bufsz);
        }

    private:
        char* buffer;
        size_t bufsz;
    };

    // Provide a type that casts a php_long to any arbitrary integer type.

    template<typename IntType>
    class php_long_cast:
        public php_long
    {
    public:
        ZTS_CONSTRUCTOR_WITH_BASE(php_long_cast,php_long)

        IntType byval_git2() const
        {
            return static_cast<IntType>(php_long::byval_git2());
        }
    };

    // Provide a type that accepts a numeric value from an underlying call.

    template<typename IntType>
    class php_long_ref
    {
    public:
        ZTS_CONSTRUCTOR(php_long_ref)

        IntType* byval_git2()
        {
            return &n;
        }

        void ret(zval* return_value)
        {
            RETVAL_LONG(static_cast<zend_long>(n));
        }

        IntType get_value() const
        {
            return n;
        }

    private:
        IntType n;
    };

    // Provide a type like php_long_ref that returns bool.

    template<typename IntType>
    class php_bool_ref:
        public php_long_ref<IntType>
    {
    public:
        ZTS_CONSTRUCTOR_WITH_BASE(php_bool_ref,php_long_ref<IntType>)

        void ret(zval* return_value)
        {
            if (get_value()) {
                RETURN_TRUE
            }

            RETURN_FALSE
        }

    protected:
        using php_long_ref<IntType>::get_value;
    };

    // Provide a type for returning a numeric value using an out parameter.

    template<typename IntType>
    class php_long_out:
        public php_long
    {
    public:
        ZTS_CONSTRUCTOR(php_long_out)

        ~php_long_out()
        {
            ZVAL_LONG(&value,static_cast<zend_long>(n));
        }

        IntType* byval_git2()
        {
            return &n;
        }

    private:
        IntType n;
    };

    // Provide generic resource types for libgit2 objects. The parameter should
    // be instantiated with some instantiation of 'git2_resource<>' (or some
    // derived class thereof). We provide one type for when a resource is used
    // as a value (for a created resource) and another when it is used by
    // reference (for an uncreated resource).

    class php_resource_base:
        public php_value_base
    {
    private:
        virtual void parse_impl(zval* zvp,int argno);
    };

    template<typename GitResource>
    class php_resource:
        public php_resource_base,
        protected php_zts_base
    {
    public:
        typedef GitResource resource_t;

        php_resource(TSRMLS_D):
            php_zts_base(TSRMLS_C)
        {
        }

        typename GitResource::git2_type byval_git2()
        {
            return get_object()->get_handle();
        }

        // This member function is used to retrieve the resource object. We must
        // make sure it has been fetched from the resource value.
        GitResource* get_object()
        {
            return reinterpret_cast<GitResource*>(Z_RES_VAL(value));
        }

    private:
        virtual void parse_impl(zval* zvp,int argno)
        {
            void* result;
            php_resource_base::parse_impl(zvp,argno);
            result = zend_fetch_resource(Z_RES(value),
                GitResource::resource_name(),
                GitResource::resource_le());

            if (result == nullptr) {
                throw php_git2_propagated_exception();
            }
        }
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

        typename GitResource::git2_type byval_git2()
        {
            GitResource* res = php_resource<GitResource>::get_object();

            // Ensure that the underlying handle is owned by the resource before
            // returning it.
            if (!res->is_owned()) {
                throw php_git2_exception("Cannot execute git2 call on non-owner resource");
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

        typename GitResource::git2_type* byval_git2()
        {
            // Create a resource backing instance if it does not already exist.
            if (rsrc == nullptr) {
                rsrc = php_git2_create_resource<GitResource>();
            }

            return rsrc->get_handle_byref();
        }

        typename GitResource::const_git2_type* byval_git2() const
        {
            // Create a resource backing instance if it does not already exist.
            if (rsrc == nullptr) {
                rsrc = php_git2_create_resource<GitResource>();
            }

            // Cast the resource object to have the same kind of constness to
            // force the compiler to call the correct overloaded member
            // function. We have to do this since it's mutable.
            return const_cast<const GitResource*>(rsrc)->get_handle_byref();
        }

        void ret(zval* return_value) const
        {
            zend_resource* zr;

            zr = zend_register_resource(rsrc,GitResource::resource_le());
            RETVAL_RES(zr);
        }

        GitResource* get_object() const
        {
            // Retrieve the resource object, creating it if it does not exist.
            if (rsrc == nullptr) {
                rsrc = php_git2_create_resource<GitResource>();
            }

            return rsrc;
        }

        void set_object(typename GitResource::git2_type obj)
        {
            // Create a resource backing instance if it does not already exist.
            if (rsrc == nullptr) {
                rsrc = php_git2_create_resource<GitResource>();
            }

            rsrc->set_handle(obj);
        }

        void set_object(typename GitResource::const_git2_type obj) const
        {
            // Create a resource backing instance if it does not already exist.
            if (rsrc == nullptr) {
                rsrc = php_git2_create_resource<GitResource>();
            }

            rsrc->set_handle(const_cast<typename GitResource::git2_type>(obj));
        }

    private:
        mutable GitResource* rsrc;
    };

    // Provide a variant of php_resource_ref that can return null if the output
    // value was NULL.

    template<typename GitResource>
    class php_resource_nullable_ref:
        private php_zts_base
    {
    public:
        php_resource_nullable_ref(TSRMLS_D):
            php_zts_base(TSRMLS_C), rsrc(nullptr), handle(nullptr)
        {
        }

        typename GitResource::git2_type*
        byval_git2()
        {
            // Just return the handle. Delay creating the resource backing until
            // later and after we know the return value is not NULL.
            return &handle;
        }

        void ret(zval* return_value)
        {
            // If the handle was non-NULL, create a resource backing. Then pass
            // it off to a new resource zval.
            if (handle != nullptr) {
                if (rsrc == nullptr) {
                    rsrc = php_git2_create_resource<GitResource>();
                    rsrc->set_handle(handle);
                }
                zend_register_resource(return_value,rsrc,GitResource::resource_le() TSRMLS_CC);
            }
            else {
                ZVAL_NULL(return_value);
            }
        }

        GitResource* get_object()
        {
            // Return the resource backing if the handle was non-NULL. Otherwise
            // return NULL.

            if (handle != nullptr) {
                if (rsrc == nullptr) {
                    rsrc = php_git2_create_resource<GitResource>();
                    rsrc->set_handle(handle);
                }

                return rsrc;
            }

            return nullptr;
        }

    private:
        GitResource* rsrc;
        typename GitResource::git2_type handle;
    };

    // Provide out parameter variants of php_resource_ref and friends. These
    // allow new resources to be returned through output parameters.

    template<typename GitResource>
    class php_resource_ref_out:
        public php_resource<GitResource>,
        public php_resource_ref<GitResource>
    {
    public:
        ZTS_CONSTRUCTOR_WITH_BASE(php_resource_ref_out,php_resource_ref<GitResource>)

        ~php_resource_ref_out()
        {
            php_resource_ref<GitResource>::ret(value);
        }

        zval* get_value() const
        {
            return const_cast<zval*>(&value);
        }

    protected:
        using php_resource<GitResource>::value;
    };

    template<typename GitResource>
    class php_resource_nullable_ref_out:
        public php_resource<GitResource>,
        public php_resource_nullable_ref<GitResource>
    {
    public:
        ZTS_CONSTRUCTOR_WITH_BASE(php_resource_nullable_ref_out,
            php_resource_nullable_ref<GitResource>)

        ~php_resource_nullable_ref_out()
        {
            php_resource_nullable_ref<GitResource>::ret(value);
        }

    protected:
        using php_resource<GitResource>::value;
    };

    // Provide a type that represents an optional resource value (one that could
    // be null).

    template<typename GitResource>
    class php_resource_nullable:
        public php_resource<GitResource>
    {
        using base = php_resource<GitResource>;
    public:
        typename GitResource::git2_type byval_git2()
        {
            // Resource may be null.
            if (base::is_null()) {
                return nullptr;
            }

            return base::byval_git2();
        }

        GitResource* get_object()
        {
            if (base::is_null()) {
                return nullptr;
            }

            return base::get_object();
        }

    protected:
        using base::value;

    private:
        virtual void parse_impl(zval* zvp,int argno)
        {
            if (Z_TYPE_P(zvp) == IS_NULL) {
                ZVAL_NULL(&value);
                return;
            }

            base::parse_impl(zvp,argno);
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

        typename GitResource::git2_type byval_git2()
        {
            // Delete the PHP resource. This will cause the resource to be
            // invalidated across any zvals that reference it and the underlying
            // handle will be destroyed (if it has no more references).
            zend_list_close(Z_RES(value));

            // The return value should not be used. We do not attempt frees
            // directly from user space.
            return nullptr;
        }

    protected:
        using php_resource<GitResource>::value;
    };

    template<typename GitResource>
    class php_resource_cleanup_delayed:
        public php_resource<GitResource>
    {
    public:
        php_resource_cleanup_delayed(TSRMLS_D):
            php_resource<GitResource>(TSRMLS_C)
        {
        }

        ~php_resource_cleanup_delayed()
        {
            // Delete the PHP resource. This will cause the resource to be
            // invalidated across any zvals that reference it and the underlying
            // handle will be destroyed (if it has no more references).
            zend_list_close(Z_RES(value));
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
        git_oid* byval_git2()
        {
            return &oid;
        }

        void ret(zval* return_value)
        {
            char buf[GIT_OID_HEXSZ + 1];
            git_oid_tostr(buf,sizeof(buf),&oid);
            RETVAL_STRING(buf);
        }

    private:
        git_oid oid;
    };

    class php_git_oid_fromstr:
        virtual public php_string
    {
    public:
        ZTS_CONSTRUCTOR(php_git_oid_fromstr)

        git_oid* byval_git2()
        {
            // Convert PHP string to git_oid.
            convert_oid_fromstr(&oid,Z_STRVAL(value),Z_STRLEN(value));
            return &oid;
        }

    private:
        git_oid oid;
    };

    class php_git_oid_fromstr_nullable:
        virtual public php_string_nullable,
        virtual public php_git_oid_fromstr
    {
    public:
        ZTS_CONSTRUCTOR(php_git_oid_fromstr_nullable)

        using php_git_oid_fromstr::byval_git2;

    private:
        git_oid oid;

        using php_string_nullable::parse_impl;
    };

    class php_git_oid_byval_fromstr:
        public php_git_oid_fromstr
    {
    public:
        ZTS_CONSTRUCTOR_WITH_BASE(php_git_oid_byval_fromstr,php_git_oid_fromstr)

        git_oid byval_git2()
        {
            return *php_git_oid_fromstr::byval_git2();
        }
    };

    // Provide a type for returning an OID value using an out parameter.

    class php_git_oid_out:
        public php_git_oid,
        public php_string
    {
    public:
        ZTS_CONSTRUCTOR_WITH_BASE(php_git_oid_out,php_git_oid)

        ~php_git_oid_out()
        {
            php_string::ret(&value);
        }
    };

    // Wrap 'git_strarray' and provide conversions to PHP userspace array. Note
    // that we never accept this type as an argument from userspace. The
    // strarray structure itself is also created on the stack.

    class php_strarray
    {
    public:
        php_strarray(TSRMLS_D)
        {
            memset(&arr,0,sizeof(git_strarray));
        }

        ~php_strarray()
        {
            git_strarray_free(&arr);
        }

        git_strarray* byval_git2()
        {
            return &arr;
        }

        void ret(zval* return_value) const
        {
            // Convert the strarray to a PHP array.
            array_init(return_value);
            for (size_t i = 0;i < arr.count;++i) {
                add_next_index_string(return_value,arr.strings[i]);
            }
        }

    private:
        git_strarray arr;
    };

    // Wrap 'git_oidarray' and provide conversions to PHP userspace array. Note
    // that we never accept this type from userspace.

    class php_oidarray
    {
    public:
        php_oidarray(TSRMLS_D)
        {
            memset(&arr,0,sizeof(git_oidarray));
        }

        ~php_oidarray()
        {
            git_oidarray_free(&arr);
        }

        git_oidarray* byval_git2()
        {
            return &arr;
        }

        void ret(zval* return_value) const
        {
            array_init(return_value);
            for (size_t i = 0;i < arr.count;++i) {
                char buf[GIT_OID_HEXSZ + 1];
                git_oid_tostr(buf,sizeof(buf),arr.ids + i);
                add_next_index_string(return_value,buf);
            }
        }

    private:
        git_oidarray arr;
    };

    // Wrap 'git_buf' and make it to convert to a PHP string.

    class php_git_buf
    {
    public:
        php_git_buf(TSRMLS_D)
        {
            memset(&buf,0,sizeof(git_buf));
        }

        ~php_git_buf()
        {
            git_buf_free(&buf);
        }

        git_buf* byval_git2()
        {
            return &buf;
        }

        void ret(zval* return_value) const
        {
            // Convert the git_buf into a PHP string. Make sure to copy the
            // buffer since the destructor will free the git_buf.
            RETVAL_STRINGL(buf.ptr,buf.size);
        }

    private:
        git_buf buf;
    };

    // Provide a type for returning a git_buf value using an out parameter.

    class php_git_buf_out:
        public php_git_buf,
        public php_string
    {
    public:
        ZTS_CONSTRUCTOR_WITH_BASE(php_git_buf_out,php_git_buf)

        ~php_git_buf_out()
        {
            php_string::ret(&value);
        }
    };

    // Provide a fixed-length buffer type.

    template<unsigned MaxLength>
    class php_fixed_buffer
    {
    public:
        ZTS_CONSTRUCTOR(php_fixed_buffer)

        char* byval_git2()
        {
            return buffer;
        }

        void ret(zval* return_value)
        {
            RETVAL_STRING(buffer);
        }

    private:
        char buffer[MaxLength];
    };

    // Provide a type that delivers a constant value to git2.

    template<typename ConstantType,ConstantType Value>
    class php_constant
    {
    public:
        ZTS_CONSTRUCTOR(php_constant)

        ConstantType byval_git2()
        {
            return Value;
        }
    };

    // Provide a base type for PHP arrays.

    class php_array_base:
        public php_value_base
    {
    private:
        virtual void parse_impl(zval* zvp,int argno);
    };

    // Provide a base type for option arrays. (This is really just a nullable
    // array base type that is commonly used for option arrays.)

    class php_option_array:
        public php_array_base
    {
    private:
        virtual void parse_impl(zval* zvp,int argno);
    };

    // Provide a type that converts PHP arrays into arrays of git2 objects. The
    // implementation supports arrays of a single type. The git2 array is
    // allocated using the PHP allocator and should only be used in read-only
    // contexts. The memory is designed to persist for the duration of a
    // function call. The SourceType should be some php_value_base derivation.

    template<typename SourceType,typename ConvertType>
    class php_array:
        public php_array_base,
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

        ConvertType* byval_git2()
        {
            size_t i;
            HashTable* ht;
            HashPosition pos;
            zval* element;

            ht = Z_ARRVAL(value);
            cnt = zend_hash_num_elements(ht);

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
            for (zend_hash_internal_pointer_reset_ex(ht,&pos);
                 (element = zend_hash_get_current_data_ex(ht,&pos)) != nullptr;
                 zend_hash_move_forward_ex(ht,&pos))
            {
                sources[i].set_value(element);
                data[i] = sources[i].byval_git2();
                i += 1;
            }

            return data;
        }

        long get_count() const
        {
            return cnt;
        }

    private:
        uint32_t cnt;
        SourceType* sources;
        ConvertType* data;
    };

    // Provide an array connector that returns the array length.

    template<typename IntType,typename ArrayType>
    class php_array_length_connector
    {
    public:
        using connect_t = php_array<
            typename ArrayType::source_t,
            typename ArrayType::convert_t
            >;
        typedef IntType target_t;

        php_array_length_connector(connect_t& obj TSRMLS_DC):
            conn(obj)
        {
        }

        target_t byval_git2()
        {
            zval* zvp = conn.get_value();
            if (Z_TYPE_P(zvp) != IS_ARRAY) {
                return IntType();
            }

            return zend_hash_num_elements(Z_ARRVAL_P(conn.get_value()));
        }

    private:
        connect_t& conn;
    };

    // Enumerate common array types.

    template<typename WrapperType>
    using php_resource_array = php_array<
        php_resource<WrapperType>,
        typename WrapperType::const_git2_type
        >;

    using php_oid_array = php_array<
        php_git_oid_fromstr,
        const git_oid*
        >;

    using php_oid_byval_array = php_array<
        php_git_oid_byval_fromstr,
        git_oid
        >;

    using php_string_array = php_array<
        php_string,
        const char*
        >;

    class php_strarray_array:
        public php_string_array
    {
    public:
        php_strarray_array(TSRMLS_D):
            php_string_array(TSRMLS_C)
        {
            memset(&arr,0,sizeof(git_strarray));
        }

        ~php_strarray_array()
        {
            if (arr.strings != nullptr) {
                for (size_t i = 0;i < arr.count;++i) {
                    efree(arr.strings[i]);
                }
                efree(arr.strings);
            }
        }

        git_strarray* byval_git2()
        {
            const char** lines = php_string_array::byval_git2();

            // Copy lines into git_strarray structure.
            arr.count = get_count();
            arr.strings = reinterpret_cast<char**>(emalloc(sizeof(char*) * arr.count));
            for (size_t i = 0;i < arr.count;++i) {
                arr.strings[i] = estrdup(lines[i]);
            }

            return &arr;
        }

    private:
        git_strarray arr;
    };

    class php_strarray_array_nullable:
        public php_strarray_array
    {
    public:
        ZTS_CONSTRUCTOR_WITH_BASE(php_strarray_array_nullable,php_strarray_array);

        git_strarray* byval_git2()
        {
            if (is_null()) {
                return nullptr;
            }

            return php_strarray_array::byval_git2();
        }
    };

    class php_strarray_byval_array:
        public php_strarray_array
    {
    public:
        ZTS_CONSTRUCTOR_WITH_BASE(php_strarray_byval_array,php_strarray_array)

        git_strarray byval_git2()
        {
            // Return structure by value. This is still a shallow copy of the
            // data.
            return *php_strarray_array::byval_git2();
        }
    };

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
    using php_git_annotated_commit = git2_resource<git_annotated_commit>;
    using php_git_branch_iterator = git2_resource<git_branch_iterator>;
    using php_git_reference_iterator = git2_resource<git_reference_iterator>;
    using php_git_config = git2_resource<git_config>;
    using php_git_config_iterator = git2_resource<git_config_iterator>;
    using php_git_tag = git2_resource<git_tag>;
    using php_git_diff = git2_resource<git_diff>;
    using php_git_diff_stats = git2_resource<git_diff_stats>;
    using php_git_index = git2_resource<git_index>;
    using php_git_index_conflict_iterator = git2_resource<git_index_conflict_iterator>;
    using php_git_status_list = git2_resource<git_status_list>;
    using php_git_note = git2_resource<git_note>;
    using php_git_note_iterator = git2_resource<git_note_iterator>;
    using php_git_reflog = git2_resource<git_reflog>;
    using php_git_refdb = git2_resource<git_refdb>;
    using php_git_patch = git2_resource<git_patch>;
    using php_git_describe_result = git2_resource<git_describe_result>;
    using php_git_rebase = git2_resource<git_rebase>;
    using php_git_remote = git2_resource<git_remote>;
    using php_git_refspec = git2_resource<const git_refspec>;
    using php_git_cred = git2_resource<git_cred>;
    using php_git_submodule = git2_resource<git_submodule>;

    // Enumerate nofree alternatives of certain resource types.

    using php_git_repository_nofree = git2_resource_nofree<git_repository>;
    using php_git_reference_nofree = git2_resource_nofree<git_reference>;
    using php_git_tree_entry_nofree = git2_resource_nofree<git_tree_entry>;
    using php_git_signature_nofree = git2_resource_nofree<git_signature>;
    using php_git_odb_nofree = git2_resource_nofree<git_odb>;
    using php_git_diff_nofree = git2_resource_nofree<git_diff>;
    using php_git_reflog_nofree = git2_resource_nofree<git_reflog>;

} // namespace php_git2

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
