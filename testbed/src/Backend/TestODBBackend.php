<?php

namespace PhpGit2\Backend;

use PhpGit2\TestCase;

/**
 * Provides a custom ODB backend that wraps the in-memory serialized odb backend
 * in order to perform test assertions.
 */
class TestODBBackend extends PHPSerializedODBBackend {
    private $unit;
    private $called = [];

    public function __construct(TestCase $unit) {
        $this->unit = $unit;
    }

    public function read(&$type,$oid) {
        $this->called[] = 'read';
        $this->unit->assertIsString($oid);
        return parent::read($type,$oid);
    }

    public function read_prefix(&$fullOid,&$type,$abbrevOid) {
        $this->called[] = 'read_prefix';
        $this->unit->assertIsString($abbrevOid);
        return parent::read_prefix($fullOid,$type,$abbrevOid);
    }

    public function read_header(&$size,&$type,$oid) {
        $this->called[] = 'read_header';
        $this->unit->assertIsString($oid);
        return parent::read_header($size,$type,$oid);
    }

    public function write($oid,$data,$type) {
        $this->called[] = 'write';
        $this->unit->assertIsString($oid);
        $this->unit->assertIsString($data);
        $this->unit->assertIsInt($type);
        parent::write($oid,$data,$type);
    }

    public function exists($oid) {
        $this->called[] = 'exists';
        $this->unit->assertIsString($oid);
        return parent::exists($oid);
    }

    public function exists_prefix(&$fullOid,$prefix) {
        $this->called[] = 'exists_prefix';
        $this->unit->assertIsString($prefix);
        return parent::exists_prefix($fullOid,$prefix);
    }

    public function for_each($callback,$payload) {
        $this->called[] = 'for_each';
        $this->unit->assertIsCallable($callback);
        return parent::for_each($callback,$payload);
    }

    public function wasCalled(string $methodName) : bool {
        return in_array($methodName,$this->called);
    }
}
