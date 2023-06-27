<?php

/**
 * @param resource $repo
 * @param resource $commit
 */
function git_cherrypick($repo,$commit,?array $options = null) : void {}

/**
 * @param resource $repo
 * @param resource $cherrypick_commit
 * @param resource $our_commit
 *
 * @return resource
 */
function git_cherrypick_commit($repo,$cherrypick_commit,$our_commit,int $mainline,?array $options = null) {}
