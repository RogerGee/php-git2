/*
 * php_function.h
 *
 * This file is a part of php-git.
 */

#ifndef PHPGIT2_FUNCTION_H
#define PHPGIT2_FUNCTION_H
#include "php-type.h"

namespace php_git2
{

    /* Here we provide a type to contain the local variables used in a php-git
     * wrapper function.
     */

    // Base object is empty.
    template<typename... Ts>
    class php_git2_local_pack
    {
    };

    // Recursive object inherits object containing the rest of the pack.
    template<typename T,typename... Ts>
    class php_git2_local_pack<T,Ts...>:
        php_git2_local_pack<Ts...>
    {
    protected:
        // Provide some meta-constructs to access an arbitrary type from a
        // parameter pack.

        template<size_t K,typename U>
        struct type_selector;

        template<typename U,typename... Us>
        struct type_selector<0,php_git2_local_pack<U,Us...> >
        {
            typedef U type;
        };

        template<size_t K,typename U,typename... Us>
        struct type_selector<K,php_git2_local_pack<U,Us...> >
        {
            typedef typename type_selector<K-1,
                php_git2_local_pack<Us...> >::type type;
        };
    public:
        // Initialize to default. This is important if PHP assumes default
        // values for some parameters.
        php_git2_local_pack():
            local(T())
        {
        }

        // The get<K> member function is used to extract a reference to the
        // zero-based, Kth element in the local variable pack.

        template<size_t K>
        typename std::enable_if<K == 0,
            typename type_selector<0,
                php_git2_local_pack<T,Ts...> >::type&>::type get()
        {
            return local;
        }

        template<size_t K>
        typename std::enable_if<(K != 0),
            typename type_selector<K,
                php_git2_local_pack<T,Ts...> >::type&>::type get()
        {
            return ((php_git2_local_pack<Ts...>*)this)->get<K-1>();
        }

        template<size_t K>
        typename std::enable_if<K == 0,
            typename type_selector<0,
                const php_git2_local_pack<T,Ts...> >::type&>::type get() const
        {
            return local;
        }

        template<size_t K>
        typename std::enable_if<(K != 0),
            typename type_selector<K,
                const php_git2_local_pack<T,Ts...> >::type&>::type get() const
        {
            return ((php_git2_local_pack<Ts...>*)this)->get<K-1>();
        }
    private:
        T local;
    };

} // namespace php_git2

// We need a function entry macro that supports naming the PHP userspace
// function differently than its C++ name (since the C++ function is a
// template).

#define PHP_GIT2_FE(name,func,arginfo) \
    ZEND_FENTRY(name,&(func),arginfo,0)

// TODO: Define the base function template for php_git2. It's name has the
// typical prefix (though this may be unimportant). We cannot use the normal
// macros since when we instantiate the templates the comma delimiters are
// interpreted by the preprocessor.

template<typename... Ts>
void zif_php_git2_function(INTERNAL_FUNCTION_PARAMETERS)
{
    php_git2::php_git2_local_pack<Ts...> pack;


}

#endif

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
