<?php

/**
 * @param resource $index
 */
function git_index_add($index,array $source_entry) : void {}

/**
 * @param resource $index
 */
function git_index_add_all($index,array $pathspec,int $flags,callable $callback,mixed $paylaod) : bool {}

/**
 * @param resource $index
 */
function git_index_add_bypath($index,string $path) : void {}

/**
 * @param resource $index
 */
function git_index_add_from_buffer($index,array $entry,string $buffer) : void {}

/**
 * @param resource $index
 */
function git_index_caps($index) : int {}

/**
 * @param resource $index
 */
function git_index_checksum($index) : string {}

/**
 * @param resource $index
 */
function git_index_clear($index) : void {}

/**
 * @param resource $index
 */
function git_index_conflict_add($index,array $ancestor_entry,array $our_entry,array $their_entry) : void {}

/**
 * @param resource $index
 */
function git_index_conflict_cleanup($index) : void {}

/**
 * @param resource $index
 */
function git_index_conflict_get(array &$ancestor,array &$ours,array &$theirs,$index,string $path) : void {}

/**
 * @param resource $iter
 */
function git_index_conflict_iterator_free($iter) : void {}

/**
 * @param resource $index
 *
 * @return resource
 */
function git_index_conflict_iterator_new($index) {}

/**
 * @param resource $iterator
 */
function git_index_conflict_next(array &$ancestor,array &$ours,array &$theirs,$iterator) : bool {}

/**
 * @param resource $index
 */
function git_index_conflict_remove($index,string $path) : void {}

/**
 * @param resource $index
 */
function git_index_entrycount($index) : int {}

function git_index_entry_is_conflict(array $entry) : bool {}

function git_index_entry_stage(array $entry) : int {}

/**
 * @param resource $index
 */
function git_index_find($index,string $path) : int {}

/**
 * @param resource $index
 */
function git_index_find_prefix($index,string $prefix) : int {}

/**
 * @param resource $index
 */
function git_index_free($index) : void {}

/**
 * @param resource $index
 */
function git_index_get_byindex($index,int $n) : array|bool {}

/**
 * @param resource $index
 */
function git_index_get_bypath($index,string $path,int $stage) : array|bool {}

/**
 * @param resource $index
 */
function git_index_has_conflicts($index) : bool {}

/**
 * @return resource
 */
function git_index_new() {}

/**
 * @return resource
 */
function git_index_open(string $index_path) {}

/**
 * @param resource $index
 *
 * @return resource
 */
function git_index_owner($index) {}

/**
 * @param resource $index
 */
function git_index_path($index) : ?string {}

/**
 * @param resource $index
 */
function git_index_read($index,bool $force) : void {}

/**
 * @param resource $index
 * @param resource $tree
 */
function git_index_read_tree($index,$tree) : void {}

/**
 * @param resource $index
 */
function git_index_remove($index,string $path,int $stage) : void {}

/**
 * @param resource $index
 */
function git_index_remove_all($index,array $pathspec,callable $callback,mixed $payload) : bool {}

/**
 * @param resource $index
 */
function git_index_remove_bypath($index,string $path) : void {}

/**
 * @param resource $index
 */
function git_index_remove_directory($index,string $dir,int $stage) : void {}

/**
 * @param resource $index
 */
function git_index_set_caps($index,int $caps) : void {}

/**
 * @param resource $index
 */
function git_index_set_version($index,int $version) : void {}

/**
 * @param resource $index
 */
function git_index_update_all($index,array $pathspec,callable $callback,mixed $payload) : bool {}

/**
 * @param resource $index
 */
function git_index_version($index) : int {}

/**
 * @param resource $index
 */
function git_index_write($index) : void {}

/**
 * @param resource $index
 */
function git_index_write_tree($index) : string {}

/**
 * @param resource $index
 * @param resource $repo
 */
function git_index_write_tree_to($index,$repo) : string {}
