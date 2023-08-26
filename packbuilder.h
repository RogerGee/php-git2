/*
 * packbuilder.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_PACKBUILDER_H
#define PHPGIT2_PACKBUILDER_H

#include "stubs/packbuilder_arginfo.h"

namespace php_git2
{

    // Specialize resource destructor for git_packbuilder
    template<> inline php_git_packbuilder::~git2_resource()
    {
        git_packbuilder_free(handle);
    }

    class php_git_packbuilder_with_callback;

    // Define shortcut to packbuilder async type.
    using packbuilder_async_callback_t = php_git2::php_callback_async_existing<
        php_git_packbuilder_with_callback,
        php_callback_sync_nullable
        >;

    class php_git_packbuilder_with_callback:
        public php_git_packbuilder
    {
        friend packbuilder_async_callback_t;
    public:
        php_git_packbuilder_with_callback():
            cb(nullptr)
        {
        }

        virtual ~php_git_packbuilder_with_callback()
        {
            // Free callback object.
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

static constexpr auto ZIF_git_packbuilder_new = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_packbuilder**,
        git_repository*>::func<git_packbuilder_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_packbuilder_with_callback>,
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_packbuilder_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_packbuilder_with_callback>
        >
    >;

static constexpr auto ZIF_git_packbuilder_hash = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        git_packbuilder*>::func<git_packbuilder_hash>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder_with_callback>
        >,
    0
    >;

static constexpr auto ZIF_git_packbuilder_insert_commit = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_packbuilder*,
        const git_oid*>::func<git_packbuilder_insert_commit>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder_with_callback>,
        php_git2::php_git_oid_fromstr
        >
    >;

static constexpr auto ZIF_git_packbuilder_insert_walk = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_packbuilder*,
        git_revwalk*>::func<git_packbuilder_insert_walk>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder_with_callback>,
        php_git2::php_resource<php_git2::php_git_revwalk>
        >
    >;

static constexpr auto ZIF_git_packbuilder_insert_recur = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_packbuilder*,
        const git_oid*,
        const char*
        >::func<git_packbuilder_insert_recur>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder_with_callback>,
        php_git2::php_git_oid_fromstr,
        php_git2::php_string_nullable
        >
    >;

static constexpr auto ZIF_git_packbuilder_insert_tree = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_packbuilder*,
        const git_oid*
        >::func<git_packbuilder_insert_tree>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder_with_callback>,
        php_git2::php_git_oid_fromstr
        >
    >;

static constexpr auto ZIF_git_packbuilder_insert = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_packbuilder*,
        const git_oid*,
        const char*>::func<git_packbuilder_insert>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder_with_callback>,
        php_git2::php_git_oid_fromstr,
        php_git2::php_string_nullable
        >
    >;

static constexpr auto ZIF_git_packbuilder_foreach = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_packbuilder*,
        git_packbuilder_foreach_cb,
        void*>::func<git_packbuilder_foreach>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder_with_callback>,

        // NOTE: We allow a NULL callback (but not a null callback handler) to
        // support a stream payload, which is a feature provided by this
        // extension. If the payload isn't a stream, then the callback function
        // is called with no PHP userspace interaction.

        php_git2::php_callback_handler<php_git2::packbuilder_foreach_callback>,
        php_git2::php_callback_sync_nullable
        >,
    -1,
    php_git2::sequence<0,2,2>, // pass callback in twice for function and payload
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_packbuilder_set_threads = zif_php_git2_function<
    php_git2::func_wrapper<
        unsigned int,
        git_packbuilder*,
        unsigned int>::func<git_packbuilder_set_threads>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder_with_callback>,
        php_git2::php_long
        >,
    -1
    >;

static constexpr auto ZIF_git_packbuilder_set_callbacks = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_packbuilder*,
        git_packbuilder_progress,
        void*>::func<git_packbuilder_set_callbacks>,
    php_git2::local_pack<
        php_git2::php_callback_handler_nullable_async<
            php_git2::packbuilder_progress_callback,
            php_git2::packbuilder_async_callback_t
            >,
        php_git2::connector_wrapper<php_git2::packbuilder_async_callback_t>,
        php_git2::php_resource<php_git2::php_git_packbuilder_with_callback>
        >,
    -1,
    php_git2::sequence<2,1,1>, // pass callback twice
    php_git2::sequence<2,0,1>
    >;

static constexpr auto ZIF_git_packbuilder_object_count = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        git_packbuilder*
        >::func<git_packbuilder_object_count>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder_with_callback>
        >,
    0
    >;

static constexpr auto ZIF_git_packbuilder_write = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_packbuilder*,
        const char*,
        unsigned int,
        git_transfer_progress_cb,
        void*>::func<git_packbuilder_write>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder>,
        php_git2::php_string,
        php_git2::php_long,
        php_git2::connector_wrapper<
            php_git2::php_callback_handler_nullable_connector<
                php_git2::transfer_progress_callback
                >
            >,
        php_git2::php_callback_sync_nullable
        >,
    -1,
    php_git2::sequence<0,1,2,4,4>, // pass callback in twice
    php_git2::sequence<0,1,2,3,4>
    >;

static constexpr auto ZIF_git_packbuilder_written = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        git_packbuilder*>::func<git_packbuilder_written>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder>
        >,
    0
    >;

// Function Entries:

#define GIT_PACKBUILDER_FE                      \
    PHP_GIT2_FE(git_packbuilder_new)            \
    PHP_GIT2_FE(git_packbuilder_free)           \
    PHP_GIT2_FE(git_packbuilder_hash)           \
    PHP_GIT2_FE(git_packbuilder_insert_commit)  \
    PHP_GIT2_FE(git_packbuilder_insert_walk)    \
    PHP_GIT2_FE(git_packbuilder_insert_recur)   \
    PHP_GIT2_FE(git_packbuilder_insert_tree)    \
    PHP_GIT2_FE(git_packbuilder_insert)         \
    PHP_GIT2_FE(git_packbuilder_foreach)        \
    PHP_GIT2_FE(git_packbuilder_set_threads)    \
    PHP_GIT2_FE(git_packbuilder_set_callbacks)  \
    PHP_GIT2_FE(git_packbuilder_object_count)   \
    PHP_GIT2_FE(git_packbuilder_write)          \
    PHP_GIT2_FE(git_packbuilder_written)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
