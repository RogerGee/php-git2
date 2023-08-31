/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: c0e1d9d7c803c2e9f2ba5554620429cb2cfafafb */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_submodule_add_finalize, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, submodule)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_submodule_add_setup, 0, 0, 4)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, url, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, use_gitlink, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_submodule_add_to_index, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, submodule)
	ZEND_ARG_TYPE_INFO(0, write_index, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_submodule_branch, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, submodule)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_submodule_fetch_recurse_submodules, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, submodule)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_submodule_foreach, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_submodule_free arginfo_git_submodule_add_finalize

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_submodule_head_id, 0, 1, IS_STRING, 1)
	ZEND_ARG_INFO(0, submodule)
ZEND_END_ARG_INFO()

#define arginfo_git_submodule_ignore arginfo_git_submodule_fetch_recurse_submodules

#define arginfo_git_submodule_index_id arginfo_git_submodule_head_id

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_submodule_init, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, submodule)
	ZEND_ARG_TYPE_INFO(0, overwrite, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_submodule_location arginfo_git_submodule_fetch_recurse_submodules

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_submodule_lookup, 0, 0, 2)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_submodule_name arginfo_git_submodule_branch

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_submodule_open, 0, 0, 1)
	ZEND_ARG_INFO(0, submodule)
ZEND_END_ARG_INFO()

#define arginfo_git_submodule_owner arginfo_git_submodule_open

#define arginfo_git_submodule_path arginfo_git_submodule_branch

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_submodule_reload, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, submodule)
	ZEND_ARG_TYPE_INFO(0, force, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_submodule_repo_init, 0, 0, 2)
	ZEND_ARG_INFO(0, submodule)
	ZEND_ARG_TYPE_INFO(0, use_gitlink, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_submodule_resolve_url, 0, 2, IS_STRING, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, url, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_submodule_set_branch, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, branch, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_submodule_set_fetch_recurse_submodules, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, fetch_recurse_submodules, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_submodule_set_ignore, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, ignore, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_submodule_set_update, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, update, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_submodule_set_url, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, url, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_submodule_status, 0, 3, IS_LONG, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, ignore, IS_LONG, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_submodule_sync arginfo_git_submodule_add_finalize

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_submodule_update, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, submodule)
	ZEND_ARG_TYPE_INFO(0, init, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, submodule_update_options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

#define arginfo_git_submodule_update_strategy arginfo_git_submodule_fetch_recurse_submodules

#define arginfo_git_submodule_url arginfo_git_submodule_branch

#define arginfo_git_submodule_wd_id arginfo_git_submodule_head_id
