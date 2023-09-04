<?php

/**
 * @param resource $note
 *
 * @return resource
 */
function git_note_author($note) {}

/**
 * @param resource $note
 *
 * @return resource
 */
function git_note_committer($note) {}

/**
 * @param resource $repo
 * @param resource $author
 * @param resource $committer
 */
function git_note_create($repo,?string $notes_ref,$author,$committer,string $oid,string $note,bool $force) : string {}

/**
 * @param resource $repo
 */
function git_note_foreach($repo,?string $notes_ref,callable $callback,mixed $payload) : void {}

/**
 * @param resource $note
 */
function git_note_free($note) : void {}

/**
 * @param resource $note
 */
function git_note_id($note) : string {}

/**
 * @param resource $note_iterator
 */
function git_note_iterator_free($note_iterator) : void {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_note_iterator_new($repo,?string $notes_ref) {}

/**
 * @param resource $note
 */
function git_note_message($note) : string {}

/**
 * @param resource $note_iterator
 */
function git_note_next(string &$annotated_id,$note_iterator) : string {}

/**
 * @param resource $repo
 *
 * @return resource
 */
function git_note_read($repo,?string $notes_ref,string $oid) {}

/**
 * @param resource $repo
 * @param resource $author
 * @param resource $committer
 */
function git_note_remove($repo,?string $notes_ref,$author,$committer,string $oid) : void {}
