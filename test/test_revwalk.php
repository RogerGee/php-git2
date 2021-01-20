<?php

namespace Git2Test\Revwalk;

require_once 'test_base.php';

function test_callback() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $ref = git_repository_head($repo);
    $walker = git_revwalk_new($repo);

    $lambda = function($commitId,$payload) {
        testbed_do_once('Hide Callback',function() use($commitId,$payload) {
            testbed_dump('callback:oid',$commitId);
            testbed_dump('callback:payload',$payload);
        });

        return $commitId[0] == 'a';
    };

    git_revwalk_add_hide_cb($walker,$lambda,33);
    git_revwalk_push($walker,git_reference_target($ref));

    while (git_revwalk_next($walker) !== false) {
        continue;
    }
}

testbed_test('Revwalk/callbacks','Git2Test\Revwalk\test_callback');
