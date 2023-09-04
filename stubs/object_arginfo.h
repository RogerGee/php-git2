/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: fce4600cea0335f90aad7224f8810eab4887fe98 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_object_dup, 0, 0, 1)
	ZEND_ARG_INFO(0, object)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_object_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, object)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_object_id, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, object)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_object_lookup, 0, 0, 3)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, id, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_object_lookup_bypath, 0, 0, 3)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_object_lookup_prefix arginfo_git_object_lookup

#define arginfo_git_object_owner arginfo_git_object_dup

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_object_peel, 0, 0, 2)
	ZEND_ARG_INFO(0, object)
	ZEND_ARG_TYPE_INFO(0, target_type, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_object_short_id arginfo_git_object_id

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_object_string2type, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, str, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_object_type, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, object)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_object_type2string, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_object_typeisloose, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
ZEND_END_ARG_INFO()
