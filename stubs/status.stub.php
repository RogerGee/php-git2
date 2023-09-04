<?php

/**
 * @param resource $statuslist
 */
function git_status_byindex($statuslist,int $idx) : array {}

/**
 * @param resource $repo
 */
function git_status_file($repo,string $path) : int|bool|null {}

/**
 * @param resource $repo
 */
function git_status_foreach($repo,callable $callback,mixed $payload) : void {}

/**
 * @param resource $repo
 */
function git_status_foreach_ext($repo,?array $status_options,callable $callback,mixed $payload) : void {}

/**
 * @param resource $statuslist
 */
function git_status_list_entrycount($statuslist) : int {}

/**
 * @param resource $statuslist
 */
function git_status_list_free($statuslist) : void {}

/**
 * @param resource $statuslist
 */
function git_status_list_get_perfdata($statuslist) : array {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_status_list_new($repo,?array $status_options) {}

/**
 * @param resource $repo
 */
function git_status_should_ignore($repo,string $path) : bool {}
