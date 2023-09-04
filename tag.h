/*
 * tag.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_TAG_H
#define PHPGIT2_TAG_H

#include "stubs/tag_arginfo.h"

namespace php_git2
{

    // Explicitly specialize git2_resource destructor for git_tag.
    template<> php_git_tag::~git2_resource()
    {
        git_tag_free(handle);
    }

} // namespace php_git2

// Functions:

static constexpr auto ZIF_git_tag_annotation_create = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_repository*,
        const char*,
        const git_object*,
        const git_signature*,
        const char*>::func<git_tag_annotation_create>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_resource<php_git2::php_git_object>,
        php_git2::php_resource<php_git2::php_git_signature>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2,3,4,5>,
    php_git2::sequence<0,1,2,3,4,5>
    >;

static constexpr auto ZIF_git_tag_create = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_repository*,
        const char*,
        const git_object*,
        const git_signature*,
        const char*,
        int>::func<git_tag_create>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_resource<php_git2::php_git_object>,
        php_git2::php_resource<php_git2::php_git_signature>,
        php_git2::php_string,
        php_git2::php_bool
        >,
    1,
    php_git2::sequence<1,2,3,4,5,6>,
    php_git2::sequence<0,1,2,3,4,5,6>
    >;

static constexpr auto ZIF_git_tag_create_frombuffer = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_repository*,
        const char*,
        int>::func<git_tag_create_frombuffer>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_bool
        >,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_tag_create_lightweight = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_repository*,
        const char*,
        const git_object*,
        int>::func<git_tag_create_lightweight>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_resource<php_git2::php_git_object>,
        php_git2::php_bool
        >,
    1,
    php_git2::sequence<1,2,3,4>,
    php_git2::sequence<0,1,2,3,4>
    >;

static constexpr auto ZIF_git_tag_lookup = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_tag**,
        git_repository*,
        const git_oid*>::func<git_tag_lookup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_tag>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_git_oid_fromstr
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_tag_lookup_prefix = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_tag**,
        git_repository*,
        const git_oid*,
        size_t>::func<git_tag_lookup_prefix>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_tag>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::connector_wrapper<php_git2::php_string_length_connector<
                                        size_t,php_git2::php_git_oid_fromstr> >,
        php_git2::php_git_oid_fromstr
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,3>,
    php_git2::sequence<0,1,3,2>
    >;

static constexpr auto ZIF_git_tag_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_tag>
        >
    >;

static constexpr auto ZIF_git_tag_id = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        const git_tag*>::func<git_tag_id>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tag>
        >,
    0
    >;

static constexpr auto ZIF_git_tag_target_id = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        const git_tag*>::func<git_tag_target_id>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tag>
        >,
    0
    >;

static constexpr auto ZIF_git_tag_message = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_tag*>::func<git_tag_message>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tag>
        >,
    0
    >;

static constexpr auto ZIF_git_tag_name = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_tag*>::func<git_tag_name>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tag>
        >,
    0
    >;

static constexpr auto ZIF_git_tag_owner = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_repository*,
        const git_tag*>::func<git_tag_owner>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tag>,
        php_git2::php_resource_ref<php_git2::php_git_repository_nofree>
        >,
    php_git2::php_owner_rethandler<php_git2::php_git_tag>,
    php_git2::sequence<0>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_git_tag_peel = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_object**,
        const git_tag*>::func<git_tag_peel>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_object>,
        php_git2::php_resource<php_git2::php_git_tag>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_tag_target = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_object**,
        const git_tag*>::func<git_tag_target>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_object>,
        php_git2::php_resource<php_git2::php_git_tag>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_tag_tagger = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        const git_signature*,
        const git_tag*>::func<git_tag_tagger>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tag>
        >,
    php_git2::php_resource_nullable_rethandler<
        php_git2::php_git_signature_nofree,
        const git_signature
        >
    >;

static constexpr auto ZIF_git_tag_target_type = zif_php_git2_function<
    php_git2::func_wrapper<
        git_object_t,
        const git_tag*>::func<git_tag_target_type>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tag>
        >,
    0
    >;

static constexpr auto ZIF_git_tag_delete = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*>::func<git_tag_delete>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_tag_dup = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_tag**,
        git_tag*>::func<git_tag_dup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_tag>,
        php_git2::php_resource<php_git2::php_git_tag>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_tag_list = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_strarray*,
        git_repository*>::func<git_tag_list>,
    php_git2::local_pack<
        php_git2::php_git_strarray,
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_tag_list_match = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_strarray*,
        const char*,
        git_repository*>::func<git_tag_list_match>,
    php_git2::local_pack<
        php_git2::php_git_strarray,
        php_git2::php_string,
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_tag_foreach = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        git_tag_foreach_cb,
        void*>::func<git_tag_foreach>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_callback_handler<php_git2::tag_foreach_callback>,
        php_git2::php_callback_sync
        >,
    -1,
    php_git2::sequence<0,2,2>, // pass callback in twice for function and payload
    php_git2::sequence<0,1,2>
    >;

// Function Entries:

#define GIT_TAG_FE                              \
    PHP_GIT2_FE(git_tag_annotation_create)      \
    PHP_GIT2_FE(git_tag_create)                 \
    PHP_GIT2_FE(git_tag_create_frombuffer)      \
    PHP_GIT2_FE(git_tag_create_lightweight)     \
    PHP_GIT2_FE(git_tag_lookup)                 \
    PHP_GIT2_FE(git_tag_lookup_prefix)          \
    PHP_GIT2_FE(git_tag_free)                   \
    PHP_GIT2_FE(git_tag_id)                     \
    PHP_GIT2_FE(git_tag_target_id)              \
    PHP_GIT2_FE(git_tag_message)                \
    PHP_GIT2_FE(git_tag_name)                   \
    PHP_GIT2_FE(git_tag_owner)                  \
    PHP_GIT2_FE(git_tag_peel)                   \
    PHP_GIT2_FE(git_tag_target)                 \
    PHP_GIT2_FE(git_tag_tagger)                 \
    PHP_GIT2_FE(git_tag_target_type)            \
    PHP_GIT2_FE(git_tag_delete)                 \
    PHP_GIT2_FE(git_tag_dup)                    \
    PHP_GIT2_FE(git_tag_list)                   \
    PHP_GIT2_FE(git_tag_list_match)             \
    PHP_GIT2_FE(git_tag_foreach)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
