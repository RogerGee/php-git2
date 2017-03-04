/*
 * php-callback.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_PHP_CALLBACK_H
#define PHPGIT2_PHP_CALLBACK_H
#include "php-type.h"

namespace php_git2
{

    // Provide a type that contains an array of zvals converted from arbitrary
    // git2 values.

    template<unsigned Count>
    class zval_array:
        private php_zts_base
    {
    public:
        zval_array(TSRMLS_D):
            php_zts_base(TSRMLS_C)
        {
            for (unsigned i = 0;i < Count;++i) {
                ALLOC_INIT_ZVAL(params[i]);
            }
        }

        // Create a member function for assigning to the zvals from git2
        // values. We create partial specializations for the kinds of values we
        // are interested in.

        template<unsigned I>
        void assign()
        {
            // The base case does nothing.
        }

        template<unsigned I,typename... Ts>
        void assign(long&& h,Ts&&... ts)
        {
            ZVAL_LONG(params[I],h);
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        template<unsigned I,typename... Ts>
        void assign(double&& h,Ts&&... ts)
        {
            ZVAL_DOUBLE(params[I],h);
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        template<unsigned I,typename... Ts>
        void assign(const char*&& h,Ts&&... ts)
        {
            ZVAL_STRING(params[I],h,1);
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        template<unsigned I,typename... Ts>
        void assign(const void*&& a,size_t&& b,Ts&&... ts)
        {
            ZVAL_STRINGL(params[I],(const char*)a,b,1);
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        template<unsigned I,typename... Ts>
        void assign(zval*&& h,Ts&&... ts)
        {
            if (h != nullptr) {
                zval_dtor(params[I]);
                params[I] = h;
            }
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        void call(zval* func,zval* ret)
        {
            int r;

            // We allow null callables, which do nothing.
            if (Z_TYPE_P(func) == IS_NULL) {
                return;
            }

            ZVAL_NULL(ret);
            r = call_user_function(EG(function_table),NULL,func,ret,Count,params TSRMLS_CC);
            if (r == FAILURE) {
                throw php_git2_exception("unable to call userspace function: "
                    "expected a callable");
            }
        }

        zval* operator [](unsigned index)
        { return params[index]; }
        const zval* operator [](unsigned index) const
        { return params[index]; }
    private:
        zval* params[Count];
    };

    /**
     * Synchronous Callbacks
     *
     * A synchronous callback is one that is executed by libgit2 within a single
     * library call. As such, we can allocate the callback data on the
     * stack. The object holds two zvals: one to represent the PHP userspace
     * function (i.e. a callable) and another to represent any payload data. The
     * object always expects the calls to byref_php() to have position arguments
     * such that the function zval is first. Payload data may be omitted if
     * desired. The object tracks the position arguments since the standard does
     * not define the order of parameter evaluation.
     */

    struct php_callback_sync:
        public php_zts_base
    {
    public:
        php_callback_sync(TSRMLS_D):
            php_zts_base(TSRMLS_C), func(nullptr), data(nullptr),
            p(std::numeric_limits<unsigned>::max())
        {
        }

        ~php_callback_sync()
        {
            // Delete our references to the variables. We always assume that
            // byval_git2() ran successfully.
            Z_DELREF_P(func);
            Z_DELREF_P(data);
        }

        zval** byref_php(unsigned pos)
        {
            // If 'p' is max_value then we haven't had a call to this function
            // yet and we store the parameter position. Otherwise we compare the
            // existing parameter position with the new one. We want 'p' set to
            // the value zero if the parameters need to be swapped such that the
            // function zval is first.

            if (p == std::numeric_limits<unsigned>::max()) {
                p = pos;
                return &func;
            }

            p = (p < pos);
            return &data;
        }

        void* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            // Figure out which parameters are which and swap as needed.
            if (!p) {
                std::swap(func,data);
            }

            // Make sure the function zval is a callable. We always allow the
            // user to omit a callable if the zval is null.
            if (Z_TYPE_P(func) != IS_NULL) {
                char* error = nullptr;
                zend_bool retval;
                retval = zend_is_callable_ex(func,NULL,0,NULL,NULL,NULL,&error TSRMLS_CC);
                if (error) {
                    efree(error);
                }
                if (!retval) {
                    php_value_base::error("callable",argno);
                }
            }

            // Up reference count. This is really only needed for when this
            // class is wrapped by php_callback_async so that the PHP values
            // will exist between function calls.
            Z_ADDREF_P(func);
            Z_ADDREF_P(data);
            return this;
        }

        zval* func;
        zval* data;
    private:
        unsigned p;
    };

    /**
     * Asynchronous Callbacks
     *
     * An asynchronous callback is one that is allocated dynamically on the
     * per-request memory heap. We implement this by allocating a
     * php_callback_sync instance dynamically. This means that the async
     * callback object (which is allocated on the stack) goes away while the
     * sync callback object persists. To avoid leaks during the lifetime of the
     * request, the async object is also a connector type so users can attach an
     * asynchronous callback object to a resource or object. In this way the
     * lifetime of the callback is the same as the lifetime of the
     * resource/object.
     */

    template<typename GitResource>
    class php_callback_async
    {
    public:
        // Connect to an arbitrary resource type. It must be a newly created
        // resource (i.e. resource ref).
        using connect_t = php_resource_ref<GitResource>;
        typedef void* target_t;

        php_callback_async(connect_t&& conn TSRMLS_DC)
        {
            // Allocate php_callback_sync object.
            cb = new (emalloc(sizeof(php_callback_sync))) php_callback_sync(TSRMLS_C);

            // Assign php_callback_sync object to resource object. It must have
            // a member called 'cb' to which it has access.
            GitResource* rsrc = conn.get_object(
                std::numeric_limits<unsigned>::max());
            rsrc->cb = cb;
        }

        zval** byref_php(unsigned pos)
        {
            return cb->byref_php(pos);
        }

        void* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            return cb->byval_git2(argno);
        }
    private:
        php_callback_sync* cb;
    };

    // This alternate form of php_callback_async works for a non-resource
    // type. This means it doesn't have to perform a lookup on an underlying
    // resource object. Instead it uses the connected object directly.

    template<typename ConnectType>
    class php_callback_async_ex
    {
    public:
        using connect_t = ConnectType;
        typedef void* target_t;

        php_callback_async_ex(connect_t&& conn TSRMLS_DC):
            stor(std::forward<connect_t>(conn))
        {
            // Allocate php_callback_sync object. Assign a new php_callback_sync
            // object to the connected object. It must have a member called 'cb'
            // to which it has access.
            conn.cb = new (emalloc(sizeof(php_callback_sync))) php_callback_sync(TSRMLS_C);
        }

        zval** byref_php(unsigned pos)
        {
            return stor.cb->byref_php(pos);
        }

        void* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            return stor.cb->byval_git2(argno);
        }
    private:
        connect_t&& stor;
    };

    // Define a type to represent a callback function handler. All this does is
    // forward the function address to a git2 call.

    template<typename CallbackFunc>
    class php_callback_handler
    {
    public:
        ZTS_CONSTRUCTOR(php_callback_handler)

        typename CallbackFunc::type byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            // Return the static address of the wrapped callback function.
            return &CallbackFunc::callback;
        }
    };

    // Here we define all the callbacks used by the library. Some are specific
    // to a particular operation whereas others are more generic.

    struct packbuilder_foreach_callback
    {
        typedef int (*type)(void*,size_t,void*);
        static int callback(void* buf,size_t size,void* payload);
    };

    struct transfer_progress_callback
    {
        typedef int (*type)(const git_transfer_progress* stats,void* payload);
        static int callback(const git_transfer_progress* stats,void* payload);
    };

    struct odb_foreach_callback
    {
        typedef int (*type)(const git_oid* oid,void* payload);
        static int callback(const git_oid* oid,void* payload);
    };

} // namespace php_git2

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
