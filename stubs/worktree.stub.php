<?php

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_worktree_add($repo,string $name,string $path,?array $opts) {}

/**
 * @param resource $worktree
 */
function git_worktree_free($worktree) : void {}

/**
 * @param resource $worktree
 */
function git_worktree_is_locked(string &$reason,$worktree) : bool {}

/**
 * @param resource $worktree
 */
function git_worktree_is_prunable($worktree,?array $opts) : bool {}

/**
 * @param resource $repo
 */
function git_worktree_list($repo) : array {}

/**
 * @param resource $worktree
 */
function git_worktree_lock($worktree,string $reason) : void {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_worktree_lookup($repo,string $name) {}

/**
 * @param resource $worktree
 */
function git_worktree_name($worktree) : string {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_worktree_open_from_repository($repo) {}

/**
 * @param resource $worktree
 */
function git_worktree_path($worktree) : string {}

/**
 * @param resource $worktree
 */
function git_worktree_prune($worktree,?array $opts) : int {}

/**
 * @param resource $worktree
 */
function git_worktree_unlock($worktree) : int {}

/**
 * @param resource $worktree
 */
function git_worktree_validate($worktree) : void {}
