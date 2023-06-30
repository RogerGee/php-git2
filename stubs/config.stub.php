<?php

/**
 * @param resource $cfg
 * @param resource $repo
 */
function git_config_add_backend($cfg,\GitConfigBackend $backend,int $level,$repo,bool $force) : void {}

/**
 * @param resource $cfg
 * @param resource $repo
 */
function git_config_add_file_ondisk($cfg,string $path,int $level,$repo,bool $force) : void {}

function git_config_backend_foreach_match(\GitConfigBackend $backend,?string $regexp,callable $callback,mixed $payload) : void {}

/**
 * @param resource $cfg
 */
function git_config_delete_entry($cfg,string $name) : void {}

/**
 * @param resource $cfg
 */
function git_config_delete_multivar($cfg,string $name,string $regexp) : void {}

function git_config_find_global() : string {}

function git_config_find_programdata() : string {}

function git_config_find_system() : string {}

function git_config_find_xdg() : string {}

/**
 * @param resource $cfg
 */
function git_config_foreach($cfg,callable $callback,mixed $payload) : void {}

/**
 * @param resource $cfg
 */
function git_config_foreach_match($cfg,string $regexp,callable $callback,mixed $payload) : void {}

/**
 * @param resource $cfg
 */
function git_config_free($cfg) : void {}

/**
 * @param resource $cfg
 */
function git_config_get_bool($cfg,string $name) : bool {}

/**
 * @param resource $cfg
 */
function git_config_get_entry($cfg,string $name) : array {}

/**
 * @param resource $cfg
 */
function git_config_get_int32($cfg,string $name) : int {}

/**
 * @param resource $cfg
 */
function git_config_get_int64($cfg,string $name) : int {}

/**
 * @param resource $cfg
 */
function git_config_get_mapped($cfg,string $name,array $maps) : int {}

/**
 * @param resource $cfg
 */
function git_config_get_multivar_foreach($cfg,string $name,?string $regexp,callable $callback,mixed $payload) : void {}

/**
 * @param resource $cfg
 */
function git_config_get_path($cfg,string $name) : string {}

/**
 * @param resource $cfg
 */
function git_config_get_string($cfg,string $name) : string {}

/**
 * @param resource $cfg
 */
function git_config_get_string_buf($cfg,string $name) : string {}

/**
 * @param resource $iter
 */
function git_config_iterator_free($iter) : void {}

/**
 * @param resource $cfg
 *
 * @return resource
 */
function git_config_iterator_glob_new($cfg,string $regexp) {}

/**
 * @param resource $cfg
 *
 * @return resource
 */
function git_config_iterator_new($cfg) {}

function git_config_lookup_map_value(array $maps,string $value) : int {}

/**
 * @param resource $cfg
 *
 * @return resource
 */
function git_config_multivar_iterator_new($cfg,string $name,?string $regexp) {}

/**
 * @return resource
 */
function git_config_new() {}

/**
 * @param resource $iter
 */
function git_config_next($iter) : array|bool {}

/**
 * @return resource
 */
function git_config_open_default() {}

/**
 * @param resource $cfg
 *
 * @return resource
 */
function git_config_open_global($cfg) {}

/**
 * @param resource $parent
 *
 * @return resource
 */
function git_config_open_level($parent,int $level) {}

/**
 * @return resource
 */
function git_config_open_ondisk(string $path) {}

function git_config_parse_bool(string $value) : bool {}

function git_config_parse_int32(string $value) : int {}

function git_config_parse_int64(string $value) : int {}

function git_config_parse_path(string $value) : string {}

/**
 * @param resource $cfg
 */
function git_config_set_bool($cfg,string $name,bool $value) : void {}

/**
 * @param resource $cfg
 */
function git_config_set_int32($cfg,string $name,int $value) : void {}

/**
 * @param resource $cfg
 */
function git_config_set_int64($cfg,string $name,int $value) : void {}

/**
 * @param resource $cfg
 */
function git_config_set_multivar($cfg,string $name,string $regexp,string $value) : void {}

/**
 * @param resource $cfg
 */
function git_config_set_string($cfg,string $name,string $value) : void {}

/**
 * @param resource $cfg
 *
 * @return resource
 */
function git_config_snapshot($cfg) {}
