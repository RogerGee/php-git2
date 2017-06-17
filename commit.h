/*
 * commit.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_COMMIT_H
#define PHPGIT2_COMMIT_H

namespace php_git2
{

    // Explicitly specialize git2_resource destructor for git_commit.
    template<> php_git_commit::~git2_resource()
    {
        git_commit_free(handle);
    }

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
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
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
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
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
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
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
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
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

// Function Entries:

#define GIT_COMMIT_FE                                                   \
    PHP_GIT2_FE(git_commit_lookup,ZIF_GIT_COMMIT_LOOKUP,NULL)           \
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
    PHP_GIT2_FE(git_commit_message_raw,ZIF_GIT_COMMIT_MESSAGE_RAW,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
