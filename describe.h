/*
 * describe.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_DESCRIBE_H
#define PHPGIT2_DESCRIBE_H

#include "stubs/describe_arginfo.h"

namespace php_git2
{
    // Specialize resource destructor for git_describe_result
    template<> inline php_git_describe_result::~git2_resource()
    {
        git_describe_result_free(handle);
    }

    class php_git_describe_options:
        public php_option_array
    {
    public:
        php_git_describe_options()
        {
            git_describe_init_options(&opts,GIT_DESCRIBE_OPTIONS_VERSION);
        }

        /*const*/ git_describe_options* byval_git2()
        {
            if (!is_null()) {
                array_wrapper arr(value);

                GIT2_ARRAY_LOOKUP_LONG(arr,version,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,max_candidates_tags,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,describe_strategy,opts);
                GIT2_ARRAY_LOOKUP_STRING_NULLABLE(arr,pattern,opts);
                GIT2_ARRAY_LOOKUP_BOOL(arr,only_follow_first_parent,opts);
                GIT2_ARRAY_LOOKUP_BOOL(arr,show_commit_oid_as_fallback,opts);
            }

            return &opts;
        }

    private:
        git_describe_options opts;
    };

    class php_git_describe_format_options:
        public php_option_array
    {
    public:
        php_git_describe_format_options()
        {
            git_describe_init_format_options(&opts,GIT_DESCRIBE_FORMAT_OPTIONS_VERSION);
        }

        const git_describe_format_options* byval_git2()
        {
            if (!is_null()) {
                array_wrapper arr(value);

                GIT2_ARRAY_LOOKUP_LONG(arr,version,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,abbreviated_size,opts);
                GIT2_ARRAY_LOOKUP_BOOL(arr,always_use_long_format,opts);
                GIT2_ARRAY_LOOKUP_STRING_NULLABLE(arr,dirty_suffix,opts);
            }

            return &opts;
        }

    private:
        git_describe_format_options opts;
    };

} // namespace php_git2

static constexpr auto ZIF_git_describe_commit = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_describe_result**,
        git_object*,
        git_describe_options*>::func<git_describe_commit>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_describe_result>,
        php_git2::php_resource<php_git2::php_git_object>,
        php_git2::php_git_describe_options
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_describe_format = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        const git_describe_result*,
        const git_describe_format_options*>::func<git_describe_format>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_resource<php_git2::php_git_describe_result>,
        php_git2::php_git_describe_format_options
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_describe_result_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_describe_result>
        >
    >;

static constexpr auto ZIF_git_describe_workdir = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_describe_result**,
        git_repository*,
        git_describe_options*>::func<git_describe_workdir>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_describe_result>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_git_describe_options
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

#define GIT_DESCRIBE_FE                         \
    PHP_GIT2_FE(git_describe_commit)            \
    PHP_GIT2_FE(git_describe_format)            \
    PHP_GIT2_FE(git_describe_result_free)       \
    PHP_GIT2_FE(git_describe_workdir)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
