/*
 * config-git2.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_CONFIG_H
#define PHPGIT2_CONFIG_H

extern "C" {
#include <git2/sys/config.h>
}
#include "stubs/config_arginfo.h"

namespace php_git2
{
    // Explicitly specialize git2_resource destructor for git_config.
    template<> php_git_config::~git2_resource()
    {
        git_config_free(handle);
    }

    // Explicitly specialize git2_resource destructor for git_config_iterator.
    template<> php_git_config_iterator::~git2_resource()
    {
        git_config_iterator_free(handle);
    }

    // Provide a type for mapping a git_config_entry to a PHP array. One version
    // does not free while the other frees.

    class php_git_config_entry
    {
    public:
        php_git_config_entry():
            entry(nullptr)
        {
        }

        git_config_entry** byval_git2()
        {
            return &entry;
        }

        void ret(zval* return_value)
        {
            if (entry == nullptr) {
                RETURN_NULL();
            }

            array_init(return_value);
            add_assoc_string(return_value,"name",(char*)entry->name);
            add_assoc_string(return_value,"value",(char*)entry->value);
            add_assoc_long(return_value,"level",entry->level);
        }

    protected:
        git_config_entry* get_entry() const
        {
            return entry;
        }

    private:
        git_config_entry* entry;
    };

    class php_git_config_entry_freeing:
        public php_git_config_entry
    {
    public:
        ~php_git_config_entry_freeing()
        {
            git_config_entry* ent = get_entry();

            if (ent != nullptr) {
                git_config_entry_free(ent);
            }
        }
    };

    // Provide a type for converting a PHP array to a git_configmap.

    class php_git_configmap:
        public php_array_base
    {
    public:
        git_configmap byval_git2()
        {
            git_configmap result;
            array_wrapper arr(value);

            GIT2_ARRAY_INDEX_LONG(arr,0,type,result);
            GIT2_ARRAY_INDEX_STRING_NULLABLE(arr,1,str_match,result);
            GIT2_ARRAY_INDEX_LONG(arr,2,map_value,result);

            return result;
        }
    };

    using php_configmap_array = php_array<php_git_configmap,git_configmap>;

    using php_configmap_array_length_connector = connector_wrapper<
        php_array_length_connector<
            size_t, php_configmap_array
            >
        >;

    // Provide type for handling config backend parameters.

    class php_git_config_backend_owner_connector:
        public php_object<php_config_backend_object>
    {
    public:
        // Make this object a connector to a php_resource that looks up a
        // php_git_config resource wrapper.
        using connect_t = php_resource<php_git_config>;
        using target_t = git_config_backend*;

        php_git_config_backend_owner_connector(connect_t& conn):
            ownerWrapper(conn)
        {
        }

        git_config_backend* byval_git2()
        {
            php_config_backend_object* object = get_storage();

            // Create custom backing if one doesn't already exist. Make sure the
            // owner is attached to the object.
            if (object->backend == nullptr) {
                object->create_custom_backend(get_value(),ownerWrapper.get_object());
            }
            else if (object->owner != nullptr) {
                throw php_git2_exception("The config backend is already owned by a config");
            }
            else {
                object->owner = ownerWrapper.get_object();
            }

            return object->backend;
        }

    private:
        connect_t& ownerWrapper;
    };

    class php_git_config_backend_byval:
        public php_object<php_config_backend_object>
    {
    public:
        git_config_backend* byval_git2()
        {
            return get_storage()->backend;
        }
    };

} // namespace php_git2

// Functions:

static constexpr auto ZIF_git_config_new = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config**>::func<git_config_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_config>
        >,
    1,
    php_git2::sequence<>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_git_config_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_config>
        >
    >;

static constexpr auto ZIF_git_config_open_default = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config**>::func<git_config_open_default>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_config>
        >,
    1,
    php_git2::sequence<>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_git_config_open_global = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config**,
        git_config*>::func<git_config_open_global>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_config>,
        php_git2::php_resource<php_git2::php_git_config>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_config_open_level = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config**,
        const git_config*,
        git_config_level_t>::func<git_config_open_level>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_config>,
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_long_cast<git_config_level_t>
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_config_open_ondisk = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config**,
        const char*>::func<git_config_open_ondisk>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_config>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_config_parse_bool = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        int*,
        const char*>::func<git_config_parse_bool>,
    php_git2::local_pack<
        php_git2::php_bool_ref<int>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_config_parse_int32 = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        int32_t*,
        const char*>::func<git_config_parse_int32>,
    php_git2::local_pack<
        php_git2::php_long_ref<int32_t>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_config_parse_int64 = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        int64_t*,
        const char*>::func<git_config_parse_int64>,
    php_git2::local_pack<
        php_git2::php_long_ref<int64_t>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_config_parse_path = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        const char*>::func<git_config_parse_path>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_config_set_bool = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config*,
        const char*,
        int>::func<git_config_set_bool>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string,
        php_git2::php_bool
        >
    >;

static constexpr auto ZIF_git_config_set_int32 = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config*,
        const char*,
        int32_t>::func<git_config_set_int32>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string,
        php_git2::php_long_cast<int32_t>
        >
    >;

static constexpr auto ZIF_git_config_set_int64 = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config*,
        const char*,
        int64_t>::func<git_config_set_int64>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string,
        php_git2::php_long_cast<int64_t>
        >
    >;

static constexpr auto ZIF_git_config_set_multivar = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config*,
        const char*,
        const char*,
        const char*>::func<git_config_set_multivar>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string,
        php_git2::php_string,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_config_set_string = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config*,
        const char*,
        const char*>::func<git_config_set_string>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_config_get_bool = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        int*,
        const git_config*,
        const char*>::func<git_config_get_bool>,
    php_git2::local_pack<
        php_git2::php_bool_ref<int>,
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_config_get_entry = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config_entry**,
        const git_config*,
        const char*>::func<git_config_get_entry>,
    php_git2::local_pack<
        php_git2::php_git_config_entry_freeing,
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_config_get_int32 = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        int32_t*,
        const git_config*,
        const char*>::func<git_config_get_int32>,
    php_git2::local_pack<
        php_git2::php_long_ref<int32_t>,
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_config_get_int64 = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        int64_t*,
        const git_config*,
        const char*>::func<git_config_get_int64>,
    php_git2::local_pack<
        php_git2::php_long_ref<int64_t>,
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_config_get_path = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        const git_config*,
        const char*>::func<git_config_get_path>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_config_get_string = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const char**,
        const git_config*,
        const char*>::func<git_config_get_string>,
    php_git2::local_pack<
        php_git2::php_string_ref,
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_config_get_string_buf = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        const git_config*,
        const char*>::func<git_config_get_string_buf>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_config_delete_entry = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config*,
        const char*>::func<git_config_delete_entry>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_config_delete_multivar = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config*,
        const char*,
        const char*>::func<git_config_delete_multivar>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_config_find_global = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*>::func<git_config_find_global>,
    php_git2::local_pack<
        php_git2::php_git_buf
        >,
    1,
    php_git2::sequence<>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_git_config_find_programdata = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*>::func<git_config_find_programdata>,
    php_git2::local_pack<
        php_git2::php_git_buf
        >,
    1,
    php_git2::sequence<>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_git_config_find_system = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*>::func<git_config_find_system>,
    php_git2::local_pack<
        php_git2::php_git_buf
        >,
    1,
    php_git2::sequence<>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_git_config_find_xdg = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*>::func<git_config_find_xdg>,
    php_git2::local_pack<
        php_git2::php_git_buf
        >,
    1,
    php_git2::sequence<>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_git_config_snapshot = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config**,
        git_config*>::func<git_config_snapshot>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_config>,
        php_git2::php_resource<php_git2::php_git_config>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_config_add_file_ondisk = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config*,
        const char*,
        git_config_level_t,
        const git_repository*,
        int>::func<git_config_add_file_ondisk>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string,
        php_git2::php_long_cast<git_config_level_t>,
        php_git2::php_resource_nullable<php_git2::php_git_repository>,
        php_git2::php_bool
        >
    >;

static constexpr auto ZIF_git_config_foreach = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const git_config*,
        git_config_foreach_cb,
        void*>::func<git_config_foreach>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_callback_handler<php_git2::config_foreach_callback>,
        php_git2::php_callback_sync
        >,
    -1,
    php_git2::sequence<0,2,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_config_foreach_match = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const git_config*,
        const char*,
        git_config_foreach_cb,
        void*>::func<git_config_foreach_match>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string,
        php_git2::php_callback_handler<php_git2::config_foreach_callback>,
        php_git2::php_callback_sync
        >,
    -1,
    php_git2::sequence<0,1,3,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_config_get_multivar_foreach = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const git_config*,
        const char*,
        const char*,
        git_config_foreach_cb,
        void*>::func<git_config_get_multivar_foreach>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string,
        php_git2::php_string_nullable,
        php_git2::php_callback_handler<php_git2::config_foreach_callback>,
        php_git2::php_callback_sync
        >,
    -1,
    php_git2::sequence<0,1,2,4,4>,
    php_git2::sequence<0,1,2,3,4>
    >;

static constexpr auto ZIF_git_config_get_mapped = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        int*,
        const git_config*,
        const char*,
        const git_configmap*,
        size_t>::func<git_config_get_mapped>,
    php_git2::local_pack<
        php_git2::php_long_ref<int>,
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string,
        php_git2::php_configmap_array_length_connector,
        php_git2::php_configmap_array
        >,
    1,
    php_git2::sequence<1,2,4>,
    php_git2::sequence<0,1,2,4,3>
    >;

static constexpr auto ZIF_git_config_add_backend = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config*,
        git_config_backend*,
        git_config_level_t,
        const git_repository*,
        int>::func<git_config_add_backend>,
    php_git2::local_pack<
        php_git2::connector_wrapper<php_git2::php_git_config_backend_owner_connector>,
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_long_cast<git_config_level_t>,
        php_git2::php_resource_nullable<php_git2::php_git_repository>,
        php_git2::php_bool
        >,
    -1,
    php_git2::sequence<1,0,2,3,4>,
    php_git2::sequence<1,0,2,3,4>
    >;

static constexpr auto ZIF_git_config_backend_foreach_match = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config_backend*,
        const char*,
        git_config_foreach_cb,
        void*>::func<git_config_backend_foreach_match>,
    php_git2::local_pack<
        php_git2::php_git_config_backend_byval,
        php_git2::php_string_nullable,
        php_git2::php_callback_handler<php_git2::config_foreach_callback>,
        php_git2::php_callback_sync
        >,
    -1,
    php_git2::sequence<0,1,3,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_config_iterator_new = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_config_iterator**,
        const git_config*>::func<git_config_iterator_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_config_iterator>,
        php_git2::php_resource<php_git2::php_git_config>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_config_multivar_iterator_new = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_config_iterator**,
        const git_config*,
        const char*,
        const char*>::func<git_config_multivar_iterator_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_config_iterator>,
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string,
        php_git2::php_string_nullable
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_config_iterator_glob_new = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_config_iterator**,
        const git_config*,
        const char*>::func<git_config_iterator_glob_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_config_iterator>,
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_config_iterator_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_config_iterator>
        >
    >;

static constexpr auto ZIF_git_config_next = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_config_entry**,
        git_config_iterator*>::func<git_config_next>,
    php_git2::local_pack<
        php_git2::php_git_config_entry,
        php_git2::php_resource<php_git2::php_git_config_iterator>
        >,
    php_git2::php_iterover_rethandler<0>,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_config_lookup_map_value = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        int*,
        const git_configmap*,
        size_t,
        const char*>::func<git_config_lookup_map_value>,
    php_git2::local_pack<
        php_git2::php_long_ref<int>,
        php_git2::php_configmap_array_length_connector,
        php_git2::php_configmap_array,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<2,3>,
    php_git2::sequence<0,2,1,3>
    >;

// Function Entries:

#define GIT_CONFIG_FE                               \
    PHP_GIT2_FE(git_config_new)                     \
    PHP_GIT2_FE(git_config_free)                    \
    PHP_GIT2_FE(git_config_open_default)            \
    PHP_GIT2_FE(git_config_open_global)             \
    PHP_GIT2_FE(git_config_open_level)              \
    PHP_GIT2_FE(git_config_open_ondisk)             \
    PHP_GIT2_FE(git_config_parse_bool)              \
    PHP_GIT2_FE(git_config_parse_int32)             \
    PHP_GIT2_FE(git_config_parse_int64)             \
    PHP_GIT2_FE(git_config_parse_path)              \
    PHP_GIT2_FE(git_config_set_bool)                \
    PHP_GIT2_FE(git_config_set_int32)               \
    PHP_GIT2_FE(git_config_set_int64)               \
    PHP_GIT2_FE(git_config_set_multivar)            \
    PHP_GIT2_FE(git_config_set_string)              \
    PHP_GIT2_FE(git_config_get_bool)                \
    PHP_GIT2_FE(git_config_get_entry)               \
    PHP_GIT2_FE(git_config_get_int32)               \
    PHP_GIT2_FE(git_config_get_int64)               \
    PHP_GIT2_FE(git_config_get_path)                \
    PHP_GIT2_FE(git_config_get_string)              \
    PHP_GIT2_FE(git_config_get_string_buf)          \
    PHP_GIT2_FE(git_config_delete_entry)            \
    PHP_GIT2_FE(git_config_delete_multivar)         \
    PHP_GIT2_FE(git_config_find_global)             \
    PHP_GIT2_FE(git_config_find_programdata)        \
    PHP_GIT2_FE(git_config_find_system)             \
    PHP_GIT2_FE(git_config_find_xdg)                \
    PHP_GIT2_FE(git_config_snapshot)                \
    PHP_GIT2_FE(git_config_add_file_ondisk)         \
    PHP_GIT2_FE(git_config_foreach)                 \
    PHP_GIT2_FE(git_config_foreach_match)           \
    PHP_GIT2_FE(git_config_get_multivar_foreach)    \
    PHP_GIT2_FE(git_config_get_mapped)              \
    PHP_GIT2_FE(git_config_add_backend)             \
    PHP_GIT2_FE(git_config_backend_foreach_match)   \
    PHP_GIT2_FE(git_config_iterator_new)            \
    PHP_GIT2_FE(git_config_iterator_free)           \
    PHP_GIT2_FE(git_config_multivar_iterator_new)   \
    PHP_GIT2_FE(git_config_iterator_glob_new)       \
    PHP_GIT2_FE(git_config_next)                    \
    PHP_GIT2_FE(git_config_lookup_map_value)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
