<?php

namespace Git2Test\Tree;

use Exception;

require_once 'test_base.php';

function test_simple_lookup() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $head = git_reference_name_to_id($repo,'HEAD');
    $commit = git_commit_lookup($repo,$head);

    $tree = git_commit_tree($commit);
    $entry = git_tree_entry_bypath($tree,'php-type.h');
    $otherEntry = git_tree_entry_byindex($tree,1);
    var_dump(git_tree_entry_name($otherEntry));
}

function callback($path,$entry,$indent) {
    $name = git_tree_entry_name($entry);
    echo str_repeat(' ',$indent) . "$path$name\n";
    $indent += 2;
}

function test_walk() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $head = git_reference_name_to_id($repo,'HEAD');
    $commit = git_commit_lookup($repo,$head);

    $tree = git_commit_tree($commit);
    $indent = 2;
    echo 'Walking:' . PHP_EOL;
    git_tree_walk($tree,GIT_TREEWALK_PRE,'Git2Test\Tree\callback',$indent);
}

function test_lookup_noent() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $head = git_reference_name_to_id($repo,'HEAD');
    $commit = git_commit_lookup($repo,$head);

    $tree = git_commit_tree($commit);

    // git_tree_entry_bypath() should throw when not found
    try {
        $entry = git_tree_entry_bypath($tree,"idonotexist.txt");
    } catch (Exception $ex) {
        echo 'fail git_tree_entry_bypath(): ' . $ex->getMessage() . PHP_EOL;
    }

    // These other ones should return NULL.
    $entry = git_tree_entry_byname($tree,"idonotexist.txt");
    var_dump($entry);
    $entry = git_tree_entry_byid($tree,str_repeat('0',40));
    var_dump($entry);
    $entry = git_tree_entry_byindex($tree,7864786);
    var_dump($entry);
}

testbed_test('Tree/simple_lookup','Git2Test\Tree\test_simple_lookup');
testbed_test('Tree/walk','Git2Test\Tree\test_walk');
testbed_test('Tree/lookup_noent','Git2Test\Tree\test_lookup_noent');
