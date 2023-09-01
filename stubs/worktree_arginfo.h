/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 6668ef174327bd65b62b3cd95ff3723d3b63ab7c */

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_worktree_add, 0, 0, 4)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, opts, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_worktree_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, worktree)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_worktree_is_locked, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(1, reason, IS_STRING, 0)
	ZEND_ARG_INFO(0, worktree)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_worktree_is_prunable, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, worktree)
	ZEND_ARG_TYPE_INFO(0, opts, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_worktree_list, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, repo)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_worktree_lock, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, worktree)
	ZEND_ARG_TYPE_INFO(0, reason, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_worktree_lookup, 0, 0, 2)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_worktree_name, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, worktree)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_worktree_open_from_repository, 0, 0, 1)
	ZEND_ARG_INFO(0, repo)
ZEND_END_ARG_INFO()

#define arginfo_git_worktree_path arginfo_git_worktree_name

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_worktree_prune, 0, 2, IS_LONG, 0)
	ZEND_ARG_INFO(0, worktree)
	ZEND_ARG_TYPE_INFO(0, opts, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_worktree_unlock, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, worktree)
ZEND_END_ARG_INFO()

#define arginfo_git_worktree_validate arginfo_git_worktree_free
