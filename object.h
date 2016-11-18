/*
 * object.h
 *
 * This file is a part of php-git.
 */

#ifndef PHPGIT2_OBJECT_H
#define PHPGIT2_OBJECT_H

namespace php_git2
{

    // Explicitly specialize git2_resource destructor for git_object.
    template<> php_git_object::~git2_resource()
    {
        git_object_free(handle);
    }

} // namespace php_git2

// Functions:

static constexpr auto ZIF_GIT_OBJECT_ID = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        const git_object*>::func<git_object_id>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_object> >,
    0 >;

static constexpr auto ZIF_GIT_OBJECT_FREE = zif_php_git2_function_void<
    php_git2::func_wrapper<
        void,
        git_object*>::func<git_object_free>,
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_object> > >;

// Function Entries:

#define GIT_OBJECT_FE \
    PHP_GIT2_FE(git_object_id,ZIF_GIT_OBJECT_ID,NULL)       \
    PHP_GIT2_FE(git_object_free,ZIF_GIT_OBJECT_FREE,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
