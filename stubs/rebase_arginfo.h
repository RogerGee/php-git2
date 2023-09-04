/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 7e892297078b673059400e1442893951832d49a8 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_rebase_abort, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, rebase)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_rebase_commit, 0, 4, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(1, id, IS_STRING, 0)
	ZEND_ARG_INFO(0, rebase)
	ZEND_ARG_INFO(0, author)
	ZEND_ARG_INFO(0, committer)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, message_encoding, IS_STRING, 0, "null")
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, message, IS_STRING, 0, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_rebase_finish, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, rebase)
	ZEND_ARG_INFO(0, signature)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_rebase_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, rebase)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_rebase_init, 0, 0, 5)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_INFO(0, branch)
	ZEND_ARG_INFO(0, upstream)
	ZEND_ARG_INFO(0, onto)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_rebase_inmemory_index, 0, 0, 1)
	ZEND_ARG_INFO(0, rebase)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_rebase_next, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, rebase)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_rebase_open, 0, 0, 2)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_rebase_operation_byindex, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, rebase)
	ZEND_ARG_TYPE_INFO(0, idx, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_rebase_operation_current, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, rebase)
ZEND_END_ARG_INFO()

#define arginfo_git_rebase_operation_entrycount arginfo_git_rebase_operation_current
