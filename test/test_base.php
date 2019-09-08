<?php

function testbed_error($message) {
    color_echo("php-git2 testbed: error: $message",COLOR_RED);
}

function testbed_fatal_error($message) {
    testbed_error($message);
    exit(1);
}

function testbed_get_root() {
    static $root;

    if (is_null($root)) {
        $env = getenv('TEMPDIR');
        if (empty($env)) {
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

function testbed_path($suffix,$ensure = false) {
    $path = testbed_get_root() . '/' . $suffix;
    if ($ensure && !is_dir($path)) {
        mkdir($path);
    }
    return $path;
}

function testbed_create_clone_remote($repo,$name,$url,$payload) {
    $fetchspec = '+refs/heads/test/*:refs/remotes/origin/test/*';
    $remote = git_remote_create_with_fetchspec(
        $repo,
        $name,
        $url,
        $fetchspec);

    return $remote;
}

function testbed_get_repo_path() {
    $path = testbed_path('test-repo.git');
    if (!file_exists($path)) {
        $reposrc = git_repository_discover('.',false,null);

        $opts = array(
            'bare' => true,
            'checkout_branch' => 'test/testbed',
            'remote_cb' => 'testbed_create_clone_remote',
        );
        $repo = git_clone("file://$reposrc",$path,$opts);
    }
    return $path;
}

function testbed_get_localrepo_path() {
    $path = testbed_path('test-repo');
    if (!file_exists($path)) {
        $reposrc = git_repository_discover('.',false,null);

        $opts = array(
            'bare' => false,
            'checkout_branch' => 'test/testbed',
            'remote_cb' => 'testbed_create_clone_remote',
        );
        $repo = git_clone("file://$reposrc",$path,$opts);
    }
    return $path;
}

function testbed_do_once($context,callable $lambda) {
    static $once;

    if (!isset($once[$context])) {
        $lambda();
        $once[$context] = true;
    }
}

define('COLOR_RED',31);
define('COLOR_GREEN',32);
define('COLOR_YELLOW',33);
define('COLOR_BLUE',34);
function color_echo($message,$color = COLOR_BLUE,$prefix = 'git2: test: ') {
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
    echo "\n$prefix$message\n\n";
    if ($hasTerminal) {
        echo "\033[0m";
    }
}

function testbed_unit($section,$mixed) {
    $tab = '  ';
    color_echo("$section",COLOR_GREEN,"{$tab}unit: ");
    $tab = "$tab$tab";
    if ($mixed instanceof Exception) {
        $message = $mixed->getMessage();
        $file = pathinfo($mixed->getFile(),PATHINFO_FILENAME);
        $line = $mixed->getLine();
        $trace = '';
        foreach ($mixed->getTrace() as $elem) {
            if (isset($elem['file'])) {
                $elemFile = pathinfo($elem['file'],PATHINFO_FILENAME);
                $trace .= '  ' . $elem['function'] . ' (' . $elemFile . ')' . PHP_EOL;
            }
            else {
                $trace .= '  ' . $elem['function'] . PHP_EOL;
            }
        }
        $text = "[Exception] $file:$line: $message" . PHP_EOL
              . $trace;
    }
    else if (is_object($mixed)) {
        ob_start();
        var_dump($mixed);
        $text = ob_get_clean();
    }
    else if (!is_resource($mixed)) {
        $text = var_export($mixed,true);
    }
    else {
        $text = 'Resource of type "' . get_resource_type($mixed) . '"' . PHP_EOL;
    }

    echo "$tab => ";
    $lines = explode("\n",$text);
    if (count($lines) == 1) {
        echo $lines[0] . PHP_EOL;
    }
    else {
        echo PHP_EOL;
        for ($i = 0;$i < count($lines);++$i) {
            echo "$tab$tab" . $lines[$i] . PHP_EOL;
        }
    }
}

function testbed_modify_file($repo,$path) {
    if (git_repository_is_bare($repo)) {
        throw new Exception('testbed_modify_file: Repository must have working tree');
    }

    $repoPath = realpath(git_repository_path($repo) . DIRECTORY_SEPARATOR . '..');
    $path = $repoPath . DIRECTORY_SEPARATOR . $path;
    file_put_contents($path,'modification' . PHP_EOL,FILE_APPEND);
}

function testbed_test($title,callable $lambda) {
    color_echo("task '$title': started");

    try {
        $ret = $lambda();
    } catch (Exception $ex) {
        $error = 'An exception was thrown with message: ' . $ex->getMessage();
    }

    if (isset($error)) {
        color_echo("task '$title': failed\n    $error",COLOR_RED);
        exit(1);
    }
    else {
        color_echo("task '$title': completed");
    }

    return $ret;
}

function testbed_remove_recursive($path) {
    if (is_dir($path)) {
        // Out of an abundance of paranoia, we will make sure each file we
        // delete is prefixed exactly with the testbed root.
        $root = testbed_get_root();
        if (substr($path,0,strlen($root)) != $root) {
            throw new Exception('fail testbed_remove_recursive!');
        }

        $entries = array_diff(scandir($path),['..','.']);
        $files = [];
        $dirs = [];
        foreach ($entries as $ent) {
            $ent = implode(DIRECTORY_SEPARATOR,[$path,$ent]);

            if (is_dir($ent)) {
                testbed_remove_recursive($ent);
            }
            if (is_file($ent)) {
                unlink($ent);
            }
        }

        rmdir($path);
    }
}
