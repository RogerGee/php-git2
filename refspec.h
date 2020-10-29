/*
 * refspec.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_REFSPEC_H
#define PHPGIT2_REFSPEC_H

namespace php_git2
{

} // namespace php_git2

static constexpr auto ZIF_GIT_REFSPEC_DIRECTION = zif_php_git2_function<
    php_git2::func_wrapper<
        git_direction,
        const git_refspec*>::func<git_refspec_direction>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_refspec>
        >,
    0
    >;

static constexpr auto ZIF_GIT_REFSPEC_DST = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_refspec*>::func<git_refspec_dst>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_refspec>
        >,
    0
    >;

static constexpr auto ZIF_GIT_REFSPEC_DST_MATCHES = zif_php_git2_function_rethandler<
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

static constexpr auto ZIF_GIT_REFSPEC_FORCE = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_refspec*>::func<git_refspec_force>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_refspec>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_GIT_REFSPEC_RTRANSFORM = zif_php_git2_function<
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

static constexpr auto ZIF_GIT_REFSPEC_SRC = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_refspec*>::func<git_refspec_src>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_refspec>
        >,
    0
    >;

static constexpr auto ZIF_GIT_REFSPEC_SRC_MATCHES = zif_php_git2_function_rethandler<
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

static constexpr auto ZIF_GIT_REFSPEC_STRING = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_refspec*>::func<git_refspec_string>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_refspec>
        >,
    0
    >;

static constexpr auto ZIF_GIT_REFSPEC_TRANSFORM = zif_php_git2_function<
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

#define GIT_REFSPEC_FE                                                  \
    PHP_GIT2_FE(git_refspec_direction,ZIF_GIT_REFSPEC_DIRECTION,NULL)   \
    PHP_GIT2_FE(git_refspec_dst,ZIF_GIT_REFSPEC_DST,NULL)               \
    PHP_GIT2_FE(git_refspec_dst_matches,ZIF_GIT_REFSPEC_DST_MATCHES,NULL) \
    PHP_GIT2_FE(git_refspec_force,ZIF_GIT_REFSPEC_FORCE,NULL)           \
    PHP_GIT2_FE(git_refspec_rtransform,ZIF_GIT_REFSPEC_RTRANSFORM,NULL) \
    PHP_GIT2_FE(git_refspec_src,ZIF_GIT_REFSPEC_SRC,NULL)               \
    PHP_GIT2_FE(git_refspec_src_matches,ZIF_GIT_REFSPEC_SRC_MATCHES,NULL) \
    PHP_GIT2_FE(git_refspec_string,ZIF_GIT_REFSPEC_STRING,NULL)         \
    PHP_GIT2_FE(git_refspec_transform,ZIF_GIT_REFSPEC_TRANSFORM,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
