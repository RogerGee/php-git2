<?php

/**
 * @param resource $repo
 */
function git_ignore_add_rule($repo,string $rules) : void {}

/**
 * @param resource $repo
 */
function git_ignore_clear_internal_rules($repo) : void {}

/**
 * @param resource $repo
 */
function git_ignore_path_is_ignored($repo,string $path) : bool {}
