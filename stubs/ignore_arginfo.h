/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: c979e57be95ec68fc8fbbdbdd3783d4889c01772 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_ignore_add_rule, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, rules, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_ignore_clear_internal_rules, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_ignore_path_is_ignored, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()
