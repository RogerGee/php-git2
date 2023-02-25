<?php

abstract class GitConfigBackend {
    /**
     * @param resource $repo
     */
    public function open(int $level,$repo) : void;

    public function get(string $key) : array;

    public function set(string $key,string $value) : void;

    public function set_multivar(string $name,string $regexp,string $value) : void;

    public function del(string $name) : void;

    public function del_multivar(string $name,string $regexp) : void;

    public function iterator_new() : mixed;

    public function iterator_next(mixed $context) : array;

    public function snapshot() : \GitConfigBackend;

    public function lock() : void;

    public function unlock(bool $success) : void;
}
