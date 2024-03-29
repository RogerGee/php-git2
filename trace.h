/*
 * trace.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_TRACE_H
#define PHPGIT2_TRACE_H

#include "stubs/trace_arginfo.h"

namespace php_git2
{
}

static constexpr auto ZIF_git_trace_set = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_trace_level_t,
        git_trace_callback>::func<git_trace_set>,
    php_git2::local_pack<
        php_git2::php_long_cast<git_trace_level_t>,
        php_git2::php_callback_handler<php_git2::trace_callback>
        >,
    -1,
    php_git2::sequence<0>,
    php_git2::sequence<0,1>
    >;

#define GIT_TRACE_FE                            \
    PHP_GIT2_FE(git_trace_set)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
