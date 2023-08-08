<?php

/**
 * @param resource $old_blob
 */
function git_diff_blob_to_buffer($old_blob,?string $old_as_path,?string $buffer,?string $buffer_as_path,?array $options,?callable $file_callback,?callable $binary_callback,?callable $hunk_callback,?callable $line_callback,mixed $payload) : void {}

/**
 * @param resource $old_blob
 * @param resource $new_blob
 */
function git_diff_blobs($old_blob,?string $old_as_path,$new_blob,?string $new_as_path,?array $options,?callable $file_callback,?callable $binary_callback,?callable $hunk_callback,?callable $line_callback,mixed $payload) : void {}

function git_diff_buffers(?string $old_buffer,?string $old_as_path,?string $new_buffer,?string $new_as_path,?array $options,?callable $file_callback,?callable $binary_callback,?callable $hunk_callback,?callable $line_callback,mixed $payload) : void {}

/**
 * @param resource $repo
 * @param resource $commit
 */
function git_diff_commit_as_email($repo,$commit,int $patch_no,int $total_patches,int $flags,?array $options) : string {}

/**
 * @param resource $diff
 */
function git_diff_find_similar($diff,?array $options) : void {}

/**
 * @param resource $diff
 */
function git_diff_foreach($diff,?callable $file_callback,?callable $binary_callback,?callable $hunk_callback,?callable $line_callback,mixed $payload) : void {}

/**
 * @param resource $diff
 */
function git_diff_format_email($diff,?array $options) : string {}

/**
 * @param resource $diff
 */
function git_diff_free($diff) : void {}

/**
 * @return resource
 */
function git_diff_from_buffer(string $buffer) {}

/**
 * @param resource $diff
 */
function git_diff_get_delta($diff,int $idx) : array {}

/**
 * @param resource $diff
 */
function git_diff_get_perfdata($diff) : array {}

/**
 * @param resource $diff
 *
 * @return resource
 */
function git_diff_get_stats($diff) {}

/**
 * @param resource $repo
 * @param resource $old_index
 * @param resource $new_index
 *
 * @return resource
 */
function git_diff_index_to_index($repo,$old_index,$new_index,?array $options) {}

/**
 * @param resource $repo
 * @param resource $index
 *
 * @return resource
 */
function git_diff_index_to_workdir($repo,$index,?array $options) {}

/**
 * @param resource $diff
 */
function git_diff_is_sorted_icase($diff) : bool {}

/**
 * @param resource $diff_onto
 * @param resource $diff_from
 */
function git_diff_merge($diff_onto,$diff_from) : void {}

/**
 * @param resource $diff
 */
function git_diff_num_deltas($diff) : int {}

/**
 * @param resource $diff
 */
function git_diff_num_deltas_of_type($diff,int $type) : int {}

/**
 * @param resource $diff
 */
function git_diff_print($diff,int $format,callable $callback,mixed $payload) : void {}

/**
 * @param resource $stats
 */
function git_diff_stats_deletions($stats) : int {}

/**
 * @param resource $stats
 */
function git_diff_stats_files_changed($stats) : int {}

/**
 * @param resource $stats
 */
function git_diff_stats_free($stats) : void {}

/**
 * @param resource $stats
 */
function git_diff_stats_insertions($stats) : int {}

/**
 * @param resource $stats
 */
function git_diff_stats_to_buf($stats,int $format,int $width) : string {}

function git_diff_status_char(int $status) : string {}

/**
 * @param resource $diff
 */
function git_diff_to_buf($diff,int $format) : string {}

/**
 * @param resource $repo
 * @param resource $old_tree
 * @param resource $index
 *
 * @return resource
 */
function git_diff_tree_to_index($repo,$old_tree,$index,?array $options) {}

/**
 * @param resource $repo
 * @param resource $old_tree
 * @param resource $new_tree
 *
 * @return resource
 */
function git_diff_tree_to_tree($repo,$old_tree,$new_tree,?array $options) {}

/**
 * @param resource $repo
 * @param resource $old_tree
 *
 * @return resource
 */
function git_diff_tree_to_workdir($repo,$old_tree,?array $options) {}

/**
 * @param resource $repo
 * @param resource $old_tree
 *
 * @return resource
 */
function git_diff_tree_to_workdir_with_index($repo,$old_tree,?array $options) {}
