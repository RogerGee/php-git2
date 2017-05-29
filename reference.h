/*
 * reference.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_REFERENCE_H
#define PHPGIT2_REFERENCE_H
#include "php-type.h"
#include "php-function.h"

namespace php_git2
{

    // Explicitly specialize git2_resource destructor for git_reference.
    template<> php_git_reference::~git2_resource()
    {
        git_reference_free(handle);
    }

} // namespace php_git2

// Functions:

static constexpr auto ZIF_GIT_REFERENCE_LIST = zif_php_git2_function<
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
    php_git2::sequence<0,0> >;

static constexpr auto ZIF_GIT_REFERENCE_LOOKUP = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_reference**,
        git_repository*,
        const char*>::func<git_reference_lookup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_reference>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string>,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1> >;

static constexpr auto ZIF_GIT_REFERENCE_FREE = zif_php_git2_function_void<
    php_git2::func_wrapper<
        void,
        git_reference*>::func<git_reference_free>,
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_reference> > >;

static constexpr auto ZIF_GIT_REFERENCE_PEEL = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_object**,
        git_reference*,
        git_otype>::func<git_reference_peel>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_object>,
        php_git2::php_resource<php_git2::php_git_reference>,
        php_git2::php_long_cast<git_otype> >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1> >;

static constexpr auto ZIF_GIT_REFERENCE_NAME_TO_ID = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_repository*,
        const char*>::func<git_reference_name_to_id>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string>,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1> >;

static constexpr auto ZIF_GIT_REFERENCE_CREATE = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_reference**,
        git_repository*,
        const char*,
        const git_oid*,
        int,
        const char*
        >::func<git_reference_create>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_reference>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_git_oid_fromstr,
        php_git2::php_bool,
        php_git2::php_string
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2,3,4,5>,
    php_git2::sequence<0,1,2,3,4,5>,
    php_git2::sequence<0,0,1,2,3,4>
    >;

static constexpr auto ZIF_GIT_REFERENCE_CREATE_MATCHING = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_reference**,
        git_repository*,
        const char*,
        const git_oid*,
        int,
        const git_oid*,
        const char*
        >::func<git_reference_create_matching>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_reference>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_git_oid_fromstr,
        php_git2::php_bool,
        php_git2::php_git_oid_fromstr,
        php_git2::php_string
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2,3,4,5,6>,
    php_git2::sequence<0,1,2,3,4,5,6>,
    php_git2::sequence<0,0,1,2,3,4,5>
    >;

// Function Entries:

#define GIT_REFERENCE_FE \
    PHP_GIT2_FE(git_reference_list,ZIF_GIT_REFERENCE_LIST,NULL)         \
    PHP_GIT2_FE(git_reference_lookup,ZIF_GIT_REFERENCE_LOOKUP,NULL)     \
    PHP_GIT2_FE(git_reference_free,ZIF_GIT_REFERENCE_FREE,NULL)         \
    PHP_GIT2_FE(git_reference_peel,ZIF_GIT_REFERENCE_PEEL,NULL)         \
    PHP_GIT2_FE(git_reference_name_to_id,ZIF_GIT_REFERENCE_NAME_TO_ID,NULL) \
    PHP_GIT2_FE(git_reference_create,ZIF_GIT_REFERENCE_CREATE,NULL)     \
    PHP_GIT2_FE(git_reference_create_matching,ZIF_GIT_REFERENCE_CREATE_MATCHING,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
