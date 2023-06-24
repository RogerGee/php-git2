/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 43c1bedad06e3a85416c08e0ca05334eaec61ec1 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_reference_cmp, 0, 2, IS_LONG, 0)
	ZEND_ARG_INFO(0, ref1)
	ZEND_ARG_INFO(0, ref2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_reference_create, 0, 0, 5)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, id, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, force, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, log_message, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_reference_create_matching, 0, 0, 6)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, id, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, force, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, current_id, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, log_message, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_reference_delete, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, ref)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_reference_dup, 0, 0, 1)
	ZEND_ARG_INFO(0, ref)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_reference_dwim, 0, 0, 2)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, shorthand, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_reference_ensure_log, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, refname, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_reference_foreach, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_reference_foreach_glob, 0, 4, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, glob, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_reference_foreach_name arginfo_git_reference_foreach

#define arginfo_git_reference_free arginfo_git_reference_delete

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_reference_has_log, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, refname, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_reference_is_branch, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, ref)
ZEND_END_ARG_INFO()

#define arginfo_git_reference_is_note arginfo_git_reference_is_branch

#define arginfo_git_reference_is_remote arginfo_git_reference_is_branch

#define arginfo_git_reference_is_tag arginfo_git_reference_is_branch

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_reference_is_valid_name, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, refname, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_reference_iterator_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, iter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_reference_iterator_glob_new, 0, 0, 2)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, glob, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_reference_iterator_new, 0, 0, 1)
	ZEND_ARG_INFO(0, repo)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_reference_list, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, repo)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_reference_lookup, 0, 0, 2)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_reference_name, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, ref)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_reference_name_to_id, 0, 2, IS_STRING, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_reference_next, 0, 0, 1)
	ZEND_ARG_INFO(0, iter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_git_reference_next_name, 0, 1, MAY_BE_STRING|MAY_BE_BOOL)
	ZEND_ARG_INFO(0, iter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_reference_normalize_name, 0, 2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_reference_owner arginfo_git_reference_dup

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_reference_peel, 0, 0, 2)
	ZEND_ARG_INFO(0, ref)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_reference_remove, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_reference_rename, 0, 0, 4)
	ZEND_ARG_INFO(0, ref)
	ZEND_ARG_TYPE_INFO(0, new_name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, force, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, log_message, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_reference_resolve arginfo_git_reference_dup

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_reference_set_target, 0, 0, 3)
	ZEND_ARG_INFO(0, ref)
	ZEND_ARG_TYPE_INFO(0, id, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, log_message, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_reference_shorthand arginfo_git_reference_name

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_reference_symbolic_create, 0, 0, 5)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, target, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, force, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, log_message, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_reference_symbolic_create_matching, 0, 0, 6)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, target, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, force, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, current_value, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, log_message, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_reference_symbolic_set_target, 0, 0, 3)
	ZEND_ARG_INFO(0, ref)
	ZEND_ARG_TYPE_INFO(0, target, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, log_message, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_reference_symbolic_target, 0, 1, IS_STRING, 1)
	ZEND_ARG_INFO(0, ref)
ZEND_END_ARG_INFO()

#define arginfo_git_reference_target arginfo_git_reference_symbolic_target

#define arginfo_git_reference_target_peel arginfo_git_reference_symbolic_target

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_reference_type, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, ref)
ZEND_END_ARG_INFO()
