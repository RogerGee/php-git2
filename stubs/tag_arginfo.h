/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 82e340efe0d57bda02030c8e074999bbd0b1f50b */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_tag_annotation_create, 0, 5, IS_STRING, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, tag_name, IS_STRING, 0)
	ZEND_ARG_INFO(0, target)
	ZEND_ARG_INFO(0, tagger)
	ZEND_ARG_TYPE_INFO(0, message, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_tag_create, 0, 6, IS_STRING, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, tag_name, IS_STRING, 0)
	ZEND_ARG_INFO(0, target)
	ZEND_ARG_INFO(0, tagger)
	ZEND_ARG_TYPE_INFO(0, message, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, force, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_tag_create_frombuffer, 0, 3, IS_STRING, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, buffer, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, force, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_tag_create_lightweight, 0, 4, IS_STRING, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, tag_name, IS_STRING, 0)
	ZEND_ARG_INFO(0, target)
	ZEND_ARG_TYPE_INFO(0, buffer, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_tag_delete, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, tag_name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_tag_dup, 0, 0, 1)
	ZEND_ARG_INFO(0, tag)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_tag_foreach, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_tag_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, tag)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_tag_id, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, tag)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_tag_list, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, repo)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_tag_list_match, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, pattern, IS_STRING, 0)
	ZEND_ARG_INFO(0, repo)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_tag_lookup, 0, 0, 2)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, id, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_tag_lookup_prefix, 0, 0, 2)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, id_prefix, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_tag_message, 0, 1, IS_STRING, 1)
	ZEND_ARG_INFO(0, tag)
ZEND_END_ARG_INFO()

#define arginfo_git_tag_name arginfo_git_tag_id

#define arginfo_git_tag_owner arginfo_git_tag_dup

#define arginfo_git_tag_peel arginfo_git_tag_dup

#define arginfo_git_tag_tagger arginfo_git_tag_dup

#define arginfo_git_tag_target arginfo_git_tag_dup

#define arginfo_git_tag_target_id arginfo_git_tag_id

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_tag_target_type, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, tag)
ZEND_END_ARG_INFO()
