<?php

/**
 * @param resource $repo
 */
function git_revparse($repo,string $spec) : array {}

/**
 * @param resource $reference
 * @param resource $repo
 *
 * @return resource
 */
function git_revparse_ext(&$reference,$repo,string $spec) {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_revparse_single($repo,string $spec) {}
