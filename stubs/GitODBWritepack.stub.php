<?php

abstract class GitODBWritepack {
    public function append(string $data,string &$stats) : void;

    public function commit(array &$stats) : void;
}

final class GitODBWritepack_Internal extends GitODBWritepack {

}
