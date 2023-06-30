<?php

/**
 * @param resource $commit_to_amend
 * @param resource $author
 * @param resource $committer
 * @param resource $tree
 */
function git_commit_amend($commit_to_amend,?string $update_ref,$author,$committer,?string $message_encoding,?string $message,$tree) : string {}

/**
 * @param resource $commit
 *
 * @return resource
 */
function git_commit_author($commit) {}

/**
 * @param resource $commit
 */
function git_commit_body($commit) : string {}

/**
 * @param resource $commit
 *
 * @return resource
 */
function git_commit_committer($commit) {}

/**
 * @param resource $repo
 * @param resource $author
 * @param resource $committer
 * @param resource $tree
 */
function git_commit_create($repo,?string $update_ref,$author,$committer,?string $message_encoding,string $message,$tree,array $parents) : string {}

/**
 * @param resource $repo
 * @param resource $author
 * @param resource $committer
 * @param resource $tree
 */
function git_commit_create_buffer($repo,$author,$committer,?string $message_encoding,string $message,$tree,array $parents) : string {}

/**
 * @param resource $repo
 * @param resource $author
 * @param resource $committer
 */
function git_commit_create_from_callback($repo,?string $update_ref,$author,$committer,?string $message_encoding,string $message,string $tree_id,callable $callback,mixed $payload) : string {}

/**
 * @param resource $repo
 * @param resource $author
 * @param resource $committer
 */
function git_commit_create_from_ids($repo,?string $update_ref,$author,$committer,?string $message_encoding,string $message,string $tree_id,array $parents) : string {}

/**
 * @param resource $repo
 */
function git_commit_create_with_signature($repo,string $commit_content,string $signature,?string $signature_field = null) : string {}

/**
 * @param resource $source
 *
 * @return resource
 */
function git_commit_dup($source) {}

/**
 * @param resource $repo
 */
function git_commit_extract_signature(string &$signed_data_out,$repo,string $commit_id,string $field = null) : string {}

/**
 * @param resource $commit
 */
function git_commit_free($commit) : void {}

/**
 * @param resource $commit
 */
function git_commit_header_field($commit,string $field) : string {}

/**
 * @param resource $commit
 */
function git_commit_id($commit) : string {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_commit_lookup($repo,string $id) {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_commit_lookup_prefix($repo,string $id) {}

/**
 * @param resource $commit
 */
function git_commit_message($commit) : string {}

/**
 * @param resource $commit
 */
function git_commit_message_encoding($commit) : ?string {}

/**
 * @param resource $commit
 */
function git_commit_message_raw($commit) : string {}

/**
 * @param resource $commit
 *
 * @return resource
 */
function git_commit_nth_gen_ancestor($commit,int $n) {}

/**
 * @param resource $commit
 *
 * @return resource
 */
function git_commit_owner($commit) {}

/**
 * @param resource $commit
 *
 * @return resource
 */
function git_commit_parent($commit,int $n) {}

/**
 * @param resource $commit
 */
function git_commit_parent_id($commit,int $n) : string {}

/**
 * @param resource $commit
 */
function git_commit_parentcount($commit) : int {}

/**
 * @param resource $commit
 */
function git_commit_raw_header($commit) : string {}

/**
 * @param resource $commit
 */
function git_commit_summary($commit) : string {}

/**
 * @param resource $commit
 */
function git_commit_time($commit) : int {}

/**
 * @param resource $commit
 */
function git_commit_time_offset($commit) : int {}

/**
 * @param resource $commit
 *
 * @return resource
 */
function git_commit_tree($commit) {}

/**
 * @param resource $commit
 */
function git_commit_tree_id($commit) : string {}
