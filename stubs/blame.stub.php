<?php

/**
 * @param resource $reference
 *
 * @return resource
 */
function git_blame_buffer($reference,string $buffer) {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_blame_file($repo,string $path,?array $options) {}

/**
 * @param resource $blame
 */
function git_blame_free($blame) : void {}

/**
 * @param resource $blame
 */
function git_blame_get_hunk_byindex($blame,int $index) : array {}

/**
 * @param resource $blame
 */
function git_blame_get_hunk_byline($blame,int $lineno) : array {}

/**
 * @param resource $blame
 */
function git_blame_get_hunk_count($blame) : int {}
