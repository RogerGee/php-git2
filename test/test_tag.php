<?php

namespace Git2Test\Tag;

use Exception;

require_once 'test_base.php';

function test_create() {
    $repo = git_repository_open_bare(testbed_get_repo_path());

    // Create lightweight (i.e. reference) tag.
    $oid = git_tag_create_lightweight(
        $repo,
        "first-commit",
        git_object_lookup($repo,'d129836a41a15fce409debb36c0d2a5503070b17',GIT_OBJ_ANY),
        true);
    echo "Created tag 'first-commit' with oid=$oid.\n";

    try {
        // Create tag object (i.e. annotated tag)
        $oid = git_tag_create(
            $repo,
            "second-commit",
            git_object_lookup($repo,'dcf12194ecb3d1e96a3014a0629d72ee33007c88',GIT_OBJ_ANY),
            git_signature_now("Tester","tester@php.net"),
            "This tag was created for testing.",
            false);

        echo "Created annotated tag 'second-commit' with oid=$oid.\n";
    } catch (Exception $ex) {
        echo 'Tag \'second-commit\' was already created: ' . $ex->getMessage() . PHP_EOL;
    }
}

function test_lookup() {
    $repo = git_repository_open_bare(testbed_get_repo_path());

    $lambda = function($name,$oid,$payload) use($repo) {
        var_dump("$name $oid $payload");

        try {
            $tag = git_tag_lookup($repo,$oid);

            var_dump($tag);
            var_dump( git_tag_id($tag) );
            var_dump( git_tag_target_id($tag) );
            var_dump( git_tag_message($tag) );
            var_dump( git_tag_name($tag) );
            var_dump( git_tag_owner($tag) );
            var_dump( git_tag_peel($tag) );
            var_dump( git_tag_target($tag) );

            $tagger = git_tag_tagger($tag);
            var_dump( git2_signature_convert($tagger) );
        } catch (Exception $ex) {
            echo "Failed to open tag object '$name' (probably not an annotated tag)\n";
            echo '  ' . $ex->getMessage() . PHP_EOL;
        }
    };

    git_tag_foreach($repo,$lambda,555);
}

function test_list() {
    $repo = git_repository_open_bare(testbed_get_repo_path());

    $list = git_tag_list($repo);

    var_dump($list);
}

testbed_test('Tag: create','Git2Test\Tag\test_create');
testbed_test('Tag: lookup','Git2Test\Tag\test_lookup');
testbed_test('Tag: list','Git2Test\Tag\test_list');
