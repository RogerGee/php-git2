/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 69c05de6515c7ee1324956a765768b3eb64c8e38 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_blame_buffer, 0, 0, 2)
	ZEND_ARG_INFO(0, reference)
	ZEND_ARG_TYPE_INFO(0, buffer, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_blame_file, 0, 0, 3)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_blame_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, blame)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_blame_get_hunk_byindex, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, blame)
	ZEND_ARG_TYPE_INFO(0, index, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_blame_get_hunk_byline, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, blame)
	ZEND_ARG_TYPE_INFO(0, lineno, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_blame_get_hunk_count, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, blame)
ZEND_END_ARG_INFO()
