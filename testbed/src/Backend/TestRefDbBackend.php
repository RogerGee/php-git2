<?php

namespace PhpGit2\Backend;

use PhpGit2\Utility\TestWrapperTrait;

class TestRefDbBackend extends PHPSerializedRefDbBackend {
    use TestWrapperTrait;

    public function exists($refName) {
        $this->called[] = 'exists';
        $this->unit->assertIsString($refName);
        return parent::exists($refName);
    }

    public function lookup($refName) {
        $this->called[] = 'lookup';
        $this->unit->assertIsString($refName);
        return parent::lookup($refName);
    }

    public function iterator_new($glob) {
        $this->called[] = 'iterator_new';
        $this->unit->assertIsStringOrNull($glob);
        parent::iterator_new($glob);
    }

    public function iterator_next(&$name) {
        $this->called[] = 'iterator_next';
        return parent::iterator_next($name);
    }

    public function write($ref,$force,$who,$message,$old,$oldTarget) {
        $this->called[] = 'write';
        $this->unit->assertResourceHasType($ref,'git_reference');
        $this->unit->assertIsBool($force);
        $this->unit->assertIsArray($who);
        $this->unit->assertIsString($message);
        $this->unit->assertIsStringOrNull($old);
        $this->unit->assertIsStringOrNull($oldTarget);
        parent::write($ref,$force,$who,$message,$old,$oldTarget);
    }

    public function rename($oldName,$newName,$force,$who,$message) {
        $this->called[] = 'rename';
        $this->unit->assertIsString($oldName);
        $this->unit->assertIsString($newName);
        $this->unit->assertIsBool($force);
        $this->unit->assertIsArray($who);
        $this->unit->assertIsString($message);
        parent::rename($oldName,$newName,$force,$who,$message);
    }

    public function del($refName,$oldId,$oldTarget) {
        $this->called[] = 'del';
        $this->unit->assertIsString($refName);
        $this->unit->assertIsStringOrNull($oldId);
        $this->unit->assertIsStringOrNull($oldTarget);
        parent::del($refName,$oldId,$oldTarget);
    }

    public function compress() {
        $this->called[] = 'compress';
        parent::compress();
    }

    public function has_log($refname) {
        $this->called[] = 'has_log';
        $this->unit->assertIsString($refname);
        return parent::has_log($refname);
    }

    public function ensure_log($refname) {
        $this->called[] = 'ensure_log';
        $this->unit->assertIsString($refname);
        return parent::ensure_log($refname);
    }

    public function reflog_write($name) {
        $this->called[] = 'reflog_write';
        $this->unit->assertIsString($name);
        return parent::reflog_write($name);
    }

    public function reflog_read($name) {
        $this->called[] = 'reflog_read';
        $this->unit->assertIsString($name);
        return parent::reflog_read($name);
    }

    public function reflog_rename($oldName,$newName) {
        $this->called[] = 'reflog_rename';
        $this->unit->assertIsString($oldName);
        $this->unit->assertIsString($newName);
        return parent::reflog_rename($oldName,$newName);
    }

    public function reflog_delete($name) {
        $this->called[] = 'reflog_delete';
        $this->unit->assertIsString($name);
        return parent::reflog_delete($name);
    }
}
