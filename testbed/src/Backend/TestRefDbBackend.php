<?php

namespace PhpGit2\Backend;

use PhpGit2\Utility\TestWrapperTrait;

class TestRefDbBackend extends PHPSerializedRefDbBackend {
    use TestWrapperTrait;

    public function exists(string $refName) : bool {
        $this->called[] = 'exists';
        $this->unit->assertIsString($refName);
        return parent::exists($refName);
    }

    public function lookup(string $refName) : mixed {
        $this->called[] = 'lookup';
        $this->unit->assertIsString($refName);
        return parent::lookup($refName);
    }

    public function iterator_new(string $glob = null) : void {
        $this->called[] = 'iterator_new';
        $this->unit->assertIsStringOrNull($glob);
        parent::iterator_new($glob);
    }

    public function iterator_next(&$name) : mixed {
        $this->called[] = 'iterator_next';
        return parent::iterator_next($name);
    }

    public function write($ref,bool $force,array $who,?string $message,?string $old,?string $oldTarget) : void {
        $this->called[] = 'write';
        $this->unit->assertResourceHasType($ref,'git_reference');
        $this->unit->assertIsBool($force);
        $this->unit->assertIsArray($who);
        $this->unit->assertIsString($message);
        $this->unit->assertIsStringOrNull($old);
        $this->unit->assertIsStringOrNull($oldTarget);
        parent::write($ref,$force,$who,$message,$old,$oldTarget);
    }

    public function rename(string $oldName,string $newName,bool $force,array $who,string $message = null) {
        $this->called[] = 'rename';
        $this->unit->assertIsString($oldName);
        $this->unit->assertIsString($newName);
        $this->unit->assertIsBool($force);
        $this->unit->assertIsArray($who);
        $this->unit->assertIsString($message);
        return parent::rename($oldName,$newName,$force,$who,$message);
    }

    public function del(string $refName,?string $oldId,?string $oldTarget) : void {
        $this->called[] = 'del';
        $this->unit->assertIsString($refName);
        $this->unit->assertIsStringOrNull($oldId);
        $this->unit->assertIsStringOrNull($oldTarget);
        parent::del($refName,$oldId,$oldTarget);
    }

    public function compress() : void {
        $this->called[] = 'compress';
        parent::compress();
    }

    public function has_log(string $refname) : bool {
        $this->called[] = 'has_log';
        $this->unit->assertIsString($refname);
        return parent::has_log($refname);
    }

    public function ensure_log(string $refname) : void {
        $this->called[] = 'ensure_log';
        $this->unit->assertIsString($refname);
        parent::ensure_log($refname);
    }

    public function reflog_write($reflog) : void {
        $this->called[] = 'reflog_write';
        $this->unit->assertIsString($name);
        parent::reflog_write($name);
    }

    public function reflog_read($name) : array {
        $this->called[] = 'reflog_read';
        $this->unit->assertIsString($name);
        return parent::reflog_read($name);
    }

    public function reflog_rename(string $oldName,string $newName) : void {
        $this->called[] = 'reflog_rename';
        $this->unit->assertIsString($oldName);
        $this->unit->assertIsString($newName);
        parent::reflog_rename($oldName,$newName);
    }

    public function reflog_delete(string $name) : void {
        $this->called[] = 'reflog_delete';
        $this->unit->assertIsString($name);
        parent::reflog_delete($name);
    }
}
