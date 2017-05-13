/*
 * tree.h
 *
 * This file is a part of php-git2.
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

    // Provide a rethandler for git_tree_owner().
    class php_git_tree_owner_rethandler
    {
    public:
        bool ret(
            git_repository* retval,
            zval* return_value,
            local_pack<php_resource<php_git_tree>,
                php_resource_ref<php_git_repository_nofree> >&& pack)
        {
            if (retval == nullptr) {
                return false;
            }

            auto& resource = pack.get<1>();
            *resource.byval_git2() = retval;
            resource.ret(return_value);

            return true;
        }
    };
}

// Template declarations for bindings:

static constexpr auto ZIF_GIT_TREE_FREE = zif_php_git2_function_void<
    php_git2::func_wrapper<
        void,
        git_tree*
        >::func<git_tree_free>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tree>
        >
    >;

static constexpr auto ZIF_GIT_TREE_ID = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        const git_tree*
        >::func<git_tree_id>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tree>
        >,
    0
    >;

static constexpr auto ZIF_GIT_TREE_LOOKUP = zif_php_git2_function<
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
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
    >;

static constexpr auto ZIF_GIT_TREE_LOOKUP_PREFIX = zif_php_git2_function<
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
    1,
    php_git2::sequence<1,3>,
    php_git2::sequence<0,1,3,2>,
    php_git2::sequence<0,0,1,0>
    >;

static constexpr auto ZIF_GIT_TREE_OWNER = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_repository*,
        const git_tree*
        >::func<git_tree_owner>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tree>,
        php_git2::php_resource_ref<php_git2::php_git_repository_nofree>
        >,
    php_git2::php_git_tree_owner_rethandler,
    php_git2::sequence<0>,
    php_git2::sequence<0>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_GIT_TREE_DUP = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_tree**,
        git_tree*
        >::func<git_tree_dup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_tree>,
        php_git2::php_resource<php_git2::php_git_tree>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

static constexpr auto ZIF_GIT_TREE_ENTRYCOUNT = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        const git_tree*
        >::func<git_tree_entrycount>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tree>
        >,
    0
    >;

static constexpr auto ZIF_GIT_TREE_ENTRY_FREE = zif_php_git2_function_void<
    php_git2::func_wrapper<
        void,
        git_tree_entry*
        >::func<git_tree_entry_free>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_tree_entry>
        >
    >;

// PHP function entry macro for this module:

#define GIT_TREE_FE                                                     \
    PHP_GIT2_FE(git_tree_free,ZIF_GIT_TREE_FREE,NULL)                   \
    PHP_GIT2_FE(git_tree_id,ZIF_GIT_TREE_ID,NULL)                       \
    PHP_GIT2_FE(git_tree_lookup,ZIF_GIT_TREE_LOOKUP,NULL)               \
    PHP_GIT2_FE(git_tree_lookup_prefix,ZIF_GIT_TREE_LOOKUP_PREFIX,NULL) \
    PHP_GIT2_FE(git_tree_owner,ZIF_GIT_TREE_OWNER,NULL)                 \
    PHP_GIT2_FE(git_tree_entrycount,ZIF_GIT_TREE_ENTRYCOUNT,NULL)       \
    PHP_GIT2_FE(git_tree_entry_free,ZIF_GIT_TREE_ENTRY_FREE,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
