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
#include <git2/sys/config.h>
}

namespace php_git2
{
    // Define the custom object types provided by php-git2. The order of the
    // elements in this enumeration maps to the internal order of each type's
    // corresponding class storage structure and thus is highly important.

    enum php_git2_object_t
    {
        php_git2_odb_writepack_obj,
        php_git2_odb_backend_obj,
        php_git2_odb_stream_obj,
        php_git2_writestream_obj,
        php_git2_config_backend_obj,
        php_git2_closure_obj,
        _php_git2_obj_top_
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
        bool derivedCall;
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
            git_odb_backend_php(zval* zv);
            ~git_odb_backend_php();

            zval* thisobj;
        };

        // Function entries for custom odb_backend implementations provided from
        // PHP userspace.
        static int read(void** datap,size_t* sizep,git_otype* typep,
            git_odb_backend* backend,const git_oid* oid);
        static int read_prefix(git_oid* oidp,void** datap,size_t* sizep,
            git_otype* typep,git_odb_backend* backend,const git_oid* prefix,
            size_t nbits);
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
            const git_oid* prefix,size_t nbits);
        static int refresh(git_odb_backend* backend);
        static int foreach(git_odb_backend* backend,git_odb_foreach_cb cb,
            void* payload);
        static int writepack(git_odb_writepack** writepackp,git_odb_backend* backend,
            git_odb* odb,git_transfer_progress_cb progress_cb,void* progress_payload);
        static void free(git_odb_backend* backend);
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
        bool derivedCall;
        php_zts_member zts;

        void create_custom_stream(zval* zobj,unsigned int mode,zval* zodbBackend = nullptr);

        static zend_object_handlers handlers;
        static void init(zend_class_entry* ce);
    private:
        // Provide a custom derivation to handle subclasses.
        struct git_odb_stream_php:
            git_odb_stream
        {
            git_odb_stream_php(zval* zv,unsigned int mode);
            ~git_odb_stream_php();

            zval* thisobj;
        };

        static int read(git_odb_stream *stream,char *buffer,size_t len);
        static int write(git_odb_stream *stream,const char *buffer,size_t len);
        static int finalize_write(git_odb_stream *stream,const git_oid *oid);
        static void free(git_odb_stream *stream);
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
            git_config_backend_php(zval* zv);
            ~git_config_backend_php();

            zval* thisobj;
        };

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

    struct php_closure_object : zend_object
    {
        php_closure_object(zend_class_entry* ce TSRMLS_DC);
        ~php_closure_object();

        zend_function func;
        void* payload;
        bool hasPayload;
        php_zts_member zts;

        static zend_object_handlers handlers;
        static void init(zend_class_entry* ce);
    };

    // Provide a routine to call during MINIT for registering the custom
    // classes.

    void php_git2_register_classes(TSRMLS_D);

    // Provide functions for creating objects easily from the extension. These
    // *should* be used when instantiating an object from the extension.

    void php_git2_make_object(zval* zp,php_git2_object_t type TSRMLS_DC);
    void php_git2_make_odb_backend(zval* zp,git_odb_backend* backend,php_git_odb* owner TSRMLS_DC);
    void php_git2_make_odb_writepack(zval* zp,git_odb_writepack* writepack,
        php_callback_sync* cb,zval* zbackend,php_git_odb* owner TSRMLS_DC);
    void php_git2_make_odb_stream(zval* zp,git_odb_stream* stream,php_git_odb* owner TSRMLS_DC);
    void php_git2_make_writestream(zval* zp,git_writestream* ws TSRMLS_DC);

    // Useful helpers

    bool is_method_overridden(zend_class_entry* ce,const char* method,int len);
    zend_function* not_allowed_get_constructor(zval* object TSRMLS_DC);
    zend_function* disallow_base_get_constructor(zval* object TSRMLS_DC);

    template<typename T>
    class derived_context
    {
    public:
        derived_context(T* object):
            obj(object)
        {
            obj->derivedCall = true;
        }

        ~derived_context()
        {
            obj->derivedCall = false;
        }

        T* object()
        {
            return obj;
        }
    private:
        T* obj;
    };

    // Extern variables in this namespace.
    extern zend_class_entry* class_entry[];
    extern zend_function_entry odb_writepack_methods[];
    extern zend_function_entry odb_backend_methods[];
    extern zend_function_entry odb_stream_methods[];
    extern zend_function_entry writestream_methods[];
    extern zend_function_entry config_backend_methods[];
    extern zend_function_entry closure_methods[];
}

// Helper macros

#define LOOKUP_OBJECT(type,val)                         \
    ((type*)zend_object_store_get_object(val TSRMLS_CC))

#define is_subclass_of(a,b)                     \
    (a == b || instanceof_function(a,b))

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
