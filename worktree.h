/*
 * worktree.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_WORKTREE_H
#define PHPGIT2_WORKTREE_H
#include "checkout.h"

namespace php_git2
{
    // Explicitly specialize git2_resource destructor for git_worktree.
    template<> php_git_worktree::~git2_resource()
    {
        git_worktree_free(handle);
    }

    class php_git_worktree_add_options:
        public php_option_array
    {
    public:
        php_git_worktree_add_options()
        {
            git_worktree_add_options_init(&opts,GIT_WORKTREE_ADD_OPTIONS_VERSION);
        }

        git_worktree_add_options* byval_git2()
        {
            if (!is_null()) {
                array_wrapper arr(value);
                php_git_checkout_options checkoutOptions;

                GIT2_ARRAY_LOOKUP_BOOL(arr,lock,opts);
                GIT2_ARRAY_LOOKUP_RESOURCE(arr,php_git_reference,ref,opts);
                GIT2_ARRAY_LOOKUP_SUBOBJECT_DEREFERENCE(arr,checkoutOptions,checkout_options,opts);

                return &opts;
            }

            return nullptr;
        }

    private:
        git_worktree_add_options opts;
    };

    class php_git_worktree_prune_options:
        public php_option_array
    {
    public:
        php_git_worktree_prune_options()
        {
            git_worktree_prune_options_init(&opts,GIT_WORKTREE_PRUNE_OPTIONS_VERSION);
        }

        git_worktree_prune_options* byval_git2()
        {
            if (!is_null()) {
                array_wrapper arr(value);

                GIT2_ARRAY_LOOKUP_LONG(arr,flags,opts);

                return &opts;
            }

            return nullptr;
        }

    private:
        git_worktree_prune_options opts;
    };

} // namespace php_git2

static constexpr auto ZIF_GIT_WORKTREE_ADD = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_worktree**,
        git_repository*,
        const char*,
        const char*,
        const git_worktree_add_options*
        >::func<git_worktree_add>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_worktree>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_string,
        php_git2::php_git_worktree_add_options
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2,3,4>,
    php_git2::sequence<0,1,2,3,4>
    >;

static constexpr auto ZIF_GIT_WORKTREE_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_worktree>
        >
    >;

static constexpr auto ZIF_GIT_WORKTREE_IS_LOCKED = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_buf*,
        const git_worktree*
        >::func<git_worktree_is_locked>,
    php_git2::local_pack<
        php_git2::php_git_buf_out,
        php_git2::php_resource<php_git2::php_git_worktree>
        >,
    php_git2::php_boolean_error_rethandler<int>
    >;
ZEND_BEGIN_ARG_INFO_EX(git_worktree_is_locked_arginfo,0,0,2)
    ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

static constexpr auto ZIF_GIT_WORKTREE_IS_PRUNABLE = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_worktree*,
        git_worktree_prune_options*
        >::func<git_worktree_is_prunable>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_worktree>,
        php_git2::php_git_worktree_prune_options
        >,
    php_git2::php_boolean_error_rethandler<int>
    >;

static constexpr auto ZIF_GIT_WORKTREE_LIST = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_strarray*,
        git_repository*
        >::func<git_worktree_list>,
    php_git2::local_pack<
        php_git2::php_git_strarray,
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_GIT_WORKTREE_LOCK = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_worktree*,
        const char*
        >::func<git_worktree_lock>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_worktree>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_GIT_WORKTREE_LOOKUP = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_worktree**,
        git_repository*,
        const char*
        >::func<git_worktree_lookup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_worktree>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_GIT_WORKTREE_NAME = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_worktree*
        >::func<git_worktree_name>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_worktree>
        >,
    0
    >;

static constexpr auto ZIF_GIT_WORKTREE_OPEN_FROM_REPOSITORY = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_worktree**,
        git_repository*
        >::func<git_worktree_open_from_repository>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_worktree>,
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_GIT_WORKTREE_PATH = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_worktree*
        >::func<git_worktree_path>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_worktree>
        >,
    0
    >;

static constexpr auto ZIF_GIT_WORKTREE_PRUNE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_worktree*,
        git_worktree_prune_options*
        >::func<git_worktree_prune>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_worktree>,
        php_git2::php_git_worktree_prune_options
        >
    >;

static constexpr auto ZIF_GIT_WORKTREE_UNLOCK = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_worktree*
        >::func<git_worktree_unlock>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_worktree>
        >
    >;

static constexpr auto ZIF_GIT_WORKTREE_VALIDATE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const git_worktree*
        >::func<git_worktree_validate>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_worktree>
        >
    >;

#define GIT_WORKTREE_FE                                                 \
    PHP_GIT2_FE(git_worktree_add,ZIF_GIT_WORKTREE_ADD,NULL)             \
    PHP_GIT2_FE(git_worktree_free,ZIF_GIT_WORKTREE_FREE,NULL)           \
    PHP_GIT2_FE(git_worktree_is_locked,ZIF_GIT_WORKTREE_IS_LOCKED,git_worktree_is_locked_arginfo) \
    PHP_GIT2_FE(git_worktree_is_prunable,ZIF_GIT_WORKTREE_IS_PRUNABLE,NULL) \
    PHP_GIT2_FE(git_worktree_list,ZIF_GIT_WORKTREE_LIST,NULL)           \
    PHP_GIT2_FE(git_worktree_lock,ZIF_GIT_WORKTREE_LOCK,NULL)           \
    PHP_GIT2_FE(git_worktree_lookup,ZIF_GIT_WORKTREE_LOOKUP,NULL)       \
    PHP_GIT2_FE(git_worktree_name,ZIF_GIT_WORKTREE_NAME,NULL)           \
    PHP_GIT2_FE(git_worktree_open_from_repository,ZIF_GIT_WORKTREE_OPEN_FROM_REPOSITORY,NULL) \
    PHP_GIT2_FE(git_worktree_path,ZIF_GIT_WORKTREE_PATH,NULL)           \
    PHP_GIT2_FE(git_worktree_prune,ZIF_GIT_WORKTREE_PRUNE,NULL)         \
    PHP_GIT2_FE(git_worktree_unlock,ZIF_GIT_WORKTREE_UNLOCK,NULL)       \
    PHP_GIT2_FE(git_worktree_validate,ZIF_GIT_WORKTREE_VALIDATE,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
