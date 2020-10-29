/*
 * revwalk.h
 *
 * Copyright (C) Roger P. Gee
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

        ZTS_CONSTRUCTOR(revwalk_next_rethandler)

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

static constexpr auto ZIF_GIT_REVWALK_NEW = zif_php_git2_function_setdeps<
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

static constexpr auto ZIF_GIT_REVWALK_PUSH = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_revwalk*,
        const git_oid*>::func<git_revwalk_push>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>,
        php_git2::php_git_oid_fromstr
        >
    >;

static constexpr auto ZIF_GIT_REVWALK_NEXT = zif_php_git2_function_rethandler<
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
        php_git2::php_resource<php_git2::php_git_revwalk>
        >
    >;

static constexpr auto ZIF_GIT_REVWALK_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_revwalk>
        >
    >;

static constexpr auto ZIF_GIT_REVWALK_ADD_HIDE_CB = zif_php_git2_function<
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

static constexpr auto ZIF_GIT_REVWALK_HIDE_GLOB = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_revwalk*,
        const char*>::func<git_revwalk_hide_glob>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_GIT_REVWALK_HIDE_HEAD = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_revwalk*>::func<git_revwalk_hide_head>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>
        >
    >;

static constexpr auto ZIF_GIT_REVWALK_HIDE_REF = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_revwalk*,
        const char*>::func<git_revwalk_hide_ref>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_GIT_REVWALK_PUSH_GLOB = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_revwalk*,
        const char*>::func<git_revwalk_push_glob>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_GIT_REVWALK_PUSH_HEAD = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_revwalk*>::func<git_revwalk_push_head>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>
        >
    >;

static constexpr auto ZIF_GIT_REVWALK_PUSH_RANGE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_revwalk*,
        const char*>::func<git_revwalk_push_range>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_GIT_REVWALK_PUSH_REF = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_revwalk*,
        const char*>::func<git_revwalk_push_ref>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_GIT_REVWALK_REPOSITORY = zif_php_git2_function_rethandler<
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

static constexpr auto ZIF_GIT_REVWALK_SIMPLIFY_FIRST_PARENT = zif_php_git2_function_void<
    php_git2::func_wrapper<
        void,
        git_revwalk*>::func<git_revwalk_simplify_first_parent>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_revwalk>
        >
    >;

// Function Entries:

#define GIT_REVWALK_FE                                              \
    PHP_GIT2_FE(git_revwalk_new,ZIF_GIT_REVWALK_NEW,NULL)           \
    PHP_GIT2_FE(git_revwalk_push,ZIF_GIT_REVWALK_PUSH,NULL)         \
    PHP_GIT2_FE(git_revwalk_next,ZIF_GIT_REVWALK_NEXT,NULL)         \
    PHP_GIT2_FE(git_revwalk_hide,ZIF_GIT_REVWALK_HIDE,NULL)         \
    PHP_GIT2_FE(git_revwalk_sorting,ZIF_GIT_REVWALK_SORTING,NULL)       \
    PHP_GIT2_FE(git_revwalk_reset,ZIF_GIT_REVWALK_RESET,NULL)           \
    PHP_GIT2_FE(git_revwalk_free,ZIF_GIT_REVWALK_FREE,NULL)             \
    PHP_GIT2_FE(git_revwalk_add_hide_cb,ZIF_GIT_REVWALK_ADD_HIDE_CB,NULL) \
    PHP_GIT2_FE(git_revwalk_hide_glob,ZIF_GIT_REVWALK_HIDE_GLOB,NULL)   \
    PHP_GIT2_FE(git_revwalk_hide_head,ZIF_GIT_REVWALK_HIDE_HEAD,NULL)   \
    PHP_GIT2_FE(git_revwalk_hide_ref,ZIF_GIT_REVWALK_HIDE_REF,NULL)     \
    PHP_GIT2_FE(git_revwalk_push_glob,ZIF_GIT_REVWALK_PUSH_GLOB,NULL)   \
    PHP_GIT2_FE(git_revwalk_push_head,ZIF_GIT_REVWALK_PUSH_HEAD,NULL)   \
    PHP_GIT2_FE(git_revwalk_push_range,ZIF_GIT_REVWALK_PUSH_RANGE,NULL) \
    PHP_GIT2_FE(git_revwalk_push_ref,ZIF_GIT_REVWALK_PUSH_REF,NULL)     \
    PHP_GIT2_FE(git_revwalk_repository,ZIF_GIT_REVWALK_REPOSITORY,NULL) \
    PHP_GIT2_FE(git_revwalk_simplify_first_parent,ZIF_GIT_REVWALK_SIMPLIFY_FIRST_PARENT,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
