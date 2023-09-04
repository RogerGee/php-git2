<?php

/**
 * @param resource $reflog
 * @param resource $committer
 */
function git_reflog_append($reflog,string $id,$committer,?string $msg) : void {}

/**
 * @param resource $repo
 */
function git_reflog_delete($repo,string $name) : void {}

/**
 * @param resource $reflog
 */
function git_reflog_drop($reflog,int $idx,bool $rewrite_previous_entry) : bool {}

/**
 * @param resource $reflog
 */
function git_reflog_entry_byindex($reflog,int $idx) : array {}

/**
 * @param resource $reflog
 */
function git_reflog_entrycount($reflog) : int {}

/**
 * @param resource $reflog
 */
function git_reflog_free($reflog) : void {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_reflog_read($repo,string $name) {}

/**
 * @param resource $repo
 */
function git_reflog_rename($repo,string $old_name,string $name) : void {}

/**
 * @param resource $reflog
 */
function git_reflog_write($reflog) : void {}
