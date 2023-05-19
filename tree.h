/*
 * tree.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_TREE_H
#define PHPGIT2_TREE_H

namespace php_git2
{
    // Provide specializations for git2_resource destructors.

    template<> php_git_tree::~git2_resource()
    {
        git_tree_free(handle);
    }

    template<> php_git_tree_entry::~git2_resource()
    {
        git_tree_entry_free(handle);
    }

    // Provide rethandler for returning git_tree_entry.
    template<typename... Ts>
    class php_git_tree_entry_rethandler
    {
    public:
        bool ret(const git_tree_entry* retval,zval* return_value,local_pack<Ts...>& pack)
        {
            // The functions that target this return handler return NULL if the
            // entry wasn't found. This doesn't generate a giterr so we return
            // NULL as well in userspace.
            if (retval == nullptr) {
                RETVAL_NULL();
                return true;
            }

            auto&& tree = pack.template get<0>();
            const php_resource_ref<php_git_tree_entry_nofree> entry;

            // Set return value. This will create a resource for the new
            // git_tree_entry handle.
            *entry.byval_git2() = retval;
            entry.ret(return_value);

            // Make the new git_tree_entry dependent on the git_tree.
            entry.get_object()->set_parent(tree.get_object());

            return true;
        }
    };

} // namespace php_git2

// Template declarations for bindings:

static constexpr auto ZIF_git_tree_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_tree>
        >
    >;

static constexpr auto ZIF_git_tree_id = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        const git_tree*
        >::func<git_tree_id>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tree>
        >,
    0
    >;

static constexpr auto ZIF_git_tree_lookup = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_tree**,
        git_repository*,
        const git_oid*
        >::func<git_tree_lookup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_tree>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_git_oid_fromstr
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_tree_lookup_prefix = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_tree**,
        git_repository*,
        const git_oid*,
        size_t
        >::func<git_tree_lookup_prefix>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_tree>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::connector_wrapper<php_git2::php_string_length_connector<
                                        size_t,php_git2::php_git_oid_fromstr> >,
        php_git2::php_git_oid_fromstr
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,3>,
    php_git2::sequence<0,1,3,2>
    >;

static constexpr auto ZIF_git_tree_owner = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_repository*,
        const git_tree*
        >::func<git_tree_owner>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tree>,
        php_git2::php_resource_ref<php_git2::php_git_repository_nofree>
        >,
    php_git2::php_owner_rethandler<php_git2::php_git_tree>,
    php_git2::sequence<0>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_git_tree_dup = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_tree**,
        git_tree*
        >::func<git_tree_dup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_tree>,
        php_git2::php_resource<php_git2::php_git_tree>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_tree_entrycount = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        const git_tree*
        >::func<git_tree_entrycount>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tree>
        >,
    0
    >;

static constexpr auto ZIF_git_tree_entry_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_tree_entry>
        >
    >;

static constexpr auto ZIF_git_tree_entry_byid = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        const git_tree_entry*,
        const git_tree*,
        const git_oid*>::func<git_tree_entry_byid>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tree>,
        php_git2::php_git_oid_fromstr
        >,
    php_git2::php_git_tree_entry_rethandler<
        php_git2::php_resource<php_git2::php_git_tree>,
        php_git2::php_git_oid_fromstr
        >
    >;

static constexpr auto ZIF_git_tree_entry_byindex = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        const git_tree_entry*,
        const git_tree*,
        size_t>::func<git_tree_entry_byindex>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tree>,
        php_git2::php_long_cast<size_t>
        >,
    php_git2::php_git_tree_entry_rethandler<
        php_git2::php_resource<php_git2::php_git_tree>,
        php_git2::php_long_cast<size_t>
        >
    >;

static constexpr auto ZIF_git_tree_entry_byname = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        const git_tree_entry*,
        const git_tree*,
        const char*>::func<git_tree_entry_byname>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tree>,
        php_git2::php_string
        >,
    php_git2::php_git_tree_entry_rethandler<
        php_git2::php_resource<php_git2::php_git_tree>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_tree_entry_bypath = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_tree_entry**,
        const git_tree*,
        const char*>::func<git_tree_entry_bypath>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_tree_entry>,
        php_git2::php_resource<php_git2::php_git_tree>,
        php_git2::php_string
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_tree_entry_dup = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_tree_entry**,
        const git_tree_entry*>::func<git_tree_entry_dup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_tree_entry>,
        php_git2::php_resource<php_git2::php_git_tree_entry>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_tree_entry_filemode = zif_php_git2_function<
    php_git2::func_wrapper<
        git_filemode_t,
        const git_tree_entry*>::func<git_tree_entry_filemode>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tree_entry>
        >,
    0
    >;

static constexpr auto ZIF_git_tree_entry_filemode_raw = zif_php_git2_function<
    php_git2::func_wrapper<
        git_filemode_t,
        const git_tree_entry*>::func<git_tree_entry_filemode_raw>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tree_entry>
        >,
    0
    >;

static constexpr auto ZIF_git_tree_entry_name = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_tree_entry*>::func<git_tree_entry_name>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tree_entry>
        >,
    0
    >;

static constexpr auto ZIF_git_tree_entry_to_object = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_object**,
        git_repository*,
        const git_tree_entry*>::func<git_tree_entry_to_object>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_object>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_tree_entry>
        >,
    php_git2::sequence<0,1>, // set dependency to repository, not tree_entry
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_tree_entry_cmp = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const git_tree_entry*,
        const git_tree_entry*>::func<git_tree_entry_cmp>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tree_entry>,
        php_git2::php_resource<php_git2::php_git_tree_entry>
        >,
    0
    >;

static constexpr auto ZIF_git_tree_entry_id = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        const git_tree_entry*>::func<git_tree_entry_id>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tree_entry>
        >,
    0
    >;

static constexpr auto ZIF_git_tree_entry_type = zif_php_git2_function<
    php_git2::func_wrapper<
        git_object_t,
        const git_tree_entry*>::func<git_tree_entry_type>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tree_entry>
        >,
    0
    >;

static constexpr auto ZIF_git_tree_walk = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const git_tree*,
        git_treewalk_mode,
        git_treewalk_cb,
        void*>::func<git_tree_walk>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tree>,
        php_git2::php_long_cast<git_treewalk_mode>,
        php_git2::php_callback_handler<php_git2::treewalk_callback>,
        php_git2::php_callback_sync
        >,
    -1,
    php_git2::sequence<0,1,3,3>, // pass callback in twice for callable and payload
    php_git2::sequence<0,1,2,3>
    >;

// PHP function entry macro for this module:

#define GIT_TREE_FE                             \
    PHP_GIT2_FE(git_tree_free)                  \
    PHP_GIT2_FE(git_tree_id)                    \
    PHP_GIT2_FE(git_tree_lookup)                \
    PHP_GIT2_FE(git_tree_lookup_prefix)         \
    PHP_GIT2_FE(git_tree_owner)                 \
    PHP_GIT2_FE(git_tree_dup)                   \
    PHP_GIT2_FE(git_tree_entrycount)            \
    PHP_GIT2_FE(git_tree_entry_free)            \
    PHP_GIT2_FE(git_tree_entry_byid)            \
    PHP_GIT2_FE(git_tree_entry_byindex)         \
    PHP_GIT2_FE(git_tree_entry_byname)          \
    PHP_GIT2_FE(git_tree_entry_bypath)          \
    PHP_GIT2_FE(git_tree_entry_dup)             \
    PHP_GIT2_FE(git_tree_entry_filemode)        \
    PHP_GIT2_FE(git_tree_entry_filemode_raw)    \
    PHP_GIT2_FE(git_tree_entry_name)            \
    PHP_GIT2_FE(git_tree_entry_to_object)       \
    PHP_GIT2_FE(git_tree_entry_cmp)             \
    PHP_GIT2_FE(git_tree_entry_id)              \
    PHP_GIT2_FE(git_tree_entry_type)            \
    PHP_GIT2_FE(git_tree_walk)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
