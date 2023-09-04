/*
 * reflog.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_REFLOG_H
#define PHPGIT2_REFLOG_H

#include "stubs/reflog_arginfo.h"

namespace php_git2
{
    // Explicitly specialize git2_resource destructor for git_reference.
    template<> php_git_reflog::~git2_resource()
    {
        git_reflog_free(handle);
    }

    using reflog_entry_byindex_args = local_pack<
        php_resource<php_git_reflog>,
        php_long_cast<size_t>
        >;

    // Define rethandler for returning a git_reflog_entry array.
    class php_git_reflog_entry_rethandler
    {
    public:
        template<typename... Ts>
        bool ret(const git_reflog_entry* retval,zval* return_value,local_pack<Ts...>& pack)
        {
            if (retval == nullptr) {
                RETVAL_FALSE;
                return true;
            }

            convert_reflog_entry(return_value,retval);
            return true;
        }
    };

} // namespace php_git2

static constexpr auto ZIF_git_reflog_append = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_reflog*,
        const git_oid*,
        const git_signature*,
        const char*>::func<git_reflog_append>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reflog>,
        php_git2::php_git_oid_fromstr,
        php_git2::php_resource<php_git2::php_git_signature>,
        php_git2::php_string_nullable
        >
    >;

static constexpr auto ZIF_git_reflog_delete = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*>::func<git_reflog_delete>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_reflog_drop = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_reflog*,
        size_t,
        int>::func<git_reflog_drop>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reflog>,
        php_git2::php_long_cast<size_t>,
        php_git2::php_bool
        >,
    php_git2::php_boolean_notfound_rethandler
    >;

static constexpr auto ZIF_git_reflog_entry_byindex = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        const git_reflog_entry*,
        const git_reflog*,
        size_t>::func<git_reflog_entry_byindex>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reflog>,
        php_git2::php_long_cast<size_t>
        >,
    php_git2::php_git_reflog_entry_rethandler
    >;

static constexpr auto ZIF_git_reflog_entrycount = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        git_reflog*>::func<git_reflog_entrycount>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reflog>
        >,
    0
    >;

static constexpr auto ZIF_git_reflog_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_reflog>
        >
    >;

static constexpr auto ZIF_git_reflog_read = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_reflog**,
        git_repository*,
        const char*>::func<git_reflog_read>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_reflog>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_reflog_rename = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*,
        const char*>::func<git_reflog_rename>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_reflog_write = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_reflog*>::func<git_reflog_write>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reflog>
        >
    >;

#define GIT_REFLOG_FE                           \
    PHP_GIT2_FE(git_reflog_append)              \
    PHP_GIT2_FE(git_reflog_delete)              \
    PHP_GIT2_FE(git_reflog_drop)                \
    PHP_GIT2_FE(git_reflog_entry_byindex)       \
    PHP_GIT2_FE(git_reflog_entrycount)          \
    PHP_GIT2_FE(git_reflog_free)                \
    PHP_GIT2_FE(git_reflog_read)                \
    PHP_GIT2_FE(git_reflog_rename)              \
    PHP_GIT2_FE(git_reflog_write)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
