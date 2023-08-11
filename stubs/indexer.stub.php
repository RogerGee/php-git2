<?php

/**
 * @param resource $indexer
 */
function git_indexer_append($indexer,string $data) : void {}

/**
 * @param resource $indexer
 */
function git_indexer_commit($indexer) : void {}

/**
 * @param resource $indexer
 */
function git_indexer_free($indexer) : void {}

/**
 * @param resource $indexer
 */
function git_indexer_hash($indexer) : string {}

/**
 * @param resource $odb
 *
 * @return resource
 */
function git_indexer_new(string $path,int $mode,$odb,?array $options) {}

/**
 * @param resource $indexer
 */
function git2_indexer_stats($indexer) : array {}
