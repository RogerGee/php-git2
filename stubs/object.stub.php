<?php

/**
 * @param resource $object
 *
 * @return resource
 */
function git_object_dup($object) {}

/**
 * @param resource $object
 */
function git_object_free($object) : void {}

/**
 * @param resource $object
 */
function git_object_id($object) : string {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_object_lookup($repo,string $id,int $type) {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_object_lookup_bypath($repo,string $path,int $type) {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_object_lookup_prefix($repo,string $id,int $type) {}

/**
 * @param resource $object
 *
 * @return resource
 */
function git_object_owner($object) {}

/**
 * @param resource $object
 *
 * @return resource
 */
function git_object_peel($object,int $target_type) {}

/**
 * @param resource $object
 */
function git_object_short_id($object) : string {}

function git_object_string2type(string $str) : int {}

/**
 * @param resource $object
 */
function git_object_type($object) : int {}

function git_object_type2string(int $type) : string {}

function git_object_typeisloose(int $type) : bool {}
