/*
 * revert.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_REVERT_H
#define PHPGIT2_REVERT_H
#include "merge.h"

namespace php_git2
{
    class php_git_revert_options:
        public php_option_array
    {
    public:
        php_git_revert_options()
        {
            git_revert_init_options(&opts,GIT_REVERT_OPTIONS_VERSION);
        }

        const git_revert_options* byval_git2()
        {
            if (!is_null()) {
                array_wrapper arr(value);
                php_git_merge_options mergeOptions;
                php_git_checkout_options checkoutOptions;

                GIT2_ARRAY_LOOKUP_LONG(arr,mainline,opts);
                GIT2_ARRAY_LOOKUP_SUBOBJECT_DEREFERENCE(arr,mergeOptions,merge_opts,opts);
                GIT2_ARRAY_LOOKUP_SUBOBJECT_DEREFERENCE(arr,checkoutOptions,checkout_opts,opts);

                return &opts;
            }

            return nullptr;
        }

    private:
        git_revert_options opts;
    };

} // namespace php_git2

static constexpr auto ZIF_GIT_REVERT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        git_commit*,
        const git_revert_options*>::func<git_revert>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_commit>,
        php_git2::php_git_revert_options
        >
    >;

static constexpr auto ZIF_GIT_REVERT_COMMIT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index**,
        git_repository*,
        git_commit*,
        git_commit*,
        unsigned int,
        const git_merge_options*>::func<git_revert_commit>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_index>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_commit>,
        php_git2::php_resource<php_git2::php_git_commit>,
        php_git2::php_long_cast<unsigned int>,
        php_git2::php_git_merge_options
        >,
    1,
    php_git2::sequence<1,2,3,4,5>,
    php_git2::sequence<0,1,2,3,4,5>
    >;

#define GIT_REVERT_FE                                           \
    PHP_GIT2_FE(git_revert,ZIF_GIT_REVERT,NULL)                 \
    PHP_GIT2_FE(git_revert_commit,ZIF_GIT_REVERT_COMMIT,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
