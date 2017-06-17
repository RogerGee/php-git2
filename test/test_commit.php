<?php

namespace Git2Test\Commit;

require_once 'test_base.php';

function test_open() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $commit = git_commit_lookup($repo,'fd211e73b0e7ede926bfbc47026d5ad0bc4b8902');

    $brief = git_commit_summary($commit);
    $body = git_commit_body($commit);
    $time = git_commit_time($commit);
    $offset = git_commit_time_offset($commit);
    $dt = date('Y-m-d H:i:s',$time);

    echo 'Commit ' . git_commit_id($commit) . PHP_EOL;
    echo "  committed on $dt ($offset)\n\n";
    echo "$brief\n\n$body\n";

    $owner = git_commit_owner($commit);
    echo PHP_EOL . 'Owner path:' . PHP_EOL;
    var_dump(git_repository_path($owner));
}

function test_lookup() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $commit = git_commit_lookup($repo,'fd211e73b0e7ede926bfbc47026d5ad0bc4b8902');

    $a = git_commit_nth_gen_ancestor($commit,3);
    var_dump(git_commit_summary($a));

    try {
        $b = git_commit_nth_gen_ancestor($commit,5000000);
    } catch (\Exception $ex) {
        var_dump($ex->getMessage());
        var_dump($ex->getCode());

        if ($ex->getCode() == GIT_ENOTFOUND) {
            echo 'The specified ancestor was not found.' . PHP_EOL;
        }
    }    
}

testbed_test('Commit/open','Git2Test\Commit\test_open');
testbed_test('Commit/lookup','Git2Test\Commit\test_lookup');
