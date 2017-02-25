<?php

require_once 'test_base.php';
require_once 'test_odb-writepack.php';

testbed_test('BACKEND READ OBJECT',function(){
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
});

testbed_test('BACKEND WRITE OBJECT',function(){
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
});
