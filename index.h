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

    // Specialize resource destructor for git_index_entrt for completeness'
    // sake. It should never be called by our implementation since a
    // git_index_entry is never freed by our code.
    template<> inline git2_resource<git_index_entry>::~git2_resource()
    {
        throw php_git2_exception(
            "git_index_entry cannot be freed in this implementation");
    }

    // Make an alias for a rethandler that processes git_index_entry.
    using php_git_index_entry_rethandler = php_resource_rethandler<
        php_git_index_entry,
        const git_index_entry
        >;

} // namespace php_git2

static constexpr auto ZIF_GIT_INDEX_ADD = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index*,
        const git_index_entry*>::func<git_index_add>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_resource<php_git2::php_git_index_entry>
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
        php_git2::php_resource<php_git2::php_git_index_entry>,
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
        php_git2::php_resource<php_git2::php_git_index_entry>,
        php_git2::php_resource<php_git2::php_git_index_entry>,
        php_git2::php_resource<php_git2::php_git_index_entry>
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
        const php_git2::php_resource_ref<php_git2::php_git_index_entry>,
        const php_git2::php_resource_ref<php_git2::php_git_index_entry>,
        const php_git2::php_resource_ref<php_git2::php_git_index_entry>,
        php_git2::php_resource<php_git2::php_git_index>,
        php_git2::php_string
        >,
    -1,
    php_git2::sequence<3,4>,
    php_git2::sequence<0,1,2,3,4>,
    php_git2::sequence<0,0,0,0,1>
    >;

/*static constexpr auto ZIF_GIT_INDEX_CONFLICT_ITERATOR_FREE = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_index_conflict_iterator_free>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_INDEX_CONFLICT_ITERATOR_NEW = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_index_conflict_iterator_new>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_INDEX_CONFLICT_NEXT = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_index_conflict_next>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;
*/
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
        php_git2::php_resource<php_git2::php_git_index_entry>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_GIT_INDEX_ENTRY_STAGE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const git_index_entry*>::func<git_index_entry_stage>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_index_entry>
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
    PHP_GIT2_FE(git_index_conflict_get,ZIF_GIT_INDEX_CONFLICT_GET,NULL) \
    PHP_GIT2_UNIMPLEMENTED(git_index_conflict_iterator_free,ZIF_GIT_INDEX_CONFLICT_ITERATOR_FREE,NULL) \
    PHP_GIT2_UNIMPLEMENTED(git_index_conflict_iterator_new,ZIF_GIT_INDEX_CONFLICT_ITERATOR_NEW,NULL) \
    PHP_GIT2_UNIMPLEMENTED(git_index_conflict_next,ZIF_GIT_INDEX_CONFLICT_NEXT,NULL) \
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
