/*
 * tag.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_TAG_H
#define PHPGIT2_TAG_H

namespace php_git2
{

    // Explicitly specialize git2_resource destructor for git_tag.
    template<> php_git_tag::~git2_resource()
    {
        git_tag_free(handle);
    }

} // namespace php_git2

// Functions:

static constexpr auto ZIF_GIT_TAG_ANNOTATION_CREATE = zif_php_git2_function<
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

static constexpr auto ZIF_GIT_TAG_CREATE = zif_php_git2_function<
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

static constexpr auto ZIF_GIT_TAG_CREATE_FROMBUFFER = zif_php_git2_function<
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

static constexpr auto ZIF_GIT_TAG_CREATE_LIGHTWEIGHT = zif_php_git2_function<
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

static constexpr auto ZIF_GIT_TAG_LOOKUP = zif_php_git2_function_setdeps<
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

static constexpr auto ZIF_GIT_TAG_LOOKUP_PREFIX = zif_php_git2_function_setdeps<
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

static constexpr auto ZIF_GIT_TAG_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_tag>
        >
    >;

static constexpr auto ZIF_GIT_TAG_ID = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        const git_tag*>::func<git_tag_id>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tag>
        >,
    0
    >;

static constexpr auto ZIF_GIT_TAG_TARGET_ID = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        const git_tag*>::func<git_tag_target_id>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tag>
        >,
    0
    >;

static constexpr auto ZIF_GIT_TAG_MESSAGE = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_tag*>::func<git_tag_message>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tag>
        >,
    0
    >;

static constexpr auto ZIF_GIT_TAG_NAME = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_tag*>::func<git_tag_name>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tag>
        >,
    0
    >;

static constexpr auto ZIF_GIT_TAG_OWNER = zif_php_git2_function_rethandler<
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

static constexpr auto ZIF_GIT_TAG_PEEL = zif_php_git2_function_setdeps<
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

static constexpr auto ZIF_GIT_TAG_TARGET = zif_php_git2_function_setdeps<
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

static constexpr auto ZIF_GIT_TAG_TAGGER = zif_php_git2_function_rethandler<
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

static constexpr auto ZIF_GIT_TAG_TARGET_TYPE = zif_php_git2_function<
    php_git2::func_wrapper<
        git_object_t,
        const git_tag*>::func<git_tag_target_type>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tag>
        >,
    0
    >;

static constexpr auto ZIF_GIT_TAG_DELETE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*>::func<git_tag_delete>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_GIT_TAG_DUP = zif_php_git2_function_setdeps<
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

static constexpr auto ZIF_GIT_TAG_LIST = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_strarray*,
        git_repository*>::func<git_tag_list>,
    php_git2::local_pack<
        php_git2::php_strarray,
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_GIT_TAG_LIST_MATCH = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_strarray*,
        const char*,
        git_repository*>::func<git_tag_list_match>,
    php_git2::local_pack<
        php_git2::php_strarray,
        php_git2::php_string,
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_GIT_TAG_FOREACH = zif_php_git2_function<
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

#define GIT_TAG_FE                                                      \
    PHP_GIT2_FE(git_tag_annotation_create,ZIF_GIT_TAG_ANNOTATION_CREATE,NULL) \
    PHP_GIT2_FE(git_tag_create,ZIF_GIT_TAG_CREATE,NULL)                 \
    PHP_GIT2_FE(git_tag_create_frombuffer,ZIF_GIT_TAG_CREATE_FROMBUFFER,NULL) \
    PHP_GIT2_FE(git_tag_create_lightweight,ZIF_GIT_TAG_CREATE_LIGHTWEIGHT,NULL) \
    PHP_GIT2_FE(git_tag_lookup,ZIF_GIT_TAG_LOOKUP,NULL)                 \
    PHP_GIT2_FE(git_tag_lookup_prefix,ZIF_GIT_TAG_LOOKUP_PREFIX,NULL)   \
    PHP_GIT2_FE(git_tag_free,ZIF_GIT_TAG_FREE,NULL)                     \
    PHP_GIT2_FE(git_tag_id,ZIF_GIT_TAG_ID,NULL)                         \
    PHP_GIT2_FE(git_tag_target_id,ZIF_GIT_TAG_TARGET_ID,NULL)           \
    PHP_GIT2_FE(git_tag_message,ZIF_GIT_TAG_MESSAGE,NULL)               \
    PHP_GIT2_FE(git_tag_name,ZIF_GIT_TAG_NAME,NULL)                     \
    PHP_GIT2_FE(git_tag_owner,ZIF_GIT_TAG_OWNER,NULL)                   \
    PHP_GIT2_FE(git_tag_peel,ZIF_GIT_TAG_PEEL,NULL)                     \
    PHP_GIT2_FE(git_tag_target,ZIF_GIT_TAG_TARGET,NULL)                 \
    PHP_GIT2_FE(git_tag_tagger,ZIF_GIT_TAG_TAGGER,NULL)                 \
    PHP_GIT2_FE(git_tag_target_type,ZIF_GIT_TAG_TARGET_TYPE,NULL)       \
    PHP_GIT2_FE(git_tag_delete,ZIF_GIT_TAG_DELETE,NULL)                 \
    PHP_GIT2_FE(git_tag_dup,ZIF_GIT_TAG_DUP,NULL)                       \
    PHP_GIT2_FE(git_tag_list,ZIF_GIT_TAG_LIST,NULL)                     \
    PHP_GIT2_FE(git_tag_list_match,ZIF_GIT_TAG_LIST_MATCH,NULL)         \
    PHP_GIT2_FE(git_tag_foreach,ZIF_GIT_TAG_FOREACH,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
