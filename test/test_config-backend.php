<?php

namespace Git2Test\ConfigBackend;

use Exception;
use GitConfigBackend;

require_once 'test_base.php';

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

function test_custom_backend() {
    $path = testbed_get_root() . '/config-backend-test.config';
    $config = git_config_new();
    $backend = new PHPGitConfig($path);

    git_config_add_backend($config,$backend,GIT_CONFIG_LEVEL_APP,true);

    git_config_set_bool($config,"memcheck.enabled",true);
    git_config_set_int32($config,"processor.bits",64);
    git_config_set_int64($config,"speaker.volume",128);
    git_config_set_multivar($config,"multivar","[a-z]+","breakfast");
    git_config_set_multivar($config,"multivar","[0-9]+","8937452903857");
    git_config_set_string($config,"user.name","Roger Gee");
    git_config_set_string($config,"projects.php-git2","~/code/projects/php-git2");

    var_dump(git_config_get_string_buf($config,"user.name"));
    var_dump(git_config_get_path($config,"projects.php-git2"));

    $iter = git_config_iterator_new($config);
    while (true) {
        $ent = git_config_next($iter);
        if ($ent == false) {
            break;
        }
        var_dump($ent);
    }

    git_config_delete_entry($config,"processor.bits");

    $iter = git_config_iterator_new($config);
    while (true) {
        $ent = git_config_next($iter);
        if ($ent == false) {
            break;
        }
        var_dump($ent);
    }
}

testbed_test('Config Backend','Git2Test\ConfigBackend\test_custom_backend');
