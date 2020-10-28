/*
 * patch.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_PATCH_H
#define PHPGIT2_PATCH_H
#include "diff.h"

namespace php_git2
{

    // Explicitly specialize git2_resource destructor for git_patch.
    template<> php_git_patch::~git2_resource()
    {
        git_patch_free(handle);
    }

    // Provide a type to return a git_diff_hunk value.
    class php_git_diff_hunk
    {
    public:
        ZTS_CONSTRUCTOR(php_git_diff_hunk)

        const git_diff_hunk** byval_git2(unsigned p = std::numeric_limits<unsigned>::max())
        {
            return &hunk;
        }

        void ret(zval* return_value)
        {
            convert_diff_hunk(return_value,hunk);
        }

    private:
        const git_diff_hunk* hunk;
    };

    // Provide a type to return a git_diff_line value.
    class php_git_diff_line
    {
    public:
        ZTS_CONSTRUCTOR(php_git_diff_line)

        const git_diff_line** byval_git2(unsigned p = std::numeric_limits<unsigned>::max())
        {
            return &line;
        }

        void ret(zval* return_value)
        {
            convert_diff_line(return_value,line);
        }

    private:
        const git_diff_line* line;
    };

} // namespace php_git2

// Funtions

static constexpr auto ZIF_GIT_PATCH_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_patch>
        >
    >;

static constexpr auto ZIF_GIT_PATCH_FROM_BLOB_AND_BUFFER = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_patch**,
        const git_blob*,
        const char*,
        const char*,
        size_t,
        const char*,
        const git_diff_options*>::func<git_patch_from_blob_and_buffer>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_patch>,
        php_git2::php_resource_null<php_git2::php_git_blob>,
        php_git2::php_nullable_string,
        php_git2::php_nullable_string,
        php_git2::php_long_cast<size_t>,
        php_git2::php_nullable_string,
        php_git2::php_git_diff_options
        >,
    1,
    php_git2::sequence<1,2,3,4,5,6>,
    php_git2::sequence<0,1,2,3,4,5,6>,
    php_git2::sequence<0,0,1,2,3,4,5>
    >;

static constexpr auto ZIF_GIT_PATCH_FROM_BLOBS = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_patch**,
        const git_blob*,
        const char*,
        const git_blob*,
        const char*,
        const git_diff_options*>::func<git_patch_from_blobs>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_patch>,
        php_git2::php_resource_null<php_git2::php_git_blob>,
        php_git2::php_nullable_string,
        php_git2::php_resource_null<php_git2::php_git_blob>,
        php_git2::php_nullable_string,
        php_git2::php_git_diff_options
        >,
    1,
    php_git2::sequence<1,2,3,4,5>,
    php_git2::sequence<0,1,2,3,4,5>,
    php_git2::sequence<0,0,1,2,3,4>
    >;

static constexpr auto ZIF_GIT_PATCH_FROM_BUFFERS = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_patch**,
        const void*,
        size_t,
        const char*,
        const char*,
        size_t,
        const char*,
        const git_diff_options*>::func<git_patch_from_buffers>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_patch>,
        php_git2::connector_wrapper<php_git2::php_string_length_connector_null<size_t> >,
        php_git2::php_nullable_string,
        php_git2::php_nullable_string,
        php_git2::connector_wrapper<php_git2::php_string_length_connector_null<size_t> >,
        php_git2::php_nullable_string,
        php_git2::php_nullable_string,
        php_git2::php_git_diff_options
        >,
    1,
    php_git2::sequence<2,3,5,6,7>,
    php_git2::sequence<0,2,1,3,5,4,6,7>,
    php_git2::sequence<0,0,0,1,2,2,3,4>
    >;

static constexpr auto ZIF_GIT_PATCH_FROM_DIFF = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_patch**,
        git_diff*,
        size_t>::func<git_patch_from_diff>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_patch>,
        php_git2::php_resource<php_git2::php_git_diff>,
        php_git2::php_long_cast<size_t>
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
    >;

static constexpr auto ZIF_GIT_PATCH_GET_DELTA = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        const git_diff_delta*,
        const git_patch*>::func<git_patch_get_delta>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_patch>
        >,
    php_git2::php_git_diff_delta_rethandler<
        php_git2::php_resource<php_git2::php_git_patch>
        >
    >;

static constexpr auto ZIF_GIT_PATCH_GET_HUNK = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const git_diff_hunk**,
        size_t*,
        git_patch*,
        size_t>::func<git_patch_get_hunk>,
    php_git2::local_pack<
        php_git2::php_git_diff_hunk,
        php_git2::php_long_out<size_t>,
        php_git2::php_resource<php_git2::php_git_patch>,
        php_git2::php_long_cast<size_t>
        >,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>,
    php_git2::sequence<0,0,1,2>
    >;
ZEND_BEGIN_ARG_INFO_EX(git_patch_get_hunk_arginfo,0,0,3)
    ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

static constexpr auto ZIF_GIT_PATCH_GET_LINE_IN_HUNK = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const git_diff_line**,
        git_patch*,
        size_t,
        size_t>::func<git_patch_get_line_in_hunk>,
    php_git2::local_pack<
        php_git2::php_git_diff_line,
        php_git2::php_resource<php_git2::php_git_patch>,
        php_git2::php_long_cast<size_t>,
        php_git2::php_long_cast<size_t>
        >,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>,
    php_git2::sequence<0,0,1,2>
    >;

static constexpr auto ZIF_GIT_PATCH_LINE_STATS = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        size_t*,
        size_t*,
        size_t*,
        const git_patch*>::func<git_patch_line_stats>,
    php_git2::local_pack<
        php_git2::php_long_ref<size_t>,
        php_git2::php_long_out<size_t>,
        php_git2::php_long_out<size_t>,
        php_git2::php_resource<php_git2::php_git_patch>
        >,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>,
    php_git2::sequence<0,0,1,2>
    >;
ZEND_BEGIN_ARG_INFO_EX(git_patch_line_stats_arginfo,0,0,3)
    ZEND_ARG_PASS_INFO(1)
    ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

static constexpr auto ZIF_GIT_PATCH_NUM_HUNKS = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        const git_patch*>::func<git_patch_num_hunks>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_patch>
        >,
    0
    >;

static constexpr auto ZIF_GIT_PATCH_NUM_LINES_IN_HUNK = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_patch*,
        size_t>::func<git_patch_num_lines_in_hunk>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_patch>,
        php_git2::php_long_cast<size_t>
        >,
    php_git2::php_notfound_retval_rethandler
    >;

static constexpr auto ZIF_GIT_PATCH_PRINT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_patch*,
        git_diff_line_cb,
        void*>::func<git_patch_print>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_patch>,
        php_git2::php_callback_handler<php_git2::diff_line_callback>,
        php_git2::php_callback_sync
        >,
    -1,
    php_git2::sequence<0,2,2>, // pass callback in twice for function and payload
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_GIT_PATCH_SIZE = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        git_patch*,
        int,
        int,
        int>::func<git_patch_size>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_patch>,
        php_git2::php_bool,
        php_git2::php_bool,
        php_git2::php_bool
        >,
    0
    >;

static constexpr auto ZIF_GIT_PATCH_TO_BUF = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        git_patch*>::func<git_patch_to_buf>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_resource<php_git2::php_git_patch>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

// Function Entries

#define GIT_PATCH_FE                                                    \
    PHP_GIT2_FE(git_patch_free,ZIF_GIT_PATCH_FREE,NULL)                 \
    PHP_GIT2_FE(git_patch_from_blob_and_buffer,ZIF_GIT_PATCH_FROM_BLOB_AND_BUFFER,NULL) \
    PHP_GIT2_FE(git_patch_from_blobs,ZIF_GIT_PATCH_FROM_BLOBS,NULL)     \
    PHP_GIT2_FE(git_patch_from_buffers,ZIF_GIT_PATCH_FROM_BUFFERS,NULL) \
    PHP_GIT2_FE(git_patch_from_diff,ZIF_GIT_PATCH_FROM_DIFF,NULL)       \
    PHP_GIT2_FE(git_patch_get_delta,ZIF_GIT_PATCH_GET_DELTA,NULL)       \
    PHP_GIT2_FE(git_patch_get_hunk,ZIF_GIT_PATCH_GET_HUNK,git_patch_get_hunk_arginfo) \
    PHP_GIT2_FE(git_patch_get_line_in_hunk,ZIF_GIT_PATCH_GET_LINE_IN_HUNK,NULL) \
    PHP_GIT2_FE(git_patch_line_stats,ZIF_GIT_PATCH_LINE_STATS,git_patch_line_stats_arginfo) \
    PHP_GIT2_FE(git_patch_num_hunks,ZIF_GIT_PATCH_NUM_HUNKS,NULL)       \
    PHP_GIT2_FE(git_patch_num_lines_in_hunk,ZIF_GIT_PATCH_NUM_LINES_IN_HUNK,NULL) \
    PHP_GIT2_FE(git_patch_print,ZIF_GIT_PATCH_PRINT,NULL)               \
    PHP_GIT2_FE(git_patch_size,ZIF_GIT_PATCH_SIZE,NULL)                 \
    PHP_GIT2_FE(git_patch_to_buf,ZIF_GIT_PATCH_TO_BUF,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
