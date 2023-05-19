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

static constexpr auto ZIF_git_submodule_add_finalize = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_submodule*>::func<git_submodule_add_finalize>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>
        >
    >;

static constexpr auto ZIF_git_submodule_add_setup = zif_php_git2_function<
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

static constexpr auto ZIF_git_submodule_add_to_index = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_submodule*,
        int>::func<git_submodule_add_to_index>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>,
        php_git2::php_bool
        >
    >;

static constexpr auto ZIF_git_submodule_branch = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        git_submodule*>::func<git_submodule_branch>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>
        >,
    0
    >;

static constexpr auto ZIF_git_submodule_fetch_recurse_submodules = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_submodule_recurse_t,
        git_submodule*>::func<git_submodule_fetch_recurse_submodules>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>
        >,
    php_git2::php_numeric_rethandler<git_submodule_recurse_t>
    >;

static constexpr auto ZIF_git_submodule_foreach = zif_php_git2_function<
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

static constexpr auto ZIF_git_submodule_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_submodule>
        >
    >;

static constexpr auto ZIF_git_submodule_head_id = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        git_submodule*>::func<git_submodule_head_id>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>
        >,
    0
    >;

static constexpr auto ZIF_git_submodule_ignore = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_submodule_ignore_t,
        git_submodule*>::func<git_submodule_ignore>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>
        >,
    php_git2::php_numeric_rethandler<git_submodule_ignore_t>
    >;

static constexpr auto ZIF_git_submodule_index_id = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        git_submodule*>::func<git_submodule_index_id>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>
        >,
    0
    >;

static constexpr auto ZIF_git_submodule_init = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_submodule*,
        int>::func<git_submodule_init>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>,
        php_git2::php_bool
        >
    >;

static constexpr auto ZIF_git_submodule_location = zif_php_git2_function<
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

static constexpr auto ZIF_git_submodule_lookup = zif_php_git2_function<
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

static constexpr auto ZIF_git_submodule_name = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        git_submodule*>::func<git_submodule_name>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>
        >,
    0
    >;

static constexpr auto ZIF_git_submodule_open = zif_php_git2_function<
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

static constexpr auto ZIF_git_submodule_owner = zif_php_git2_function_rethandler<
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

static constexpr auto ZIF_git_submodule_path = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        git_submodule*>::func<git_submodule_path>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>
        >,
    0
    >;

static constexpr auto ZIF_git_submodule_reload = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_submodule*,
        int>::func<git_submodule_reload>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>,
        php_git2::php_bool
        >
    >;

static constexpr auto ZIF_git_submodule_repo_init = zif_php_git2_function<
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

static constexpr auto ZIF_git_submodule_resolve_url = zif_php_git2_function<
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

static constexpr auto ZIF_git_submodule_set_branch = zif_php_git2_function<
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

static constexpr auto ZIF_git_submodule_set_fetch_recurse_submodules = zif_php_git2_function<
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

static constexpr auto ZIF_git_submodule_set_ignore = zif_php_git2_function<
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

static constexpr auto ZIF_git_submodule_set_update = zif_php_git2_function<
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

static constexpr auto ZIF_git_submodule_set_url = zif_php_git2_function<
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

static constexpr auto ZIF_git_submodule_status = zif_php_git2_function<
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

static constexpr auto ZIF_git_submodule_sync = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_submodule*>::func<git_submodule_sync>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>
        >
    >;

static constexpr auto ZIF_git_submodule_update = zif_php_git2_function<
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

static constexpr auto ZIF_git_submodule_update_strategy = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_submodule_update_t,
        git_submodule*>::func<git_submodule_update_strategy>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>
        >,
    php_git2::php_numeric_rethandler<git_submodule_update_t>
    >;

static constexpr auto ZIF_git_submodule_url = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        git_submodule*>::func<git_submodule_url>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>
        >,
    0
    >;

static constexpr auto ZIF_git_submodule_wd_id = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        git_submodule*>::func<git_submodule_wd_id>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_submodule>
        >,
    0
    >;

// Function entry macro

#define GIT_SUBMODULE_FE                                    \
    PHP_GIT2_FE(git_submodule_add_finalize)                 \
    PHP_GIT2_FE(git_submodule_add_setup)                    \
    PHP_GIT2_FE(git_submodule_add_to_index)                 \
    PHP_GIT2_FE(git_submodule_branch)                       \
    PHP_GIT2_FE(git_submodule_fetch_recurse_submodules)     \
    PHP_GIT2_FE(git_submodule_foreach)                      \
    PHP_GIT2_FE(git_submodule_free)                         \
    PHP_GIT2_FE(git_submodule_head_id)                      \
    PHP_GIT2_FE(git_submodule_ignore)                       \
    PHP_GIT2_FE(git_submodule_index_id)                     \
    PHP_GIT2_FE(git_submodule_init)                         \
    PHP_GIT2_FE(git_submodule_location)                     \
    PHP_GIT2_FE(git_submodule_lookup)                       \
    PHP_GIT2_FE(git_submodule_name)                         \
    PHP_GIT2_FE(git_submodule_open)                         \
    PHP_GIT2_FE(git_submodule_owner)                        \
    PHP_GIT2_FE(git_submodule_path)                         \
    PHP_GIT2_FE(git_submodule_reload)                       \
    PHP_GIT2_FE(git_submodule_repo_init)                    \
    PHP_GIT2_FE(git_submodule_resolve_url)                  \
    PHP_GIT2_FE(git_submodule_set_branch)                   \
    PHP_GIT2_FE(git_submodule_set_fetch_recurse_submodules) \
    PHP_GIT2_FE(git_submodule_set_ignore)                   \
    PHP_GIT2_FE(git_submodule_set_update)                   \
    PHP_GIT2_FE(git_submodule_set_url)                      \
    PHP_GIT2_FE(git_submodule_status)                       \
    PHP_GIT2_FE(git_submodule_sync)                         \
    PHP_GIT2_FE(git_submodule_update)                       \
    PHP_GIT2_FE(git_submodule_update_strategy)              \
    PHP_GIT2_FE(git_submodule_url)                          \
    PHP_GIT2_FE(git_submodule_wd_id)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
