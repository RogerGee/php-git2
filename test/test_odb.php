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

function test_read_stream() {
    $repoPath = testbed_get_repo_path();
    $repo = git_repository_open($repoPath);
    $odb = git_repository_odb($repo);

    // NOTE: Creating a read stream should fail since default ODB backends do
    // not support read streaming.
    try {
        $oid = '69d56efa8f56450c00041bd523dd19b24e7a39ec';
        $stream = git_odb_open_rstream($odb,$oid);
    } catch (\Exception $ex) {
        testbed_dump('Exception',$ex->getMessage());
    }
}

function test_write_stream() {
    $repoPath = testbed_get_repo_path();
    $repo = git_repository_open($repoPath);
    $odb = git_repository_odb($repo);

    $stream = git_odb_open_wstream($odb,120,GIT_OBJ_BLOB);
    testbed_dump('stream',$stream);
    testbed_dump('stream->mode',$stream->mode);
    testbed_dump('stream->declared_size',$stream->declared_size);
    testbed_dump('stream->received_bytes',$stream->received_bytes);

    // NOTE: We apparently have to use the high-level function in this case.
    git_odb_stream_write($stream,str_repeat('roger',24));

    testbed_dump('stream->received_bytes',$stream->received_bytes);

    $oid = git_odb_stream_finalize_write($stream);
    testbed_dump('result',$oid);
}

function test_stream_lifetime() {
    $lambda = function() {
        $repoPath = testbed_get_repo_path();
        $repo = git_repository_open($repoPath);
        $odb = git_repository_odb($repo);

        testbed_dump('odb:before',$odb);

        $stream = git_odb_open_wstream($odb,120,GIT_OBJ_BLOB);
        git_odb_free($odb);

        testbed_dump('odb:after',$odb);

        return $stream;
    };

    $stream = $lambda();

    testbed_dump('stream',$stream);
    testbed_dump('stream:backend',$stream->backend);
    testbed_dump('stream:backend:odb',$stream->backend->odb);

    git_odb_stream_write($stream,str_repeat('aaaaa',24));
    $oid = git_odb_stream_finalize_write($stream);

    testbed_dump('result',$oid);
}

function test_foreach() {
    $repoPath = testbed_get_repo_path();
    $repo = git_repository_open($repoPath);
    $odb = git_repository_odb($repo);

    $iters = 0;
    $lambda = function($oid,$payload) use (&$iters) {
        $iters += 1;
    };

    git_odb_foreach($odb,$lambda,new \stdClass);
    testbed_dump('iterations',$iters);
}

function test_foreach_break() {
    $repoPath = testbed_get_repo_path();
    $repo = git_repository_open($repoPath);
    $odb = git_repository_odb($repo);

    $iters = 0;
    $lambda = function($oid,$payload) use (&$iters) {
        $iters += 1;
        return false;
    };

    git_odb_foreach($odb,$lambda,new \stdClass);
    testbed_dump('iterations',$iters);
}

testbed_test('ODB/Read Header','Git2Test\ODB\test_read_header');
testbed_test('ODB/Read Prefix','Git2Test\ODB\test_read_prefix');
testbed_test('ODB/Exists Prefix','Git2Test\ODB\test_exists_prefix');
testbed_test('ODB/Read Stream','Git2Test\ODB\test_read_stream');
testbed_test('ODB/Write Stream','Git2Test\ODB\test_write_stream');
testbed_test('ODB/Stream Lifetime','Git2Test\ODB\test_stream_lifetime');
testbed_test('ODB/Foreach','Git2Test\ODB\test_foreach');
testbed_test('ODB/Foreach Break','Git2Test\ODB\test_foreach_break');
