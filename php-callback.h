/*
 * php-callback.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_PHP_CALLBACK_H
#define PHPGIT2_PHP_CALLBACK_H
#include "php-type.h"

namespace php_git2
{
    int php_git2_invoke_callback(zval* func,zval* ret,int paramCount,zval* params[] TSRMLS_DC);

    // Provide a type that contains an array of zvals converted from primative
    // values.

    template<unsigned Count>
    class zval_array:
        private php_zts_base
    {
    public:
        zval_array(TSRMLS_D):
            php_zts_base(TSRMLS_C)
        {
            for (unsigned i = 0;i < Count;++i) {
                ZVAL_NULL(params + i);
            }
        }

        ~zval_array()
        {
            for (unsigned i = 0;i < Count;++i) {
                zval_ptr_dtor(params + i);
            }
        }

        // Create a member function for assigning to the zvals from primative
        // values. The assignment may be recursive to allow multiple
        // assignments. We create partial specializations for the kinds of
        // values we are interested in.

        template<unsigned I>
        void assign()
        {
            // The base case does nothing.
        }

        template<unsigned I,typename... Ts>
        void assign(long h,Ts&&... ts)
        {
            ZVAL_LONG(params + I,h);
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        template<unsigned I,typename... Ts>
        void assign(unsigned int h,Ts&&... ts)
        {
            ZVAL_LONG(params + I,static_cast<long>(h));
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        template<unsigned I,typename... Ts>
        void assign(size_t sz,Ts&&... ts)
        {
            ZVAL_LONG(params + I,sz);
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        template<unsigned I,typename... Ts>
        void assign(double h,Ts&&... ts)
        {
            ZVAL_DOUBLE(params + I,h);
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        template<unsigned I,typename... Ts>
        void assign(bool b,Ts&&... ts)
        {
            ZVAL_BOOL(params + I,b);
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        template<unsigned I,typename... Ts>
        void assign(const char* str,Ts&&... ts)
        {
            if (str == nullptr) {
                ZVAL_NULL(params + I);
            }
            else {
                ZVAL_STRING(params + I,str);
            }
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        template<unsigned I,typename... Ts>
        void assign(const void* a,size_t& b,Ts&&... ts)
        {
            ZVAL_STRINGL(params + I,(const char*)a,b);
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        template<unsigned I,typename... Ts>
        void assign(zval* h,Ts&&... ts)
        {
            if (h != nullptr) {
                zval_ptr_dtor(params + I);
                ZVAL_COPY(params + I,h);
            }
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        int call(zval* func,zval* ret)
        {
            return php_git2_invoke_callback(func,ret,Count,params TSRMLS_CC);
        }

        zval* operator [](unsigned index)
        {
            return params + index;
        }
        const zval* operator [](unsigned index) const
        {
            return params + index;
        }

    private:
        zval params[Count];
    };

    /**
     * Synchronous Callbacks
     *
     * A synchronous callback is one that is executed by libgit2 within a single
     * library call. As such, we can allocate the callback data on the
     * stack. The object holds two zvals: one to represent the PHP userspace
     * function (i.e. a callable) and another to represent any payload
     * data. Payload data may be omitted if desired.
     */

    class php_callback_base:
        public php_value_base
    {
    public:
        php_callback_base();
        ~php_callback_base();

        void* byval_git2()
        {
            return this;
        }

        void set_members(zval* zvpFunc,zval* zvpPayload)
        {
            parse_with_context(zvpFunc,"callback");
            parse_with_context(zvpPayload,"payload");
        }

    protected:
        // NOTE: Callable is stored in base 'value' member.
        zval data; // payload

    private:
        virtual void parse_impl(zval* zvp,int argno);
    };

    using php_callback_sync = php_callback_base;

    // Provide a variant that allows a NULL callback.

    class php_callback_sync_nullable:
        public php_callback_sync
    {
    private:
        virtual void parse_impl(zval* zvp,int argno);
    };

    /**
     * Asynchronous Callbacks
     *
     * An asynchronous callback is a wrapper that allows a callback to persist
     * so that it can be executed out-of-line. We implement this by allocating a
     * php_callback_sync instance dynamically. This means that the async
     * callback object (which is allocated on the stack) goes away while the
     * sync callback object persists. To avoid leaks during the lifetime of the
     * request, the async object is also a connector type so users can attach an
     * asynchronous callback object to a resource or object. In this way the
     * lifetime of the callback is the same as the lifetime of the
     * resource/object.
     */

    template<typename CallbackSyncType = php_callback_sync>
    class php_callback_async_base:
        public php_value_base
    {
    public:
        php_callback_base* get_callback() const
        {
            return cb;
        }

        void* byval_git2()
        {
            return cb->byval_git2();
        }

    protected:
        CallbackSyncType* cb;

    private:
        virtual void parse_impl(zval* zvp,int argno)
        {
            return cb->parse_impl(zvp,argno);
        }
    };

    template<typename GitResource,typename CallbackSyncType = php_callback_sync>
    class php_callback_async:
        public php_callback_async_base<CallbackSyncType>
    {
    public:
        // Connect to an arbitrary resource type. It must be a newly created
        // resource (i.e. resource ref).
        using connect_t = php_resource_ref<GitResource>;
        using target_t = void*;

        php_callback_async(connect_t& conn)
        {
            // Allocate php_callback_sync object.
            cb = new (emalloc(sizeof(CallbackSyncType))) CallbackSyncType;

            // Assign php_callback_sync object to resource object. It must have
            // a member called 'cb' to which we have access.
            conn.get_object()->cb = cb;
        }

    private:
        using php_callback_async_base<CallbackSyncType>::cb;
    };

    // This alternate form of php_callback_async works for a non-resource
    // type. This means it doesn't have to perform a lookup on an underlying
    // resource object. Instead it uses the connected object directly.

    template<typename ConnectType,typename CallbackSyncType = php_callback_sync>
    class php_callback_async_ex:
        public php_callback_async_base<CallbackSyncType>
    {
    public:
        using connect_t = ConnectType;
        using target_t = void*;

        php_callback_async_ex(connect_t& conn)
        {
            // Allocate CallbackSyncType object. Assign a new CallbackSyncType
            // object to the connected object. It must have a member called 'cb'
            // to which we have access.

            cb = new (emalloc(sizeof(CallbackSyncType))) CallbackSyncType;
            conn.cb = cb;
        }

    private:
        using php_callback_async_base<CallbackSyncType>::cb;
    };

    // This alternate form of php_callback_async writes the callback to the
    // resource object at the end of its lifetime. This allows assignment to an
    // existing resource.

    template<typename GitResource,typename CallbackSyncType = php_callback_sync>
    class php_callback_async_existing:
        public php_callback_async_base<CallbackSyncType>
    {
    public:
        // Connect to an arbitrary resource type.
        using connect_t = php_resource<GitResource>;
        typedef void* target_t;

        php_callback_async_existing(connect_t& conn):
            stor(conn)
        {
            // Allocate CallbackSyncType object.
            cb = new (emalloc(sizeof(CallbackSyncType))) CallbackSyncType;
        }

        ~php_callback_async_existing()
        {
            // Assign CallbackSyncType object to resource object. It must have
            // a member called 'cb' to which we have access. This is run here in
            // the destructor to allow the resource wrapper's zval to get
            // assigned properly so we can access the GitResource.

            GitResource* rsrc = stor.get_object();
            if (rsrc->cb == nullptr) {
                rsrc->cb = cb;
            }
        }

    private:
        connect_t& stor;
        using php_callback_async_base<CallbackSyncType>::cb;
    };

    // Define a type to represent a callback function handler. All this does is
    // forward the function address to a git2 call.

    template<typename CallbackFunc>
    class php_callback_handler
    {
    public:
        ZTS_CONSTRUCTOR(php_callback_handler)

        constexpr typename CallbackFunc::type byval_git2() const
        {
            // Return the static address of the wrapped callback function.
            return &CallbackFunc::callback;
        }
    };

    // Create connector types that allows callback handlers to be nullable. The
    // connector type connects to an object that accesses a php_callback_base
    // object (either directly or indirectly).

    template<typename CallbackFunc>
    class php_callback_handler_nullable_connector:
        public php_callback_handler<CallbackFunc>
    {
    public:
        using connect_t = php_callback_base;
        using target_t = typename CallbackFunc::type;

        php_callback_handler_nullable_connector(connect_t& obj TSRMLS_DC):
            php_callback_handler<CallbackFunc>(TSRMLS_C), conn(obj)
        {
        }

        typename CallbackFunc::type byval_git2()
        {
            // Return null if the callback zval is NULL.
            if (Z_TYPE_P(conn.get_value()) == IS_NULL) {
                return nullptr;
            }

            return php_callback_handler<CallbackFunc>::byval_git2();
        }

    private:
        connect_t& conn;
    };

    template<typename CallbackFunc,typename CallbackAsyncType>
    class php_callback_handler_nullable_connector_async:
        public php_callback_handler<CallbackFunc>
    {
    public:
        using connect_t = CallbackAsyncType;
        using target_t = typename CallbackFunc::type;

        php_callback_handler_nullable_connector_async(connect_t& obj TSRMLS_DC):
            php_callback_handler<CallbackFunc>(TSRMLS_C), conn(obj)
        {
        }

        typename CallbackFunc::type byval_git2()
        {
            php_callback_base* cb = conn.get_callback();

            // Return null if the callback zval is NULL.
            if (Z_TYPE_P(cb->get_value()) == IS_NULL) {
                return nullptr;
            }

            return php_callback_handler<CallbackFunc>::byval_git2();
        }

    private:
        connect_t& conn;
    };

    // Here we define all the callbacks used by the library. Some are specific
    // to a particular operation whereas others are more generic.

    struct packbuilder_foreach_callback
    {
        typedef int (*type)(void*,size_t,void*);
        static int callback(void* buf,size_t size,void* payload);
    };

    struct transfer_progress_callback
    {
        typedef int (*type)(const git_transfer_progress* stats,void* payload);
        static int callback(const git_transfer_progress* stats,void* payload);
    };

    struct odb_foreach_callback
    {
        typedef int (*type)(const git_oid* oid,void* payload);
        static int callback(const git_oid* oid,void* payload);
    };

    struct treewalk_callback
    {
        typedef int (*type)(const char* root,const git_tree_entry* entry,void* payload);
        static int callback(const char* root,const git_tree_entry* entry,void* payload);
    };

    struct commit_parent_callback
    {
        // Create a sync callback derivative for storing a git_oid to use as the
        // callback return value. This will be allocated on the stack for the
        // duration of calls to the callback.
        struct sync_callback:
            php_callback_sync
        {
            ZTS_CONSTRUCTOR_WITH_BASE(sync_callback,php_callback_sync)

            git_oid oidbuf;
        };

        typedef const git_oid* (*type)(size_t idx,void* payload);
        static const git_oid* callback(size_t idx,void* payload);
    };

    struct reference_foreach_callback
    {
        typedef git_reference_foreach_cb type;
        static int callback(git_reference* ref,void* payload);
    };

    struct reference_foreach_name_callback
    {
        typedef git_reference_foreach_name_cb type;
        static int callback(const char* name,void* payload);
    };

    struct packbuilder_progress_callback
    {
        typedef int (*type)(int stage,uint32_t current,uint32_t total,void* payload);
        static int callback(int stage,uint32_t current,uint32_t total,void* payload);
    };

    struct config_foreach_callback
    {
        typedef git_config_foreach_cb type;
        static int callback(const git_config_entry* entry,void* payload);
    };

    struct tag_foreach_callback
    {
        typedef git_tag_foreach_cb type;
        static int callback(const char* name,git_oid* oid,void* payload);
    };

    struct repository_create_callback
    {
        typedef git_repository_create_cb type;
        static int callback(git_repository** out,const char* path,int bare,void* payload);
    };

    struct git_diff_options_callback_info
    {
        git_diff_options_callback_info(TSRMLS_D):
            notifyCallback(TSRMLS_C), progressCallback(TSRMLS_C)
        {
        }

        php_callback_base notifyCallback;
        php_callback_base progressCallback;
    };

    struct diff_notify_callback
    {
        typedef git_diff_notify_cb type;
        static int callback(
            const git_diff* diff_so_far,
            const git_diff_delta* delta_to_add,
            const char* matched_pathspec,
            void* payload /* git_diff_options_callback_info */);
    };

    struct diff_progress_callback
    {
        typedef git_diff_progress_cb type;
        static int callback(
            const git_diff* diff_so_far,
            const char* old_path,
            const char* new_path,
            void* payload /* git_diff_options_callback_info */);
    };

    struct git_diff_callback_info
    {
        php_callback_base fileCallback;
        php_callback_base binaryCallback;
        php_callback_base hunkCallback;
        php_callback_base lineCallback;
        zval zpayload;
    };

    struct diff_file_callback
    {
        typedef git_diff_file_cb type;
        static int callback(const git_diff_delta* delta,float progress,void* payload);
    };

    struct diff_binary_callback
    {
        typedef git_diff_binary_cb type;
        static int callback(
            const git_diff_delta* delta,
            const git_diff_binary* binary,
            void* payload);
    };

    struct diff_hunk_callback
    {
        typedef git_diff_hunk_cb type;
        static int callback(
            const git_diff_delta* delta,
            const git_diff_hunk* hunk,
            void* payload);
    };

    struct diff_line_callback
    {
        typedef git_diff_line_cb type;
        static int callback(
            const git_diff_delta* delta,
            const git_diff_hunk* hunk,
            const git_diff_line* line,
            void* payload);
    };

    struct index_matched_path_callback
    {
        typedef git_index_matched_path_cb type;
        static int callback(
            const char* path,
            const char* matched_pathspec,
            void* payload);
    };

    struct revwalk_hide_callback
    {
        typedef git_revwalk_hide_cb type;
        static int callback(
            const git_oid* commit_id,
            void* payload);
    };

    struct trace_callback
    {
        typedef git_trace_callback type;
        static void callback(
            git_trace_level_t level,
            const char* msg);
    };

    struct attr_foreach_callback
    {
        typedef git_attr_foreach_cb type;
        static int callback(
            const char* name,
            const char* value,
            void* payload);
    };

    struct status_callback
    {
        typedef git_status_cb type;
        static int callback(
            const char* path,
            unsigned int status_flags,
            void* payload);
    };

    struct note_foreach_callback
    {
        typedef git_note_foreach_cb type;
        static int callback(
            const git_oid* blob_id,
            const git_oid* annotated_object_id,
            void* payload);
    };

    struct stash_callback
    {
        typedef git_stash_cb type;
        static int callback(
            size_t index,
            const char* message,
            const git_oid* stash_id,
            void* payload);
    };

    struct stash_apply_progress_callback
    {
        typedef git_stash_apply_progress_cb type;
        static int callback(
            git_stash_apply_progress_t progress,
            void* payload);
    };

    struct cred_acquire_callback
    {
        typedef git_cred_acquire_cb type;
        static int callback(
            git_cred** cred,
            const char* url,
            const char* username_from_url,
            unsigned int allowed_types,
            void* payload);
    };

    struct transport_certificate_check_callback
    {
        typedef git_transport_certificate_check_cb type;
        static int callback(
            git_cert* cert,
            int valid,
            const char* host,
            void* payload);
    };

    struct git_remote_callbacks_info
    {
        git_remote_callbacks_info(TSRMLS_D):
            transportMessageCallback(TSRMLS_C),
            completionCallback(TSRMLS_C),
            credAcquireCallback(TSRMLS_C),
            transportCertificateCheckCallback(TSRMLS_C),
            transferProgressCallback(TSRMLS_C),
            updateTipsCallback(TSRMLS_C),
            packbuilderProgressCallback(TSRMLS_C),
            pushTransferProgressCallback(TSRMLS_C),
            pushUpdateReferenceCallback(TSRMLS_C),
            pushNegotiationCallback(TSRMLS_C)
        {
        }

        // TODO Handle lifetime of callbacks. The php_callback_base currently
        // leaves the callback+payload zvals allocated.

        php_callback_base transportMessageCallback;
        php_callback_base completionCallback;
        php_callback_base credAcquireCallback;
        php_callback_base transportCertificateCheckCallback;
        php_callback_base transferProgressCallback;
        php_callback_base updateTipsCallback;
        php_callback_base packbuilderProgressCallback;
        php_callback_base pushTransferProgressCallback;
        php_callback_base pushUpdateReferenceCallback;
        php_callback_base pushNegotiationCallback;
        //php_callback_base transportCallback;
    };

    struct remote_transport_message_callback
    {
        typedef git_transport_message_cb type;
        static int callback(
            const char* str,
            int len,
            void* payload);
    };

    struct remote_completion_callback
    {
        typedef int (*type)(git_remote_completion_type,void*);
        static int callback(
            git_remote_completion_type type,
            void* payload);
    };

    struct remote_cred_acquire_callback
    {
        typedef git_cred_acquire_cb type;
        static int callback(
            git_cred** cred,
            const char* url,
            const char* username_from_url,
            unsigned int allowed_types,
            void* payload);
    };

    struct remote_transport_certificate_check_callback
    {
        typedef git_transport_certificate_check_cb type;
        static int callback(
            git_cert* cert,
            int valid,
            const char* host,
            void* payload);
    };

    struct remote_transfer_progress_callback
    {
        typedef git_transfer_progress_cb type;
        static int callback(
            const git_transfer_progress* stats,
            void* payload);
    };

    struct remote_update_tips_callback
    {
        typedef int (*type)(const char*,const git_oid*,const git_oid*,void*);
        static int callback(
            const char* refname,
            const git_oid* a,
            const git_oid* b,
            void* payload);
    };

    struct remote_packbuilder_progress_callback
    {
        typedef git_packbuilder_progress type;
        static int callback(
            int stage,
            uint32_t current,
            uint32_t total,
            void* payload);
    };

    struct remote_push_transfer_progress_callback
    {
        typedef git_push_transfer_progress type;
        static int callback(
            unsigned int current,
            unsigned int total,
            size_t bytes,
            void* payload);
    };

    struct remote_push_update_reference_callback
    {
        typedef int (*type)(const char*,const char*,void*);
        static int callback(
            const char* refname,
            const char* status,
            void* payload);
    };

    struct remote_push_negotiation_callback
    {
        typedef git_push_negotiation type;
        static int callback(
            const git_push_update** updates,
            size_t len,
            void *payload);
    };

    struct git_proxy_callbacks_info
    {
        git_proxy_callbacks_info(TSRMLS_D):
            credAcquireCallback(TSRMLS_C),
            transportCertificateCheckCallback(TSRMLS_C)
        {
        }

        php_callback_sync credAcquireCallback;
        php_callback_sync transportCertificateCheckCallback;
    };

    struct proxy_cred_acquire_callback
    {
        typedef git_cred_acquire_cb type;
        static int callback(
            git_cred** cred,
            const char* url,
            const char* username_from_url,
            unsigned int allowed_types,
            void* payload);
    };

    struct proxy_transport_certificate_check_callback
    {
        typedef git_transport_certificate_check_cb type;
        static int callback(
            git_cert* cert,
            int valid,
            const char* host,
            void* payload);
    };

    struct remote_create_callback
    {
        typedef git_remote_create_cb type;
        static int callback(
            git_remote** out,
            git_repository* repo,
            const char* name,
            const char* url,
            void* payload);
    };

    struct repository_fetchhead_foreach_callback
    {
        typedef git_repository_fetchhead_foreach_cb type;
        static int callback(
            const char* ref_name,
            const char* remote_url,
            const git_oid* oid,
            unsigned int is_merge,
            void* payload);
    };

    struct repository_mergehead_foreach_callback
    {
        typedef git_repository_mergehead_foreach_cb type;
        static int callback(
            const git_oid* oid,
            void* payload);
    };

    struct treebuilder_filter_callback
    {
        typedef git_treebuilder_filter_cb type;
        static int callback(
            const git_tree_entry* entry,
            void* payload);
    };

    struct submodule_foreach_callback
    {
        typedef git_submodule_cb type;
        static int callback(
            git_submodule* sm,
            const char* name,
            void* payload);
    };

} // namespace php_git2

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
