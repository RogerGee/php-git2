<?php

require_once 'test_base.php';

class PHPSQLiteStore {
    private $conn;
    private $createTables;

    public function __construct($dbname) {
        $this->createTables = !file_exists($dbname);
        $this->conn = new SQLite3($dbname);

    }

    public function getConnection() {
        return $conn;
    }

    public function shouldCreateTables() {
        return $this->createTables;
    }
}

if (!class_exists('SQLite3')) {
    testbed_error('Cannot run test: sqlite3 extension is not enabled');
}
