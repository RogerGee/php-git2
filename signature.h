/*
 * signature.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_SIGNATURE_H
#define PHPGIT2_SIGNATURE_H

namespace php_git2
{

    // Explicitly specialize git2_resource destructor for git_signature.
    template<> php_git_signature::~git2_resource()
    {
        git_signature_free(handle);
    }

} // php_git2

// Functions:

static constexpr auto ZIF_GIT_SIGNATURE_NEW = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_signature**,
        const char*,
        const char*,
        git_time_t,
        int>::func<git_signature_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_signature>,
        php_git2::php_string,
        php_git2::php_string,
        php_git2::php_long,
        php_git2::php_long
        >,
    1,
    php_git2::sequence<1,2,3,4>,
    php_git2::sequence<0,1,2,3,4>,
    php_git2::sequence<0,0,1,2,3>
    >;

static constexpr auto ZIF_GIT_SIGNATURE_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_signature>
        >
    >;

static constexpr auto ZIF_GIT_SIGNATURE_DUP = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_signature**,
        const git_signature*>::func<git_signature_dup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_signature>,
        php_git2::php_resource<php_git2::php_git_signature>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

static constexpr auto ZIF_GIT_SIGNATURE_DEFAULT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_signature**,
        git_repository*>::func<git_signature_default>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_signature>,
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

static constexpr auto ZIF_GIT_SIGNATURE_NOW = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_signature**,
        const char*,
        const char*>::func<git_signature_now>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_signature>,
        php_git2::php_string,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
    >;

static constexpr auto ZIF_GIT_SIGNATURE_FROM_BUFFER = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_signature**,
        const char*>::func<git_signature_from_buffer>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_signature>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

static PHP_FUNCTION(git2_signature_convert)
{
    git_signature* handle;
    php_git2::php_resource<php_git2::php_git_signature> signature;

    if (zend_parse_parameters(ZEND_NUM_ARGS(),"z",signature.byref_php()) == FAILURE) {
        return;
    }

    try {
        handle = signature.byval_git2();
    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }
        return;
    }

    php_git2::convert_signature(return_value,handle);
}

// Function Entries:

#define GIT_SIGNATURE_FE                                                \
    PHP_GIT2_FE(git_signature_new,ZIF_GIT_SIGNATURE_NEW,NULL)           \
    PHP_GIT2_FE(git_signature_free,ZIF_GIT_SIGNATURE_FREE,NULL)         \
    PHP_GIT2_FE(git_signature_dup,ZIF_GIT_SIGNATURE_DUP,NULL)           \
    PHP_GIT2_FE(git_signature_default,ZIF_GIT_SIGNATURE_DEFAULT,NULL)   \
    PHP_GIT2_FE(git_signature_now,ZIF_GIT_SIGNATURE_NOW,NULL)           \
    PHP_GIT2_FE(git_signature_from_buffer,ZIF_GIT_SIGNATURE_FROM_BUFFER,NULL) \
    PHP_FE(git2_signature_convert,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
