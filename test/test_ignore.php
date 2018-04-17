<?php

namespace Git2Test\Ignore;

require_once 'test_base.php';

function test_add() {
    $repo = git_repository_open(testbed_get_localrepo_path());

    testbed_unit('before',git_ignore_path_is_ignored($repo,'silly'));
    git_ignore_add_rule($repo,'silly');
    testbed_unit('after',git_ignore_path_is_ignored($repo,'silly'));

    git_ignore_clear_internal_rules($repo);
    testbed_unit('after clear',git_ignore_path_is_ignored($repo,'silly'));
}

function test_query() {
    $repo = git_repository_open(testbed_get_localrepo_path());

    testbed_unit('query: modules/git2.so',git_ignore_path_is_ignored($repo,'modules/git2.so'));
    testbed_unit('query: php-funk.cpp',git_ignore_path_is_ignored($repo,'php-funk.cpp'));
}

testbed_test('Ignore/add','Git2Test\Ignore\test_add');
testbed_test('Ignore/query','Git2Test\Ignore\test_query');
