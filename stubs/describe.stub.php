<?php

/**
 * @param resource $committish
 *
 * @return resource
 */
function git_describe_commit($committish,?array $options = null) {}

/**
 * @param resource $result
 */
function git_describe_format($result,?array $options = null) : string {}

/**
 * @param resource $result
 */
function git_describe_result_free($result) : void {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_describe_workdir($repo,?array $options = null) {}
