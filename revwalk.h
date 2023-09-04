/*
 * revwalk.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_REVWALK_H
#define PHPGIT2_REVWALK_H

#include "stubs/revwalk_arginfo.h"

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

        bool ret(int retval,zval* return_value,lpack& pack)
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

    // Provide alias for revwalk async callback type.

    class php_git_revwalk_ex;
    using revwalk_async_callback_t =
        php_git2::php_callback_async_existing<php_git2::php_git_revwalk_ex>;

    // Subclass php_git_revwalk to store callback information. Any revwalk must
    // be managed by this class.

    class php_git_revwalk_ex:
        public php_git_revwalk
    {
        friend revwalk_async_callback_t;
    public:
        // NOTE: The callback may be set by a php_callback_async connector
        // instance if the user calls git_revwalk_add_hide_cb. Only one callback
        // is added at a time.

        // TODO Support more than one callback at a time.

        php_git_revwalk_ex():
            cb(nullptr)
        {
        }

        virtual ~php_git_revwalk_ex()
        {
            if (cb != nullptr) {
                cb->~php_callback_sync();
                efree(cb);
            }
        }

    private:
        php_callback_sync* cb;
    };

} // namespace php_git2

// Functions:

static constexpr auto ZIF_git_revwalk_new = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_revwalk**,
        git_repository*>::func<git_revwalk_new>,
    php_git2::local_pack<
        // NOTE: We must create a resource instance that allocates a callback!
        php_git2::php_resource_ref<php_git2::php_git_revwalk_ex>,
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_revwalk_push = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_revwalk*,
        const git_oid*>::func<git_revwalk_push>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>,
        php_git2::php_git_oid_fromstr
        >
    >;

static constexpr auto ZIF_git_revwalk_next = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_revwalk*>::func<git_revwalk_next>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_revwalk>
        >,
    php_git2::revwalk_next_rethandler,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_revwalk_hide = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_revwalk*,
        const git_oid*>::func<git_revwalk_hide>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>,
        php_git2::php_git_oid_fromstr
        >
    >;

static constexpr auto ZIF_git_revwalk_sorting = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_revwalk*,
        unsigned int>::func<git_revwalk_sorting>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>,
        php_git2::php_long
        >
    >;

static constexpr auto ZIF_git_revwalk_reset = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_revwalk*>::func<git_revwalk_reset>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>
        >
    >;

static constexpr auto ZIF_git_revwalk_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_revwalk>
        >
    >;

static constexpr auto ZIF_git_revwalk_add_hide_cb = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_revwalk*,
        git_revwalk_hide_cb,
        void*>::func<git_revwalk_add_hide_cb>,
    php_git2::local_pack<
        php_git2::connector_wrapper<php_git2::revwalk_async_callback_t>,
        php_git2::php_resource<php_git2::php_git_revwalk_ex>,
        php_git2::php_callback_handler<php_git2::revwalk_hide_callback>
        >,
    -1,
    php_git2::sequence<1,0,0>,
    php_git2::sequence<1,2,0>
    >;

static constexpr auto ZIF_git_revwalk_hide_glob = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_revwalk*,
        const char*>::func<git_revwalk_hide_glob>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_revwalk_hide_head = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_revwalk*>::func<git_revwalk_hide_head>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>
        >
    >;

static constexpr auto ZIF_git_revwalk_hide_ref = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_revwalk*,
        const char*>::func<git_revwalk_hide_ref>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_revwalk_push_glob = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_revwalk*,
        const char*>::func<git_revwalk_push_glob>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_revwalk_push_head = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_revwalk*>::func<git_revwalk_push_head>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>
        >
    >;

static constexpr auto ZIF_git_revwalk_push_range = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_revwalk*,
        const char*>::func<git_revwalk_push_range>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_revwalk_push_ref = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_revwalk*,
        const char*>::func<git_revwalk_push_ref>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_revwalk_repository = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_repository*,
        git_revwalk*>::func<git_revwalk_repository>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>,
        php_git2::php_resource_ref<php_git2::php_git_repository_nofree>
        >,
    php_git2::php_owner_rethandler<php_git2::php_git_revwalk>,
    php_git2::sequence<0>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_git_revwalk_simplify_first_parent = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_revwalk*>::func<git_revwalk_simplify_first_parent>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>
        >
    >;

// Function Entries:

#define GIT_REVWALK_FE                              \
    PHP_GIT2_FE(git_revwalk_new)                    \
    PHP_GIT2_FE(git_revwalk_push)                   \
    PHP_GIT2_FE(git_revwalk_next)                   \
    PHP_GIT2_FE(git_revwalk_hide)                   \
    PHP_GIT2_FE(git_revwalk_sorting)                \
    PHP_GIT2_FE(git_revwalk_reset)                  \
    PHP_GIT2_FE(git_revwalk_free)                   \
    PHP_GIT2_FE(git_revwalk_add_hide_cb)            \
    PHP_GIT2_FE(git_revwalk_hide_glob)              \
    PHP_GIT2_FE(git_revwalk_hide_head)              \
    PHP_GIT2_FE(git_revwalk_hide_ref)               \
    PHP_GIT2_FE(git_revwalk_push_glob)              \
    PHP_GIT2_FE(git_revwalk_push_head)              \
    PHP_GIT2_FE(git_revwalk_push_range)             \
    PHP_GIT2_FE(git_revwalk_push_ref)               \
    PHP_GIT2_FE(git_revwalk_repository)             \
    PHP_GIT2_FE(git_revwalk_simplify_first_parent)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
