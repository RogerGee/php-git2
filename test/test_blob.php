<?php

namespace Git2Test\Blob;

require_once 'test_base.php';

function test_owner() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $blob = git_blob_lookup($repo,'063a11c4dc16ffaa1469b5f29ce83903f6cac77e');

    $repoSame = git_blob_owner($blob);

    $pathA = git_repository_path($repo);
    $pathB = git_repository_path($repoSame);
    echo "$pathA == $pathB\n";
    
    // This would raise an exception since the resource is not freeable.
    //git_repository_free($repoSame);
}

function test_print_content() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $blob = git_blob_lookup($repo,'063a11c4dc16ffaa1469b5f29ce83903f6cac77e');

    $content = git_blob_rawcontent($blob);
    $size = git_blob_rawsize($blob);

    echo "The blob size is $size.\n\n";
    var_dump($content);
}

function test_create() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $buffer = 'I made this blob in PHP!';

    $oid = git_blob_create_frombuffer($repo,$buffer);
    echo "The new blob has oid=$oid.\n\n";

    $blob = git_blob_lookup($repo,$oid);
    var_dump(git_blob_rawcontent($blob));
}

testbed_test('Blob/owner','Git2Test\Blob\test_owner');
testbed_test('Blob/print content','Git2Test\Blob\test_print_content');
testbed_test('Blob/create','Git2Test\Blob\test_create');
