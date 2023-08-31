/*
 * reset.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_RESET_H
#define PHPGIT2_RESET_H

#include "checkout.h"
#include "stubs/reset_arginfo.h"

namespace php_git2
{


} // namespace php_git2

static constexpr auto ZIF_git_reset = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const git_object*,
        git_reset_t,
        const git_checkout_options*>::func<git_reset>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_object>,
        php_git2::php_long_cast<git_reset_t>,
        php_git2::php_git_checkout_options
        >
    >;

static constexpr auto ZIF_git_reset_default = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const git_object*,
        const git_strarray*>::func<git_reset_default>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource_nullable<php_git2::php_git_object>,
        php_git2::php_git_strarray_array
        >
    >;

static constexpr auto ZIF_git_reset_from_annotated = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const git_annotated_commit*,
        git_reset_t,
        const git_checkout_options*>::func<git_reset_from_annotated>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_annotated_commit>,
        php_git2::php_long_cast<git_reset_t>,
        php_git2::php_git_checkout_options
        >
    >;

#define GIT_RESET_FE                            \
    PHP_GIT2_FE(git_reset)                      \
    PHP_GIT2_FE(git_reset_default)              \
    PHP_GIT2_FE(git_reset_from_annotated)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
