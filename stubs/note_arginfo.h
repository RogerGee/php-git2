/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 5e93fada7d4de08f448e2c95741e4938dee8042b */

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_note_author, 0, 0, 1)
	ZEND_ARG_INFO(0, note)
ZEND_END_ARG_INFO()

#define arginfo_git_note_committer arginfo_git_note_author

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_note_create, 0, 7, IS_STRING, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, notes_ref, IS_STRING, 1)
	ZEND_ARG_INFO(0, author)
	ZEND_ARG_INFO(0, committer)
	ZEND_ARG_TYPE_INFO(0, oid, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, note, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, force, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_note_foreach, 0, 4, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, notes_ref, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_note_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, note)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_note_id, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, note)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_note_iterator_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, note_iterator)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_note_iterator_new, 0, 0, 2)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, notes_ref, IS_STRING, 1)
ZEND_END_ARG_INFO()

#define arginfo_git_note_message arginfo_git_note_id

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_note_next, 0, 2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(1, annotated_id, IS_STRING, 0)
	ZEND_ARG_INFO(0, note_iterator)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_note_read, 0, 0, 3)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, notes_ref, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, oid, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_note_remove, 0, 5, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, notes_ref, IS_STRING, 1)
	ZEND_ARG_INFO(0, author)
	ZEND_ARG_INFO(0, committer)
	ZEND_ARG_TYPE_INFO(0, oid, IS_STRING, 0)
ZEND_END_ARG_INFO()
