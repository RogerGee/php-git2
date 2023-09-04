/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: f85bd0579b926b9b351cfb470ce27eed41a4f136 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_indexer_append, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, indexer)
	ZEND_ARG_TYPE_INFO(0, data, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_indexer_commit, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, indexer)
ZEND_END_ARG_INFO()

#define arginfo_git_indexer_free arginfo_git_indexer_commit

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_indexer_hash, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, indexer)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_indexer_new, 0, 0, 4)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, mode, IS_LONG, 0)
	ZEND_ARG_INFO(0, odb)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git2_indexer_stats, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, indexer)
ZEND_END_ARG_INFO()
