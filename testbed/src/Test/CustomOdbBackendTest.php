<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryBareTestCase;
use PhpGit2\Backend\TestODBBackend;
use PhpGit2\Callback\CallbackPayload;

final class CustomOdbBackendTest extends RepositoryBareTestCase {
    /**
     * 
     */
    public function testProperties() {
        $repo = git_repository_new();
        $odb = git_odb_new();

        $backend = new TestODBBackend($this);
        $this->assertNull($backend->odb);
        $this->assertNull($backend->version);

        git_odb_add_backend($odb,$backend,1);
        git_repository_set_odb($repo,$odb);

        $this->assertResourceHasType($backend->odb,'git_odb');
        $this->assertIsInt($backend->version);
    }

    /**
     * 
     */
    public function testReadWrite() {
        $repo = git_repository_new();
        $odb = git_odb_new();
        $backend = new TestODBBackend($this);

        git_odb_add_backend($odb,$backend,1);
        git_repository_set_odb($repo,$odb);

        $data =<<<EOF
PHP is a general purpose programming language, especially well-suited for Web
development. It can be extended through writing extension modules that are either
compiled directly into the engine or loaded dynamically at runtime.
EOF;
        $oid = git_blob_create_frombuffer($repo,$data);
        $blob = git_blob_lookup($repo,$oid);

        $this->assertTrue($backend->wasCalled('exists'));
        $this->assertTrue($backend->wasCalled('read'));
        $this->assertTrue($backend->wasCalled('write'));

        $prefix = substr($oid,0,7);
        $result = git_odb_read_prefix($odb,$prefix);

        $this->assertTrue($backend->wasCalled('read_prefix'));

        $result = git_odb_exists_prefix($odb,$prefix);

        $this->assertTrue($backend->wasCalled('exists_prefix'));

        $size = git_odb_read_header($type,$odb,$oid);

        $this->assertTrue($backend->wasCalled('read_header'));
    }

    /**
     * 
     */
    public function testReadNotFound() {
        $repo = git_repository_new();
        $odb = git_odb_new();
        $backend = new TestODBBackend($this);

        git_odb_add_backend($odb,$backend,1);
        git_repository_set_odb($repo,$odb);

        try {
            $prefix = 'aaaaaaa';
            $result = git_odb_read_prefix($odb,$prefix);

            $this->assertTrue(false);
        } catch (\Exception $ex) {
            $this->assertEquals($ex->getCode(),GIT_ENOTFOUND);
        }

        $this->assertTrue($backend->wasCalled('read_prefix'));

        try {
            $oid = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
            $result = git_odb_read($odb,$oid);

            $this->assertTrue(false);
        } catch (\Exception $ex) {
            $this->assertEquals($ex->getCode(),GIT_ENOTFOUND);
        }

        $this->assertTrue($backend->wasCalled('read'));
    }

    /**
     * 
     */
    public function testForEach() {
        $repo = git_repository_new();
        $odb = git_odb_new();
        $backend = new TestODBBackend($this);

        git_odb_add_backend($odb,$backend,1);
        git_repository_set_odb($repo,$odb);

        $data =<<<EOF
PHP is a general purpose programming language, especially well-suited for Web
development. It can be extended through writing extension modules that are either
compiled directly into the engine or loaded dynamically at runtime.
EOF;
        $oid = git_blob_create_frombuffer($repo,$data);

        $callback = function($oid,$payload) {
            $this->assertIsString($oid);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $payload = new CallbackPayload;
        git_odb_foreach($odb,$callback,$payload);

        $this->assertTrue($backend->wasCalled('for_each'));
    }
}
