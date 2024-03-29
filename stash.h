/*
 * stash.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_STASH_H
#define PHPGIT2_STASH_H

#include "checkout.h"
#include "stubs/stash_arginfo.h"

namespace php_git2
{
    class php_git_stash_apply_options:
        public php_option_array
    {
    public:
        php_git_stash_apply_options()
        {
            git_stash_apply_init_options(&opts,GIT_STASH_APPLY_OPTIONS_VERSION);
        }

        const git_stash_apply_options* byval_git2()
        {
            if (!is_null()) {
                array_wrapper arr(value);
                php_git_checkout_options checkoutOptions;

                GIT2_ARRAY_LOOKUP_LONG(arr,flags,opts);
                GIT2_ARRAY_LOOKUP_SUBOBJECT_DEREFERENCE(arr,checkoutOptions,checkout_options,opts);
                GIT2_ARRAY_LOOKUP_CALLBACK_NULLABLE(
                    arr,
                    stash_apply_progress_callback,
                    callback,
                    progress_cb,
                    progress_payload,
                    opts);

                return &opts;
            }

            return nullptr;
        }

    private:
        git_stash_apply_options opts;
        php_callback_sync callback;
    };

} // namespace php_git2

static constexpr auto ZIF_git_stash_apply = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        size_t,
        const git_stash_apply_options*>::func<git_stash_apply>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_long_cast<size_t>,
        php_git2::php_git_stash_apply_options
        >
    >;

static constexpr auto ZIF_git_stash_drop = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        size_t>::func<git_stash_drop>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_long_cast<size_t>
        >
    >;

static constexpr auto ZIF_git_stash_foreach = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        git_stash_cb,
        void*>::func<git_stash_foreach>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_callback_handler<php_git2::stash_callback>,
        php_git2::php_callback_sync
        >,
    -1,
    php_git2::sequence<0,2,2>, // pass callback in twice for function and payload
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_stash_pop = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        size_t,
        const git_stash_apply_options*>::func<git_stash_pop>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_long_cast<size_t>,
        php_git2::php_git_stash_apply_options
        >
    >;

static constexpr auto ZIF_git_stash_save = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_repository*,
        const git_signature*,
        const char*,
        uint32_t>::func<git_stash_save>,
    php_git2::local_pack<
        php_git2::php_git_oid_out,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_signature>,
        php_git2::php_string,
        php_git2::php_long_cast<uint32_t>
        >,
    1,
    php_git2::sequence<1,2,3,4>,
    php_git2::sequence<0,1,2,3,4>
    >;

#define GIT_STASH_FE                            \
    PHP_GIT2_FE(git_stash_apply)                \
    PHP_GIT2_FE(git_stash_drop)                 \
    PHP_GIT2_FE(git_stash_foreach)              \
    PHP_GIT2_FE(git_stash_pop)                  \
    PHP_GIT2_FE(git_stash_save)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
