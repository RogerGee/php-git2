/*
 * php_function.h
 *
 * This file is a part of php-git.
 */

#ifndef PHPGIT2_FUNCTION_H
#define PHPGIT2_FUNCTION_H
#include "php-type.h"
#include <limits>
#include <utility>

namespace php_git2
{

    // Here we provide a type to contain the local variables used in a php-git
    // wrapper function.

    // Base object is empty.
    template<typename... Ts>
    class local_pack
    {
    public:
        static constexpr unsigned size()
        { return 0; }
    };

    // Recursive object inherits object containing the rest of the pack.
    template<typename T,typename... Ts>
    class local_pack<T,Ts...>:
        local_pack<Ts...>
    {
    protected:
        // Provide some meta-constructs to access an arbitrary type from a
        // parameter pack.

        template<unsigned K,typename U>
        struct type_selector;

        template<typename U,typename... Us>
        struct type_selector<0,local_pack<U,Us...> >
        {
            typedef U type;
        };

        template<unsigned K,typename U,typename... Us>
        struct type_selector<K,local_pack<U,Us...> >
        {
            typedef typename type_selector<K-1,
                local_pack<Us...> >::type type;
        };
    public:
        // Initialize to default. This is important if PHP assumes default
        // values for some parameters.
        local_pack():
            local(T())
        {
        }

        // The get<K> member function is used to extract a reference to the
        // zero-based, Kth element in the local variable pack.

        template<unsigned K>
        typename std::enable_if<K == 0,
            typename type_selector<0,
                local_pack<T,Ts...> >::type&>::type get()
        {
            return local;
        }

        template<unsigned K>
        typename std::enable_if<(K != 0),
            typename type_selector<K,
                local_pack<T,Ts...> >::type&>::type get()
        {
            return ((local_pack<Ts...>*)this)->get<K-1>();
        }

        // Provide the size of the pack as a constant function expression.

        static constexpr unsigned size()
        { return sizeof...(Ts) + 1; }
    private:
        T local;
    };

    // Provide some meta-constructs to generate a zero-based, integer sequence
    // parameter pack.

    template<unsigned... Ns>
    struct sequence
    {
    };

    template<unsigned N,unsigned... Ns>
    struct gen_seq:
        gen_seq<N-1,N,Ns...>
    {
    };

    template<unsigned... Ns>
    struct gen_seq<0,Ns...>
    {
        using type = sequence<0,Ns...>;
    };

    // Provide a special case for when N wraps around to 0-1. This will become
    // the max unsigned integer. We need this to generate an empty sequence.
    template<>
    struct gen_seq<std::numeric_limits<unsigned>::max()>
    {
        using type = sequence<>;
    };

    template<unsigned N>
    using make_seq = typename gen_seq<N-1>::type;

    // Provide a function that extracts zvals into a local pack.

    template<typename... Ts,unsigned... Ns>
    inline void php_extract_args(local_pack<Ts...>&& pack,sequence<Ns...>&& seq)
    {
        if (zend_get_parameters(0,int(sizeof...(Ns)),pack.template get<Ns>().byref_php()...) == FAILURE) {
            throw php_git2_exception("incorrect number of arguments passed to function");
        }
    }

    // Provide a template type that contains the function pointer and signature
    // of any libgit2 function we could want to call.

    template<typename T,typename... Ts>
    struct func_wrapper
    {
        typedef T (*func_type)(Ts...);

        template<func_type F>
        struct func
        {
            typedef T return_type;
            static constexpr unsigned arg_count()
            { return sizeof...(Ts); }
            static constexpr func_type name = F;
        };
    };

    // Provide a function that calls a wrapped library function with a local
    // pack.

    template<typename FuncWrapper,
        typename... Ts,
        unsigned... Ns,
        unsigned... Ps>
    inline typename FuncWrapper::return_type library_call(
        FuncWrapper&& wrapper,
        local_pack<Ts...>&& pack,
        sequence<Ns...>&& seq,
        sequence<Ps...>&& pos)
    {
        return FuncWrapper::name(pack.template get<Ns>().byval_git2(Ps + 1)...);
    }

    // Provide a function to handle return values. We enable different
    // specializations depending on if we want the actual library call return
    // value or to assign no return value.

    // This case assigns a return value from the local variable pack.
    template<int ReturnPos,
        typename T,
        typename... Ts>
    inline typename std::enable_if<0 < ReturnPos,void>::type
    php_return(
        T&& retval,
        local_pack<Ts...>&& pack,
        zval* return_value)
    {
        pack.template get<unsigned(ReturnPos) - 1>().ret(return_value);
        (void)retval;
    }

    // This case assigns the return value from the actual libgit2 library
    // call. We have specializations for: int.
    template<int ReturnPos,
        typename... Ts>
    inline typename std::enable_if<ReturnPos == 0,void>::type
    php_return(
        int&& retval,
        local_pack<Ts...>&& pack,
        zval* return_value)
    {
        RETVAL_LONG(long(retval));
        (void)pack;
    }

    // Otherwise, no action is taken.
    template<int ReturnPos,
        typename T,
        typename... Ts>
    inline typename std::enable_if<ReturnPos < 0,void>::type
    php_return(
        T&& retval,
        local_pack<Ts...>&& pack,
        zval* return_value)
    {
        (void)retval;
        (void)pack;
        (void)return_value;
    }

} // namespace php_git2

// We need a function entry macro that supports naming the PHP userspace
// function differently than its C++ name (since the C++ function is a
// template).

#define PHP_GIT2_FE(name,func,arginfo) \
    ZEND_FENTRY(name,&(func),arginfo,0)

// Define the base function template for php_git2. It's name has the typical
// prefix (though this may be unimportant). We cannot use the normal macros
// since when we instantiate the templates the comma delimiters are interpreted
// by the preprocessor.
//
// This function template separates the four major parts of a PHP wrapper:
//  1. Obtaining values from PHP userspace (by extracting them from zvals).
//  2. Modify them so that we can set up a call to the wrapped function.
//  3. Call the wrapped library function with the correct arguments.
//  4. Perform any housekeeping and optionally return a value.

template<
    typename FuncWrapper,
    typename LocalVars,
    int ReturnPos = -1,
    typename PHPForward = php_git2::make_seq<LocalVars::size()>,
    typename GitForward = php_git2::make_seq<LocalVars::size()>,
    typename AllParams = php_git2::make_seq<FuncWrapper::arg_count()> >
void zif_php_git2_function(INTERNAL_FUNCTION_PARAMETERS)
{
    // Create a local_pack object that houses all the variables we need for the
    // call. The constructors take care of any initialization.
    LocalVars vars;
    typename FuncWrapper::return_type retval;

    try {
        // Obtain values from PHP userspace.
        php_git2::php_extract_args(std::forward<LocalVars>(vars),PHPForward());

        // Call wrapped function.
        retval = php_git2::library_call(
            FuncWrapper(),
            std::forward<LocalVars>(vars),
            GitForward(),
            AllParams());

        // Handle the return value.
        php_git2::php_return<ReturnPos>(
            std::forward<typename FuncWrapper::return_type>(retval),
            std::forward<LocalVars>(vars),
            return_value);
    } catch (php_git2::php_git2_exception ex) {
        zend_throw_exception(nullptr,ex.what(),0 TSRMLS_CC);
    }
}

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
