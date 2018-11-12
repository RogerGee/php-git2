/*
 * rebase.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_REBASE_H
#define PHPGIT2_REBASE_H
#include "merge.h"
#include "checkout.h"

namespace php_git2
{

    // Explicitly specialize git2_resource destructor for git_rebase.
    template<> php_git_rebase::~git2_resource()
    {
        git_rebase_free(handle);
    }

    class php_git_rebase_options:
        public php_value_base
    {
    public:
        ZTS_CONSTRUCTOR(php_git_rebase_options)

        const git_rebase_options* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            if (Z_TYPE_P(value) == IS_ARRAY) {
                array_wrapper arr(value);

                git_rebase_init_options(&opts,GIT_REBASE_OPTIONS_VERSION);

                GIT2_ARRAY_LOOKUP_LONG(arr,version,opts);
                GIT2_ARRAY_LOOKUP_BOOL(arr,quiet,opts);
                GIT2_ARRAY_LOOKUP_BOOL(arr,inmemory,opts);
                GIT2_ARRAY_LOOKUP_STRING_NULLABLE(arr,rewrite_notes_ref,opts);
                GIT2_ARRAY_LOOKUP_SUBOBJECT_DEREFERENCE(arr,mergeOpts,merge_options,opts);
                GIT2_ARRAY_LOOKUP_SUBOBJECT_DEREFERENCE(arr,checkoutOpts,checkout_options,opts);

                return &opts;
            }

            return nullptr;
        }

    private:
        git_rebase_options opts;
        php_git_merge_options mergeOpts;
        php_git_checkout_options checkoutOpts;
    };

    class php_git_rebase_operation
    {
    public:
        ZTS_CONSTRUCTOR(php_git_rebase_operation)

        git_rebase_operation** byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            return &operation;
        }

        void ret(zval* return_value) const
        {
            convert_rebase_operation(return_value,operation);
        }

    private:
        git_rebase_operation* operation;
    };

    class php_git_rebase_operation_rethandler
    {
    public:
        template<typename... Ts>
        bool ret(git_rebase_operation* retval,zval* return_value,local_pack<Ts...>& pack)
        {
            php_git_rebase_operation oper;
            *oper.byval_git2() = retval;

            oper.ret(return_value);

            return true;

            UNUSED(pack);
        }
    };

    class php_git_rebase_commit_rethandler
    {
    public:
        template<typename... Ts>
        bool ret(int retval,zval* return_value,local_pack<Ts...>& pack)
        {
            if (retval == 0 || retval == GIT_EUNMERGED || retval == GIT_EAPPLIED) {
                RETVAL_LONG(retval);
                return true;
            }

            return false;
        }
    };

} // php_git2

static constexpr auto ZIF_GIT_REBASE_ABORT = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_rebase*>::func<git_rebase_abort>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_rebase>
        >,
    php_git2::php_boolean_notfound_rethandler
    >;

static constexpr auto ZIF_GIT_REBASE_COMMIT = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_rebase*,
        const git_signature*,
        const git_signature*,
        const char*,
        const char*>::func<git_rebase_commit>,
    php_git2::local_pack<
        php_git2::php_git_oid_out,
        php_git2::php_resource<php_git2::php_git_rebase>,
        php_git2::php_resource_null<php_git2::php_git_signature>,
        php_git2::php_resource<php_git2::php_git_signature>,
        php_git2::php_nullable_string,
        php_git2::php_nullable_string
        >,
    php_git2::php_git_rebase_commit_rethandler
    >;
ZEND_BEGIN_ARG_INFO_EX(git_rebase_commit_arginfo,0,0,6)
    ZEND_ARG_PASS_INFO(1)
ZEND_END_ARG_INFO()

static constexpr auto ZIF_GIT_REBASE_FINISH = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_rebase*,
        const git_signature*>::func<git_rebase_finish>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_rebase>,
        php_git2::php_resource_null<php_git2::php_git_signature>
        >
    >;

static constexpr auto ZIF_GIT_REBASE_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_rebase>
        >
    >;

static constexpr auto ZIF_GIT_REBASE_INIT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_rebase**,
        git_repository*,
        const git_annotated_commit*,
        const git_annotated_commit*,
        const git_annotated_commit*,
        const git_rebase_options*>::func<git_rebase_init>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_rebase>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_resource_null<php_git2::php_git_annotated_commit>,
        php_git2::php_resource_null<php_git2::php_git_annotated_commit>,
        php_git2::php_resource_null<php_git2::php_git_annotated_commit>,
        php_git2::php_git_rebase_options
        >,
    1,
    php_git2::sequence<1,2,3,4,5>,
    php_git2::sequence<0,1,2,3,4,5>,
    php_git2::sequence<0,0,1,2,3,4>
    >;

static constexpr auto ZIF_GIT_REBASE_INMEMORY_INDEX = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_index**,
        git_rebase*>::func<git_rebase_inmemory_index>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_index>,
        php_git2::php_resource<php_git2::php_git_rebase>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

static constexpr auto ZIF_GIT_REBASE_NEXT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_rebase_operation**,
        git_rebase*>::func<git_rebase_next>,
    php_git2::local_pack<
        php_git2::php_git_rebase_operation,
        php_git2::php_resource<php_git2::php_git_rebase>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

static constexpr auto ZIF_GIT_REBASE_OPEN = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_rebase**,
        git_repository*,
        const git_rebase_options*>::func<git_rebase_open>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_rebase>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_git_rebase_options
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
    >;

static constexpr auto ZIF_GIT_REBASE_OPERATION_BYINDEX = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_rebase_operation*,
        git_rebase*,
        size_t>::func<git_rebase_operation_byindex>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_rebase>,
        php_git2::php_long_cast<size_t>
        >,
    php_git2::php_git_rebase_operation_rethandler
    >;

static constexpr auto ZIF_GIT_REBASE_OPERATION_CURRENT = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        git_rebase*>::func<git_rebase_operation_current>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_rebase>
        >,
    0
    >;

static constexpr auto ZIF_GIT_REBASE_OPERATION_ENTRYCOUNT = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        git_rebase*>::func<git_rebase_operation_entrycount>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_rebase>
        >,
    0
    >;

#define GIT_REBASE_FE                                                   \
    PHP_GIT2_FE(git_rebase_abort,ZIF_GIT_REBASE_ABORT,NULL)             \
    PHP_GIT2_FE(git_rebase_commit,ZIF_GIT_REBASE_COMMIT,git_rebase_commit_arginfo) \
    PHP_GIT2_FE(git_rebase_finish,ZIF_GIT_REBASE_FINISH,NULL)           \
    PHP_GIT2_FE(git_rebase_free,ZIF_GIT_REBASE_FREE,NULL)               \
    PHP_GIT2_FE(git_rebase_init,ZIF_GIT_REBASE_INIT,NULL)               \
    PHP_GIT2_FE(git_rebase_inmemory_index,ZIF_GIT_REBASE_INMEMORY_INDEX,NULL) \
    PHP_GIT2_FE(git_rebase_next,ZIF_GIT_REBASE_NEXT,NULL)               \
    PHP_GIT2_FE(git_rebase_open,ZIF_GIT_REBASE_OPEN,NULL)               \
    PHP_GIT2_FE(git_rebase_operation_byindex,ZIF_GIT_REBASE_OPERATION_BYINDEX,NULL) \
    PHP_GIT2_FE(git_rebase_operation_current,ZIF_GIT_REBASE_OPERATION_CURRENT,NULL) \
    PHP_GIT2_FE(git_rebase_operation_entrycount,ZIF_GIT_REBASE_OPERATION_ENTRYCOUNT,NULL)


#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
