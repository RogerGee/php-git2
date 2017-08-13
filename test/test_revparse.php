<?php

namespace Git2Test\Revparse;

require_once 'test_base.php';

function test_revparse() {
    $repo = git_repository_open_bare(testbed_get_repo_path());

    $one = git_revparse($repo,'master');
    var_dump($one);
    if (isset($one['from'])) {
        var_dump(git_object_id($one['from']));
    }
    if (isset($one['to'])) {
        var_dump(git_object_id($one['to']));
    }

    $two = git_revparse($repo,'master~10..master');
    var_dump($two);
    if (isset($two['from'])) {
        var_dump(git_object_id($two['from']));
    }
    if (isset($two['to'])) {
        var_dump(git_object_id($two['to']));
    }
}

function test_single() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $commitish = 'HEAD~10';

    $obj = git_revparse_single($repo,$commitish);
    var_dump(git_object_id($obj));
}

function test_ext() {
    $repo = git_repository_open_bare(testbed_get_repo_path());

    // This revision will have a reference (i.e. refs/heads/master).
    $commitish = 'master';
    echo "revparse: $commitish\n";
    $obj = git_revparse_ext($ref,$repo,$commitish);
    echo '  '; var_dump($obj);
    echo '  '; var_dump($ref);
    if (isset($ref)) {
        echo '  '; var_dump(git_reference_target($ref));
        echo '  '; var_dump(git_reference_name($ref));
    }

    // This revision may not have a reference (i.e. does not necessarily have a
    // reference).
    $commitish = 'master~10';
    echo "revparse: $commitish\n";
    $obj = git_revparse_ext($ref,$repo,$commitish);
    echo '  '; var_dump($obj);
    echo '  '; var_dump($ref);
    if (isset($ref)) {
        echo '  '; var_dump(git_reference_target($ref));
        echo '  '; var_dump(git_reference_name($ref));
    }
}

testbed_test('Revparse/revparse','Git2Test\Revparse\test_revparse');
testbed_test('Revparse/single','Git2Test\Revparse\test_single');
testbed_test('Revparse/ext','Git2Test\Revparse\test_ext');
