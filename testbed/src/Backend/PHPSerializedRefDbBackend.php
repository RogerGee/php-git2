<?php

namespace PhpGit2\Backend;

class PHPSerializedRefDbBackend extends \GitRefDBBackend {
    private $storage = [];

    /**
     * Implements GitRefDbBackend::exists().
     *
     * @param string $refName
     *
     * @return bool
     */
    public function exists($refName) {
        return isset($this->storage[$refName]);
    }

    /**
     * Implements GitRefDbBackend::lookup().
     *
     * @param string $refName
     *
     * @return string
     */
    public function lookup($refName) {
        return $this->storage[$refName];
    }

    /**
     * Implements GitRefDbBackend::iterator_new().
     *
     * @param string $glob
     */
    public function iterator_new($glob) {
        reset($this->storage);
    }

    /**
     * Implements GitRefDbBackend::iterator_next().
     *
     * @param string &$name
     *
     * @return string
     */
    public function iterator_next(&$name) {
        $name = key($this->storage);
        $oid = current($this->storage);

        next($this->storage);

        return $oid;
    }

    /**
     * Implements GitRefDbBackend::write().
     *
     * @param resource $ref
     * @param bool $force
     * @param array $who
     * @param string $message
     * @param string $old
     * @param string $oldTarget
     */
    public function write($ref,$force,$who,$message,$old,$oldTarget) {
        $name = git_reference_name($ref);
        $target = git_reference_target($ref);

        if (isset($this->storage[$name]) && !$force) {
            throw new Exception("Reference with name '$name' already exists");
        }

        if (isset($this->storage[$name])) {
            self::verifyRef($this->storage[$name],$old,$oldTarget);
        }

        $this->storage[$name] = $target;
    }

    /**
     * Implements GitRefDbBackend::rename().
     *
     * @param string $oldName
     * @param string $newName
     * @param bool $force
     * @param array $who
     * @param string $message
     */
    public function rename($oldName,$newName,$force,$who,$message) {
        if (!isset($this->storage[$oldName])) {
            throw new Exception("Reference with name '$oldName' does not exist");
        }

        if (isset($this->storage[$newName]) && !$force) {
            throw new Exception("Reference with name '$newName' already exists");
        }

        $this->storage[$newName] = $this->storage[$oldName];
        unset($this->storage[$oldName]);
    }

    /**
     * Implements GitRefDbBackend::del().
     *
     * @param string $refName
     * @param string $oldId
     * @param string $oldTarget
     */
    public function del($refName,$oldId,$oldTarget) {
        if (!isset($this->storage[$refName])) {
            throw new Exception("Reference with name '$refName' does not exist");
        }

        $oid = $this->storage[$refName];
        self::verifyRef($oid,$oldId,$oldTarget);

        unset($this->storage[$refName]);
    }

    /**
     * Implements GitRefDbBackend::compress().
     */
    public function compress() {
        // Compression is not relevant here.
    }

    /**
     * Implements GitRefDbBackend::has_log().
     */
    public function has_log($refname) {
        throw new Exception('Reflogs are not implemented by this backend');
    }

    /**
     * Implements GitRefDbBackend::ensure_log().
     */
    public function ensure_log($refname) {
        throw new Exception('Reflogs are not implemented by this backend');
    }

    /**
     * Implements GitRefDbBackend::reflog_write().
     */
    public function reflog_write($name) {
        throw new Exception('Reflogs are not implemented by this backend');
    }

    /**
     * Implements GitRefDbBackend::reflog_read().
     */
    public function reflog_read($name) {
        throw new Exception('Reflogs are not implemented by this backend');
    }

    /**
     * Implements GitRefDbBackend::reflog_rename().
     */
    public function reflog_rename($oldName,$newName) {
        throw new Exception('Reflogs are not implemented by this backend');
    }

    /**
     * Implements GitRefDbBackend::reflog_delete().
     */
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
