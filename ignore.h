/*
 * ignore.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_IGNORE_H
#define PHPGIT2_IGNORE_H

namespace php_git2
{

}

static constexpr auto ZIF_git_ignore_add_rule = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*>::func<git_ignore_add_rule>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    -1
    >;

static constexpr auto ZIF_git_ignore_clear_internal_rules = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*>::func<git_ignore_clear_internal_rules>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    -1
    >;

static constexpr auto ZIF_git_ignore_path_is_ignored = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        int*,
        git_repository*,
        const char*>::func<git_ignore_path_is_ignored>,
    php_git2::local_pack<
        php_git2::php_bool_ref<int>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

#define GIT_IGNORE_FE                               \
    PHP_GIT2_FE(git_ignore_add_rule)                \
    PHP_GIT2_FE(git_ignore_clear_internal_rules)    \
    PHP_GIT2_FE(git_ignore_path_is_ignored)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
