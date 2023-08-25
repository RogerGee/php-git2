<?php

/**
 * @param resource $repo
 */
function git_merge($repo,array $their_heads,?array $merge_options,?array $checkout_options) : void {}

/**
 * @param resource $repo
 */
function git_merge_analysis(int &$preference,$repo,array $their_heads) : int {}

/**
 * @param resource $repo
 */
function git_merge_base($repo,string $one,string $two) : string {}

/**
 * @param resource $repo
 */
function git_merge_base_many($repo,array $input_array) : string {}

/**
 * @param resource $repo
 */
function git_merge_base_octopus($repo,array $input_array) : string {}

/**
 * @param resource $repo
 */
function git_merge_bases($repo,string $one,string $two) : array {}

/**
 * @param resource $repo
 */
function git_merge_bases_many($repo,array $input_array) : array {}

/**
 * @param resource $repo
 * @param resource $our_commit
 * @param resource $their_commit
 *
 * @return resource
 */
function git_merge_commits($repo,$our_commit,$their_commit,?array $merge_options) {}

function git_merge_file(array $ancestor,array $ours,array $theirs,?array $merge_file_options) : array {}

/**
 * @param resource $repo
 */
function git_merge_file_from_index($repo,?array $ancestor,array $ours,array $theirs,?array $merge_file_options) : array {}

/**
 * @param resource $repo
 * @param resource $ancestor_tree
 * @param resource $our_tree
 * @param resource $their_tree
 *
 * @return resource
 */
function git_merge_trees($repo,$ancestor_tree,$our_tree,$their_tree,?array $merge_options) {}
