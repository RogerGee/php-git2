<?php

namespace Git2Test\Remote;

require_once 'test_base.php';

function test_general() {
    $testfn = function($remote) {
        testbed_dump('git_remote_autotag', git_remote_autotag($remote));
        testbed_dump('git_remote_connected', git_remote_connected($remote));
        try {
            testbed_dump('git_remote_default_branch', git_remote_default_branch($remote));
        } catch (\Exception $ex) {
            testbed_dump('EX: git_remote_default_branch',$ex->getMessage());
        }
        try {
            testbed_dump('git_remote_ls', git_remote_ls($remote));
        } catch (\Exception $ex) {
            testbed_dump('EX: git_remote_ls', $ex->getMessage());
        }
        try {
            testbed_dump('git_remote_get_fetch_refspecs', git_remote_get_fetch_refspecs($remote));
        } catch (\Exception $ex) {
            testbed_dump('EX: git_remote_get_fetch_refspecs', $ex->getMessage());
        }
        try {
            testbed_dump('git_remote_get_push_refspecs', git_remote_get_push_refspecs($remote));
        } catch (\Exception $ex) {
            testbed_dump('EX: git_remote_get_push_refspecs', $ex->getMessage());
        }
    };
    
    $repoPath = testbed_get_localrepo_path();
    $repo = git_repository_open($repoPath);

    try {
        git_remote_delete($repo,'foo');

    } catch (\Exception $ex) { }

    $remote = git_remote_create($repo,'foo','https://example.com/repos/foo.git');
    $testfn($remote);

    $remote = git_remote_create_anonymous($repo,'https://example.com/repos/anon.git');
    $testfn($remote);

    $remote = git_remote_dup($remote);
    $testfn($remote);

    $remote = git_remote_lookup($repo,'origin');
    $testfn($remote);

    testbed_dump('git_remote_is_valid_name', git_remote_is_valid_name('roger'));
    testbed_dump('git_remote_is_valid_name', git_remote_is_valid_name('+--=-=-'));
}

function test_fetch() {
    $repoPath = testbed_get_localrepo_path();
    $repo = git_repository_open($repoPath);

    $callbacks = [
        'transfer_progress' => function($stats,$payload) {
            echo 'TRANSFER PROGRESS CALLBACK' . PHP_EOL;
            var_dump($stats);
            var_dump($payload);
        },
        'sideband_progress' => function($str,$payload) {
            echo 'TRANSPORT_MESSAGE_CALLBACK' . PHP_EOL;
            var_dump($str);
            var_dump($payload);
        },
        'payload' => 33,
    ];

    $opts = [
        'callbacks' => $callbacks,
    ];

    $remote = git_remote_lookup($repo,'origin');
    git_remote_fetch($remote,null,$opts,'Doing a test fetch');
}

testbed_test('Remote/general','Git2Test\Remote\test_general');
testbed_test('Remote/fetch','Git2Test\Remote\test_fetch');
