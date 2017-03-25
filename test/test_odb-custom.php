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
        return $_SESSION[$oid]['data'];
    }

    public function write($oid,$data,$type) {
        $_SESSION[$oid] = array(
            'data' => $data,
            'type' => $type,
        );
    }

    public function exists($oid) {
        return isset($_SESSION[$oid]);
    }
}

function test_create() {
    $obj = new SessionODB;
    $repo = git_repository_new();
    $odb = git_odb_new();

    git_odb_add_backend($odb,$obj,5);
    git_repository_set_odb($repo,$odb);
}

testbed_test('Custom ODB/Create','Git2Test\ODBCustom\test_create');
