/*
 * reference.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_REFERENCE_H
#define PHPGIT2_REFERENCE_H

extern "C" {
#include <git2/sys/refs.h>
}

namespace php_git2
{
    // Explicitly specialize git2_resource destructor for git_reference.
    template<> php_git_reference::~git2_resource()
    {
        git_reference_free(handle);
    }

    // Explicitly specialize git2_resource destructor for git_reference_iterator.
    template<> php_git_reference_iterator::~git2_resource()
    {
        git_reference_iterator_free(handle);
    }

} // namespace php_git2

// Functions:

static constexpr auto ZIF_git_reference_list = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_strarray*,
        git_repository*>::func<git_reference_list>,
    php_git2::local_pack<
        php_git2::php_git_strarray,
        php_git2::php_resource<php_git2::php_git_repository> >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_reference_lookup = zif_php_git2_function_setdeps<
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
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_reference_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_reference> > >;

static constexpr auto ZIF_git_reference_peel = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_object**,
        const git_reference*,
        git_object_t>::func<git_reference_peel>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_object>,
        php_git2::php_resource<php_git2::php_git_reference>,
        php_git2::php_long_cast<git_object_t> >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_reference_name_to_id = zif_php_git2_function<
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
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_reference_create = zif_php_git2_function_setdeps<
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
    php_git2::sequence<0,1,2,3,4,5>
    >;

static constexpr auto ZIF_git_reference_create_matching = zif_php_git2_function_setdeps<
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
    php_git2::sequence<0,1,2,3,4,5,6>
    >;

static constexpr auto ZIF_git_reference_target = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        const git_reference*>::func<git_reference_target>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference> >,
    0
    >;

static constexpr auto ZIF_git_reference_target_peel = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        const git_reference*>::func<git_reference_target_peel>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference> >,
    0
    >;

static constexpr auto ZIF_git_reference_name = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_reference*>::func<git_reference_name>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference> >,
    0
    >;

static constexpr auto ZIF_git_reference_cmp = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const git_reference*,
        const git_reference*>::func<git_reference_cmp>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference>,
        php_git2::php_resource<php_git2::php_git_reference>
        >,
    0
    >;

static constexpr auto ZIF_git_reference__alloc = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_reference*,
        const char*,
        const git_oid*,
        const git_oid*>::func<git_reference__alloc>,
    php_git2::local_pack<
        php_git2::php_string,
        php_git2::php_git_oid_fromstr,
        php_git2::php_git_oid_fromstr_nullable
        >,
    php_git2::php_resource_nodeps_rethandler<
        php_git2::php_git_reference,
        const git_reference
        >
    >;

static constexpr auto ZIF_git_reference__alloc_symbolic = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_reference*,
        const char*,
        const char*>::func<git_reference__alloc_symbolic>,
    php_git2::local_pack<
        php_git2::php_string,
        php_git2::php_string
        >,
    php_git2::php_resource_nodeps_rethandler<
        php_git2::php_git_reference,
        const git_reference
        >
    >;

static constexpr auto ZIF_git_reference_delete = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_reference*>::func<git_reference_delete>,
    php_git2::local_pack<
        // The git_reference is invalidated and must be cleaned up.
        php_git2::php_resource_cleanup_delayed<php_git2::php_git_reference>
        >
    >;

static constexpr auto ZIF_git_reference_dup = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_reference**,
        git_reference*>::func<git_reference_dup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_reference>,
        php_git2::php_resource<php_git2::php_git_reference>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_reference_dwim = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_reference**,
        git_repository*,
        const char*>::func<git_reference_dwim>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_reference>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_reference_ensure_log = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*>::func<git_reference_ensure_log>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_reference_has_log = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*>::func<git_reference_has_log>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    php_git2::php_boolean_error_rethandler<int>
    >;

static constexpr auto ZIF_git_reference_is_branch = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_reference*>::func<git_reference_is_branch>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_git_reference_is_note = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_reference*>::func<git_reference_is_note>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_git_reference_is_remote = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_reference*>::func<git_reference_is_remote>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_git_reference_is_tag = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_reference*>::func<git_reference_is_tag>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_git_reference_is_valid_name = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const char*>::func<git_reference_is_valid_name>,
    php_git2::local_pack<
        php_git2::php_string
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_git_reference_iterator_new = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_reference_iterator**,
        git_repository*>::func<git_reference_iterator_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_reference_iterator>,
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_reference_iterator_glob_new = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_reference_iterator**,
        git_repository*,
        const char*>::func<git_reference_iterator_glob_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_reference_iterator>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_reference_iterator_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_reference_iterator>
        >
    >;

static constexpr auto ZIF_git_reference_next = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_reference**,
        git_reference_iterator*>::func<git_reference_next>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_reference>,
        php_git2::php_resource<php_git2::php_git_reference_iterator>
        >,
    php_git2::php_resource_iterover_rethandler<0,php_git2::sequence<0,1> >,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_reference_next_name = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const char**,
        git_reference_iterator*>::func<git_reference_next_name>,
    php_git2::local_pack<
        php_git2::php_string_ref,
        php_git2::php_resource<php_git2::php_git_reference_iterator>
        >,
    php_git2::php_iterover_rethandler<0>,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_reference_normalize_name = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        char*,
        size_t,
        const char*,
        unsigned int>::func<git_reference_normalize_name>,
    php_git2::local_pack<
        php_git2::php_fixed_buffer<1024>,
        php_git2::php_constant<size_t,1024>,
        php_git2::php_string,
        php_git2::php_long_cast<unsigned int>
        >,
    1,
    php_git2::sequence<2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_reference_owner = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_repository*,
        const git_reference*>::func<git_reference_owner>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference>,
        php_git2::php_resource_ref<php_git2::php_git_repository_nofree>
        >,
    php_git2::php_owner_rethandler<php_git2::php_git_reference>,
    php_git2::sequence<0>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_git_reference_remove = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*>::func<git_reference_remove>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_reference_rename = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_reference**,
        git_reference*,
        const char*,
        int,
        const char*>::func<git_reference_rename>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_reference>,
        php_git2::php_resource<php_git2::php_git_reference>,
        php_git2::php_string,
        php_git2::php_bool,
        php_git2::php_string
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2,3,4>,
    php_git2::sequence<0,1,2,3,4>
    >;

static constexpr auto ZIF_git_reference_resolve = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_reference**,
        const git_reference*>::func<git_reference_resolve>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_reference>,
        php_git2::php_resource<php_git2::php_git_reference>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_reference_set_target = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_reference**,
        git_reference*,
        const git_oid*,
        const char*>::func<git_reference_set_target>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_reference>,
        php_git2::php_resource<php_git2::php_git_reference>,
        php_git2::php_git_oid_fromstr,
        php_git2::php_string
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_reference_shorthand = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_reference*>::func<git_reference_shorthand>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference>
        >,
    0
    >;

static constexpr auto ZIF_git_reference_type = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_ref_t,
        const git_reference*>::func<git_reference_type>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference>
        >,
    php_git2::php_numeric_rethandler<git_ref_t>
    >;

static constexpr auto ZIF_git_reference_foreach = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        git_reference_foreach_cb,
        void*>::func<git_reference_foreach>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_callback_handler<php_git2::reference_foreach_callback>,
        php_git2::php_callback_sync
        >,
    -1,
    php_git2::sequence<0,2,2>, // pass callback in twice for function and payload
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_reference_foreach_name = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        git_reference_foreach_name_cb,
        void*>::func<git_reference_foreach_name>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_callback_handler<php_git2::reference_foreach_name_callback>,
        php_git2::php_callback_sync
        >,
    -1,
    php_git2::sequence<0,2,2>, // pass callback in twice for function and payload
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_reference_foreach_glob = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*,
        git_reference_foreach_name_cb,
        void*>::func<git_reference_foreach_glob>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_callback_handler<php_git2::reference_foreach_name_callback>,
        php_git2::php_callback_sync
        >,
    -1,
    php_git2::sequence<0,1,3,3>, // pass callback in twice for function and payload
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_reference_symbolic_create = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_reference**,
        git_repository*,
        const char*,
        const char*,
        int,
        const char*>::func<git_reference_symbolic_create>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_reference>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_string,
        php_git2::php_bool,
        php_git2::php_string
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2,3,4,5>,
    php_git2::sequence<0,1,2,3,4,5>
    >;

static constexpr auto ZIF_git_reference_symbolic_create_matching = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_reference**,
        git_repository*,
        const char*,
        const char*,
        int,
        const char*,
        const char*>::func<git_reference_symbolic_create_matching>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_reference>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_string,
        php_git2::php_bool,
        php_git2::php_string,
        php_git2::php_string
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2,3,4,5,6>,
    php_git2::sequence<0,1,2,3,4,5,6>
    >;

static constexpr auto ZIF_git_reference_symbolic_set_target = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_reference**,
        git_reference*,
        const char*,
        const char*>::func<git_reference_symbolic_set_target>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_reference>,
        php_git2::php_resource<php_git2::php_git_reference>,
        php_git2::php_string,
        php_git2::php_string
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_reference_symbolic_target = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_reference*>::func<git_reference_symbolic_target>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference>
        >,
    0
    >;

// Function Entries:

#define GIT_REFERENCE_FE                                \
    PHP_GIT2_FE(git_reference_list)                     \
    PHP_GIT2_FE(git_reference_lookup)                   \
    PHP_GIT2_FE(git_reference_free)                     \
    PHP_GIT2_FE(git_reference_peel)                     \
    PHP_GIT2_FE(git_reference_name_to_id)               \
    PHP_GIT2_FE(git_reference_create)                   \
    PHP_GIT2_FE(git_reference_create_matching)          \
    PHP_GIT2_FE(git_reference_target)                   \
    PHP_GIT2_FE(git_reference_target_peel)              \
    PHP_GIT2_FE(git_reference_name)                     \
    PHP_GIT2_FE(git_reference_cmp)                      \
    PHP_GIT2_FE(git_reference__alloc)                   \
    PHP_GIT2_FE(git_reference__alloc_symbolic)          \
    PHP_GIT2_FE(git_reference_delete)                   \
    PHP_GIT2_FE(git_reference_dup)                      \
    PHP_GIT2_FE(git_reference_dwim)                     \
    PHP_GIT2_FE(git_reference_ensure_log)               \
    PHP_GIT2_FE(git_reference_has_log)                  \
    PHP_GIT2_FE(git_reference_is_branch)                \
    PHP_GIT2_FE(git_reference_is_note)                  \
    PHP_GIT2_FE(git_reference_is_remote)                \
    PHP_GIT2_FE(git_reference_is_tag)                   \
    PHP_GIT2_FE(git_reference_is_valid_name)            \
    PHP_GIT2_FE(git_reference_iterator_free)            \
    PHP_GIT2_FE(git_reference_iterator_new)             \
    PHP_GIT2_FE(git_reference_iterator_glob_new)        \
    PHP_GIT2_FE(git_reference_next)                     \
    PHP_GIT2_FE(git_reference_next_name)                \
    PHP_GIT2_FE(git_reference_normalize_name)           \
    PHP_GIT2_FE(git_reference_owner)                    \
    PHP_GIT2_FE(git_reference_remove)                   \
    PHP_GIT2_FE(git_reference_rename)                   \
    PHP_GIT2_FE(git_reference_resolve)                  \
    PHP_GIT2_FE(git_reference_set_target)               \
    PHP_GIT2_FE(git_reference_shorthand)                \
    PHP_GIT2_FE(git_reference_type)                     \
    PHP_GIT2_FE(git_reference_foreach)                  \
    PHP_GIT2_FE(git_reference_foreach_name)             \
    PHP_GIT2_FE(git_reference_foreach_glob)             \
    PHP_GIT2_FE(git_reference_symbolic_create)          \
    PHP_GIT2_FE(git_reference_symbolic_create_matching) \
    PHP_GIT2_FE(git_reference_symbolic_set_target)      \
    PHP_GIT2_FE(git_reference_symbolic_target)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
