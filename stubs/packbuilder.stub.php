<?php

/**
 * @param resource $packbuilder
 */
function git_packbuilder_foreach($packbuilder,callable $callback,mixed $payload_or_stream) : void {}

/**
 * @param resource $packbuilder
 */
function git_packbuilder_free($packbuilder) : void {}

/**
 * @param resource $packbuilder
 */
function git_packbuilder_hash($packbuilder) : string {}

/**
 * @param resource $packbuilder
 */
function git_packbuilder_insert($packbuilder,string $id,?string $name) : void {}

/**
 * @param resource $packbuilder
 */
function git_packbuilder_insert_commit($packbuilder,string $id) : void {}

/**
 * @param resource $packbuilder
 */
function git_packbuilder_insert_recur($packbuilder,string $id,?string $name) : void {}

/**
 * @param resource $packbuilder
 */
function git_packbuilder_insert_tree($packbuilder,string $id) : void {}

/**
 * @param resource $packbuilder
 * @param resource $revwalk
 */
function git_packbuilder_insert_walk($packbuilder,resource $revwalk) : void {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_packbuilder_new($repo) {}

/**
 * @param resource $packbuilder
 */
function git_packbuilder_object_count($packbuilder) : int {}

/**
 * @param resource $packbuilder
 */
function git_packbuilder_set_callbacks($packbuilder,callable $progress_callback,mixed $payload) : void {}

/**
 * @param resource $packbuilder
 */
function git_packbuilder_set_threads($packbuilder,int $n_threads) : void {}

/**
 * @param resource $packbuilder
 */
function git_packbuilder_write($packbuilder,string $path,int $mode,callable $progress_callback,mixed $payload) : void {}

/**
 * @param resource $packbuilder
 */
function git_packbuilder_written($packbuilder) : int {}
