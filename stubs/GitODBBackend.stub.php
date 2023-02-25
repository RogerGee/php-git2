<?php

abstract class GitODBBackend {
    public function read(int &$type,string $oid) : string;

    public function read_prefix(string &$full_oid,int &$type,string $abbrev_oid) : string;

    public function read_header(int &$size,int &$type,string $oid) : void;

    public function write(string $oid,string $data,int $type) : void;

    public function writestream(int $offset,int $object_type) : \GitODBStream;

    public function readstream(int &$length,int &$type,string $oid) : \GitODBStream;

    public function exists(string $oid) : bool;

    public function exists_prefix(string &$full_oid,string $prefix) : bool;

    public function refresh() : void;

    public function for_each(callable $callback,mixed $payload = null) : void;

    /** @param resource $odb */
    public function writepack($odb,callable $callback,mixed $payload = null) : \GitODBWritepack;
}

final class GitODBBackend_Internal extends GitODBBackend {

}
