/*
 * clone.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_CLONE_H
#define PHPGIT2_CLONE_H

#include "checkout.h"

namespace php_git2
{

    class php_git_clone_options:
        public php_value_base
    {
    public:
        php_git_clone_options(TSRMLS_D)
        {
            git_clone_init_options(&opts,GIT_CLONE_OPTIONS_VERSION);
        }

        git_clone_options* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            if (value != nullptr && Z_TYPE_P(value) == IS_ARRAY) {
                array_wrapper arr(value);

                GIT2_ARRAY_LOOKUP_LONG(arr,bare,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,local,opts);
                GIT2_ARRAY_LOOKUP_STRING_NULLABLE(arr,checkout_branch,opts);
                GIT2_ARRAY_LOOKUP_SUBOBJECT_DEREFERENCE(arr,checkoutOpts,checkout_opts,opts);
                GIT2_ARRAY_LOOKUP_CALLBACK(
                    arr,
                    repository_create_callback,
                    callback,
                    repository_cb,
                    repository_cb_payload,
                    opts);

                // TODO Handle fetch_opts property.
                // TODO Handle remote_cb property.

                return &opts;
            }

            return nullptr;
        }

    private:
        git_clone_options opts;
        php_git_checkout_options checkoutOpts;
        php_callback_sync callback;
    };

} // namespace php_git2

// Functions:

static constexpr auto ZIF_GIT_CLONE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository**,
        const char*,
        const char*,
        const git_clone_options*>::func<git_clone>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_string,
        php_git2::php_git_clone_options
        >,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>,
    php_git2::sequence<0,0,1,2>
    >;

// Function Entries:

#define GIT_CLONE_FE                            \
    PHP_GIT2_FE(git_clone,ZIF_GIT_CLONE,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
