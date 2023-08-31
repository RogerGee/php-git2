/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 595f773f2dc4ad64962163fd27d4f743601da7d2 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_reset, 0, 4, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_INFO(0, target_object)
	ZEND_ARG_TYPE_INFO(0, reset_type, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, checkout_options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_reset_default, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_INFO(0, target_object)
	ZEND_ARG_TYPE_INFO(0, pathspecs, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_reset_from_annotated, 0, 4, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_INFO(0, annotated_commit)
	ZEND_ARG_TYPE_INFO(0, reset_type, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, checkout_options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()
