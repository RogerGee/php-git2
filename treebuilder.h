/*
 * treebuilder.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_TREEBUILDER_H
#define PHPGIT2_TREEBUILDER_H

#include "tree.h"
#include "stubs/treebuilder_arginfo.h"

namespace php_git2
{
    // Provide specialization of git2_resource destructor for treebuilder.

    template<> php_git_treebuilder::~git2_resource()
    {
        git_treebuilder_free(handle);
    }

} // php_git2

// Template declarations for bindings:

static constexpr auto ZIF_git_treebuilder_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_treebuilder>
        >
    >;

static constexpr auto ZIF_git_treebuilder_new = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_treebuilder**,
        git_repository*,
        const git_tree*>::func<git_treebuilder_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_treebuilder>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource_nullable<php_git2::php_git_tree>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_treebuilder_write = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_treebuilder*>::func<git_treebuilder_write>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_treebuilder>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_treebuilder_insert = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        const git_tree_entry**,
        git_treebuilder*,
        const char*,
        const git_oid*,
        git_filemode_t>::func<git_treebuilder_insert>,
    php_git2::local_pack<
        const php_git2::php_resource_ref<php_git2::php_git_tree_entry_nofree>,
        php_git2::php_resource<php_git2::php_git_treebuilder>,
        php_git2::php_string,
        php_git2::php_git_oid_fromstr,
        php_git2::php_long_cast<git_filemode_t>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2,3,4>,
    php_git2::sequence<0,1,2,3,4>
    >;

static constexpr auto ZIF_git_treebuilder_get = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        const git_tree_entry*,
        git_treebuilder*,
        const char*
        >::func<git_treebuilder_get>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_treebuilder>,
        php_git2::php_string
        >,
    php_git2::php_git_tree_entry_rethandler<
        php_git2::php_resource<php_git2::php_git_treebuilder>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_treebuilder_remove = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_treebuilder*,
        const char*>::func<git_treebuilder_remove>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_treebuilder>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_treebuilder_clear = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_treebuilder*>::func<git_treebuilder_clear>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_treebuilder>
        >
    >;

static constexpr auto ZIF_git_treebuilder_entrycount = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        git_treebuilder*>::func<git_treebuilder_entrycount>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_treebuilder>
        >,
    0
    >;

static constexpr auto ZIF_git_treebuilder_filter = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_treebuilder*,
        git_treebuilder_filter_cb,
        void*>::func<git_treebuilder_filter>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_treebuilder>,
        php_git2::php_callback_handler<php_git2::treebuilder_filter_callback>,
        php_git2::php_callback_sync
        >,
    -1,
    php_git2::sequence<0,2,2>, // pass callback in twice for function and payload
    php_git2::sequence<0,1,2>
    >;

// PHP function entry macro for this module:

#define GIT_TREEBUILDER_FE                      \
    PHP_GIT2_FE(git_treebuilder_free)           \
    PHP_GIT2_FE(git_treebuilder_new)            \
    PHP_GIT2_FE(git_treebuilder_write)          \
    PHP_GIT2_FE(git_treebuilder_insert)         \
    PHP_GIT2_FE(git_treebuilder_get)            \
    PHP_GIT2_FE(git_treebuilder_remove)         \
    PHP_GIT2_FE(git_treebuilder_clear)          \
    PHP_GIT2_FE(git_treebuilder_entrycount)     \
    PHP_GIT2_FE(git_treebuilder_filter)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
