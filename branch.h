/*
 * branch.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_BRANCH_H
#define PHPGIT2_BRANCH_H

namespace php_git2
{

    // Explicitly specialize git2_resource destructor for git_branch_iterator.
    template<> php_git_branch_iterator::~git2_resource()
    {
        git_branch_iterator_free(handle);
    }

} // namespace php_git2

// Functions:

static constexpr auto ZIF_git_branch_create = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_reference**,
        git_repository*,
        const char*,
        const git_commit*,
        int>::func<git_branch_create>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_reference>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_resource<php_git2::php_git_commit>,
        php_git2::php_bool
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2,3,4>,
    php_git2::sequence<0,1,2,3,4>
    >;

static constexpr auto ZIF_git_branch_create_from_annotated = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_reference**,
        git_repository*,
        const char*,
        const git_annotated_commit*,
        int>::func<git_branch_create_from_annotated>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_reference>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_resource<php_git2::php_git_annotated_commit>,
        php_git2::php_bool
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2,3,4>,
    php_git2::sequence<0,1,2,3,4>
    >;

static constexpr auto ZIF_git_branch_delete = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_reference*>::func<git_branch_delete>,
    php_git2::local_pack<
        // The git_reference is invalidated and must be cleaned up.
        php_git2::php_resource_cleanup_delayed<php_git2::php_git_reference>
        >,
    -1
    >;

static constexpr auto ZIF_git_branch_is_head = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_reference*>::func<git_branch_is_head>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference>
        >,
    php_git2::php_boolean_error_rethandler<int>
    >;

static constexpr auto ZIF_git_branch_iterator_new = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_branch_iterator**,
        git_repository*,
        git_branch_t>::func<git_branch_iterator_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_branch_iterator>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_long_cast<git_branch_t>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_branch_iterator_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_branch_iterator> > >;

static constexpr auto ZIF_git_branch_next = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_reference**,
        git_branch_t*,
        git_branch_iterator*>::func<git_branch_next>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_reference>,
        php_git2::php_long_out<git_branch_t>,
        php_git2::php_resource<php_git2::php_git_branch_iterator>
        >,
    php_git2::php_resource_iterover_rethandler<0,php_git2::sequence<0,2> >,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_branch_lookup = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_reference**,
        git_repository*,
        const char*,
        git_branch_t>::func<git_branch_lookup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_reference>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_long_cast<git_branch_t>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_branch_move = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_reference**,
        git_reference*,
        const char*,
        int>::func<git_branch_move>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_reference>,
        php_git2::php_resource<php_git2::php_git_reference>,
        php_git2::php_string,
        php_git2::php_bool
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_branch_name = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const char**,
        const git_reference*>::func<git_branch_name>,
    php_git2::local_pack<
        php_git2::php_string_ref,
        php_git2::php_resource<php_git2::php_git_reference>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_branch_set_upstream = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_reference*,
        const char*>::func<git_branch_set_upstream>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference>,
        php_git2::php_string_nullable
        >,
    -1
    >;

static constexpr auto ZIF_git_branch_upstream = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_reference**,
        const git_reference*>::func<git_branch_upstream>,
    php_git2::local_pack<
        php_git2::php_resource_nullable_ref<php_git2::php_git_reference>,
        php_git2::php_resource<php_git2::php_git_reference>
        >,
    php_git2::php_resource_notfound_rethandler<0,php_git2::sequence<0,1> >,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_branch_upstream_name = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_buf*,
        git_repository*,
        const char*>::func<git_branch_upstream_name>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    php_git2::php_notfound_rethandler<0>,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_branch_upstream_remote = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        git_repository*,
        const char*>::func<git_branch_upstream_remote>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

// Function Entries:

#define GIT_BRANCH_FE                               \
    PHP_GIT2_FE(git_branch_create)                  \
    PHP_GIT2_FE(git_branch_create_from_annotated)   \
    PHP_GIT2_FE(git_branch_delete)                  \
    PHP_GIT2_FE(git_branch_is_head)                 \
    PHP_GIT2_FE(git_branch_iterator_new)            \
    PHP_GIT2_FE(git_branch_iterator_free)           \
    PHP_GIT2_FE(git_branch_next)                    \
    PHP_GIT2_FE(git_branch_lookup)                  \
    PHP_GIT2_FE(git_branch_move)                    \
    PHP_GIT2_FE(git_branch_name)                    \
    PHP_GIT2_FE(git_branch_set_upstream)            \
    PHP_GIT2_FE(git_branch_upstream)                \
    PHP_GIT2_FE(git_branch_upstream_name)           \
    PHP_GIT2_FE(git_branch_upstream_remote)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
