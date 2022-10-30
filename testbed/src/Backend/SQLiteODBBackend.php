<?php

namespace PhpGit2\Backend;

use PhpGit2\TestCase;
use PhpGit2\Writepack\TmpODBWritepack;

/**
 * Provides a custom ODB backend that uses a sqlite3 database. This class
 * requires the sqlite3 php extension.
 */
class SQLiteODBBackend extends \GitODBBackend {
    private $conn;
    private $stmt = [];

    /**
     * Creates a new SQLiteODBBackend instance.
     *
     * @param string $path
     *  The path of the database file to open.
     */
    public function __construct(string $path) {
        $this->conn = new \SQLite3($path);
        $this->initDb();

        $query = 'INSERT INTO odb (oid,type,data) VALUES (:oid,:type,:data)';
        $this->stmt['write'] = $this->conn->prepare($query);

        $query = 'SELECT type,data FROM odb WHERE oid = :oid';
        $this->stmt['read'] = $this->conn->prepare($query);

        $query = 'SELECT type FROM odb WHERE oid = :oid';
        $this->stmt['exists'] = $this->conn->prepare($query);

        $query = 'SELECT oid FROM odb';
        $this->stmt['list'] = $this->conn->prepare($query);
    }

    /**
     * Implements GitODBBackend::read().
     *
     * @param int &$type
     *  Returns the type of the object.
     * @param string $oid
     *  The ID of the object to read.
     *
     * @return string
     *  The object contents.
     */
    public function read(&$type,string $oid) : string {
        $this->stmt['read']->bindValue(':oid',$oid,SQLITE3_TEXT);
        $result = $this->stmt['read']->execute();
        $arr = $result->fetchArray();
        $this->stmt['read']->reset();

        $type = $arr[0];
        return $arr[1];
    }

    /**
     * Implements GitODBBackend::write().
     *
     * @param string $oid
     *  The OID of the object to write.
     * @param string $data
     *  The object contents.
     * @param int $type
     *  The type of the objects.
     */
    public function write(string $oid,string $data,int $type) : void {
        $this->stmt['write']->bindValue(':oid',$oid,SQLITE3_TEXT);
        $this->stmt['write']->bindValue(':type',$type,SQLITE3_INTEGER);
        $this->stmt['write']->bindValue(':data',$data,SQLITE3_BLOB);
        $this->stmt['write']->execute();
        $this->stmt['write']->reset();
    }

    /**
     * Implements GitODBBackend::exists().
     *
     * @param string $oid
     *  The OID of the object to write.
     *
     * @return bool
     */
    public function exists(string $oid) : bool {
        $this->stmt['exists']->bindValue(':oid',$oid,SQLITE3_TEXT);
        $result = $this->stmt['exists']->execute();
        $arr = $result->fetchArray();
        $this->stmt['exists']->reset();

        return $arr !== false;
    }

    /**
     * Implements GitODBBackend::for_each().
     *
     * @param callable $callback
     *  The callback to execute for each object.
     * @param mixed $payload
     *  The payload to pass through.
     */
    public function for_each(callable $callback,$payload) : void {
        $result = $this->stmt['list']->execute();
        while (true) {
            $row = $result->fetchArray();
            if ($row === false) {
                break;
            }
            if ($callback($row[0],$payload) === false) {
                break;
            }
        }
    }

    /**
     * Implements GitODBBackend::writepack().
     *
     * @param callable $progressCallback
     * @param mixed $payload
     *
     * @return \GitODBWritepack
     */
    public function writepack(callable $progressCallback,$payload) : \GitODBWritepack {
        $path = $payload;
        if (empty($path) || !is_string($path)) {
            $path = TestCase::makeDirectory('writepack','SQLiteODBBackend');
        }

        /**
         * uses GitODBBackend::$odb
         */
        return new TmpODBWritepack($path,$this->odb);
    }

    /**
     * Gets the number of objects in the ODB.
     *
     * @return int
     */
    public function count() : int {
        $query = 'SELECT count(oid) FROM odb';
        $stmt = $this->conn->prepare($query);
        $result = $stmt->execute();
        $row = $result->fetchArray();

        return $row[0];
    }

    private function initDB() : void {
        $sql = "SELECT name FROM sqlite_master
                WHERE type='table'
                ORDER BY name;";

        $result = $this->conn->query($sql);
        if (empty($result)) {
            $sql = 'CREATE TABLE odb (type INTEGER, oid TEXT, data BLOB)';
            $this->conn->exec($sql);

            $sql = 'CREATE UNIQUE INDEX idx_odb_oid ON odb (oid,type)';
            $this->conn->exec($sql);
        }
    }
}
