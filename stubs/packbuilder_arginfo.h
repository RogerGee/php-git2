/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 22865027d184681b70d851716b65e905dd0604f5 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_packbuilder_foreach, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, packbuilder)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO(0, payload_or_stream, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_packbuilder_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, packbuilder)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_packbuilder_hash, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, packbuilder)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_packbuilder_insert, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, packbuilder)
	ZEND_ARG_TYPE_INFO(0, id, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_packbuilder_insert_commit, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, packbuilder)
	ZEND_ARG_TYPE_INFO(0, id, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_packbuilder_insert_recur arginfo_git_packbuilder_insert

#define arginfo_git_packbuilder_insert_tree arginfo_git_packbuilder_insert_commit

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_packbuilder_insert_walk, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, packbuilder)
	ZEND_ARG_OBJ_INFO(0, revwalk, resource, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_packbuilder_new, 0, 0, 1)
	ZEND_ARG_INFO(0, repo)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_packbuilder_object_count, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, packbuilder)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_packbuilder_set_callbacks, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, packbuilder)
	ZEND_ARG_TYPE_INFO(0, progress_callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_packbuilder_set_threads, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, packbuilder)
	ZEND_ARG_TYPE_INFO(0, n_threads, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_packbuilder_write, 0, 5, IS_VOID, 0)
	ZEND_ARG_INFO(0, packbuilder)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, mode, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, progress_callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_packbuilder_written arginfo_git_packbuilder_object_count
