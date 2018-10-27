<?php

require_once 'PHPSQLiteStore.php';

class PHPSQLiteODB extends GitODBBackend {
    private $conn;
    private $stmt = [];

    public function __construct(PHPSQLiteStore $store) {
        $this->conn = $store->getConnection();

        if ($store->shouldCreateTables()) {
            $this->conn->query('create table odb (type INTEGER, oid TEXT, data BLOB)');
        }

        $query = 'insert into odb (oid,type,data) values (:oid,:type,:data)';
        $this->stmt['write'] = $this->conn->prepare($query);

        $query = 'select type,data from odb where oid = :oid';
        $this->stmt['read'] = $this->conn->prepare($query);

        $query = 'select type from odb where oid = :oid';
        $this->stmt['exists'] = $this->conn->prepare($query);
    }

    public function read(&$type,$oid) {
        $this->stmt['read']->bindValue(':oid',$oid,SQLITE3_TEXT);
        $result = $this->stmt['read']->execute();
        $arr = $result->fetchArray();
        $this->stmt['read']->reset();

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
        $this->stmt['exists']->bindValue(':oid',$oid,SQLITE3_TEXT);
        $result = $this->stmt['exists']->execute();
        $arr = $result->fetchArray();
        $this->stmt['exists']->reset();

        return $arr !== false;
    }

    public function writepack($progressCallback,$payload) {
        // TODO
    }
}
