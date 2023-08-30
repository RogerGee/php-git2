/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 25a5a30aca69dde9e79c9a0a487d7caa04a52663 */

ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(arginfo_git_refdb_backend_fs, 0, 1, GitRefDBBackend, 0)
	ZEND_ARG_INFO(0, refdb)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_refdb_compress, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, refdb)
ZEND_END_ARG_INFO()

#define arginfo_git_refdb_free arginfo_git_refdb_compress

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_refdb_new, 0, 0, 1)
	ZEND_ARG_INFO(0, repo)
ZEND_END_ARG_INFO()

#define arginfo_git_refdb_open arginfo_git_refdb_new

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_refdb_set_backend, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, refdb)
	ZEND_ARG_OBJ_INFO(0, backend, GitRefDBBackend, 0)
ZEND_END_ARG_INFO()
