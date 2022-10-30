/*
 * diff.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_DIFF_H
#define PHPGIT2_DIFF_H

namespace php_git2
{
    // Specialize resource destructor for git_diff
    template<> inline php_git_diff::~git2_resource()
    {
        git_diff_free(handle);
    }

    template<> inline php_git_diff_stats::~git2_resource()
    {
        git_diff_stats_free(handle);
    }

    // Define type to wrap git_diff_options.

    class php_git_diff_options:
        public php_option_array
    {
    public:
        const git_diff_options* byval_git2()
        {
            if (!is_null()) {
                array_wrapper arr(value);
                git_diff_init_options(&opts,GIT_DIFF_OPTIONS_VERSION);

                GIT2_ARRAY_LOOKUP_LONG(arr,version,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,flags,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,ignore_submodules,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,context_lines,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,interhunk_lines,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,id_abbrev,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,max_size,opts);
                GIT2_ARRAY_LOOKUP_STRING_NULLABLE(arr,old_prefix,opts);
                GIT2_ARRAY_LOOKUP_STRING_NULLABLE(arr,new_prefix,opts);
                GIT2_ARRAY_LOOKUP_SUBOBJECT(arr,strarray,pathspec,opts);
                GIT2_ARRAY_LOOKUP_CALLBACK_NULLABLE(
                    arr,
                    diff_notify_callback,
                    callbacks.notifyCallback,
                    notify_cb,
                    payload,
                    opts);
                GIT2_ARRAY_LOOKUP_CALLBACK_NULLABLE(
                    arr,
                    diff_progress_callback,
                    callbacks.progressCallback,
                    progress_cb,
                    payload,
                    opts);

                // Force payload to be pair of callbacks.
                if (opts.notify_cb != nullptr) {
                    opts.payload = reinterpret_cast<void*>(&callbacks);
                }
                if (opts.progress_cb != nullptr) {
                    opts.payload = reinterpret_cast<void*>(&callbacks);
                }

                return &opts;
            }

            return nullptr;
        }

    private:
        git_diff_options opts;
        git_diff_options_callback_info callbacks;
        php_strarray_byval_array strarray;
    };

    // Define type to wrap git_diff_find_options.

    class php_git_diff_find_options:
        public php_option_array
    {
    public:
        const git_diff_find_options* byval_git2()
        {
            if (!is_null()) {
                array_wrapper arr(value);
                git_diff_find_init_options(&opts,GIT_DIFF_FIND_OPTIONS_VERSION);

                GIT2_ARRAY_LOOKUP_LONG(arr,version,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,flags,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,rename_threshold,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,rename_from_rewrite_threshold,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,copy_threshold,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,break_rewrite_threshold,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,rename_limit,opts);

                // TODO Handle 'metric' member. For now we'll just leave this
                // NULL in the options struct.

                return &opts;
            }

            return nullptr;
        }

    private:
        git_diff_find_options opts;
    };

    class php_git_diff_callback_payload:
        public php_value_generic
    {
    public:
        php_git_diff_callback_payload():
            info(get_value())
        {
        }

        void* byval_git2()
        {
            return reinterpret_cast<void*>(&info);
        }

        git_diff_callback_info& get_info()
        {
            return info;
        }

    private:
        git_diff_callback_info info;
    };

    // Define types for grabbing callback functions. All callbacks target the
    // same git_diff_callback_info object which is obtained via connectors. The
    // types are specialized for each callback. Each object is also used to
    // obtain the underlying ptr-to-function for each callback if a valid PHP
    // callback was obtained.

    template<typename CallbackType>
    class php_git_diff_callback_base:
        public php_callback_sync_nullable
    {
    public:
        typedef php_git_diff_callback_payload connect_t;
        typedef typename CallbackType::type target_t;

        php_git_diff_callback_base(php_git_diff_callback_payload& obj):
           info(obj.get_info())
        {
        }

        typename CallbackType::type byval_git2()
        {
            if (is_null()) {
                return nullptr;
            }

            return CallbackType::callback;
        }

    protected:
        git_diff_callback_info& info;
    };

    template<typename CallbackType>
    class php_git_diff_callback:
        public php_git_diff_callback_base<CallbackType>
    {
    };

    template<>
    class php_git_diff_callback<diff_file_callback>:
        public php_git_diff_callback_base<diff_file_callback>
    {
    public:
        using base_t = php_git_diff_callback_base<diff_file_callback>;
        using typename base_t::connect_t;
        using typename base_t::target_t;

        php_git_diff_callback(php_git_diff_callback_payload& obj):
           base_t(obj)
        {
            info.fileCallback = this;
        }
    };

    template<>
    class php_git_diff_callback<diff_binary_callback>:
        public php_git_diff_callback_base<diff_binary_callback>
    {
        using base_t = php_git_diff_callback_base<diff_binary_callback>;
    public:
        using typename base_t::connect_t;
        using typename base_t::target_t;

        php_git_diff_callback(php_git_diff_callback_payload& obj):
           base_t(obj)
        {
            info.binaryCallback = this;
        }
    };

    template<>
    class php_git_diff_callback<diff_hunk_callback>:
        public php_git_diff_callback_base<diff_hunk_callback>
    {
        using base_t = php_git_diff_callback_base<diff_hunk_callback>;
    public:
        using typename base_t::connect_t;
        using typename base_t::target_t;

        php_git_diff_callback(php_git_diff_callback_payload& obj):
           base_t(obj)
        {
            info.hunkCallback = this;
        }
    };

    template<>
    class php_git_diff_callback<diff_line_callback>:
        public php_git_diff_callback_base<diff_line_callback>
    {
    public:
        using base_t = php_git_diff_callback_base<diff_line_callback>;
        using typename base_t::connect_t;
        using typename base_t::target_t;

        php_git_diff_callback(php_git_diff_callback_payload& obj):
           base_t(obj)
        {
            info.lineCallback = this;
        }
    };

    // Enumerate connector types for the git_diff callback specializations. The
    // callbacks always have the same parameter order, so we can parameterize
    // the connector pack offsets here.

    using diff_line_callback_info = php_git_diff_callback<diff_line_callback>;
    using diff_line_callback_connector = connector_wrapper<diff_line_callback_info,1>;

    using diff_hunk_callback_info = php_git_diff_callback<diff_hunk_callback>;
    using diff_hunk_callback_connector = connector_wrapper<diff_hunk_callback_info,2>;

    using diff_binary_callback_info = php_git_diff_callback<diff_binary_callback>;
    using diff_binary_callback_connector = connector_wrapper<diff_binary_callback_info,3>;

    using diff_file_callback_info = php_git_diff_callback<diff_file_callback>;
    using diff_file_callback_connector = connector_wrapper<diff_file_callback_info,4>;

    // Misc. helper types

    using diff_nullable_buffer_length_connector = php_git2::connector_wrapper<
        php_git2::php_string_length_connector<size_t,php_git2::php_string_nullable>
        >;

    using diff_buffer_length_connector = php_git2::connector_wrapper<
        php_git2::php_string_length_connector<size_t,php_git2::php_string>
        >;

    template<typename... Ts>
    class php_git_diff_delta_rethandler
    {
    public:
        bool ret(const git_diff_delta* delta,zval* return_value,local_pack<Ts...>& pack)
        {
            if (delta == nullptr) {
                RETVAL_NULL();
            }
            else {
                convert_diff_delta(return_value,delta);
            }

            return true;
        }
    };

    class php_git_diff_perfdata
    {
    public:
        git_diff_perfdata* byval_git2()
        {
            return &perfdata;
        }

        void ret(zval* return_value) const
        {
            convert_diff_perfdata(return_value,&perfdata);
        }

    private:
        git_diff_perfdata perfdata;
    };

    // Define type to wrap git_diff_format_email_options.

    class php_git_diff_format_email_options:
        public php_array_base
    {
    public:
        php_git_diff_format_email_options():
            sig(nullptr)
        {
            git_diff_format_email_init_options(&opts,GIT_DIFF_FORMAT_EMAIL_OPTIONS_VERSION);
        }

        ~php_git_diff_format_email_options()
        {
            if (sig != nullptr) {
                git_signature_free(sig);
            }
        }

        const git_diff_format_email_options* byval_git2()
        {
            array_wrapper arr(value);

            GIT2_ARRAY_LOOKUP_LONG(arr,version,opts);
            GIT2_ARRAY_LOOKUP_LONG(arr,flags,opts);
            GIT2_ARRAY_LOOKUP_LONG(arr,patch_no,opts);
            GIT2_ARRAY_LOOKUP_LONG(arr,total_patches,opts);
            GIT2_ARRAY_LOOKUP_SUBOBJECT(arr,oid,id,opts);
            GIT2_ARRAY_LOOKUP_STRING(arr,summary,opts);
            GIT2_ARRAY_LOOKUP_STRING(arr,body,opts);

            if (arr.query("author",sizeof("author")-1)) {
                zval* zv = arr.get_value();
                git_signature* author;

                if (Z_TYPE_P(zv) == IS_ARRAY) {
                    sig = convert_signature(zv);
                    if (sig == nullptr) {
                        throw php_git2_exception(
                            "Cannot parse 'author' from "
                            "git_diff_format_email_options array");
                    }
                    author = sig;
                }
                else if (Z_TYPE_P(zv) == IS_RESOURCE) {
                    php_resource<php_git_signature> sigres;

                    sigres.parse_with_context(zv,"author");
                    author = sigres.get_object()->get_handle();
                }
                else {
                    throw php_git2_exception(
                        "Field 'author' is invalid in git_diff_format_email_options array");
                }

                opts.author = author;
            }

            return &opts;
        }

    private:
        git_diff_format_email_options opts;
        php_git_oid_fromstr oid;
        git_signature* sig;
    };

} // namespace php_git2

// Functions:

static constexpr auto ZIF_GIT_DIFF_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_diff>
        >
    >;

static constexpr auto ZIF_GIT_DIFF_BLOB_TO_BUFFER = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const git_blob*,
        const char*,
        const char*,
        size_t,
        const char*,
        const git_diff_options*,
        git_diff_file_cb,
        git_diff_binary_cb,
        git_diff_hunk_cb,
        git_diff_line_cb,
        void*>::func<git_diff_blob_to_buffer>,
    php_git2::local_pack<
        php_git2::php_resource_nullable<php_git2::php_git_blob>,
        php_git2::php_string_nullable,
        php_git2::diff_nullable_buffer_length_connector,
        php_git2::php_string_nullable,
        php_git2::php_string_nullable,
        php_git2::php_git_diff_options,

        // Callback info
        php_git2::diff_file_callback_connector,
        php_git2::diff_binary_callback_connector,
        php_git2::diff_hunk_callback_connector,
        php_git2::diff_line_callback_connector,

        // Base callback info
        php_git2::php_git_diff_callback_payload
        >,
    -1,
    php_git2::sequence<0,1,3,4,5,6,7,8,9,10>,
    php_git2::sequence<0,1,3,2,4,5,6,7,8,9,10>
    >;

static constexpr auto ZIF_GIT_DIFF_BLOBS = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const git_blob*,
        const char*,
        const git_blob*,
        const char*,
        const git_diff_options*,
        git_diff_file_cb,
        git_diff_binary_cb,
        git_diff_hunk_cb,
        git_diff_line_cb,
        void*>::func<git_diff_blobs>,
    php_git2::local_pack<
        php_git2::php_resource_nullable<php_git2::php_git_blob>,
        php_git2::php_string_nullable,
        php_git2::php_resource_nullable<php_git2::php_git_blob>,
        php_git2::php_string_nullable,
        php_git2::php_git_diff_options,

        // Callback info
        php_git2::diff_file_callback_connector,
        php_git2::diff_binary_callback_connector,
        php_git2::diff_hunk_callback_connector,
        php_git2::diff_line_callback_connector,

        // Base callback info
        php_git2::php_git_diff_callback_payload
        >
    >;

static constexpr auto ZIF_GIT_DIFF_BUFFERS = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const void*,
        size_t,
        const char*,
        const void*,
        size_t,
        const char*,
        const git_diff_options*,
        git_diff_file_cb,
        git_diff_binary_cb,
        git_diff_hunk_cb,
        git_diff_line_cb,
        void*>::func<git_diff_buffers>,
    php_git2::local_pack<
        php_git2::diff_nullable_buffer_length_connector,
        php_git2::php_string_nullable,
        php_git2::php_string_nullable,
        php_git2::diff_nullable_buffer_length_connector,
        php_git2::php_string_nullable,
        php_git2::php_string_nullable,
        php_git2::php_git_diff_options,

        // Callback info
        php_git2::diff_file_callback_connector,
        php_git2::diff_binary_callback_connector,
        php_git2::diff_hunk_callback_connector,
        php_git2::diff_line_callback_connector,

        // Base callback info
        php_git2::php_git_diff_callback_payload
        >,
    -1,
    php_git2::sequence<1,2,4,5,6,7,8,9,10,11>,
    php_git2::sequence<1,0,2,4,3,5,6,7,8,9,10,11>
    >;

static constexpr auto ZIF_GIT_DIFF_COMMIT_AS_EMAIL = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        git_repository*,
        git_commit*,
        size_t,
        size_t,
        git_diff_format_email_flags_t,
        const git_diff_options*>::func<git_diff_commit_as_email>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_commit>,
        php_git2::php_long_cast<size_t>,
        php_git2::php_long_cast<size_t>,
        php_git2::php_long_cast<git_diff_format_email_flags_t>,
        php_git2::php_git_diff_options
        >,
    1,
    php_git2::sequence<1,2,3,4,5,6>,
    php_git2::sequence<0,1,2,3,4,5,6>
    >;

static constexpr auto ZIF_GIT_DIFF_FROM_BUFFER = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_diff**,
        const char*,
        size_t>::func<git_diff_from_buffer>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_diff>,
        php_git2::diff_buffer_length_connector,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<2>,
    php_git2::sequence<0,2,1>
    >;

static constexpr auto ZIF_GIT_DIFF_PRINT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_diff*,
        git_diff_format_t,
        git_diff_line_cb,
        void*>::func<git_diff_print>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_diff>,
        php_git2::php_long_cast<git_diff_format_t>,
        php_git2::diff_line_callback_connector,
        php_git2::php_git_diff_callback_payload
        >
    >;

static constexpr auto ZIF_GIT_DIFF_GET_DELTA = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        const git_diff_delta*,
        const git_diff*,
        size_t>::func<git_diff_get_delta>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_diff>,
        php_git2::php_long_cast<size_t>
        >,
    php_git2::php_git_diff_delta_rethandler<
        php_git2::php_resource<php_git2::php_git_diff>,
        php_git2::php_long_cast<size_t>
        >,
    php_git2::sequence<0,1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_GIT_DIFF_GET_PERFDATA = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_diff_perfdata*,
        const git_diff*>::func<git_diff_get_perfdata>,
    php_git2::local_pack<
        php_git2::php_git_diff_perfdata,
        php_git2::php_resource<php_git2::php_git_diff>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_GIT_DIFF_IS_SORTED_ICASE = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_diff*>::func<git_diff_is_sorted_icase>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_diff>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_GIT_DIFF_FOREACH = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_diff*,
        git_diff_file_cb,
        git_diff_binary_cb,
        git_diff_hunk_cb,
        git_diff_line_cb,
        void*>::func<git_diff_foreach>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_diff>,

        // Callback info
        php_git2::diff_file_callback_connector,
        php_git2::diff_binary_callback_connector,
        php_git2::diff_hunk_callback_connector,
        php_git2::diff_line_callback_connector,

        // Base callback info
        php_git2::php_git_diff_callback_payload
        >
    >;

static constexpr auto ZIF_GIT_DIFF_MERGE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_diff*,
        const git_diff*>::func<git_diff_merge>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_diff>,
        php_git2::php_resource<php_git2::php_git_diff>
        >,
    -1
    >;

static constexpr auto ZIF_GIT_DIFF_TO_BUF = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        git_diff*,
        git_diff_format_t>::func<git_diff_to_buf>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_resource<php_git2::php_git_diff>,
        php_git2::php_long_cast<git_diff_format_t>
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_GIT_DIFF_TREE_TO_TREE = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_diff**,
        git_repository*,
        git_tree*,
        git_tree*,
        const git_diff_options*>::func<git_diff_tree_to_tree>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_diff>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource_nullable<php_git2::php_git_tree>,
        php_git2::php_resource_nullable<php_git2::php_git_tree>,
        php_git2::php_git_diff_options
        >,
    php_git2::sequence<0,1>, // Make the git_diff dependent on the git_repository
    1,
    php_git2::sequence<1,2,3,4>,
    php_git2::sequence<0,1,2,3,4>
    >;

static constexpr auto ZIF_GIT_DIFF_TREE_TO_WORKDIR = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_diff**,
        git_repository*,
        git_tree*,
        const git_diff_options*>::func<git_diff_tree_to_workdir>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_diff>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource_nullable<php_git2::php_git_tree>,
        php_git2::php_git_diff_options
        >,
    php_git2::sequence<0,1>, // Make the git_diff dependent on the git_repository
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_GIT_DIFF_TREE_TO_WORKDIR_WITH_INDEX = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_diff**,
        git_repository*,
        git_tree*,
        const git_diff_options*>::func<git_diff_tree_to_workdir_with_index>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_diff>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource_nullable<php_git2::php_git_tree>,
        php_git2::php_git_diff_options
        >,
    php_git2::sequence<0,1>, // Make the git_diff dependent on the git_repository
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_GIT_DIFF_STATUS_CHAR = zif_php_git2_function<
    php_git2::func_wrapper<
        char,
        git_delta_t>::func<git_diff_status_char>,
    php_git2::local_pack<
        php_git2::php_long_cast<git_delta_t>
        >,
    0
    >;

static constexpr auto ZIF_GIT_DIFF_NUM_DELTAS = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        const git_diff*>::func<git_diff_num_deltas>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_diff>
        >,
    0
    >;

static constexpr auto ZIF_GIT_DIFF_NUM_DELTAS_OF_TYPE = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        const git_diff*,
        git_delta_t>::func<git_diff_num_deltas_of_type>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_diff>,
        php_git2::php_long_cast<git_delta_t>
        >,
    0
    >;

static constexpr auto ZIF_GIT_DIFF_FIND_SIMILAR = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_diff*,
        const git_diff_find_options*>::func<git_diff_find_similar>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_diff>,
        php_git2::php_git_diff_find_options
        >
    >;

static constexpr auto ZIF_GIT_DIFF_GET_STATS = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_diff_stats**,
        git_diff*>::func<git_diff_get_stats>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_diff_stats>,
        php_git2::php_resource<php_git2::php_git_diff>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_GIT_DIFF_STATS_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_diff_stats>
        >
    >;

static constexpr auto ZIF_GIT_DIFF_STATS_DELETIONS = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        const git_diff_stats*>::func<git_diff_stats_deletions>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_diff_stats>
        >,
    0
    >;

static constexpr auto ZIF_GIT_DIFF_STATS_FILES_CHANGED = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        const git_diff_stats*>::func<git_diff_stats_files_changed>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_diff_stats>
        >,
    0
    >;

static constexpr auto ZIF_GIT_DIFF_STATS_INSERTIONS = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        const git_diff_stats*>::func<git_diff_stats_insertions>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_diff_stats>
        >,
    0
    >;

static constexpr auto ZIF_GIT_DIFF_STATS_TO_BUF = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        const git_diff_stats*,
        git_diff_stats_format_t,
        size_t>::func<git_diff_stats_to_buf>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_resource<php_git2::php_git_diff_stats>,
        php_git2::php_long_cast<git_diff_stats_format_t>,
        php_git2::php_long_cast<size_t>
        >,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_GIT_DIFF_FORMAT_EMAIL = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        git_diff*,
        const git_diff_format_email_options*>::func<git_diff_format_email>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_resource<php_git2::php_git_diff>,
        php_git2::php_git_diff_format_email_options
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_GIT_DIFF_INDEX_TO_INDEX = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_diff**,
        git_repository*,
        git_index*,
        git_index*,
        const git_diff_options*>::func<git_diff_index_to_index>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_diff>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource_nullable<php_git2::php_git_index>,
        php_git2::php_resource_nullable<php_git2::php_git_index>,
        php_git2::php_git_diff_options
        >,
    php_git2::sequence<0,1>, // Make the git_diff dependent on the git_repository
    1,
    php_git2::sequence<1,2,3,4>,
    php_git2::sequence<0,1,2,3,4>
    >;

static constexpr auto ZIF_GIT_DIFF_TREE_TO_INDEX = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_diff**,
        git_repository*,
        git_tree*,
        git_index*,
        const git_diff_options*>::func<git_diff_tree_to_index>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_diff>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource_nullable<php_git2::php_git_tree>,
        php_git2::php_resource_nullable<php_git2::php_git_index>,
        php_git2::php_git_diff_options
        >,
    php_git2::sequence<0,1>, // Make the git_diff dependent on the git_repository
    1,
    php_git2::sequence<1,2,3,4>,
    php_git2::sequence<0,1,2,3,4>
    >;

static constexpr auto ZIF_GIT_DIFF_INDEX_TO_WORKDIR = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_diff**,
        git_repository*,
        git_index*,
        const git_diff_options*>::func<git_diff_index_to_workdir>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_diff>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource_nullable<php_git2::php_git_index>,
        php_git2::php_git_diff_options
        >,
    php_git2::sequence<0,1>, // Make the git_diff dependent on the git_repository
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

// Function Entries:

#define GIT_DIFF_FE                                                     \
    PHP_GIT2_FE(git_diff_free,ZIF_GIT_DIFF_FREE,NULL)                   \
    PHP_GIT2_FE(git_diff_blob_to_buffer,ZIF_GIT_DIFF_BLOB_TO_BUFFER,NULL) \
    PHP_GIT2_FE(git_diff_blobs,ZIF_GIT_DIFF_BLOBS,NULL)                 \
    PHP_GIT2_FE(git_diff_buffers,ZIF_GIT_DIFF_BUFFERS,NULL)             \
    PHP_GIT2_FE(git_diff_commit_as_email,ZIF_GIT_DIFF_COMMIT_AS_EMAIL,NULL) \
    PHP_GIT2_FE(git_diff_from_buffer,ZIF_GIT_DIFF_FROM_BUFFER,NULL)     \
    PHP_GIT2_FE(git_diff_print,ZIF_GIT_DIFF_PRINT,NULL)                 \
    PHP_GIT2_FE(git_diff_get_delta,ZIF_GIT_DIFF_GET_DELTA,NULL)         \
    PHP_GIT2_FE(git_diff_get_perfdata,ZIF_GIT_DIFF_GET_PERFDATA,NULL)   \
    PHP_GIT2_FE(git_diff_is_sorted_icase,ZIF_GIT_DIFF_IS_SORTED_ICASE,NULL) \
    PHP_GIT2_FE(git_diff_foreach,ZIF_GIT_DIFF_FOREACH,NULL)             \
    PHP_GIT2_FE(git_diff_merge,ZIF_GIT_DIFF_MERGE,NULL)                 \
    PHP_GIT2_FE(git_diff_to_buf,ZIF_GIT_DIFF_TO_BUF,NULL)               \
    PHP_GIT2_FE(git_diff_tree_to_tree,ZIF_GIT_DIFF_TREE_TO_TREE,NULL)   \
    PHP_GIT2_FE(git_diff_tree_to_workdir,ZIF_GIT_DIFF_TREE_TO_WORKDIR,NULL) \
    PHP_GIT2_FE(git_diff_tree_to_workdir_with_index,ZIF_GIT_DIFF_TREE_TO_WORKDIR_WITH_INDEX,NULL) \
    PHP_GIT2_FE(git_diff_status_char,ZIF_GIT_DIFF_STATUS_CHAR,NULL)     \
    PHP_GIT2_FE(git_diff_num_deltas,ZIF_GIT_DIFF_NUM_DELTAS,NULL)       \
    PHP_GIT2_FE(git_diff_num_deltas_of_type,ZIF_GIT_DIFF_NUM_DELTAS_OF_TYPE,NULL) \
    PHP_GIT2_FE(git_diff_find_similar,ZIF_GIT_DIFF_FIND_SIMILAR,NULL)   \
    PHP_GIT2_FE(git_diff_get_stats,ZIF_GIT_DIFF_GET_STATS,NULL)         \
    PHP_GIT2_FE(git_diff_stats_free,ZIF_GIT_DIFF_STATS_FREE,NULL)       \
    PHP_GIT2_FE(git_diff_stats_deletions,ZIF_GIT_DIFF_STATS_DELETIONS,NULL) \
    PHP_GIT2_FE(git_diff_stats_files_changed,ZIF_GIT_DIFF_STATS_FILES_CHANGED,NULL) \
    PHP_GIT2_FE(git_diff_stats_insertions,ZIF_GIT_DIFF_STATS_INSERTIONS,NULL) \
    PHP_GIT2_FE(git_diff_stats_to_buf,ZIF_GIT_DIFF_STATS_TO_BUF,NULL)   \
    PHP_GIT2_FE(git_diff_format_email,ZIF_GIT_DIFF_FORMAT_EMAIL,NULL)   \
    PHP_GIT2_FE(git_diff_index_to_index,ZIF_GIT_DIFF_INDEX_TO_INDEX,NULL) \
    PHP_GIT2_FE(git_diff_tree_to_index,ZIF_GIT_DIFF_TREE_TO_INDEX,NULL) \
    PHP_GIT2_FE(git_diff_index_to_workdir,ZIF_GIT_DIFF_INDEX_TO_WORKDIR,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
