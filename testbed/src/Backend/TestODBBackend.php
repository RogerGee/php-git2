<?php

namespace PhpGit2\Backend;

use PhpGit2\Utility\TestWrapperTrait;

/**
 * Provides a custom ODB backend that wraps the in-memory serialized odb backend
 * in order to perform test assertions.
 */
class TestODBBackend extends PHPSerializedODBBackend {
    use TestWrapperTrait;

    public function read(&$type,string $oid) : string {
        $this->called[] = 'read';
        $this->unit->assertIsString($oid);
        return parent::read($type,$oid);
    }

    public function read_prefix(&$fullOid,&$type,string $abbrevOid) : string {
        $this->called[] = 'read_prefix';
        $this->unit->assertIsString($abbrevOid);
        return parent::read_prefix($fullOid,$type,$abbrevOid);
    }

    public function read_header(&$size,&$type,string $oid) : bool {
        $this->called[] = 'read_header';
        $this->unit->assertIsString($oid);
        return parent::read_header($size,$type,$oid);
    }

    public function write(string $oid,string $data,int $type) : void {
        $this->called[] = 'write';
        $this->unit->assertIsString($oid);
        $this->unit->assertIsString($data);
        $this->unit->assertIsInt($type);
        parent::write($oid,$data,$type);
    }

    public function exists(string $oid) : bool {
        $this->called[] = 'exists';
        $this->unit->assertIsString($oid);
        return parent::exists($oid);
    }

    public function exists_prefix(&$fullOid,string $prefix) : bool {
        $this->called[] = 'exists_prefix';
        $this->unit->assertIsString($prefix);
        return parent::exists_prefix($fullOid,$prefix);
    }

    public function for_each(callable $callback,mixed $payload = null) : void {
        $this->called[] = 'for_each';
        $this->unit->assertIsCallable($callback);
        parent::for_each($callback,$payload);
    }
}
