/*
 * php-object.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_PHPOBJECT_H
#define PHPGIT2_PHPOBJECT_H
#include "php-callback.h"
extern "C" {
#include <git2/sys/odb_backend.h>
#include <git2/sys/refdb_backend.h>
#include <git2/sys/config.h>
}

// Helper macros

#define LOOKUP_OBJECT(type,val)                             \
    ((type*)zend_object_store_get_object(val TSRMLS_CC))

#define is_subclass_of(a,b)                         \
    (a == b || instanceof_function(a,b TSRMLS_CC))

#define PHP_EMPTY_METHOD(C,M)                   \
    PHP_METHOD(C,M) { }

namespace php_git2
{
    // Define the custom object types provided by php-git2. The order of the
    // elements in this enumeration maps to the internal order of each type's
    // corresponding class storage structure and thus is highly important.

    enum php_git2_object_t
    {
        php_git2_odb_writepack_obj,
        php_git2_odb_backend_obj,
        php_git2_odb_backend_internal_obj,
        php_git2_odb_stream_obj,
        php_git2_odb_stream_internal_obj,
        php_git2_writestream_obj,
        php_git2_config_backend_obj,
        php_git2_refdb_backend_obj,
        php_git2_refdb_backend_internal_obj,
        php_git2_closure_obj,
        _php_git2_obj_top_
    };

    // Define a type for wrapping method calls.

    template<typename ObjectType,typename BackingType>
    class php_object_wrapper:
        protected php_zts_base_fetched
    {
    public:
        php_object_wrapper(typename ObjectType::base_class* base):
            wrapperObject(static_cast<ObjectType*>(base)),
            backingObject(nullptr)
        {
        }

        zval* thisobj()
        {
            return wrapperObject->thisobj;
        }

        ObjectType* object()
        {
            return wrapperObject;
        }

        BackingType* backing()
        {
            // Lazy load the backing object.
            if (backingObject == nullptr) {
                backingObject = LOOKUP_OBJECT(BackingType,wrapperObject->thisobj);
            }

            return backingObject;
        }

    protected:
        zval** thisobj_pp()
        {
            return &wrapperObject->thisobj;
        }

    private:
        ObjectType* wrapperObject;
        BackingType* backingObject;
    };

    template<typename ObjectType,typename BackingType>
    class php_method_wrapper:
        public php_object_wrapper<ObjectType,BackingType>
    {
    public:
        using object_wrapper = php_object_wrapper<ObjectType,BackingType>;
        using object_wrapper::TSRMLS_C;

        php_method_wrapper(const char* methodName,typename ObjectType::base_class* base):
            object_wrapper(base)
        {
            INIT_ZVAL(zmethod);
            ZVAL_STRING(&zmethod,methodName,1);
            INIT_ZVAL(zretval);
        }

        ~php_method_wrapper()
        {
            zval_dtor(&zmethod);
            zval_dtor(&zretval);
        }

        int call(int nparams = 0,zval* params[] = nullptr)
        {
            php_bailer bailer ZTS_CTOR;
            php_bailout_context ctx(bailer TSRMLS_CC);
            int result = GIT_OK;
#ifdef ZTS
            TSRMLS_D = ZTS_MEMBER_C(object_wrapper::backing()->zts);
#endif

            if (BAILOUT_ENTER_REGION(ctx)) {
                int retval;

                retval = call_user_function(
                    NULL,
                    object_wrapper::thisobj_pp(),
                    &zmethod,
                    &zretval,
                    nparams,
                    params TSRMLS_CC);

                if (retval == FAILURE) {
                    php_git2_giterr_set(GITERR_INVALID,"Failed to invoke userspace method");
                    result = GIT_EPHPFATAL;
                }
                else {
                    php_exception_wrapper ex ZTS_CTOR;

                    // Handle case where PHP userspace threw an exception.
                    if (ex.has_exception()) {
                        ex.set_giterr();
                        result = GIT_EPHPEXPROP;
                    }
                }
            }
            else {
                // Set a libgit2 error for completeness.
                php_git2_giterr_set(GITERR_INVALID,"PHP reported a fatal error");

                // Allow the fatal error to propogate.
                result = GIT_EPHPFATALPROP;
                bailer.handled();
            }

            return result;
        }

        zval* retval()
        {
            return &zretval;
        }

    private:
        zval zmethod;
        zval zretval;
    };

    // Define zend_object derivations for objects that require an extended
    // backend structure. Each class must provide a static 'handlers' member and
    // a static 'init' function for handling class initialization.

    struct php_odb_backend_object : zend_object
    {
        php_odb_backend_object(zend_class_entry* ce TSRMLS_DC);
        ~php_odb_backend_object();

        git_odb_backend* backend;
        php_git_odb* owner;
        php_zts_member zts;

        void create_custom_backend(zval* zobj,php_git_odb* newOwner);

        static zend_object_handlers handlers;
        static void init(zend_class_entry* ce);
    private:
        // Provide a custom odb_backend derivation that remembers the PHP object
        // to which it's attached.
        struct git_odb_backend_php:
            git_odb_backend
        {
            typedef git_odb_backend base_class;

            git_odb_backend_php(zval* zv TSRMLS_DC);
            ~git_odb_backend_php();

            zval* thisobj;
        };

        using method_wrapper = php_method_wrapper<
            php_odb_backend_object::git_odb_backend_php,
            php_odb_backend_object
            >;

        // Function entries for custom odb_backend implementations provided from
        // PHP userspace.
        static int read(void** datap,size_t* sizep,git_otype* typep,
            git_odb_backend* backend,const git_oid* oid);
        static int read_prefix(git_oid* oidp,void** datap,size_t* sizep,
            git_otype* typep,git_odb_backend* backend,const git_oid* prefix,
            size_t len);
        static int read_header(size_t* sizep,git_otype* typep,
            git_odb_backend* backend,const git_oid* oid);
        static int write(git_odb_backend* backend,const git_oid* oid,
            const void* data,size_t size,git_otype type);
        static int writestream(git_odb_stream** streamp,git_odb_backend* backend,
            git_off_t off,git_otype type);
        static int readstream(git_odb_stream** streamp,git_odb_backend* backend,
            const git_oid* oid);
        static int exists(git_odb_backend* backend,const git_oid* oid);
        static int exists_prefix(git_oid* oidp,git_odb_backend* backend,
            const git_oid* prefix,size_t len);
        static int refresh(git_odb_backend* backend);
        static int foreach(git_odb_backend* backend,git_odb_foreach_cb cb,
            void* payload);
        static int writepack(git_odb_writepack** writepackp,git_odb_backend* backend,
            git_odb* odb,git_transfer_progress_cb progress_cb,void* progress_payload);
        static void free(git_odb_backend* backend);
    };

    struct php_odb_backend_internal_object : php_odb_backend_object
    {
        php_odb_backend_internal_object(zend_class_entry* ce TSRMLS_DC);

        static zend_object_handlers handlers;
        static void init(zend_class_entry* ce);
    };

    struct php_odb_writepack_object : zend_object
    {
        php_odb_writepack_object(zend_class_entry* ce TSRMLS_DC);
        ~php_odb_writepack_object();

        git_odb_writepack* writepack;
        git_transfer_progress prog;
        php_callback_sync* cb;
        php_git_odb* owner;
        php_zts_member zts;

        static zend_object_handlers handlers;
        static void init(zend_class_entry* ce);
    };

    struct php_odb_stream_object : zend_object
    {
        php_odb_stream_object(zend_class_entry* ce TSRMLS_DC);
        ~php_odb_stream_object();

        git_odb_stream* stream;
        php_git_odb* owner;
        zval* zbackend;
        php_zts_member zts;

        void create_custom_stream(zval* zobj,unsigned int mode,zval* zodbBackend = nullptr);

        static zend_object_handlers handlers;
        static void init(zend_class_entry* ce);
    private:
        // Provide a custom derivation to handle subclasses.
        struct git_odb_stream_php:
            git_odb_stream
        {
            typedef git_odb_stream base_class;

            git_odb_stream_php(zval* zv,unsigned int mode);
            ~git_odb_stream_php();

            zval* thisobj;
        };

        using method_wrapper = php_method_wrapper<
            php_odb_stream_object::git_odb_stream_php,
            php_odb_stream_object
            >;

        static int read(git_odb_stream *stream,char *buffer,size_t len);
        static int write(git_odb_stream *stream,const char *buffer,size_t len);
        static int finalize_write(git_odb_stream *stream,const git_oid *oid);
        static void free(git_odb_stream *stream);
    };

    struct php_odb_stream_internal_object : php_odb_stream_object
    {
        php_odb_stream_internal_object(zend_class_entry* ce TSRMLS_DC);

        static zend_object_handlers handlers;
        static void init(zend_class_entry* ce);
    };

    struct php_writestream_object : zend_object
    {
        php_writestream_object(zend_class_entry* ce TSRMLS_DC);
        ~php_writestream_object();

        git_writestream* ws;
        php_zts_member zts;

        static zend_object_handlers handlers;
        static void init(zend_class_entry* ce);
    };

    struct php_config_backend_object : zend_object
    {
        php_config_backend_object(zend_class_entry* ce TSRMLS_DC);
        ~php_config_backend_object();

        git_config_backend* backend;
        php_git_config* owner;
        php_zts_member zts;

        void create_custom_backend(zval* zobj,php_git_config* owner);

        static zend_object_handlers handlers;
        static void init(zend_class_entry* ce);
    private:
        // Provide a custom config_backend derivation that remembers the PHP
        // object to which it's attached.
        struct git_config_backend_php:
            git_config_backend
        {
            typedef git_config_backend base_class;

            git_config_backend_php(zval* zv);
            ~git_config_backend_php();

            zval* thisobj;
        };

        using method_wrapper = php_method_wrapper<
            php_config_backend_object::git_config_backend_php,
            php_config_backend_object
            >;

        // Function entries for custom config_backend implementations provided
        // from PHP userspace.
        static int open(git_config_backend* cfg,git_config_level_t level);
        static int get(git_config_backend* cfg,const char* key,git_config_entry** out);
        static int set(git_config_backend* cfg,const char* name,const char* value);
        static int set_multivar(git_config_backend* cfg,const char* name,
            const char* regexp,const char* value);
        static int del(git_config_backend* cfg,const char* name);
        static int del_multivar(git_config_backend* cfg,const char* name,const char* regexp);
        static int iterator(git_config_iterator** iter,git_config_backend* cfg);
        static int snapshot(git_config_backend** out,git_config_backend* cfg);
        static int lock(git_config_backend* cfg);
        static int unlock(git_config_backend* cfg,int success);
        static void free(git_config_backend* cfg);
    };

    struct php_refdb_backend_object : zend_object
    {
        php_refdb_backend_object(zend_class_entry* ce TSRMLS_DC);
        ~php_refdb_backend_object();

        git_refdb_backend* backend;
        php_git_refdb* owner;
        php_zts_member zts;

        void create_custom_backend(zval* zobj,php_git_refdb* owner = nullptr);

        static zend_object_handlers handlers;
        static void init(zend_class_entry* ce);
    private:
        struct git_refdb_backend_php:
            git_refdb_backend
        {
            typedef git_refdb_backend base_class;

            git_refdb_backend_php(zval* zv);
            ~git_refdb_backend_php();

            zval* thisobj;
        };

        using method_wrapper = php_method_wrapper<
            php_refdb_backend_object::git_refdb_backend_php,
            php_refdb_backend_object
            >;

        static int exists(
            int *exists,
            git_refdb_backend *backend,
            const char *ref_name);
        static int lookup(
            git_reference **out,
            git_refdb_backend *backend,
            const char *ref_name);
        static int iterator(
            git_reference_iterator **iter,
            struct git_refdb_backend *backend,
            const char *glob);
        static int write(git_refdb_backend *backend,
            const git_reference *ref, int force,
            const git_signature *who, const char *message,
            const git_oid *old, const char *old_target);
        static int rename(
            git_reference **out, git_refdb_backend *backend,
            const char *old_name, const char *new_name, int force,
            const git_signature *who, const char *message);
        static int del(
            git_refdb_backend *backend,
            const char *ref_name,
            const git_oid *old_id,
            const char *old_target);
        static int compress(git_refdb_backend *backend);
        static int has_log(git_refdb_backend *backend, const char *refname);
        static int ensure_log(git_refdb_backend *backend, const char *refname);
        static int reflog_read(git_reflog **out, git_refdb_backend *backend, const char *name);
        static int reflog_write(git_refdb_backend *backend, git_reflog *reflog);
        static int reflog_rename(git_refdb_backend *backend, const char *old_name, const char *new_name);
        static int reflog_delete(git_refdb_backend *backend, const char *name);
        static int lock(void **payload_out, git_refdb_backend *backend, const char *refname);
        static int unlock(git_refdb_backend *backend, void *payload, int success, int update_reflog,
            const git_reference *ref, const git_signature *sig, const char *message);
        static void free(git_refdb_backend *backend);
    };

    struct php_refdb_backend_internal_object : php_refdb_backend_object
    {
        php_refdb_backend_internal_object(zend_class_entry* ce TSRMLS_DC);
        ~php_refdb_backend_internal_object();

        git_reference_iterator* iter;

        static zend_object_handlers handlers;
        static void init(zend_class_entry* ce);
    };

    struct php_closure_object : zend_object
    {
        typedef void (*closure_dstor)(void*);

        php_closure_object(zend_class_entry* ce TSRMLS_DC);
        ~php_closure_object();

        zend_function func;
        void* payload;
        bool hasPayload;
        closure_dstor payloadDestructor;
        php_zts_member zts;

        static zend_object_handlers handlers;
        static void init(zend_class_entry* ce);
    };

    // Provide a routine to call during MINIT for registering the custom
    // classes.

    void php_git2_register_classes(TSRMLS_D);

    // Provide functions for creating objects easily from the extension. These
    // *should* be used when instantiating an object from the extension. In the
    // case of abstract backend classes, these functions create an object of a
    // derived, internal type.

    void php_git2_make_object(zval* zp,php_git2_object_t type TSRMLS_DC);
    void php_git2_make_odb_backend(zval* zp,git_odb_backend* backend,php_git_odb* owner TSRMLS_DC);
    void php_git2_make_odb_writepack(zval* zp,git_odb_writepack* writepack,
        php_callback_sync* cb,zval* zbackend,php_git_odb* owner TSRMLS_DC);
    void php_git2_make_odb_stream(zval* zp,git_odb_stream* stream,php_git_odb* owner TSRMLS_DC);
    void php_git2_make_writestream(zval* zp,git_writestream* ws TSRMLS_DC);
    void php_git2_make_refdb_backend(zval* zp,git_refdb_backend* backend,php_git_refdb* owner TSRMLS_DC);

    // Useful helpers

    bool is_method_overridden(zend_class_entry* ce,const char* method,int len);
    zend_function* not_allowed_get_constructor(zval* object TSRMLS_DC);
    zend_function* disallow_base_get_constructor(zval* object TSRMLS_DC);

    // Extern variables in this namespace.
    extern zend_class_entry* class_entry[];
    extern zend_function_entry odb_writepack_methods[];
    extern zend_function_entry odb_backend_methods[];
    extern zend_function_entry odb_backend_internal_methods[];
    extern zend_function_entry odb_stream_methods[];
    extern zend_function_entry odb_stream_internal_methods[];
    extern zend_function_entry writestream_methods[];
    extern zend_function_entry config_backend_methods[];
    extern zend_function_entry refdb_backend_methods[];
    extern zend_function_entry refdb_backend_internal_methods[];
    extern zend_function_entry closure_methods[];
}

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
