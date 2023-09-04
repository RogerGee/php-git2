<?php

/**
 * @param resource $repo
 */
function git_attr_add_macro($repo,string $name,string $values) : void {}

/**
 * @param resource $repo
 */
function git_attr_cache_flush($repo) : void {}

/**
 * @param resource $repo
 */
function git_attr_foreach($repo,int $flags,string $path,callable $callback,mixed $payload) : void {}

/**
 * @param resource $repo
 */
function git_attr_get($repo,int $flags,string $path,string $name) : string {}

function git_attr_value(string $attr) : int {}
