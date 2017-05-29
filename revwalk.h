/*
 * revwalk.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_REVWALK_H
#define PHPGIT2_REVWALK_H

namespace php_git2
{

    // Explicitly specialize git2_resource destructor for git_revwalk.
    template<> php_git_revwalk::~git2_resource()
    {
        git_revwalk_free(handle);
    }

    struct revwalk_next_rethandler
    {
        using lpack = local_pack<php_git_oid,php_resource<php_git_revwalk> >;

        bool ret(int retval,zval* return_value,lpack&& pack)
        {
            if (retval == GIT_ITEROVER) {
                RETVAL_FALSE;
            }
            else if (retval == 0) {
                pack.get<0>().ret(return_value);
            }
            else {
                return false;
            }
            return true;
        }
    };

} // namespace php_git2

// Functions:

static constexpr auto ZIF_GIT_REVWALK_NEW = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_revwalk**,
        git_repository*>::func<git_revwalk_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_revwalk>,
        php_git2::php_resource<php_git2::php_git_repository> >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0> >;

static constexpr auto ZIF_GIT_REVWALK_PUSH = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_revwalk*,
        const git_oid*>::func<git_revwalk_push>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>,
        php_git2::php_git_oid_fromstr> >;

static constexpr auto ZIF_GIT_REVWALK_NEXT = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_revwalk*>::func<git_revwalk_next>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_revwalk> >,
    php_git2::revwalk_next_rethandler,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0> >;

static constexpr auto ZIF_GIT_REVWALK_HIDE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_revwalk*,
        const git_oid*>::func<git_revwalk_hide>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>,
        php_git2::php_git_oid_fromstr
        >
    >;

static constexpr auto ZIF_GIT_REVWALK_SORTING = zif_php_git2_function_void<
    php_git2::func_wrapper<
        void,
        git_revwalk*,
        unsigned int>::func<git_revwalk_sorting>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>,
        php_git2::php_long
        > 
    >;

static constexpr auto ZIF_GIT_REVWALK_RESET = zif_php_git2_function_void<
    php_git2::func_wrapper<
        void,
        git_revwalk*>::func<git_revwalk_reset>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk> > >;

static constexpr auto ZIF_GIT_REVWALK_FREE = zif_php_git2_function_void<
    php_git2::func_wrapper<
        void,
        git_revwalk*>::func<git_revwalk_free>,
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_revwalk> > >;

// Function Entries:

#define GIT_REVWALK_FE                                              \
    PHP_GIT2_FE(git_revwalk_new,ZIF_GIT_REVWALK_NEW,NULL)           \
    PHP_GIT2_FE(git_revwalk_push,ZIF_GIT_REVWALK_PUSH,NULL)         \
    PHP_GIT2_FE(git_revwalk_next,ZIF_GIT_REVWALK_NEXT,NULL)         \
    PHP_GIT2_FE(git_revwalk_hide,ZIF_GIT_REVWALK_HIDE,NULL)         \
    PHP_GIT2_FE(git_revwalk_sorting,ZIF_GIT_REVWALK_SORTING,NULL)   \
    PHP_GIT2_FE(git_revwalk_reset,ZIF_GIT_REVWALK_RESET,NULL)       \
    PHP_GIT2_FE(git_revwalk_free,ZIF_GIT_REVWALK_FREE,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
