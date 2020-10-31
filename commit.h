/*
 * commit.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_COMMIT_H
#define PHPGIT2_COMMIT_H
#include <git2/sys/commit.h>

namespace php_git2
{

    // Explicitly specialize git2_resource destructor for git_commit.

    template<> php_git_commit::~git2_resource()
    {
        git_commit_free(handle);
    }

    // Provide a type to manage arrays of git_commits.

    using php_git_commit_array = php_resource_array<php_git_commit>;

    using php_git_commit_array_length_connector = php_git2::connector_wrapper<
        php_git2::php_array_length_connector<
            size_t,
            php_git2::php_git_commit_array
            >
        >;

} // php_git2

// Functions:

static constexpr auto ZIF_GIT_COMMIT_LOOKUP = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_commit**,
        git_repository*,
        const git_oid*>::func<git_commit_lookup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_commit>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_git_oid_fromstr
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_GIT_COMMIT_LOOKUP_PREFIX = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_commit**,
        git_repository*,
        const git_oid*,
        size_t>::func<git_commit_lookup_prefix>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_commit>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::connector_wrapper<
            php_git2::php_string_length_connector<size_t,php_git2::php_git_oid_fromstr> >,
        php_git2::php_git_oid_fromstr
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,3>,
    php_git2::sequence<0,1,3,2>
    >;

static constexpr auto ZIF_GIT_COMMIT_PARENTCOUNT = zif_php_git2_function<
    php_git2::func_wrapper<
        unsigned int,
        const git_commit*>::func<git_commit_parentcount>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_commit>
        >,
    0
    >;

static constexpr auto ZIF_GIT_COMMIT_PARENT_ID = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        const git_commit*,
        unsigned int>::func<git_commit_parent_id>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_commit>,
        php_git2::php_long
        >,
    0
    >;

static constexpr auto ZIF_GIT_COMMIT_PARENT = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_commit**,
        const git_commit*,
        unsigned int>::func<git_commit_parent>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_commit>,
        php_git2::php_resource<php_git2::php_git_commit>,
        php_git2::php_long
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_GIT_COMMIT_NTH_GEN_ANCESTOR = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_commit**,
        const git_commit*,
        unsigned int>::func<git_commit_nth_gen_ancestor>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_commit>,
        php_git2::php_resource<php_git2::php_git_commit>,
        php_git2::php_long
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_GIT_COMMIT_ID = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        const git_commit*>::func<git_commit_id>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_commit>
        >,
    0
    >;

static constexpr auto ZIF_GIT_COMMIT_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_commit>
        >
    >;

static constexpr auto ZIF_GIT_COMMIT_OWNER = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_repository*,
        const git_commit*>::func<git_commit_owner>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_commit>,
        php_git2::php_resource_ref<php_git2::php_git_repository_nofree>
        >,
    php_git2::php_owner_rethandler<php_git2::php_git_commit>,
    php_git2::sequence<0>,
    php_git2::sequence<0>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_GIT_COMMIT_TREE = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_tree**,
        const git_commit*
        >::func<git_commit_tree>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_tree>,
        php_git2::php_resource<php_git2::php_git_commit>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_GIT_COMMIT_TREE_ID = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        const git_commit*
        >::func<git_commit_tree_id>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_commit>
        >,
    0
    >;

static constexpr auto ZIF_GIT_COMMIT_TIME = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_time_t,
        const git_commit*>::func<git_commit_time>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_commit>
        >,
    php_git2::php_numeric_rethandler<git_time_t>
    >;

static constexpr auto ZIF_GIT_COMMIT_TIME_OFFSET = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_commit*>::func<git_commit_time_offset>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_commit>
        >,
    php_git2::php_numeric_rethandler<int>
    >;

static constexpr auto ZIF_GIT_COMMIT_SUMMARY = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        git_commit*>::func<git_commit_summary>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_commit>
        >,
    0
    >;

static constexpr auto ZIF_GIT_COMMIT_RAW_HEADER = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_commit*>::func<git_commit_raw_header>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_commit>
        >,
    0
    >;

static constexpr auto ZIF_GIT_COMMIT_BODY = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        git_commit*>::func<git_commit_body>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_commit>
        >,
    0
    >;

static constexpr auto ZIF_GIT_COMMIT_MESSAGE = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_commit*>::func<git_commit_message>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_commit>
        >,
    0
    >;

static constexpr auto ZIF_GIT_COMMIT_MESSAGE_ENCODING = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_commit*>::func<git_commit_message_encoding>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_commit>
        >,
    0
    >;

static constexpr auto ZIF_GIT_COMMIT_MESSAGE_RAW = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_commit*>::func<git_commit_message_raw>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_commit>
        >,
    0
    >;

static constexpr auto ZIF_GIT_COMMIT_AUTHOR = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        const git_signature*,
        const git_commit*>::func<git_commit_author>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_commit>
        >,
    php_git2::php_resource_rethandler<
        php_git2::php_git_signature_nofree,
        const git_signature
        >
    >;

static constexpr auto ZIF_GIT_COMMIT_COMMITTER = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        const git_signature*,
        const git_commit*>::func<git_commit_committer>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_commit>
        >,
    php_git2::php_resource_rethandler<
        php_git2::php_git_signature_nofree,
        const git_signature
        >
    >;

static constexpr auto ZIF_GIT_COMMIT_CREATE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_repository*,
        const char*,
        const git_signature*,
        const git_signature*,
        const char*,
        const char*,
        const git_tree*,
        size_t,
        const git_commit*[]>::func<git_commit_create>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string_nullable,
        php_git2::php_resource<php_git2::php_git_signature>,
        php_git2::php_resource<php_git2::php_git_signature>,
        php_git2::php_string_nullable,
        php_git2::php_string,
        php_git2::php_resource<php_git2::php_git_tree>,
        php_git2::connector_wrapper<
            php_git2::php_array_length_connector<
                size_t,
                php_git2::php_git_commit_array
                >
            >,
        php_git2::php_git_commit_array
        >,
    1, // Return the git_oid string
    php_git2::sequence<1,2,3,4,5,6,7,9>,
    php_git2::sequence<0,1,2,3,4,5,6,7,8,9>
    >;

static constexpr auto ZIF_GIT_COMMIT_CREATE_BUFFER = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        git_repository*,
        const git_signature*,
        const git_signature*,
        const char*,
        const char*,
        const git_tree*,
        size_t,
        const git_commit*[]>::func<git_commit_create_buffer>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_signature>,
        php_git2::php_resource<php_git2::php_git_signature>,
        php_git2::php_string_nullable,
        php_git2::php_string,
        php_git2::php_resource<php_git2::php_git_tree>,
        php_git2::connector_wrapper<
            php_git2::php_array_length_connector<
                size_t,
                php_git2::php_git_commit_array
                >
            >,
        php_git2::php_git_commit_array
        >,
    1, // Return the git_buf string
    php_git2::sequence<1,2,3,4,5,6,8>,
    php_git2::sequence<0,1,2,3,4,5,6,7,8>
    >;

static constexpr auto ZIF_GIT_COMMIT_CREATE_FROM_CALLBACK = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_repository*,
        const char*,
        const git_signature*,
        const git_signature*,
        const char*,
        const char*,
        const git_oid*,
        git_commit_parent_callback,
        void*>::func<git_commit_create_from_callback>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string_nullable,
        php_git2::php_resource<php_git2::php_git_signature>,
        php_git2::php_resource<php_git2::php_git_signature>,
        php_git2::php_string_nullable,
        php_git2::php_string,
        php_git2::php_git_oid_fromstr,
        php_git2::php_callback_handler<php_git2::commit_parent_callback>,
        php_git2::commit_parent_callback::sync_callback
        >,
    1, // Return the git_oid string
    php_git2::sequence<1,2,3,4,5,6,7,9,9>, // pass callback twice: for callable and payload
    php_git2::sequence<0,1,2,3,4,5,6,7,8,9>
    >;

static constexpr auto ZIF_GIT_COMMIT_CREATE_FROM_IDS = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_repository*,
        const char*,
        const git_signature*,
        const git_signature*,
        const char*,
        const char*,
        const git_oid*,
        size_t,
        const git_oid*[]>::func<git_commit_create_from_ids>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string_nullable,
        php_git2::php_resource<php_git2::php_git_signature>,
        php_git2::php_resource<php_git2::php_git_signature>,
        php_git2::php_string_nullable,
        php_git2::php_string,
        php_git2::php_git_oid_fromstr,
        php_git2::connector_wrapper<
            php_git2::php_array_length_connector<
                size_t,
                php_git2::php_oid_array
                >
            >,
        php_git2::php_oid_array
        >,
    1, // Return the git_oid string
    php_git2::sequence<1,2,3,4,5,6,7,9>,
    php_git2::sequence<0,1,2,3,4,5,6,7,8,9>
    >;

static constexpr auto ZIF_GIT_COMMIT_CREATE_WITH_SIGNATURE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_repository*,
        const char*,
        const char*,
        const char*>::func<git_commit_create_with_signature>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_string,
        php_git2::php_string
        >,
    1, // Return the git_oid string
    php_git2::sequence<1,2,3,4>,
    php_git2::sequence<0,1,2,3,4>
    >;

static constexpr auto ZIF_GIT_COMMIT_AMEND = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        const git_commit*,
        const char*,
        const git_signature*,
        const git_signature*,
        const char*,
        const char*,
        const git_tree*>::func<git_commit_amend>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_commit>,
        php_git2::php_string_nullable,
        php_git2::php_resource_nullable<php_git2::php_git_signature>,
        php_git2::php_resource_nullable<php_git2::php_git_signature>,
        php_git2::php_string_nullable,
        php_git2::php_string_nullable,
        php_git2::php_resource_nullable<php_git2::php_git_tree>
        >,
    1, // Return the git_oid string
    php_git2::sequence<1,2,3,4,5,6,7>,
    php_git2::sequence<0,1,2,3,4,5,6,7>
    >;

static constexpr auto ZIF_GIT_COMMIT_DUP = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_commit**,
        git_commit*
        >::func<git_commit_dup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_commit>,
        php_git2::php_resource<php_git2::php_git_commit>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_GIT_COMMIT_EXTRACT_SIGNATURE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        git_buf*,
        git_repository*,
        git_oid*,
        const char*>::func<git_commit_extract_signature>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_git_buf_out,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_git_oid_fromstr,
        php_git2::php_string_nullable
        >,
    1,
    php_git2::sequence<1,2,3,4>,
    php_git2::sequence<0,1,2,3,4>
    >;
ZEND_BEGIN_ARG_INFO_EX(git_commit_extract_signature_arginfo,0,0,4)
    ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

static constexpr auto ZIF_GIT_COMMIT_HEADER_FIELD = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        const git_commit*,
        const char*>::func<git_commit_header_field>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_resource<php_git2::php_git_commit>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

// Function Entries:

#define GIT_COMMIT_FE                                                   \
    PHP_GIT2_FE(git_commit_lookup,ZIF_GIT_COMMIT_LOOKUP,NULL)           \
    PHP_GIT2_FE(git_commit_lookup_prefix,ZIF_GIT_COMMIT_LOOKUP_PREFIX,NULL) \
    PHP_GIT2_FE(git_commit_parentcount,ZIF_GIT_COMMIT_PARENTCOUNT,NULL) \
    PHP_GIT2_FE(git_commit_parent_id,ZIF_GIT_COMMIT_PARENT_ID,NULL)     \
    PHP_GIT2_FE(git_commit_parent,ZIF_GIT_COMMIT_PARENT,NULL)           \
    PHP_GIT2_FE(git_commit_nth_gen_ancestor,ZIF_GIT_COMMIT_NTH_GEN_ANCESTOR,NULL) \
    PHP_GIT2_FE(git_commit_id,ZIF_GIT_COMMIT_ID,NULL)                   \
    PHP_GIT2_FE(git_commit_free,ZIF_GIT_COMMIT_FREE,NULL)               \
    PHP_GIT2_FE(git_commit_owner,ZIF_GIT_COMMIT_OWNER,NULL)             \
    PHP_GIT2_FE(git_commit_tree,ZIF_GIT_COMMIT_TREE,NULL)               \
    PHP_GIT2_FE(git_commit_tree_id,ZIF_GIT_COMMIT_TREE_ID,NULL)         \
    PHP_GIT2_FE(git_commit_time,ZIF_GIT_COMMIT_TIME,NULL)               \
    PHP_GIT2_FE(git_commit_time_offset,ZIF_GIT_COMMIT_TIME_OFFSET,NULL) \
    PHP_GIT2_FE(git_commit_summary,ZIF_GIT_COMMIT_SUMMARY,NULL)         \
    PHP_GIT2_FE(git_commit_raw_header,ZIF_GIT_COMMIT_RAW_HEADER,NULL)   \
    PHP_GIT2_FE(git_commit_body,ZIF_GIT_COMMIT_BODY,NULL)               \
    PHP_GIT2_FE(git_commit_message,ZIF_GIT_COMMIT_MESSAGE,NULL)         \
    PHP_GIT2_FE(git_commit_message_encoding,ZIF_GIT_COMMIT_MESSAGE_ENCODING,NULL) \
    PHP_GIT2_FE(git_commit_message_raw,ZIF_GIT_COMMIT_MESSAGE_RAW,NULL) \
    PHP_GIT2_FE(git_commit_author,ZIF_GIT_COMMIT_AUTHOR,NULL)           \
    PHP_GIT2_FE(git_commit_committer,ZIF_GIT_COMMIT_COMMITTER,NULL)     \
    PHP_GIT2_FE(git_commit_create,ZIF_GIT_COMMIT_CREATE,NULL)           \
    PHP_GIT2_FE(git_commit_create_buffer,ZIF_GIT_COMMIT_CREATE_BUFFER,NULL) \
    PHP_GIT2_FE(git_commit_create_with_signature,ZIF_GIT_COMMIT_CREATE_WITH_SIGNATURE,NULL) \
    PHP_GIT2_FE(git_commit_create_from_callback,ZIF_GIT_COMMIT_CREATE_FROM_CALLBACK,NULL) \
    PHP_GIT2_FE(git_commit_create_from_ids,ZIF_GIT_COMMIT_CREATE_FROM_IDS,NULL) \
    PHP_GIT2_FE(git_commit_amend,ZIF_GIT_COMMIT_AMEND,NULL)             \
    PHP_GIT2_FE(git_commit_dup,ZIF_GIT_COMMIT_DUP,NULL)                 \
    PHP_GIT2_FE(git_commit_extract_signature,ZIF_GIT_COMMIT_EXTRACT_SIGNATURE,git_commit_extract_signature_arginfo) \
    PHP_GIT2_FE(git_commit_header_field,ZIF_GIT_COMMIT_HEADER_FIELD,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
