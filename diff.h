/*
 * diff.h
 *
 * This file is a part of php-git2.
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

    // Define type to wrap git_diff_options.

    class php_git_diff_options:
        public php_value_base
    {
    public:
        ZTS_CONSTRUCTOR(php_git_diff_options)

        const git_diff_options* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            if (value != nullptr && Z_TYPE_P(value) == IS_ARRAY) {
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
                GIT2_ARRAY_LOOKUP_CALLBACK(
                    arr,
                    diff_notify_callback,
                    callbacks.notifyCallback,
                    notify_cb,
                    payload,
                    opts);
                GIT2_ARRAY_LOOKUP_CALLBACK(
                    arr,
                    diff_progress_callback,
                    callbacks.progressCallback,
                    progress_cb,
                    payload,
                    opts);

                // Force payload to be pair of callbacks and check callable zval
                // type.
                if (opts.notify_cb != nullptr) {
                    if (!callbacks.notifyCallback.is_callable()) {
                        error_custom("'notify_cb' must be a callable",argno);
                    }
                    opts.payload = reinterpret_cast<void*>(&callbacks);
                }
                if (opts.progress_cb != nullptr) {
                    if (!callbacks.progressCallback.is_callable()) {
                        error_custom("'progress_cb' must be a callable",argno);
                    }
                    opts.payload = reinterpret_cast<void*>(&callbacks);
                }

                return &opts;
            }

            return nullptr;
        }

    private:
        git_diff_options opts;
        git_diff_options_callback_info callbacks;
        php_strarray_array strarray;
    };

    // Enumerate the callback handlers needed for generating git_diff functions.

    using diff_file_callback_handler = php_callback_handler<diff_file_callback>;
    using diff_binary_callback_handler = php_callback_handler<diff_binary_callback>;
    using diff_hunk_callback_handler = php_callback_handler<diff_hunk_callback>;
    using diff_line_callback_handler = php_callback_handler<diff_line_callback>;

    // Define types for grabbing callback functions. All callbacks target the
    // same git_diff_callback_info object which is obtained by chaining
    // connectors through a local pack. The types are specialized for each
    // callback.

    template<typename ConnectType>
    class php_git_diff_callback_base
    {
    public:
        typedef ConnectType connect_t;
        typedef void* target_t;

        php_git_diff_callback_base(git_diff_callback_info& obj TSRMLS_DC):
           info(obj)
        {
        }

        // NOTE: This member function is not used, but required to complete the
        // connector type.
        void* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            return nullptr;
        }

        operator git_diff_callback_info&()
        {
            return this->info;
        }
    protected:
        git_diff_callback_info& info;
    };

    template<typename ConnectType,typename CallbackHandlerType>
    class php_git_diff_callback : public php_git_diff_callback_base<ConnectType> {};

    template<typename ConnectType>
    class php_git_diff_callback<ConnectType,diff_file_callback_handler>:
        public php_git_diff_callback_base<ConnectType>
    {
    public:
        using my_base = php_git_diff_callback_base<ConnectType>;
        using my_base::connect_t;
        using my_base::target_t;

        php_git_diff_callback(git_diff_callback_info& obj TSRMLS_DC):
            my_base(obj)
        {
        }

        zval** byref_php(unsigned pos)
        {
            return &this->info.fileCallback.func;
        }
    };

    template<typename ConnectType>
    class php_git_diff_callback<ConnectType,diff_binary_callback_handler>:
        public php_git_diff_callback_base<ConnectType>
    {
    public:
        using my_base = php_git_diff_callback_base<ConnectType>;
        using my_base::connect_t;
        using my_base::target_t;

        php_git_diff_callback(git_diff_callback_info& obj TSRMLS_DC):
            my_base(obj)
        {
        }

        zval** byref_php(unsigned pos)
        {
            return &this->info.binaryCallback.func;
        }
    };

    template<typename ConnectType>
    class php_git_diff_callback<ConnectType,diff_hunk_callback_handler>:
        public php_git_diff_callback_base<ConnectType>
    {
    public:
        using my_base = php_git_diff_callback_base<ConnectType>;
        using my_base::connect_t;
        using my_base::target_t;

        php_git_diff_callback(git_diff_callback_info& obj TSRMLS_DC):
            my_base(obj)
        {
        }

        zval** byref_php(unsigned pos)
        {
            return &this->info.hunkCallback.func;
        }
    };

    template<typename ConnectType>
    class php_git_diff_callback<ConnectType,diff_line_callback_handler>:
        public php_git_diff_callback_base<ConnectType>
    {
    public:
        using my_base = php_git_diff_callback_base<ConnectType>;
        using my_base::connect_t;
        using my_base::target_t;

        php_git_diff_callback(git_diff_callback_info& obj TSRMLS_DC):
            my_base(obj)
        {
        }

        zval** byref_php(unsigned pos)
        {
            return &this->info.lineCallback.func;
        }
    };

    class php_git_diff_callback_payload
    {
    public:
        ZTS_CONSTRUCTOR(php_git_diff_callback_payload)

        zval** byref_php(unsigned pos)
        {
            return &info.zpayload;
        }

        void* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            // Check types of all callbacks.
            if (!info.fileCallback.is_callable()) {
                php_value_base::error_custom("expected 'callable' for file callback",
                    std::numeric_limits<unsigned>::max());
            }
            if (!info.binaryCallback.is_callable()) {
                php_value_base::error_custom("expected 'callable' for binary callback",
                    std::numeric_limits<unsigned>::max());
            }
            if (!info.hunkCallback.is_callable()) {
                php_value_base::error_custom("expected 'callable' for hunk callback",
                    std::numeric_limits<unsigned>::max());
            }
            if (!info.lineCallback.is_callable()) {
                php_value_base::error_custom("expected 'callable' for line callback",
                    std::numeric_limits<unsigned>::max());
            }

            return reinterpret_cast<void*>(&info);
        }

        operator git_diff_callback_info&()
        {
            return info;
        }
    private:
        git_diff_callback_info info;
    };

    // Enumerate connector types for the git_diff callback specializations.

    using diff_line_callback_info = php_git_diff_callback<
        php_git_diff_callback_payload,
        diff_line_callback_handler
        >;
    using diff_line_callback_connector = connector_wrapper<diff_line_callback_info>;

    using diff_hunk_callback_info = php_git_diff_callback<
        diff_line_callback_info,
        diff_hunk_callback_handler
        >;
    using diff_hunk_callback_connector = connector_wrapper_ex<diff_hunk_callback_info>;

    using diff_binary_callback_info = php_git_diff_callback<
        diff_hunk_callback_info,
        diff_binary_callback_handler
        >;
    using diff_binary_callback_connector = connector_wrapper_ex<diff_binary_callback_info>;

    using diff_file_callback_info = php_git_diff_callback<
        diff_binary_callback_info,
        diff_file_callback_handler
        >;
    using diff_file_callback_connector = connector_wrapper_ex<diff_file_callback_info>;

    // Misc. helper types

    using diff_nullable_buffer_length_connector = php_git2::connector_wrapper<
        php_git2::php_string_length_connector<size_t,php_git2::php_nullable_string>
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
        git_diff_perfdata* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
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
        php_git2::php_resource_null<php_git2::php_git_blob>,
        php_git2::php_nullable_string,
        php_git2::diff_nullable_buffer_length_connector,
        php_git2::php_nullable_string,
        php_git2::php_nullable_string,
        php_git2::php_git_diff_options,

        // Callback handlers
        php_git2::diff_file_callback_handler,
        php_git2::diff_binary_callback_handler,
        php_git2::diff_hunk_callback_handler,
        php_git2::diff_line_callback_handler,

        // Callback info
        php_git2::diff_file_callback_connector,
        php_git2::diff_binary_callback_connector,
        php_git2::diff_hunk_callback_connector,
        php_git2::diff_line_callback_connector,

        // Base callback info
        php_git2::php_git_diff_callback_payload
        >,
    -1,
    php_git2::sequence<0,1,3,4,5,10,11,12,13,14>,
    php_git2::sequence<0,1,3,2,4,5,6,7,8,9,14>,
    php_git2::sequence<0,1,2,0,3,4,5,6,7,8,9>
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
        php_git2::php_resource_null<php_git2::php_git_blob>,
        php_git2::php_nullable_string,
        php_git2::php_resource_null<php_git2::php_git_blob>,
        php_git2::php_nullable_string,
        php_git2::php_git_diff_options,

        // Callback handlers
        php_git2::diff_file_callback_handler,
        php_git2::diff_binary_callback_handler,
        php_git2::diff_hunk_callback_handler,
        php_git2::diff_line_callback_handler,

        // Callback info
        php_git2::diff_file_callback_connector,
        php_git2::diff_binary_callback_connector,
        php_git2::diff_hunk_callback_connector,
        php_git2::diff_line_callback_connector,

        // Base callback info
        php_git2::php_git_diff_callback_payload
        >,
    -1,
    php_git2::sequence<0,1,2,3,4,9,10,11,12,13>,
    php_git2::sequence<0,1,2,3,4,5,6,7,8,13>,
    php_git2::sequence<0,1,2,3,4,5,6,7,8,9>
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
        php_git2::php_nullable_string,
        php_git2::php_nullable_string,
        php_git2::diff_nullable_buffer_length_connector,
        php_git2::php_nullable_string,
        php_git2::php_nullable_string,
        php_git2::php_git_diff_options,

        // Callback handlers
        php_git2::diff_file_callback_handler,
        php_git2::diff_binary_callback_handler,
        php_git2::diff_hunk_callback_handler,
        php_git2::diff_line_callback_handler,

        // Callback info
        php_git2::diff_file_callback_connector,
        php_git2::diff_binary_callback_connector,
        php_git2::diff_hunk_callback_connector,
        php_git2::diff_line_callback_connector,

        // Base callback info
        php_git2::php_git_diff_callback_payload
        >,
    -1,
    php_git2::sequence<1,2,4,5,6,11,12,13,14,15>,
    php_git2::sequence<1,0,2,4,3,5,6,7,8,9,10,15>,
    php_git2::sequence<0,0,1,0,2,3,4,5,6,7,8,9>
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
    php_git2::sequence<0,1,2,3,4,5,6>,
    php_git2::sequence<0,0,1,2,3,4,5>
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
    php_git2::sequence<0,2,1>,
    php_git2::sequence<0,0,0>
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
        php_git2::diff_line_callback_handler,
        php_git2::diff_line_callback_connector,
        php_git2::php_git_diff_callback_payload
        >,
    -1,
    php_git2::sequence<0,1,3,4>,
    php_git2::sequence<0,1,2,4>,
    php_git2::sequence<0,1,0,3>
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
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
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
    PHP_GIT2_FE(git_diff_is_sorted_icase,ZIF_GIT_DIFF_IS_SORTED_ICASE,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
