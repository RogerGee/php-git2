<?php

/**
 * @return resource
 */
function git_credential_default_new() {}

/**
 * @param resource $cred
 */
function git_credential_free($cred) : void {}

/**
 * @param resource $cred
 */
function git_credential_has_username($cred) : bool {}

/**
 * @return resource
 */
function git_credential_ssh_key_from_agent(string $username) {}

/**
 * @return resource
 */
function git_credential_ssh_key_memory_new(string $username,string $publickey,string $privatekey,string $passphrase) {}

/**
 * @return resource
 */
function git_credential_ssh_key_new(string $username,string $publickey,string $privatekey,string $passphrase) {}

/**
 * @return resource
 */
function git_credential_username_new(string $username) {}

/**
 * @return resource
 */
function git_credential_userpass_plaintext_new(string $username,string $password) {}
