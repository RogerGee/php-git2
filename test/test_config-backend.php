<?php

namespace Git2Test\ConfigBackend;

use Exception;
use PHPSerializedConfig;

require_once 'test_base.php';
require_once 'PHPSerializedConfig.php';

function test_custom_backend() {
    $config = git_config_new();
    $backend = new PHPSerializedConfig('test1');

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

function test_custom_backend_lifetime() {
    $inner = function($backend) {
        $config = git_config_new();
        git_config_add_backend($config,$backend,GIT_CONFIG_LEVEL_APP,true);

        $val = git_config_get_string_buf($config,"user.name");
        testbed_dump('get user.name',$val);
    };

    // Open the existing config store from a previous test.
    $backend = new PHPSerializedConfig('test1');

    // NOTE: each call to $inner creates a new git2 backend that references the
    // same object.

    $inner($backend);
    $inner($backend);
}

function test_custom_backend_lifetime2() {
    $inner = function() {
        // Open the existing config store from a previous test.
        $backend = new PHPSerializedConfig('test1');

        $config = git_config_new();
        git_config_add_backend($config,$backend,GIT_CONFIG_LEVEL_APP,true);

        return $config;
    };

    $config = $inner();

    $val = git_config_get_string_buf($config,'user.name');
    testbed_dump('get user.name',$val);
}

testbed_test('Config Backend','Git2Test\ConfigBackend\test_custom_backend');
testbed_test('Config Backend Lifetime','Git2Test\ConfigBackend\test_custom_backend_lifetime');
testbed_test('Config Backend Lifetime2','Git2Test\ConfigBackend\test_custom_backend_lifetime2');
