/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: b4bd84e48de32e393f3cfa195672b9bf0244fe5a */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_treebuilder_clear, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, treebuilder)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_treebuilder_entrycount, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, treebuilder)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_treebuilder_filter, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, treebuilder)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_treebuilder_free arginfo_git_treebuilder_clear

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_treebuilder_get, 0, 0, 2)
	ZEND_ARG_INFO(0, treebuilder)
	ZEND_ARG_TYPE_INFO(0, filename, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_treebuilder_insert, 0, 0, 4)
	ZEND_ARG_INFO(0, treebuilder)
	ZEND_ARG_TYPE_INFO(0, filename, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, id, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, filemode, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_treebuilder_new, 0, 0, 2)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_INFO(0, tree)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_treebuilder_remove, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, treebuilder)
	ZEND_ARG_TYPE_INFO(0, filename, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_treebuilder_write, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, treebuilder)
ZEND_END_ARG_INFO()
