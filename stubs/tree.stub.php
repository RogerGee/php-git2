<?php

/**
 * @param resource $tree
 *
 * @return resource
 */
function git_tree_dup($tree) {}

/**
 * @param resource $tree
 *
 * @return resource
 */
function git_tree_entry_byid($tree,string $id) {}

/**
 * @param resource $tree
 *
 * @return resource
 */
function git_tree_entry_byindex($tree,int $idx) {}

/**
 * @param resource $tree
 *
 * @return resource
 */
function git_tree_entry_byname($tree,string $filename) {}

/**
 * @param resource $tree
 *
 * @return resource
 */
function git_tree_entry_bypath($tree,string $path) {}

/**
 * @param resource $tree_entry_1
 * @param resource $tree_entry_2
 */
function git_tree_entry_cmp($tree_entry_1,$tree_entry_2) : int {}

/**
 * @param resource $tree_entry
 *
 * @return resource
 */
function git_tree_entry_dup($tree_entry) {}

/**
 * @param resource $tree_entry
 */
function git_tree_entry_filemode($tree_entry) : int {}

/**
 * @param resource $tree_entry
 */
function git_tree_entry_filemode_raw($tree_entry) : int {}

/**
 * @param resource $tree_entry
 */
function git_tree_entry_free($tree_entry) : void {}

/**
 * @param resource $tree_entry
 */
function git_tree_entry_id($tree_entry) : string {}

/**
 * @param resource $tree_entry
 */
function git_tree_entry_name($tree_entry) : string {}

/**
 * @param resource $repo
 * @param resource $tree_entry
 *
 * @return resource
 */
function git_tree_entry_to_object($repo,$tree_entry) {}

/**
 * @param resource $tree_entry
 */
function git_tree_entry_type($tree_entry) : int {}

/**
 * @param resource $tree
 */
function git_tree_entrycount($tree) : int {}

/**
 * @param resource $tree
 */
function git_tree_free($tree) : void {}

/**
 * @param resource $tree
 */
function git_tree_id($tree) : string {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_tree_lookup($repo,string $id) {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_tree_lookup_prefix($repo,string $id_prefix) {}

/**
 * @param resource $tree
 *
 * @return resource
 */
function git_tree_owner($tree) {}

/**
 * @param resource $tree
 */
function git_tree_walk($tree,int $mode,callable $callback,mixed $payload) : void {}
