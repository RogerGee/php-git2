<?php

/**
 * @param resource $rebase
 */
function git_rebase_abort($rebase) : bool {}

/**
 * @param resource $rebase
 * @param resource $author
 * @param resource $committer
 */
function git_rebase_commit(string &$id,$rebase,$author,$committer,string $message_encoding = null,string $message = null) : int {}

/**
 * @param resource $rebase
 * @param resource $signature
 */
function git_rebase_finish($rebase,$signature) : void {}

/**
 * @param resource $rebase
 */
function git_rebase_free($rebase) : void {}

/**
 * @param resource $repo
 * @param resource $branch
 * @param resource $upstream
 * @param resource $onto
 *
 * @return resource
 */
function git_rebase_init($repo,$branch,$upstream,$onto,?array $options) {}

/**
 * @param resource $rebase
 *
 * @return resource
 */
function git_rebase_inmemory_index($rebase) {}

/**
 * @param resource $rebase
 */
function git_rebase_next($rebase) : array {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_rebase_open($repo,?array $options) {}

/**
 * @param resource $rebase
 */
function git_rebase_operation_byindex($rebase,int $idx) : array {}

/**
 * @param resource $rebase
 */
function git_rebase_operation_current($rebase) : int {}

/**
 * @param resource $rebase
 */
function git_rebase_operation_entrycount($rebase) : int {}
