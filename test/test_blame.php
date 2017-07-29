<?php

namespace Git2Test\Blame;

require_once 'test_base.php';

function test_file() {
    $opts = array(
        'flags' => GIT_BLAME_TRACK_COPIES_SAME_FILE,
        'min_line' => 15,
    );

    $repo = git_repository_open_bare(testbed_get_repo_path());
    $blame = git_blame_file($repo,'php-git2.cpp',$opts);
    $hunk0 = git_blame_get_hunk_byindex($blame,0);
    $hunk1 = git_blame_get_hunk_byindex($blame,1);
    $hunk = git_blame_get_hunk_byline($blame,33);

    var_dump(git_blame_get_hunk_count($blame));
    var_dump($hunk0);
    var_dump($hunk1);
    var_dump($hunk);
}

function test_buffer() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $blame = git_blame_file($repo,'php-git2.cpp',null);

    // This crashes: I think it's a bug in git2:
    // $blametmp = git_blame_buffer($blame,'a');

    $blametmp = git_blame_buffer($blame,file_get_contents('php-git2.cpp'));

    $hunk0 = git_blame_get_hunk_byindex($blametmp,0);
    var_dump($hunk0);
    git_blame_free($blametmp);
    git_blame_free($blame);
}

testbed_test('Blame/file','Git2Test\Blame\test_file');
testbed_test('blame/buffer','Git2Test\Blame\test_buffer');
