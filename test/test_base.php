<?php

function testbed_fatal_error($message) {
    fwrite(STDERR,"php-git2 testbed: fatal error: $message" . PHP_EOL);
    exit(1);
}

function testbed_get_root() {
    static $root;

    if (is_null($root)) {
        $env = getenv('TEMPDIR');
        if (is_null($env)) {
            $env = '/tmp';
        }

        $root = "$env/.php-git2-testbed";
        if (!file_exists($root)) {
            mkdir($root);
        }
        else if (!is_dir($root)) {
            testbed_fatal_error('testbed root directory already exists as something other than a directory');
        }
    }

    return $root;
}

function testbed_path($suffix) {
    return testbed_get_root() . '/' . $suffix;
}

function testbed_get_repo_path() {
    $path = testbed_path('test-repo.git');
    if (!file_exists($path)) {
        $reposrc = git_repository_discover('.',false,null);

        // TODO: replace with git2 functionality instead of external process
        // invocation.
        shell_exec("git clone --bare $reposrc $path");
    }
    return $path;
}

define('COLOR_RED',31);
define('COLOR_GREEN',32);
define('COLOR_YELLOW',33);
define('COLOR_BLUE',34);
function color_echo($message,$color = COLOR_BLUE) {
    static $hasTerminal = null;

    if (is_null($hasTerminal)) {
        if (function_exists('posix_isatty')) {
            if (posix_isatty(STDOUT)) {
                $hasTerminal = true;
            }
        }
    }

    if ($hasTerminal) {
        echo "\033[1m\033[{$color}m";
    }
    echo "\ngit2: test: $message\n\n";
    if ($hasTerminal) {
        echo "\033[0m";
    }
}

function testbed_test($title,callable $lambda) {
    color_echo("task '$title': started");

    try {
        $ret = $lambda();
    } catch (Exception $ex) {
        $error = 'php-git2 testbed: error: ' . $ex->getMessage() . PHP_EOL;
    }

    if (isset($error)) {
        color_echo("task '$title': error",COLOR_RED);
        exit(1);
    }
    else {
        color_echo("task '$title': completed");
    }

    return $ret;
}
