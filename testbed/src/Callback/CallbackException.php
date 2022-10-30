<?php

namespace PhpGit2\Callback;

use Exception;

class CallbackException extends Exception {
    public function __construct(
        string $message = 'Test exception for callbacks',
        int $code = -1)
    {
        parent::__construct($message,$code);
    }
}
