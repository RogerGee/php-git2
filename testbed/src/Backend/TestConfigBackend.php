<?php

namespace PhpGit2\Backend;

use PhpGit2\Callback\CallbackPayload;
use PhpGit2\Utility\TestWrapperTrait;

class TestConfigBackend extends PHPSerializedConfigBackend {
    use TestWrapperTrait;

    private static $__latest = null;
    public static function getLatest() {
        return static::$__latest;
    }

    public function open(int $level,$repo) : void {
        static::$__latest = $this;
        $this->called[] = 'open';
        parent::open($level,$repo);
    }

    public function get(string $name) : array|bool {
        $this->called[] = 'get';
        $this->unit->assertIsString($name);
        return parent::get($name);
    }

    public function set(string $name,string $value) : void {
        $this->called[] = 'set';
        $this->unit->assertIsString($name);
        $this->unit->assertIsString($value);
        parent::set($name,$value);
    }

    public function set_multivar(string $name,string $regexp,string $value) : void {
        $this->called[] = 'set_multivar';
        $this->unit->assertIsString($name);
        $this->unit->assertIsString($regexp);
        $this->unit->assertIsString($value);
        parent::set_multivar($name,$regexp,$value);
    }

    public function del(string $name) : void {
        $this->called[] = 'del';
        $this->unit->assertIsString($name);
        parent::del($name);
    }

    public function del_multivar(string $name,string $regexp) : void {
        $this->called[] = 'del_multivar';
        $this->unit->assertIsString($name);
        $this->unit->assertIsString($regexp);
        parent::del_multivar($name,$regexp);
    }

    public function iterator_new() : mixed {
        $this->called[] = 'iterator_new';
        parent::iterator_new();
        return new CallbackPayload;
    }

    public function iterator_next($context) : array|bool {
        $this->called[] = 'iterator_next';
        $this->unit->assertInstanceOf(CallbackPayload::class,$context);
        return parent::iterator_next($context);
    }

    public function snapshot() : \GitConfigBackend {
        $this->called[] = 'snapshot';
        return parent::snapshot();
    }

    public function lock() : void {
        $this->called[] = 'lock';
        parent::lock();
    }

    public function unlock(bool $success) : void {
        $this->called[] = 'unlock';
        $this->unit->assertIsBool($success);
        parent::unlock();
    }
}
