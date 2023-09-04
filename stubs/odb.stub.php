<?php

/**
 * @param resource $odb
 */
function git_odb_add_alternate($odb,\GitODBBackend $backend,int $priority) : void {}

/**
 * @param resource $odb
 */
function git_odb_add_backend($odb,\GitODBBackend $backend,int $priority) : void {}

/**
 * @param resource $odb
 */
function git_odb_add_disk_alternate($odb,string $path) : void {}

function git_odb_backend_loose(string $objects_dir,int $compression_level,bool $do_fsync,int $dir_mode,int $file_mode) : \GitODBBackend {}

function git_odb_backend_one_pack(string $index_file) : \GitODBBackend {}

function git_odb_backend_pack(string $objects_dir) : \GitODBBackend {}

/**
 * @param resource $odb
 */
function git_odb_exists($odb,string $id) : bool {}

/**
 * @param resource $odb
 */
function git_odb_exists_prefix($odb,string $prefix) : string {}

/**
 * @param resource $odb
 */
function git_odb_expand_ids($odb,array &$ids) : void {}

/**
 * @param resource $odb
 */
function git_odb_foreach($odb,callable $callback,mixed $payload) : void {}

/**
 * @param resource $odb
 */
function git_odb_free($odb) : void {}

/**
 * @param resource $odb
 */
function git_odb_get_backend($odb,int $pos) : \GitODBBackend {}

function git_odb_hash(string $data,int $type) : string {}

function git_odb_hashfile(string $path,int $type) : string {}

/**
 * @return resource
 */
function git_odb_new() {}

/**
 * @param resource $odb
 */
function git_odb_num_backends($odb) : int {}

/**
 * @param resource $odb_object
 */
function git_odb_object_data($odb_object) : string {}

/**
 * @param resource $odb_object
 *
 * @return resource
 */
function git_odb_object_dup($odb_object) {}

/**
 * @param resource $odb_object
 */
function git_odb_object_free($odb_object) : void {}

/**
 * @param resource $odb_object
 */
function git_odb_object_id($odb_object) : string {}

/**
 * @param resource $odb_object
 */
function git_odb_object_size($odb_object) : int {}

/**
 * @param resource $odb_object
 */
function git_odb_object_type($odb_object) : int {}

/**
 * @return resource
 */
function git_odb_open(string $objects_dir) {}

/**
 * @param resource $odb
 */
function git_odb_open_rstream(int &$length,int &$type,$odb,string $oid) : \GitODBStream {}

/**
 * @param resource $odb
 */
function git_odb_open_wstream($odb,int $size,int $type) : \GitODBStream {}

/**
 * @param resource $odb
 *
 * @return resource
 */
function git_odb_read($odb,string $id) {}

/**
 * @param resource $odb
 */
function git_odb_read_header(int &$type,$odb,string $id) : int {}

/**
 * @param resource $odb
 *
 * @return resource
 */
function git_odb_read_prefix($odb,string $prefix) {}

/**
 * @param resource $odb
 */
function git_odb_refresh($odb) : void {}

function git_odb_stream_finalize_write(\GitODBStream $stream) : string {}

function git_odb_stream_read(\GitODBStream $stream,int $buffer_length) : string {}

function git_odb_stream_write(\GitODBStream $stream,string $buffer) : void {}

/**
 * @param resource $odb
 */
function git_odb_write($odb,string $data,int $type) : string {}

/**
 * @param resource $odb
 */
function git_odb_write_pack($odb,callable $progress_callback,mixed $payload) : \GitODBWritepack {}
