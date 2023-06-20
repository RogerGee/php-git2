<?php

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_repository_config($repo) {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_repository_config_snapshot($repo) {}

/**
 * @param resource $repo
 */
function git_repository_detach_head($repo) : int {}

function git_repository_discover(string $start_path,bool $across_fs,string $ceiling_dirs = null) : string {}

/**
 * @param resource $repo
 */
function git_repository_fetchhead_foreach($repo,callable $callback,mixed $payload) : void {}

/**
 * @param resource $repo
 */
function git_repository_free($repo) : void {}

/**
 * @param resource $repo
 */
function git_repository_get_namespace($repo) : string {}

/**
 * @param resource $repo
 */
function git_repository_hashfile($repo,string $path,int $type,string $as_path = null) : string {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_repository_head($repo) {}

/**
 * @param resource $repo
 */
function git_repository_head_detached($repo) : bool {}

/**
 * @param resource $repo
 */
function git_repository_head_unborn($repo) : bool {}

/**
 * @param resource $repo
 */
function git_repository_ident(string &$email,$repo) : string {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_repository_index($repo) {}

/**
 * @return resource
 */
function git_repository_init(string $path,bool $is_bare = false) {}

/**
 * @return resource
 */
function git_repository_init_ext(string $path,array $options = null) {}

/**
 * @param resource $repo
 */
function git_repository_is_bare($repo) : bool {}

/**
 * @param resource $repo
 */
function git_repository_is_empty($repo) : bool {}

/**
 * @param resource $repo
 */
function git_repository_is_shallow($repo) : bool {}

/**
 * @param resource $repo
 */
function git_repository_mergehead_foreach($repo,callable $callback,mixed $payload) : void {}

/**
 * @param resource $repo
 */
function git_repository_message($repo) : string|bool {}

/**
 * @param resource $repo
 */
function git_repository_message_remove($repo) : void {}

/**
 * @return resource
 */
function git_repository_new() {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_repository_odb($repo) {}

/**
 * @return resource
 */
function git_repository_open(string $path) {}

/**
 * @return resource
 */
function git_repository_open_bare(string $bare_path) {}

/**
 * @return resource
 */
function git_repository_open_ext(?string $path,int $flags,string $ceiling_dirs) {}

/**
 * @param resource $repo
 */
function git_repository_path($repo) : string {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_repository_refdb($repo) {}

/**
 * @param resource $repo
 */
function git_repository_reinit_filesystem($repo,bool $recurse_submodules = true) : void {}

/**
 * @param resource $repo
 */
function git_repository_set_bare($repo) : void {}

/**
 * @param resource $repo
 * @param resource $config
 */
function git_repository_set_config($repo,$config) : void {}

/**
 * @param resource $repo
 */
function git_repository_set_head($repo,string $refname) : void {}

/**
 * @param resource $repo
 */
function git_repository_set_head_detached($repo,string $committish) : bool {}

/**
 * @param resource $repo
 * @param resource $committish
 */
function git_repository_set_head_detached_from_annotated($repo,$committish) : bool {}

/**
 * @param resource $repo
 */
function git_repository_set_ident($repo,string $name,string $email) : void {}

/**
 * @param resource $repo
 * @param resource $index
 */
function git_repository_set_index($repo,$index) : void {}

/**
 * @param resource $repo
 */
function git_repository_set_namespace($repo,string $namespace) : void {}

/**
 * @param resource $repo
 * @param resource $odb
 */
function git_repository_set_odb($repo,$odb) : void {}

/**
 * @param resource $repo
 * @param resource $refdb
 */
function git_repository_set_refdb($repo,$refdb) : void {}

/**
 * @param resource $repo
 */
function git_repository_set_workdir($repo,string $workdir,bool $update_gitlink) : void {}

/**
 * @param resource $repo
 */
function git_repository_state($repo) : int {}

/**
 * @param resource $repo
 */
function git_repository_state_cleanup($repo) : void {}

/**
 * @param resource $repo
 */
function git_repository_workdir($repo) : ?string {}

/**
 * @param resource $odb
 *
 * @return resource
 */
function git_repository_wrap_odb($odb) {}
