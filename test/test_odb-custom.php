<?php

namespace Git2Test\ODBCustom;

use GitODBBackend;

require_once 'test_base.php';

/**
 * Create a custom ODB backend that uses the PHP session.
 */
class SessionODB extends GitODBBackend {
    public function __construct() {
        if (session_status() == PHP_SESSION_NONE) {
            session_id('git2');
            session_start();
        }
    }

    public function read(&$type,$oid) {

    }

    public function write($oid,$data,$type) {

    }
}

function test_create() {
    $obj = new SessionODB;
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $odb = git_repository_odb($repo);

    git_odb_add_backend($odb,$obj,5);
}

testbed_test('Custom ODB/Create','Git2Test\ODBCustom\test_create');
