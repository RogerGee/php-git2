/*
 * revparse.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_REVPARSE_H
#define PHPGIT2_REVPARSE_H

#include "stubs/revparse_arginfo.h"

namespace php_git2
{
    // Define a type for creating a new refspec and converting it to a PHP array
    // for userspace.

    class php_git2_revspec_ref
    {
    public:
        php_git2_revspec_ref()
        {
            memset(&revspec,0,sizeof(git_revspec));
        }

        git_revspec* byval_git2()
        {
            return &revspec;
        }

        void ret(zval* return_value)
        {
            // Convert the revspec into a PHP array.

            php_resource_ref<php_git_object> fieldFrom;
            php_resource_ref<php_git_object> fieldTo;

            array_init(return_value);

            // Create resources to wrap the git_object values. It is possible
            // that the git_object pointers may be null if the revspec didn't
            // imply a range.

            if (revspec.from != nullptr) {
                zval zfrom;
                *fieldFrom.byval_git2() = revspec.from;
                fieldFrom.ret(&zfrom);
                add_assoc_zval(return_value,"from",&zfrom);
            }
            else {
                add_assoc_null(return_value,"from");
            }

            if (revspec.to != nullptr) {
                zval zto;
                *fieldTo.byval_git2() = revspec.to;
                fieldTo.ret(&zto);
                add_assoc_zval(return_value,"to",&zto);
            }
            else {
                add_assoc_null(return_value,"to");
            }

            add_assoc_long(return_value,"flags",revspec.flags);
        }

    private:
        git_revspec revspec;
    };

} // namespace php_git2

// Functions:

static constexpr auto ZIF_git_revparse = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_revspec*,
        git_repository*,
        const char*>::func<git_revparse>,
    php_git2::local_pack<
        php_git2::php_git2_revspec_ref,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_revparse_ext = zif_php_git2_function_setdeps2<
    php_git2::func_wrapper<
        int,
        git_object**,
        git_reference**,
        git_repository*,
        const char*>::func<git_revparse_ext>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_object>,
        php_git2::php_resource_nullable_ref_out<php_git2::php_git_reference>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    php_git2::sequence<0,2>, // Make the object dependent on the repository.
    php_git2::sequence<1,2>, // Make the reference dependent on the repository.
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_revparse_single = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_object**,
        git_repository*,
        const char*>::func<git_revparse_single>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_object>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    php_git2::sequence<0,1>, // Make the object dependent on the repository.
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

// Function Entries:

#define GIT_REVPARSE_FE                         \
    PHP_GIT2_FE(git_revparse)                   \
    PHP_GIT2_FE(git_revparse_ext)               \
    PHP_GIT2_FE(git_revparse_single)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
