<?php

namespace Git2Test\ODBBackend;

require_once 'test_base.php';
require_once 'test_odb-writepack.php';

function test_read() {
    $repoPath = testbed_get_repo_path();
    $backend = git_odb_backend_pack("$repoPath/objects");
    var_dump($backend);
    echo "\n";

    $oid = 'af5626b4a114abcb82d63db7c8082c3c4756e51b';

    // Read blob from the pack inserted in test_odb-writepack. It should contain
    // the text "Hello, world!\n".
    var_dump($backend->read($type,$oid));
    var_dump($type);
    echo "\n";

    // Now try the same thing but use an abbreviated id.
    $abbrev = 'af5626b';
    var_dump($backend->read_prefix($outid,$targ,$abbrev));
    var_dump($outid);
    var_dump($targ);
    echo "\n";

    // Now try the same thing but just get the header.
    var_dump($backend->read_header($headerSize,$headerType,$oid));
    var_dump($headerSize);
    var_dump($headerType);
}

function test_write() {
    $repoPath = testbed_get_repo_path();
    $backend = git_odb_backend_loose("$repoPath/objects",7,false,0,0);

    // Here's a blob object.
    $oid = 'bb95160cb6e07358f54a28a208ae41e69889c97b';
    $object = '33330a';

    // $prefix = substr($oid,0,7);
    // var_dump($oid);
    // var_dump($prefix);
    // var_dump($backend->exists_prefix($full,$prefix));
    // var_dump($full);

    // See if the object exists in the ODB.
    if ($backend->exists($oid)) {
        echo "The object already exists in the ODB.\n";

        $data = $backend->read($type,$oid);
        echo "The commit has type=$type (" . GIT_OBJ_BLOB . ")\n";
        echo "Payload:\n";
        var_dump($data);
    }
    else {
        // Write the object into the packfile backend.
        echo "The object does not exist in the ODB.\n";
        var_dump($backend->write($oid,hex2bin($object),GIT_OBJ_BLOB));

        // Now test for existence using the exists_prefix() method.
        $prefix = substr($oid,0,7);
        if ($backend->exists_prefix($full,$prefix)) {
            echo "Found the object that we just wrote: $prefix --> $full.\n";

            $data = $backend->read($type,$oid);
            echo "The commit has type=$type (" . GIT_OBJ_COMMIT . ")\n";
            echo "Payload:\n";
            var_dump($data);
        }
        else {
            throw new Exception("Failed to insert object into backend.");
        }
    }
}

function test_foreach() {
    $repoPath = testbed_get_repo_path();
    $backend = git_odb_backend_loose("$repoPath/objects",7,false,0,0);

    $callback = function($oid,$payload){
        static $n = 1;
        echo "$oid ($payload $n)\n";
        $n += 1;
    };

    $backend->foreach($callback,'object #');
}

function test_writepack() {
    // This is the same pack test as for git_odb_write_pack() except refactored
    // for git_odb_backend.

    $repoPath = testbed_get_repo_path();
    $backend = git_odb_backend_pack("$repoPath/objects");
    $wp = $backend->writepack('var_dump',33);

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
}

function test_writepack2() {
    // This is just like test_writepack() except it uses a slightly different
    // invocation of writepack.

    $objectsdir = testbed_get_root() . '/objects';
    $packdir = "$objectsdir/pack";
    if (!is_dir($packdir)) {
        mkdir($packdir,0777,true);
    }

    $backend = git_odb_backend_pack($objectsdir);
    $wp = $backend->writepack(null,null);

    $pack = '5041434b00000002000000039a09789c95cbc10dc3200c00c03f5378815436a4604'
        . 'b51d55fffddc0a126ed232242b07fb342a4fb5e6f66c05c44a3a9fa5c107995358be7'
        . '3951e2e8a99c54a404733afab73678d7cd1abcceb9b46323d2a7e671b3cf7800cd9c6'
        . '2087427983022ba5cf7fdd7bb5d5b8edc1fd6552bcca102789c33343030333151c848'
        . 'cdc9c967581fa6b665a1c8ead34dd76cb79fe0d0b1710f7b2a0d00bfb20c8f3e789cf'
        . '348cdc9c9d75128cf2fca4951e4020024f20494afed7ccfb9056fe89ef493d1983c26'
        . 'd10f291f39';

    $wp->append(hex2bin($pack));
    echo "The result: \n";
    var_dump($wp->commit());
}

testbed_test('ODB BACKEND READ OBJECT','Git2Test\ODBBackend\test_read');
testbed_test('ODB BACKEND WRITE OBJECT','Git2Test\ODBBackend\test_write');
testbed_test('ODB BACKEND FOREACH','Git2Test\ODBBackend\test_foreach');
testbed_test('ODB BACKEND WRITEPACK','Git2Test\ODBBackend\test_writepack');
testbed_test('ODB BACKEND WRITEPACK/ALT','Git2Test\ODBBackend\test_writepack2');
