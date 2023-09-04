/*
 * status.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_STATUS_H
#define PHPGIT2_STATUS_H

#include "diff.h"
#include "stubs/status_arginfo.h"

namespace php_git2
{
    // Explicitly specialize git2_resource destructor for git_status_list.

    template<> php_git_status_list::~git2_resource()
    {
        git_status_list_free(handle);
    }

    // Provide a type for converting a PHP array into a git_status_options
    // struct.

    class php_git_status_options:
        public php_option_array
    {
    public:
        php_git_status_options()
        {
            git_status_init_options(&opts,GIT_STATUS_OPTIONS_VERSION);
        }

        git_status_options* byval_git2()
        {
            if (!is_null()) {
                array_wrapper arr(value);

                GIT2_ARRAY_LOOKUP_LONG(arr,show,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,flags,opts);
                GIT2_ARRAY_LOOKUP_SUBOBJECT(arr,strarray,pathspec,opts);
            }

            return &opts;
        }

    private:
        git_status_options opts;
        php_git_strarray_byval_array strarray;
    };

    // Provide a type for returning a git_status_entry.

    template<typename... Ts>
    class php_git_status_entry_rethandler
    {
    public:
        bool ret(const git_status_entry* ent,zval* return_value,local_pack<Ts...>& pack)
        {
            if (ent == nullptr) {
                RETVAL_FALSE;
            }
            else {
                convert_status_entry(return_value,ent);
            }

            return true;
        }
    };

    // Provide a type for handling the return value from git_status_file.

    class php_git_status_file_rethandler
    {
    public:
        using pack_type = local_pack<
            php_long_ref<unsigned int>,
            php_resource<php_git_repository>,
            php_string
            >;

        bool ret(int result,zval* return_value,pack_type& pack)
        {
            if (result == 0) {
                pack.get<0>().ret(return_value);
                return true;
            }
            if (result == GIT_ENOTFOUND) {
                RETVAL_FALSE;
                return true;
            }
            if (result == GIT_EAMBIGUOUS) {
                RETVAL_NULL();
                return true;
            }

            return false;
        }
    };

} // namespace php_git2

static constexpr auto ZIF_git_status_byindex = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        const git_status_entry*,
        git_status_list*,
        size_t>::func<git_status_byindex>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_status_list>,
        php_git2::php_long_cast<size_t>
        >,
    php_git2::php_git_status_entry_rethandler<
        php_git2::php_resource<php_git2::php_git_status_list>,
        php_git2::php_long_cast<size_t>
        >
    >;

static constexpr auto ZIF_git_status_file = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        unsigned int*,
        git_repository*,
        const char*>::func<git_status_file>,
    php_git2::php_git_status_file_rethandler::pack_type,
    php_git2::php_git_status_file_rethandler,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_status_foreach = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        git_status_cb,
        void*>::func<git_status_foreach>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_callback_handler<php_git2::status_callback>,
        php_git2::php_callback_sync
        >,
    -1,
    php_git2::sequence<0,2,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_status_foreach_ext = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const git_status_options*,
        git_status_cb,
        void*>::func<git_status_foreach_ext>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_git_status_options,
        php_git2::php_callback_handler<php_git2::status_callback>,
        php_git2::php_callback_sync
        >,
    -1,
    php_git2::sequence<0,1,3,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_status_list_entrycount = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        git_status_list*>::func<git_status_list_entrycount>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_status_list>
        >,
    0
    >;

static constexpr auto ZIF_git_status_list_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_status_list>
        >
    >;

static constexpr auto ZIF_git_status_list_get_perfdata = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_diff_perfdata*,
        const git_status_list*>::func<git_status_list_get_perfdata>,
    php_git2::local_pack<
        php_git2::php_git_diff_perfdata,
        php_git2::php_resource<php_git2::php_git_status_list>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_status_list_new = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_status_list**,
        git_repository*,
        const git_status_options*>::func<git_status_list_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_status_list>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_git_status_options
        >,
    -1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_status_should_ignore = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        int*,
        git_repository*,
        const char*>::func<git_status_should_ignore>,
    php_git2::local_pack<
        php_git2::php_bool_ref<int>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

#define GIT_STATUS_FE                           \
    PHP_GIT2_FE(git_status_byindex)             \
    PHP_GIT2_FE(git_status_file)                \
    PHP_GIT2_FE(git_status_foreach)             \
    PHP_GIT2_FE(git_status_foreach_ext)         \
    PHP_GIT2_FE(git_status_list_entrycount)     \
    PHP_GIT2_FE(git_status_list_free)           \
    PHP_GIT2_FE(git_status_list_get_perfdata)   \
    PHP_GIT2_FE(git_status_list_new)            \
    PHP_GIT2_FE(git_status_should_ignore)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
