/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 5da98298ca353e7a67b7949b92f21b1b55484df8 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_add, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_TYPE_INFO(0, source_entry, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_add_all, 0, 5, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_TYPE_INFO(0, pathspec, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO(0, paylaod, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_add_bypath, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_add_from_buffer, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_TYPE_INFO(0, entry, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, buffer, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_caps, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_checksum, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_clear, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_conflict_add, 0, 4, IS_VOID, 0)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_TYPE_INFO(0, ancestor_entry, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, our_entry, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, their_entry, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_index_conflict_cleanup arginfo_git_index_clear

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_conflict_get, 0, 5, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(1, ancestor, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(1, ours, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(1, theirs, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_conflict_iterator_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, iter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_index_conflict_iterator_new, 0, 0, 1)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_conflict_next, 0, 4, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(1, ancestor, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(1, ours, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(1, theirs, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, iterator)
ZEND_END_ARG_INFO()

#define arginfo_git_index_conflict_remove arginfo_git_index_add_bypath

#define arginfo_git_index_entrycount arginfo_git_index_caps

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_entry_is_conflict, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, entry, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_entry_stage, 0, 1, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, entry, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_find, 0, 2, IS_LONG, 0)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_find_prefix, 0, 2, IS_LONG, 0)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_TYPE_INFO(0, prefix, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_index_free arginfo_git_index_clear

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_git_index_get_byindex, 0, 2, MAY_BE_ARRAY|MAY_BE_BOOL)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_git_index_get_bypath, 0, 3, MAY_BE_ARRAY|MAY_BE_BOOL)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, stage, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_has_conflicts, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_index_new, 0, 0, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_index_open, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, index_path, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_index_owner arginfo_git_index_conflict_iterator_new

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_path, 0, 1, IS_STRING, 1)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_read, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_TYPE_INFO(0, force, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_read_tree, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_INFO(0, tree)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_remove, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, stage, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_remove_all, 0, 4, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_TYPE_INFO(0, pathspec, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_index_remove_bypath arginfo_git_index_add_bypath

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_remove_directory, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_TYPE_INFO(0, dir, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, stage, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_set_caps, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_TYPE_INFO(0, caps, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_set_version, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_TYPE_INFO(0, version, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_index_update_all arginfo_git_index_remove_all

#define arginfo_git_index_version arginfo_git_index_caps

#define arginfo_git_index_write arginfo_git_index_clear

#define arginfo_git_index_write_tree arginfo_git_index_checksum

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_index_write_tree_to, 0, 2, IS_STRING, 0)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_INFO(0, repo)
ZEND_END_ARG_INFO()
