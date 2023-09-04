/*
 * note.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_NOTE_H
#define PHPGIT2_NOTE_H

#include "stubs/note_arginfo.h"

namespace php_git2
{

    // Explicitly specialize git2_resource destructor for git_note.
    template<> php_git_note::~git2_resource()
    {
        git_note_free(handle);
    }

    // Explicitly specialize git2_resource destructor for git_note_iterator.
    template<> php_git_note_iterator::~git2_resource()
    {
        git_note_iterator_free(handle);
    }

} // namespace php_git2

// Functions

static constexpr auto ZIF_git_note_author = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        const git_signature*,
        const git_note*>::func<git_note_author>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_note>
        >,
    php_git2::php_resource_rethandler<
        php_git2::php_git_signature_nofree,
        const git_signature
        >
    >;

static constexpr auto ZIF_git_note_committer = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        const git_signature*,
        const git_note*>::func<git_note_committer>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_note>
        >,
    php_git2::php_resource_rethandler<
        php_git2::php_git_signature_nofree,
        const git_signature
        >
    >;

static constexpr auto ZIF_git_note_create = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_repository*,
        const char*,
        const git_signature*,
        const git_signature*,
        const git_oid*,
        const char*,
        int>::func<git_note_create>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string_nullable,
        php_git2::php_resource<php_git2::php_git_signature>,
        php_git2::php_resource<php_git2::php_git_signature>,
        php_git2::php_git_oid_fromstr,
        php_git2::php_string,
        php_git2::php_bool
        >,
    1,
    php_git2::sequence<1,2,3,4,5,6,7>,
    php_git2::sequence<0,1,2,3,4,5,6,7>
    >;

static constexpr auto ZIF_git_note_foreach = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*,
        git_note_foreach_cb,
        void*>::func<git_note_foreach>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string_nullable,
        php_git2::php_callback_handler<php_git2::note_foreach_callback>,
        php_git2::php_callback_sync
        >,
    -1,
    php_git2::sequence<0,1,3,3>, // pass callback in twice for function and payload
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_note_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_note>
        >
    >;

static constexpr auto ZIF_git_note_id = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        const git_note*>::func<git_note_id>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_note>
        >,
    0
    >;

static constexpr auto ZIF_git_note_iterator_free = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_note_iterator>
        >
    >;

static constexpr auto ZIF_git_note_iterator_new = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_note_iterator**,
        git_repository*,
        const char*>::func<git_note_iterator_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_note_iterator>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string_nullable
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_note_message = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_note*>::func<git_note_message>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_note>
        >,
    0
    >;

static constexpr auto ZIF_git_note_next = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_oid*,
        git_note_iterator*>::func<git_note_next>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_git_oid_out,
        php_git2::php_resource<php_git2::php_git_note_iterator>
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>
    >;

static constexpr auto ZIF_git_note_read = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_note**,
        git_repository*,
        const char*,
        const git_oid*>::func<git_note_read>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_note>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string_nullable,
        php_git2::php_git_oid_fromstr
        >,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_git_note_remove = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_repository*,
        const char*,
        const git_signature*,
        const git_signature*,
        const git_oid*>::func<git_note_remove>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string_nullable,
        php_git2::php_resource<php_git2::php_git_signature>,
        php_git2::php_resource<php_git2::php_git_signature>,
        php_git2::php_git_oid_fromstr
        >
    >;

// Function Entries

#define GIT_NOTE_FE                             \
    PHP_GIT2_FE(git_note_author)                \
    PHP_GIT2_FE(git_note_committer)             \
    PHP_GIT2_FE(git_note_create)                \
    PHP_GIT2_FE(git_note_foreach)               \
    PHP_GIT2_FE(git_note_free)                  \
    PHP_GIT2_FE(git_note_id)                    \
    PHP_GIT2_FE(git_note_iterator_free)         \
    PHP_GIT2_FE(git_note_iterator_new)          \
    PHP_GIT2_FE(git_note_message)               \
    PHP_GIT2_FE(git_note_next)                  \
    PHP_GIT2_FE(git_note_read)                  \
    PHP_GIT2_FE(git_note_remove)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
