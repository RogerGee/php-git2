/*
 * php-callback.h
 *
 * This file is a part of php-git.
 */

#ifndef PHPGIT2_PHP_CALLBACK_H
#define PHPGIT2_PHP_CALLBACK_H
#include "php-type.h"

namespace php_git2
{

    static inline void init_zvals(zval* z,unsigned n)
    {
        for (unsigned i = 0;i < n;++i) {
            zval* p = z+i;
            INIT_PZVAL(p);
            ZVAL_NULL(p);
        }
    }

    static inline void delete_zvals(zval* z,unsigned n)
    {
        for (unsigned i = 0;i < n;++i) {
            zval_dtor(z + i);
        }
    }

    // Provide a type that contains an array of zvals converted from arbitrary
    // git2 values.

    template<unsigned Count>
    class zval_array
    {
    public:
        zval_array()
        {
            init_zvals(params,Count);
        }

        ~zval_array()
        {
            delete_zvals(params,Count);
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
            ZVAL_LONG(params+I,h);
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        template<unsigned I,typename... Ts>
        void assign(double&& h,Ts&&... ts)
        {
            ZVAL_DOUBLE(params+I,h);
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        template<unsigned I,typename... Ts>
        void assign(const char*&& h,Ts&&... ts)
        {
            ZVAL_STRING(params+I,h,1);
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        template<unsigned I,typename... Ts>
        void assign(const void*&& a,size_t&& b,Ts&&... ts)
        {
            ZVAL_STRINGL(params+I,(const char*)a,b,1);
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        template<unsigned I,typename... Ts>
        void assign(zval*&& h,Ts&&... ts)
        {
            if (h != nullptr) {
                // Perform a deep copy of the zval.
                params[I] = *h;
                zval_copy_ctor(params + I);
            }
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        void call(zval* func,zval* ret)
        {
            int r;
            zval* ps[Count];

            // We allow null callables, which do nothing.
            if (Z_TYPE_P(func) == IS_NULL) {
                return;
            }

            // Otherwise assume the zval is a callable and attempt to call it.
            for (unsigned i = 0;i < Count;++i) {
                ps[i] = params + i;
            }
            ZVAL_NULL(ret);
            r = call_user_function(EG(function_table),NULL,func,ret,Count,ps TSRMLS_CC);
            if (r == FAILURE) {
                throw php_git2_exception("unable to call userspace function: "
                    "expected a callable");
            }
        }

        zval* get_params()
        { return params; }
        const zval* get_params() const
        { return params; }

        zval* operator [](unsigned index)
        { return params + index; }
        const zval* operator [](unsigned index) const
        { return params + index; }
    private:
        zval params[Count];
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

    struct php_callback_sync
    {
    public:
        php_callback_sync():
            func(nullptr), data(nullptr), p(std::numeric_limits<unsigned>::max())
        {
        }

        ~php_callback_sync()
        {
            // Delete our references to the variables. Member 'p' is negative
            // when we've upped the count.
            if (p < 0) {
                Z_DELREF_P(func);
                Z_DELREF_P(data);
            }
        }

        zval** byref_php(unsigned pos)
        {
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

            // Make sure the function zval is either a string or an array. We
            // allow the user to omit a callable if NULL is given.
            if (Z_TYPE_P(func) != IS_NULL && Z_TYPE_P(func) != IS_STRING
                && Z_TYPE_P(func) != IS_ARRAY)
            {
                php_value_base::error("callable",argno);
            }

            // Up reference count. This is really only needed for when this
            // class is wrapped by php_callback_async so that the PHP values
            // with exist between function calls.
            Z_ADDREF_P(func);
            Z_ADDREF_P(data);
            p = -1;
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
     * asynchronous callback object to a resource. In this way the lifetime of
     * the callback is the same as the lifetime of the resource.
     */

    template<typename GitResource>
    class php_callback_async
    {
    public:
        // Connect to an arbitrary resource type. It must be a newly created
        // resource (i.e. resource ref).
        using connect_t = php_resource_ref<GitResource>;
        typedef void* target_t;

        php_callback_async(connect_t&& conn)
        {
            // Allocate php_callback_sync object.
            cb = new (emalloc(sizeof(php_callback_sync))) php_callback_sync;

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

    // Define a type to represent a callback function handler. All this does is
    // forward the function address to a git2 call.

    template<typename CallbackFunc>
    class php_callback_handler
    {
    public:
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

} // namespace php_git2

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
