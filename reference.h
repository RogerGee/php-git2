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

static constexpr auto ZIF_GIT_REFERENCE_FREE = zif_php_git2_function_free<
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

static constexpr auto ZIF_GIT_REFERENCE_TARGET = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        const git_reference*>::func<git_reference_target>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference> >,
    0>;

static constexpr auto ZIF_GIT_REFERENCE_TARGET_PEEL = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        const git_reference*>::func<git_reference_target_peel>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference> >,
    0>;

static constexpr auto ZIF_GIT_REFERENCE_NAME = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_reference*>::func<git_reference_name>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference> >,
    0>;

static constexpr auto ZIF_GIT_REFERENCE_CMP = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_reference*,
        const git_reference*>::func<git_reference_cmp>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference>,
        php_git2::php_resource<php_git2::php_git_reference>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_GIT_REFERENCE__ALLOC = zif_php_git2_function_rethandler<
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

static constexpr auto ZIF_GIT_REFERENCE__ALLOC_SYMBOLIC = zif_php_git2_function_rethandler<
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

static constexpr auto ZIF_GIT_REFERENCE_DELETE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_reference*>::func<git_reference_delete>,
    php_git2::local_pack<
        // The git_reference is invalidated and must be cleaned up.
        php_git2::php_resource_cleanup_delayed<php_git2::php_git_reference>
        >
    >;

static constexpr auto ZIF_GIT_REFERENCE_DUP = zif_php_git2_function_setdeps<
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
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

static constexpr auto ZIF_GIT_REFERENCE_DWIM = zif_php_git2_function_setdeps<
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
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
    >;

static constexpr auto ZIF_GIT_REFERENCE_ENSURE_LOG = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*>::func<git_reference_ensure_log>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_GIT_REFERENCE_HAS_LOG = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*>::func<git_reference_has_log>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_GIT_REFERENCE_IS_BRANCH = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_reference*>::func<git_reference_is_branch>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_GIT_REFERENCE_IS_NOTE = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_reference*>::func<git_reference_is_note>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_GIT_REFERENCE_IS_REMOTE = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_reference*>::func<git_reference_is_remote>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_GIT_REFERENCE_IS_TAG = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_reference*>::func<git_reference_is_tag>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_GIT_REFERENCE_IS_VALID_NAME = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const char*>::func<git_reference_is_valid_name>,
    php_git2::local_pack<
        php_git2::php_string
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_GIT_REFERENCE_ITERATOR_NEW = zif_php_git2_function_setdeps<
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
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

static constexpr auto ZIF_GIT_REFERENCE_ITERATOR_GLOB_NEW = zif_php_git2_function_setdeps<
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
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
    >;

static constexpr auto ZIF_GIT_REFERENCE_ITERATOR_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_reference_iterator>
        >
    >;

static constexpr auto ZIF_GIT_REFERENCE_NEXT = zif_php_git2_function_rethandler<
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
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

static constexpr auto ZIF_GIT_REFERENCE_NEXT_NAME = zif_php_git2_function_rethandler<
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
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

static constexpr auto ZIF_GIT_REFERENCE_NORMALIZE_NAME = zif_php_git2_function<
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
    php_git2::sequence<0,1,2,3>,
    php_git2::sequence<0,0,1,2>
    >;

static constexpr auto ZIF_GIT_REFERENCE_OWNER = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_repository*,
        const git_reference*>::func<git_reference_owner>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference>,
        php_git2::php_resource_ref<php_git2::php_git_repository_nofree>
        >,
    php_git2::php_owner_rethandler<php_git2::php_git_reference>,
    php_git2::sequence<0>,
    php_git2::sequence<0>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_GIT_REFERENCE_REMOVE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*>::func<git_reference_remove>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_GIT_REFERENCE_RENAME = zif_php_git2_function_setdeps<
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
    php_git2::sequence<0,1,2,3,4>,
    php_git2::sequence<0,0,1,2,3>
    >;

static constexpr auto ZIF_GIT_REFERENCE_RESOLVE = zif_php_git2_function_setdeps<
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
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

static constexpr auto ZIF_GIT_REFERENCE_SET_TARGET = zif_php_git2_function_setdeps<
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
    php_git2::sequence<0,1,2,3>,
    php_git2::sequence<0,0,1,2>
    >;

static constexpr auto ZIF_GIT_REFERENCE_SHORTHAND = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_reference*>::func<git_reference_shorthand>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference>
        >,
    0
    >;

static constexpr auto ZIF_GIT_REFERENCE_TYPE = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_ref_t,
        const git_reference*>::func<git_reference_type>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference>
        >,
    php_git2::php_numeric_rethandler<git_ref_t>
    >;

static constexpr auto ZIF_GIT_REFERENCE_FOREACH = zif_php_git2_function<
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
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_GIT_REFERENCE_FOREACH_NAME = zif_php_git2_function<
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
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_GIT_REFERENCE_FOREACH_GLOB = zif_php_git2_function<
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
    php_git2::sequence<0,1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_GIT_REFERENCE_SYMBOLIC_CREATE = zif_php_git2_function_setdeps<
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
    php_git2::sequence<0,1,2,3,4,5>,
    php_git2::sequence<0,0,1,2,3,4>
    >;

static constexpr auto ZIF_GIT_REFERENCE_SYMBOLIC_CREATE_MATCHING = zif_php_git2_function_setdeps<
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
    php_git2::sequence<0,1,2,3,4,5,6>,
    php_git2::sequence<0,0,1,2,3,4,5>
    >;

static constexpr auto ZIF_GIT_REFERENCE_SYMBOLIC_SET_TARGET = zif_php_git2_function_setdeps<
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
    php_git2::sequence<0,1,2,3>,
    php_git2::sequence<0,0,1,2>
    >;

static constexpr auto ZIF_GIT_REFERENCE_SYMBOLIC_TARGET = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_reference*>::func<git_reference_symbolic_target>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference>
        >,
    0
    >;

// Function Entries:

#define GIT_REFERENCE_FE                                                \
    PHP_GIT2_FE(git_reference_list,ZIF_GIT_REFERENCE_LIST,NULL)         \
    PHP_GIT2_FE(git_reference_lookup,ZIF_GIT_REFERENCE_LOOKUP,NULL)     \
    PHP_GIT2_FE(git_reference_free,ZIF_GIT_REFERENCE_FREE,NULL)         \
    PHP_GIT2_FE(git_reference_peel,ZIF_GIT_REFERENCE_PEEL,NULL)         \
    PHP_GIT2_FE(git_reference_name_to_id,ZIF_GIT_REFERENCE_NAME_TO_ID,NULL) \
    PHP_GIT2_FE(git_reference_create,ZIF_GIT_REFERENCE_CREATE,NULL)     \
    PHP_GIT2_FE(git_reference_create_matching,ZIF_GIT_REFERENCE_CREATE_MATCHING,NULL) \
    PHP_GIT2_FE(git_reference_target,ZIF_GIT_REFERENCE_TARGET,NULL)     \
    PHP_GIT2_FE(git_reference_target_peel,ZIF_GIT_REFERENCE_TARGET_PEEL,NULL) \
    PHP_GIT2_FE(git_reference_name,ZIF_GIT_REFERENCE_NAME,NULL)         \
    PHP_GIT2_FE(git_reference_cmp,ZIF_GIT_REFERENCE_CMP,NULL)           \
    PHP_GIT2_FE(git_reference__alloc,ZIF_GIT_REFERENCE__ALLOC,NULL)     \
    PHP_GIT2_FE(git_reference__alloc_symbolic,ZIF_GIT_REFERENCE__ALLOC_SYMBOLIC,NULL) \
    PHP_GIT2_FE(git_reference_delete,ZIF_GIT_REFERENCE_DELETE,NULL)     \
    PHP_GIT2_FE(git_reference_dup,ZIF_GIT_REFERENCE_DUP,NULL)           \
    PHP_GIT2_FE(git_reference_dwim,ZIF_GIT_REFERENCE_DWIM,NULL)         \
    PHP_GIT2_FE(git_reference_ensure_log,ZIF_GIT_REFERENCE_ENSURE_LOG,NULL) \
    PHP_GIT2_FE(git_reference_has_log,ZIF_GIT_REFERENCE_HAS_LOG,NULL)   \
    PHP_GIT2_FE(git_reference_is_branch,ZIF_GIT_REFERENCE_IS_BRANCH,NULL) \
    PHP_GIT2_FE(git_reference_is_note,ZIF_GIT_REFERENCE_IS_NOTE,NULL)   \
    PHP_GIT2_FE(git_reference_is_remote,ZIF_GIT_REFERENCE_IS_REMOTE,NULL) \
    PHP_GIT2_FE(git_reference_is_tag,ZIF_GIT_REFERENCE_IS_TAG,NULL)     \
    PHP_GIT2_FE(git_reference_is_valid_name,ZIF_GIT_REFERENCE_IS_VALID_NAME,NULL) \
    PHP_GIT2_FE(git_reference_iterator_new,ZIF_GIT_REFERENCE_ITERATOR_NEW,NULL) \
    PHP_GIT2_FE(git_reference_iterator_glob_new,ZIF_GIT_REFERENCE_ITERATOR_GLOB_NEW,NULL) \
    PHP_GIT2_FE(git_reference_next,ZIF_GIT_REFERENCE_NEXT,NULL)         \
    PHP_GIT2_FE(git_reference_next_name,ZIF_GIT_REFERENCE_NEXT_NAME,NULL) \
    PHP_GIT2_FE(git_reference_normalize_name,ZIF_GIT_REFERENCE_NORMALIZE_NAME,NULL) \
    PHP_GIT2_FE(git_reference_owner,ZIF_GIT_REFERENCE_OWNER,NULL)       \
    PHP_GIT2_FE(git_reference_remove,ZIF_GIT_REFERENCE_REMOVE,NULL)     \
    PHP_GIT2_FE(git_reference_rename,ZIF_GIT_REFERENCE_RENAME,NULL)     \
    PHP_GIT2_FE(git_reference_resolve,ZIF_GIT_REFERENCE_RESOLVE,NULL)   \
    PHP_GIT2_FE(git_reference_set_target,ZIF_GIT_REFERENCE_SET_TARGET,NULL) \
    PHP_GIT2_FE(git_reference_shorthand,ZIF_GIT_REFERENCE_SHORTHAND,NULL) \
    PHP_GIT2_FE(git_reference_type,ZIF_GIT_REFERENCE_TYPE,NULL)         \
    PHP_GIT2_FE(git_reference_foreach,ZIF_GIT_REFERENCE_FOREACH,NULL)   \
    PHP_GIT2_FE(git_reference_foreach_name,ZIF_GIT_REFERENCE_FOREACH_NAME,NULL) \
    PHP_GIT2_FE(git_reference_foreach_glob,ZIF_GIT_REFERENCE_FOREACH_GLOB,NULL) \
    PHP_GIT2_FE(git_reference_symbolic_create,ZIF_GIT_REFERENCE_SYMBOLIC_CREATE,NULL) \
    PHP_GIT2_FE(git_reference_symbolic_create_matching,ZIF_GIT_REFERENCE_SYMBOLIC_CREATE_MATCHING,NULL) \
    PHP_GIT2_FE(git_reference_symbolic_set_target,ZIF_GIT_REFERENCE_SYMBOLIC_SET_TARGET,NULL) \
    PHP_GIT2_FE(git_reference_symbolic_target,ZIF_GIT_REFERENCE_SYMBOLIC_TARGET,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
