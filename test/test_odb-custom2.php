<?php

namespace Git2Test\ODBCustom2;

use GitODBBackend;
use GitODBStream;

/**
 * Create a custom ODB backend that uses the PHP session.
 */
class SessionODB extends GitODBBackend {
    public function __construct() {
        if (session_status() == PHP_SESSION_NONE) {
            session_id('git2-test-repo2');
            session_start();
        }
    }

    public function read(&$type,$oid) {
        $type = $_SESSION[$oid]['t'];
        return gzdeflate($_SESSION[$oid]['d']);
    }

    public function write($oid,$data,$type) {
        $_SESSION[$oid] = array(
            'd' => gzcompress($data),
            't' => $type,
        );
    }

    public function writestream($size,$type) {
        return new TestWritestream($size,$type);
    }

    public function exists($oid) {
        return isset($_SESSION[$oid]);
    }
}

/**
 * Create a custom GitODBStream for writing.
 */
class TestWritestream extends GitODBStream {
    private $filebuf;
    private $type;

    public function __construct($size,$type) {
        $this->filebuf = tmpfile();
        $this->type = $type;
    }

    public function write($buf) {
        fwrite($this->filebuf,$buf);
    }

    public function finalize_write($oid) {
        fseek($this->filebuf,0);
        //var_dump($this->backend);
        $this->backend->write($oid,stream_get_contents($this->filebuf),$this->type);
    }
}

function transfer_object($oid,$store) {
    list($src,$dst) = $store;

    $obj = git_odb_read($src,$oid);
    $data = git_odb_object_data($obj);
    $type = git_odb_object_type($obj);

    // This stream is a custom stream.
    $stream = git_odb_open_wstream($dst,strlen($data),$type);
    //var_dump($stream);
    //var_dump($stream->backend);

    $stream->write($data);
    $stream->finalize_write($oid);
    $stream->free();

    //echo "$oid\n";
}

function test_session_backend() {
    $backend = new SessionODB;
    $repo = git_repository_new();
    $odb = git_odb_new();

    git_odb_add_backend($odb,$backend,1);
    git_repository_set_odb($repo,$odb);

    if (!git_odb_exists($odb,'9182bfa491861a4ac38b955d0b7262b09d8ddbd6')) {
        // Create a blob in the repository.
        $data =<<<EOF
PHP is a general purpose programming language, especially well-suited for Web
development. It can be extended through writing extension modules that are either
compiled directly into the engine or loaded dynamically at runtime.
EOF;
        $oid = git_blob_create_frombuffer($repo,$data);
        echo "Created blob with oid=$oid\n";

        // Stream the objects from the test repository into our new repository.
        $srcrepo = git_repository_open_bare(testbed_get_repo_path());
        $srcodb = git_repository_odb($srcrepo);

        // $lambda = function($oid,$store) {
        //     list($src,$dst) = $store;

        //     $obj = git_odb_read($src,$oid);
        //     $data = git_odb_object_data($obj);
        //     $type = git_odb_object_type($obj);

        //     // This will work because git2 opens a fake wstream when the backend doesn't
        //     // have a direct wstream implementation.
        //     $stream = git_odb_open_wstream($dst,strlen($data),$type);
        //     $stream->write($data);
        //     $stream->finalize_write($oid);
        //     $stream->free();
        // };

        git_odb_foreach(
            $srcodb,/*$lambda*/'\Git2Test\ODBCustom2\transfer_object',
            [$srcodb,$odb]);

        echo 'Session has ' . count($_SESSION) . " entries.\n";
        //var_dump(array_keys($_SESSION));
    }
    else {
        echo "Operation was already performed. Delete the session to run again.\n";
    }
}

require_once 'test_base.php';
testbed_test('Custom ODB/Writestream','\Git2Test\ODBCustom2\test_session_backend');
