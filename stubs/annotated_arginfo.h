/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: cc61d900ec2975a1b2f2c6ddd2e21722d1ce94b0 */

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_annotated_commit_from_fetchhead, 0, 0, 4)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, branch_name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, remote_url, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, oid, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_annotated_commit_from_ref, 0, 0, 2)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_INFO(0, ref)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_annotated_commit_from_revspec, 0, 0, 2)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, revspec, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_annotated_commit_lookup, 0, 0, 2)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, id, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_annotated_commit_id, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, commit)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_annotated_commit_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, commit)
ZEND_END_ARG_INFO()
