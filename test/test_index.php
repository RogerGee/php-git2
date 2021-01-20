<?php

namespace Git2Test\Index;

require_once 'test_base.php';

function test_misc() {
    $repo = git_repository_open(testbed_get_localrepo_path());
    $index = git_repository_index($repo);

    testbed_dump('git_index_caps',git_index_caps($index));
    testbed_dump('git_index_checksum',git_index_checksum($index));
    $owner = git_index_owner($index);
    testbed_dump('git_index_owner',$owner);
    testbed_dump('git_index_has_conflicts',git_index_has_conflicts($index));
    testbed_dump('git_index_path',git_index_path($index));
    testbed_dump('git_index_version',git_index_version($index));
}

testbed_test('Index/Misc','Git2Test\Index\test_misc');
