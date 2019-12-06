<?php

namespace Git2Test\Treebuilder;

use Exception;

require_once 'test_base.php';

function test_create() {
    $repo = git_repository_open_bare(testbed_get_repo_path());

    $blob = git_blob_create_frombuffer($repo,'This is some text.');

    $builder = git_treebuilder_new($repo,null);
    $entry = git_treebuilder_insert($builder,'paper-clips.txt',$blob,0100644);

    echo 'Number of entries: ' . git_treebuilder_entrycount($builder) . PHP_EOL;
    echo 'Tree entry ID: ' . git_tree_entry_id($entry) . PHP_EOL;

    echo 'Wrote tree with OID=' . ($treeid = git_treebuilder_write($builder)) . PHP_EOL;

    try {
        $refName = "refs/heads/test_treebuilder";
        $ref = git_reference_lookup($repo,$refName);
        $parents = [git_reference_target($ref)];
    } catch (Exception $ex) {
        $parents = [];
    }

    $author = git_signature_now('Dorkus Barg','dork@email.example.com');
    $commitid = git_commit_create_from_ids(
        $repo,
        $refName,
        $author,
        $author,
        null,
        "[test] Add paper clips documentation",
        $treeid,
        $parents);
    echo "Created a commit with OID=$commitid to reference the tree. See ref=test_treebuilder";

    git_treebuilder_clear($builder);
    echo 'After clear: ' . git_treebuilder_entrycount($builder) . PHP_EOL;
}

testbed_test('Treebuilder\create','Git2Test\Treebuilder\test_create');
