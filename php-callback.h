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
            func(nullptr), data(nullptr),p(std::numeric_limits<unsigned>::max())
        {
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

            // Make sure the function zval is either a string or an array.
            if (Z_TYPE_P(func) != IS_STRING && Z_TYPE_P(func) != IS_ARRAY) {
                php_value_base::error("callable",argno);
            }

            return this;
        }

        zval* func;
        zval* data;
    private:
        unsigned p;
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

} // namespace php_git2

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
