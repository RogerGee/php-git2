/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: cab867c37870423647bc235a63b2090ae473cd4c */

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_tree_dup, 0, 0, 1)
	ZEND_ARG_INFO(0, tree)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_tree_entry_byid, 0, 0, 2)
	ZEND_ARG_INFO(0, tree)
	ZEND_ARG_TYPE_INFO(0, id, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_tree_entry_byindex, 0, 0, 2)
	ZEND_ARG_INFO(0, tree)
	ZEND_ARG_TYPE_INFO(0, idx, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_tree_entry_byname, 0, 0, 2)
	ZEND_ARG_INFO(0, tree)
	ZEND_ARG_TYPE_INFO(0, filename, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_tree_entry_bypath, 0, 0, 2)
	ZEND_ARG_INFO(0, tree)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_tree_entry_cmp, 0, 2, IS_LONG, 0)
	ZEND_ARG_INFO(0, tree_entry_1)
	ZEND_ARG_INFO(0, tree_entry_2)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_tree_entry_dup, 0, 0, 1)
	ZEND_ARG_INFO(0, tree_entry)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_tree_entry_filemode, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, tree_entry)
ZEND_END_ARG_INFO()

#define arginfo_git_tree_entry_filemode_raw arginfo_git_tree_entry_filemode

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_tree_entry_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, tree_entry)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_tree_entry_id, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, tree_entry)
ZEND_END_ARG_INFO()

#define arginfo_git_tree_entry_name arginfo_git_tree_entry_id

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_tree_entry_to_object, 0, 0, 2)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_INFO(0, tree_entry)
ZEND_END_ARG_INFO()

#define arginfo_git_tree_entry_type arginfo_git_tree_entry_filemode

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_tree_entrycount, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, tree)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_tree_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, tree)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_tree_id, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, tree)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_tree_lookup, 0, 0, 2)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, id, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_tree_lookup_prefix, 0, 0, 2)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, id_prefix, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_tree_owner arginfo_git_tree_dup

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_tree_walk, 0, 4, IS_VOID, 0)
	ZEND_ARG_INFO(0, tree)
	ZEND_ARG_TYPE_INFO(0, mode, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()
