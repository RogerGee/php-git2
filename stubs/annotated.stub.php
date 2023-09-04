<?php

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_annotated_commit_from_fetchhead($repo,string $branch_name,string $remote_url,string $oid) {}

/**
 * @param resource $repo
 * @param resource $ref
 *
 * @return resource
 */
function git_annotated_commit_from_ref($repo,$ref) {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_annotated_commit_from_revspec($repo,string $revspec) {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_annotated_commit_lookup($repo,string $id) {}

/**
 * @param resource $commit
 */
function git_annotated_commit_id($commit) : string {}

/**
 * @param resource $commit
 */
function git_annotated_commit_free($commit) : void {}
