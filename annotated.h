/*
 * annotated.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_ANNOTATED_H
#define PHPGIT2_ANNOTATED_H
#include "stubs/annotated_arginfo.h"

namespace php_git2
{

    // Explicitly specialize git2_resource destructor for git_annotated_commit.
    template<> php_git_annotated_commit::~git2_resource()
    {
        git_annotated_commit_free(handle);
    }

} // namespace php_git2

// Functions:

static constexpr auto ZIF_git_annotated_commit_from_fetchhead = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_annotated_commit**,
        git_repository*,
        const char*,
        const char*,
        const git_oid*>::func<git_annotated_commit_from_fetchhead>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_annotated_commit>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_string,
        php_git2::php_git_oid_fromstr
        >,
    php_git2::sequence<0,1>, // Make the annotated commit dependent on the repository
    1,
    php_git2::sequence<1,2,3,4>,
    php_git2::sequence<0,1,2,3,4>
    >;

static constexpr auto ZIF_git_annotated_commit_from_ref = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_annotated_commit**,
        git_repository*,
        const git_reference*>::func<git_annotated_commit_from_ref>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_annotated_commit>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_reference>
        >,
    php_git2::sequence<0,1>, // Make the annotated commit dependent on the repository
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_annotated_commit_from_revspec = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_annotated_commit**,
        git_repository*,
        const char*>::func<git_annotated_commit_from_revspec>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_annotated_commit>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    php_git2::sequence<0,1>, // Make the annotated commit dependent on the repository
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_annotated_commit_lookup = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_annotated_commit**,
        git_repository*,
        const git_oid*>::func<git_annotated_commit_lookup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_annotated_commit>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_git_oid_fromstr
        >,
    php_git2::sequence<0,1>, // Make the annotated commit dependent on the repository
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_annotated_commit_id = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        const git_annotated_commit*>::func<git_annotated_commit_id>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_annotated_commit>
        >,
    0
    >;

static constexpr auto ZIF_git_annotated_commit_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_annotated_commit>
        >
    >;

// Function entries:

#define GIT_ANNOTATED_FE                                \
    PHP_GIT2_FE(git_annotated_commit_from_fetchhead)    \
    PHP_GIT2_FE(git_annotated_commit_from_ref)          \
    PHP_GIT2_FE(git_annotated_commit_from_revspec)      \
    PHP_GIT2_FE(git_annotated_commit_lookup)            \
    PHP_GIT2_FE(git_annotated_commit_id)                \
    PHP_GIT2_FE(git_annotated_commit_free)

#endif

/*
 * Local Variables:11
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
