ZEND_BEGIN_ARG_INFO(arginfo_git2_version,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_libgit2_version,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_libgit2_prerelease,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_libgit2_features,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_repository_ident,0,0,2)
    ZEND_ARG_INFO(1,email)
    ZEND_ARG_INFO(0,repo)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_init,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_new,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_open,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_open_bare,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository__cleanup,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_detach_head,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_discover,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_get_namespace,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_path,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_hashfile,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_odb,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_set_odb,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_set_head,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_set_head_detached,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_head,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_is_empty,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_is_bare,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_is_shallow,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_head_detached,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_head_unborn,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_init_ext,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_message,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_message_remove,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_open_ext,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_reinit_filesystem,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_set_bare,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_set_head_detached_from_annotated,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_set_ident,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_set_namespace,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_set_workdir,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_state,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_state_cleanup,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_workdir,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_wrap_odb,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_config,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_config_snapshot,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_set_config,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_index,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_set_index,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_refdb,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_set_refdb,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_fetchhead_foreach,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_repository_mergehead_foreach,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_list,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_lookup,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_peel,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_name_to_id,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_create,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_create_matching,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_target,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_target_peel,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_name,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_cmp,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference__alloc,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference__alloc_symbolic,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_delete,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_dup,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_dwim,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_ensure_log,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_has_log,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_is_branch,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_is_note,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_is_remote,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_is_tag,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_is_valid_name,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_iterator_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_iterator_new,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_iterator_glob_new,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_next,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_next_name,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_normalize_name,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_owner,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_remove,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_rename,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_resolve,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_set_target,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_shorthand,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_type,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_foreach,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_foreach_name,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_foreach_glob,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_symbolic_create,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_symbolic_create_matching,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_symbolic_set_target,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_reference_symbolic_target,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_object_id,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_object_lookup,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_object_peel,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_object_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_object_dup,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_object_lookup_bypath,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_object_lookup_prefix,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_object_owner,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_object_short_id,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_object_string2type,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_object_type,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_object_type2string,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_object_typeisloose,{})
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

ZEND_BEGIN_ARG_INFO(arginfo_git_indexer_new,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_indexer_hash,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_indexer_append,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_indexer_commit,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_indexer_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git2_indexer_stats,{})
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

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_commit_extract_signature,0,0,4)
    ZEND_ARG_INFO(1,signed_data_out)
    ZEND_ARG_INFO(0,repo)
    ZEND_ARG_INFO(0,commit_id)
    ZEND_ARG_INFO_WITH_DEFAULT_VALUE(0,field,"null")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_lookup,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_lookup_prefix,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_parentcount,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_parent_id,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_parent,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_nth_gen_ancestor,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_id,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_owner,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_tree,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_tree_id,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_time,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_time_offset,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_summary,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_raw_header,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_body,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_message,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_message_encoding,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_message_raw,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_author,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_committer,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_create,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_create_buffer,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_create_with_signature,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_create_from_callback,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_create_from_ids,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_amend,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_dup,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_commit_header_field,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_blob_create_frombuffer,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_blob_create_fromdisk,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_blob_create_fromworkdir,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_blob_filtered_content,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_blob_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_blob_id,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_blob_is_binary,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_blob_lookup,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_blob_lookup_prefix,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_blob_owner,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_blob_rawcontent,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_blob_rawsize,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_blob_dup,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_blob_create_fromstream,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_blob_create_fromstream_commit,{})
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

ZEND_BEGIN_ARG_INFO(arginfo_git_blame_file,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_blame_buffer,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_blame_get_hunk_byindex,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_blame_get_hunk_byline,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_blame_get_hunk_count,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_blame_free,{})
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

ZEND_BEGIN_ARG_INFO(arginfo_git_branch_create,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_branch_create_from_annotated,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_branch_delete,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_branch_is_head,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_branch_iterator_new,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_branch_iterator_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_branch_lookup,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_branch_move,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_branch_name,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_branch_next,0,0,2)
    ZEND_ARG_INFO(1,out_type)
    ZEND_ARG_INFO(0,iter)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_branch_set_upstream,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_branch_upstream,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_branch_upstream_name,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_branch_upstream_remote,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_new,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_open_default,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_open_global,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_open_level,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_open_ondisk,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_parse_bool,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_parse_int32,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_parse_int64,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_parse_path,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_set_bool,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_set_int32,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_set_int64,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_set_multivar,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_set_string,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_get_bool,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_get_entry,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_get_int32,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_get_int64,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_get_path,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_get_string,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_get_string_buf,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_delete_entry,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_delete_multivar,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_find_global,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_find_programdata,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_find_system,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_find_xdg,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_snapshot,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_add_file_ondisk,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_foreach,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_foreach_match,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_get_multivar_foreach,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_get_mapped,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_add_backend,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_backend_foreach_match,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_iterator_new,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_iterator_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_multivar_iterator_new,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_iterator_glob_new,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_next,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_config_lookup_map_value,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_checkout_head,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_checkout_tree,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_checkout_index,{})
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

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_blob_to_buffer,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_blobs,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_buffers,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_commit_as_email,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_from_buffer,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_print,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_get_delta,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_get_perfdata,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_is_sorted_icase,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_foreach,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_merge,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_to_buf,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_tree_to_tree,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_tree_to_workdir,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_tree_to_workdir_with_index,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_status_char,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_num_deltas,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_num_deltas_of_type,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_find_similar,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_get_stats,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_stats_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_stats_deletions,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_stats_files_changed,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_stats_insertions,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_stats_to_buf,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_format_email,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_index_to_index,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_tree_to_index,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_diff_index_to_workdir,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_add,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_add_all,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_add_bypath,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_add_frombuffer,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_caps,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_checksum,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_clear,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_conflict_add,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_conflict_cleanup,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_index_conflict_get,0,0,5)
    ZEND_ARG_INFO(1,ancestor)
    ZEND_ARG_INFO(1,ours)
    ZEND_ARG_INFO(1,theirs)
    ZEND_ARG_INFO(0,index)
    ZEND_ARG_INFO(0,path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_conflict_iterator_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_conflict_iterator_new,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_index_conflict_next,0,0,4)
    ZEND_ARG_INFO(1,ancestor)
    ZEND_ARG_INFO(1,ours)
    ZEND_ARG_INFO(1,theirs)
    ZEND_ARG_INFO(0,iterator)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_conflict_remove,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_entrycount,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_entry_is_conflict,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_entry_stage,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_find,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_find_prefix,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_get_byindex,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_get_bypath,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_has_conflicts,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_new,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_open,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_owner,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_path,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_read,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_read_tree,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_remove,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_remove_all,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_remove_bypath,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_remove_directory,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_set_caps,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_set_version,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_update_all,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_version,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_write,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_write_tree,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_index_write_tree_to,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_trace_set,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_ignore_add_rule,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_ignore_clear_internal_rules,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_ignore_path_is_ignored,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_attr_add_macro,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_attr_cache_flush,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_attr_foreach,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_attr_get,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_attr_value,{})
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

ZEND_BEGIN_ARG_INFO(arginfo_git_cherrypick,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_cherrypick_commit,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_merge,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_merge_analysis,0,0,3)
    ZEND_ARG_INFO(1,preference)
    ZEND_ARG_INFO(0,repo)
    ZEND_ARG_INFO(0,their_heads)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_merge_base,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_merge_base_many,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_merge_base_octopus,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_merge_bases,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_merge_bases_many,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_merge_commits,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_merge_file,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_merge_file_from_index,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_merge_trees,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_note_author,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_note_committer,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_note_create,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_note_foreach,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_note_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_note_id,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_note_iterator_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_note_iterator_new,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_note_message,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_note_next,0,0,2)
    ZEND_ARG_INFO(1,annotated_id)
    ZEND_ARG_INFO(0,iterator)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_note_read,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_note_remove,{})
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

ZEND_BEGIN_ARG_INFO(arginfo_git_describe_commit,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_describe_format,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_describe_result_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_describe_workdir,{})
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

ZEND_BEGIN_ARG_INFO(arginfo_git_cred_default_new,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_cred_free,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_cred_has_username,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_cred_ssh_key_from_agent,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_cred_ssh_key_memory_new,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_cred_ssh_key_new,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_cred_username_new,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_cred_userpass_plaintext_new,{})
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_git_clone,{})
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
