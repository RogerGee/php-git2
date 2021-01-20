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

static constexpr auto ZIF_GIT_OBJECT_ID = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        const git_object*>::func<git_object_id>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_object>
        >,
    0
    >;

static constexpr auto ZIF_GIT_OBJECT_LOOKUP = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_object**,
        git_repository*,
        const git_oid*,
        git_otype>::func<git_object_lookup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_object>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_git_oid_fromstr,
        php_git2::php_long_cast<git_otype>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_GIT_OBJECT_PEEL = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_object**,
        const git_object*,
        git_otype>::func<git_object_peel>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_object>,
        php_git2::php_resource<php_git2::php_git_object>,
        php_git2::php_long_cast<git_otype>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_GIT_OBJECT_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_object>
        >
    >;

static constexpr auto ZIF_GIT_OBJECT_DUP = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_object**,
        git_object*>::func<git_object_dup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_object>,
        php_git2::php_resource<php_git2::php_git_object>
        >,
    -1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_GIT_OBJECT_LOOKUP_BYPATH = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_object**,
        const git_object*,
        const char*,
        git_otype>::func<git_object_lookup_bypath>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_object>,
        php_git2::php_resource<php_git2::php_git_object>,
        php_git2::php_string,
        php_git2::php_long_cast<git_otype>
        >,
    php_git2::sequence<0,1>,
    -1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_GIT_OBJECT_LOOKUP_PREFIX = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_object**,
        git_repository*,
        const git_oid*,
        size_t,
        git_otype>::func<git_object_lookup_prefix>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_object>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::connector_wrapper<
            php_git2::php_string_length_connector<size_t,php_git2::php_git_oid_fromstr>
            >,
        php_git2::php_git_oid_fromstr,
        php_git2::php_long_cast<git_otype>
        >,
    php_git2::sequence<0,1>,
    -1,
    php_git2::sequence<1,3,4>,
    php_git2::sequence<0,1,3,2,4>
    >;

static constexpr auto ZIF_GIT_OBJECT_OWNER = zif_php_git2_function_rethandler<
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

static constexpr auto ZIF_GIT_OBJECT_SHORT_ID = zif_php_git2_function<
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

static constexpr auto ZIF_GIT_OBJECT_STRING2TYPE = zif_php_git2_function<
    php_git2::func_wrapper<
        git_otype,
        const char*>::func<git_object_string2type>,
    php_git2::local_pack<
        php_git2::php_string
        >,
    0
    >;

static constexpr auto ZIF_GIT_OBJECT_TYPE = zif_php_git2_function<
    php_git2::func_wrapper<
        git_otype,
        const git_object*>::func<git_object_type>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_object>
        >,
    0
    >;

static constexpr auto ZIF_GIT_OBJECT_TYPE2STRING = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        git_otype>::func<git_object_type2string>,
    php_git2::local_pack<
        php_git2::php_long_cast<git_otype>
        >,
    0
    >;

static constexpr auto ZIF_GIT_OBJECT_TYPEISLOOSE = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_otype>::func<git_object_typeisloose>,
    php_git2::local_pack<
        php_git2::php_long_cast<git_otype>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

// Function Entries:

#define GIT_OBJECT_FE                                                   \
    PHP_GIT2_FE(git_object_id,ZIF_GIT_OBJECT_ID,NULL)                   \
    PHP_GIT2_FE(git_object_lookup,ZIF_GIT_OBJECT_LOOKUP,NULL)           \
    PHP_GIT2_FE(git_object_peel,ZIF_GIT_OBJECT_PEEL,NULL)               \
    PHP_GIT2_FE(git_object_free,ZIF_GIT_OBJECT_FREE,NULL)               \
    PHP_GIT2_FE(git_object_dup,ZIF_GIT_OBJECT_DUP,NULL)                 \
    PHP_GIT2_FE(git_object_lookup_bypath,ZIF_GIT_OBJECT_LOOKUP_BYPATH,NULL) \
    PHP_GIT2_FE(git_object_lookup_prefix,ZIF_GIT_OBJECT_LOOKUP_PREFIX,NULL) \
    PHP_GIT2_FE(git_object_owner,ZIF_GIT_OBJECT_OWNER,NULL) \
    PHP_GIT2_FE(git_object_short_id,ZIF_GIT_OBJECT_SHORT_ID,NULL)       \
    PHP_GIT2_FE(git_object_string2type,ZIF_GIT_OBJECT_STRING2TYPE,NULL) \
    PHP_GIT2_FE(git_object_type,ZIF_GIT_OBJECT_TYPE,NULL)               \
    PHP_GIT2_FE(git_object_type2string,ZIF_GIT_OBJECT_TYPE2STRING,NULL) \
    PHP_GIT2_FE(git_object_typeisloose,ZIF_GIT_OBJECT_TYPEISLOOSE,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
