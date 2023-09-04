<?php

/**
 * @param resource $treebuilder
 */
function git_treebuilder_clear($treebuilder) : void {}

/**
 * @param resource $treebuilder
 */
function git_treebuilder_entrycount($treebuilder) : int {}

/**
 * @param resource $treebuilder
 */
function git_treebuilder_filter($treebuilder,callable $callback,mixed $payload) : void {}

/**
 * @param resource $treebuilder
 */
function git_treebuilder_free($treebuilder) : void {}

/**
 * @param resource $treebuilder
 *
 * @return resource
 */
function git_treebuilder_get($treebuilder,string $filename) {}

/**
 * @param resource $treebuilder
 *
 * @return resource
 */
function git_treebuilder_insert($treebuilder,string $filename,string $id,int $filemode) {}

/**
 * @param resource $repo
 * @param resource $tree
 *
 * @return resource
 */
function git_treebuilder_new($repo,$tree) {}

/**
 * @param resource $treebuilder
 */
function git_treebuilder_remove($treebuilder,string $filename) : void {}

/**
 * @param resource $treebuilder
 */
function git_treebuilder_write($treebuilder) : string {}
