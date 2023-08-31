<?php

/**
 * @param resource $repo
 * @param resource $target
 * @param resource $tagger
 */
function git_tag_annotation_create($repo,string $tag_name,$target,$tagger,string $message) : string {}

/**
 * @param resource $repo
 * @param resource $target
 * @param resource $tagger
 */
function git_tag_create($repo,string $tag_name,$target,$tagger,string $message,bool $force) : string {}

/**
 * @param resource $repo
 */
function git_tag_create_frombuffer($repo,string $buffer,bool $force) : string {}

/**
 * @param resource $repo
 * @param resource $target
 */
function git_tag_create_lightweight($repo,string $tag_name,$target,bool $buffer) : string {}

/**
 * @param resource $repo
 */
function git_tag_delete($repo,string $tag_name) : void {}

/**
 * @param resource $tag
 *
 * @return resource
 */
function git_tag_dup($tag) {}

/**
 * @param resource $repo
 */
function git_tag_foreach($repo,callable $callback,mixed $payload) : void {}

/**
 * @param resource $tag
 */
function git_tag_free($tag) : void {}

/**
 * @param resource $tag
 */
function git_tag_id($tag) : string {}

/**
 * @param resource $repo
 */
function git_tag_list($repo) : array {}

/**
 * @param resource $repo
 */
function git_tag_list_match(string $pattern,$repo) : array {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_tag_lookup($repo,string $id) {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_tag_lookup_prefix($repo,string $id_prefix) {}

/**
 * @param resource $tag
 */
function git_tag_message($tag) : ?string {}

/**
 * @param resource $tag
 */
function git_tag_name($tag) : string {}

/**
 * @param resource $tag
 *
 * @return resource
 */
function git_tag_owner($tag) {}

/**
 * @param resource $tag
 *
 * @return resource
 */
function git_tag_peel($tag) {}

/**
 * @param resource $tag
 *
 * @return resource
 */
function git_tag_tagger($tag) {}

/**
 * @param resource $tag
 *
 * @return resource
 */
function git_tag_target($tag) {}

/**
 * @param resource $tag
 */
function git_tag_target_id($tag) : string {}

/**
 * @param resource $tag
 */
function git_tag_target_type($tag) : int {}
