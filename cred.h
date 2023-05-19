/*
 * cred.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_CRED_H
#define PHPGIT2_CRED_H

namespace php_git2
{

    template<> php_git_cred::~git2_resource()
    {
        git_cred_free(handle);
    }

} // namespace php-git2

static constexpr auto ZIF_git_cred_default_new = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_cred**>::func<git_cred_default_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_cred>
        >,
    1,
    php_git2::sequence<>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_git_cred_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_cred>
        >
    >;

static constexpr auto ZIF_git_cred_has_username = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_cred*>::func<git_cred_has_username>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_cred>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

/*
static constexpr auto ZIF_git_cred_ssh_custom_new = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_cred_ssh_custom_new>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_git_cred_ssh_interactive_new = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_cred_ssh_interactive_new>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;
*/

static constexpr auto ZIF_git_cred_ssh_key_from_agent = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_cred**,
        const char*>::func<git_cred_ssh_key_from_agent>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_cred>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_cred_ssh_key_memory_new = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_cred**,
        const char*,
        const char*,
        const char*,
        const char*>::func<git_cred_ssh_key_memory_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_cred>,
        php_git2::php_string,
        php_git2::php_string,
        php_git2::php_string,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2,3,4>,
    php_git2::sequence<0,1,2,3,4>
    >;

static constexpr auto ZIF_git_cred_ssh_key_new = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_cred**,
        const char*,
        const char*,
        const char*,
        const char*>::func<git_cred_ssh_key_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_cred>,
        php_git2::php_string,
        php_git2::php_string,
        php_git2::php_string,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2,3,4>,
    php_git2::sequence<0,1,2,3,4>
    >;

static constexpr auto ZIF_git_cred_username_new = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_cred**,
        const char*>::func<git_cred_username_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_cred>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_cred_userpass_plaintext_new = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_cred**,
        const char*,
        const char*>::func<git_cred_userpass_plaintext_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_cred>,
        php_git2::php_string,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

#define GIT_CRED_FE                                         \
    PHP_GIT2_FE(git_cred_default_new)                       \
    PHP_GIT2_FE(git_cred_free)                              \
    PHP_GIT2_FE(git_cred_has_username)                      \
    PHP_GIT2_UNIMPLEMENTED(git_cred_ssh_custom_new)         \
    PHP_GIT2_UNIMPLEMENTED(git_cred_ssh_interactive_new)    \
    PHP_GIT2_FE(git_cred_ssh_key_from_agent)                \
    PHP_GIT2_FE(git_cred_ssh_key_memory_new)                \
    PHP_GIT2_FE(git_cred_ssh_key_new)                       \
    PHP_GIT2_FE(git_cred_username_new)                      \
    PHP_GIT2_FE(git_cred_userpass_plaintext_new)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
