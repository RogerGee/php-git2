/*
 * reference.h
 *
 * This file is a part of php-git.
 */

#ifndef PHPGIT2_REFERENCE_H
#define PHPGIT2_REFERENCE_H
#include "php-type.h"
#include "php-function.h"

namespace php_git2
{

    // Explicitly specialize git2_resource destructor for git_repository.
    template<> php_git_reference::~git2_resource()
    {
        git_reference_free(handle);
    }

} // namespace php_git2

// Functions:

static constexpr auto GIT_REFERENCE_LIST = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_strarray*,
        git_repository*>::func<git_reference_list>,
    php_git2::local_pack<
        php_git2::php_strarray,
        php_git2::php_resource<php_git2::php_git_repository> >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>,
    php_git2::sequence<0,1> >;

static constexpr auto GIT_REFERENCE_LOOKUP = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_reference**,
        git_repository*,
        const char*>::func<git_reference_lookup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_reference>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string>,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2> >;

static constexpr auto GIT_REFERENCE_FREE = zif_php_git2_function_void<
    php_git2::func_wrapper<
        void,
        git_reference*>::func<git_reference_free>,
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_reference> > >;

// Function Entries:

#define GIT_REFERENCE_FE \
    PHP_GIT2_FE(git_reference_list,GIT_REFERENCE_LIST,NULL)     \
    PHP_GIT2_FE(git_reference_lookup,GIT_REFERENCE_LOOKUP,NULL) \
    PHP_GIT2_FE(git_reference_free,GIT_REFERENCE_FREE,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
