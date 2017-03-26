/*
 * blob.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_BLOB_H
#define PHPGIT2_BLOB_H

namespace php_git2
{

    // Explicitly specialize git2_resource destructor for git_blob.

    template<> php_git_blob::~git2_resource()
    {
        git_blob_free(handle);
    }

} // namespace php_git2

#endif

static constexpr auto ZIF_GIT_BLOB_CREATE_FROMBUFFER = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_repository*,
        const void*,
        size_t
        >::func<git_blob_create_frombuffer>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::connector_wrapper<php_git2::php_string_length_connector<size_t> >,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,3>,
    php_git2::sequence<0,1,3,2>,
    php_git2::sequence<0,0,1,0>
    >;

static constexpr auto ZIF_GIT_BLOB_CREATE_FROMDISK = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_repository*,
        const char*
        >::func<git_blob_create_fromdisk>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
    >;

static constexpr auto ZIF_GIT_BLOB_CREATE_FROMWORKDIR = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_repository*,
        const char*
        >::func<git_blob_create_fromworkdir>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
    >;

static constexpr auto ZIF_GIT_BLOB_FILTERED_CONTENT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        git_blob*,
        const char*,
        int
        >::func<git_blob_filtered_content>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_resource<php_git2::php_git_blob>,
        php_git2::php_string,
        php_git2::php_bool
        >,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>,
    php_git2::sequence<0,0,1,2>
    >;

static constexpr auto ZIF_GIT_BLOB_FREE = zif_php_git2_function_void<
    php_git2::func_wrapper<
        void,
        git_blob*
        >::func<git_blob_free>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_blob>
        >
    >;

static constexpr auto ZIF_GIT_BLOB_ID = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        const git_blob*
        >::func<git_blob_id>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_blob>
        >,
    0
    >;

static constexpr auto ZIF_GIT_BLOB_IS_BINARY = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const git_blob*
        >::func<git_blob_is_binary>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_blob>
        >,
    0
    >;

static constexpr auto ZIF_GIT_BLOB_LOOKUP = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_blob**,
        git_repository*,
        const git_oid*
        >::func<git_blob_lookup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_blob>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_git_oid_fromstr
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
    >;

static constexpr auto ZIF_GIT_BLOB_LOOKUP_PREFIX = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_blob**,
        git_repository*,
        const git_oid*,
        size_t
        >::func<git_blob_lookup_prefix>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_blob>,
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

static constexpr auto ZIF_GIT_BLOB_OWNER = zif_php_git2_function<
    php_git2::func_wrapper<
        git_repository*,
        const git_blob*
        >::func<git_blob_owner>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_blob>,
        php_git2::php_resource_ref<php_git2::php_git_repository_nofree>
        >,
    0,
    php_git2::sequence<0>,
    php_git2::sequence<0>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_GIT_BLOB_RAWCONTENT = zif_php_git2_function<
    php_git2::func_wrapper<
        const void*,
        const git_blob*
        >::func<git_blob_rawcontent>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_blob>
        >,
    0
    >;

static constexpr auto ZIF_GIT_BLOB_RAWSIZE = zif_php_git2_function<
    php_git2::func_wrapper<
        git_off_t,
        const git_blob*
        >::func<git_blob_rawsize>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_blob>
        >,
    0
    >;

#define GIT_BLOB_FE                                                     \
    PHP_GIT2_FE(git_blob_create_frombuffer,ZIF_GIT_BLOB_CREATE_FROMBUFFER,NULL) \
    PHP_GIT2_FE(git_blob_create_fromdisk,ZIF_GIT_BLOB_CREATE_FROMDISK,NULL) \
    PHP_GIT2_FE(git_blob_create_fromworkdir,ZIF_GIT_BLOB_CREATE_FROMWORKDIR,NULL) \
    PHP_GIT2_FE(git_blob_filtered_content,ZIF_GIT_BLOB_FILTERED_CONTENT,NULL) \
    PHP_GIT2_FE(git_blob_free,ZIF_GIT_BLOB_FREE,NULL)                   \
    PHP_GIT2_FE(git_blob_id,ZIF_GIT_BLOB_ID,NULL)                       \
    PHP_GIT2_FE(git_blob_is_binary,ZIF_GIT_BLOB_IS_BINARY,NULL)         \
    PHP_GIT2_FE(git_blob_lookup,ZIF_GIT_BLOB_LOOKUP,NULL)               \
    PHP_GIT2_FE(git_blob_lookup_prefix,ZIF_GIT_BLOB_LOOKUP_PREFIX,NULL) \
    PHP_GIT2_FE(git_blob_owner,ZIF_GIT_BLOB_OWNER,NULL)                 \
    PHP_GIT2_FE(git_blob_rawcontent,ZIF_GIT_BLOB_RAWCONTENT,NULL)       \
    PHP_GIT2_FE(git_blob_rawsize,ZIF_GIT_BLOB_RAWSIZE,NULL)

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
