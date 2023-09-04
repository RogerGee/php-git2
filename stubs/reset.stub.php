<?php

/**
 * @param resource $repo
 * @param resource $target_object
 */
function git_reset($repo,$target_object,int $reset_type,?array $checkout_options) : void {}

/**
 * @param resource $repo
 * @param resource $target_object
 */
function git_reset_default($repo,$target_object,array $pathspecs) : void {}

/**
 * @param resource $repo
 * @param resource $annotated_commit
 */
function git_reset_from_annotated($repo,$annotated_commit,int $reset_type,?array $checkout_options) : void {}
