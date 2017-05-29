/*
 * repository.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_REPOSITORY_H
#define PHPGIT2_REPOSITORY_H
extern "C" {
#include <git2/sys/repository.h>
}

namespace php_git2
{

    // Explicitly specialize git2_resource destructor for git_repository.
    template<> php_git_repository::~git2_resource()
    {
        git_repository_free(handle);
    }

} // namespace php_git2

// Functions:

static constexpr auto ZIF_GIT_REPOSITORY_INIT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository**,
        const char*,
        unsigned int>::func<git_repository_init>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_bool>,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1> >;

static constexpr auto ZIF_GIT_REPOSITORY_NEW = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository**>::func<git_repository_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_repository> >,
    1,
    php_git2::sequence<>,
    php_git2::sequence<0>,
    php_git2::sequence<0> >;

static constexpr auto ZIF_GIT_REPOSITORY_FREE = zif_php_git2_function_void<
    php_git2::func_wrapper<
        void,
        git_repository*>::func<git_repository_free>,
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_repository> > >;

static constexpr auto ZIF_GIT_REPOSITORY_OPEN = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository**,
        const char*>::func<git_repository_open>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_repository>,
        php_git2::php_string>,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0> >;

static constexpr auto ZIF_GIT_REPOSITORY_OPEN_BARE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository**,
        const char*>::func<git_repository_open_bare>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_repository>,
        php_git2::php_string>,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0> >;

static constexpr auto ZIF_GIT_REPOSITORY__CLEANUP = zif_php_git2_function_void<
    php_git2::func_wrapper<
        void,
        git_repository*>::func<git_repository__cleanup>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository> > >;

static constexpr auto ZIF_GIT_REPOSITORY_DETACH_HEAD = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*>::func<git_repository_detach_head>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository> > >;

static constexpr auto ZIF_GIT_REPOSITORY_DISCOVER = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        const char*,
        int,
        const char*>::func<git_repository_discover>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_string,
        php_git2::php_bool,
        php_git2::php_nullable_string>,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>,
    php_git2::sequence<0,0,1,2> >;

static constexpr auto ZIF_GIT_REPOSITORY_GET_NAMESPACE = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        git_repository*>::func<git_repository_get_namespace>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository> >,
    0 >;

static constexpr auto ZIF_GIT_REPOSITORY_PATH = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        git_repository*>::func<git_repository_path>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository> >,
    0 >;

static constexpr auto ZIF_GIT_REPOSITORY_HASHFILE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_repository*,
        const char*,
        git_otype,
        const char*>::func<git_repository_hashfile>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_long_cast<git_otype>,
        php_git2::php_nullable_string>,
    1,
    php_git2::sequence<1,2,3,4>,
    php_git2::sequence<0,1,2,3,4>,
    php_git2::sequence<0,0,1,2,3> >;

static constexpr auto ZIF_GIT_REPOSITORY_ODB = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_odb**,
        git_repository*>::func<git_repository_odb>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_odb>,
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    php_git2::sequence<0,1>, // Make the ODB dependent on the repository.
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

static constexpr auto ZIF_GIT_REPOSITORY_SET_ODB = zif_php_git2_function_setdeps_void<
    php_git2::func_wrapper<
        void,
        git_repository*,
        git_odb*>::func<git_repository_set_odb>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_odb>
        >,
    php_git2::sequence<0,1>
    >;

// Function Entries:

#define GIT_REPOSITORY_FE \
    PHP_GIT2_FE(git_repository_init,ZIF_GIT_REPOSITORY_INIT,NULL)           \
    PHP_GIT2_FE(git_repository_new,ZIF_GIT_REPOSITORY_NEW,NULL)             \
    PHP_GIT2_FE(git_repository_free,ZIF_GIT_REPOSITORY_FREE,NULL)           \
    PHP_GIT2_FE(git_repository_open,ZIF_GIT_REPOSITORY_OPEN,NULL)           \
    PHP_GIT2_FE(git_repository_open_bare,ZIF_GIT_REPOSITORY_OPEN_BARE,NULL) \
    PHP_GIT2_FE(git_repository__cleanup,ZIF_GIT_REPOSITORY__CLEANUP,NULL) \
    PHP_GIT2_FE(git_repository_detach_head,ZIF_GIT_REPOSITORY_DETACH_HEAD,NULL) \
    PHP_GIT2_FE(git_repository_discover,ZIF_GIT_REPOSITORY_DISCOVER,NULL) \
    PHP_GIT2_FE(git_repository_get_namespace,ZIF_GIT_REPOSITORY_GET_NAMESPACE,NULL) \
    PHP_GIT2_FE(git_repository_path,ZIF_GIT_REPOSITORY_PATH,NULL) \
    PHP_GIT2_FE(git_repository_hashfile,ZIF_GIT_REPOSITORY_HASHFILE,NULL) \
    PHP_GIT2_FE(git_repository_odb,ZIF_GIT_REPOSITORY_ODB,NULL)         \
    PHP_GIT2_FE(git_repository_set_odb,ZIF_GIT_REPOSITORY_SET_ODB,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
