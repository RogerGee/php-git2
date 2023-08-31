/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: a471dc243470601a91af1aad0225ca30dd18e24e */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_remote_add_fetch, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, remote, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, refspec, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_remote_add_push arginfo_git_remote_add_fetch

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_remote_autotag, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, remote)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_remote_connect, 0, 5, IS_VOID, 0)
	ZEND_ARG_INFO(0, remote)
	ZEND_ARG_TYPE_INFO(0, direction, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, callbacks, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, proxy_options, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, custom_headers, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_remote_connected, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, remote)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_remote_create, 0, 0, 3)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, url, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_remote_create_anonymous, 0, 0, 2)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, url, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_remote_create_with_fetchspec, 0, 0, 4)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, url, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, fetchspec, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_remote_default_branch, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, remote)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_remote_delete, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_remote_disconnect, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, remote)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_remote_download, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, remote)
	ZEND_ARG_TYPE_INFO(0, refspecs, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, fetch_options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_remote_dup, 0, 0, 1)
	ZEND_ARG_INFO(0, remote)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_remote_fetch, 0, 4, IS_VOID, 0)
	ZEND_ARG_INFO(0, remote)
	ZEND_ARG_TYPE_INFO(0, refspecs, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, fetch_options, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, reflog_message, IS_STRING, 1)
ZEND_END_ARG_INFO()

#define arginfo_git_remote_free arginfo_git_remote_disconnect

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_remote_get_fetch_refspecs, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, remote)
ZEND_END_ARG_INFO()

#define arginfo_git_remote_get_push_refspecs arginfo_git_remote_get_fetch_refspecs

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_remote_get_refspec, 0, 0, 2)
	ZEND_ARG_INFO(0, remote)
	ZEND_ARG_TYPE_INFO(0, n, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_remote_is_valid_name, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, remote_name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_remote_list, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, repo)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_remote_lookup, 0, 0, 2)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_remote_ls arginfo_git_remote_get_fetch_refspecs

#define arginfo_git_remote_name arginfo_git_remote_default_branch

#define arginfo_git_remote_owner arginfo_git_remote_dup

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_remote_prune, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, remote)
	ZEND_ARG_TYPE_INFO(0, callbacks, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

#define arginfo_git_remote_prune_refs arginfo_git_remote_disconnect

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_remote_push, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, remote)
	ZEND_ARG_TYPE_INFO(0, refspecs, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, push_options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

#define arginfo_git_remote_pushurl arginfo_git_remote_default_branch

#define arginfo_git_remote_refspec_count arginfo_git_remote_autotag

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_remote_rename, 0, 3, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, name, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, new_name, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_remote_set_autotag, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, remote, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_remote_set_pushurl, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_TYPE_INFO(0, remote, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, url, IS_STRING, 0)
ZEND_END_ARG_INFO()

#define arginfo_git_remote_set_url arginfo_git_remote_set_pushurl

#define arginfo_git_remote_stats arginfo_git_remote_get_fetch_refspecs

#define arginfo_git_remote_stop arginfo_git_remote_disconnect

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_remote_update_tips, 0, 5, IS_VOID, 0)
	ZEND_ARG_INFO(0, remote)
	ZEND_ARG_TYPE_INFO(0, callbacks, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, update_fetchhead, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, download_tags, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, reflog_message, IS_STRING, 1)
ZEND_END_ARG_INFO()

#define arginfo_git_remote_upload arginfo_git_remote_push

#define arginfo_git_remote_url arginfo_git_remote_default_branch
