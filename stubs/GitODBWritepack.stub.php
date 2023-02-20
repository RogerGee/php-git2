<?php

abstract class GitODBWritepack {
    public function append(string $data,string &$stats) : void;

    public function commit(array &$stats) : void;
}

abstract class GitODBWritepack_Internal extends GitODBWritepack {

}
