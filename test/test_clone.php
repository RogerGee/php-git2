<?php

/**
 * NOTE: this is not ready to include in the test suite since writepack is
 * required to clone using a custom ODB backend.
 */

namespace Git2Test\Cloning;

use PHPSQLiteODB;
use PHPSQLiteStore;

require_once 'test_base.php';
require_once 'PHPSQLiteODB.php';
require_once 'PHPSQLiteStore.php';

function do_clone($path,array $opts) {
    testbed_remove_recursive($path);
    return git_clone(testbed_get_repo_path(),$path,$opts);
}

function test_clone() {
    $opts1 = array(
        'base' => true,
    );

    $opts2 = array(
        'base' => false,
        'remote_cb' => function($repo,$name,$url,$payload) {
            testbed_unit('remote_create_cb', [
                'repo' => $repo,
                'name' => $name,
                'url' => $url,
                'payload' => $payload,
            ]);

            $name = 'barg';

            return git_remote_create($repo,$name,$url);
        },
        'remote_cb_payload' => 'REMOTE CREATE!',
    );

    $opts3 = array(
        'bare' => true,
        'repository_cb' => function($path,$bare,$payload) {
            testbed_unit('repository_create_cb', [
                'path' => $path,
                'bare' => $bare,
                'payload' => $payload,
            ]);

            $repo = git_repository_init($path,$bare);
            $odb = git_odb_new();

            $store = new PHPSQLiteStore("$path/odb.sqlite3",true);

            $backend = new PHPSQLiteODB($store);
            git_odb_add_backend($odb,$backend,1);
            git_repository_set_odb($repo,$odb);

            return $repo;
        },
        'repository_cb_payload' => 'CLONES!',
    );

    $basePath = testbed_path('clones',true);

    // Using openssl causes leaks to be reported and also some invalid accesses
    // in git2...
    //$repoA = git_clone("https://github.com/RogerGee/minecontrol.git","$basePath/minecontrol",$opts);
    //$repoB = git_clone("https://github.com/RogerGee/compile.git","$basePath/compile",$opts);

    do_clone("$basePath/php-git2-1",$opts1);
    do_clone("$basePath/php-git2-2",$opts2);
}

testbed_test('Cloning\test_clone','\Git2Test\Cloning\test_clone');
