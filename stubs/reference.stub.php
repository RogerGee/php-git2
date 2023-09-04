<?php

/**
 * @return resource
 */
function git_reference__alloc(string $name,string $oid,?string $peel) {}

/**
 * @return resource
 */
function git_reference__alloc_symbolic(string $name,string $target) {}

/**
 * @param resource $ref1
 * @param resource $ref2
 */
function git_reference_cmp($ref1,$ref2) : int {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_reference_create($repo,string $name,string $id,bool $force,string $log_message) {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_reference_create_matching($repo,string $name,string $id,bool $force,string $current_id,string $log_message) {}

/**
 * @param resource $ref
 */
function git_reference_delete($ref) : void {}

/**
 * @param resource $ref
 *
 * @return resource
 */
function git_reference_dup($ref) {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_reference_dwim($repo,string $shorthand) {}

/**
 * @param resource $repo
 */
function git_reference_ensure_log($repo,string $refname) : void {}

/**
 * @param resource $repo
 */
function git_reference_foreach($repo,callable $callback,mixed $payload) : void {}

/**
 * @param resource $repo
 */
function git_reference_foreach_glob($repo,string $glob,callable $callback,mixed $payload) : void {}

/**
 * @param resource $repo
 */
function git_reference_foreach_name($repo,callable $callback,mixed $payload) : void {}

/**
 * @param resource $ref
 */
function git_reference_free($ref) : void {}

/**
 * @param resource $repo
 */
function git_reference_has_log($repo,string $refname) : bool {}

/**
 * @param resource $ref
 */
function git_reference_is_branch($ref) : bool {}

/**
 * @param resource $ref
 */
function git_reference_is_note($ref) : bool {}

/**
 * @param resource $ref
 */
function git_reference_is_remote($ref) : bool {}

/**
 * @param resource $ref
 */
function git_reference_is_tag($ref) : bool {}

function git_reference_is_valid_name(string $refname) : bool {}

/**
 * @param resource $iter
 */
function git_reference_iterator_free($iter) : void {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_reference_iterator_glob_new($repo,string $glob) {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_reference_iterator_new($repo) {}

/**
 * @param resource $repo
 */
function git_reference_list($repo) : array {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_reference_lookup($repo,string $name) {}

/**
 * @param resource $ref
 */
function git_reference_name($ref) : string {}

/**
 * @param resource $repo
 */
function git_reference_name_to_id($repo,string $name) : string {}

/**
 * @param resource $iter
 *
 * @return resource|bool
 */
function git_reference_next($iter) {}

/**
 * @param resource $iter
 */
function git_reference_next_name($iter) : string|bool {}

function git_reference_normalize_name(string $name,int $flags) : string {}

/**
 * @param resource $ref
 *
 * @return resource
 */
function git_reference_owner($ref) {}

/**
 * @param resource $ref
 *
 * @return resource
 */
function git_reference_peel($ref,int $type) {}

/**
 * @param resource $repo
 */
function git_reference_remove($repo,string $name) : void {}

/**
 * @param resource $ref
 *
 * @return resource
 */
function git_reference_rename($ref,string $new_name,bool $force,string $log_message) {}

/**
 * @param resource $ref
 *
 * @return resource
 */
function git_reference_resolve($ref) {}

/**
 * @param resource $ref
 *
 * @return resource
 */
function git_reference_set_target($ref,string $id,string $log_message) {}

/**
 * @param resource $ref
 */
function git_reference_shorthand($ref) : string {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_reference_symbolic_create($repo,string $name,string $target,bool $force,string $log_message) {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_reference_symbolic_create_matching($repo,string $name,string $target,bool $force,string $current_value,string $log_message) {}

/**
 * @param resource $ref
 *
 * @return resource
 */
function git_reference_symbolic_set_target($ref,string $target,string $log_message) {}

/**
 * @param resource $ref
 */
function git_reference_symbolic_target($ref) : ?string {}

/**
 * @param resource $ref
 */
function git_reference_target($ref) : ?string {}

/**
 * @param resource $ref
 */
function git_reference_target_peel($ref) : ?string {}

/**
 * @param resource $ref
 */
function git_reference_type($ref) : int {}
