/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 32221f625fb53cfd2672a485143365a7dd804925 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_diff_blob_to_buffer, 0, 10, IS_VOID, 0)
	ZEND_ARG_INFO(0, old_blob)
	ZEND_ARG_TYPE_INFO(0, old_as_path, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, buffer, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, buffer_as_path, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, file_callback, IS_CALLABLE, 1)
	ZEND_ARG_TYPE_INFO(0, binary_callback, IS_CALLABLE, 1)
	ZEND_ARG_TYPE_INFO(0, hunk_callback, IS_CALLABLE, 1)
	ZEND_ARG_TYPE_INFO(0, line_callback, IS_CALLABLE, 1)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_diff_blobs, 0, 10, IS_VOID, 0)
	ZEND_ARG_INFO(0, old_blob)
	ZEND_ARG_TYPE_INFO(0, old_as_path, IS_STRING, 1)
	ZEND_ARG_INFO(0, new_blob)
	ZEND_ARG_TYPE_INFO(0, new_as_path, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, file_callback, IS_CALLABLE, 1)
	ZEND_ARG_TYPE_INFO(0, binary_callback, IS_CALLABLE, 1)
	ZEND_ARG_TYPE_INFO(0, hunk_callback, IS_CALLABLE, 1)
	ZEND_ARG_TYPE_INFO(0, line_callback, IS_CALLABLE, 1)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_diff_buffers, 0, 10, IS_VOID, 0)
	ZEND_ARG_TYPE_INFO(0, old_buffer, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, old_as_path, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, new_buffer, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, new_as_path, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
	ZEND_ARG_TYPE_INFO(0, file_callback, IS_CALLABLE, 1)
	ZEND_ARG_TYPE_INFO(0, binary_callback, IS_CALLABLE, 1)
	ZEND_ARG_TYPE_INFO(0, hunk_callback, IS_CALLABLE, 1)
	ZEND_ARG_TYPE_INFO(0, line_callback, IS_CALLABLE, 1)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_diff_commit_as_email, 0, 6, IS_STRING, 0)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_INFO(0, commit)
	ZEND_ARG_TYPE_INFO(0, patch_no, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, total_patches, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_diff_find_similar, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, diff)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_diff_foreach, 0, 6, IS_VOID, 0)
	ZEND_ARG_INFO(0, diff)
	ZEND_ARG_TYPE_INFO(0, file_callback, IS_CALLABLE, 1)
	ZEND_ARG_TYPE_INFO(0, binary_callback, IS_CALLABLE, 1)
	ZEND_ARG_TYPE_INFO(0, hunk_callback, IS_CALLABLE, 1)
	ZEND_ARG_TYPE_INFO(0, line_callback, IS_CALLABLE, 1)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_diff_format_email, 0, 2, IS_STRING, 0)
	ZEND_ARG_INFO(0, diff)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_diff_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, diff)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_diff_from_buffer, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, buffer, IS_STRING, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_diff_get_delta, 0, 2, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, diff)
	ZEND_ARG_TYPE_INFO(0, idx, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_diff_get_perfdata, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, diff)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_diff_get_stats, 0, 0, 1)
	ZEND_ARG_INFO(0, diff)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_diff_index_to_index, 0, 0, 4)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_INFO(0, old_index)
	ZEND_ARG_INFO(0, new_index)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_diff_index_to_workdir, 0, 0, 3)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_diff_is_sorted_icase, 0, 1, _IS_BOOL, 0)
	ZEND_ARG_INFO(0, diff)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_diff_merge, 0, 2, IS_VOID, 0)
	ZEND_ARG_INFO(0, diff_onto)
	ZEND_ARG_INFO(0, diff_from)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_diff_num_deltas, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, diff)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_diff_num_deltas_of_type, 0, 2, IS_LONG, 0)
	ZEND_ARG_INFO(0, diff)
	ZEND_ARG_TYPE_INFO(0, type, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_diff_print, 0, 4, IS_VOID, 0)
	ZEND_ARG_INFO(0, diff)
	ZEND_ARG_TYPE_INFO(0, format, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_diff_stats_deletions, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, stats)
ZEND_END_ARG_INFO()

#define arginfo_git_diff_stats_files_changed arginfo_git_diff_stats_deletions

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_diff_stats_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, stats)
ZEND_END_ARG_INFO()

#define arginfo_git_diff_stats_insertions arginfo_git_diff_stats_deletions

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_diff_stats_to_buf, 0, 3, IS_STRING, 0)
	ZEND_ARG_INFO(0, stats)
	ZEND_ARG_TYPE_INFO(0, format, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, width, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_diff_status_char, 0, 1, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, status, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_diff_to_buf, 0, 2, IS_STRING, 0)
	ZEND_ARG_INFO(0, diff)
	ZEND_ARG_TYPE_INFO(0, format, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_diff_tree_to_index, 0, 0, 4)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_INFO(0, old_tree)
	ZEND_ARG_INFO(0, index)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_diff_tree_to_tree, 0, 0, 4)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_INFO(0, old_tree)
	ZEND_ARG_INFO(0, new_tree)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_diff_tree_to_workdir, 0, 0, 3)
	ZEND_ARG_INFO(0, repo)
	ZEND_ARG_INFO(0, old_tree)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

#define arginfo_git_diff_tree_to_workdir_with_index arginfo_git_diff_tree_to_workdir
