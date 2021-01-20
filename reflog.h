/*
 * reflog.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_REFLOG_H
#define PHPGIT2_REFLOG_H

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

static constexpr auto ZIF_GIT_REFLOG_APPEND = zif_php_git2_function<
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

static constexpr auto ZIF_GIT_REFLOG_DELETE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*>::func<git_reflog_delete>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_GIT_REFLOG_DROP = zif_php_git2_function_rethandler<
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

static constexpr auto ZIF_GIT_REFLOG_ENTRY_BYINDEX = zif_php_git2_function_rethandler<
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

static constexpr auto ZIF_GIT_REFLOG_ENTRYCOUNT = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        git_reflog*>::func<git_reflog_entrycount>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reflog>
        >,
    0
    >;

static constexpr auto ZIF_GIT_REFLOG_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_reflog>
        >
    >;

static constexpr auto ZIF_GIT_REFLOG_READ = zif_php_git2_function<
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

static constexpr auto ZIF_GIT_REFLOG_RENAME = zif_php_git2_function<
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

static constexpr auto ZIF_GIT_REFLOG_WRITE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_reflog*>::func<git_reflog_write>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_reflog>
        >
    >;

#define GIT_REFLOG_FE                                                   \
    PHP_GIT2_FE(git_reflog_append,ZIF_GIT_REFLOG_APPEND,NULL)           \
    PHP_GIT2_FE(git_reflog_delete,ZIF_GIT_REFLOG_DELETE,NULL)           \
    PHP_GIT2_FE(git_reflog_drop,ZIF_GIT_REFLOG_DROP,NULL)               \
    PHP_GIT2_FE(git_reflog_entry_byindex,ZIF_GIT_REFLOG_ENTRY_BYINDEX,NULL) \
    PHP_GIT2_FE(git_reflog_entrycount,ZIF_GIT_REFLOG_ENTRYCOUNT,NULL)   \
    PHP_GIT2_FE(git_reflog_free,ZIF_GIT_REFLOG_FREE,NULL)               \
    PHP_GIT2_FE(git_reflog_read,ZIF_GIT_REFLOG_READ,NULL)               \
    PHP_GIT2_FE(git_reflog_rename,ZIF_GIT_REFLOG_RENAME,NULL)           \
    PHP_GIT2_FE(git_reflog_write,ZIF_GIT_REFLOG_WRITE,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
