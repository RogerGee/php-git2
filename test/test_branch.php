<?php

namespace Git2Test\Branch;

require_once 'test_base.php';

function test_create() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $ref = git_repository_head($repo);
    $commit = git_commit_lookup($repo,git_reference_target($ref));

    $newbranch = git_branch_create($repo,"life-and-limb",$commit,true);
    var_dump($newbranch);
    var_dump(git_branch_is_head($newbranch));

    $delete = git_branch_create($repo,"delete-me",$commit,true);
    var_dump($delete);
    $cpy = $delete;
    git_branch_delete($delete);
    var_dump($delete);
    var_dump($cpy);
}

function test_lookup() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $branch = git_branch_lookup($repo,"life-and-limb",GIT_BRANCH_ALL);
    var_dump($branch);

    var_dump(git_branch_is_head($branch));
    var_dump(git_branch_name($branch));

    $cpy = git_branch_move($branch,"life-and-limb-born-again",true);
    var_dump($cpy);
    var_dump(git_branch_name($cpy));
}

function test_iterate() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $iter = git_branch_iterator_new($repo,GIT_BRANCH_ALL);

    while (true) {
        $branch = git_branch_next($type,$iter);
        if ($branch === false) {
            break;
        }

        var_dump($type);
        var_dump($branch);
        var_dump(git_branch_name($branch));
    }
}

function test_upstream() {
    $repo = git_repository_open(testbed_get_localrepo_path());
    $branch = git_branch_lookup($repo,"test/testbed",GIT_BRANCH_ALL);

    // Test looking up initial upstream (i.e. tracking branch). It should be
    // empty (i.e. return FALSE).
    var_dump(git_branch_upstream($branch));
    $name = git_branch_upstream_name($repo,"refs/heads/test/testbed");
    var_dump($name);

    // Apply tracking branch and print to verify.
    git_branch_set_upstream($branch,"origin/test/testbed");
    $name = git_branch_upstream_name($repo,"refs/heads/test/testbed");
    var_dump($name);
    $name = git_branch_upstream_remote($repo,"refs/heads/test/testbed");
    var_dump($name);

    // Unset remote tracking and print to verify.
    git_branch_set_upstream($branch,null);
    var_dump(git_branch_upstream($branch));
    $name = git_branch_upstream_name($repo,"refs/heads/test/testbed");
    var_dump($name);
}

testbed_test('Branch/create','Git2Test\Branch\test_create');
testbed_test('Branch/lookup','Git2Test\Branch\test_lookup');
testbed_test('Branch/iterate','Git2Test\Branch\test_iterate');
testbed_test('Branch/upstream','Git2Test\Branch\test_upstream');
