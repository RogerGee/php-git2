<?php

/**
 * @param resource $patch
 */
function git_patch_free($patch) : void {}

/**
 * @param resource $old_blob
 *
 * @return resource
 */
function git_patch_from_blob_and_buffer($old_blob,?string $old_as_path,?string $buffer,?string $buffer_as_path,?array $options) {}

/**
 * @param resource $old_blob
 * @param resource $new_blob
 *
 * @return resource
 */
function git_patch_from_blobs($old_blob,?string $old_as_path,$new_blob,?string $new_as_path,?array $options) {}

/**
 * @return resource
 */
function git_patch_from_buffers(?string $old_buffer,?string $old_as_path,?string $new_buffer,?string $new_as_path,?array $options) {}

/**
 * @param resource $diff
 *
 * @return resource
 */
function git_patch_from_diff($diff,int $idx) {}

/**
 * @param resource $patch
 */
function git_patch_get_delta($patch) : array {}

/**
 * @param resource $patch
 */
function git_patch_get_hunk(int &$lines_in_hunk,$patch,int $hunk_idx) : array {}

/**
 * @param resource $patch
 */
function git_patch_get_line_in_hunk($patch,int $hunk_idx,int $line_of_hunk) : array {}

/**
 * @param resource $patch
 */
function git_patch_line_stats(int &$total_additions,int &$total_deletions,$patch) : int {}

/**
 * @param resource $patch
 */
function git_patch_num_hunks($patch) : int {}

/**
 * @param resource $patch
 */
function git_patch_num_lines_in_hunk($patch,int $hunk_idx) : int|bool {}

/**
 * @param resource $patch
 */
function git_patch_print($patch,callable $callback,mixed $payload) : void {}

/**
 * @param resource $patch
 */
function git_patch_size($patch,bool $include_context,bool $include_hunk_headers,bool $include_file_headers) : int {}

/**
 * @param resource $patch
 */
function git_patch_to_buf($patch) : string {}
