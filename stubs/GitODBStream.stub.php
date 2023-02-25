<?php

abstract class GitODBStream {
    public function read(int $bufsz) : string;

    public function write(string $buffer) : void;

    public function finalize_write(string $oid) : void;
}

final class GitODBStream_Internal extends GitODBStream {

}
