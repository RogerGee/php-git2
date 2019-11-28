<?php

require_once 'test_base.php';

/**
 * Create a custom GitODBStream for writing.
 */
class PHPTestWritestream extends GitODBStream {
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
        $this->backend->write($oid,stream_get_contents($this->filebuf),$this->type);
    }
}
