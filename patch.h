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
        const git_diff_hunk** byval_git2()
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
        const git_diff_line** byval_git2()
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

static constexpr auto ZIF_git_patch_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_patch>
        >
    >;

static constexpr auto ZIF_git_patch_from_blob_and_buffer = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_patch**,
        const git_blob*,
        const char*,
        const void*,
        size_t,
        const char*,
        const git_diff_options*>::func<git_patch_from_blob_and_buffer>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_patch>,
        php_git2::php_resource_nullable<php_git2::php_git_blob>,
        php_git2::php_string_nullable,
        php_git2::connector_wrapper<
            php_git2::php_string_length_connector_nullable<size_t>
            >,
        php_git2::php_string_nullable,
        php_git2::php_string_nullable,
        php_git2::php_git_diff_options
        >,
    1,
    php_git2::sequence<1,2,4,5,6>,
    php_git2::sequence<0,1,2,4,3,5,6>
    >;

static constexpr auto ZIF_git_patch_from_blobs = zif_php_git2_function<
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
        php_git2::php_resource_nullable<php_git2::php_git_blob>,
        php_git2::php_string_nullable,
        php_git2::php_resource_nullable<php_git2::php_git_blob>,
        php_git2::php_string_nullable,
        php_git2::php_git_diff_options
        >,
    1,
    php_git2::sequence<1,2,3,4,5>,
    php_git2::sequence<0,1,2,3,4,5>
    >;

static constexpr auto ZIF_git_patch_from_buffers = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_patch**,
        const void*,
        size_t,
        const char*,
        const void*,
        size_t,
        const char*,
        const git_diff_options*>::func<git_patch_from_buffers>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_patch>,
        php_git2::connector_wrapper<
            php_git2::php_string_length_connector_nullable<size_t>
            >,
        php_git2::php_string_nullable,
        php_git2::php_string_nullable,
        php_git2::connector_wrapper<
            php_git2::php_string_length_connector_nullable<size_t>
            >,
        php_git2::php_string_nullable,
        php_git2::php_string_nullable,
        php_git2::php_git_diff_options
        >,
    1,
    php_git2::sequence<2,3,5,6,7>,
    php_git2::sequence<0,2,1,3,5,4,6,7>
    >;

static constexpr auto ZIF_git_patch_from_diff = zif_php_git2_function<
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
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_patch_get_delta = zif_php_git2_function_rethandler<
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

static constexpr auto ZIF_git_patch_get_hunk = zif_php_git2_function<
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
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_patch_get_line_in_hunk = zif_php_git2_function<
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
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_patch_line_stats = zif_php_git2_function<
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
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_patch_num_hunks = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        const git_patch*>::func<git_patch_num_hunks>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_patch>
        >,
    0
    >;

static constexpr auto ZIF_git_patch_num_lines_in_hunk = zif_php_git2_function_rethandler<
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

static constexpr auto ZIF_git_patch_print = zif_php_git2_function<
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
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_patch_size = zif_php_git2_function<
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

static constexpr auto ZIF_git_patch_to_buf = zif_php_git2_function<
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
    php_git2::sequence<0,1>
    >;

// Function Entries

#define GIT_PATCH_FE                            \
    PHP_GIT2_FE(git_patch_free)                 \
    PHP_GIT2_FE(git_patch_from_blob_and_buffer) \
    PHP_GIT2_FE(git_patch_from_blobs)           \
    PHP_GIT2_FE(git_patch_from_buffers)         \
    PHP_GIT2_FE(git_patch_from_diff)            \
    PHP_GIT2_FE(git_patch_get_delta)            \
    PHP_GIT2_FE(git_patch_get_hunk)             \
    PHP_GIT2_FE(git_patch_get_line_in_hunk)     \
    PHP_GIT2_FE(git_patch_line_stats)           \
    PHP_GIT2_FE(git_patch_num_hunks)            \
    PHP_GIT2_FE(git_patch_num_lines_in_hunk)    \
    PHP_GIT2_FE(git_patch_print)                \
    PHP_GIT2_FE(git_patch_size)                 \
    PHP_GIT2_FE(git_patch_to_buf)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
