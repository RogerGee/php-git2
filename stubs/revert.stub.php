<?php

/**
 * @param resource $repo
 * @param resource $commit
 */
function git_revert($repo,$commit,?array $revert_options) : void {}

/**
 * @param resource $repo
 * @param resource $revert_commit
 * @param resource $our_commit
 *
 * @return resource
 */
function git_revert_commit($repo,$revert_commit,$our_commit,int $mainline,?array $merge_options) {}
