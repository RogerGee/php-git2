/*
 * index.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_INDEX_H
#define PHPGIT2_INDEX_H

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
        public php_value_base
    {
    public:
        php_git_index_entry(TSRMLS_D)
        {
            memset(&ent,0,sizeof(git_index_entry));
        }

        const git_index_entry* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            if (!Z_TYPE_P(value)) {
                error("array",argno);
            }

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
            public php_value_base
        {
        public:
            php_git_index_time(TSRMLS_D)
            {
                memset(&tv,0,sizeof(git_index_time));
            }

            const git_index_time* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
            {
                assert(Z_TYPE_P(value) == IS_ARRAY);

                array_wrapper arr(value);

                GIT2_ARRAY_LOOKUP_LONG(arr,seconds,tv);
                GIT2_ARRAY_LOOKUP_LONG(arr,nanoseconds,tv);

                return &tv;
            }

        private:
            git_index_time tv;
        };
    };

    // Provide a type for converting git_index_entry to PHP array.

    class php_git_index_entry_ref
    {
    public:
        ZTS_CONSTRUCTOR(php_git_index_entry_ref)

        const git_index_entry** byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            return &ent;
        }

        void ret(zval* return_value)
        {
            php_git2::convert_index_entry(return_value,ent);
        }
    private:
        const git_index_entry* ent;
    };

    class php_git_index_entry_out:
        public php_value_base,
        public php_git_index_entry_ref
    {
    public:
        ZTS_CONSTRUCTOR_WITH_BASE(php_git_index_entry_out,php_git_index_entry_ref)

        ~php_git_index_entry_out()
        {
            ret(value);
        }
    };

    // Provide a rethandler for php_git_index_entry.

    class php_git_index_entry_rethandler
    {
    public:
        template<typename... Ts>
        bool ret(const git_index_entry* entry,zval* return_value,local_pack<Ts...>& pack)
        {
            php_git2::convert_index_entry(return_value,entry);
            return true;
        }
    };

} // namespace php_git2

static constexpr auto ZIF_GIT_INDEX_ADD = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        const git_index_entry*>::func<git_index_add>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_git_index_entry
        >
    >;

static constexpr auto ZIF_GIT_INDEX_ADD_ALL = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        const git_strarray*,
        unsigned int,
        git_index_matched_path_cb,
        void*>::func<git_index_add_all>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_strarray_array,
        php_git2::php_long_cast<unsigned int>,
        php_git2::php_callback_handler<php_git2::index_matched_path_callback>,
        php_git2::php_callback_sync
        >,
    -1,
    php_git2::sequence<0,1,2,4,4>,
    php_git2::sequence<0,1,2,3,4>,
    php_git2::sequence<0,1,2,0,3>
    >;

static constexpr auto ZIF_GIT_INDEX_ADD_BYPATH = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        const char*>::func<git_index_add_bypath>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_GIT_INDEX_ADD_FROMBUFFER = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        const git_index_entry*,
        const void*,
        size_t>::func<git_index_add_frombuffer>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_git_index_entry,
        php_git2::connector_wrapper<php_git2::php_string_length_connector<size_t> >,
        php_git2::php_string
        >,
    -1,
    php_git2::sequence<0,1,3>,
    php_git2::sequence<0,1,3,2>,
    php_git2::sequence<0,1,2,0>
    >;

static constexpr auto ZIF_GIT_INDEX_CAPS = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const git_index*>::func<git_index_caps>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>
        >,
    0
    >;

static constexpr auto ZIF_GIT_INDEX_CHECKSUM = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        git_index*>::func<git_index_checksum>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>
        >,
    0
    >;

static constexpr auto ZIF_GIT_INDEX_CLEAR = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*>::func<git_index_clear>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>
        >
    >;

static constexpr auto ZIF_GIT_INDEX_CONFLICT_ADD = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        const git_index_entry*,
        const git_index_entry*,
        const git_index_entry*>::func<git_index_conflict_add>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_git_index_entry,
        php_git2::php_git_index_entry,
        php_git2::php_git_index_entry
        >
    >;

static constexpr auto ZIF_GIT_INDEX_CONFLICT_CLEANUP = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*>::func<git_index_conflict_cleanup>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>
        >
    >;

static constexpr auto ZIF_GIT_INDEX_CONFLICT_GET = zif_php_git2_function<
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
ZEND_BEGIN_ARG_INFO_EX(git_index_conflict_get_arginfo,0,0,5)
    ZEND_ARG_PASS_INFO(1)
    ZEND_ARG_PASS_INFO(1)
    ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

static constexpr auto ZIF_GIT_INDEX_CONFLICT_ITERATOR_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_index_conflict_iterator>
        >
    >;

static constexpr auto ZIF_GIT_INDEX_CONFLICT_ITERATOR_NEW = zif_php_git2_function_setdeps<
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
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

static constexpr auto ZIF_GIT_INDEX_CONFLICT_NEXT = zif_php_git2_function_rethandler<
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
ZEND_BEGIN_ARG_INFO_EX(git_index_conflict_next_arginfo,0,0,4)
    ZEND_ARG_PASS_INFO(1)
    ZEND_ARG_PASS_INFO(1)
    ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

static constexpr auto ZIF_GIT_INDEX_CONFLICT_REMOVE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        const char*>::func<git_index_conflict_remove>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_GIT_INDEX_ENTRYCOUNT = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        const git_index*>::func<git_index_entrycount>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>
        >,
    0
    >;

static constexpr auto ZIF_GIT_INDEX_ENTRY_IS_CONFLICT = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_index_entry*>::func<git_index_entry_is_conflict>,
    php_git2::local_pack<
        php_git2::php_git_index_entry
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_GIT_INDEX_ENTRY_STAGE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const git_index_entry*>::func<git_index_entry_stage>,
    php_git2::local_pack<
        php_git2::php_git_index_entry
        >,
    0
    >;

static constexpr auto ZIF_GIT_INDEX_FIND = zif_php_git2_function<
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
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
    >;

static constexpr auto ZIF_GIT_INDEX_FIND_PREFIX = zif_php_git2_function<
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
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
    >;

static constexpr auto ZIF_GIT_INDEX_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_index>
        >
    >;

static constexpr auto ZIF_GIT_INDEX_GET_BYINDEX = zif_php_git2_function_rethandler<
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

static constexpr auto ZIF_GIT_INDEX_GET_BYPATH = zif_php_git2_function_rethandler<
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

static constexpr auto ZIF_GIT_INDEX_HAS_CONFLICTS = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_index*>::func<git_index_has_conflicts>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_GIT_INDEX_NEW = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index**>::func<git_index_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_index>
        >,
    1,
    php_git2::sequence<>,
    php_git2::sequence<0>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_GIT_INDEX_OPEN = zif_php_git2_function<
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
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

static constexpr auto ZIF_GIT_INDEX_OWNER = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_repository*,
        const git_index*>::func<git_index_owner>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_resource_ref<php_git2::php_git_repository_nofree>
        >,
    php_git2::php_owner_rethandler<php_git2::php_git_index>,
    php_git2::sequence<0>,
    php_git2::sequence<0>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_GIT_INDEX_PATH = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_index*>::func<git_index_path>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>
        >,
    0
    >;

static constexpr auto ZIF_GIT_INDEX_READ = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        int>::func<git_index_read>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_bool
        >
    >;

static constexpr auto ZIF_GIT_INDEX_READ_TREE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        const git_tree*>::func<git_index_read_tree>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_resource<php_git2::php_git_tree>
        >
    >;

static constexpr auto ZIF_GIT_INDEX_REMOVE = zif_php_git2_function<
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

static constexpr auto ZIF_GIT_INDEX_REMOVE_ALL = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        const git_strarray*,
        git_index_matched_path_cb,
        void*>::func<git_index_remove_all>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_strarray_array,
        php_git2::php_callback_handler<php_git2::index_matched_path_callback>,
        php_git2::php_callback_sync
        >,
    -1,
    php_git2::sequence<0,1,3,3>,
    php_git2::sequence<0,1,2,3>,
    php_git2::sequence<0,1,0,2>
    >;

static constexpr auto ZIF_GIT_INDEX_REMOVE_BYPATH = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        const char*>::func<git_index_remove_bypath>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_GIT_INDEX_REMOVE_DIRECTORY = zif_php_git2_function<
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

static constexpr auto ZIF_GIT_INDEX_SET_CAPS = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        int>::func<git_index_set_caps>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_long
        >
    >;

static constexpr auto ZIF_GIT_INDEX_SET_VERSION = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        unsigned int>::func<git_index_set_version>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_long_cast<unsigned int>
        >
    >;

static constexpr auto ZIF_GIT_INDEX_UPDATE_ALL = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        const git_strarray*,
        git_index_matched_path_cb,
        void*>::func<git_index_update_all>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_strarray_array,
        php_git2::php_callback_handler<php_git2::index_matched_path_callback>,
        php_git2::php_callback_sync
        >,
    -1,
    php_git2::sequence<0,1,3,3>,
    php_git2::sequence<0,1,2,3>,
    php_git2::sequence<0,1,0,2>
    >;

static constexpr auto ZIF_GIT_INDEX_VERSION = zif_php_git2_function<
    php_git2::func_wrapper<
        unsigned int,
        git_index*>::func<git_index_version>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>
        >,
    0
    >;

static constexpr auto ZIF_GIT_INDEX_WRITE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*>::func<git_index_write>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>
        >,
    -1
    >;

static constexpr auto ZIF_GIT_INDEX_WRITE_TREE = zif_php_git2_function<
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
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

static constexpr auto ZIF_GIT_INDEX_WRITE_TREE_TO = zif_php_git2_function<
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
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
    >;

#define GIT_INDEX_FE                                                    \
    PHP_GIT2_FE(git_index_add,ZIF_GIT_INDEX_ADD,NULL)                   \
    PHP_GIT2_FE(git_index_add_all,ZIF_GIT_INDEX_ADD_ALL,NULL)           \
    PHP_GIT2_FE(git_index_add_bypath,ZIF_GIT_INDEX_ADD_BYPATH,NULL)     \
    PHP_GIT2_FE(git_index_add_frombuffer,ZIF_GIT_INDEX_ADD_FROMBUFFER,NULL) \
    PHP_GIT2_FE(git_index_caps,ZIF_GIT_INDEX_CAPS,NULL)                 \
    PHP_GIT2_FE(git_index_checksum,ZIF_GIT_INDEX_CHECKSUM,NULL)         \
    PHP_GIT2_FE(git_index_clear,ZIF_GIT_INDEX_CLEAR,NULL)               \
    PHP_GIT2_FE(git_index_conflict_add,ZIF_GIT_INDEX_CONFLICT_ADD,NULL) \
    PHP_GIT2_FE(git_index_conflict_cleanup,ZIF_GIT_INDEX_CONFLICT_CLEANUP,NULL) \
    PHP_GIT2_FE(git_index_conflict_get,ZIF_GIT_INDEX_CONFLICT_GET,git_index_conflict_get_arginfo) \
    PHP_GIT2_FE(git_index_conflict_iterator_free,ZIF_GIT_INDEX_CONFLICT_ITERATOR_FREE,NULL) \
    PHP_GIT2_FE(git_index_conflict_iterator_new,ZIF_GIT_INDEX_CONFLICT_ITERATOR_NEW,NULL) \
    PHP_GIT2_FE(git_index_conflict_next,ZIF_GIT_INDEX_CONFLICT_NEXT,git_index_conflict_next_arginfo) \
    PHP_GIT2_FE(git_index_conflict_remove,ZIF_GIT_INDEX_CONFLICT_REMOVE,NULL) \
    PHP_GIT2_FE(git_index_entrycount,ZIF_GIT_INDEX_ENTRYCOUNT,NULL)     \
    PHP_GIT2_FE(git_index_entry_is_conflict,ZIF_GIT_INDEX_ENTRY_IS_CONFLICT,NULL) \
    PHP_GIT2_FE(git_index_entry_stage,ZIF_GIT_INDEX_ENTRY_STAGE,NULL)   \
    PHP_GIT2_FE(git_index_find,ZIF_GIT_INDEX_FIND,NULL)                 \
    PHP_GIT2_FE(git_index_find_prefix,ZIF_GIT_INDEX_FIND_PREFIX,NULL)   \
    PHP_GIT2_FE(git_index_free,ZIF_GIT_INDEX_FREE,NULL)                 \
    PHP_GIT2_FE(git_index_get_byindex,ZIF_GIT_INDEX_GET_BYINDEX,NULL)   \
    PHP_GIT2_FE(git_index_get_bypath,ZIF_GIT_INDEX_GET_BYPATH,NULL)     \
    PHP_GIT2_FE(git_index_has_conflicts,ZIF_GIT_INDEX_HAS_CONFLICTS,NULL) \
    PHP_GIT2_FE(git_index_new,ZIF_GIT_INDEX_NEW,NULL)                   \
    PHP_GIT2_FE(git_index_open,ZIF_GIT_INDEX_OPEN,NULL)                 \
    PHP_GIT2_FE(git_index_owner,ZIF_GIT_INDEX_OWNER,NULL)               \
    PHP_GIT2_FE(git_index_path,ZIF_GIT_INDEX_PATH,NULL)                 \
    PHP_GIT2_FE(git_index_read,ZIF_GIT_INDEX_READ,NULL)                 \
    PHP_GIT2_FE(git_index_read_tree,ZIF_GIT_INDEX_READ_TREE,NULL)       \
    PHP_GIT2_FE(git_index_remove,ZIF_GIT_INDEX_REMOVE,NULL)             \
    PHP_GIT2_FE(git_index_remove_all,ZIF_GIT_INDEX_REMOVE_ALL,NULL)     \
    PHP_GIT2_FE(git_index_remove_bypath,ZIF_GIT_INDEX_REMOVE_BYPATH,NULL) \
    PHP_GIT2_FE(git_index_remove_directory,ZIF_GIT_INDEX_REMOVE_DIRECTORY,NULL) \
    PHP_GIT2_FE(git_index_set_caps,ZIF_GIT_INDEX_SET_CAPS,NULL)         \
    PHP_GIT2_FE(git_index_set_version,ZIF_GIT_INDEX_SET_VERSION,NULL)   \
    PHP_GIT2_FE(git_index_update_all,ZIF_GIT_INDEX_UPDATE_ALL,NULL)     \
    PHP_GIT2_FE(git_index_version,ZIF_GIT_INDEX_VERSION,NULL)           \
    PHP_GIT2_FE(git_index_write,ZIF_GIT_INDEX_WRITE,NULL)               \
    PHP_GIT2_FE(git_index_write_tree,ZIF_GIT_INDEX_WRITE_TREE,NULL)     \
    PHP_GIT2_FE(git_index_write_tree_to,ZIF_GIT_INDEX_WRITE_TREE_TO,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
