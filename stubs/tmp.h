ZEND_BEGIN_ARG_INFO(arginfo_git_reference__alloc,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference__alloc_symbolic,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_revwalk_new,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_revwalk_push,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_revwalk_next,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_revwalk_hide,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_revwalk_sorting,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_revwalk_reset,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_revwalk_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_revwalk_add_hide_cb,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_revwalk_hide_glob,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_revwalk_hide_head,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_revwalk_hide_ref,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_revwalk_push_glob,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_revwalk_push_head,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_revwalk_push_range,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_revwalk_push_ref,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_revwalk_repository,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_revwalk_simplify_first_parent,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_packbuilder_new,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_packbuilder_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_packbuilder_hash,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_packbuilder_insert_commit,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_packbuilder_insert_walk,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_packbuilder_insert_recur,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_packbuilder_insert_tree,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_packbuilder_insert,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_packbuilder_foreach,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_packbuilder_set_threads,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_packbuilder_set_callbacks,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_packbuilder_object_count,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_packbuilder_write,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_packbuilder_written,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_new,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_write_pack,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_open,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_odb_open_rstream,0,0,4)
    ZEND_ARG_INFO(1,length)
    ZEND_ARG_INFO(1,type)
    ZEND_ARG_INFO(0,odb)
    ZEND_ARG_INFO(0,oid)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_write,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_read,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_odb_read_header,0,0,3)
    ZEND_ARG_INFO(1,type)
    ZEND_ARG_INFO(0,odb)
    ZEND_ARG_INFO(0,id)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_read_prefix,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_object_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_object_data,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_object_size,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_object_id,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_object_type,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_object_dup,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_backend_pack,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_backend_loose,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_backend_one_pack,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_open_wstream,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_stream_read,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_stream_write,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_stream_finalize_write,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_add_alternate,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_add_disk_alternate,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_add_backend,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_exists,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_exists_prefix,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_expand_ids,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_foreach,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_refresh,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_get_backend,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_num_backends,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_hash,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_odb_hashfile,{})
ZEND_END_ARG_INFO()

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

ZEND_BEGIN_ARG_INFO(arginfo_git_signature_new,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_signature_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_signature_dup,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_signature_default,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_signature_now,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_signature_from_buffer,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git2_signature_convert,{})
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

ZEND_BEGIN_ARG_INFO(arginfo_git_revparse,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_revparse_ext,0,0,2)
    ZEND_ARG_INFO(1,reference)
    ZEND_ARG_INFO(0,repo)
    ZEND_ARG_INFO(0,spec)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_revparse_single,{})
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

ZEND_BEGIN_ARG_INFO(arginfo_git_status_byindex,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_status_file,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_status_foreach,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_status_foreach_ext,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_status_list_entrycount,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_status_list_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_status_list_get_perfdata,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_status_list_new,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_status_should_ignore,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reflog_append,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reflog_delete,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reflog_drop,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reflog_entry_byindex,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reflog_entrycount,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reflog_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reflog_read,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reflog_rename,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reflog_write,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_refdb_backend_fs,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_refdb_compress,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_refdb_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_refdb_new,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_refdb_open,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_refdb_set_backend,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_patch_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_patch_from_blob_and_buffer,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_patch_from_blobs,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_patch_from_buffers,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_patch_from_diff,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_patch_get_delta,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_patch_get_hunk,0,0,3)
    ZEND_ARG_INFO(1,lines_in_hunk)
    ZEND_ARG_INFO(0,patch)
    ZEND_ARG_INFO(0,hunk_idx)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_patch_get_line_in_hunk,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_patch_line_stats,0,0,3)
    ZEND_ARG_INFO(1,total_additions)
    ZEND_ARG_INFO(1,total_deletions)
    ZEND_ARG_INFO(0,patch)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_patch_num_hunks,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_patch_num_lines_in_hunk,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_patch_print,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_patch_size,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_patch_to_buf,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_rebase_abort,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_rebase_commit,0,0,6)
    ZEND_ARG_INFO(1,id)
    ZEND_ARG_INFO(0,rebase)
    ZEND_ARG_INFO(0,author)
    ZEND_ARG_INFO(0,committer)
    ZEND_ARG_INFO(0,message_encoding)
    ZEND_ARG_INFO(0,message)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_rebase_finish,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_rebase_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_rebase_init,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_rebase_inmemory_index,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_rebase_next,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_rebase_open,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_rebase_operation_byindex,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_rebase_operation_current,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_rebase_operation_entrycount,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reset,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reset_default,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reset_from_annotated,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_revert,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_revert_commit,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_stash_apply,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_stash_drop,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_stash_foreach,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_stash_pop,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_stash_save,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_add_fetch,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_add_push,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_autotag,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_connect,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_connected,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_create,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_create_anonymous,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_create_with_fetchspec,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_default_branch,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_delete,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_disconnect,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_download,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_dup,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_fetch,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_get_fetch_refspecs,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_get_push_refspecs,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_get_refspec,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_is_valid_name,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_list,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_lookup,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_ls,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_name,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_owner,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_prune,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_prune_refs,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_push,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_pushurl,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_refspec_count,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_rename,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_set_autotag,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_set_pushurl,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_set_url,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_stats,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_stop,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_update_tips,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_upload,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_remote_url,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_refspec_direction,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_refspec_dst,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_refspec_dst_matches,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_refspec_force,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_refspec_rtransform,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_refspec_src,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_refspec_src_matches,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_refspec_string,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_refspec_transform,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_add_finalize,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_add_setup,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_add_to_index,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_branch,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_fetch_recurse_submodules,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_foreach,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_head_id,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_ignore,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_index_id,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_init,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_location,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_lookup,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_name,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_open,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_owner,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_path,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_reload,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_repo_init,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_resolve_url,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_set_branch,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_set_fetch_recurse_submodules,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_set_ignore,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_set_update,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_set_url,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_status,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_sync,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_update,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_update_strategy,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_url,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_submodule_wd_id,{})
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
