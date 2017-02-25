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
        shell_exec("git clone --bare $reposrc $path");
    }
    return $path;
}

function testbed_test($title,callable $lambda) {
    try {
        ob_start();
        $ret = $lambda();
        $output = ob_get_clean();
    } catch (Exception $ex) {
        ob_get_clean();
        echo 'php-git2 testbed: error: ' . $ex->getMessage() . PHP_EOL;
        exit(1);
    }

    fwrite(STDOUT,"$title - \n\n");
    foreach (explode("\n",$output) as $line) {
        fwrite(STDOUT,"    $line\n");
    }
    fwrite(STDOUT,"\n");

    return $ret;
}
