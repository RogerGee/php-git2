/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: c6e8095da978c4565da176c65894a495dd80391c */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_revwalk_add_hide_cb, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, revwalk)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_revwalk_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, revwalk)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_revwalk_hide, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, revwalk)
	ZEND_ARG_TYPE_INFO(0, commit_id, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_revwalk_hide_glob, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, revwalk)
	ZEND_ARG_TYPE_INFO(0, glob, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_revwalk_hide_head arginfo_git_revwalk_free

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_revwalk_hide_ref, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, revwalk)
	ZEND_ARG_TYPE_INFO(0, refname, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_revwalk_new, 0, 0, 1)
	ZEND_ARG_INFO(0, repo)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_git_revwalk_next, 0, 1, MAY_BE_STRING|MAY_BE_BOOL)
	ZEND_ARG_INFO(0, revwalk)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_revwalk_push, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, revwalk)
	ZEND_ARG_TYPE_INFO(0, id, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_revwalk_push_glob arginfo_git_revwalk_hide_glob

#define arginfo_git_revwalk_push_head arginfo_git_revwalk_free

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_revwalk_push_range, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, revwalk)
	ZEND_ARG_TYPE_INFO(0, range, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_revwalk_push_ref arginfo_git_revwalk_hide_ref

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_revwalk_repository, 0, 0, 1)
	ZEND_ARG_INFO(0, revwalk)
ZEND_END_ARG_INFO()

#define arginfo_git_revwalk_reset arginfo_git_revwalk_free

#define arginfo_git_revwalk_simplify_first_parent arginfo_git_revwalk_free

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_revwalk_sorting, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, revwalk)
	ZEND_ARG_TYPE_INFO(0, sort_mode, IS_LONG, 0)
ZEND_END_ARG_INFO()
