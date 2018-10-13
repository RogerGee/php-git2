/*
 * php_function.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_FUNCTION_H
#define PHPGIT2_FUNCTION_H
#include "php-type.h"
#include <limits>
#include <utility>

namespace php_git2
{

    // Here we provide a type to contain the local variables used in a php-git2
    // wrapper function.

    // Base object is empty.
    template<typename... Ts>
    class local_pack
    {
    public:
        local_pack(TSRMLS_D)
        {
#ifdef ZTS
            UNUSED(TSRMLS_C);
#endif
        }

        static constexpr unsigned size()
        {
            return 0;
        }
    };

    // Recursive object inherits object containing the rest of the pack.
    template<typename T,typename... Ts>
    class local_pack<T,Ts...>:
        private local_pack<Ts...>
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
        local_pack(TSRMLS_D):
            local_pack<Ts...>(TSRMLS_C), local(T(TSRMLS_C))
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
            return ((local_pack<Ts...>*)this)->template get<K-1>();
        }

        // Provide the size of the pack as a constant function expression.

        static constexpr unsigned size()
        {
            return sizeof...(Ts) + 1;
        }
    private:
        T local;
    };

    // Provide a type to connect adjacent variables in a local_pack. This will
    // merely serve as a wrapper for connectors defined in the different
    // implementations. The wrapper's template parameter must define a
    // byval_git2() member function and two member types called 'target_t' and
    // 'connect_t'. We must specialize local_pack for this type.

    template<typename T>
    class connector_wrapper
    {
    public:
        connector_wrapper(typename T::connect_t& obj TSRMLS_DC):
            connector(obj TSRMLS_CC)
        {
        }

        connector_wrapper(typename T::connect_t&& obj TSRMLS_DC):
            connector(std::forward<typename T::connect_t>(obj) TSRMLS_CC)
        {
        }

        zval** byref_php(unsigned pos)
        {
            return connector.byref_php(pos);
        }

        typename T::target_t
        byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            return connector.byval_git2(argno);
        }

        void ret(zval* return_value)
        {
            connector.ret(return_value);
        }

        T& get_connector()
        {
            return connector;
        }

        const T& get_connector() const
        {
            return connector;
        }
    private:
        T connector;
    };

    template<typename T>
    class connector_wrapper_ex:
        public connector_wrapper<T>
    {
    public:
        connector_wrapper_ex(connector_wrapper<typename T::connect_t>& obj TSRMLS_DC):
            connector_wrapper<T>(obj.get_connector() TSRMLS_CC)
        {
        }

        connector_wrapper_ex(connector_wrapper<typename T::connect_t>&& obj TSRMLS_DC):
            connector_wrapper<T>(obj.get_connector() TSRMLS_CC)
        {
        }
    };

    template<typename T,typename... Ts>
    class local_pack<connector_wrapper<T>,Ts...>:
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
        local_pack(TSRMLS_D):
            local_pack<Ts...>(TSRMLS_C), local(get<1>() TSRMLS_CC)
        {
        }

        // The get<K> member function is used to extract a reference to the
        // zero-based, Kth element in the local variable pack.

        template<unsigned K>
        typename std::enable_if<K == 0,
            typename type_selector<0,
                local_pack<connector_wrapper<T>,Ts...> >::type&>::type get()
        {
            return local;
        }

        template<unsigned K>
        typename std::enable_if<(K != 0),
            typename type_selector<K,
                local_pack<connector_wrapper<T>,Ts...> >::type&>::type get()
        {
            return ((local_pack<Ts...>*)this)->template get<K-1>();
        }

        // Provide the size of the pack as a constant function expression.

        static constexpr unsigned size()
        {
            return sizeof...(Ts) + 1;
        }
    private:
        connector_wrapper<T> local;
    };

    template<typename T,typename... Ts>
    class local_pack<connector_wrapper_ex<T>,Ts...>:
        private local_pack<Ts...>
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
        local_pack(TSRMLS_D):
            local_pack<Ts...>(TSRMLS_C), local(get<1>() TSRMLS_CC)
        {
        }

        // The get<K> member function is used to extract a reference to the
        // zero-based, Kth element in the local variable pack.

        template<unsigned K>
        typename std::enable_if<K == 0,
            typename type_selector<0,
                local_pack<connector_wrapper_ex<T>,Ts...> >::type&>::type get()
        {
            return local;
        }

        template<unsigned K>
        typename std::enable_if<(K != 0),
            typename type_selector<K,
                local_pack<connector_wrapper_ex<T>,Ts...> >::type&>::type get()
        {
            return ((local_pack<Ts...>*)this)->template get<K-1>();
        }

        // Provide the size of the pack as a constant function expression.

        static constexpr unsigned size()
        {
            return sizeof...(Ts) + 1;
        }
    private:
        connector_wrapper_ex<T> local;
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

    template<typename... Ts,unsigned... Ns,unsigned... Ps>
    inline void php_extract_args_impl(local_pack<Ts...>& pack,
        sequence<Ns...>&& seq,
        sequence<Ps...>&& pos)
    {
        if (zend_get_parameters(0,int(sizeof...(Ns)),
                pack.template get<Ns>().byref_php(Ps)...) == FAILURE)
        {
            php_error(E_ERROR,"Incorrect number of arguments passed to function");
        }
    }

    template<typename... Ts,unsigned... Ns>
    inline void php_extract_args(local_pack<Ts...>& pack,sequence<Ns...>&& seq)
    {
        // Produce a meta-construct that contains the position parameters.
        php_extract_args_impl(pack,
            std::forward<sequence<Ns...> >(seq),
            std::forward<make_seq<sizeof...(Ns)> >(
                make_seq<sizeof...(Ns)>()));
    }

    template<typename... Ts>
    inline void php_extract_args(local_pack<Ts...>&,sequence<>&&)
    {
        // Specialize call for empty sequence where no args need to be extracted
        // from userspace.
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
            {
                return sizeof...(Ts);
            }

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
        local_pack<Ts...>& pack,
        sequence<Ns...>&& seq,
        sequence<Ps...>&& pos)
    {
        return FuncWrapper::name(pack.template get<Ns>().byval_git2(Ps+1)...);
    }

    // Provide template function overloads to handle return values. We enable
    // different specializations depending on which value (if any) is to be used
    // as the return value (i.e. the function return value or some value in the
    // local pack).

    // CASE #1: Assign a return value from the local variable pack. The variable
    // in the pack must have a 'ret' member function.

    template<int ReturnPos,typename T,typename... Ts>
    inline typename std::enable_if<0 < ReturnPos,void>::type
    php_return(const T&,local_pack<Ts...>& pack,zval* return_value)
    {
        pack.template get<unsigned(ReturnPos) - 1>().ret(return_value);
    }

    // CASE #2: Assign the return value from the actual libgit2 library call. We
    // have specializations for many various types. These functions use the
    // RETVAL_* macros from the PHP API that expect a zval in scope called
    // "return_value".

    template<int ReturnPos,typename... Ts>
    inline typename std::enable_if<ReturnPos == 0,void>::type
    php_return(char retval,local_pack<Ts...>&,zval* return_value)
    {
        char str[2] = {retval,0};
        RETVAL_STRING(str,1);
    }

    template<int ReturnPos,typename... Ts>
    inline typename std::enable_if<ReturnPos == 0,void>::type
    php_return(long retval,local_pack<Ts...>&,zval* return_value)
    {
        RETVAL_LONG(retval);
    }

    template<int ReturnPos,typename... Ts>
    inline typename std::enable_if<ReturnPos == 0,void>::type
    php_return(int retval,local_pack<Ts...>&,zval* return_value)
    {
        RETVAL_LONG(static_cast<long>(retval));
    }

    template<int ReturnPos,typename... Ts>
    inline typename std::enable_if<ReturnPos == 0,void>::type
    php_return(unsigned int retval,local_pack<Ts...>&,zval* return_value)
    {
        RETVAL_LONG(static_cast<long>(retval));
    }

    template<int ReturnPos,typename... Ts>
    inline typename std::enable_if<ReturnPos == 0,void>::type
    php_return(long unsigned int retval,local_pack<Ts...>&,zval* return_value)
    {
        RETVAL_LONG(static_cast<long>(retval));
    }

    template<int ReturnPos,typename... Ts>
    inline typename std::enable_if<ReturnPos == 0,void>::type
    php_return(long long int& retval,local_pack<Ts...>&,zval* return_value)
    {
        RETVAL_LONG(static_cast<long>(retval));
    }

    template<int ReturnPos,typename... Ts>
    inline typename std::enable_if<ReturnPos == 0,void>::type
    php_return(const char* retval,local_pack<Ts...>& pack,zval* return_value)
    {
        if (retval != nullptr) {
            RETVAL_STRING(retval,1);
        }
        else {
            // Just in case.
            RETVAL_NULL();
        }
    }

    template<int ReturnPos,typename... Ts>
    inline typename std::enable_if<ReturnPos == 0,void>::type
    php_return(const git_oid* retval,local_pack<Ts...>& pack,zval* return_value)
    {
        if (retval != nullptr) {
            char buf[GIT_OID_HEXSZ + 1];
            git_oid_tostr(buf,sizeof(buf),retval);
            RETVAL_STRING(buf,strlen(buf));
        }
        else {
            RETVAL_NULL();
        }
    }

    // CASE #3: Assign no return value.

    template<int ReturnPos,typename T,typename... Ts>
    inline typename std::enable_if<ReturnPos < 0,void>::type
    php_return(const T&,local_pack<Ts...>&,zval*)
    {
    }

    // Provide a function that specializes checking return values from php
    // functions. These only work in the general case so be mindful.

    template<typename T>
    inline bool check_return(const T&)
    {
        // By default, everything succeeds.
        return true;
    }

    template<>
    inline bool check_return(const int& retval)
    {
        // Most libgit2 functions are in error if they return less than 0.
        return retval >= 0;
    }

    // Provide a function for setting up resource dependencies.

    template<typename T,typename U>
    inline void php_set_resource_dependency_impl(
        git2_resource<T>* child,
        git2_resource<U>* parent)
    {
        if (child != nullptr && parent != nullptr) {
            child->set_parent(parent);
        }
    }

    template<typename... Ts,unsigned... Ns>
    inline void php_set_resource_dependency(local_pack<Ts...>& pack,sequence<Ns...>&& seq)
    {
        php_set_resource_dependency_impl(pack.template get<Ns>().get_object()...);
    }

} // namespace php_git2

// We need a function entry macro that supports naming the PHP userspace
// function differently than its C++ name (since the C++ function is a
// template).

#define PHP_GIT2_FE(name,func,arginfo) \
    ZEND_FENTRY(name,(func),arginfo,0)

#define PHP_GIT2_UNIMPLEMENTED(name,func,arginfo)

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
//
// The function template accepts lists of integer position values called
// sequences. These sequences describe the position of parameters in the local
// pack as well as the order of parameters to the PHP userspace
// function. Sequences are zero-based, meaning the first position is always
// denoted with a zero. Despite being called sequences, sequences are actually
// ordered multisets in disguise (sorry).
//
// We provide different variations of this template to meet certain
// requirements, but all of them are based on the following primary template:

template<
    // The function wrapper type used to call the wrapped function.
    typename FuncWrapper,

    // The local variable pack type to instantiate local variables for the
    // call. This should be some instantiation of local_pack<T,Ts...>.
    typename LocalVars,

    // The position of the local pack variable to return. If -1 then no value
    // is returned. If 0 then the actual return value is returned (must be an
    // integer in this case).
    int ReturnPos = -1,

    // The sequence used to forward the local pack variables into the
    // zend_get_arguments() call.
    typename PHPForward = php_git2::make_seq<LocalVars::size()>,

    // The sequence used to forward the local pack variables into the call to
    // the wrapped libgit2 function.
    typename GitForward = php_git2::make_seq<LocalVars::size()>,

    // The sequence that describes the position of the arguments in the
    // GitForward set relative to the PHP function call. This pack must be the
    // same length as GitForward. For example, if GitForward is <2> and
    // AllParams is <0> then this means that the third pack variable was the
    // first argument to the PHP function.
    typename AllParams = php_git2::make_seq<FuncWrapper::arg_count()> >
static void zif_php_git2_function(INTERNAL_FUNCTION_PARAMETERS)
{
    php_git2::php_bailer bailer;
    php_git2::php_bailout_context ctx(bailer);

    // Create a local_pack object that houses all the variables we need for the
    // call. The constructors take care of any initialization.

    LocalVars vars ZTS_CTOR;
    typename FuncWrapper::return_type retval;

    if (BAILOUT_ENTER_REGION(ctx)) {
        try {
            // Obtain values from PHP userspace.
            php_git2::php_extract_args(vars,PHPForward());

            // Call wrapped function.
            retval = php_git2::library_call(FuncWrapper(),vars,GitForward(),AllParams());

            // Check for errors (when return value is less than zero).
            if (php_git2::check_return(retval)) {
                // Handle the return value.
                php_git2::php_return<ReturnPos>(retval,vars,return_value);
            }
            else {
                // Throw error with formatted message from git2.
                php_git2::git_error(retval);
            }

        } catch (php_git2::php_git2_exception_base& ex) {
            php_git2::php_bailout_context ctx2(bailer);

            if (BAILOUT_ENTER_REGION(ctx2)) {
                ex.handle(TSRMLS_C);
            }
        }
    }
}

// Provide a variant for less intuitive but more fine-tuned return value
// handling. It should be used in exceptional cases. A type (ReturnHandler) is
// provided that contains a member function 'ReturnHandler::ret()' that is
// called to handle the return value. This function accepts the following
// parameters:
//  1. return value from libgit2 function
//  2. return_value zval from php function handler
//  3. local variable pack
// The function should return 'true' if it handled the return value,

template<
    typename FuncWrapper,
    typename LocalVars,
    typename ReturnHandler,
    typename PHPForward = php_git2::make_seq<LocalVars::size()>,
    typename GitForward = php_git2::make_seq<LocalVars::size()>,
    typename AllParams = php_git2::make_seq<FuncWrapper::arg_count()>
    >
static void zif_php_git2_function_rethandler(INTERNAL_FUNCTION_PARAMETERS)
{
    php_git2::php_bailer bailer;
    php_git2::php_bailout_context ctx(bailer);

    // Create a local_pack object that houses all the variables we need for the
    // call. The constructors take care of any initialization.

    LocalVars vars ZTS_CTOR;
    typename FuncWrapper::return_type retval;

    if (BAILOUT_ENTER_REGION(ctx)) {
        try {
            // Obtain values from PHP userspace.
            php_git2::php_extract_args(vars,PHPForward());

            // Call wrapped function.
            retval = php_git2::library_call(FuncWrapper(),vars,GitForward(),AllParams());

            // Instantiate a return value handler to handle the return value.
            ReturnHandler rethandler;
            if (!rethandler.ret(retval,return_value,vars)) {
                // Throw error with formatted message from git2.
                php_git2::git_error(retval);
            }
        } catch (php_git2::php_git2_exception_base& ex) {
            php_git2::php_bailout_context ctx2(bailer);

            if (BAILOUT_ENTER_REGION(ctx2)) {
                ex.handle(TSRMLS_C);
            }
        }
    }
}

// Provide a similar function for wrapped library calls that have return type
// void.

template<
    typename FuncWrapper,
    typename LocalVars,
    typename PHPForward = php_git2::make_seq<LocalVars::size()>,
    typename GitForward = php_git2::make_seq<LocalVars::size()>,
    typename AllParams = php_git2::make_seq<FuncWrapper::arg_count()>
    >
static void zif_php_git2_function_void(INTERNAL_FUNCTION_PARAMETERS)
{
    php_git2::php_bailer bailer;
    php_git2::php_bailout_context ctx(bailer);

    // Create a local_pack object that houses all the variables we need for
    // the call. The constructors take care of any initialization.

    LocalVars vars ZTS_CTOR;

    if (BAILOUT_ENTER_REGION(ctx)) {
        try {
            php_git2::php_extract_args(vars,PHPForward());
            php_git2::library_call(FuncWrapper(),vars,GitForward(),AllParams());
        } catch (php_git2::php_git2_exception_base& ex) {
            php_git2::php_bailout_context ctx2(bailer);

            if (BAILOUT_ENTER_REGION(ctx2)) {
                ex.handle(TSRMLS_C);
            }
        }
    }
}

// Provide a variant to handle setting resource dependencies. This adds a
// template argument 'ResourceDeps' which is a sequence describing the
// subsequence of the local pack representing the resource dependency
// sequence. This should always have two elements. The first is the dependent
// resource, and the second is the resource that is the dependency (i.e. 1st is
// child, 2nd is parent).

template<
    typename FuncWrapper,
    typename LocalVars,
    typename ResourceDeps,
    int ReturnPos = -1,
    typename PHPForward = php_git2::make_seq<LocalVars::size()>,
    typename GitForward = php_git2::make_seq<LocalVars::size()>,
    typename AllParams = php_git2::make_seq<FuncWrapper::arg_count()>
    >
static void zif_php_git2_function_setdeps(INTERNAL_FUNCTION_PARAMETERS)
{
    php_git2::php_bailer bailer;
    php_git2::php_bailout_context ctx(bailer);

    // Create a local_pack object that houses all the variables we need for the
    // call. The constructors take care of any initialization.

    LocalVars vars ZTS_CTOR;
    typename FuncWrapper::return_type retval;

    if (BAILOUT_ENTER_REGION(ctx)) {
        try {
            // Obtain values from PHP userspace.
            php_git2::php_extract_args(vars,PHPForward());

            // Call wrapped function.
            retval = php_git2::library_call(FuncWrapper(),vars,GitForward(),AllParams());

            // Check for errors (when return value is less than zero).
            if (php_git2::check_return(retval)) {
                // Handle the return value.
                php_git2::php_return<ReturnPos>(retval,vars,return_value);

                // Call function to set resource dependency. We only do this
                // on success!
                php_git2::php_set_resource_dependency(vars,ResourceDeps());
            }
            else {
                // Throw error with formatted message from git2.
                php_git2::git_error(retval);
            }
        } catch (php_git2::php_git2_exception_base& ex) {
            php_git2::php_bailout_context ctx2(bailer);

            if (BAILOUT_ENTER_REGION(ctx2)) {
                ex.handle(TSRMLS_C);
            }
        }
    }
}

template<
    typename FuncWrapper,
     typename LocalVars,
    typename ResourceDeps1,
    typename ResourceDeps2,
    int ReturnPos = -1,
    typename PHPForward = php_git2::make_seq<LocalVars::size()>,
    typename GitForward = php_git2::make_seq<LocalVars::size()>,
    typename AllParams = php_git2::make_seq<FuncWrapper::arg_count()>
    >
static void zif_php_git2_function_setdeps2(INTERNAL_FUNCTION_PARAMETERS)
{
    php_git2::php_bailer bailer;
    php_git2::php_bailout_context ctx(bailer);

    // Create a local_pack object that houses all the variables we need for the
    // call. The constructors take care of any initialization.

    LocalVars vars ZTS_CTOR;
    typename FuncWrapper::return_type retval;

    if (BAILOUT_ENTER_REGION(ctx)) {
        try {
            // Obtain values from PHP userspace.
            php_git2::php_extract_args(vars,PHPForward());

            // Call wrapped function.
            retval = php_git2::library_call(FuncWrapper(),vars,GitForward(),AllParams());

            // Check for errors (when return value is less than zero).
            if (php_git2::check_return(retval)) {
                // Handle the return value.
                php_git2::php_return<ReturnPos>(retval,vars,return_value);

                // Call function to set resource dependencies. We only do this
                // on success!
                php_git2::php_set_resource_dependency(vars,ResourceDeps1());
                php_git2::php_set_resource_dependency(vars,ResourceDeps2());
            }
            else {
                // Throw error with formatted message from git2.
                php_git2::git_error(retval);
            }
        } catch (php_git2::php_git2_exception_base& ex) {
            php_git2::php_bailout_context ctx2(bailer);

            if (BAILOUT_ENTER_REGION(ctx2)) {
                ex.handle(TSRMLS_C);
            }
        }
    }
}

template<
    typename FuncWrapper,
    typename LocalVars,
    typename ResourceDeps,
    typename PHPForward = php_git2::make_seq<LocalVars::size()>,
    typename GitForward = php_git2::make_seq<LocalVars::size()>,
    typename AllParams = php_git2::make_seq<FuncWrapper::arg_count()>
    >
static void zif_php_git2_function_setdeps_void(INTERNAL_FUNCTION_PARAMETERS)
{
    php_git2::php_bailer bailer;
    php_git2::php_bailout_context ctx(bailer);

    // Create a local_pack object that houses all the variables we need for the
    // call. The constructors take care of any initialization.

    LocalVars vars ZTS_CTOR;

    if (BAILOUT_ENTER_REGION(ctx)) {
        try {
            // Obtain values from PHP userspace.
            php_git2::php_extract_args(vars,PHPForward());

            // Make call to underlying git2 function.
            php_git2::library_call(FuncWrapper(),vars,GitForward(),AllParams());

            // Call function to set resource dependency.
            php_git2::php_set_resource_dependency(vars,ResourceDeps());
        } catch (php_git2::php_git2_exception_base& ex) {
            php_git2::php_bailout_context ctx2(bailer);

            if (BAILOUT_ENTER_REGION(ctx2)) {
                ex.handle(TSRMLS_C);
            }
        }
    }
}

// Provide a variant for handling the free functions. These never call the
// underlying free function in libgit2 directly. Instead, they invalidate the
// resource and let PHP decide if the resource object should get cleaned up.

template<
    typename LocalVars,
    typename PHPForward = php_git2::make_seq<LocalVars::size()>
    >
static void zif_php_git2_function_free(INTERNAL_FUNCTION_PARAMETERS)
{
    php_git2::php_bailer bailer;
    php_git2::php_bailout_context ctx(bailer);

    // Create a local_pack object that houses all the variables we need for the
    // call. The constructors take care of any initialization.

    LocalVars vars ZTS_CTOR;

    if (BAILOUT_ENTER_REGION(ctx)) {
        try {
            php_git2::php_extract_args(vars,PHPForward());

            // Assume the first element is the resource to delete. Call its
            // byval_git2() member function to cause it to be freed. We never
            // call the library function directly since the resource handler
            // handles freeing instead.
            vars.template get<0>().byval_git2(1);
        } catch (php_git2::php_git2_exception_base& ex) {
            php_git2::php_bailout_context ctx2(bailer);

            if (BAILOUT_ENTER_REGION(ctx2)) {
                ex.handle(TSRMLS_C);
            }
        }
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
