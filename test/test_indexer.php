<?php

namespace Git2Test\Indexer;

require_once 'test_base.php';

function make_pack($repo,$path) {
    $builder = git_packbuilder_new($repo);

    $commits[] = 'b12e60c8e3701d11fe78c27d67fa4343619d067b';
    $commits[] = '4cc0ee059fc5c949c3715bd0fca5357acff2323a';
    $commits[] = 'd129836a41a15fce409debb36c0d2a5503070b17';

    foreach ($commits as $oid) {
        git_packbuilder_insert_commit($builder,$oid);
    }

    $pack = fopen($path,'w');
    git_packbuilder_foreach($builder,null,$pack);
    fclose($pack);

    return fopen($path,'r');
}

function test_general() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $odb = git_repository_odb($repo);

    $packFile = make_pack($repo,testbed_path('indexer-test.pack'));

    $indexerPath = testbed_path('indexer-storage',true);
    $progressfn = function($stats,$payload) {
        testbed_dump('transfer_progress_callback',[
            'stats' => $stats,
            'payload' => $payload,
        ]);
    };

    $indexer = git_indexer_new($indexerPath,0,$odb,$progressfn,33);

    while (true) {
        $chunk = fread($packFile,128);
        if ($chunk === false || empty($chunk)) {
            break;
        }

        git_indexer_append($indexer,$chunk);
    }

    git_indexer_commit($indexer);

    testbed_dump('hash',git_indexer_hash($indexer));
    testbed_dump('stats',git2_indexer_stats($indexer));

    git_indexer_free($indexer);
}

function test_null_callback() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $odb = git_repository_odb($repo);

    $packFile = make_pack($repo,testbed_path('indexer-test.pack'));

    $indexerPath = testbed_path('indexer-storage',true);

    $indexer = git_indexer_new($indexerPath,0,$odb,null,null);

    while (true) {
        $chunk = fread($packFile,128);
        if ($chunk === false || empty($chunk)) {
            break;
        }

        git_indexer_append($indexer,$chunk);
    }

    git_indexer_commit($indexer);

    testbed_dump('hash',git_indexer_hash($indexer));
    testbed_dump('stats',git2_indexer_stats($indexer));

    git_indexer_free($indexer);
}

testbed_test('Indexer/general','Git2Test\Indexer\test_general');
testbed_test('Indexer/null','Git2Test\Indexer\test_null_callback');
