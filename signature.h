/*
 * signature.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_SIGNATURE_H
#define PHPGIT2_SIGNATURE_H

namespace php_git2
{

    // Explicitly specialize git2_resource destructor for git_signature.
    template<> php_git_signature::~git2_resource()
    {
        git_signature_free(handle);
    }



} // php_git2

// Functions:

static constexpr auto ZIF_GIT_SIGNATURE_NEW = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_signature**,
        const char*,
        const char*,
        git_time_t,
        int>::func<git_signature_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_signature>,
        php_git2::php_string,
        php_git2::php_string,
        php_git2::php_long,
        php_git2::php_long
        >,
    1,
    php_git2::sequence<1,2,3,4>,
    php_git2::sequence<0,1,2,3,4>,
    php_git2::sequence<0,0,1,2,3>
    >;

// Function Entries:

#define GIT_SIGNATURE_FE                        \
    PHP_GIT2_FE(git_signature_new,ZIF_GIT_SIGNATURE_NEW,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
