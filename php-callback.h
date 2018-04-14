/*
 * php-callback.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_PHP_CALLBACK_H
#define PHPGIT2_PHP_CALLBACK_H
#include "php-type.h"

namespace php_git2
{
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
                ALLOC_INIT_ZVAL(params[i]);
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
            ZVAL_LONG(params[I],h);
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        template<unsigned I,typename... Ts>
        void assign(size_t sz,Ts&&... ts)
        {
            ZVAL_LONG(params[I],sz);
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        template<unsigned I,typename... Ts>
        void assign(double h,Ts&&... ts)
        {
            ZVAL_DOUBLE(params[I],h);
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        template<unsigned I,typename... Ts>
        void assign(bool b,Ts&&... ts)
        {
            ZVAL_BOOL(params[I],b);
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        template<unsigned I,typename... Ts>
        void assign(const char* str,Ts&&... ts)
        {
            if (str == nullptr) {
                ZVAL_NULL(params[I]);
            }
            else {
                ZVAL_STRING(params[I],str,1);
            }
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        template<unsigned I,typename... Ts>
        void assign(const void* a,size_t& b,Ts&&... ts)
        {
            ZVAL_STRINGL(params[I],(const char*)a,b,1);
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        template<unsigned I,typename... Ts>
        void assign(zval* h,Ts&&... ts)
        {
            if (h != nullptr) {
                zval_dtor(params[I]);
                params[I] = h;

                // Up ref for zval since we will try to destruct it later.
                Z_ADDREF_P(h);
            }
            assign<I+1>(std::forward<Ts>(ts)...);
        }

        void call(zval* func,zval* ret)
        {
            int r;

            // We allow null callables, which do nothing.
            if (Z_TYPE_P(func) == IS_NULL) {
                return;
            }

            INIT_ZVAL(*ret);
            r = call_user_function(EG(function_table),NULL,func,ret,Count,params TSRMLS_CC);
            if (r == FAILURE) {
                if (EG(exception)) {
                    throw php_git2_propagated_exception();
                }

                php_error(E_ERROR,"failed to invoke userspace callback");
            }
        }

        zval* operator [](unsigned index)
        { return params[index]; }
        const zval* operator [](unsigned index) const
        { return params[index]; }
    private:
        zval* params[Count];
    };

    // Provide a base class for callbacks.

    struct php_callback_base
    {
        php_callback_base();

        zval* func;
        zval* data;

        bool is_callable() const;
    };

    /**
     * Synchronous Callbacks
     *
     * A synchronous callback is one that is executed by libgit2 within a single
     * library call. As such, we can allocate the callback data on the
     * stack. The object holds two zvals: one to represent the PHP userspace
     * function (i.e. a callable) and another to represent any payload data. The
     * object always expects the calls to byref_php() to have position arguments
     * such that the function zval is first. Payload data may be omitted if
     * desired. The object tracks the position arguments since the standard does
     * not define the order of parameter evaluation.
     */

    class php_callback_sync:
        public php_zts_base,
        public php_callback_base
    {
    public:
        php_callback_sync(TSRMLS_D):
            php_zts_base(TSRMLS_C), p(std::numeric_limits<unsigned>::max())
        {
        }

        ~php_callback_sync()
        {
            // Delete the references to the variables.
            if (func != nullptr) {
                zval_ptr_dtor(&func);
            }
            if (data != nullptr) {
                zval_ptr_dtor(&data);
            }
        }

        zval** byref_php(unsigned pos)
        {
            // If 'p' is max_value then we haven't had a call to this function
            // yet and we store the parameter position. Otherwise we compare the
            // existing parameter position with the new one. We want 'p' set to
            // the value zero if the parameters need to be swapped such that the
            // function zval is first.

            if (p == std::numeric_limits<unsigned>::max()) {
                p = pos;
                return &func;
            }

            p = (p < pos);
            return &data;
        }

        void* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            // Figure out which parameters are which and swap as needed. Only
            // consider a swap if both were set (assume 'func' is always set).
            if (!p && data != nullptr) {
                std::swap(func,data);
            }

            // Make sure the function zval is a callable. We always allow the
            // user to omit a callable if the zval is null.
            if (!is_callable()) {
                php_value_base::error("callable",argno);
            }

            // Up reference count. This is really only needed for when this
            // class is wrapped by php_callback_async so that the PHP values
            // will exist between function calls.
            Z_ADDREF_P(func);
            Z_ADDREF_P(data);
            return this;
        }
    private:
        unsigned p;
    };

    /**
     * Asynchronous Callbacks
     *
     * An asynchronous callback is one that is allocated dynamically on the
     * per-request memory heap. We implement this by allocating a
     * php_callback_sync instance dynamically. This means that the async
     * callback object (which is allocated on the stack) goes away while the
     * sync callback object persists. To avoid leaks during the lifetime of the
     * request, the async object is also a connector type so users can attach an
     * asynchronous callback object to a resource or object. In this way the
     * lifetime of the callback is the same as the lifetime of the
     * resource/object.
     */

    template<typename GitResource>
    class php_callback_async
    {
    public:
        // Connect to an arbitrary resource type. It must be a newly created
        // resource (i.e. resource ref).
        using connect_t = php_resource_ref<GitResource>;
        typedef void* target_t;

        php_callback_async(connect_t& conn TSRMLS_DC)
        {
            // Allocate php_callback_sync object.
            cb = new (emalloc(sizeof(php_callback_sync))) php_callback_sync(TSRMLS_C);

            // Assign php_callback_sync object to resource object. It must have
            // a member called 'cb' to which we have access.
            GitResource* rsrc = conn.get_object(
                std::numeric_limits<unsigned>::max());
            rsrc->cb = cb;
        }

        zval** byref_php(unsigned pos)
        {
            return cb->byref_php(pos);
        }

        void* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            return cb->byval_git2(argno);
        }
    private:
        php_callback_sync* cb;
    };

    // This alternate form of php_callback_async works for a non-resource
    // type. This means it doesn't have to perform a lookup on an underlying
    // resource object. Instead it uses the connected object directly.

    template<typename ConnectType>
    class php_callback_async_ex
    {
    public:
        using connect_t = ConnectType;
        typedef void* target_t;

        php_callback_async_ex(connect_t& conn TSRMLS_DC):
            stor(conn)
        {
            // Allocate php_callback_sync object. Assign a new php_callback_sync
            // object to the connected object. It must have a member called 'cb'
            // to which we have access.
            conn.cb = new (emalloc(sizeof(php_callback_sync))) php_callback_sync(TSRMLS_C);
        }

        zval** byref_php(unsigned pos)
        {
            return stor.cb->byref_php(pos);
        }

        void* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            return stor.cb->byval_git2(argno);
        }
    private:
        connect_t& stor;
    };


    // This alternate form of php_callback_async writes the callback to the
    // resource object at the end of its lifetime. This allows assignment to an
    // existing resource.

    template<typename GitResource>
    class php_callback_async_existing
    {
    public:
        // Connect to an arbitrary resource type.
        using connect_t = php_resource<GitResource>;
        typedef void* target_t;

        php_callback_async_existing(connect_t& conn TSRMLS_DC):
            stor(conn)
        {
            // Allocate php_callback_sync object.
            cb = new (emalloc(sizeof(php_callback_sync))) php_callback_sync(TSRMLS_C);
        }

        ~php_callback_async_existing()
        {
            // Assign php_callback_sync object to resource object. It must have
            // a member called 'cb' to which we have access. This is run here in
            // the destructor to allow the resource wrapper's zval to get
            // assigned properly so we can access the GitResource.

            GitResource* rsrc = stor.get_object(
                std::numeric_limits<unsigned>::max());
            if (rsrc->cb == nullptr) {
                rsrc->cb = cb;
            }
        }

        zval** byref_php(unsigned pos)
        {
            return cb->byref_php(pos);
        }

        void* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            return cb->byval_git2(argno);
        }
    private:
        connect_t& stor;
        php_callback_sync* cb;
    };

    // Define a type to represent a callback function handler. All this does is
    // forward the function address to a git2 call.

    template<typename CallbackFunc>
    class php_callback_handler
    {
    public:
        ZTS_CONSTRUCTOR(php_callback_handler)

        constexpr typename CallbackFunc::type byval_git2(unsigned argno = std::numeric_limits<unsigned>::max()) const
        {
            // Return the static address of the wrapped callback function.
            return &CallbackFunc::callback;
        }
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
        zval* zpayload;
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

} // namespace php_git2

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
