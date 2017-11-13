<?php

/**
 * NOTE: this is not ready to include in the test suite since writepack is
 * required to clone using a custom ODB backend.
 */

namespace Git2Test\Cloning;

use GitODBBackend;
use SQLite3;

require_once 'test_base.php';

class SQLiteBackend extends GitODBBackend {
    private $conn;
    private $stmtWrite;

    public function __construct($dbname) {
        $createTables = file_exists($dbname);
        $this->conn = new SQLite3($dbname);

        if (!$createTables) {
            $this->conn->query('create table odb (type INTEGER, oid TEXT, data BLOB)');
        }

        $query = 'insert into odb (oid,type,data) values (:oid,:type,:data)';
        $this->stmtWrite = $this->conn->prepare($query);
    }

    public function read(&$type,$oid) {
        $result = $this->conn->query("select type,data from odb where oid = '$oid'");
        $arr = $result->fetchArray();

        $type = $arr[0];
        return $arr[1];
    }

    public function write($oid,$data,$type) {
        $this->stmtWrite->bindValue(':oid',$oid,SQLITE3_TEXT);
        $this->stmtWrite->bindValue(':type',$type,SQLITE3_INTEGER);
        $this->stmtWrite->bindValue(':data',$data,SQLITE3_BLOB);
        $this->stmtWrite->execute();
        $this->stmtWrite->reset();
    }

    public function exists($oid) {
        $result = $this->conn->query("select type from odb where oid = '$oid'");
        return $result->fetchArray() !== false;
    }

    public function writepack($progressCallback,$payload) {
        // TODO
    }
}

function test_clone() {
    if (!class_exists('\SQLite3')) {
        testbed_error('Cannot run test: sqlite3 extension is not enabled');
        return;
    }

    $opts = array(
        'bare' => true,
        'repository_cb' => function($path,$bare,$payload) {
            var_dump($path);
            var_dump($bare);
            var_dump($payload);

            $repo = git_repository_init($path,$bare);
            $odb = git_odb_new();

            $backend = new SQLiteBackend("$path-odb.sqlite3");
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

    $repo = git_clone(testbed_get_repo_path(),"$basePath/php-git2",$opts);
}

testbed_test('Cloning\test_clone','\Git2Test\Cloning\test_clone');
