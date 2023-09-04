<?php

/**
 * @param resource $refdb
 */
function git_refdb_backend_fs($refdb) : \GitRefDBBackend {}

/**
 * @param resource $refdb
 */
function git_refdb_compress($refdb) : void {}

/**
 * @param resource $refdb
 */
function git_refdb_free($refdb) : void {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_refdb_new($repo) {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_refdb_open($repo) {}

/**
 * @param resource $refdb
 */
function git_refdb_set_backend($refdb,\GitRefDBBackend $backend) : void {}
