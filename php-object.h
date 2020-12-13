/*
 * php-object.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_PHPOBJECT_H
#define PHPGIT2_PHPOBJECT_H
#include "php-type.h"
#include "php-callback.h"
#include <new>
extern "C" {
#include <git2/sys/odb_backend.h>
#include <git2/sys/refdb_backend.h>
#include <git2/sys/config.h>
}

// Helper macros

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
        php_git2_odb_backend_obj,
        php_git2_odb_backend_internal_obj,
        php_git2_odb_writepack_obj,
        php_git2_odb_writepack_internal_obj,
        php_git2_odb_stream_obj,
        php_git2_odb_stream_internal_obj,
        php_git2_writestream_obj,
        php_git2_config_backend_obj,
        php_git2_refdb_backend_obj,
        php_git2_refdb_backend_internal_obj,
        php_git2_closure_obj,
        _php_git2_obj_top_
    };

    // Store class entry for global lookup by php_git2_object_t enumerator.

    extern zend_class_entry* class_entry[];

    // Define generic class for custom zend_object. This type must employ
    // standard layout.

    template<typename StorageType>
    struct php_zend_object
    {
        php_zend_object(zend_class_entry* ce)
        {
            void* ptr = ecalloc(1,sizeof(StorageType));
            storage = new(ptr) StorageType();

            zend_object_std_init(&std,ce);
            object_properties_init(&std,ce);
            std.handlers = &handlers;
        }

        ~php_zend_object()
        {
            zend_object_std_dtor(&std);
            storage->~StorageType();
            efree(storage);
        }

        StorageType* storage; // indirect to force standard layout
        zend_object std; // last

        static zend_object_handlers handlers;
        static void init(zend_class_entry* ce);

        static constexpr size_t offset()
        {
            return offsetof(php_zend_object,storage);
        }

        static inline php_zend_object* get_wrapper(zend_object* zo)
        {
            return reinterpret_cast<php_zend_object*>(zo - offset());
        }

        static inline StorageType* get_storage(zend_object* zo)
        {
            return reinterpret_cast<php_zend_object*>(zo - offset())->storage;
        }
    };

    // Define base class for object PHP values.

    template<typename StorageType>
    class php_object:
        public php_value_base
    {
        using wrapper_t = php_zend_object<StorageType>;
    public:
        StorageType* get_storage()
        {
            return wrapper_t::get_storage(Z_OBJ(value));
        }

    private:
        virtual void parse_impl(zval* zvp,int argno)
        {
            int result;
            zval* dummy;
            zend_class_entry* ce = php_git2::class_entry[StorageType::get_type()];

            result = zend_parse_parameter(ZEND_PARSE_PARAMS_THROW,
                argno,
                zvp,
                "O",
                &dummy,
                ce);
            if (result == FAILURE) {
                throw php_git2_propagated_exception();
            }

            ZVAL_COPY_VALUE(&value,zvp);
        }
    };

    // Define a type for wrapping method calls.

    template<typename BackendType,typename StorageType>
    class php_object_wrapper
    {
        using wrapper_t = php_zend_object<StorageType>;
    public:
        php_object_wrapper(typename BackendType::base_class* base):
            wrapperObject(static_cast<BackendType*>(base))
        {
        }

        zval* thisobj()
        {
            return &wrapperObject->thisobj;
        }

        BackendType* object()
        {
            return wrapperObject;
        }

        StorageType* backing()
        {
            return wrapper_t::get_storage(Z_OBJ(wrapperObject->thisobj));
        }

    private:
        BackendType* wrapperObject;
    };

    template<typename BackendType,typename StorageType>
    class php_method_wrapper:
        public php_object_wrapper<BackendType,StorageType>
    {
    public:
        using object_wrapper = php_object_wrapper<BackendType,StorageType>;

        php_method_wrapper(const char* methodName,typename BackendType::base_class* base):
            object_wrapper(base)
        {
            ZVAL_STRING(&zmethod,methodName);
            ZVAL_NULL(&zretval);
        }

        ~php_method_wrapper()
        {
            zval_dtor(&zmethod);
            zval_dtor(&zretval);
        }

        int call()
        {
            return php_git2_invoke_callback(
                object_wrapper::thisobj(),
                &zmethod,
                &zretval,
                0,
                nullptr);
        }

        template<unsigned Count>
        int call(zval_array<Count>& params)
        {
            return params.call(object_wrapper::thisobj(),&zmethod,&zretval);
        }

        zval* retval()
        {
            return &zretval;
        }

    private:
        zval zmethod;
        zval zretval;
    };

    // Define custom storage types for custom classes.

    struct php_odb_backend_object
    {
        enum backend_kind
        {
            unset,
            conventional,
            user,
            custom
        };

        php_odb_backend_object();
        ~php_odb_backend_object();

        git_odb_backend* backend;
        backend_kind kind;
        php_git_odb* owner;

        void create_custom_backend(zval* obj);
        void create_conventional_backend(php_git_odb* newOwner)
        {
            assign_owner(newOwner);
            kind = conventional;
        }
        void assign_owner(php_git_odb* newOwner);
        void unset_backend(zval* obj);

        constexpr static php_git2_object_t get_type()
        {
            return php_git2_odb_backend_obj;
        }

    private:
        // Provide a custom odb_backend derivation that remembers the PHP object
        // to which it's attached.
        struct git_odb_backend_php:
            git_odb_backend
        {
            typedef git_odb_backend base_class;

            git_odb_backend_php(zend_object* obj);
            ~git_odb_backend_php();

            zval thisobj;
        };

        using method_wrapper = php_method_wrapper<
            php_odb_backend_object::git_odb_backend_php,
            php_odb_backend_object
            >;

        // Function entries for custom odb_backend implementations provided from
        // PHP userspace.
        static int read(void** datap,size_t* sizep,git_otype* typep,
            git_odb_backend* backend,const git_oid* oid);
        static int read_prefix(git_oid* oidp,
            void** datap,
            size_t* sizep,
            git_otype* typep,
            git_odb_backend* backend,
            const git_oid* prefix,
            size_t len);
        static int read_header(size_t* sizep,
            git_otype* typep,
            git_odb_backend* backend,
            const git_oid* oid);
        static int write(git_odb_backend* backend,
            const git_oid* oid,
            const void* data,
            size_t size,
            git_otype type);
        static int writestream(git_odb_stream** streamp,
            git_odb_backend* backend,
            git_off_t off,
            git_otype type);
        static int readstream(git_odb_stream** streamp,
            git_odb_backend* backend,
            const git_oid* oid);
        static int exists(git_odb_backend* backend,const git_oid* oid);
        static int exists_prefix(git_oid* oidp,
            git_odb_backend* backend,
            const git_oid* prefix,
            size_t len);
        static int refresh(git_odb_backend* backend);
        static int foreach(git_odb_backend* backend,
            git_odb_foreach_cb cb,
            void* payload);
        static int writepack(git_odb_writepack** writepackp,
            git_odb_backend* backend,
            git_odb* odb,
            git_transfer_progress_cb progress_cb,
            void* progress_payload);
        static void free(git_odb_backend* backend);
    };

    struct php_odb_backend_internal_object : php_odb_backend_object
    {
        php_odb_backend_internal_object();

        constexpr static php_git2_object_t get_type()
        {
            return php_git2_odb_backend_internal_obj;
        }
    };

    struct php_odb_writepack_object
    {
        php_odb_writepack_object();
        ~php_odb_writepack_object();

        git_odb_writepack* writepack;
        php_git_odb* owner;

        void create_custom_writepack(zval* zobj,zval* zbackend);
        void assign_owner(php_git_odb* newOwner);
        void unset_writepack()
        {
            writepack = nullptr;
        }

        constexpr static php_git2_object_t get_type()
        {
            return php_git2_odb_writepack_obj;
        }

    private:
        // Provide a custom derivation to handle subclasses.
        struct git_odb_writepack_php:
            git_odb_writepack
        {
            typedef git_odb_writepack base_class;

            git_odb_writepack_php(zend_object* obj);
            ~git_odb_writepack_php();

            zval thisobj;
        };

        using method_wrapper = php_method_wrapper<
            php_odb_writepack_object::git_odb_writepack_php,
            php_odb_writepack_object
            >;

        static int append(git_odb_writepack* writepack,
            const void* data,
            size_t length,
            git_transfer_progress* prog);
        static int commit(git_odb_writepack* writepack,git_transfer_progress* prog);
        static void free(git_odb_writepack* writepack);
    };

    struct php_odb_writepack_internal_object : php_odb_writepack_object
    {
        php_odb_writepack_internal_object();
        ~php_odb_writepack_internal_object();

        git_transfer_progress prog;
        php_callback_sync* cb;

        constexpr static php_git2_object_t get_type()
        {
            return php_git2_odb_writepack_internal_obj;
        }
    };

    struct php_odb_stream_object
    {
        enum stream_kind
        {
            unset,
            conventional,
            user,
            custom
        };

        php_odb_stream_object();
        ~php_odb_stream_object();

        git_odb_stream* stream;
        stream_kind kind;
        php_git_odb* owner;
        zval* zbackend;

        void create_custom_stream(zval* zobj,zval* zbackendObject,unsigned int mode);
        void assign_owner(php_git_odb* owner);
        void unset_stream()
        {
            stream = nullptr;
            kind = unset;
        }

        constexpr static php_git2_object_t get_type()
        {
            return php_git2_odb_stream_obj;
        }

    private:
        // Provide a custom derivation to handle subclasses.
        struct git_odb_stream_php:
            git_odb_stream
        {
            typedef git_odb_stream base_class;

            git_odb_stream_php(zend_object* obj,unsigned int mode);
            ~git_odb_stream_php();

            zval thisobj;
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
        php_odb_stream_internal_object();

        constexpr static php_git2_object_t get_type()
        {
            return php_git2_odb_stream_internal_obj;
        }
    };

    struct php_writestream_object
    {
        php_writestream_object();
        ~php_writestream_object();

        git_writestream* ws;

        constexpr static php_git2_object_t get_type()
        {
            return php_git2_writestream_obj;
        }
    };

    struct php_config_backend_object
    {
        php_config_backend_object();

        git_config_backend* backend;
        php_git_config* owner;

        void create_custom_backend(zval* zobj,php_git_config* owner);

        constexpr static php_git2_object_t get_type()
        {
            return php_git2_config_backend_obj;
        }

    private:
        // Provide a custom config_backend derivation that remembers the PHP
        // object to which it's attached.
        struct git_config_backend_php:
            git_config_backend
        {
            typedef git_config_backend base_class;

            git_config_backend_php(zend_object* obj);
            ~git_config_backend_php();

            zval thisobj;
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
        static int set_multivar(git_config_backend* cfg,
            const char* name,
            const char* regexp,
            const char* value);
        static int del(git_config_backend* cfg,const char* name);
        static int del_multivar(git_config_backend* cfg,const char* name,const char* regexp);
        static int iterator(git_config_iterator** iter,git_config_backend* cfg);
        static int snapshot(git_config_backend** out,git_config_backend* cfg);
        static int lock(git_config_backend* cfg);
        static int unlock(git_config_backend* cfg,int success);
        static void free(git_config_backend* cfg);
    };

    struct php_refdb_backend_object
    {
        enum backend_kind
        {
            unset,
            conventional,
            user,
            custom
        };

        php_refdb_backend_object();
        ~php_refdb_backend_object();

        git_refdb_backend* backend;
        backend_kind kind;
        php_git_refdb* owner;

        void create_custom_backend(zval* zobj);
        void create_conventional_backend(php_git_refdb* newOwner)
        {
            assign_owner(newOwner);
            kind = conventional;
        }
        void assign_owner(php_git_refdb* owner);

        constexpr static php_git2_object_t get_type()
        {
            return php_git2_refdb_backend_obj;
        }

    private:
        struct git_refdb_backend_php:
            git_refdb_backend
        {
            typedef git_refdb_backend base_class;

            git_refdb_backend_php(zend_object* obj);
            ~git_refdb_backend_php();

            zval thisobj;
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
            const git_reference *ref,
            int force,
            const git_signature *who,
            const char *message,
            const git_oid *old,
            const char *old_target);
        static int rename(
            git_reference **out,
            git_refdb_backend *backend,
            const char *old_name,
            const char *new_name,
            int force,
            const git_signature *who,
            const char *message);
        static int del(
            git_refdb_backend *backend,
            const char *ref_name,
            const git_oid *old_id,
            const char *old_target);
        static int compress(git_refdb_backend *backend);
        static int has_log(git_refdb_backend *backend, const char *refname);
        static int ensure_log(git_refdb_backend *backend, const char *refname);
        static int reflog_read(git_reflog **out,
            git_refdb_backend *backend,
            const char *name);
        static int reflog_write(git_refdb_backend *backend, git_reflog *reflog);
        static int reflog_rename(git_refdb_backend *backend,
            const char *old_name,
            const char *new_name);
        static int reflog_delete(git_refdb_backend *backend, const char *name);
        static int lock(void **payload_out, git_refdb_backend *backend, const char *refname);
        static int unlock(git_refdb_backend *backend,
            void *payload,
            int success,
            int update_reflog,
            const git_reference *ref,
            const git_signature *sig,
            const char *message);
        static void free(git_refdb_backend *backend);
    };

    struct php_refdb_backend_internal_object : php_refdb_backend_object
    {
        php_refdb_backend_internal_object();
        ~php_refdb_backend_internal_object();

        git_reference_iterator* iter;

        constexpr static php_git2_object_t get_type()
        {
            return php_git2_refdb_backend_internal_obj;
        }
    };

    struct php_closure_object
    {
        typedef void (*closure_dstor)(void*);

        php_closure_object();
        ~php_closure_object();

        zend_function func;
        void* payload;
        bool hasPayload;
        closure_dstor payloadDestructor;

        constexpr static php_git2_object_t get_type()
        {
            return php_git2_closure_obj;
        }
    };

    // Provide a routine to call during MINIT for registering the custom
    // classes.

    void php_git2_register_classes();

    // Provide functions for creating objects easily from the extension. These
    // *should* be used when instantiating an object from the extension. In the
    // case of abstract backend classes, these functions create an object of a
    // derived, internal type.

    void php_git2_make_object(zval* zp,php_git2_object_t type);
    void php_git2_make_odb_backend(zval* zp,git_odb_backend* backend,php_git_odb* owner);
    void php_git2_make_odb_writepack(zval* zp,
        git_odb_writepack* writepack,
        php_callback_sync* cb,
        zval* zbackend,
        php_git_odb* owner);
    void php_git2_make_odb_stream(zval* zp,
        git_odb_stream* stream,
        php_git_odb* owner);
    void php_git2_make_writestream(zval* zp,
        git_writestream* ws);
    void php_git2_make_refdb_backend(zval* zp,
        git_refdb_backend* backend,
        php_git_refdb* owner);

    // Useful helpers

    bool is_method_overridden(zend_class_entry* ce,const char* method,int len);
    zend_function* not_allowed_get_constructor(zval* object);
    zend_function* disallow_base_get_constructor(zval* object);

    // Function entry lists for class methods.

    extern zend_function_entry odb_backend_methods[];
    extern zend_function_entry odb_backend_internal_methods[];
    extern zend_function_entry odb_writepack_methods[];
    extern zend_function_entry odb_writepack_internal_methods[];
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
