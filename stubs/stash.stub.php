<?php

/**
 * @param resource $repo
 */
function git_stash_apply($repo,int $index,?array $stash_apply_options) : void {}

/**
 * @param resource $repo
 */
function git_stash_drop($repo,int $index) : void {}

/**
 * @param resource $repo
 */
function git_stash_foreach($repo,callable $callback,mixed $payload) : void {}

/**
 * @param resource $repo
 */
function git_stash_pop($repo,int $index,?array $stash_apply_options) : void {}

/**
 * @param resource $repo
 * @param resource $signature
 */
function git_stash_save($repo,$signature,string $message,int $flags) : string {}
