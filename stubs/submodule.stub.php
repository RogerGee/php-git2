<?php

/**
 * @param resource $submodule
 */
function git_submodule_add_finalize($submodule) : void {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_submodule_add_setup($repo,string $url,string $path,bool $use_gitlink) {}

/**
 * @param resource $submodule
 */
function git_submodule_add_to_index($submodule,bool $write_index) : void {}

/**
 * @param resource $submodule
 */
function git_submodule_branch($submodule) : string {}

/**
 * @param resource $submodule
 */
function git_submodule_fetch_recurse_submodules($submodule) : int {}

/**
 * @param resource $repo
 */
function git_submodule_foreach($repo,callable $callback,mixed $payload) : void {}

/**
 * @param resource $submodule
 */
function git_submodule_free($submodule) : void {}

/**
 * @param resource $submodule
 */
function git_submodule_head_id($submodule) : ?string {}

/**
 * @param resource $submodule
 */
function git_submodule_ignore($submodule) : int {}

/**
 * @param resource $submodule
 */
function git_submodule_index_id($submodule) : ?string {}

/**
 * @param resource $submodule
 */
function git_submodule_init($submodule,bool $overwrite) : void {}

/**
 * @param resource $submodule
 */
function git_submodule_location($submodule) : int {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_submodule_lookup($repo,string $name) {}

/**
 * @param resource $submodule
 */
function git_submodule_name($submodule) : string {}

/**
 * @param resource $submodule
 *
 * @return resource
 */
function git_submodule_open($submodule) {}

/**
 * @param resource $submodule
 *
 * @return resource
 */
function git_submodule_owner($submodule) {}

/**
 * @param resource $submodule
 */
function git_submodule_path($submodule) : string {}

/**
 * @param resource $submodule
 */
function git_submodule_reload($submodule,bool $force) : void {}

/**
 * @param resource $submodule
 *
 * @return resource
 */
function git_submodule_repo_init($submodule,bool $use_gitlink) {}

/**
 * @param resource $repo
 */
function git_submodule_resolve_url($repo,string $url) : string {}

/**
 * @param resource $repo
 */
function git_submodule_set_branch($repo,string $name,string $branch) : void {}

/**
 * @param resource $repo
 */
function git_submodule_set_fetch_recurse_submodules($repo,string $name,int $fetch_recurse_submodules) : void {}

/**
 * @param resource $repo
 */
function git_submodule_set_ignore($repo,string $name,int $ignore) : void {}

/**
 * @param resource $repo
 */
function git_submodule_set_update($repo,string $name,int $update) : void {}

/**
 * @param resource $repo
 */
function git_submodule_set_url($repo,string $name,string $url) : void {}

/**
 * @param resource $repo
 */
function git_submodule_status($repo,string $name,int $ignore) : int {}

/**
 * @param resource $submodule
 */
function git_submodule_sync($submodule) : void {}

/**
 * @param resource $submodule
 */
function git_submodule_update($submodule,bool $init,?array $submodule_update_options) : void {}

/**
 * @param resource $submodule
 */
function git_submodule_update_strategy($submodule) : int {}

/**
 * @param resource $submodule
 */
function git_submodule_url($submodule) : string {}

/**
 * @param resource $submodule
 */
function git_submodule_wd_id($submodule) : ?string {}
