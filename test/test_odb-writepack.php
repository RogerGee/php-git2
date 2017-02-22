<?php

require_once 'test_base.php';

function callback($stats,$payload) {
    echo 'transfer_progress_callback:' . PHP_EOL;
    echo "  received: {$stats['received_objects']}/{$stats['total_objects']}\n";
    echo "  indexed: {$stats['indexed_objects']}/{$stats['total_objects']}\n";
}

testbed_test('WRITEPACK',function(){
    $repoPath = testbed_get_repo_path();
    $repo = git_repository_open_bare($repoPath);
    $odb = git_repository_odb($repo);
    $wp = git_odb_write_pack($odb,'callback',33);

    // This is a random packfile that adds a small, root commit to the database.
    $pack = '5041434b00000002000000039a09789c95cbc10dc3200c00c03f5378815436a4604'
        . 'b51d55fffddc0a126ed232242b07fb342a4fb5e6f66c05c44a3a9fa5c107995358be7'
        . '3951e2e8a99c54a404733afab73678d7cd1abcceb9b46323d2a7e671b3cf7800cd9c6'
        . '2087427983022ba5cf7fdd7bb5d5b8edc1fd6552bcca102789c33343030333151c848'
        . 'cdc9c967581fa6b665a1c8ead34dd76cb79fe0d0b1710f7b2a0d00bfb20c8f3e789cf'
        . '348cdc9c9d75128cf2fca4951e4020024f20494afed7ccfb9056fe89ef493d1983c26'
        . 'd10f291f39';

    var_dump($wp);
    $backend = $wp->backend;
    var_dump($backend->version);
    echo "writepack: append\n";
    var_dump($wp->append(hex2bin($pack)));
    echo "\nwritepack: commit\n";
    var_dump($wp->commit());

    // Freeing is optional but we should test it.
    $wp->free();
});

$ret = testbed_test('WRITEPACK/LIFETIME OF OBJECT',function(){
    $lambda = function(){
        $repoPath = testbed_get_repo_path();
        $repo = git_repository_open_bare($repoPath);
        $odb = git_repository_odb($repo);
        $wp = git_odb_write_pack($odb,'callback',33);

        // This is a random packfile that adds a small, root commit to the database.
        $pack = '5041434b00000002000000039a09789c95cbc10dc3200c00c03f5378815436a4604'
            . 'b51d55fffddc0a126ed232242b07fb342a4fb5e6f66c05c44a3a9fa5c107995358be7'
            . '3951e2e8a99c54a404733afab73678d7cd1abcceb9b46323d2a7e671b3cf7800cd9c6'
            . '2087427983022ba5cf7fdd7bb5d5b8edc1fd6552bcca102789c33343030333151c848'
            . 'cdc9c967581fa6b665a1c8ead34dd76cb79fe0d0b1710f7b2a0d00bfb20c8f3e789cf'
            . '348cdc9c9d75128cf2fca4951e4020024f20494afed7ccfb9056fe89ef493d1983c26'
            . 'd10f291f39';

        echo "writepack: append\n";
        var_dump($wp->append(hex2bin($pack)));

        echo "\nwritepack: commit\n";
        var_dump($wp->commit());

        // Return before freeing.
        return $wp;
    };

    $wp = $lambda();
    var_dump($wp);
    $wp->free();
});
