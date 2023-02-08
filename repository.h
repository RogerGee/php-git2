/*
 * repository.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_REPOSITORY_H
#define PHPGIT2_REPOSITORY_H
extern "C" {
#include <git2/sys/repository.h>
}

namespace php_git2
{
    // Explicitly specialize git2_resource destructor for git_repository.
    template<> php_git_repository::~git2_resource()
    {
        git_repository_free(handle);
    }

    class php_git_repository_init_options:
        public php_option_array
    {
    public:
        php_git_repository_init_options()
        {
            git_repository_init_init_options(&opts,GIT_REPOSITORY_INIT_OPTIONS_VERSION);
        }

        git_repository_init_options* byval_git2()
        {
            if (!is_null()) {
                array_wrapper arr(value);

                GIT2_ARRAY_LOOKUP_LONG(arr,flags,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,version,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,mode,opts);
                GIT2_ARRAY_LOOKUP_STRING(arr,workdir_path,opts);
                GIT2_ARRAY_LOOKUP_STRING(arr,description,opts);
                GIT2_ARRAY_LOOKUP_STRING(arr,template_path,opts);
                GIT2_ARRAY_LOOKUP_STRING(arr,initial_head,opts);
                GIT2_ARRAY_LOOKUP_STRING(arr,origin_url,opts);
            }

            return &opts;
        }

    private:
        git_repository_init_options opts;
    };

} // namespace php_git2

// Functions:

static constexpr auto ZIF_GIT_REPOSITORY_INIT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository**,
        const char*,
        unsigned int>::func<git_repository_init>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_bool
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_GIT_REPOSITORY_NEW = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository**>::func<git_repository_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_repository>
        >,
    1,
    php_git2::sequence<>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_GIT_REPOSITORY_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_repository> >
    >;

static constexpr auto ZIF_GIT_REPOSITORY_OPEN = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository**,
        const char*>::func<git_repository_open>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_GIT_REPOSITORY_OPEN_BARE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository**,
        const char*>::func<git_repository_open_bare>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_repository>,
        php_git2::php_string>,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_GIT_REPOSITORY__CLEANUP = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*>::func<git_repository__cleanup>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >
    >;

static constexpr auto ZIF_GIT_REPOSITORY_DETACH_HEAD = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*>::func<git_repository_detach_head>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    0
    >;

static constexpr auto ZIF_GIT_REPOSITORY_DISCOVER = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        const char*,
        int,
        const char*>::func<git_repository_discover>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_string,
        php_git2::php_bool,
        php_git2::php_string_nullable
        >,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_GIT_REPOSITORY_GET_NAMESPACE = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        git_repository*>::func<git_repository_get_namespace>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    0 >;

static constexpr auto ZIF_GIT_REPOSITORY_PATH = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_repository*>::func<git_repository_path>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    0
    >;

static constexpr auto ZIF_GIT_REPOSITORY_HASHFILE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_repository*,
        const char*,
        git_object_t,
        const char*>::func<git_repository_hashfile>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_long_cast<git_object_t>,
        php_git2::php_string_nullable
        >,
    1,
    php_git2::sequence<1,2,3,4>,
    php_git2::sequence<0,1,2,3,4>
    >;

static constexpr auto ZIF_GIT_REPOSITORY_ODB = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_odb**,
        git_repository*>::func<git_repository_odb>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_odb>,
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    php_git2::sequence<0,1>, // Make the ODB dependent on the repository.
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_GIT_REPOSITORY_SET_ODB = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        git_odb*>::func<git_repository_set_odb>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_odb>
        >
    >;

static constexpr auto ZIF_GIT_REPOSITORY_SET_HEAD = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*
        >::func<git_repository_set_head>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    -1
    >;

static constexpr auto ZIF_GIT_REPOSITORY_SET_HEAD_DETACHED = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const git_oid*>::func<git_repository_set_head_detached>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_git_oid_fromstr
        >,
    php_git2::php_boolean_notfound_rethandler
    >;

static constexpr auto ZIF_GIT_REPOSITORY_HEAD = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_reference**,
        git_repository*>::func<git_repository_head>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_reference>,
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    php_git2::sequence<0,1>, // Make the reference dependent on the repository.
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_GIT_REPOSITORY_IS_EMPTY = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_repository*>::func<git_repository_is_empty>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    php_git2::php_boolean_error_rethandler<int>
    >;

static constexpr auto ZIF_GIT_REPOSITORY_IS_BARE = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_repository*>::func<git_repository_is_bare>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_GIT_REPOSITORY_IS_SHALLOW = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_repository*>::func<git_repository_is_shallow>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_GIT_REPOSITORY_HEAD_DETACHED = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_repository*>::func<git_repository_head_detached>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    php_git2::php_boolean_error_rethandler<int>
    >;

static constexpr auto ZIF_GIT_REPOSITORY_HEAD_UNBORN = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_repository*>::func<git_repository_head_unborn>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    php_git2::php_boolean_error_rethandler<int>
    >;

static constexpr auto ZIF_GIT_REPOSITORY_IDENT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const char**,
        const char**,
        const git_repository*>::func<git_repository_ident>,
    php_git2::local_pack<
        php_git2::php_string_ref,
        php_git2::php_string_out,
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;
ZEND_BEGIN_ARG_INFO_EX(git_repository_ident_arginfo,0,0,2)
    ZEND_ARG_INFO(1,email)
    ZEND_ARG_INFO(0,repo)
ZEND_END_ARG_INFO()

static constexpr auto ZIF_GIT_REPOSITORY_INIT_EXT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository**,
        const char*,
        git_repository_init_options*>::func<git_repository_init_ext>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_git_repository_init_options
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_GIT_REPOSITORY_MESSAGE = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_buf*,
        git_repository*>::func<git_repository_message>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    php_git2::php_notfound_rethandler<0>,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_GIT_REPOSITORY_MESSAGE_REMOVE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*>::func<git_repository_message_remove>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >
    >;

static constexpr auto ZIF_GIT_REPOSITORY_OPEN_EXT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository**,
        const char*,
        unsigned int,
        const char*>::func<git_repository_open_ext>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_repository>,
        php_git2::php_string_nullable,
        php_git2::php_long_cast<unsigned int>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_GIT_REPOSITORY_REINIT_FILESYSTEM = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        int>::func<git_repository_reinit_filesystem>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_bool
        >
    >;

static constexpr auto ZIF_GIT_REPOSITORY_SET_BARE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*>::func<git_repository_set_bare>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >
    >;

static constexpr auto ZIF_GIT_REPOSITORY_SET_HEAD_DETACHED_FROM_ANNOTATED = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const git_annotated_commit*>::func<git_repository_set_head_detached_from_annotated>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_annotated_commit>
        >,
    php_git2::php_boolean_notfound_rethandler
    >;

static constexpr auto ZIF_GIT_REPOSITORY_SET_IDENT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*,
        const char*>::func<git_repository_set_ident>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_GIT_REPOSITORY_SET_NAMESPACE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*>::func<git_repository_set_namespace>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_GIT_REPOSITORY_SET_WORKDIR = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*,
        int>::func<git_repository_set_workdir>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_bool
        >
    >;

static constexpr auto ZIF_GIT_REPOSITORY_STATE = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*>::func<git_repository_state>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    0
    >;

static constexpr auto ZIF_GIT_REPOSITORY_STATE_CLEANUP = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*>::func<git_repository_state_cleanup>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >
    >;

static constexpr auto ZIF_GIT_REPOSITORY_WORKDIR = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_repository*>::func<git_repository_workdir>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    0
    >;

static constexpr auto ZIF_GIT_REPOSITORY_WRAP_ODB = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository**,
        git_odb*>::func<git_repository_wrap_odb>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_odb>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_GIT_REPOSITORY_CONFIG = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_config**,
        git_repository*>::func<git_repository_config>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_config>,
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    php_git2::sequence<0,1>, // Make the git_config dependent on the repository.
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_GIT_REPOSITORY_CONFIG_SNAPSHOT = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_config**,
        git_repository*>::func<git_repository_config_snapshot>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_config>,
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    php_git2::sequence<0,1>, // Make the git_config dependent on the repository.
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_GIT_REPOSITORY_SET_CONFIG = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        git_config*>::func<git_repository_set_config>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_config>
        >
    >;

static constexpr auto ZIF_GIT_REPOSITORY_INDEX = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index**,
        git_repository*>::func<git_repository_index>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_index>,
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_GIT_REPOSITORY_SET_INDEX = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        git_index*>::func<git_repository_set_index>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_index>
        >
    >;

static constexpr auto ZIF_GIT_REPOSITORY_REFDB = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_refdb**,
        git_repository*>::func<git_repository_refdb>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_refdb>,
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    php_git2::sequence<0,1>, // Make the git_refdb dependent on the repository.
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_GIT_REPOSITORY_SET_REFDB = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        git_refdb*>::func<git_repository_set_refdb>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_refdb>
        >
    >;

static constexpr auto ZIF_GIT_REPOSITORY_FETCHHEAD_FOREACH = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        git_repository_fetchhead_foreach_cb,
        void*>::func<git_repository_fetchhead_foreach>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_callback_handler<php_git2::repository_fetchhead_foreach_callback>,
        php_git2::php_callback_sync
        >,
    -1,
    php_git2::sequence<0,2,2>, // pass callback in twice for function and payload
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_GIT_REPOSITORY_MERGEHEAD_FOREACH = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        git_repository_mergehead_foreach_cb,
        void*>::func<git_repository_mergehead_foreach>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_callback_handler<php_git2::repository_mergehead_foreach_callback>,
        php_git2::php_callback_sync
        >,
    -1,
    php_git2::sequence<0,2,2>, // pass callback in twice for function and payload
    php_git2::sequence<0,1,2>
    >;

// Function Entries:

#define GIT_REPOSITORY_FE                                               \
    PHP_GIT2_FE(git_repository_init,ZIF_GIT_REPOSITORY_INIT,NULL)       \
    PHP_GIT2_FE(git_repository_new,ZIF_GIT_REPOSITORY_NEW,NULL)         \
    PHP_GIT2_FE(git_repository_free,ZIF_GIT_REPOSITORY_FREE,NULL)       \
    PHP_GIT2_FE(git_repository_open,ZIF_GIT_REPOSITORY_OPEN,NULL)       \
    PHP_GIT2_FE(git_repository_open_bare,ZIF_GIT_REPOSITORY_OPEN_BARE,NULL) \
    PHP_GIT2_FE(git_repository__cleanup,ZIF_GIT_REPOSITORY__CLEANUP,NULL) \
    PHP_GIT2_FE(git_repository_detach_head,ZIF_GIT_REPOSITORY_DETACH_HEAD,NULL) \
    PHP_GIT2_FE(git_repository_discover,ZIF_GIT_REPOSITORY_DISCOVER,NULL) \
    PHP_GIT2_FE(git_repository_get_namespace,ZIF_GIT_REPOSITORY_GET_NAMESPACE,NULL) \
    PHP_GIT2_FE(git_repository_path,ZIF_GIT_REPOSITORY_PATH,NULL)       \
    PHP_GIT2_FE(git_repository_hashfile,ZIF_GIT_REPOSITORY_HASHFILE,NULL) \
    PHP_GIT2_FE(git_repository_odb,ZIF_GIT_REPOSITORY_ODB,NULL)         \
    PHP_GIT2_FE(git_repository_set_odb,ZIF_GIT_REPOSITORY_SET_ODB,NULL) \
    PHP_GIT2_FE(git_repository_set_head,ZIF_GIT_REPOSITORY_SET_HEAD,NULL) \
    PHP_GIT2_FE(git_repository_set_head_detached,ZIF_GIT_REPOSITORY_SET_HEAD_DETACHED,NULL) \
    PHP_GIT2_FE(git_repository_head,ZIF_GIT_REPOSITORY_HEAD,NULL)       \
    PHP_GIT2_FE(git_repository_is_empty,ZIF_GIT_REPOSITORY_IS_EMPTY,NULL) \
    PHP_GIT2_FE(git_repository_is_bare,ZIF_GIT_REPOSITORY_IS_BARE,NULL) \
    PHP_GIT2_FE(git_repository_is_shallow,ZIF_GIT_REPOSITORY_IS_SHALLOW,NULL) \
    PHP_GIT2_FE(git_repository_head_detached,ZIF_GIT_REPOSITORY_HEAD_DETACHED,NULL) \
    PHP_GIT2_FE(git_repository_head_unborn,ZIF_GIT_REPOSITORY_HEAD_UNBORN,NULL) \
    PHP_GIT2_FE(git_repository_ident,ZIF_GIT_REPOSITORY_IDENT,git_repository_ident_arginfo) \
    PHP_GIT2_FE(git_repository_init_ext,ZIF_GIT_REPOSITORY_INIT_EXT,NULL) \
    PHP_GIT2_FE(git_repository_message,ZIF_GIT_REPOSITORY_MESSAGE,NULL) \
    PHP_GIT2_FE(git_repository_message_remove,ZIF_GIT_REPOSITORY_MESSAGE_REMOVE,NULL) \
    PHP_GIT2_FE(git_repository_open_ext,ZIF_GIT_REPOSITORY_OPEN_EXT,NULL) \
    PHP_GIT2_FE(git_repository_reinit_filesystem,ZIF_GIT_REPOSITORY_REINIT_FILESYSTEM,NULL) \
    PHP_GIT2_FE(git_repository_set_bare,ZIF_GIT_REPOSITORY_SET_BARE,NULL) \
    PHP_GIT2_FE(git_repository_set_head_detached_from_annotated,ZIF_GIT_REPOSITORY_SET_HEAD_DETACHED_FROM_ANNOTATED,NULL) \
    PHP_GIT2_FE(git_repository_set_ident,ZIF_GIT_REPOSITORY_SET_IDENT,NULL) \
    PHP_GIT2_FE(git_repository_set_namespace,ZIF_GIT_REPOSITORY_SET_NAMESPACE,NULL) \
    PHP_GIT2_FE(git_repository_set_workdir,ZIF_GIT_REPOSITORY_SET_WORKDIR,NULL) \
    PHP_GIT2_FE(git_repository_state,ZIF_GIT_REPOSITORY_STATE,NULL)     \
    PHP_GIT2_FE(git_repository_state_cleanup,ZIF_GIT_REPOSITORY_STATE_CLEANUP,NULL) \
    PHP_GIT2_FE(git_repository_workdir,ZIF_GIT_REPOSITORY_WORKDIR,NULL) \
    PHP_GIT2_FE(git_repository_wrap_odb,ZIF_GIT_REPOSITORY_WRAP_ODB,NULL) \
    PHP_GIT2_FE(git_repository_config,ZIF_GIT_REPOSITORY_CONFIG,NULL)   \
    PHP_GIT2_FE(git_repository_config_snapshot,ZIF_GIT_REPOSITORY_CONFIG_SNAPSHOT,NULL) \
    PHP_GIT2_FE(git_repository_set_config,ZIF_GIT_REPOSITORY_SET_CONFIG,NULL) \
    PHP_GIT2_FE(git_repository_index,ZIF_GIT_REPOSITORY_INDEX,NULL)     \
    PHP_GIT2_FE(git_repository_set_index,ZIF_GIT_REPOSITORY_SET_INDEX,NULL) \
    PHP_GIT2_FE(git_repository_refdb,ZIF_GIT_REPOSITORY_REFDB,NULL)     \
    PHP_GIT2_FE(git_repository_set_refdb,ZIF_GIT_REPOSITORY_SET_REFDB,NULL) \
    PHP_GIT2_FE(git_repository_fetchhead_foreach,ZIF_GIT_REPOSITORY_FETCHHEAD_FOREACH,NULL) \
    PHP_GIT2_FE(git_repository_mergehead_foreach,ZIF_GIT_REPOSITORY_MERGEHEAD_FOREACH,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
