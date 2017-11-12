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

    class php_git_packbuilder_with_callback;

    // Define shortcut to packbuilder async type.
    using packbuilder_async_callback_t =
        php_git2::php_callback_async_existing<php_git2::php_git_packbuilder_with_callback>;

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

static constexpr auto ZIF_GIT_PACKBUILDER_NEW = zif_php_git2_function_setdeps<
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
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

static constexpr auto ZIF_GIT_PACKBUILDER_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_packbuilder_with_callback>
        >
    >;

static constexpr auto ZIF_GIT_PACKBUILDER_HASH = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        git_packbuilder*>::func<git_packbuilder_hash>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder_with_callback> >,
    0
    >;

static constexpr auto ZIF_GIT_PACKBUILDER_INSERT_COMMIT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_packbuilder*,
        const git_oid*>::func<git_packbuilder_insert_commit>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder_with_callback>,
        php_git2::php_git_oid_fromstr>
    >;

static constexpr auto ZIF_GIT_PACKBUILDER_INSERT_WALK = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_packbuilder*,
        git_revwalk*>::func<git_packbuilder_insert_walk>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder_with_callback>,
        php_git2::php_resource<php_git2::php_git_revwalk>
        >
    >;

static constexpr auto ZIF_GIT_PACKBUILDER_INSERT_RECUR = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_packbuilder*,
        const git_oid*,
        const char*
        >::func<git_packbuilder_insert_recur>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder_with_callback>,
        php_git2::php_git_oid_fromstr,
        php_git2::php_nullable_string
        >
    >;

static constexpr auto ZIF_GIT_PACKBUILDER_INSERT_TREE = zif_php_git2_function<
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

static constexpr auto ZIF_GIT_PACKBUILDER_INSERT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_packbuilder*,
        const git_oid*,
        const char*>::func<git_packbuilder_insert>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder_with_callback>,
        php_git2::php_git_oid_fromstr,
        php_git2::php_nullable_string> >;

static constexpr auto ZIF_GIT_PACKBUILDER_FOREACH = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_packbuilder*,
        git_packbuilder_foreach_cb,
        void*>::func<git_packbuilder_foreach>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder_with_callback>,
        php_git2::php_callback_handler<php_git2::packbuilder_foreach_callback>,
        php_git2::php_callback_sync>,
    -1,
    php_git2::sequence<0,2,2>, // pass callback in twice for function and payload
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,1,2> >;

static constexpr auto ZIF_GIT_PACKBUILDER_SET_THREADS = zif_php_git2_function<
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

static constexpr auto ZIF_GIT_PACKBUILDER_SET_CALLBACKS = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_packbuilder*,
        git_packbuilder_progress,
        void*>::func<git_packbuilder_set_callbacks>,
    php_git2::local_pack<
        php_git2::connector_wrapper<php_git2::packbuilder_async_callback_t>,
        php_git2::php_resource<php_git2::php_git_packbuilder_with_callback>,
        php_git2::php_callback_handler<php_git2::packbuilder_progress_callback>
        >,
    -1,
    php_git2::sequence<1,0,0>, // pass callback twice
    php_git2::sequence<1,2,0>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_GIT_PACKBUILDER_OBJECT_COUNT = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        git_packbuilder*
        >::func<git_packbuilder_object_count>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder_with_callback>
        >,
    0
    >;

static constexpr auto ZIF_GIT_PACKBUILDER_WRITE = zif_php_git2_function<
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
        php_git2::php_callback_sync,
        php_git2::php_callback_handler<php_git2::transfer_progress_callback>
        >,
    -1,
    php_git2::sequence<0,1,2,3,3>, // pass callback in twice
    php_git2::sequence<0,1,2,4,3>,
    php_git2::sequence<0,1,2,0,3>
    >;

static constexpr auto ZIF_GIT_PACKBUILDER_WRITTEN = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        git_packbuilder*>::func<git_packbuilder_written>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_packbuilder>
        >,
    0
    >;

// Function Entries:

#define GIT_PACKBUILDER_FE                                              \
    PHP_GIT2_FE(git_packbuilder_new,ZIF_GIT_PACKBUILDER_NEW,NULL)       \
    PHP_GIT2_FE(git_packbuilder_free,ZIF_GIT_PACKBUILDER_FREE,NULL)     \
    PHP_GIT2_FE(git_packbuilder_hash,ZIF_GIT_PACKBUILDER_HASH,NULL)     \
    PHP_GIT2_FE(git_packbuilder_insert_commit,ZIF_GIT_PACKBUILDER_INSERT_COMMIT,NULL) \
    PHP_GIT2_FE(git_packbuilder_insert_walk,ZIF_GIT_PACKBUILDER_INSERT_WALK,NULL) \
    PHP_GIT2_FE(git_packbuilder_insert_recur,ZIF_GIT_PACKBUILDER_INSERT_RECUR,NULL) \
    PHP_GIT2_FE(git_packbuilder_insert_tree,ZIF_GIT_PACKBUILDER_INSERT_TREE,NULL) \
    PHP_GIT2_FE(git_packbuilder_insert,ZIF_GIT_PACKBUILDER_INSERT,NULL) \
    PHP_GIT2_FE(git_packbuilder_foreach,ZIF_GIT_PACKBUILDER_FOREACH,NULL) \
    PHP_GIT2_FE(git_packbuilder_set_threads,ZIF_GIT_PACKBUILDER_SET_THREADS,NULL) \
    PHP_GIT2_FE(git_packbuilder_set_callbacks,ZIF_GIT_PACKBUILDER_SET_CALLBACKS,NULL) \
    PHP_GIT2_FE(git_packbuilder_object_count,ZIF_GIT_PACKBUILDER_OBJECT_COUNT,NULL) \
    PHP_GIT2_FE(git_packbuilder_write,ZIF_GIT_PACKBUILDER_WRITE,NULL)   \
    PHP_GIT2_FE(git_packbuilder_written,ZIF_GIT_PACKBUILDER_WRITTEN,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
