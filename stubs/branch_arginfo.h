/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 36ea2eedffb28fc4a7fb863130ede052e942c26e */

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_branch_create, 0, 0, 4)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, branch_name, IS_STRING, 0)
	ZEND_ARG_INFO(0, target)
	ZEND_ARG_TYPE_INFO(0, force, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_branch_create_from_annotated arginfo_git_branch_create

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_branch_delete, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, branch)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_branch_is_head, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, branch)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_branch_iterator_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, iter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_branch_iterator_new, 0, 0, 2)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_branch_lookup, 0, 0, 3)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, branch_name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, branch_type, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_branch_move, 0, 0, 3)
	ZEND_ARG_INFO(0, branch)
	ZEND_ARG_TYPE_INFO(0, new_branch_name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, force, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_branch_name, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, branch)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_branch_next, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(1, type, IS_LONG, 0)
	ZEND_ARG_INFO(0, iter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_branch_set_upstream, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, branch)
	ZEND_ARG_TYPE_INFO(0, branch_name, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_branch_upstream, 0, 0, 1)
	ZEND_ARG_INFO(0, branch)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_git_branch_upstream_name, 0, 2, MAY_BE_STRING|MAY_BE_BOOL)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, refname, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_branch_upstream_remote, 0, 2, IS_STRING, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, refname, IS_STRING, 0)
ZEND_END_ARG_INFO()
