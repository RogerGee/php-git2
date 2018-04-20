<?php

namespace Git2Test\Attr;

require_once 'test_base.php';

function test_misc() {
    testbed_unit('git_attr_value(asdf)',git_attr_value('asdf'));
    testbed_unit('git_attr_value(eol)',git_attr_value('eol'));
}

function test_repo() {
    $repo = git_repository_open(testbed_get_localrepo_path());

    git_attr_add_macro($repo,'my_macro','text -crlf');

    $lambda = function($name,$value,$payload) {
        echo "git_attr_foreach($name,$value,$payload)" . PHP_EOL;
    };
    git_attr_foreach($repo,GIT_ATTR_CHECK_FILE_THEN_INDEX,'php-git2.cpp',$lambda,33);

    testbed_unit('git_attr_get()',git_attr_get($repo,
                                               GIT_ATTR_CHECK_FILE_THEN_INDEX,
                                               'php-git2.cpp',
                                               'eol'));

    git_attr_cache_flush($repo);
}

testbed_test('Attr/misc','Git2Test\Attr\test_misc');
testbed_test('Attr/repo','Git2Test\Attr\test_repo');
