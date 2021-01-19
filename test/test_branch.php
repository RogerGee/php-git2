<?php

namespace Git2Test\Branch;

require_once 'test_base.php';

function test_create() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $ref = git_repository_head($repo);
    $commit = git_commit_lookup($repo,git_reference_target($ref));

    $newbranch = git_branch_create($repo,"life-and-limb",$commit,true);
    testbed_dump('create',$newbranch);
    testbed_dump('create:is_head',git_branch_is_head($newbranch));

    $delete = git_branch_create($repo,"delete-me",$commit,true);
    testbed_dump('create:delete-me:before',$delete);
    git_branch_delete($delete);
    testbed_dump('create:delete-me:after',$delete);
}

function test_lookup() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $branch = git_branch_lookup($repo,"life-and-limb",GIT_BRANCH_ALL);
    testbed_dump('lookup',$branch);

    testbed_dump('lookup:is_head',git_branch_is_head($branch));
    testbed_dump('lookup:name',git_branch_name($branch));

    $cpy = git_branch_move($branch,"life-and-limb-born-again",true);
    testbed_dump('lookup:moved',$cpy);
    testbed_dump('lookup:moved:name',git_branch_name($cpy));
}

function test_iterate() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $iter = git_branch_iterator_new($repo,GIT_BRANCH_ALL);

    while (true) {
        $branch = git_branch_next($type,$iter);
        if ($branch === false) {
            break;
        }

        testbed_dump('iterate:type',$type);
        testbed_dump('iterate:branch',$branch);
        testbed_dump('iterate:branch:name',git_branch_name($branch));
    }
}

function test_upstream() {
    $repo = git_repository_open(testbed_get_localrepo_path());
    $branch = git_branch_lookup($repo,"test/testbed",GIT_BRANCH_ALL);

    // Test looking up initial upstream (i.e. tracking branch). It should be
    // empty (i.e. return FALSE).
    testbed_dump('upstream',git_branch_upstream($branch));
    $name = git_branch_upstream_name($repo,"refs/heads/test/testbed");
    testbed_dump('upstream:name',$name);

    // Apply tracking branch and print to verify.
    git_branch_set_upstream($branch,"origin/test/testbed");
    $name = git_branch_upstream_name($repo,"refs/heads/test/testbed");
    testbed_dump('upstream:set:name',$name);
    $name = git_branch_upstream_remote($repo,"refs/heads/test/testbed");
    testbed_dump('upstream:remote:name',$name);

    // Unset remote tracking and print to verify.
    git_branch_set_upstream($branch,null);
    testbed_dump('upstream:set:null',git_branch_upstream($branch));
    $name = git_branch_upstream_name($repo,"refs/heads/test/testbed");
    testbed_dump('upstream:set:null:restore',$name);
}

testbed_test('Branch/create','Git2Test\Branch\test_create');
testbed_test('Branch/lookup','Git2Test\Branch\test_lookup');
testbed_test('Branch/iterate','Git2Test\Branch\test_iterate');
testbed_test('Branch/upstream','Git2Test\Branch\test_upstream');
