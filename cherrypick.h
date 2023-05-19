/*
 * cherrypick.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_CHERRYPICK_H
#define PHPGIT2_CHERRYPICK_H
#include "merge.h"
#include "checkout.h"

namespace php_git2
{
    // Provide a type for converting a PHP array into a git_cherrypick_options
    // struct.

    class php_git_cherrypick_options:
        public php_option_array
    {
    public:
        php_git_cherrypick_options()
        {
            git_cherrypick_init_options(&opts,GIT_CHERRYPICK_OPTIONS_VERSION);
        }

        git_cherrypick_options* byval_git2()
        {
            if (!is_null()) {
                array_wrapper arr(value);

                GIT2_ARRAY_LOOKUP_LONG(arr,mainline,opts);
                GIT2_ARRAY_LOOKUP_SUBOBJECT_DEREFERENCE(arr,mergeOpts,merge_opts,opts);
                GIT2_ARRAY_LOOKUP_SUBOBJECT_DEREFERENCE(arr,checkoutOpts,checkout_opts,opts);

                return &opts;
            }

            return nullptr;
        }

    private:
        git_cherrypick_options opts;
        php_git_strarray_byval_array strarray;
        php_git_merge_options mergeOpts;
        php_git_checkout_options checkoutOpts;
    };

} // namespace php_git2

static constexpr auto ZIF_git_cherrypick = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        git_commit*,
        const git_cherrypick_options*>::func<git_cherrypick>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_commit>,
        php_git2::php_git_cherrypick_options
        >
    >;

static constexpr auto ZIF_git_cherrypick_commit = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index**,
        git_repository*,
        git_commit*,
        git_commit*,
        unsigned int,
        const git_merge_options*>::func<git_cherrypick_commit>,
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

#define GIT_CHERRYPICK_FE                       \
    PHP_GIT2_FE(git_cherrypick)                 \
    PHP_GIT2_FE(git_cherrypick_commit)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
