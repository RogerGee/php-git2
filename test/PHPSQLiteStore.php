<?php

require_once 'test_base.php';

class PHPSQLiteStore {
    private $conn;
    private $createTables;

    public function __construct($dbname,$globalPath = false) {
        // Open SQLite database file.

        if (!$globalPath) {
            $sqlite3Path = testbed_path('SQLiteODBs');
            $path = implode(DIRECTORY_SEPARATOR,$sqlite3Path,$dbname);
        }
        else {
            $path = $dbname;
        }

        $this->conn = new SQLite3($path);

        // Figure out if the database has any tables.

        $result = $this->conn->query("SELECT name FROM sqlite_master
                                      WHERE type='table'
                                      ORDER BY name;");

        $results = $result->fetchArray();
        $this->createTables = $results === false;
    }

    public function getConnection() {
        return $this->conn;
    }

    public function shouldCreateTables() {
        return $this->createTables;
    }
}

if (!class_exists('SQLite3')) {
    testbed_error('Cannot run test: sqlite3 extension is not enabled');
}
