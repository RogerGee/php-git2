<?php

require_once 'test_base.php';

/**
 * Create a custom ODB backend that uses the PHP serialization.
 */
class PHPSerializedODB extends GitODBBackend {
    private $path;
    private $vars;

    public function __construct($name) {
        $this->path = testbed_path('PHPSerialized',true) . "/$name.odb.phpdb";

        if (is_file($this->path)) {
            $this->vars = unserialize(file_get_contents($this->path));
        }
        else {
            $this->vars = [];
        }
    }

    public function __destruct() {
        assert(!isset($this->odb));

        file_put_contents($this->path,serialize($this->vars));
        unset($this->vars);
    }

    public function read(&$type,$oid) {
        $type = $this->vars[$oid]['t'];
        return gzuncompress($this->vars[$oid]['d']);
    }

    public function write($oid,$data,$type) {
        $this->vars[$oid] = array(
            'd' => gzcompress($data),
            't' => $type,
        );
    }

    public function exists($oid) {
        return isset($this->vars[$oid]);
    }

    public function count() {
        return count($this->vars);
    }

    public function for_each($callback,$payload) {
        foreach (array_keys($this->vars) as $oid) {
            $callback($oid,$payload);
        }
    }
}

/**
 * Extend custom ODB backend to use custom stream implementation.
 */
class PHPSerializedODB_WithStream extends PHPSerializedODB {
    public function writestream($size,$type) {
        require_once 'PHPTestWritestream.php';
        return new PHPTestWritestream($size,$type);
    }
}

/**
 * Extend custom ODB backend to use custom writepack implementation.
 */
class PHPSerializedODB_WithWritepack extends PHPSerializedODB {
    public function writepack($odb,$callback,$payload) {
        require_once 'PHPTestWritepack.php';
        return new PHPTestWritepack($odb,$callback,$payload);
    }
}
