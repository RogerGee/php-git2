<?php

namespace Git2Test\RefDB;

use PHPSerializedRefDB;

require_once 'test_base.php';
require_once 'PHPSerializedRefDB.php';

function test_builtin() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $refdb = git_repository_refdb($repo);
    $backend = git_refdb_backend_fs($repo);

    $ref = $backend->lookup('refs/heads/test/testbed');
    testbed_unit('lookup.name',git_reference_name($ref));
    testbed_unit('lookup.oid',git_reference_target($ref));
}

function test_compress() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $refdb = git_repository_refdb($repo);

    git_refdb_compress($refdb);

    echo 'Called git_refdb_compress()' . PHP_EOL;
}

function test_custom() {
    $storePath = testbed_path('global_store',true);
    $odbBackend = git_odb_backend_loose($storePath,5,false,0700,0600);

    $repo = git_repository_new();
    $backend = new PHPSerializedRefDB('test1');
    $refdb = git_refdb_new($repo);
    $odb = git_odb_new();

    git_refdb_set_backend($refdb,$backend);
    git_odb_add_backend($odb,$odbBackend,1);

    git_repository_set_refdb($repo,$refdb);
    git_repository_set_odb($repo,$odb);

    // Create a commit in the repository to ensure we have something to
    // reference.

    $builder = git_treebuilder_new($repo,null);
    $treeId = git_treebuilder_write($builder);
    git_treebuilder_free($builder);

    $author = git_signature_now('Testing Framework','testing@example.com');
    $commitId = git_commit_create(
        $repo,
        null,
        $author,
        $author,
        null,
        'This is a test commit for the refdb testing module.',
        git_tree_lookup($repo,$treeId),
        []);

    for ($i = 0;$i < 10;++$i) {
        $ref = git_reference_create($repo,"refs/test/r$i",$commitId,true,"");

        testbed_do_once('refdb/custom',function() use ($ref) {
            testbed_unit('created ref name',git_reference_name($ref));
            testbed_unit('created ref target',git_reference_target($ref));
        });
    }

    $ref = git_reference_lookup($repo,'refs/test/r5');
    $result = [
        'name' => git_reference_name($ref),
        'target' => git_reference_target($ref),
    ];
    testbed_unit('custom lookup',$result);
}

function test_custom_iteration() {
    $storePath = testbed_path('global_store',true);
    $odbBackend = git_odb_backend_loose($storePath,5,false,0700,0600);

    $repo = git_repository_new();
    $backend = new PHPSerializedRefDB('test1');
    $refdb = git_refdb_new($repo);
    $odb = git_odb_new();

    git_refdb_set_backend($refdb,$backend);
    git_odb_add_backend($odb,$odbBackend,1);

    git_repository_set_refdb($repo,$refdb);
    git_repository_set_odb($repo,$odb);

    $callback = function($ref,$payload) {
        $name = git_reference_name($ref);
        $target = git_reference_target($ref);

        testbed_unit('callback ref',"$name => $target");
    };

    git_reference_foreach($repo,$callback,null);
}

testbed_test('RefDB/builtin','Git2Test\RefDB\test_builtin');
testbed_test('RefDB/compress','Git2Test\RefDB\test_compress');
testbed_test('RefDB/custom','Git2Test\RefDB\test_custom');
testbed_test('RefDB/custom/iteration','Git2Test\RefDB\test_custom_iteration');
