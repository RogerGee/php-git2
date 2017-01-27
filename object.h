/*
 * object.h
 *
 * This file is a part of php-git2.
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

static constexpr auto ZIF_GIT_OBJECT_LOOKUP = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_object**,
        git_repository*,
        const git_oid*,
        git_otype>::func<git_object_lookup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_object>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_git_oid_fromstr,
        php_git2::php_long_cast<git_otype>
        >,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>,
    php_git2::sequence<0,0,1,2>
    >;

static constexpr auto ZIF_GIT_OBJECT_FREE = zif_php_git2_function_void<
    php_git2::func_wrapper<
        void,
        git_object*>::func<git_object_free>,
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_object> > >;

// Function Entries:

#define GIT_OBJECT_FE \
    PHP_GIT2_FE(git_object_id,ZIF_GIT_OBJECT_ID,NULL)           \
    PHP_GIT2_FE(git_object_lookup,ZIF_GIT_OBJECT_LOOKUP,NULL)   \
    PHP_GIT2_FE(git_object_free,ZIF_GIT_OBJECT_FREE,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
