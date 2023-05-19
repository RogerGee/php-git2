/*
 * refspec.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_REFSPEC_H
#define PHPGIT2_REFSPEC_H

namespace php_git2
{
    // Explicitly specialize git2_resource destructor for git_refspec.
    template<> php_git_refspec::~git2_resource()
    {
        // Nothing to do here since the git_refspec is always used read-only.
    }

} // namespace php_git2

static constexpr auto ZIF_git_refspec_direction = zif_php_git2_function<
    php_git2::func_wrapper<
        git_direction,
        const git_refspec*>::func<git_refspec_direction>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_refspec>
        >,
    0
    >;

static constexpr auto ZIF_git_refspec_dst = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_refspec*>::func<git_refspec_dst>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_refspec>
        >,
    0
    >;

static constexpr auto ZIF_git_refspec_dst_matches = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_refspec*,
        const char*>::func<git_refspec_dst_matches>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_refspec>,
        php_git2::php_string
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_git_refspec_force = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_refspec*>::func<git_refspec_force>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_refspec>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_git_refspec_rtransform = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        const git_refspec*,
        const char*>::func<git_refspec_rtransform>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_resource<php_git2::php_git_refspec>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_refspec_src = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_refspec*>::func<git_refspec_src>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_refspec>
        >,
    0
    >;

static constexpr auto ZIF_git_refspec_src_matches = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_refspec*,
        const char*>::func<git_refspec_src_matches>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_refspec>,
        php_git2::php_string
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_git_refspec_string = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_refspec*>::func<git_refspec_string>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_refspec>
        >,
    0
    >;

static constexpr auto ZIF_git_refspec_transform = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        const git_refspec*,
        const char*>::func<git_refspec_transform>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_resource<php_git2::php_git_refspec>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

#define GIT_REFSPEC_FE                          \
    PHP_GIT2_FE(git_refspec_direction)          \
    PHP_GIT2_FE(git_refspec_dst)                \
    PHP_GIT2_FE(git_refspec_dst_matches)        \
    PHP_GIT2_FE(git_refspec_force)              \
    PHP_GIT2_FE(git_refspec_rtransform)         \
    PHP_GIT2_FE(git_refspec_src)                \
    PHP_GIT2_FE(git_refspec_src_matches)        \
    PHP_GIT2_FE(git_refspec_string)             \
    PHP_GIT2_FE(git_refspec_transform)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
