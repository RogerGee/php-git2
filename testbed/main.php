<?php

use PHPUnit\TextUI\Command as PHPUnitCommand;

require_once('vendor/autoload.php');
define('PHPGIT2_BRANCH_VERSION',7);

function run_php_unit(array $userArgs) : int {
    $hasGit2 = extension_loaded('git2');
    if (!$hasGit2) {
        error_log("");
        return 1;
    }

    $original = getcwd();
    $testdir = getenv('PHPGIT2_TESTDIR');
    chdir($testdir);

    if (empty($userArgs)) {
        $userArgs[] = 'src/Test';
    }

    $args = [
        'phpunit',
        ...$userArgs,
    ];

    $app = new PHPUnitCommand;
    $result = $app->run($args,false);

    chdir($original);

    return $result;
}

function run_php_unit_with_ini(string $php,array $userArgs) : int {
    $original = getcwd();
    $testdir = getenv('PHPGIT2_TESTDIR');
    assert(is_dir($testdir));

    if (empty($userArgs)) {
        $userArgs[] = 'src/Test';
    }

    $args = [
        $php,
        '-c',
        "$testdir/php.ini",
        'vendor/bin/phpunit',
        ...$userArgs,
    ];

    $descriptors = [
        0 => STDIN,
        1 => STDOUT,
        2 => STDOUT,
    ];

    $proc = proc_open($args,$descriptors,$pipes,$testdir);
    $status = proc_close($proc);

    return $status;
}

function discover_project() : string {
    // Discover base directory of php-git2 distribution.

    $dir = getcwd();

    $moduledir = implode(DIRECTORY_SEPARATOR,[$dir,'modules']);
    if (is_dir($moduledir)) {
        return $dir;
    }

    $dir = realpath(implode(DIRECTORY_SEPARATOR,[$dir,'..']));
    $moduledir = implode(DIRECTORY_SEPARATOR,[$dir,'modules']);
    if (is_dir($dir)) {
        return $dir;
    }

    throw new RuntimeError(
        'Cannot discover project directory: is the test command '
        .'running from a php-git2 source tree?'
    );
}

function set_test_env(string $projectdir) : string {
    $testdir = implode(DIRECTORY_SEPARATOR,[$projectdir,'testbed']);
    putenv("PHPGIT2_TESTDIR=$testdir");
    return $testdir;
}

function main(string $php = 'php',string ...$args) : int {
    if (PHP_MAJOR_VERSION < PHPGIT2_BRANCH_VERSION) {
        throw new RuntimeError('This project does not support PHP ' . PHP_MAJOR_VERSION . '.');
    }
    if (PHP_MAJOR_VERSION > PHPGIT2_BRANCH_VERSION) {
        throw new RuntimeError(
            'Warning! You are running PHP '
              . PHP_MAJOR_VERSION
              . ', but this branch supports PHP '
              . PHPGIT2_BRANCH_VERSION
        );
    }

    // Run directly for child process.
    if (getenv('PHPGIT2_TESTDIR')) {
        return run_php_unit();
    }

    $hasGit2 = extension_loaded('git2');

    $projectdir = getenv('PHPGIT2_BASEDIR');
    if (!$projectdir) {
        $projectdir = discover_project();
        putenv("PHPGIT2_BASEDIR=$projectdir");
    }

    set_test_env($projectdir);
    if ($hasGit2) {
        // Run under current environment if the extension is already loaded.
        return run_php_unit($args);
    }

    // Run under environment configured via our php.ini file. This will ensure
    // the git2 extension is loaded (assuming it is built).
    return run_php_unit_with_ini($php,$args);
}

try {
    $status = main('php',...array_slice($argv,1));
    exit($status);
} catch (\Exception $ex) {
    error_log($argv[0] . ': ' . $ex->getMessage());
    exit(1);
}
