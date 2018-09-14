/*
 * reflog.h
 *
 * This file is a part of php-git2.
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

} // namespace php_git2
/*
static constexpr auto ZIF_GIT_REFLOG_APPEND = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_reflog_append>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REFLOG_DELETE = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_reflog_delete>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REFLOG_DROP = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_reflog_drop>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REFLOG_ENTRY_BYINDEX = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_reflog_entry_byindex>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REFLOG_ENTRYCOUNT = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_reflog_entrycount>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REFLOG_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_reflog>
        >
    >;

static constexpr auto ZIF_GIT_REFLOG_READ = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_reflog_read>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REFLOG_RENAME = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_reflog_rename>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REFLOG_WRITE = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_reflog_write>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;
*/
#define GIT_REFLOG_FE                                                   \
    PHP_GIT2_UNIMPLEMENTED(git_reflog_append,ZIF_GIT_REFLOG_APPEND,NULL) \
    PHP_GIT2_UNIMPLEMENTED(git_reflog_delete,ZIF_GIT_REFLOG_DELETE,NULL) \
    PHP_GIT2_UNIMPLEMENTED(git_reflog_drop,ZIF_GIT_REFLOG_DROP,NULL)    \
    PHP_GIT2_UNIMPLEMENTED(git_reflog_entry_byindex,ZIF_GIT_REFLOG_ENTRY_BYINDEX,NULL) \
    PHP_GIT2_UNIMPLEMENTED(git_reflog_entrycount,ZIF_GIT_REFLOG_ENTRYCOUNT,NULL) \
    PHP_GIT2_UNIMPLEMENTED(git_reflog_free,ZIF_GIT_REFLOG_FREE,NULL)    \
    PHP_GIT2_UNIMPLEMENTED(git_reflog_read,ZIF_GIT_REFLOG_READ,NULL)    \
    PHP_GIT2_UNIMPLEMENTED(git_reflog_rename,ZIF_GIT_REFLOG_RENAME,NULL) \
    PHP_GIT2_UNIMPLEMENTED(git_reflog_write,ZIF_GIT_REFLOG_WRITE,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
