/*
 * packbuilder.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_PACKBUILDER_H
#define PHPGIT2_PACKBUILDER_H

namespace php_git2
{

    // Specialize resource destructor for git_packbuilder
    template<> inline php_git_packbuilder::~git2_resource()
    {
        git_packbuilder_free(handle);
    }

} // namespace php_git2

// Functions:

static constexpr auto ZIF_GIT_PACKBUILDER_NEW = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_packbuilder**,
        git_repository*>::func<git_packbuilder_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_packbuilder>,
        php_git2::php_resource<php_git2::php_git_repository> >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0> >;

static constexpr auto ZIF_GIT_PACKBUILDER_FREE = zif_php_git2_function_void<
    php_git2::func_wrapper<
        void,
        git_packbuilder*>::func<git_packbuilder_free>,
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_packbuilder> > >;

static constexpr auto ZIF_GIT_PACKBUILDER_HASH = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        git_packbuilder*>::func<git_packbuilder_hash>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder> >,
    0>;

static constexpr auto ZIF_GIT_PACKBUILDER_INSERT_COMMIT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_packbuilder*,
        const git_oid*>::func<git_packbuilder_insert_commit>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder>,
        php_git2::php_git_oid_fromstr> >;

static constexpr auto ZIF_GIT_PACKBUILDER_INSERT_WALK = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_packbuilder*,
        git_revwalk*>::func<git_packbuilder_insert_walk>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder>,
        php_git2::php_resource<php_git2::php_git_revwalk>
        >
    >;

static constexpr auto ZIF_GIT_PACKBUILDER_INSERT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_packbuilder*,
        const git_oid*,
        const char*>::func<git_packbuilder_insert>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder>,
        php_git2::php_git_oid_fromstr,
        php_git2::php_nullable_string> >;

static constexpr auto ZIF_GIT_PACKBUILDER_FOREACH = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_packbuilder*,
        git_packbuilder_foreach_cb,
        void*>::func<git_packbuilder_foreach>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder>,
        php_git2::php_callback_handler<php_git2::packbuilder_foreach_callback>,
        php_git2::php_callback_sync>,
    -1,
    php_git2::sequence<0,2,2>, // pass callback in twice for function and payload
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1> >;

static constexpr auto ZIF_GIT_PACKBUILDER_SET_THREADS = zif_php_git2_function<
    php_git2::func_wrapper<
        unsigned int,
        git_packbuilder*,
        unsigned int>::func<git_packbuilder_set_threads>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder>,
        php_git2::php_long
        >,
    -1
    >;

// Function Entries:

#define GIT_PACKBUILDER_FE                                              \
    PHP_GIT2_FE(git_packbuilder_new,ZIF_GIT_PACKBUILDER_NEW,NULL)       \
    PHP_GIT2_FE(git_packbuilder_free,ZIF_GIT_PACKBUILDER_FREE,NULL)     \
    PHP_GIT2_FE(git_packbuilder_hash,ZIF_GIT_PACKBUILDER_HASH,NULL)     \
    PHP_GIT2_FE(git_packbuilder_insert_commit,ZIF_GIT_PACKBUILDER_INSERT_COMMIT,NULL) \
    PHP_GIT2_FE(git_packbuilder_insert_walk,ZIF_GIT_PACKBUILDER_INSERT_WALK,NULL) \
    PHP_GIT2_FE(git_packbuilder_insert,ZIF_GIT_PACKBUILDER_INSERT,NULL) \
    PHP_GIT2_FE(git_packbuilder_foreach,ZIF_GIT_PACKBUILDER_FOREACH,NULL) \
    PHP_GIT2_FE(git_packbuilder_set_threads,ZIF_GIT_PACKBUILDER_SET_THREADS,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
