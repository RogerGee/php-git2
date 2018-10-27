<?php

namespace Git2Test\Reference;

require_once 'test_base.php';

function test_foreach() {
    $repo = git_repository_open_bare(testbed_get_repo_path());

    $lambda1 = function($ref,$thing) {
        //var_dump($ref);
        //var_dump($thing);
        //var_dump(git_reference_name($ref));
        $name = git_reference_name($ref);
        $short = git_reference_shorthand($ref);

        var_dump("$thing --> $name --> $short");
    };

    $lambda2 = function($name,$thing) {
        var_dump("$thing --> $name");
    };

    $lambda3 = function($name,$thing) {
        var_dump($name);
        var_dump($thing);
    };

    git_reference_foreach($repo,$lambda1,33);
    echo str_repeat('-',30) . PHP_EOL;
    git_reference_foreach_name($repo,$lambda2,33);
    echo str_repeat('-',30) . PHP_EOL;
    git_reference_foreach_glob($repo,'*mas*',$lambda3,33);
}

function test_free() {
    $lambda = function () {
        $repo = git_repository_open_bare(testbed_get_repo_path());
        $ref = git_reference_lookup($repo,'refs/heads/master');

        git_repository_free($repo);

        return $ref;
    };

    $ref = $lambda();

    testbed_unit('ref-name',git_reference_name($ref));
    testbed_unit('ref-shorthand',git_reference_shorthand($ref));
}

function test_misc() {
    $repo = git_repository_open_bare(testbed_get_repo_path());

    $ref = git_reference__alloc('name',str_repeat('0',40),null);

    $ref = git_reference_lookup($repo,'refs/heads/master');
    $cpy = git_reference_resolve($ref);
    var_dump($cpy);
}

testbed_test('Reference/foreach','Git2Test\Reference\test_foreach');
testbed_test('Reference/free','Git2Test\Reference\test_free');
testbed_test('Reference/misc','Git2Test\Reference\test_misc');
