/*
 * repository.h
 *
 * This file is a part of php-git.
 */

#ifndef PHPGIT2_REPOSITORY_H
#define PHPGIT2_REPOSITORY_H
#include "php-git2.h"
#include "php-function.h"

namespace php_git2
{

    // Explicitly specialize git2_resource destructor for git_repository.
    template<> git2_resource<git_repository>::~git2_resource()
    {
        git_repository_free(handle);
    }

    using php_git_repository = git2_resource<git_repository>;

} // namespace php_git2

// Functions:

static constexpr auto GIT_REPOSITORY_INIT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository**,
        const char*,
        unsigned int>::func<git_repository_init>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_bool>,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>,
    php_git2::sequence<-1,1,2> >;

static constexpr auto GIT_REPOSITORY_FREE = zif_php_git2_function_void<
    php_git2::func_wrapper<
        void,
        git_repository*>::func<git_repository_free>,
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_repository> > >;

// Function Entries:

#define GIT_REPOSITORY_FE \
    PHP_GIT2_FE(git_repository_init,GIT_REPOSITORY_INIT,NULL)   \
    PHP_GIT2_FE(git_repository_free,GIT_REPOSITORY_FREE,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
