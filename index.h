/*
 * index.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_INDEX_H
#define PHPGIT2_INDEX_H

#include "stubs/index_arginfo.h"

namespace php_git2
{
    // Specialize resource destructor for git_index.
    template<> inline php_git_index::~git2_resource()
    {
        git_index_free(handle);
    }

    // Specialize resource destructor for git_index_conflict_iterator.
    template<> inline php_git_index_conflict_iterator::~git2_resource()
    {
        git_index_conflict_iterator_free(handle);
    }

    // Provide a type for converting user-supplied PHP array into
    // git_index_entry.

    class php_git_index_entry:
        public php_array_base
    {
    public:
        php_git_index_entry()
        {
            memset(&ent,0,sizeof(git_index_entry));
        }

        const git_index_entry* byval_git2()
        {
            array_wrapper arr(value);
            php_git_index_time tm;

            GIT2_ARRAY_LOOKUP_SUBOBJECT_DEREFERENCE(arr,tm,ctime,ent);
            GIT2_ARRAY_LOOKUP_SUBOBJECT_DEREFERENCE(arr,tm,mtime,ent);
            GIT2_ARRAY_LOOKUP_LONG(arr,dev,ent);
            GIT2_ARRAY_LOOKUP_LONG(arr,ino,ent);
            GIT2_ARRAY_LOOKUP_LONG(arr,mode,ent);
            GIT2_ARRAY_LOOKUP_LONG(arr,uid,ent);
            GIT2_ARRAY_LOOKUP_LONG(arr,gid,ent);
            GIT2_ARRAY_LOOKUP_LONG(arr,file_size,ent);
            GIT2_ARRAY_LOOKUP_OID(arr,id,ent);
            GIT2_ARRAY_LOOKUP_LONG(arr,flags,ent);
            GIT2_ARRAY_LOOKUP_LONG(arr,flags_extended,ent);
            GIT2_ARRAY_LOOKUP_STRING(arr,path,ent);

            return &ent;
        }

    private:
        git_index_entry ent;

        class php_git_index_time:
            public php_array_base
        {
        public:
            php_git_index_time()
            {
                memset(&tv,0,sizeof(git_index_time));
            }

            const git_index_time* byval_git2()
            {
                array_wrapper arr(value);

                GIT2_ARRAY_LOOKUP_LONG(arr,seconds,tv);
                GIT2_ARRAY_LOOKUP_LONG(arr,nanoseconds,tv);

                return &tv;
            }

        private:
            git_index_time tv;
        };
    };

    // Provide a nullable version of git_index_entry.

    class php_git_index_entry_nullable:
        public php_option_array
    {
    public:
        const git_index_entry* byval_git2()
        {
            if (is_null()) {
                return nullptr;
            }

            entry.set_value(get_value());
            return entry.byval_git2();
        }

    private:
        php_git_index_entry entry;
    };

    // Provide a type for converting git_index_entry to PHP array.

    class php_git_index_entry_ref
    {
    public:
        php_git_index_entry_ref():
            ent(nullptr)
        {
        }

        const git_index_entry** byval_git2()
        {
            return &ent;
        }

        void ret(zval* return_value)
        {
            if (ent != nullptr) {
                php_git2::convert_index_entry(return_value,ent);
            }
            else {
                ZVAL_NULL(return_value);
            }
        }

    private:
        const git_index_entry* ent;
    };

    class php_git_index_entry_out:
        public php_output_parameter,
        public php_git_index_entry_ref
    {
    public:
        ~php_git_index_entry_out()
        {
            ret(get_value());
        }
    };

    // Provide a rethandler for php_git_index_entry.

    class php_git_index_entry_rethandler
    {
    public:
        template<typename... Ts>
        bool ret(const git_index_entry* entry,zval* return_value,local_pack<Ts...>& pack)
        {
            if (entry == nullptr) {
                RETVAL_FALSE;
                return true;
            }

            php_git2::convert_index_entry(return_value,entry);
            return true;
        }
    };

} // namespace php_git2

static constexpr auto ZIF_git_index_add = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        const git_index_entry*>::func<git_index_add>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_git_index_entry
        >
    >;

static constexpr auto ZIF_git_index_add_all = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_index*,
        const git_strarray*,
        unsigned int,
        git_index_matched_path_cb,
        void*>::func<git_index_add_all>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_git_strarray_array,
        php_git2::php_long_cast<unsigned int>,
        php_git2::php_callback_handler_nullable<php_git2::index_matched_path_callback>,
        php_git2::php_callback_sync_nullable
        >,
    php_git2::php_git_callback_error_rethandler,
    php_git2::sequence<0,1,2,4,4>,
    php_git2::sequence<0,1,2,3,4>
    >;

static constexpr auto ZIF_git_index_add_bypath = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        const char*>::func<git_index_add_bypath>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_index_add_from_buffer = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        const git_index_entry*,
        const void*,
        size_t>::func<git_index_add_from_buffer>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_git_index_entry,
        php_git2::connector_wrapper<php_git2::php_string_length_connector<size_t> >,
        php_git2::php_string
        >,
    -1,
    php_git2::sequence<0,1,3>,
    php_git2::sequence<0,1,3,2>
    >;

static constexpr auto ZIF_git_index_caps = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const git_index*>::func<git_index_caps>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>
        >,
    0
    >;

static constexpr auto ZIF_git_index_checksum = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        git_index*>::func<git_index_checksum>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>
        >,
    0
    >;

static constexpr auto ZIF_git_index_clear = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*>::func<git_index_clear>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>
        >
    >;

static constexpr auto ZIF_git_index_conflict_add = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        const git_index_entry*,
        const git_index_entry*,
        const git_index_entry*>::func<git_index_conflict_add>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_git_index_entry_nullable,
        php_git2::php_git_index_entry_nullable,
        php_git2::php_git_index_entry_nullable
        >
    >;

static constexpr auto ZIF_git_index_conflict_cleanup = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*>::func<git_index_conflict_cleanup>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>
        >
    >;

static constexpr auto ZIF_git_index_conflict_get = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const git_index_entry**,
        const git_index_entry**,
        const git_index_entry**,
        git_index*,
        const char*>::func<git_index_conflict_get>,
    php_git2::local_pack<
        php_git2::php_git_index_entry_out,
        php_git2::php_git_index_entry_out,
        php_git2::php_git_index_entry_out,
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_string
        >,
    -1
    >;

static constexpr auto ZIF_git_index_conflict_iterator_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_index_conflict_iterator>
        >
    >;

static constexpr auto ZIF_git_index_conflict_iterator_new = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_index_conflict_iterator**,
        git_index*>::func<git_index_conflict_iterator_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_index_conflict_iterator>,
        php_git2::php_resource<php_git2::php_git_index>
        >,
    php_git2::sequence<0,1>, // Make the iterator depend on the index.
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_index_conflict_next = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_index_entry**,
        const git_index_entry**,
        const git_index_entry**,
        git_index_conflict_iterator*>::func<git_index_conflict_next>,
    php_git2::local_pack<
        php_git2::php_git_index_entry_out,
        php_git2::php_git_index_entry_out,
        php_git2::php_git_index_entry_out,
        php_git2::php_resource<php_git2::php_git_index_conflict_iterator>
        >,
    php_git2::php_boolean_iterover_rethandler
    >;

static constexpr auto ZIF_git_index_conflict_remove = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        const char*>::func<git_index_conflict_remove>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_index_entrycount = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        const git_index*>::func<git_index_entrycount>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>
        >,
    0
    >;

static constexpr auto ZIF_git_index_entry_is_conflict = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_index_entry*>::func<git_index_entry_is_conflict>,
    php_git2::local_pack<
        php_git2::php_git_index_entry
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_git_index_entry_stage = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const git_index_entry*>::func<git_index_entry_stage>,
    php_git2::local_pack<
        php_git2::php_git_index_entry
        >,
    0
    >;

static constexpr auto ZIF_git_index_find = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        size_t*,
        git_index*,
        const char*>::func<git_index_find>,
    php_git2::local_pack<
        php_git2::php_long_ref<size_t>,
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_index_find_prefix = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        size_t*,
        git_index*,
        const char*>::func<git_index_find_prefix>,
    php_git2::local_pack<
        php_git2::php_long_ref<size_t>,
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_index_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_index>
        >
    >;

static constexpr auto ZIF_git_index_get_byindex = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        const git_index_entry*,
        git_index*,
        size_t>::func<git_index_get_byindex>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_long_cast<size_t>
        >,
    php_git2::php_git_index_entry_rethandler
    >;

static constexpr auto ZIF_git_index_get_bypath = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        const git_index_entry*,
        git_index*,
        const char*,
        int>::func<git_index_get_bypath>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_string,
        php_git2::php_long_cast<int>
        >,
    php_git2::php_git_index_entry_rethandler
    >;

static constexpr auto ZIF_git_index_has_conflicts = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_index*>::func<git_index_has_conflicts>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_git_index_new = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index**>::func<git_index_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_index>
        >,
    1,
    php_git2::sequence<>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_git_index_open = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index**,
        const char*>::func<git_index_open>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_index>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_index_owner = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_repository*,
        const git_index*>::func<git_index_owner>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_resource_ref<php_git2::php_git_repository_nofree>
        >,
    php_git2::php_owner_rethandler<php_git2::php_git_index>,
    php_git2::sequence<0>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_git_index_path = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_index*>::func<git_index_path>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>
        >,
    0
    >;

static constexpr auto ZIF_git_index_read = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        int>::func<git_index_read>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_bool
        >
    >;

static constexpr auto ZIF_git_index_read_tree = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        const git_tree*>::func<git_index_read_tree>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_resource<php_git2::php_git_tree>
        >
    >;

static constexpr auto ZIF_git_index_remove = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        const char*,
        int>::func<git_index_remove>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_string,
        php_git2::php_long
        >
    >;

static constexpr auto ZIF_git_index_remove_all = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_index*,
        const git_strarray*,
        git_index_matched_path_cb,
        void*>::func<git_index_remove_all>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_git_strarray_array,
        php_git2::php_callback_handler<php_git2::index_matched_path_callback>,
        php_git2::php_callback_sync
        >,
    php_git2::php_git_callback_error_rethandler,
    php_git2::sequence<0,1,3,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_index_remove_bypath = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        const char*>::func<git_index_remove_bypath>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_index_remove_directory = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        const char*,
        int>::func<git_index_remove_directory>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_string,
        php_git2::php_long
        >
    >;

static constexpr auto ZIF_git_index_set_caps = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        int>::func<git_index_set_caps>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_long
        >
    >;

static constexpr auto ZIF_git_index_set_version = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        unsigned int>::func<git_index_set_version>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_long_cast<unsigned int>
        >
    >;

static constexpr auto ZIF_git_index_update_all = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_index*,
        const git_strarray*,
        git_index_matched_path_cb,
        void*>::func<git_index_update_all>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_git_strarray_array,
        php_git2::php_callback_handler<php_git2::index_matched_path_callback>,
        php_git2::php_callback_sync
        >,
    php_git2::php_git_callback_error_rethandler,
    php_git2::sequence<0,1,3,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_index_version = zif_php_git2_function<
    php_git2::func_wrapper<
        unsigned int,
        git_index*>::func<git_index_version>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>
        >,
    0
    >;

static constexpr auto ZIF_git_index_write = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*>::func<git_index_write>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>
        >,
    -1
    >;

static constexpr auto ZIF_git_index_write_tree = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_index*>::func<git_index_write_tree>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_index>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_index_write_tree_to = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_index*,
        git_repository*>::func<git_index_write_tree_to>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

#define GIT_INDEX_FE                                \
    PHP_GIT2_FE(git_index_add)                      \
    PHP_GIT2_FE(git_index_add_all)                  \
    PHP_GIT2_FE(git_index_add_bypath)               \
    PHP_GIT2_FE(git_index_add_from_buffer)          \
    PHP_GIT2_FE(git_index_caps)                     \
    PHP_GIT2_FE(git_index_checksum)                 \
    PHP_GIT2_FE(git_index_clear)                    \
    PHP_GIT2_FE(git_index_conflict_add)             \
    PHP_GIT2_FE(git_index_conflict_cleanup)         \
    PHP_GIT2_FE(git_index_conflict_get)             \
    PHP_GIT2_FE(git_index_conflict_iterator_free)   \
    PHP_GIT2_FE(git_index_conflict_iterator_new)    \
    PHP_GIT2_FE(git_index_conflict_next)            \
    PHP_GIT2_FE(git_index_conflict_remove)          \
    PHP_GIT2_FE(git_index_entrycount)               \
    PHP_GIT2_FE(git_index_entry_is_conflict)        \
    PHP_GIT2_FE(git_index_entry_stage)              \
    PHP_GIT2_FE(git_index_find)                     \
    PHP_GIT2_FE(git_index_find_prefix)              \
    PHP_GIT2_FE(git_index_free)                     \
    PHP_GIT2_FE(git_index_get_byindex)              \
    PHP_GIT2_FE(git_index_get_bypath)               \
    PHP_GIT2_FE(git_index_has_conflicts)            \
    PHP_GIT2_FE(git_index_new)                      \
    PHP_GIT2_FE(git_index_open)                     \
    PHP_GIT2_FE(git_index_owner)                    \
    PHP_GIT2_FE(git_index_path)                     \
    PHP_GIT2_FE(git_index_read)                     \
    PHP_GIT2_FE(git_index_read_tree)                \
    PHP_GIT2_FE(git_index_remove)                   \
    PHP_GIT2_FE(git_index_remove_all)               \
    PHP_GIT2_FE(git_index_remove_bypath)            \
    PHP_GIT2_FE(git_index_remove_directory)         \
    PHP_GIT2_FE(git_index_set_caps)                 \
    PHP_GIT2_FE(git_index_set_version)              \
    PHP_GIT2_FE(git_index_update_all)               \
    PHP_GIT2_FE(git_index_version)                  \
    PHP_GIT2_FE(git_index_write)                    \
    PHP_GIT2_FE(git_index_write_tree)               \
    PHP_GIT2_FE(git_index_write_tree_to)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
