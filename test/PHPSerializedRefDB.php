<?php

require_once 'test_base.php';

class PHPSerializedRefDB extends GitRefDBBackend {
    private $path;
    private $store;

    public function __construct($name) {
        $this->path = testbed_path('PHPSerialized',true) . "/$name";

        if (is_file($this->path)) {
            $this->store = unserialize(file_get_contents($this->path));
        }
        else {
            $this->store = [];
        }
    }

    public function __destruct() {
        file_put_contents($this->path,serialize($this->store));
    }

    public function exists($refName) {
        return isset($this->store[$refName]);
    }

    public function lookup($refName) {
        return $this->store[$refName];
    }

    public function iterator_new($glob) {
        reset($this->store);
    }

    public function iterator_next(&$name) {
        $name = key($this->store);
        $oid = next($this->store);

        return $oid;
    }

    public function write($ref,$force,$who,$message,$old,$oldTarget) {
        $name = git_reference_name($ref);
        $target = git_reference_target($ref);

        if (isset($this->store[$name]) && !$force) {
            throw new Exception("Reference with name '$name' already exists");
        }

        if (isset($this->store[$name])) {
            self::verifyRef($this->store[$name],$old,$oldTarget);
        }

        $this->store[$name] = $target;
    }

    public function rename($oldName,$newName,$force,$who,$message) {
        if (!isset($this->store[$oldName])) {
            throw new Exception("Reference with name '$oldName' does not exist");
        }

        if (isset($this->store[$newName]) && !$force) {
            throw new Exception("Reference with name '$newName' already exists");
        }

        $this->store[$newName] = $this->store[$oldName];
        unset($this->store[$oldName]);
    }

    public function del($refName,$oldId,$oldTarget) {
        if (!isset($this->store[$refName])) {
            throw new Exception("Reference with name '$refName' does not exist");
        }

        $oid = $this->store[$refName];
        self::verifyRef($oid,$oldId,$oldTarget);

        unset($this->store[$refName]);
    }

    public function compress() {
        // We don't offer compression in this version...
    }

    public function has_log($refname) {
        throw new Exception('Reflogs are not implemented by this backend');
    }

    public function ensure_log($refname) {
        throw new Exception('Reflogs are not implemented by this backend');
    }

    public function reflog_write($name) {
        throw new Exception('Reflogs are not implemented by this backend');
    }

    public function reflog_read($name) {
        throw new Exception('Reflogs are not implemented by this backend');
    }

    public function reflog_rename($old_name,$new_name) {
        throw new Exception('Reflogs are not implemented by this backend');
    }

    public function reflog_delete($name) {
        throw new Exception('Reflogs are not implemented by this backend');
    }

    private static function isSymbolic($value) {
        return substr($value,0,5) == "ref: ";
    }

    private static function verifyRef($existing,$oldId,$oldTarget) {
        if (self::isSymbolic($existing)) {
            return $existing == $oldTarget;
        }

        return $existing == $oldId;
    }
}
