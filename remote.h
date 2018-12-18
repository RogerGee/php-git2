/*
 * remote.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_REMOTE_H
#define PHPGIT2_REMOTE_H

namespace php_git2
{
    // Explicitly specialize git2_resource destructor for git_remote.
    template<> php_git_remote::~git2_resource()
    {
        git_remote_free(handle);
    }

    class php_git_remote_callbacks:
        public php_value_base
    {
    public:
        php_git_remote_callbacks(TSRMLS_D)
        {
            git_remote_init_callbacks(&opts,GIT_REMOTE_CALLBACKS_VERSION);
        }

        const git_remote_callbacks* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            if (Z_TYPE_P(value) == IS_ARRAY) {
                array_wrapper arr(value);

                GIT2_ARRAY_LOOKUP_CALLBACK(
                    arr,
                    remote_transport_message_callback,
                    info.transportMessageCallback,
                    sideband_progress,
                    opts);

                GIT2_ARRAY_LOOKUP_CALLBACK(
                    arr,
                    remote_cred_acquire_callback,
                    info.credAcquireCallback,
                    credentials,
                    opts);

                GIT2_ARRAY_LOOKUP_CALLBACK(
                    arr,
                    remote_transport_certificate_check_callback,
                    info.transportCertificateCheckCallback,
                    certificate_check,
                    opts);

                GIT2_ARRAY_LOOKUP_CALLBACK(
                    arr,
                    remote_transfer_progress_callback,
                    info.transferProgressCallback,
                    transfer_progress,
                    opts);

                GIT2_ARRAY_LOOKUP_CALLBACK(
                    arr,
                    remote_update_tips_callback,
                    info.updateTipsCallback,
                    update_tips,
                    opts);

                GIT2_ARRAY_LOOKUP_CALLBACK(
                    arr,
                    remote_packbuilder_progress_callback,
                    info.packbuilderProgressCallback,
                    pack_progress,
                    opts);

                GIT2_ARRAY_LOOKUP_CALLBACK(
                    arr,
                    remote_push_transfer_progress_callback,
                    info.pushTransferProgressCallback,
                    push_transfer_progress,
                    opts);

                GIT2_ARRAY_LOOKUP_CALLBACK(
                    arr,
                    remote_push_update_reference_callback,
                    info.pushUpdateReferenceCallback,
                    push_update_reference,
                    opts);

                GIT2_ARRAY_LOOKUP_CALLBACK(
                    arr,
                    remote_push_negotiation_callback,
                    info.pushNegotiationCallback,
                    push_negotiation,
                    opts);

                // TODO Handle transport callback.

                opts.payload = &info;
            }

            return nullptr;
        }

    private:
        git_remote_callbacks opts;
        git_remote_callbacks_info info;
    };

    class php_git_proxy_options
    {
    };

    class php_git_fetch_options
    {
    };

    class php_git_push_options
    {
    };

    class php_git_remote_head
    {
    public:
        typedef size_t connect_t;
        typedef const git_remote_head*** target_t;

        php_git_remote_head(connect_t& obj TSRMLS_DC):
            conn(obj)
        {
        }

        target_t byval_git2(unsigned p)
        {
            return &arr;
        }

        void ret(zval* return_value)
        {
            array_init(return_value);
            for (size_t i = 0;i < conn;++i) {
                zval* zv;
                MAKE_STD_ZVAL(zv);
                convert_push_head(zv,arr[i]);

                add_next_index_zval(return_value,zv);
            }
        }

    private:
        const git_remote_head** arr;
        connect_t& conn;
    };

} // namespace php-git2

static constexpr auto ZIF_GIT_REMOTE_ADD_FETCH = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*,
        const char*>::func<git_remote_add_fetch>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_ADD_PUSH = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*,
        const char*>::func<git_remote_add_push>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_AUTOTAG = zif_php_git2_function<
    php_git2::func_wrapper<
        git_remote_autotag_option_t,
        const git_remote*>::func<git_remote_autotag>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>
        >,
    0
    >;

static constexpr auto ZIF_GIT_REMOTE_CONNECT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_remote*,
        git_direction,
        const git_remote_callbacks*,
        const git_proxy_options*,
        const git_strarray*>::func<git_remote_connect>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_CONNECTED = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_remote*>::func<git_remote_connected>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_GIT_REMOTE_CREATE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_remote**,
        git_repository*,
        const char*,
        const char*>::func<git_remote_create>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_CREATE_ANONYMOUS = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_remote**,
        git_repository*,
        const char*>::func<git_remote_create_anonymous>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_CREATE_WITH_FETCHSPEC = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_remote**,
        git_repository*,
        const char*,
        const char*,
        const char*>::func<git_remote_create_with_fetchspec>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_DEFAULT_BRANCH = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        git_remote*>::func<git_remote_default_branch>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_DELETE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*>::func<git_remote_delete>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_DISCONNECT = zif_php_git2_function_void<
    php_git2::func_wrapper<
        void,
        git_remote*>::func<git_remote_disconnect>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>
        >
    >;

static constexpr auto ZIF_GIT_REMOTE_DOWNLOAD = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_remote*,
        const git_strarray*,
        const git_fetch_options*>::func<git_remote_download>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_DUP = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_remote**,
        git_remote*>::func<git_remote_dup>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_FETCH = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_remote*,
        const git_strarray*,
        const git_fetch_options*,
        const char*>::func<git_remote_fetch>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_remote>
        >
    >;

static constexpr auto ZIF_GIT_REMOTE_GET_FETCH_REFSPECS = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_strarray*,
        const git_remote*>::func<git_remote_get_fetch_refspecs>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_GET_PUSH_REFSPECS = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_strarray*,
        const git_remote*>::func<git_remote_get_push_refspecs>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_GET_REFSPEC = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_refspec*,
        const git_remote*,
        size_t>::func<git_remote_get_refspec>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_IS_VALID_NAME = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const char*>::func<git_remote_is_valid_name>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_LIST = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_strarray*,
        git_repository*>::func<git_remote_list>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_LOOKUP = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_remote**,
        git_repository*,
        const char*>::func<git_remote_lookup>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_LS = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const git_remote_head***,
        size_t*,
        git_remote*>::func<git_remote_ls>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_NAME = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_remote*>::func<git_remote_name>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>
        >,
    0
    >;

static constexpr auto ZIF_GIT_REMOTE_OWNER = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_repository*,
        const git_remote*>::func<git_remote_owner>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>
        >,
    php_git2::php_owner_rethandler<php_git2::php_git_remote>
    >;

static constexpr auto ZIF_GIT_REMOTE_PRUNE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_remote*,
        const git_remote_callbacks*>::func<git_remote_prune>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_PRUNE_REFS = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const git_remote*>::func<git_remote_prune_refs>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_PUSH = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_remote*,
        const git_strarray*,
        const git_push_options*>::func<git_remote_push>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_PUSHURL = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_remote*>::func<git_remote_pushurl>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>
        >,
    0
    >;

static constexpr auto ZIF_GIT_REMOTE_REFSPEC_COUNT = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        const git_remote*>::func<git_remote_refspec_count>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>
        >,
    0
    >;

static constexpr auto ZIF_GIT_REMOTE_RENAME = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_strarray*,
        git_repository*,
        const char*,
        const char*>::func<git_remote_rename>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_SET_AUTOTAG = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*,
        git_remote_autotag_option_t>::func<git_remote_set_autotag>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_SET_PUSHURL = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*,
        const char*>::func<git_remote_set_pushurl>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_SET_URL = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*,
        const char*>::func<git_remote_set_url>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_STATS = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_remote_stats>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_STOP = zif_php_git2_function_void<
    php_git2::func_wrapper<
        int,
        git_remote*>::func<git_remote_stop>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>
        >
    >;

static constexpr auto ZIF_GIT_REMOTE_UPDATE_TIPS = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_remote*,
        const git_remote_callbacks*,
        int,
        git_remote_autotag_option_t,
        const char*>::func<git_remote_update_tips>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_UPLOAD = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_remote*,
        const git_strarray*,
        const git_push_options*>::func<git_remote_upload>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_REMOTE_URL = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_remote*>::func<git_remote_url>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>
        >,
    0
    >;

#define GIT_REMOTE_FE                                                   \
    PHP_GIT2_FE(git_remote_add_fetch,ZIF_GIT_REMOTE_ADD_FETCH,NULL)     \
    PHP_GIT2_FE(git_remote_add_push,ZIF_GIT_REMOTE_ADD_PUSH,NULL)       \
    PHP_GIT2_FE(git_remote_autotag,ZIF_GIT_REMOTE_AUTOTAG,NULL)         \
    PHP_GIT2_FE(git_remote_connect,ZIF_GIT_REMOTE_CONNECT,NULL)         \
    PHP_GIT2_FE(git_remote_connected,ZIF_GIT_REMOTE_CONNECTED,NULL)     \
    PHP_GIT2_FE(git_remote_create,ZIF_GIT_REMOTE_CREATE,NULL)           \
    PHP_GIT2_FE(git_remote_create_anonymous,ZIF_GIT_REMOTE_CREATE_ANONYMOUS,NULL) \
    PHP_GIT2_FE(git_remote_create_with_fetchspec,ZIF_GIT_REMOTE_CREATE_WITH_FETCHSPEC,NULL) \
    PHP_GIT2_FE(git_remote_default_branch,ZIF_GIT_REMOTE_DEFAULT_BRANCH,NULL) \
    PHP_GIT2_FE(git_remote_delete,ZIF_GIT_REMOTE_DELETE,NULL)           \
    PHP_GIT2_FE(git_remote_disconnect,ZIF_GIT_REMOTE_DISCONNECT,NULL)   \
    PHP_GIT2_FE(git_remote_download,ZIF_GIT_REMOTE_DOWNLOAD,NULL)       \
    PHP_GIT2_FE(git_remote_dup,ZIF_GIT_REMOTE_DUP,NULL)                 \
    PHP_GIT2_FE(git_remote_fetch,ZIF_GIT_REMOTE_FETCH,NULL)             \
    PHP_GIT2_FE(git_remote_free,ZIF_GIT_REMOTE_FREE,NULL)               \
    PHP_GIT2_FE(git_remote_get_fetch_refspecs,ZIF_GIT_REMOTE_GET_FETCH_REFSPECS,NULL) \
    PHP_GIT2_FE(git_remote_get_push_refspecs,ZIF_GIT_REMOTE_GET_PUSH_REFSPECS,NULL) \
    PHP_GIT2_FE(git_remote_get_refspec,ZIF_GIT_REMOTE_GET_REFSPEC,NULL) \
    PHP_GIT2_FE(git_remote_is_valid_name,ZIF_GIT_REMOTE_IS_VALID_NAME,NULL) \
    PHP_GIT2_FE(git_remote_list,ZIF_GIT_REMOTE_LIST,NULL)               \
    PHP_GIT2_FE(git_remote_lookup,ZIF_GIT_REMOTE_LOOKUP,NULL)           \
    PHP_GIT2_FE(git_remote_ls,ZIF_GIT_REMOTE_LS,NULL)                   \
    PHP_GIT2_FE(git_remote_name,ZIF_GIT_REMOTE_NAME,NULL)               \
    PHP_GIT2_FE(git_remote_owner,ZIF_GIT_REMOTE_OWNER,NULL)             \
    PHP_GIT2_FE(git_remote_prune,ZIF_GIT_REMOTE_PRUNE,NULL)             \
    PHP_GIT2_FE(git_remote_prune_refs,ZIF_GIT_REMOTE_PRUNE_REFS,NULL)   \
    PHP_GIT2_FE(git_remote_push,ZIF_GIT_REMOTE_PUSH,NULL)               \
    PHP_GIT2_FE(git_remote_pushurl,ZIF_GIT_REMOTE_PUSHURL,NULL)         \
    PHP_GIT2_FE(git_remote_refspec_count,ZIF_GIT_REMOTE_REFSPEC_COUNT,NULL) \
    PHP_GIT2_FE(git_remote_rename,ZIF_GIT_REMOTE_RENAME,NULL)           \
    PHP_GIT2_FE(git_remote_set_autotag,ZIF_GIT_REMOTE_SET_AUTOTAG,NULL) \
    PHP_GIT2_FE(git_remote_set_pushurl,ZIF_GIT_REMOTE_SET_PUSHURL,NULL) \
    PHP_GIT2_FE(git_remote_set_url,ZIF_GIT_REMOTE_SET_URL,NULL)         \
    PHP_GIT2_FE(git_remote_stats,ZIF_GIT_REMOTE_STATS,NULL)             \
    PHP_GIT2_FE(git_remote_stop,ZIF_GIT_REMOTE_STOP,NULL)               \
    PHP_GIT2_FE(git_remote_update_tips,ZIF_GIT_REMOTE_UPDATE_TIPS,NULL) \
    PHP_GIT2_FE(git_remote_upload,ZIF_GIT_REMOTE_UPLOAD,NULL)           \
    PHP_GIT2_FE(git_remote_url,ZIF_GIT_REMOTE_URL,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
