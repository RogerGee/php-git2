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

        $query = 'select oid from odb';
        $this->stmt['list'] = $this->conn->prepare($query);
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
        $this->stmt['write']->bindValue(':oid',$oid,SQLITE3_TEXT);
        $this->stmt['write']->bindValue(':type',$type,SQLITE3_INTEGER);
        $this->stmt['write']->bindValue(':data',$data,SQLITE3_BLOB);
        $this->stmt['write']->execute();
        $this->stmt['write']->reset();
    }

    public function exists($oid) {
        $this->stmt['exists']->bindValue(':oid',$oid,SQLITE3_TEXT);
        $result = $this->stmt['exists']->execute();
        $arr = $result->fetchArray();
        $this->stmt['exists']->reset();

        return $arr !== false;
    }

    public function for_each(callable $callback,$payload) {
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

    public function writepack($progressCallback,$payload) {
        require_once 'PHPProxyWritepack.php';
        return new PHPProxyWritepack($this->odb);
    }

    public function count() {
        $query = 'select count(oid) from odb';
        $stmt = $this->conn->prepare($query);
        $result = $stmt->execute();
        $row = $result->fetchArray();

        return (int)$row[0];
    }
}
