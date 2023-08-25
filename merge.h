/*
 * merge.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_MERGE_H
#define PHPGIT2_MERGE_H
#include "checkout.h"
#include "index.h"

namespace php_git2
{
    // Provide a type for converting a PHP array into a git_merge_options
    // struct.

    class php_git_merge_options:
        public php_option_array
    {
    public:
        php_git_merge_options()
        {
            git_merge_init_options(&opts,GIT_MERGE_OPTIONS_VERSION);
        }

        git_merge_options* byval_git2()
        {
            if (!is_null()) {
                array_wrapper arr(value);

                GIT2_ARRAY_LOOKUP_LONG(arr,flags,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,rename_threshold,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,target_limit,opts);
                // TODO Handle 'metric' member.
                GIT2_ARRAY_LOOKUP_LONG(arr,recursion_limit,opts);
                GIT2_ARRAY_LOOKUP_STRING(arr,default_driver,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,file_favor,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,file_flags,opts);

                return &opts;
            }

            return nullptr;
        }

    private:
        git_merge_options opts;
        php_git_strarray_byval_array strarray;
    };

    // Provide type for git_merge_file_options.

    class php_git_merge_file_options:
        public php_option_array
    {
    public:
        php_git_merge_file_options()
        {
            git_merge_file_init_options(&opts,GIT_MERGE_FILE_OPTIONS_VERSION);
        }

        const git_merge_file_options* byval_git2()
        {
            if (!is_null()) {
                array_wrapper arr(value);

                GIT2_ARRAY_LOOKUP_STRING(arr,ancestor_label,opts);
                GIT2_ARRAY_LOOKUP_STRING(arr,our_label,opts);
                GIT2_ARRAY_LOOKUP_STRING(arr,their_label,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,favor,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,flags,opts);

                return &opts;
            }

            return nullptr;
        }

    private:
        git_merge_file_options opts;
    };

    // Provide type for git_merge_file_input.

    class php_git_merge_file_input:
        public php_array_base
    {
    public:
        php_git_merge_file_input()
        {
            git_merge_file_init_input(&input,GIT_MERGE_FILE_INPUT_VERSION);
        }

        const git_merge_file_input* byval_git2()
        {
            array_wrapper arr(value);

            GIT2_ARRAY_LOOKUP_STRING_WITH_LENGTH(arr,ptr,size,input);
            GIT2_ARRAY_LOOKUP_STRING_NULLABLE(arr,path,input);
            GIT2_ARRAY_LOOKUP_LONG(arr,mode,input);

            return &input;
        }

    private:
        git_merge_file_input input;
    };

    // Provide type for git_merge_file_result conversion to PHP array.

    class php_git_merge_file_result_ref
    {
    public:
        ~php_git_merge_file_result_ref()
        {
            git_merge_file_result_free(&result);
        }

        git_merge_file_result* byval_git2()
        {
            return &result;
        }

        void ret(zval* return_value)
        {
            convert_merge_file_result(return_value,&result);
        }

    private:
        git_merge_file_result result;
    };

    // Define array types.

    using php_git_merge_annot_array = php_array<
        php_resource<php_git_annotated_commit>,
        const git_annotated_commit*
        >;

    using php_git_merge_annot_array_length = connector_wrapper<
        php_array_length_connector<
            size_t,
            php_git2::php_git_merge_annot_array
            >
        >;

} // namespace php_git2

static constexpr auto ZIF_git_merge = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const git_annotated_commit**,
        size_t,
        const git_merge_options*,
        const git_checkout_options*>::func<git_merge>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_git_merge_annot_array_length,
        php_git2::php_git_merge_annot_array,
        php_git2::php_git_merge_options,
        php_git2::php_git_checkout_options
        >,
    -1,
    php_git2::sequence<0,2,3,4>,
    php_git2::sequence<0,2,1,3,4>
    >;

static constexpr auto ZIF_git_merge_analysis = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_merge_analysis_t*,
        git_merge_preference_t*,
        git_repository*,
        const git_annotated_commit**,
        size_t>::func<git_merge_analysis>,
    php_git2::local_pack<
        php_git2::php_long_ref<git_merge_analysis_t>,
        php_git2::php_long_out<git_merge_preference_t>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_git_merge_annot_array_length,
        php_git2::php_git_merge_annot_array
        >,
    1,
    php_git2::sequence<1,2,4>,
    php_git2::sequence<0,1,2,4,3>
    >;

static constexpr auto ZIF_git_merge_base = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_repository*,
        const git_oid*,
        const git_oid*>::func<git_merge_base>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_git_oid_fromstr,
        php_git2::php_git_oid_fromstr
        >,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_merge_base_many = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_repository*,
        size_t,
        const git_oid[]>::func<git_merge_base_many>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::connector_wrapper<
            php_git2::php_array_length_connector<
                size_t,
                php_git2::php_git_oid_byval_array
                >
            >,
        php_git2::php_git_oid_byval_array
        >,
    1,
    php_git2::sequence<1,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_merge_base_octopus = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_repository*,
        size_t,
        const git_oid[]>::func<git_merge_base_octopus>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::connector_wrapper<
            php_git2::php_array_length_connector<
                size_t,
                php_git2::php_git_oid_byval_array
                >
            >,
        php_git2::php_git_oid_byval_array
        >,
    1,
    php_git2::sequence<1,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_merge_bases = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oidarray*,
        git_repository*,
        const git_oid*,
        const git_oid*>::func<git_merge_bases>,
    php_git2::local_pack<
        php_git2::php_git_oidarray,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_git_oid_fromstr,
        php_git2::php_git_oid_fromstr
        >,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_merge_bases_many = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oidarray*,
        git_repository*,
        size_t,
        const git_oid[]>::func<git_merge_bases_many>,
    php_git2::local_pack<
        php_git2::php_git_oidarray,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::connector_wrapper<
            php_git2::php_array_length_connector<
                size_t,
                php_git2::php_git_oid_byval_array
                >
            >,
        php_git2::php_git_oid_byval_array
        >,
    1,
    php_git2::sequence<1,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_merge_commits = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index**,
        git_repository*,
        const git_commit*,
        const git_commit*,
        const git_merge_options*>::func<git_merge_commits>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_index>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource<php_git2::php_git_commit>,
        php_git2::php_resource<php_git2::php_git_commit>,
        php_git2::php_git_merge_options
        >,
    1,
    php_git2::sequence<1,2,3,4>,
    php_git2::sequence<0,1,2,3,4>
    >;

/*static constexpr auto ZIF_git_merge_driver_lookup = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_merge_driver_lookup>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_git_merge_driver_register = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_merge_driver_register>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_git_merge_driver_source_ancestor = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_merge_driver_source_ancestor>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_git_merge_driver_source_file_options = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_merge_driver_source_file_options>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_git_merge_driver_source_ours = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_merge_driver_source_ours>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_git_merge_driver_source_repo = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_merge_driver_source_repo>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_git_merge_driver_source_theirs = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_merge_driver_source_theirs>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_git_merge_driver_unregister = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_merge_driver_unregister>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;
*/
static constexpr auto ZIF_git_merge_file = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_merge_file_result*,
        const git_merge_file_input*,
        const git_merge_file_input*,
        const git_merge_file_input*,
        const git_merge_file_options*>::func<git_merge_file>,
    php_git2::local_pack<
        php_git2::php_git_merge_file_result_ref,
        php_git2::php_git_merge_file_input,
        php_git2::php_git_merge_file_input,
        php_git2::php_git_merge_file_input,
        php_git2::php_git_merge_file_options
        >,
    1,
    php_git2::sequence<1,2,3,4>,
    php_git2::sequence<0,1,2,3,4>
    >;

static constexpr auto ZIF_git_merge_file_from_index = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_merge_file_result*,
        git_repository*,
        const git_index_entry*,
        const git_index_entry*,
        const git_index_entry*,
        const git_merge_file_options*>::func<git_merge_file_from_index>,
    php_git2::local_pack<
        php_git2::php_git_merge_file_result_ref,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_git_index_entry_nullable,
        php_git2::php_git_index_entry,
        php_git2::php_git_index_entry,
        php_git2::php_git_merge_file_options
        >,
    1,
    php_git2::sequence<1,2,3,4,5>,
    php_git2::sequence<0,1,2,3,4,5>
    >;

static constexpr auto ZIF_git_merge_trees = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index**,
        git_repository*,
        const git_tree*,
        const git_tree*,
        const git_tree*,
        const git_merge_options*>::func<git_merge_trees>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_index>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource_nullable<php_git2::php_git_tree>,
        php_git2::php_resource<php_git2::php_git_tree>,
        php_git2::php_resource<php_git2::php_git_tree>,
        php_git2::php_git_merge_options
        >,
    1,
    php_git2::sequence<1,2,3,4,5>,
    php_git2::sequence<0,1,2,3,4,5>
    >;

#define GIT_MERGE_FE                                                \
    PHP_GIT2_FE(git_merge)                                          \
    PHP_GIT2_FE(git_merge_analysis)                                 \
    PHP_GIT2_FE(git_merge_base)                                     \
    PHP_GIT2_FE(git_merge_base_many)                                \
    PHP_GIT2_FE(git_merge_base_octopus)                             \
    PHP_GIT2_FE(git_merge_bases)                                    \
    PHP_GIT2_FE(git_merge_bases_many)                               \
    PHP_GIT2_FE(git_merge_commits)                                  \
    PHP_GIT2_UNIMPLEMENTED(git_merge_driver_lookup)                 \
    PHP_GIT2_UNIMPLEMENTED(git_merge_driver_register)               \
    PHP_GIT2_UNIMPLEMENTED(git_merge_driver_source_ancestor)        \
    PHP_GIT2_UNIMPLEMENTED(git_merge_driver_source_file_options)    \
    PHP_GIT2_UNIMPLEMENTED(git_merge_driver_source_ours)            \
    PHP_GIT2_UNIMPLEMENTED(git_merge_driver_source_repo)            \
    PHP_GIT2_UNIMPLEMENTED(git_merge_driver_source_theirs)          \
    PHP_GIT2_UNIMPLEMENTED(git_merge_driver_unregister)             \
    PHP_GIT2_FE(git_merge_file)                                     \
    PHP_GIT2_FE(git_merge_file_from_index)                          \
    PHP_GIT2_FE(git_merge_trees)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
