<?php

/**
 * @param resource $repo
 * @param resource $target
 *
 * @return resource
 */
function git_branch_create($repo,string $branch_name,$target,bool $force) {}

/**
 * @param resource $repo
 * @param resource $target
 *
 * @return resource
 */
function git_branch_create_from_annotated($repo,string $branch_name,$target,bool $force) {}

/**
 * @param resource $branch
 */
function git_branch_delete($branch) : void {}

/**
 * @param resource $branch
 */
function git_branch_is_head($branch) : bool {}

/**
 * @param resource $iter
 */
function git_branch_iterator_free($iter) : void {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_branch_iterator_new($repo,int $flags) {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_branch_lookup($repo,string $branch_name,int $branch_type) {}

/**
 * @param resource $branch
 *
 * @return resource
 */
function git_branch_move($branch,string $new_branch_name,bool $force) {}

/**
 * @param resource $branch
 */
function git_branch_name($branch) : string {}

/**
 * @param resource $iter
 *
 * @return resource|bool
 */
function git_branch_next(int &$type,$iter) {}

/**
 * @param resource $branch
 */
function git_branch_set_upstream($branch,?string $branch_name) : void {}

/**
 * @param resource $branch
 *
 * @return resource|bool
 */
function git_branch_upstream($branch) {}

/**
 * @param resource $repo
 */
function git_branch_upstream_name($repo,string $refname) : string|bool {}

/**
 * @param resource $repo
 */
function git_branch_upstream_remote($repo,string $refname) : string {}
