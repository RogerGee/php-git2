<?php

namespace Git2Test\ODBWritepack;

use Exception;
use PHPSerializedODB_WithWritepack;

require_once 'test_base.php';

function callback($stats,$payload) {
    echo 'transfer_progress_callback:' . PHP_EOL;
    echo "  received: {$stats['received_objects']}/{$stats['total_objects']}\n";
    echo "  indexed: {$stats['indexed_objects']}/{$stats['total_objects']}\n";
}

function test_basic() {
    $repoPath = testbed_get_repo_path();
    $repo = git_repository_open_bare($repoPath);
    $odb = git_repository_odb($repo);
    $wp = git_odb_write_pack($odb,'Git2Test\ODBWritepack\callback',33);

    // This is a random packfile that adds a small, root commit to the database.
    $pack = '5041434b00000002000000039a09789c95cbc10dc3200c00c03f5378815436a4604'
        . 'b51d55fffddc0a126ed232242b07fb342a4fb5e6f66c05c44a3a9fa5c107995358be7'
        . '3951e2e8a99c54a404733afab73678d7cd1abcceb9b46323d2a7e671b3cf7800cd9c6'
        . '2087427983022ba5cf7fdd7bb5d5b8edc1fd6552bcca102789c33343030333151c848'
        . 'cdc9c967581fa6b665a1c8ead34dd76cb79fe0d0b1710f7b2a0d00bfb20c8f3e789cf'
        . '348cdc9c9d75128cf2fca4951e4020024f20494afed7ccfb9056fe89ef493d1983c26'
        . 'd10f291f39';

    testbed_unit('writepack',$wp);
    testbed_unit('writepack->backend->version',$wp->backend->version);
    testbed_unit('writepack->progress (before)',$wp->progress);
    testbed_unit('writepack->append()',$wp->append(hex2bin($pack)));
    testbed_unit('writepack->commit()',$wp->commit());
    testbed_unit('writeback->progress (after)',$wp->progress);
}

function test_lifetime() {
    $lambda = function(){
        $repoPath = testbed_get_repo_path();
        $repo = git_repository_open_bare($repoPath);
        $odb = git_repository_odb($repo);
        $wp = git_odb_write_pack($odb,'Git2Test\ODBWritepack\callback',33);

        // This is a random packfile that adds a small, root commit to the database.
        $pack = '5041434b00000002000000039a09789c95cbc10dc3200c00c03f5378815436a4604'
            . 'b51d55fffddc0a126ed232242b07fb342a4fb5e6f66c05c44a3a9fa5c107995358be7'
            . '3951e2e8a99c54a404733afab73678d7cd1abcceb9b46323d2a7e671b3cf7800cd9c6'
            . '2087427983022ba5cf7fdd7bb5d5b8edc1fd6552bcca102789c33343030333151c848'
            . 'cdc9c967581fa6b665a1c8ead34dd76cb79fe0d0b1710f7b2a0d00bfb20c8f3e789cf'
            . '348cdc9c9d75128cf2fca4951e4020024f20494afed7ccfb9056fe89ef493d1983c26'
            . 'd10f291f39';

        testbed_unit('writepack->append',$wp->append(hex2bin($pack)));
        testbed_unit('writepack->commit',$wp->commit());

        // Return before freeing.
        return $wp;
    };

    $wp = $lambda();
    testbed_unit('writepack->backend',$wp->backend);
    testbed_unit('writepack->progress',$wp->progress);
}

function test_callback_errors() {
    $lambda = function($stats,$payload) {
        $payload->hasError = true;

        // Uncomment to generate fatal error to test handling.
        //throw $payload;

        throw new \Exception('Thrown from inside the callback');
    };

    try {
        $repoPath = testbed_get_repo_path();
        $repo = git_repository_open_bare($repoPath);
        $odb = git_repository_odb($repo);
        $payload = new \stdClass;
        $wp = git_odb_write_pack($odb,$lambda,$payload);

        // This is a random packfile that adds a small, root commit to the database.
        $pack = '5041434b00000002000000039a09789c95cbc10dc3200c00c03f5378815436a4604'
              . 'b51d55fffddc0a126ed232242b07fb342a4fb5e6f66c05c44a3a9fa5c107995358be7'
              . '3951e2e8a99c54a404733afab73678d7cd1abcceb9b46323d2a7e671b3cf7800cd9c6'
              . '2087427983022ba5cf7fdd7bb5d5b8edc1fd6552bcca102789c33343030333151c848'
              . 'cdc9c967581fa6b665a1c8ead34dd76cb79fe0d0b1710f7b2a0d00bfb20c8f3e789cf'
              . '348cdc9c9d75128cf2fca4951e4020024f20494afed7ccfb9056fe89ef493d1983c26'
              . 'd10f291f39';

        $wp->append(hex2bin($pack));
        $wp->commit();
    } catch (\Exception $ex) {
        testbed_unit('Caught exception',$ex);
    }
}

function test_custom() {
    require_once 'PHPSerializedODB.php';

    $backend = new PHPSerializedODB_WithWritepack('test-writepack');
    $repo = git_repository_new();
    $odb = git_odb_new();

    git_odb_add_backend($odb,$backend,1);
    git_repository_set_odb($repo,$odb);

    $wp = git_odb_write_pack($odb,'Git2Test\ODBWritepack\callback',33);

    // This is a random packfile that adds a small, root commit to the database.
    $pack = '5041434b00000002000000039a09789c95cbc10dc3200c00c03f5378815436a4604'
        . 'b51d55fffddc0a126ed232242b07fb342a4fb5e6f66c05c44a3a9fa5c107995358be7'
        . '3951e2e8a99c54a404733afab73678d7cd1abcceb9b46323d2a7e671b3cf7800cd9c6'
        . '2087427983022ba5cf7fdd7bb5d5b8edc1fd6552bcca102789c33343030333151c848'
        . 'cdc9c967581fa6b665a1c8ead34dd76cb79fe0d0b1710f7b2a0d00bfb20c8f3e789cf'
        . '348cdc9c9d75128cf2fca4951e4020024f20494afed7ccfb9056fe89ef493d1983c26'
        . 'd10f291f39';

    testbed_unit('writepack',$wp);
    testbed_unit('writepack->backend->version',$wp->backend->version);
    testbed_unit('writepack->progress (before)',$wp->progress);
    testbed_unit('writepack->append()',$wp->append(hex2bin($pack)));
    testbed_unit('writepack->commit()',$wp->commit());
    testbed_unit('writeback->progress (after)',$wp->progress);
}

testbed_test('Writepack','Git2Test\ODBWritepack\test_basic');
testbed_test('Writepack/Lifetime of object','Git2Test\ODBWritepack\test_lifetime');
testbed_test('Writepack/Callback errors','Git2Test\ODBWritepack\test_callback_errors');
testbed_test('Writepack/Custom','Git2Test\ODBWritepack\test_custom');
