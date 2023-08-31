/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 43f7f20c2357790afe4e6eae9e9f4800f3fc1fcb */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_revert, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_INFO(0, commit)
	ZEND_ARG_TYPE_INFO(0, revert_options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_revert_commit, 0, 0, 5)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_INFO(0, revert_commit)
	ZEND_ARG_INFO(0, our_commit)
	ZEND_ARG_TYPE_INFO(0, mainline, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, merge_options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()
