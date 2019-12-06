<?php

namespace Git2Test\Signature;

require_once 'test_base.php';

function test_create() {
    $sig = git_signature_new('Roger Gee','roger@example.com',56,43);
    $dup = git_signature_dup($sig);

    echo 'Original: ' . PHP_EOL;
    var_dump(git2_signature_convert($sig));

    echo 'Duplicate: ' . PHP_EOL;
    var_dump(git2_signature_convert($dup));

    echo PHP_EOL . 'Freed: ' . PHP_EOL;
    git_signature_free($dup);
    var_dump($dup);
    //var_dump(git2_signature_convert($dup));

    echo PHP_EOL . 'Now: ' . PHP_EOL;
    var_dump(git2_signature_convert(git_signature_now('Roger Gee','roger@example.com')));
}

function test_default() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $sig = git_signature_default($repo);

    var_dump($info = git2_signature_convert($sig));

    echo 'The default signature was:' . PHP_EOL;
    echo "  Name: {$info['name']}" . PHP_EOL;
    echo "  Email: {$info['email']}" . PHP_EOL;
    $t = date('Y-m-d H:i:s',$info['when.time'])
        . ' -' . str_pad(abs($info['when.offset'] / 60) * 100,4,'0',STR_PAD_LEFT);
    echo "  Time: $t" . PHP_EOL;
}

function test_parse() {
    $text = 'Person of theWorld <person@example.com> ' . time() . ' -0700';
    $sig = git_signature_from_buffer($text);

    var_dump($info = git2_signature_convert($sig));
    echo 'The default signature was:' . PHP_EOL;
    echo "  Name: {$info['name']}" . PHP_EOL;
    echo "  Email: {$info['email']}" . PHP_EOL;
    $t = date('Y-m-d H:i:s',$info['when.time'])
        . ' -' . str_pad(abs($info['when.offset'] / 60) * 100,4,'0',STR_PAD_LEFT);
    echo "  Time: $t" . PHP_EOL;
}

testbed_test('Signature/create','Git2Test\Signature\test_create');
testbed_test('Signature/default','Git2Test\Signature\test_default');
testbed_test('Signature/parse','Git2Test\Signature\test_parse');
