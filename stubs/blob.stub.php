<?php

/**
 * @param resource $repo
 */
function git_blob_create_frombuffer($repo,string $buffer) : string {}

/**
 * @param resource $repo
 */
function git_blob_create_fromdisk($repo,string $path) : string {}

/**
 * @param resource $repo
 */
function git_blob_create_fromstream($repo,?string $hintpath) : \GitWritestream {}

function git_blob_create_fromstream_commit(\GitWritestream $stream) : string {}

/**
 * @param resource $repo
 */
function git_blob_create_fromworkdir($repo,string $relative_path) : string {}

/**
 * @param resource $blob
 *
 * @return resource
 */
function git_blob_dup($blob) {}

/**
 * @param resource $blob
 */
function git_blob_filtered_content($blob,string $as_path,bool $check_for_binary_data) : string {}

/**
 * @param resource $blob
 */
function git_blob_free($blob) : void {}

/**
 * @param resource $blob
 */
function git_blob_id($blob) : string {}

/**
 * @param resource $blob
 */
function git_blob_is_binary($blob) : bool {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_blob_lookup($repo,string $id) {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_blob_lookup_prefix($repo,string $id) {}

/**
 * @param resource $blob
 *
 * @return resource
 */
function git_blob_owner($blob) {}

/**
 * @param resource $blob
 */
function git_blob_rawcontent($blob) : string {}

/**
 * @param resource $blob
 */
function git_blob_rawsize($blob) : int {}
