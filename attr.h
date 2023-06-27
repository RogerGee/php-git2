/*
 * attr.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_ATTR_H
#define PHPGIT2_ATTR_H

#include "stubs/attr_arginfo.h"

namespace php_git2
{
}

static constexpr auto ZIF_git_attr_add_macro = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*,
        const char*>::func<git_attr_add_macro>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_attr_cache_flush = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*>::func<git_attr_cache_flush>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >
    >;

static constexpr auto ZIF_git_attr_foreach = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        uint32_t,
        const char*,
        git_attr_foreach_cb,
        void*>::func<git_attr_foreach>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_long_cast<uint32_t>,
        php_git2::php_string,
        php_git2::php_callback_handler<php_git2::attr_foreach_callback>,
        php_git2::php_callback_sync
        >,
    -1,
    php_git2::sequence<0,1,2,4,4>,
    php_git2::sequence<0,1,2,3,4>
    >;

static constexpr auto ZIF_git_attr_get = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const char**,
        git_repository*,
        uint32_t,
        const char*,
        const char*>::func<git_attr_get>,
    php_git2::local_pack<
        php_git2::php_string_ref,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_long_cast<uint32_t>,
        php_git2::php_string,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2,3,4>,
    php_git2::sequence<0,1,2,3,4>
    >;

// static constexpr auto ZIF_git_attr_get_many = zif_php_git2_function<
//     php_git2::func_wrapper<

//         >::func<git_attr_get_many>,
//     php_git2::local_pack<

//         >,
//     -1,
//     php_git2::sequence<>,
//     php_git2::sequence<>,
//     php_git2::sequence<>
//     >;

static constexpr auto ZIF_git_attr_value = zif_php_git2_function<
    php_git2::func_wrapper<
        git_attr_t,
        const char*>::func<git_attr_value>,
    php_git2::local_pack<
        php_git2::php_string
        >,
    0
    >;

#define GIT_ATTR_FE                             \
    PHP_GIT2_FE(git_attr_add_macro)             \
    PHP_GIT2_FE(git_attr_cache_flush)           \
    PHP_GIT2_FE(git_attr_foreach)               \
    PHP_GIT2_FE(git_attr_get)                   \
    PHP_GIT2_UNIMPLEMENTED(git_attr_get_many)   \
    PHP_GIT2_FE(git_attr_value)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
