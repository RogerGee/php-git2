/*
 * object.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_OBJECT_H
#define PHPGIT2_OBJECT_H

namespace php_git2
{

    // Explicitly specialize git2_resource destructor for git_object.
    template<> php_git_object::~git2_resource()
    {
        git_object_free(handle);
    }

} // namespace php_git2

// Functions:

static constexpr auto ZIF_git_object_id = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        const git_object*>::func<git_object_id>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_object>
        >,
    0
    >;

static constexpr auto ZIF_git_object_lookup = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_object**,
        git_repository*,
        const git_oid*,
        git_object_t>::func<git_object_lookup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_object>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_git_oid_fromstr,
        php_git2::php_long_cast<git_object_t>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_object_peel = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_object**,
        const git_object*,
        git_object_t>::func<git_object_peel>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_object>,
        php_git2::php_resource<php_git2::php_git_object>,
        php_git2::php_long_cast<git_object_t>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_object_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_object>
        >
    >;

static constexpr auto ZIF_git_object_dup = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_object**,
        git_object*>::func<git_object_dup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_object>,
        php_git2::php_resource<php_git2::php_git_object>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_object_lookup_bypath = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_object**,
        const git_object*,
        const char*,
        git_object_t>::func<git_object_lookup_bypath>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_object>,
        php_git2::php_resource<php_git2::php_git_object>,
        php_git2::php_string,
        php_git2::php_long_cast<git_object_t>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_object_lookup_prefix = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_object**,
        git_repository*,
        const git_oid*,
        size_t,
        git_object_t>::func<git_object_lookup_prefix>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_object>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::connector_wrapper<
            php_git2::php_string_length_connector<size_t,php_git2::php_git_oid_fromstr>
            >,
        php_git2::php_git_oid_fromstr,
        php_git2::php_long_cast<git_object_t>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,3,4>,
    php_git2::sequence<0,1,3,2,4>
    >;

static constexpr auto ZIF_git_object_owner = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_repository*,
        const git_object*>::func<git_object_owner>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_object>,
        php_git2::php_resource_ref<php_git2::php_git_repository_nofree>
        >,
    php_git2::php_owner_rethandler<php_git2::php_git_object>,
    php_git2::sequence<0>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_git_object_short_id = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        const git_object*>::func<git_object_short_id>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_resource<php_git2::php_git_object>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_object_string2type = zif_php_git2_function<
    php_git2::func_wrapper<
        git_object_t,
        const char*>::func<git_object_string2type>,
    php_git2::local_pack<
        php_git2::php_string
        >,
    0
    >;

static constexpr auto ZIF_git_object_type = zif_php_git2_function<
    php_git2::func_wrapper<
        git_object_t,
        const git_object*>::func<git_object_type>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_object>
        >,
    0
    >;

static constexpr auto ZIF_git_object_type2string = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        git_object_t>::func<git_object_type2string>,
    php_git2::local_pack<
        php_git2::php_long_cast<git_object_t>
        >,
    0
    >;

static constexpr auto ZIF_git_object_typeisloose = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_object_t>::func<git_object_typeisloose>,
    php_git2::local_pack<
        php_git2::php_long_cast<git_object_t>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

// Function Entries:

#define GIT_OBJECT_FE                           \
    PHP_GIT2_FE(git_object_id)                  \
    PHP_GIT2_FE(git_object_lookup)              \
    PHP_GIT2_FE(git_object_peel)                \
    PHP_GIT2_FE(git_object_free)                \
    PHP_GIT2_FE(git_object_dup)                 \
    PHP_GIT2_FE(git_object_lookup_bypath)       \
    PHP_GIT2_FE(git_object_lookup_prefix)       \
    PHP_GIT2_FE(git_object_owner)               \
    PHP_GIT2_FE(git_object_short_id)            \
    PHP_GIT2_FE(git_object_string2type)         \
    PHP_GIT2_FE(git_object_type)                \
    PHP_GIT2_FE(git_object_type2string)         \
    PHP_GIT2_FE(git_object_typeisloose)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
