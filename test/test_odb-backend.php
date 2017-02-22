<?php

require_once 'test_base.php';
require_once 'test_odb-writepack.php';

testbed_test('BACKEND READ OBJECT',function(){
    $repoPath = testbed_get_repo_path();
    $repo = git_repository_open_bare($repoPath);
    $odb = git_repository_odb($repo);

    $oid = 'af5626b4a114abcb82d63db7c8082c3c4756e51b';

    $backend = git_odb_backend_pack("$repoPath/objects");
    var_dump($backend);
    echo "\n";

    // Read blob from the pack inserted in test_odb-writepack. It should contain
    // the text "Hello, world!\n".
    var_dump($backend->read($size,$type,$oid));
    var_dump($size);
    var_dump($type);
    echo "\n";

    // Now try the same thing but use an abbreviated id.
    $abbrev = 'af5626b';
    var_dump($backend->read_prefix($outid,$a,$b,$abbrev));
    var_dump($outid);
    var_dump($a);
    var_dump($b);
    echo "\n";

    // Now try the same thing but just get the header.
    var_dump($backend->read_header($headerSize,$headerType,$oid));
    var_dump($headerSize);
    var_dump($headerType);
});
