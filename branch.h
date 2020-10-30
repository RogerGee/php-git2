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

static constexpr auto ZIF_GIT_BRANCH_CREATE = zif_php_git2_function_setdeps<
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

static constexpr auto ZIF_GIT_BRANCH_CREATE_FROM_ANNOTATED = zif_php_git2_function_setdeps<
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

static constexpr auto ZIF_GIT_BRANCH_DELETE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_reference*>::func<git_branch_delete>,
    php_git2::local_pack<
        // The git_reference is invalidated and must be cleaned up.
        php_git2::php_resource_cleanup_delayed<php_git2::php_git_reference>
        >,
    -1
    >;

static constexpr auto ZIF_GIT_BRANCH_IS_HEAD = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_reference*>::func<git_branch_is_head>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reference>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_GIT_BRANCH_ITERATOR_NEW = zif_php_git2_function_setdeps<
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

static constexpr auto ZIF_GIT_BRANCH_ITERATOR_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_branch_iterator> > >;

static constexpr auto ZIF_GIT_BRANCH_NEXT = zif_php_git2_function_rethandler<
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
ZEND_BEGIN_ARG_INFO_EX(git_branch_next_arginfo,0,0,2)
    ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

static constexpr auto ZIF_GIT_BRANCH_LOOKUP = zif_php_git2_function_setdeps<
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

static constexpr auto ZIF_GIT_BRANCH_MOVE = zif_php_git2_function_setdeps<
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

static constexpr auto ZIF_GIT_BRANCH_NAME = zif_php_git2_function<
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

static constexpr auto ZIF_GIT_BRANCH_SET_UPSTREAM = zif_php_git2_function<
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

static constexpr auto ZIF_GIT_BRANCH_UPSTREAM = zif_php_git2_function_rethandler<
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

static constexpr auto ZIF_GIT_BRANCH_UPSTREAM_NAME = zif_php_git2_function_rethandler<
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

static constexpr auto ZIF_GIT_BRANCH_UPSTREAM_REMOTE = zif_php_git2_function<
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

#define GIT_BRANCH_FE                                                   \
    PHP_GIT2_FE(git_branch_create,ZIF_GIT_BRANCH_CREATE,NULL)           \
    PHP_GIT2_FE(git_branch_create_from_annotated,ZIF_GIT_BRANCH_CREATE_FROM_ANNOTATED,NULL) \
    PHP_GIT2_FE(git_branch_delete,ZIF_GIT_BRANCH_DELETE,NULL)           \
    PHP_GIT2_FE(git_branch_is_head,ZIF_GIT_BRANCH_IS_HEAD,NULL)         \
    PHP_GIT2_FE(git_branch_iterator_new,ZIF_GIT_BRANCH_ITERATOR_NEW,NULL) \
    PHP_GIT2_FE(git_branch_iterator_free,ZIF_GIT_BRANCH_ITERATOR_FREE,NULL) \
    PHP_GIT2_FE(git_branch_next,ZIF_GIT_BRANCH_NEXT,git_branch_next_arginfo) \
    PHP_GIT2_FE(git_branch_lookup,ZIF_GIT_BRANCH_LOOKUP,NULL)           \
    PHP_GIT2_FE(git_branch_move,ZIF_GIT_BRANCH_MOVE,NULL)               \
    PHP_GIT2_FE(git_branch_name,ZIF_GIT_BRANCH_NAME,NULL)               \
    PHP_GIT2_FE(git_branch_set_upstream,ZIF_GIT_BRANCH_SET_UPSTREAM,NULL) \
    PHP_GIT2_FE(git_branch_upstream,ZIF_GIT_BRANCH_UPSTREAM,NULL)       \
    PHP_GIT2_FE(git_branch_upstream_name,ZIF_GIT_BRANCH_UPSTREAM_NAME,NULL) \
    PHP_GIT2_FE(git_branch_upstream_remote,ZIF_GIT_BRANCH_UPSTREAM_REMOTE,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
