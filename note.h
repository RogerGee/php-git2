/*
 * note.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_NOTE_H
#define PHPGIT2_NOTE_H

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

static constexpr auto ZIF_GIT_NOTE_AUTHOR = zif_php_git2_function_rethandler<
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

static constexpr auto ZIF_GIT_NOTE_COMMITTER = zif_php_git2_function_rethandler<
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

static constexpr auto ZIF_GIT_NOTE_CREATE = zif_php_git2_function<
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

static constexpr auto ZIF_GIT_NOTE_FOREACH = zif_php_git2_function<
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

static constexpr auto ZIF_GIT_NOTE_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_note>
        >
    >;

static constexpr auto ZIF_GIT_NOTE_ID = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        const git_note*>::func<git_note_id>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_note>
        >,
    0
    >;

static constexpr auto ZIF_GIT_NOTE_ITERATOR_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_note_iterator>
        >
    >;

static constexpr auto ZIF_GIT_NOTE_ITERATOR_NEW = zif_php_git2_function<
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

static constexpr auto ZIF_GIT_NOTE_MESSAGE = zif_php_git2_function<
    php_git2::func_wrapper<
        const char*,
        const git_note*>::func<git_note_message>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_note>
        >,
    0
    >;

static constexpr auto ZIF_GIT_NOTE_NEXT = zif_php_git2_function<
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
ZEND_BEGIN_ARG_INFO_EX(git_note_next_arginfo,0,0,2)
    ZEND_ARG_INFO(1,annotated_id)
    ZEND_ARG_INFO(0,iterator)
ZEND_END_ARG_INFO()

static constexpr auto ZIF_GIT_NOTE_READ = zif_php_git2_function<
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

static constexpr auto ZIF_GIT_NOTE_REMOVE = zif_php_git2_function<
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

#define GIT_NOTE_FE                                                     \
    PHP_GIT2_FE(git_note_author,ZIF_GIT_NOTE_AUTHOR,NULL)               \
    PHP_GIT2_FE(git_note_committer,ZIF_GIT_NOTE_COMMITTER,NULL)         \
    PHP_GIT2_FE(git_note_create,ZIF_GIT_NOTE_CREATE,NULL)               \
    PHP_GIT2_FE(git_note_foreach,ZIF_GIT_NOTE_FOREACH,NULL)             \
    PHP_GIT2_FE(git_note_free,ZIF_GIT_NOTE_FREE,NULL)                   \
    PHP_GIT2_FE(git_note_id,ZIF_GIT_NOTE_ID,NULL)                       \
    PHP_GIT2_FE(git_note_iterator_free,ZIF_GIT_NOTE_ITERATOR_FREE,NULL) \
    PHP_GIT2_FE(git_note_iterator_new,ZIF_GIT_NOTE_ITERATOR_NEW,NULL)   \
    PHP_GIT2_FE(git_note_message,ZIF_GIT_NOTE_MESSAGE,NULL)             \
    PHP_GIT2_FE(git_note_next,ZIF_GIT_NOTE_NEXT,git_note_next_arginfo)  \
    PHP_GIT2_FE(git_note_read,ZIF_GIT_NOTE_READ,NULL)                   \
    PHP_GIT2_FE(git_note_remove,ZIF_GIT_NOTE_REMOVE,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
