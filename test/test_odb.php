<?php

namespace Git2Test\ODB;

require_once 'test_base.php';

function test_read_header() {
    $repoPath = testbed_get_repo_path();
    $repo = git_repository_open($repoPath);
    $odb = git_repository_odb($repo);

    $n = git_odb_num_backends($odb);
    echo "Number of backends in odb: $n\n\n";

    $oid = '69d56efa8f56450c00041bd523dd19b24e7a39ec';
    $type = 4444444;
    $size = git_odb_read_header($type,$odb,$oid);

    echo "Read header for $oid\n";
    var_dump($size);
    var_dump($type);
    echo 'GIT_OBJ_COMMIT == ' . GIT_OBJ_COMMIT . PHP_EOL;
}

function test_read_prefix() {
    $repoPath = testbed_get_repo_path();
    $repo = git_repository_open($repoPath);
    $odb = git_repository_odb($repo);

    $poid = '69d56ef';
    $obj = git_odb_read_prefix($odb,$poid);

    var_dump(git_odb_object_size($obj));
    var_dump(git_odb_object_type($obj));
    var_dump(git_odb_object_data($obj));
}

function test_exists_prefix() {
    $repoPath = testbed_get_repo_path();
    $repo = git_repository_open($repoPath);
    $odb = git_repository_odb($repo);

    $poid = '69d56ef';
    $result = git_odb_exists_prefix($full,$odb,$poid);

    var_dump($result);
    var_dump($full);
}

testbed_test('ODB READ HEADER','Git2Test\ODB\test_read_header');
testbed_test('ODB READ PREFIX','Git2Test\ODB\test_read_prefix');
testbed_test('ODB EXISTS PREFIX','Git2Test\ODB\test_exists_prefix');
