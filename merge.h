/*
 * merge.h
 *
 * This file is a part of php-git2.
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
        public php_value_base
    {
    public:
        php_git_merge_options(TSRMLS_D)
        {
            git_merge_init_options(&opts,GIT_MERGE_OPTIONS_VERSION);
        }

        git_merge_options* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            if (value != nullptr && Z_TYPE_P(value) == IS_ARRAY) {
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
        php_strarray_byval_array strarray;
    };

    // Provide type for git_merge_file_options.

    class php_git_merge_file_options:
        public php_value_base
    {
    public:
        php_git_merge_file_options(TSRMLS_D)
        {
            git_merge_file_init_options(&opts,GIT_MERGE_FILE_OPTIONS_VERSION);
        }

        const git_merge_file_options* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            if (value != nullptr && Z_TYPE_P(value) == IS_ARRAY) {
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
        public php_value_base
    {
    public:
        php_git_merge_file_input(TSRMLS_D)
        {
            git_merge_file_init_input(&input,GIT_MERGE_FILE_INPUT_VERSION);
        }

        const git_merge_file_input* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            if (value != nullptr && Z_TYPE_P(value) == IS_ARRAY) {
                array_wrapper arr(value);

                GIT2_ARRAY_LOOKUP_STRING_WITH_LENGTH(arr,ptr,size,input);
                GIT2_ARRAY_LOOKUP_STRING_NULLABLE(arr,path,input);
                GIT2_ARRAY_LOOKUP_LONG(arr,mode,input);

                return &input;
            }

            return nullptr;
        }
    private:
        git_merge_file_input input;
    };

    // Provide type for git_merge_file_result conversion to PHP array.

    class php_git_merge_file_result_ref
    {
    public:
        ZTS_CONSTRUCTOR(php_git_merge_file_result_ref)

        ~php_git_merge_file_result_ref()
        {
            git_merge_file_result_free(&result);
        }

        git_merge_file_result* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
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

static constexpr auto ZIF_GIT_MERGE = zif_php_git2_function<
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
    php_git2::sequence<0,2,1,3,4>,
    php_git2::sequence<0,1,0,2,3>
    >;

static constexpr auto ZIF_GIT_MERGE_ANALYSIS = zif_php_git2_function<
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
    php_git2::sequence<0,1,2,4,3>,
    php_git2::sequence<0,0,1,2,0>
    >;
ZEND_BEGIN_ARG_INFO_EX(git_merge_analysis_arginfo,0,0,3)
    ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

static constexpr auto ZIF_GIT_MERGE_BASE = zif_php_git2_function<
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
    php_git2::sequence<0,1,2,3>,
    php_git2::sequence<0,0,1,2>
    >;

static constexpr auto ZIF_GIT_MERGE_BASE_MANY = zif_php_git2_function<
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
                php_git2::php_oid_byval_array
                >
            >,
        php_git2::php_oid_byval_array
        >,
    1,
    php_git2::sequence<1,3>,
    php_git2::sequence<0,1,2,3>,
    php_git2::sequence<0,0,0,1>
    >;

static constexpr auto ZIF_GIT_MERGE_BASE_OCTOPUS = zif_php_git2_function<
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
                php_git2::php_oid_byval_array
                >
            >,
        php_git2::php_oid_byval_array
        >,
    1,
    php_git2::sequence<1,3>,
    php_git2::sequence<0,1,2,3>,
    php_git2::sequence<0,0,0,1>
    >;

static constexpr auto ZIF_GIT_MERGE_BASES = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oidarray*,
        git_repository*,
        const git_oid*,
        const git_oid*>::func<git_merge_bases>,
    php_git2::local_pack<
        php_git2::php_oidarray,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_git_oid_fromstr,
        php_git2::php_git_oid_fromstr
        >,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>,
    php_git2::sequence<0,0,1,2>
    >;

static constexpr auto ZIF_GIT_MERGE_BASES_MANY = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oidarray*,
        git_repository*,
        size_t,
        const git_oid[]>::func<git_merge_bases_many>,
    php_git2::local_pack<
        php_git2::php_oidarray,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::connector_wrapper<
            php_git2::php_array_length_connector<
                size_t,
                php_git2::php_oid_byval_array
                >
            >,
        php_git2::php_oid_byval_array
        >,
    1,
    php_git2::sequence<1,3>,
    php_git2::sequence<0,1,2,3>,
    php_git2::sequence<0,0,0,1>
    >;

static constexpr auto ZIF_GIT_MERGE_COMMITS = zif_php_git2_function<
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
    php_git2::sequence<0,1,2,3,4>,
    php_git2::sequence<0,0,1,2,3>
    >;

/*static constexpr auto ZIF_GIT_MERGE_DRIVER_LOOKUP = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_merge_driver_lookup>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_MERGE_DRIVER_REGISTER = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_merge_driver_register>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_MERGE_DRIVER_SOURCE_ANCESTOR = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_merge_driver_source_ancestor>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_MERGE_DRIVER_SOURCE_FILE_OPTIONS = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_merge_driver_source_file_options>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_MERGE_DRIVER_SOURCE_OURS = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_merge_driver_source_ours>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_MERGE_DRIVER_SOURCE_REPO = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_merge_driver_source_repo>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_MERGE_DRIVER_SOURCE_THEIRS = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_merge_driver_source_theirs>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;

static constexpr auto ZIF_GIT_MERGE_DRIVER_UNREGISTER = zif_php_git2_function<
    php_git2::func_wrapper<

        >::func<git_merge_driver_unregister>,
    php_git2::local_pack<

        >,
    -1,
    php_git2::sequence<>,
    php_git2::sequence<>,
    php_git2::sequence<>
    >;
*/
static constexpr auto ZIF_GIT_MERGE_FILE = zif_php_git2_function<
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
    php_git2::sequence<0,1,2,3,4>,
    php_git2::sequence<0,0,1,2,3>
    >;

static constexpr auto ZIF_GIT_MERGE_FILE_FROM_INDEX = zif_php_git2_function<
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
        php_git2::php_git_index_entry,
        php_git2::php_git_index_entry,
        php_git2::php_git_index_entry,
        php_git2::php_git_merge_file_options
        >,
    1,
    php_git2::sequence<1,2,3,4,5>,
    php_git2::sequence<0,1,2,3,4,5>,
    php_git2::sequence<0,0,1,2,3,4>
    >;

static constexpr auto ZIF_GIT_MERGE_TREES = zif_php_git2_function<
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
        php_git2::php_resource_null<php_git2::php_git_tree>,
        php_git2::php_resource<php_git2::php_git_tree>,
        php_git2::php_resource<php_git2::php_git_tree>,
        php_git2::php_git_merge_options
        >,
    1,
    php_git2::sequence<1,2,3,4,5>,
    php_git2::sequence<0,1,2,3,4,5>,
    php_git2::sequence<0,0,1,2,3,4>
    >;

#define GIT_MERGE_FE                                                    \
    PHP_GIT2_FE(git_merge,ZIF_GIT_MERGE,NULL)                           \
    PHP_GIT2_FE(git_merge_analysis,ZIF_GIT_MERGE_ANALYSIS,git_merge_analysis_arginfo) \
    PHP_GIT2_FE(git_merge_base,ZIF_GIT_MERGE_BASE,NULL)                 \
    PHP_GIT2_FE(git_merge_base_many,ZIF_GIT_MERGE_BASE_MANY,NULL)       \
    PHP_GIT2_FE(git_merge_base_octopus,ZIF_GIT_MERGE_BASE_OCTOPUS,NULL) \
    PHP_GIT2_FE(git_merge_bases,ZIF_GIT_MERGE_BASES,NULL)               \
    PHP_GIT2_FE(git_merge_bases_many,ZIF_GIT_MERGE_BASES_MANY,NULL)     \
    PHP_GIT2_FE(git_merge_commits,ZIF_GIT_MERGE_COMMITS,NULL)           \
    PHP_GIT2_UNIMPLEMENTED(git_merge_driver_lookup,ZIF_GIT_MERGE_DRIVER_LOOKUP,NULL) \
    PHP_GIT2_UNIMPLEMENTED(git_merge_driver_register,ZIF_GIT_MERGE_DRIVER_REGISTER,NULL) \
    PHP_GIT2_UNIMPLEMENTED(git_merge_driver_source_ancestor,ZIF_GIT_MERGE_DRIVER_SOURCE_ANCESTOR,NULL) \
    PHP_GIT2_UNIMPLEMENTED(git_merge_driver_source_file_options,ZIF_GIT_MERGE_DRIVER_SOURCE_FILE_OPTIONS,NULL) \
    PHP_GIT2_UNIMPLEMENTED(git_merge_driver_source_ours,ZIF_GIT_MERGE_DRIVER_SOURCE_OURS,NULL) \
    PHP_GIT2_UNIMPLEMENTED(git_merge_driver_source_repo,ZIF_GIT_MERGE_DRIVER_SOURCE_REPO,NULL) \
    PHP_GIT2_UNIMPLEMENTED(git_merge_driver_source_theirs,ZIF_GIT_MERGE_DRIVER_SOURCE_THEIRS,NULL) \
    PHP_GIT2_UNIMPLEMENTED(git_merge_driver_unregister,ZIF_GIT_MERGE_DRIVER_UNREGISTER,NULL) \
    PHP_GIT2_FE(git_merge_file,ZIF_GIT_MERGE_FILE,NULL)                 \
    PHP_GIT2_FE(git_merge_file_from_index,ZIF_GIT_MERGE_FILE_FROM_INDEX,NULL) \
    PHP_GIT2_FE(git_merge_trees,ZIF_GIT_MERGE_TREES,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
