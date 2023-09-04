<?php

/**
 * @param resource $repo
 */
function git_remote_add_fetch($repo,string $remote,string $refspec) : void {}

/**
 * @param resource $repo
 */
function git_remote_add_push($repo,string $remote,string $refspec) : void {}

/**
 * @param resource $remote
 */
function git_remote_autotag($remote) : int {}

/**
 * @param resource $remote
 */
function git_remote_connect($remote,int $direction,?array $callbacks,?array $proxy_options,?array $custom_headers) : void {}

/**
 * @param resource $remote
 */
function git_remote_connected($remote) : bool {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_remote_create($repo,string $name,string $url) {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_remote_create_anonymous($repo,string $url) {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_remote_create_with_fetchspec($repo,string $name,string $url,string $fetchspec) {}

/**
 * @param resource $remote
 */
function git_remote_default_branch($remote) : string {}

/**
 * @param resource $repo
 */
function git_remote_delete($repo,string $name) : void {}

/**
 * @param resource $remote
 */
function git_remote_disconnect($remote) : void {}

/**
 * @param resource $remote
 */
function git_remote_download($remote,?array $refspecs,?array $fetch_options) : void {}

/**
 * @param resource $remote
 *
 * @return resource
 */
function git_remote_dup($remote) {}

/**
 * @param resource $remote
 */
function git_remote_fetch($remote,?array $refspecs,?array $fetch_options,?string $reflog_message) : void {}

/**
 * @param resource $remote
 */
function git_remote_free($remote) : void {}

/**
 * @param resource $remote
 */
function git_remote_get_fetch_refspecs($remote) : array {}

/**
 * @param resource $remote
 */
function git_remote_get_push_refspecs($remote) : array {}

/**
 * @param resource $remote
 *
 * @return resource
 */
function git_remote_get_refspec($remote,int $n) {}

function git_remote_is_valid_name(string $remote_name) : bool {}

/**
 * @param resource $repo
 */
function git_remote_list($repo) : array {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_remote_lookup($repo,string $name) {}

/**
 * @param resource $remote
 */
function git_remote_ls($remote) : array {}

/**
 * @param resource $remote
 */
function git_remote_name($remote) : string {}

/**
 * @param resource $remote
 *
 * @return resource
 */
function git_remote_owner($remote) {}

/**
 * @param resource $remote
 */
function git_remote_prune($remote,?array $callbacks) : void {}

/**
 * @param resource $remote
 */
function git_remote_prune_refs($remote) : void {}

/**
 * @param resource $remote
 */
function git_remote_push($remote,?array $refspecs,?array $push_options) : void {}

/**
 * @param resource $remote
 */
function git_remote_pushurl($remote) : string {}

/**
 * @param resource $remote
 */
function git_remote_refspec_count($remote) : int {}

/**
 * @param resource $repo
 */
function git_remote_rename($repo,string $name,string $new_name) : array {}

/**
 * @param resource $repo
 */
function git_remote_set_autotag($repo,string $remote,int $value) : void {}

/**
 * @param resource $repo
 */
function git_remote_set_pushurl($repo,string $remote,string $url) : void {}

/**
 * @param resource $repo
 */
function git_remote_set_url($repo,string $remote,string $url) : void {}

/**
 * @param resource $remote
 */
function git_remote_stats($remote) : array {}

/**
 * @param resource $remote
 */
function git_remote_stop($remote) : void {}

/**
 * @param resource $remote
 */
function git_remote_update_tips($remote,?array $callbacks,bool $update_fetchhead,int $download_tags,?string $reflog_message) : void {}

/**
 * @param resource $remote
 */
function git_remote_upload($remote,?array $refspecs,?array $push_options) : void {}

/**
 * @param resource $remote
 */
function git_remote_url($remote) : string {}
