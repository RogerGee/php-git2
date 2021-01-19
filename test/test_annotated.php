<?php

namespace Git2Test\Annotated;

require_once 'test_base.php';

function test_create() {
    $repo = git_repository_open_bare(testbed_get_repo_path());

    $ref = git_repository_head($repo);
    $annotated = git_annotated_commit_from_ref($repo,$ref);

    testbed_dump('create:id',git_annotated_commit_id($annotated));
    git_annotated_commit_free($annotated);
}

function test_lookup() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $ref = git_repository_head($repo);
    $id = git_reference_target($ref);

    $annotated = git_annotated_commit_lookup($repo,$id);
    testbed_dump('lookup',$annotated);
    testbed_dump('lookup:id',git_annotated_commit_id($annotated));
}

testbed_test('Annotated/create','Git2Test\Annotated\test_create');
testbed_test('Annotated/lookup','Git2Test\Annotated\test_lookup');
