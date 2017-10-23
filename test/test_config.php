<?php

namespace Git2Test\Config;

use Exception;

require_once 'test_base.php';

function test_get() {
    $config = git_config_open_default();

    try {
        $ent = git_config_get_entry($config,"user.email");
        var_dump($ent);
    } catch (Exception $ex) {
        echo "Config/get lookup user.email failed: " . $ex->getMessage() . PHP_EOL;
    }

    try {
        $ent = git_config_get_entry($config,"user.snail-mail");
        var_dump($ent);
    } catch (Exception $ex) {
        echo "Config/get lookup user.snail-mail failed: " . $ex->getMessage() . PHP_EOL;
    }
}

function test_get_indv() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $config = git_repository_config_snapshot($repo);

    try {
        $value = git_config_get_string($config,"remote.origin.url");
        var_dump($value);
    } catch (Exception $ex) {
        echo "Config/get_indv lookup remote.origin.url failed: " . $ex->getMessage() . PHP_EOL;
    }

    try {
        $value = git_config_get_string($config,"php.is.not.that.terrible");
        var_dump($value);
    } catch (Exception $ex) {
        echo "Config/get_indv lookup php.is.not.that.terrible failed: " . $ex->getMessage() . PHP_EOL;
    }
}

function test_set() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $config = git_repository_config($repo);

    try {
        git_config_set_bool($config,"thing.place.value",false);
        $val = git_config_get_bool($config,"thing.place.value");
        var_dump($val);
    } catch (Exception $ex) {
        echo "failed: " . $ex->getMessage() . PHP_EOL;
    }

    $path = git_repository_path($repo);
    $path .= 'config.alt';
    git_config_add_file_ondisk($config,$path,GIT_CONFIG_LEVEL_LOCAL,true);
    git_config_set_int32($config,"roger.gee.id",33);
}

function test_foreach() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $config = git_repository_config($repo);

    $lambda = function($entry,$payload) {
        var_dump($entry);
        var_dump($payload);
    };

    $quitearly = function($entry) {
        print "----BAIL----\n";
        var_dump($entry);
        print "------------\n";
        return 1;
    };

    git_config_foreach($config,$lambda,45);
    git_config_foreach($config,$quitearly,null);
    git_config_foreach_match($config,"remote",$lambda,33);
}

function test_get_mapped() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $config = git_repository_config($repo);

    git_config_set_string($config,"wasTested.a","yes");
    git_config_set_string($config,"wasTested.b","no");
    git_config_set_string($config,"wasTested.c","tested");

    $cvars = array(
        [GIT_CVAR_TRUE,null,1000],
        [GIT_CVAR_FALSE,null,1001],
        [GIT_CVAR_STRING,"tested",1002],
    );

    foreach (array('a','b','c') as $property) {
        $val = git_config_get_mapped($config,"wasTested.$property",$cvars);
        var_dump($val);
    }
}

testbed_test('Config/get','Git2Test\Config\test_get');
testbed_test('Config/get_indv','Git2Test\Config\test_get_indv');
testbed_test('Config/set','Git2Test\Config\test_set');
testbed_test('Config/foreach','Git2Test\Config\test_foreach');
testbed_test('Config/get_mapped','Git2Test\Config\test_get_mapped');
