/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: c4ac7a072a9b98e5b789ceb2a50681d41317aa2e */

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_describe_commit, 0, 0, 1)
	ZEND_ARG_INFO(0, committish)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_describe_format, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, result)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_describe_result_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, result)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_describe_workdir, 0, 0, 1)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_ARRAY, 1, "null")
ZEND_END_ARG_INFO()
