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
#include "stubs/repository_arginfo.h"

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

static constexpr auto ZIF_git_repository_init = zif_php_git2_function<
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

static constexpr auto ZIF_git_repository_new = zif_php_git2_function<
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

static constexpr auto ZIF_git_repository_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_repository> >
    >;

static constexpr auto ZIF_git_repository_open = zif_php_git2_function<
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

static constexpr auto ZIF_git_repository_open_bare = zif_php_git2_function<
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

static constexpr auto ZIF_git_repository_detach_head = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*>::func<git_repository_detach_head>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    0
    >;

static constexpr auto ZIF_git_repository_discover = zif_php_git2_function<
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

static constexpr auto ZIF_git_repository_get_namespace = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        git_repository*>::func<git_repository_get_namespace>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    0 >;

static constexpr auto ZIF_git_repository_path = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_repository*>::func<git_repository_path>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    0
    >;

static constexpr auto ZIF_git_repository_hashfile = zif_php_git2_function<
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

static constexpr auto ZIF_git_repository_odb = zif_php_git2_function_setdeps<
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

static constexpr auto ZIF_git_repository_set_odb = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        git_odb*>::func<git_repository_set_odb>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_odb>
        >
    >;

static constexpr auto ZIF_git_repository_set_head = zif_php_git2_function<
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

static constexpr auto ZIF_git_repository_set_head_detached = zif_php_git2_function_rethandler<
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

static constexpr auto ZIF_git_repository_head = zif_php_git2_function_setdeps<
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

static constexpr auto ZIF_git_repository_is_empty = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_repository*>::func<git_repository_is_empty>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    php_git2::php_boolean_error_rethandler<int>
    >;

static constexpr auto ZIF_git_repository_is_bare = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        const git_repository*>::func<git_repository_is_bare>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_git_repository_is_shallow = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_repository*>::func<git_repository_is_shallow>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    php_git2::php_boolean_rethandler<int>
    >;

static constexpr auto ZIF_git_repository_head_detached = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_repository*>::func<git_repository_head_detached>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    php_git2::php_boolean_error_rethandler<int>
    >;

static constexpr auto ZIF_git_repository_head_unborn = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_repository*>::func<git_repository_head_unborn>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    php_git2::php_boolean_error_rethandler<int>
    >;

static constexpr auto ZIF_git_repository_ident = zif_php_git2_function<
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

static constexpr auto ZIF_git_repository_init_ext = zif_php_git2_function<
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

static constexpr auto ZIF_git_repository_message = zif_php_git2_function_rethandler<
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

static constexpr auto ZIF_git_repository_message_remove = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*>::func<git_repository_message_remove>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >
    >;

static constexpr auto ZIF_git_repository_open_ext = zif_php_git2_function<
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

static constexpr auto ZIF_git_repository_reinit_filesystem = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        int>::func<git_repository_reinit_filesystem>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_bool
        >
    >;

static constexpr auto ZIF_git_repository_set_bare = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*>::func<git_repository_set_bare>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >
    >;

static constexpr auto ZIF_git_repository_set_head_detached_from_annotated = zif_php_git2_function_rethandler<
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

static constexpr auto ZIF_git_repository_set_ident = zif_php_git2_function<
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

static constexpr auto ZIF_git_repository_set_namespace = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*>::func<git_repository_set_namespace>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_git_repository_set_workdir = zif_php_git2_function<
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

static constexpr auto ZIF_git_repository_state = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*>::func<git_repository_state>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    0
    >;

static constexpr auto ZIF_git_repository_state_cleanup = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*>::func<git_repository_state_cleanup>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >
    >;

static constexpr auto ZIF_git_repository_workdir = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_repository*>::func<git_repository_workdir>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    0
    >;

static constexpr auto ZIF_git_repository_wrap_odb = zif_php_git2_function<
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

static constexpr auto ZIF_git_repository_config = zif_php_git2_function_setdeps<
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

static constexpr auto ZIF_git_repository_config_snapshot = zif_php_git2_function_setdeps<
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

static constexpr auto ZIF_git_repository_set_config = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        git_config*>::func<git_repository_set_config>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_config>
        >
    >;

static constexpr auto ZIF_git_repository_index = zif_php_git2_function<
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

static constexpr auto ZIF_git_repository_set_index = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        git_index*>::func<git_repository_set_index>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_index>
        >
    >;

static constexpr auto ZIF_git_repository_refdb = zif_php_git2_function_setdeps<
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

static constexpr auto ZIF_git_repository_set_refdb = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        git_refdb*>::func<git_repository_set_refdb>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_refdb>
        >
    >;

static constexpr auto ZIF_git_repository_fetchhead_foreach = zif_php_git2_function<
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

static constexpr auto ZIF_git_repository_mergehead_foreach = zif_php_git2_function<
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

#define GIT_REPOSITORY_FE                                           \
    PHP_GIT2_FE(git_repository_init)                                \
    PHP_GIT2_FE(git_repository_new)                                 \
    PHP_GIT2_FE(git_repository_free)                                \
    PHP_GIT2_FE(git_repository_open)                                \
    PHP_GIT2_FE(git_repository_open_bare)                           \
    PHP_GIT2_FE(git_repository_detach_head)                         \
    PHP_GIT2_FE(git_repository_discover)                            \
    PHP_GIT2_FE(git_repository_get_namespace)                       \
    PHP_GIT2_FE(git_repository_path)                                \
    PHP_GIT2_FE(git_repository_hashfile)                            \
    PHP_GIT2_FE(git_repository_odb)                                 \
    PHP_GIT2_FE(git_repository_set_odb)                             \
    PHP_GIT2_FE(git_repository_set_head)                            \
    PHP_GIT2_FE(git_repository_set_head_detached)                   \
    PHP_GIT2_FE(git_repository_head)                                \
    PHP_GIT2_FE(git_repository_is_empty)                            \
    PHP_GIT2_FE(git_repository_is_bare)                             \
    PHP_GIT2_FE(git_repository_is_shallow)                          \
    PHP_GIT2_FE(git_repository_head_detached)                       \
    PHP_GIT2_FE(git_repository_head_unborn)                         \
    PHP_GIT2_FE(git_repository_ident)                               \
    PHP_GIT2_FE(git_repository_init_ext)                            \
    PHP_GIT2_FE(git_repository_message)                             \
    PHP_GIT2_FE(git_repository_message_remove)                      \
    PHP_GIT2_FE(git_repository_open_ext)                            \
    PHP_GIT2_FE(git_repository_reinit_filesystem)                   \
    PHP_GIT2_FE(git_repository_set_bare)                            \
    PHP_GIT2_FE(git_repository_set_head_detached_from_annotated)    \
    PHP_GIT2_FE(git_repository_set_ident)                           \
    PHP_GIT2_FE(git_repository_set_namespace)                       \
    PHP_GIT2_FE(git_repository_set_workdir)                         \
    PHP_GIT2_FE(git_repository_state)                               \
    PHP_GIT2_FE(git_repository_state_cleanup)                       \
    PHP_GIT2_FE(git_repository_workdir)                             \
    PHP_GIT2_FE(git_repository_wrap_odb)                            \
    PHP_GIT2_FE(git_repository_config)                              \
    PHP_GIT2_FE(git_repository_config_snapshot)                     \
    PHP_GIT2_FE(git_repository_set_config)                          \
    PHP_GIT2_FE(git_repository_index)                               \
    PHP_GIT2_FE(git_repository_set_index)                           \
    PHP_GIT2_FE(git_repository_refdb)                               \
    PHP_GIT2_FE(git_repository_set_refdb)                           \
    PHP_GIT2_FE(git_repository_fetchhead_foreach)                   \
    PHP_GIT2_FE(git_repository_mergehead_foreach)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
