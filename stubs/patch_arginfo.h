/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 2c0bccd3243757b5b8f60107a0027c43b0612552 */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_patch_free, 0, 1, IS_VOID, 0)
	ZEND_ARG_INFO(0, patch)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_patch_from_blob_and_buffer, 0, 0, 5)
	ZEND_ARG_INFO(0, old_blob)
	ZEND_ARG_TYPE_INFO(0, old_as_path, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, buffer, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, buffer_as_path, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_patch_from_blobs, 0, 0, 5)
	ZEND_ARG_INFO(0, old_blob)
	ZEND_ARG_TYPE_INFO(0, old_as_path, IS_STRING, 1)
	ZEND_ARG_INFO(0, new_blob)
	ZEND_ARG_TYPE_INFO(0, new_as_path, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_patch_from_buffers, 0, 0, 5)
	ZEND_ARG_TYPE_INFO(0, old_buffer, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, old_as_path, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, new_buffer, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, new_as_path, IS_STRING, 1)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_git_patch_from_diff, 0, 0, 2)
	ZEND_ARG_INFO(0, diff)
	ZEND_ARG_TYPE_INFO(0, idx, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_patch_get_delta, 0, 1, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, patch)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_patch_get_hunk, 0, 3, IS_ARRAY, 0)
	ZEND_ARG_TYPE_INFO(1, lines_in_hunk, IS_LONG, 0)
	ZEND_ARG_INFO(0, patch)
	ZEND_ARG_TYPE_INFO(0, hunk_idx, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_patch_get_line_in_hunk, 0, 3, IS_ARRAY, 0)
	ZEND_ARG_INFO(0, patch)
	ZEND_ARG_TYPE_INFO(0, hunk_idx, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, line_of_hunk, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_patch_line_stats, 0, 3, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(1, total_additions, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(1, total_deletions, IS_LONG, 0)
	ZEND_ARG_INFO(0, patch)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_patch_num_hunks, 0, 1, IS_LONG, 0)
	ZEND_ARG_INFO(0, patch)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_MASK_EX(arginfo_git_patch_num_lines_in_hunk, 0, 2, MAY_BE_LONG|MAY_BE_BOOL)
	ZEND_ARG_INFO(0, patch)
	ZEND_ARG_TYPE_INFO(0, hunk_idx, IS_LONG, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_patch_print, 0, 3, IS_VOID, 0)
	ZEND_ARG_INFO(0, patch)
	ZEND_ARG_TYPE_INFO(0, callback, IS_CALLABLE, 0)
	ZEND_ARG_TYPE_INFO(0, payload, IS_MIXED, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_patch_size, 0, 4, IS_LONG, 0)
	ZEND_ARG_INFO(0, patch)
	ZEND_ARG_TYPE_INFO(0, include_context, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, include_hunk_headers, _IS_BOOL, 0)
	ZEND_ARG_TYPE_INFO(0, include_file_headers, _IS_BOOL, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_git_patch_to_buf, 0, 1, IS_STRING, 0)
	ZEND_ARG_INFO(0, patch)
ZEND_END_ARG_INFO()
