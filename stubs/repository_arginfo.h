/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 31ecdd9de4e6862cf3542762354672d9cec536df */

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_repository_config, 0, 0, 1)
	ZEND_ARG_INFO(0, repo)
ZEND_END_ARG_INFO()

#define arginfo_git_repository_config_snapshot arginfo_git_repository_config

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_repository_detach_head, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, repo)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_repository_discover, 0, 2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, start_path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, across_fs, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, ceiling_dirs, IS_STRING, 0, "null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_repository_fetchhead_foreach, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_repository_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_repository_get_namespace, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, repo)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_repository_hashfile, 0, 3, IS_STRING, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, as_path, IS_STRING, 0, "null")
ZEND_END_ARG_INFO()

#define arginfo_git_repository_head arginfo_git_repository_config

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_repository_head_detached, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, repo)
ZEND_END_ARG_INFO()

#define arginfo_git_repository_head_unborn arginfo_git_repository_head_detached

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_repository_ident, 0, 2, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(1, email, IS_STRING, 0)
	ZEND_ARG_INFO(0, repo)
ZEND_END_ARG_INFO()

#define arginfo_git_repository_index arginfo_git_repository_config

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_repository_init, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, is_bare, _IS_BOOL, 0, "false")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_repository_init_ext, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, options, IS_ARRAY, 0, "null")
ZEND_END_ARG_INFO()

#define arginfo_git_repository_is_bare arginfo_git_repository_head_detached

#define arginfo_git_repository_is_empty arginfo_git_repository_head_detached

#define arginfo_git_repository_is_shallow arginfo_git_repository_head_detached

#define arginfo_git_repository_mergehead_foreach arginfo_git_repository_fetchhead_foreach

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_git_repository_message, 0, 1, MAY_BE_STRING|MAY_BE_BOOL)
	ZEND_ARG_INFO(0, repo)
ZEND_END_ARG_INFO()

#define arginfo_git_repository_message_remove arginfo_git_repository_free

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_repository_new, 0, 0, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_repository_odb arginfo_git_repository_config

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_repository_open, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_repository_open_bare, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, bare_path, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_repository_open_ext, 0, 0, 3)
	ZEND_ARG_TYPE_INFO(0, path, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, ceiling_dirs, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_repository_path arginfo_git_repository_get_namespace

#define arginfo_git_repository_refdb arginfo_git_repository_config

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_repository_reinit_filesystem, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, recurse_submodules, _IS_BOOL, 0, "true")
ZEND_END_ARG_INFO()

#define arginfo_git_repository_set_bare arginfo_git_repository_free

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_repository_set_config, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_INFO(0, config)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_repository_set_head, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, refname, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_repository_set_head_detached, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, committish, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_repository_set_head_detached_from_annotated, 0, 2, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_INFO(0, committish)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_repository_set_ident, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, email, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_repository_set_index, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_INFO(0, index)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_repository_set_namespace, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, namespace, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_repository_set_odb, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_INFO(0, odb)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_repository_set_refdb, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_INFO(0, refdb)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_repository_set_workdir, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, workdir, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, update_gitlink, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_repository_state arginfo_git_repository_detach_head

#define arginfo_git_repository_state_cleanup arginfo_git_repository_free

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_repository_workdir, 0, 1, IS_STRING, 1)
	ZEND_ARG_INFO(0, repo)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_repository_wrap_odb, 0, 0, 1)
	ZEND_ARG_INFO(0, odb)
ZEND_END_ARG_INFO()
