/*
 * checkout.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_CHECKOUT_H
#define PHPGIT2_CHECKOUT_H

namespace php_git2
{

    class php_git_checkout_options:
        public php_value_base
    {
    public:
        php_git_checkout_options(TSRMLS_D)
        {
            git_checkout_init_options(&opts,GIT_CHECKOUT_OPTIONS_VERSION);
        }

        git_checkout_options* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            if (value != nullptr && Z_TYPE_P(value) == IS_ARRAY) {
                array_wrapper arr(value);

                GIT2_ARRAY_LOOKUP_LONG(arr,version,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,checkout_strategy,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,dir_mode,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,file_mode,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,file_open_flags,opts);
                // TODO Handle git_checkout_notify_cb and payload properties.
                // TODO Handle git_checkout_progress_cb and payload properties.
                GIT2_ARRAY_LOOKUP_RESOURCE(arr,php_git_tree,baseline,opts);
                // TODO Handle baseline_index property
                GIT2_ARRAY_LOOKUP_STRING(arr,target_directory,opts);
                GIT2_ARRAY_LOOKUP_STRING(arr,ancestor_label,opts);
                GIT2_ARRAY_LOOKUP_STRING(arr,our_label,opts);
                GIT2_ARRAY_LOOKUP_STRING(arr,their_label,opts);
                // TODO Handle git_checkout_perfdata_cb and payload properties.

                return &opts;
            }

            return nullptr;
        }
    private:
        git_checkout_options opts;
    };

} // namespace php_git2

// Functions:

static constexpr auto ZIF_GIT_CHECKOUT_HEAD = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const git_checkout_options*>::func<git_checkout_head>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_git_checkout_options
        >
    >;

static constexpr auto ZIF_GIT_CHECKOUT_TREE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const git_object*,
        const git_checkout_options*>::func<git_checkout_tree>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_object>,
        php_git2::php_git_checkout_options
        >
    >;

static constexpr auto ZIF_GIT_CHECKOUT_INDEX = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        git_index*,
        const git_checkout_options*>::func<git_checkout_index>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_git_checkout_options
        >
    >;

// Function Entries:

#define GIT_CHECKOUT_FE                                         \
    PHP_GIT2_FE(git_checkout_head,ZIF_GIT_CHECKOUT_HEAD,NULL)   \
    PHP_GIT2_FE(git_checkout_tree,ZIF_GIT_CHECKOUT_TREE,NULL)   \
    PHP_GIT2_FE(git_checkout_index,ZIF_GIT_CHECKOUT_INDEX,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
