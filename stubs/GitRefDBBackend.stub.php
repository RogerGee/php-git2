<?php

abstract class GitRefDBBackend {
    public function exists(string $ref_name) : bool;

    public function lookup(string $ref_name) : string;

    public function iterator_new(string $glob) : void;

    public function iterator_next(string &$name) : string;

    /**
     * @param resource $ref
     */
    public function write($ref,bool $force,array $who,string $message,string $old,string $old_target) : void;

    /**
     * @return resource
     */
    public function rename(string $old_name,string $new_name,bool $force,array $who,string $message);

    public function del(string $ref_name,string $old_id,string $old_target) : void;

    public function compress() : void;

    public function has_log(string $ref_name) : bool;

    public function ensure_log(string $ref_name) : void;

    public function reflog_read(string $name) : array;

    /**
     * @param resource $reflog
     */
    public function reflog_write($reflog) : void;

    public function reflog_rename(string $old_name,string $new_name) : void;

    public function reflog_delete(string $name) : void;

    public function lock(string $ref_name) : mixed;

    /**
     * @param resource $ref
     */
    public function unlock(mixed $payload,bool $success,bool $update_reflog,$ref,array $sig,string $message) : void;
}

class GitRefDBBackend_Internal extends GitRefDBBackend {

}
