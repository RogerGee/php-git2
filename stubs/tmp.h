ZEND_BEGIN_ARG_INFO(arginfo_git_tree_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tree_id,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tree_lookup,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tree_lookup_prefix,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tree_owner,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tree_dup,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tree_entrycount,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tree_entry_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tree_entry_byid,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tree_entry_byindex,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tree_entry_byname,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tree_entry_bypath,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tree_entry_dup,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tree_entry_filemode,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tree_entry_filemode_raw,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tree_entry_name,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tree_entry_to_object,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tree_entry_cmp,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tree_entry_id,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tree_entry_type,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tree_walk,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_treebuilder_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_treebuilder_new,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_treebuilder_write,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_treebuilder_insert,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_treebuilder_get,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_treebuilder_remove,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_treebuilder_clear,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_treebuilder_entrycount,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_treebuilder_filter,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tag_annotation_create,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tag_create,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tag_create_frombuffer,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tag_create_lightweight,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tag_lookup,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tag_lookup_prefix,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tag_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tag_id,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tag_target_id,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tag_message,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tag_name,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tag_owner,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tag_peel,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tag_target,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tag_tagger,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tag_target_type,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tag_delete,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tag_dup,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tag_list,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tag_list_match,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_tag_foreach,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_trace_set,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_worktree_add,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_worktree_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_worktree_is_locked,0,0,2)
    ZEND_ARG_INFO(1,reason)
    ZEND_ARG_INFO(0,worktree)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_worktree_is_prunable,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_worktree_list,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_worktree_lock,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_worktree_lookup,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_worktree_name,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_worktree_open_from_repository,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_worktree_path,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_worktree_prune,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_worktree_unlock,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_worktree_validate,{})
ZEND_END_ARG_INFO()
