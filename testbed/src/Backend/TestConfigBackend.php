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

    public function open($level,$repo) {
        static::$__latest = $this;
        $this->called[] = 'open';
        parent::open($level,$repo);
    }

    public function get($name) {
        $this->called[] = 'get';
        $this->unit->assertIsString($name);
        return parent::get($name);
    }

    public function set($name,$value) {
        $this->called[] = 'set';
        $this->unit->assertIsString($name);
        $this->unit->assertIsString($value);
        return parent::set($name,$value);
    }

    public function set_multivar($name,$regexp,$value) {
        $this->called[] = 'set_multivar';
        $this->unit->assertIsString($name);
        $this->unit->assertIsString($regexp);
        $this->unit->assertIsString($value);
        return parent::set_multivar($name,$regexp,$value);
    }

    public function del($name) {
        $this->called[] = 'del';
        $this->unit->assertIsString($name);
        return parent::del($name);
    }

    public function del_multivar($name,$regexp) {
        $this->called[] = 'del_multivar';
        $this->unit->assertIsString($name);
        $this->unit->assertIsString($regexp);
        return parent::del_multivar($name,$regexp);
    }

    public function iterator_new() {
        $this->called[] = 'iterator_new';
        parent::iterator_new();
        return new CallbackPayload;
    }

    public function iterator_next($context) {
        $this->called[] = 'iterator_next';
        $this->unit->assertInstanceOf(CallbackPayload::class,$context);
        return parent::iterator_next($context);
    }

    public function snapshot() {
        $this->called[] = 'snapshot';
        return parent::snapshot();
    }

    public function lock() {
        $this->called[] = 'lock';
        return parent::lock();
    }

    public function unlock() {
        $this->called[] = 'unlock';
        return parent::unlock();
    }
}
