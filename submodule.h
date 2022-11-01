/*
 * submodule.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_SUBMODULE_H
#define PHPGIT2_SUBMODULE_H
#include "checkout.h"
#include "remote.h"

namespace php_git2
{
    // Specialize git2_resource destructor for git_submodule.
    template<> php_git_submodule::~git2_resource()
    {
        git_submodule_free(handle);
    }

    // Create type to wrap git_submodule_update_options.

    class php_git_submodule_update_options:
        public php_option_array
    {
    public:
        php_git_submodule_update_options()
        {
            git_submodule_update_init_options(&opts,GIT_SUBMODULE_UPDATE_OPTIONS_VERSION);
        }

        git_submodule_update_options* byval_git2()
        {
            if (!is_null()) {
                array_wrapper arr(value);
                php_git_checkout_options checkoutOpts;
                php_git_fetch_options fetchOpts;

                GIT2_ARRAY_LOOKUP_LONG(arr,version,opts);
                GIT2_ARRAY_LOOKUP_SUBOBJECT_DEREFERENCE(arr,checkoutOpts,checkout_opts,opts);
                GIT2_ARRAY_LOOKUP_SUBOBJECT_DEREFERENCE(arr,fetchOpts,fetch_opts,opts);
                GIT2_ARRAY_LOOKUP_BOOL(arr,allow_fetch,opts);

                return &opts;
            }

            return nullptr;
        }

    private:
        git_submodule_update_options opts;
    };

} // namespace php_git2

// Function template declarations

static constexpr auto ZIF_GIT_SUBMODULE_ADD_FINALIZE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_submodule*>::func<git_submodule_add_finalize>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>
        >
    >;

static constexpr auto ZIF_GIT_SUBMODULE_ADD_SETUP = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_submodule**,
        git_repository*,
        const char*,
        const char*,
        int>::func<git_submodule_add_setup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_submodule>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_string,
        php_git2::php_bool
        >,
    1,
    php_git2::sequence<1,2,3,4>,
    php_git2::sequence<0,1,2,3,4>
    >;

static constexpr auto ZIF_GIT_SUBMODULE_ADD_TO_INDEX = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_submodule*,
        int>::func<git_submodule_add_to_index>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>,
        php_git2::php_bool
        >
    >;

static constexpr auto ZIF_GIT_SUBMODULE_BRANCH = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        git_submodule*>::func<git_submodule_branch>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>
        >,
    0
    >;

static constexpr auto ZIF_GIT_SUBMODULE_FETCH_RECURSE_SUBMODULES = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_submodule_recurse_t,
        git_submodule*>::func<git_submodule_fetch_recurse_submodules>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>
        >,
    php_git2::php_numeric_rethandler<git_submodule_recurse_t>
    >;

static constexpr auto ZIF_GIT_SUBMODULE_FOREACH = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        git_submodule_cb,
        void*>::func<git_submodule_foreach>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_callback_handler<php_git2::submodule_foreach_callback>,
        php_git2::php_callback_sync
        >,
    -1,
    php_git2::sequence<0,2,2>, // pass callback in twice for function and payload
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_GIT_SUBMODULE_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_submodule>
        >
    >;

static constexpr auto ZIF_GIT_SUBMODULE_HEAD_ID = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        git_submodule*>::func<git_submodule_head_id>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>
        >,
    0
    >;

static constexpr auto ZIF_GIT_SUBMODULE_IGNORE = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_submodule_ignore_t,
        git_submodule*>::func<git_submodule_ignore>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>
        >,
    php_git2::php_numeric_rethandler<git_submodule_ignore_t>
    >;

static constexpr auto ZIF_GIT_SUBMODULE_INDEX_ID = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        git_submodule*>::func<git_submodule_index_id>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>
        >,
    0
    >;

static constexpr auto ZIF_GIT_SUBMODULE_INIT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_submodule*,
        int>::func<git_submodule_init>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>,
        php_git2::php_bool
        >
    >;

static constexpr auto ZIF_GIT_SUBMODULE_LOCATION = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        unsigned int*,
        git_submodule*>::func<git_submodule_location>,
    php_git2::local_pack<
        php_git2::php_long_ref<unsigned int>,
        php_git2::php_resource<php_git2::php_git_submodule>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_GIT_SUBMODULE_LOOKUP = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_submodule**,
        git_repository*,
        const char*>::func<git_submodule_lookup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_submodule>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_GIT_SUBMODULE_NAME = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        git_submodule*>::func<git_submodule_name>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>
        >,
    0
    >;

static constexpr auto ZIF_GIT_SUBMODULE_OPEN = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository**,
        git_submodule*>::func<git_submodule_open>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_submodule>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_GIT_SUBMODULE_OWNER = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_repository*,
        git_submodule*>::func<git_submodule_owner>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>,
        php_git2::php_resource_ref<php_git2::php_git_repository_nofree>
        >,
    php_git2::php_owner_rethandler<php_git2::php_git_submodule>,
    php_git2::sequence<0>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_GIT_SUBMODULE_PATH = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        git_submodule*>::func<git_submodule_path>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>
        >,
    0
    >;

static constexpr auto ZIF_GIT_SUBMODULE_RELOAD = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_submodule*,
        int>::func<git_submodule_reload>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>,
        php_git2::php_bool
        >
    >;

static constexpr auto ZIF_GIT_SUBMODULE_REPO_INIT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository**,
        const git_submodule*,
        int>::func<git_submodule_repo_init>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_submodule>,
        php_git2::php_bool
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_GIT_SUBMODULE_RESOLVE_URL = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        git_repository*,
        const char*>::func<git_submodule_resolve_url>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_GIT_SUBMODULE_SET_BRANCH = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*,
        const char*>::func<git_submodule_set_branch>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_GIT_SUBMODULE_SET_FETCH_RECURSE_SUBMODULES = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*,
        git_submodule_recurse_t>::func<git_submodule_set_fetch_recurse_submodules>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_long_cast<git_submodule_recurse_t>
        >
    >;

static constexpr auto ZIF_GIT_SUBMODULE_SET_IGNORE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*,
        git_submodule_ignore_t>::func<git_submodule_set_ignore>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_long_cast<git_submodule_ignore_t>
        >
    >;

static constexpr auto ZIF_GIT_SUBMODULE_SET_UPDATE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*,
        git_submodule_update_t>::func<git_submodule_set_update>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_long_cast<git_submodule_update_t>
        >
    >;

static constexpr auto ZIF_GIT_SUBMODULE_SET_URL = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*,
        const char*>::func<git_submodule_set_url>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_GIT_SUBMODULE_STATUS = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        unsigned int*,
        git_repository*,
        const char*,
        git_submodule_ignore_t>::func<git_submodule_status>,
    php_git2::local_pack<
        php_git2::php_long_ref<unsigned int>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_long_cast<git_submodule_ignore_t>
        >,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_GIT_SUBMODULE_SYNC = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_submodule*>::func<git_submodule_sync>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>
        >
    >;

static constexpr auto ZIF_GIT_SUBMODULE_UPDATE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_submodule*,
        int,
        git_submodule_update_options*>::func<git_submodule_update>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>,
        php_git2::php_bool,
        php_git2::php_git_submodule_update_options
        >
    >;

static constexpr auto ZIF_GIT_SUBMODULE_UPDATE_STRATEGY = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_submodule_update_t,
        git_submodule*>::func<git_submodule_update_strategy>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>
        >,
    php_git2::php_numeric_rethandler<git_submodule_update_t>
    >;

static constexpr auto ZIF_GIT_SUBMODULE_URL = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        git_submodule*>::func<git_submodule_url>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>
        >,
    0
    >;

static constexpr auto ZIF_GIT_SUBMODULE_WD_ID = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        git_submodule*>::func<git_submodule_wd_id>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>
        >,
    0
    >;

// Function entry macro

#define GIT_SUBMODULE_FE                                                \
    PHP_GIT2_FE(git_submodule_add_finalize,ZIF_GIT_SUBMODULE_ADD_FINALIZE,NULL) \
    PHP_GIT2_FE(git_submodule_add_setup,ZIF_GIT_SUBMODULE_ADD_SETUP,NULL) \
    PHP_GIT2_FE(git_submodule_add_to_index,ZIF_GIT_SUBMODULE_ADD_TO_INDEX,NULL) \
    PHP_GIT2_FE(git_submodule_branch,ZIF_GIT_SUBMODULE_BRANCH,NULL)     \
    PHP_GIT2_FE(git_submodule_fetch_recurse_submodules,ZIF_GIT_SUBMODULE_FETCH_RECURSE_SUBMODULES,NULL) \
    PHP_GIT2_FE(git_submodule_foreach,ZIF_GIT_SUBMODULE_FOREACH,NULL)   \
    PHP_GIT2_FE(git_submodule_free,ZIF_GIT_SUBMODULE_FREE,NULL)         \
    PHP_GIT2_FE(git_submodule_head_id,ZIF_GIT_SUBMODULE_HEAD_ID,NULL)   \
    PHP_GIT2_FE(git_submodule_ignore,ZIF_GIT_SUBMODULE_IGNORE,NULL)     \
    PHP_GIT2_FE(git_submodule_index_id,ZIF_GIT_SUBMODULE_INDEX_ID,NULL) \
    PHP_GIT2_FE(git_submodule_init,ZIF_GIT_SUBMODULE_INIT,NULL)         \
    PHP_GIT2_FE(git_submodule_location,ZIF_GIT_SUBMODULE_LOCATION,NULL) \
    PHP_GIT2_FE(git_submodule_lookup,ZIF_GIT_SUBMODULE_LOOKUP,NULL)     \
    PHP_GIT2_FE(git_submodule_name,ZIF_GIT_SUBMODULE_NAME,NULL)         \
    PHP_GIT2_FE(git_submodule_open,ZIF_GIT_SUBMODULE_OPEN,NULL)         \
    PHP_GIT2_FE(git_submodule_owner,ZIF_GIT_SUBMODULE_OWNER,NULL)       \
    PHP_GIT2_FE(git_submodule_path,ZIF_GIT_SUBMODULE_PATH,NULL)         \
    PHP_GIT2_FE(git_submodule_reload,ZIF_GIT_SUBMODULE_RELOAD,NULL)     \
    PHP_GIT2_FE(git_submodule_repo_init,ZIF_GIT_SUBMODULE_REPO_INIT,NULL) \
    PHP_GIT2_FE(git_submodule_resolve_url,ZIF_GIT_SUBMODULE_RESOLVE_URL,NULL) \
    PHP_GIT2_FE(git_submodule_set_branch,ZIF_GIT_SUBMODULE_SET_BRANCH,NULL) \
    PHP_GIT2_FE(git_submodule_set_fetch_recurse_submodules,ZIF_GIT_SUBMODULE_SET_FETCH_RECURSE_SUBMODULES,NULL) \
    PHP_GIT2_FE(git_submodule_set_ignore,ZIF_GIT_SUBMODULE_SET_IGNORE,NULL) \
    PHP_GIT2_FE(git_submodule_set_update,ZIF_GIT_SUBMODULE_SET_UPDATE,NULL) \
    PHP_GIT2_FE(git_submodule_set_url,ZIF_GIT_SUBMODULE_SET_URL,NULL)   \
    PHP_GIT2_FE(git_submodule_status,ZIF_GIT_SUBMODULE_STATUS,NULL)     \
    PHP_GIT2_FE(git_submodule_sync,ZIF_GIT_SUBMODULE_SYNC,NULL)         \
    PHP_GIT2_FE(git_submodule_update,ZIF_GIT_SUBMODULE_UPDATE,NULL)     \
    PHP_GIT2_FE(git_submodule_update_strategy,ZIF_GIT_SUBMODULE_UPDATE_STRATEGY,NULL) \
    PHP_GIT2_FE(git_submodule_url,ZIF_GIT_SUBMODULE_URL,NULL)           \
    PHP_GIT2_FE(git_submodule_wd_id,ZIF_GIT_SUBMODULE_WD_ID,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
