<?php

namespace Git2Test\Packbuilder;

require_once 'test_base.php';

function test_generate() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $build = git_packbuilder_new($repo);

    $commits[] = 'b12e60c8e3701d11fe78c27d67fa4343619d067b';
    $commits[] = '4cc0ee059fc5c949c3715bd0fca5357acff2323a';
    $commits[] = 'd129836a41a15fce409debb36c0d2a5503070b17';

    foreach ($commits as $oid) {
        git_packbuilder_insert_commit($build,$oid);
    }

    $pack = fopen(testbed_path('1.pack'),'w');
    git_packbuilder_foreach($build,null,$pack);

    git_packbuilder_free($build);
    git_repository_free($repo);
}

function test_callbacks() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $ref = git_repository_head($repo);
    $build = git_packbuilder_new($repo);

    $progressfunc = function($stage,$current,$total,$payload) {
        $vals = array_map(function($x){ return str_pad($x,4,' ',STR_PAD_LEFT); },
                          [$stage,$current,$total,$payload]);
        foreach ($vals as $v) {
            print $v;
        }
        print PHP_EOL;
    };

    git_packbuilder_set_callbacks($build,$progressfunc,33);
    git_packbuilder_insert_recur($build,git_reference_target($ref),null);

    $pack = fopen(testbed_path('2.pack'),'w');
    git_packbuilder_foreach($build,null,$pack);

    echo 'Setting callback to null...' . PHP_EOL;
    $build = git_packbuilder_new($repo);
    git_packbuilder_set_callbacks($build,null,null);
    git_packbuilder_insert_recur($build,git_reference_target($ref),null);
    $pack = fopen(testbed_path('2-1.pack'),'w');
    git_packbuilder_foreach($build,null,$pack);
}

function test_write() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $ref = git_repository_head($repo);
    $build = git_packbuilder_new($repo);

    git_packbuilder_insert_recur($build,git_reference_target($ref),null);
    $path = testbed_path('packdir1',true);

    $progressCallback = function(array $stats,$payload) {
        $keys = array_keys($stats);
        foreach ($keys as $key) {
            /**
             * NOTE: Skip this because of libgit2 bug where stats in
             * git_packbuilder_write is uninitialized. This ensures we don't get
             * uninitialized errors from valgrind. This value will contain
             * garbage, but since it's just an integer (and not a pointer that
             * will be dereferenced) it is a harmless issue.
             */
            if ($key == 'received_bytes') {
                continue;
            }

            print str_pad($stats[$key],4,' ',STR_PAD_LEFT);
        }
        print PHP_EOL;
        var_dump($payload);
    };

    git_packbuilder_write($build,$path,0660,$progressCallback,33);
    var_dump(git_packbuilder_written($build));
}

function test_write_cancel() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $build = git_packbuilder_new($repo);

    git_packbuilder_insert_recur($build,'ae65689605c57e3cad647da378a7654e3e081dab',null);
    $path = testbed_path('packdir2',true);

    $progressCallback = function(array $stats,$payload) {
        return false;
    };

    try {
        git_packbuilder_write($build,$path,0660,$progressCallback,null);
        var_dump(git_packbuilder_written($build));
    } catch (\Exception $ex) {
        echo 'Caught error: ' . $ex->getMessage() . PHP_EOL;
    }
}

testbed_test('Packbuilder/generate','Git2Test\Packbuilder\test_generate');
testbed_test('Packbuilder/callbacks','Git2Test\Packbuilder\test_callbacks');
testbed_test('Packbuilder/write','Git2Test\Packbuilder\test_write');
testbed_test('Packbuilder/write/cancel','Git2Test\Packbuilder\test_write_cancel');
