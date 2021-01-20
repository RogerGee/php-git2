<?php

class PHPEmptyODB extends GitODBBackend {
    public function for_each(callable $callback,$payload) {
        throw new Exception('Method is unimplemented in PHPEmptyODB');
    }
}
