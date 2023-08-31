<?php

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_signature_default($repo) {}

/**
 * @param resource $signature
 *
 * @return resource
 */
function git_signature_dup($signature) {}

/**
 * @param resource $signature
 */
function git_signature_free($signature) : void {}

/**
 * @return resource
 */
function git_signature_from_buffer(string $buffer) {}

/**
 * @return resource
 */
function git_signature_new(string $name,string $email,int $time,int $offset) {}

function git_signature_now(string $name,string $email) : resource {}

/**
 * @param resource $signature
 */
function git2_signature_convert($signature) : array {}
