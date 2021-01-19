<?php

namespace Git2Test\Status;

require_once 'test_base.php';

function test_status() {
    $repo = git_repository_open(testbed_get_localrepo_path());
    $index = git_repository_index($repo);

    $entry = git_index_get_bypath($index,'README',0);
    testbed_dump('Modifying Entry',$entry);

    $status = git_status_file($repo,$entry['path']);
    testbed_dump('Status Before',$status);

    testbed_modify_file($repo,$entry['path']);

    $status = git_status_file($repo,$entry['path']);
    testbed_dump('Status After',$status);

    $opts = array(
        'checkout_strategy' => GIT_CHECKOUT_FORCE,
        'paths' => ['README'],
    );
    git_checkout_index($repo,null,$opts);
}

function test_foreach() {
    $repo = git_repository_open(testbed_get_localrepo_path());

    testbed_modify_file($repo,'README');
    testbed_modify_file($repo,'php-git2.cpp');

    $lambda = function($path,$flags,$payload) {
        testbed_dump('path',$path);
        testbed_dump('flags',$flags);
        testbed_do_once('test_status_payload',function() use ($payload) {
            testbed_dump('payload',$payload);
        });
    };

    git_status_foreach($repo,$lambda,33);

    $opts = array(
        'checkout_strategy' => GIT_CHECKOUT_FORCE,
        'paths' => ['README', 'php-git2.cpp'],
    );
    git_checkout_index($repo,null,$opts);
}

testbed_test('Status/misc','Git2Test\Status\test_status');
testbed_test('Status/foreach','Git2Test\Status\test_foreach');
