/*
 * rebase.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_REBASE_H
#define PHPGIT2_REBASE_H

#include "merge.h"
#include "checkout.h"
#include "stubs/rebase_arginfo.h"

namespace php_git2
{
    // Explicitly specialize git2_resource destructor for git_rebase.
    template<> php_git_rebase::~git2_resource()
    {
        git_rebase_free(handle);
    }

    class php_git_rebase_options:
        public php_option_array
    {
    public:
        const git_rebase_options* byval_git2()
        {
            if (!is_null()) {
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
        git_rebase_operation** byval_git2()
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

static constexpr auto ZIF_git_rebase_abort = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        git_rebase*>::func<git_rebase_abort>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_rebase>
        >,
    php_git2::php_boolean_notfound_rethandler
    >;

static constexpr auto ZIF_git_rebase_commit = zif_php_git2_function_rethandler<
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
        php_git2::php_resource_nullable<php_git2::php_git_signature>,
        php_git2::php_resource<php_git2::php_git_signature>,
        php_git2::php_string_nullable,
        php_git2::php_string_nullable
        >,
    php_git2::php_git_rebase_commit_rethandler
    >;

static constexpr auto ZIF_git_rebase_finish = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_rebase*,
        const git_signature*>::func<git_rebase_finish>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_rebase>,
        php_git2::php_resource_nullable<php_git2::php_git_signature>
        >
    >;

static constexpr auto ZIF_git_rebase_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_rebase>
        >
    >;

static constexpr auto ZIF_git_rebase_init = zif_php_git2_function<
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
        php_git2::php_resource_nullable<php_git2::php_git_annotated_commit>,
        php_git2::php_resource_nullable<php_git2::php_git_annotated_commit>,
        php_git2::php_resource_nullable<php_git2::php_git_annotated_commit>,
        php_git2::php_git_rebase_options
        >,
    1,
    php_git2::sequence<1,2,3,4,5>,
    php_git2::sequence<0,1,2,3,4,5>
    >;

static constexpr auto ZIF_git_rebase_inmemory_index = zif_php_git2_function<
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
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_rebase_next = zif_php_git2_function<
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
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_git_rebase_open = zif_php_git2_function<
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
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_rebase_operation_byindex = zif_php_git2_function_rethandler<
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

static constexpr auto ZIF_git_rebase_operation_current = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        git_rebase*>::func<git_rebase_operation_current>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_rebase>
        >,
    0
    >;

static constexpr auto ZIF_git_rebase_operation_entrycount = zif_php_git2_function<
    php_git2::func_wrapper<
        size_t,
        git_rebase*>::func<git_rebase_operation_entrycount>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_rebase>
        >,
    0
    >;

#define GIT_REBASE_FE                               \
    PHP_GIT2_FE(git_rebase_abort)                   \
    PHP_GIT2_FE(git_rebase_commit)                  \
    PHP_GIT2_FE(git_rebase_finish)                  \
    PHP_GIT2_FE(git_rebase_free)                    \
    PHP_GIT2_FE(git_rebase_init)                    \
    PHP_GIT2_FE(git_rebase_inmemory_index)          \
    PHP_GIT2_FE(git_rebase_next)                    \
    PHP_GIT2_FE(git_rebase_open)                    \
    PHP_GIT2_FE(git_rebase_operation_byindex)       \
    PHP_GIT2_FE(git_rebase_operation_current)       \
    PHP_GIT2_FE(git_rebase_operation_entrycount)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
