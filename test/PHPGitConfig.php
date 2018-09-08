<?php

class PHPGitConfig extends GitConfigBackend {
    private $readOnly;
    private $path;
    private $backing;
    private $level;

    public function __construct($path,$readOnly = false) {
        $this->readOnly = $readOnly;
        $this->path = $path;
        $this->backing = [];
    }

    public function __destruct() {
        $this->commit();
    }

    public function open($level) {
        $this->level = $level;
        if (file_exists($this->path)) {
            $this->backing = unserialize(file_get_contents($this->path));
        }
    }

    public function get($name) {
        if (!isset($this->backing[$name])) {
            return false;
        }

        return ['value' => $this->backing[$name], 'name' => $name];
    }

    public function set($key,$value) {
        $this->backing[$key] = $value;
    }

    public function set_multivar($name,$regexp,$value) {

    }

    public function del($name) {
        $this->checkReadOnly();
        unset($this->backing[$name]);
    }

    public function del_multivar($name,$regexp) {
        $this->checkReadOnly();

    }

    public function iterator_new() {
        reset($this->backing);
    }

    public function iterator_next() {
        if (is_null(key($this->backing))) {
            return false;
        }

        $ent = array(
            'name' => key($this->backing),
            'value' => current($this->backing),
        );

        next($this->backing);

        return $ent;
    }

    public function snapshot() {
        $this->commit();
        $snapshot = new self($this->path,true);
        $snapshot->open($this->level);
        return $snapshot;
    }

    public function lock() {

    }

    public function unlock($success) {

    }

    private function commit() {
        file_put_contents($this->path,serialize($this->backing));
    }

    private function checkReadOnly() {
        if ($this->readOnly) {
            throw new Exception("PHPGitConfig is a snapshot");
        }
    }
}
