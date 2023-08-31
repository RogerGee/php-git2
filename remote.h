/*
 * remote.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_REMOTE_H
#define PHPGIT2_REMOTE_H

#include "stubs/remote_arginfo.h"

namespace php_git2
{
    // Explicitly specialize git2_resource destructor for git_remote.
    template<> php_git_remote::~git2_resource()
    {
        git_remote_free(handle);
    }

    class php_git_remote_callbacks:
        public php_option_array
    {
    public:
        php_git_remote_callbacks()
        {
            git_remote_init_callbacks(&opts,GIT_REMOTE_CALLBACKS_VERSION);
        }

        const git_remote_callbacks* byval_git2()
        {
            if (!is_null()) {
                array_wrapper arr(value);

                GIT2_ARRAY_LOOKUP_CALLBACK_NULLABLE(
                    arr,
                    remote_transport_message_callback,
                    info.transportMessageCallback,
                    sideband_progress,
                    payload,
                    opts);

                GIT2_ARRAY_LOOKUP_CALLBACK_NULLABLE(
                    arr,
                    remote_cred_acquire_callback,
                    info.credAcquireCallback,
                    credentials,
                    payload,
                    opts);

                GIT2_ARRAY_LOOKUP_CALLBACK_NULLABLE(
                    arr,
                    remote_transport_certificate_check_callback,
                    info.transportCertificateCheckCallback,
                    certificate_check,
                    payload,
                    opts);

                GIT2_ARRAY_LOOKUP_CALLBACK_NULLABLE(
                    arr,
                    remote_transfer_progress_callback,
                    info.transferProgressCallback,
                    transfer_progress,
                    payload,
                    opts);

                GIT2_ARRAY_LOOKUP_CALLBACK_NULLABLE(
                    arr,
                    remote_update_tips_callback,
                    info.updateTipsCallback,
                    update_tips,
                    payload,
                    opts);

                GIT2_ARRAY_LOOKUP_CALLBACK_NULLABLE(
                    arr,
                    remote_packbuilder_progress_callback,
                    info.packbuilderProgressCallback,
                    pack_progress,
                    payload,
                    opts);

                GIT2_ARRAY_LOOKUP_CALLBACK_NULLABLE(
                    arr,
                    remote_push_transfer_progress_callback,
                    info.pushTransferProgressCallback,
                    push_transfer_progress,
                    payload,
                    opts);

                GIT2_ARRAY_LOOKUP_CALLBACK_NULLABLE(
                    arr,
                    remote_push_update_reference_callback,
                    info.pushUpdateReferenceCallback,
                    push_update_reference,
                    payload,
                    opts);

                GIT2_ARRAY_LOOKUP_CALLBACK_NULLABLE(
                    arr,
                    remote_push_negotiation_callback,
                    info.pushNegotiationCallback,
                    push_negotiation,
                    payload,
                    opts);

                // TODO Handle transport callback.

                opts.payload = &info;
            }

            return &opts;
        }

    private:
        git_remote_callbacks opts;
        git_remote_callbacks_info info;
    };

    class php_git_proxy_options:
        public php_option_array
    {
    public:
        php_git_proxy_options()
        {
            git_proxy_init_options(&opts,GIT_PROXY_OPTIONS_VERSION);
        }

        const git_proxy_options* byval_git2()
        {
            if (!is_null()) {
                array_wrapper arr(value);

                GIT2_ARRAY_LOOKUP_LONG(arr,version,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,type,opts);
                GIT2_ARRAY_LOOKUP_STRING(arr,url,opts);

                GIT2_ARRAY_LOOKUP_CALLBACK_NULLABLE(
                    arr,
                    proxy_cred_acquire_callback,
                    info.credAcquireCallback,
                    credentials,
                    payload,
                    opts);

                GIT2_ARRAY_LOOKUP_CALLBACK_NULLABLE(
                    arr,
                    proxy_transport_certificate_check_callback,
                    info.transportCertificateCheckCallback,
                    certificate_check,
                    payload,
                    opts);

                opts.payload = &info;
            }

            return &opts;
        }

    private:
        git_proxy_options opts;
        git_proxy_callbacks_info info;
    };

    class php_git_fetch_options:
        public php_option_array
    {
    public:
        php_git_fetch_options()
        {
            git_fetch_init_options(&opts,GIT_FETCH_OPTIONS_VERSION);
        }

        const git_fetch_options* byval_git2()
        {
            if (!is_null()) {
                array_wrapper arr(value);
                php_git_remote_callbacks callbacks;
                php_git_proxy_options proxy_opts;

                GIT2_ARRAY_LOOKUP_LONG(arr,version,opts);
                GIT2_ARRAY_LOOKUP_SUBOBJECT_DEREFERENCE(
                    arr,
                    callbacks,
                    callbacks,
                    opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,prune,opts);
                GIT2_ARRAY_LOOKUP_BOOL(arr,update_fetchhead,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,download_tags,opts);
                GIT2_ARRAY_LOOKUP_SUBOBJECT_DEREFERENCE(
                    arr,
                    proxy_opts,
                    proxy_opts,
                    opts);

                // NOTE: custom_headers is stored on the object so that it
                // cleans up the dynamic memory when the object is destroyed.
                GIT2_ARRAY_LOOKUP_SUBOBJECT_DEREFERENCE(
                    arr,
                    custom_headers,
                    custom_headers,
                    opts);
            }

            return &opts;
        }

    private:
        git_fetch_options opts;
        php_git_strarray_array custom_headers;
    };

    class php_git_push_options:
        public php_option_array
    {
    public:
        php_git_push_options()
        {
            git_push_init_options(&opts,GIT_PUSH_OPTIONS_VERSION);
        }

        const git_push_options* byval_git2()
        {
            if (!is_null()) {
                array_wrapper arr(value);
                php_git_remote_callbacks callbacks;
                php_git_proxy_options proxy_opts;

                GIT2_ARRAY_LOOKUP_LONG(arr,version,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,pb_parallelism,opts);
                GIT2_ARRAY_LOOKUP_SUBOBJECT_DEREFERENCE(
                    arr,
                    callbacks,
                    callbacks,
                    opts);
                GIT2_ARRAY_LOOKUP_SUBOBJECT_DEREFERENCE(
                    arr,
                    proxy_opts,
                    proxy_opts,
                    opts);

                // NOTE: custom_headers is stored on the object so that it
                // cleans up the dynamic memory when the object is destroyed.
                GIT2_ARRAY_LOOKUP_SUBOBJECT_DEREFERENCE(
                    arr,
                    custom_headers,
                    custom_headers,
                    opts);
            }

            return &opts;
        }

    private:
        git_push_options opts;
        php_git_strarray_array custom_headers;
    };

    class php_git_remote_head
    {
    public:
        using connect_t = php_long_ref<size_t>;
        using target_t = const git_remote_head***;
        using connector_type = connector_wrapper<php_git_remote_head>;

        php_git_remote_head(connect_t& obj):
            conn(obj)
        {
        }

        target_t byval_git2()
        {
            return &arr;
        }

        void ret(zval* return_value)
        {
            size_t n = conn.get_value();

            array_init(return_value);
            for (size_t i = 0;i < n;++i) {
                zval zv;
                convert_remote_head(&zv,arr[i]);

                add_next_index_zval(return_value,&zv);
            }
        }

    private:
        const git_remote_head** arr;
        connect_t& conn;
    };

    class php_refspec_rethandler
    {
    public:
        template<typename... Ts>
        bool ret(const git_refspec* refspec,zval* return_value,local_pack<Ts...>&)
        {
            if (refspec == nullptr) {
                RETVAL_FALSE;
                return true;
            }

            php_resource_ref<php_git_refspec> res;

            *res.byval_git2() = refspec;
            res.ret(return_value);

            return true;
        }
    };

    class php_transfer_progress_rethandler
    {
    public:
        template<typename... Ts>
        bool ret(const git_transfer_progress* stats,zval* return_value,local_pack<Ts...>&)
        {
            if (stats == nullptr) {
                RETVAL_NULL();
                return true;
            }

            convert_transfer_progress(return_value,stats);
            return true;
        }
    };

} // namespace php-git2

static constexpr auto ZIF_git_remote_add_fetch = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*,
        const char*>::func<git_remote_add_fetch>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_remote_add_push = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*,
        const char*>::func<git_remote_add_push>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_remote_autotag = zif_php_git2_function<
    php_git2::func_wrapper<
        git_remote_autotag_option_t,
        const git_remote*>::func<git_remote_autotag>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>
        >,
    0
    >;

static constexpr auto ZIF_git_remote_connect = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_remote*,
        git_direction,
        const git_remote_callbacks*,
        const git_proxy_options*,
        const git_strarray*>::func<git_remote_connect>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>,
        php_git2::php_long_cast<git_direction>,
        php_git2::php_git_remote_callbacks,
        php_git2::php_git_proxy_options,
        php_git2::php_git_strarray_array_nullable
        >
    >;

static constexpr auto ZIF_git_remote_connected = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_remote*>::func<git_remote_connected>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_git_remote_create = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_remote**,
        git_repository*,
        const char*,
        const char*>::func<git_remote_create>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_remote>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_string
        >,
    php_git2::sequence<0,1>, // Make the remote depend on the repository
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_remote_create_anonymous = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_remote**,
        git_repository*,
        const char*>::func<git_remote_create_anonymous>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_remote>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    php_git2::sequence<0,1>, // Make the remote depend on the repository
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_remote_create_with_fetchspec = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_remote**,
        git_repository*,
        const char*,
        const char*,
        const char*>::func<git_remote_create_with_fetchspec>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_remote>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_string,
        php_git2::php_string
        >,
    php_git2::sequence<0,1>, // Make the remote depend on the repository
    1,
    php_git2::sequence<1,2,3,4>,
    php_git2::sequence<0,1,2,3,4>
    >;

static constexpr auto ZIF_git_remote_default_branch = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        git_remote*>::func<git_remote_default_branch>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_resource<php_git2::php_git_remote>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_remote_delete = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*>::func<git_remote_delete>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_remote_disconnect = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_remote*>::func<git_remote_disconnect>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>
        >
    >;

static constexpr auto ZIF_git_remote_download = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_remote*,
        const git_strarray*,
        const git_fetch_options*>::func<git_remote_download>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>,
        php_git2::php_git_strarray_array_nullable,
        php_git2::php_git_fetch_options
        >
    >;

static constexpr auto ZIF_git_remote_dup = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_remote**,
        git_remote*>::func<git_remote_dup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_remote>,
        php_git2::php_resource<php_git2::php_git_remote>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_remote_fetch = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_remote*,
        const git_strarray*,
        const git_fetch_options*,
        const char*>::func<git_remote_fetch>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>,
        php_git2::php_git_strarray_array_nullable,
        php_git2::php_git_fetch_options,
        php_git2::php_string_nullable
        >
    >;

static constexpr auto ZIF_git_remote_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_remote>
        >
    >;

static constexpr auto ZIF_git_remote_get_fetch_refspecs = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_strarray*,
        const git_remote*>::func<git_remote_get_fetch_refspecs>,
    php_git2::local_pack<
        php_git2::php_git_strarray,
        php_git2::php_resource<php_git2::php_git_remote>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_remote_get_push_refspecs = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_strarray*,
        const git_remote*>::func<git_remote_get_push_refspecs>,
    php_git2::local_pack<
        php_git2::php_git_strarray,
        php_git2::php_resource<php_git2::php_git_remote>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_remote_get_refspec = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        const git_refspec*,
        const git_remote*,
        size_t>::func<git_remote_get_refspec>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>,
        php_git2::php_long_cast<size_t>
        >,
    php_git2::php_refspec_rethandler
    >;

static constexpr auto ZIF_git_remote_is_valid_name = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const char*>::func<git_remote_is_valid_name>,
    php_git2::local_pack<
        php_git2::php_string
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_git_remote_list = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_strarray*,
        git_repository*>::func<git_remote_list>,
    php_git2::local_pack<
        php_git2::php_git_strarray,
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_remote_lookup = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_remote**,
        git_repository*,
        const char*>::func<git_remote_lookup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_remote>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_remote_ls = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const git_remote_head***,
        size_t*,
        git_remote*>::func<git_remote_ls>,
    php_git2::local_pack<
        php_git2::php_git_remote_head::connector_type,
        php_git2::php_long_ref<size_t>,
        php_git2::php_resource<php_git2::php_git_remote>
        >,
    1,
    php_git2::sequence<2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_remote_name = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_remote*>::func<git_remote_name>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>
        >,
    0
    >;

static constexpr auto ZIF_git_remote_owner = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_repository*,
        const git_remote*>::func<git_remote_owner>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>,
        php_git2::php_resource_ref<php_git2::php_git_repository_nofree>
        >,
    php_git2::php_owner_rethandler<php_git2::php_git_remote>,
    php_git2::sequence<0>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_git_remote_prune = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_remote*,
        const git_remote_callbacks*>::func<git_remote_prune>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>,
        php_git2::php_git_remote_callbacks
        >
    >;

static constexpr auto ZIF_git_remote_prune_refs = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_remote*>::func<git_remote_prune_refs>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_git_remote_push = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_remote*,
        const git_strarray*,
        const git_push_options*>::func<git_remote_push>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>,
        php_git2::php_git_strarray_array_nullable,
        php_git2::php_git_push_options
        >
    >;

static constexpr auto ZIF_git_remote_pushurl = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_remote*>::func<git_remote_pushurl>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>
        >,
    0
    >;

static constexpr auto ZIF_git_remote_refspec_count = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        const git_remote*>::func<git_remote_refspec_count>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>
        >,
    0
    >;

static constexpr auto ZIF_git_remote_rename = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_strarray*,
        git_repository*,
        const char*,
        const char*>::func<git_remote_rename>,
    php_git2::local_pack<
        php_git2::php_git_strarray,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_remote_set_autotag = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*,
        git_remote_autotag_option_t>::func<git_remote_set_autotag>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_long_cast<git_remote_autotag_option_t>
        >
    >;

static constexpr auto ZIF_git_remote_set_pushurl = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*,
        const char*>::func<git_remote_set_pushurl>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_remote_set_url = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*,
        const char*>::func<git_remote_set_url>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_remote_stats = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        const git_transfer_progress*,
        git_remote*>::func<git_remote_stats>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>
        >,
    php_git2::php_transfer_progress_rethandler
    >;

static constexpr auto ZIF_git_remote_stop = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_remote*>::func<git_remote_stop>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>
        >
    >;

static constexpr auto ZIF_git_remote_update_tips = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_remote*,
        const git_remote_callbacks*,
        int,
        git_remote_autotag_option_t,
        const char*>::func<git_remote_update_tips>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>,
        php_git2::php_git_remote_callbacks,
        php_git2::php_bool,
        php_git2::php_long_cast<git_remote_autotag_option_t>,
        php_git2::php_string_nullable
        >
    >;

static constexpr auto ZIF_git_remote_upload = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_remote*,
        const git_strarray*,
        const git_push_options*>::func<git_remote_upload>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>,
        php_git2::php_git_strarray_array_nullable,
        php_git2::php_git_push_options
        >
    >;

static constexpr auto ZIF_git_remote_url = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_remote*>::func<git_remote_url>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_remote>
        >,
    0
    >;

#define GIT_REMOTE_FE                               \
    PHP_GIT2_FE(git_remote_add_fetch)               \
    PHP_GIT2_FE(git_remote_add_push)                \
    PHP_GIT2_FE(git_remote_autotag)                 \
    PHP_GIT2_FE(git_remote_connect)                 \
    PHP_GIT2_FE(git_remote_connected)               \
    PHP_GIT2_FE(git_remote_create)                  \
    PHP_GIT2_FE(git_remote_create_anonymous)        \
    PHP_GIT2_FE(git_remote_create_with_fetchspec)   \
    PHP_GIT2_FE(git_remote_default_branch)          \
    PHP_GIT2_FE(git_remote_delete)                  \
    PHP_GIT2_FE(git_remote_disconnect)              \
    PHP_GIT2_FE(git_remote_download)                \
    PHP_GIT2_FE(git_remote_dup)                     \
    PHP_GIT2_FE(git_remote_fetch)                   \
    PHP_GIT2_FE(git_remote_free)                    \
    PHP_GIT2_FE(git_remote_get_fetch_refspecs)      \
    PHP_GIT2_FE(git_remote_get_push_refspecs)       \
    PHP_GIT2_FE(git_remote_get_refspec)             \
    PHP_GIT2_FE(git_remote_is_valid_name)           \
    PHP_GIT2_FE(git_remote_list)                    \
    PHP_GIT2_FE(git_remote_lookup)                  \
    PHP_GIT2_FE(git_remote_ls)                      \
    PHP_GIT2_FE(git_remote_name)                    \
    PHP_GIT2_FE(git_remote_owner)                   \
    PHP_GIT2_FE(git_remote_prune)                   \
    PHP_GIT2_FE(git_remote_prune_refs)              \
    PHP_GIT2_FE(git_remote_push)                    \
    PHP_GIT2_FE(git_remote_pushurl)                 \
    PHP_GIT2_FE(git_remote_refspec_count)           \
    PHP_GIT2_FE(git_remote_rename)                  \
    PHP_GIT2_FE(git_remote_set_autotag)             \
    PHP_GIT2_FE(git_remote_set_pushurl)             \
    PHP_GIT2_FE(git_remote_set_url)                 \
    PHP_GIT2_FE(git_remote_stats)                   \
    PHP_GIT2_FE(git_remote_stop)                    \
    PHP_GIT2_FE(git_remote_update_tips)             \
    PHP_GIT2_FE(git_remote_upload)                  \
    PHP_GIT2_FE(git_remote_url)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
