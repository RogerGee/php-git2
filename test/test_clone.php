<?php

/**
 * NOTE: this is not ready to include in the test suite since writepack is
 * required to clone using a custom ODB backend.
 */

namespace Git2Test\Cloning;

use PHPSQLiteODB;

require_once 'test_base.php';
require_once 'PHPSQLiteODB.php';

function do_clone($path,array $opts) {
    testbed_remove_recursive($path);
    return git_clone(testbed_get_repo_path(),$path,$opts);
}

function test_clone() {
    $opts = array(
        'bare' => true,
        'repository_cb' => function($path,$bare,$payload) {
            testbed_unit('repository_create_cb', [
                'path' => $path,
                'bare' => $bare,
                'payload' => $payload,
            ]);

            $repo = git_repository_init($path,$bare);
            $odb = git_odb_new();

            $backend = new PHPSQLiteODB("$path-odb.sqlite3");
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

    do_clone("$basePath/php-git2",$opts);
}

testbed_test('Cloning\test_clone','\Git2Test\Cloning\test_clone');
