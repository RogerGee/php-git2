<?php

/**
 * @param resource $revwalk
 */
function git_revwalk_add_hide_cb($revwalk,callable $callback,mixed $payload) : void {}

/**
 * @param resource $revwalk
 */
function git_revwalk_free($revwalk) : void {}

/**
 * @param resource $revwalk
 */
function git_revwalk_hide($revwalk,string $commit_id) : void {}

/**
 * @param resource $revwalk
 */
function git_revwalk_hide_glob($revwalk,string $glob) : void {}

/**
 * @param resource $revwalk
 */
function git_revwalk_hide_head($revwalk) : void {}

/**
 * @param resource $revwalk
 */
function git_revwalk_hide_ref($revwalk,string $refname) : void {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_revwalk_new($repo) {}

/**
 * @param resource $revwalk
 */
function git_revwalk_next($revwalk) : string|bool {}

/**
 * @param resource $revwalk
 */
function git_revwalk_push($revwalk,string $id) : void {}

/**
 * @param resource $revwalk
 */
function git_revwalk_push_glob($revwalk,string $glob) : void {}

/**
 * @param resource $revwalk
 */
function git_revwalk_push_head($revwalk) : void {}

/**
 * @param resource $revwalk
 */
function git_revwalk_push_range($revwalk,string $range) : void {}

/**
 * @param resource $revwalk
 */
function git_revwalk_push_ref($revwalk,string $refname) : void {}

/**
 * @param resource $revwalk
 *
 * @return resource
 */
function git_revwalk_repository($revwalk) {}

/**
 * @param resource $revwalk
 */
function git_revwalk_reset($revwalk) : void {}

/**
 * @param resource $revwalk
 */
function git_revwalk_simplify_first_parent($revwalk) : void {}

/**
 * @param resource $revwalk
 */
function git_revwalk_sorting($revwalk,int $sort_mode) : void {}
