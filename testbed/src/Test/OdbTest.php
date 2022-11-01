<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryBareTestCase;
use PhpGit2\Callback\CallbackPayload;

/**
 * @phpGitRemoved git_odb_backend_malloc
 * @phpGitRemoved git_odb_expand_ids
 * @phpGitRemoved git_odb_init_backend
 * @phpGitRemoved git_odb_stream_free
 */
final class OdbTest extends RepositoryBareTestCase {
    /**
     * @phpGitTest git_odb_new
     */
    public function testNew() {
        $result = git_odb_new();

        $this->assertResourceHasType($result,'git_odb');
    }

    /**
     * @phpGitTest git_odb_backend_loose
     */
    public function testBackendLoose() {
        $objectsDir = static::makeDirectory('loose-1');
        $compressionLevel = 1;
        $doFsync = false;
        $dirMode = 0700;
        $fileMode = 0600;
        $result = git_odb_backend_loose(
            $objectsDir,
            $compressionLevel,
            $doFsync,
            $dirMode,
            $fileMode
        );

        $this->assertInstanceOf(\GitODBBackend_Internal::class,$result);
    }

    /**
     * @phpGitTest git_odb_add_alternate
     */
    public function testAddAlternate() {
        $odb = git_odb_new();
        $backend = git_odb_backend_loose(
            static::makeDirectory('loose-2'),
            1,
            false,
            0700,
            0600,
        );
        $priority = 2;
        $result = git_odb_add_alternate($odb,$backend,$priority);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_odb_add_backend
     */
    public function testAddBackend() {
        $odb = git_odb_new();
        $backend = git_odb_backend_loose(
            static::makeDirectory('loose-2'),
            1,
            false,
            0700,
            0600,
        );
        $priority = 2;
        $result = git_odb_add_backend($odb,$backend,$priority);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_odb_add_disk_alternate
     */
    public function testAddDiskAlternate() {
        $odb = static::getRepoOdb();
        $path = static::makeDirectory('loose-1');
        $result = git_odb_add_disk_alternate($odb,$path);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_odb_backend_one_pack
     */
    public function testBackendOnePack() {
        static::copyFile(
            'resources/pack-cdd36839de329689da0e0b7b080509f79cfffff3.pack',
            'packfiles'
        );

        $idxFile = static::copyFile(
            'resources/pack-cdd36839de329689da0e0b7b080509f79cfffff3.idx',
            'packfiles'
        );
        $result = git_odb_backend_one_pack($idxFile);

        $this->assertInstanceOf(\GitODBBackend_Internal::class,$result);
    }

    /**
     * @phpGitTest git_odb_backend_pack
     */
    public function testBackendPack() {
        $objectsDir = static::makePath('packfiles-created');
        $result = git_odb_backend_pack($objectsDir);

        $this->assertInstanceOf(\GitODBBackend_Internal::class,$result);
    }

    /**
     * @phpGitTest git_odb_exists
     */
    public function testExists() {
        $odb = static::getRepoOdb();
        $id = 'aaaabbbbccccddddeeeeffffaabbcceeff00';
        $result = git_odb_exists($odb,$id);

        $this->assertIsBool($result);
    }

    /**
     * @phpGitTest git_odb_exists_prefix
     */
    public function testExistsPrefix() {
        $odb = static::getRepoOdb();
        $prefix = '64db48a';
        $result = git_odb_exists_prefix($odb,$prefix);

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_odb_exists_prefix
     */
    public function testExistsPrefix_NotFound() {
        $this->expectException(\Exception::class);
        $this->expectExceptionCode(GIT_ENOTFOUND);

        $odb = static::getRepoOdb();
        $prefix = str_repeat('1',40);
        $result = git_odb_exists_prefix($odb,$prefix);
    }

    /**
     * @phpGitTest git_odb_exists_prefix
     */
    public function testExistsPrefix_Ambiguous() {
        $this->expectException(\Exception::class);
        $this->expectExceptionCode(GIT_EAMBIGUOUS);

        $odb = static::getRepoOdb();
        $prefix = 'a';
        $result = git_odb_exists_prefix($odb,$prefix);
    }

    /**
     * @phpGitTest git_odb_foreach
     */
    public function testForeach() {
        $n = 0;

        $odb = static::getRepoOdb();
        $callback = function($id,$payload) use(&$n) {
            $n += 1;
            $this->assertIsString($id);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $payload = new CallbackPayload;
        $result = git_odb_foreach($odb,$callback,$payload);

        $this->assertNull($result);
        $this->assertGreaterThan(1,$n);
    }

    /**
     * @phpGitTest git_odb_foreach
     */
    public function testForeach_StopIter() {
        $n = 0;

        $odb = static::getRepoOdb();
        $callback = function($id,$payload) use(&$n) {
            $n += 1;
            $this->assertIsString($id);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
            return false;
        };
        $payload = new CallbackPayload;
        $result = git_odb_foreach($odb,$callback,$payload);

        $this->assertNull($result);
        $this->assertEquals(1,$n);
    }

    /**
     * @phpGitTest git_odb_free
     */
    public function testFree() {
        $odb = git_odb_new();

        $result = git_odb_free($odb);

        $this->assertNull($result);
        $this->assertResourceHasType($odb,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_odb[A-Za-z0-9_]* resource/'
        );

        git_odb_num_backends($odb);
    }

    /**
     * @phpGitTest git_odb_get_backend
     */
    public function testGetBackend() {
        $odb = static::getRepoOdb();
        $pos = 0;
        $result = git_odb_get_backend($odb,$pos);

        $this->assertInstanceOf(\GitODBBackend_Internal::class,$result);
    }

    /**
     * @phpGitTest git_odb_hash
     */
    public function testHash() {
        $data = "\x74\x72\x65\x65\x20\x37\x64\x64\x62\x36\x36\x66"
               ."\x30\x38\x62\x34\x31\x66\x34\x63\x36\x66\x35\x65"
               ."\x63\x61\x66\x35\x36\x32\x38\x63\x39\x33\x61\x65"
               ."\x31\x64\x66\x36\x34\x32\x36\x33\x33\x0a\x70\x61"
               ."\x72\x65\x6e\x74\x20\x65\x62\x66\x64\x33\x39\x39"
               ."\x64\x36\x36\x37\x31\x30\x32\x38\x30\x39\x35\x37"
               ."\x32\x37\x35\x33\x30\x30\x34\x32\x62\x36\x62\x30"
               ."\x37\x61\x35\x65\x38\x64\x61\x65\x61\x0a\x61\x75"
               ."\x74\x68\x6f\x72\x20\x52\x6f\x67\x65\x72\x20\x47"
               ."\x65\x65\x20\x3c\x72\x70\x67\x31\x31\x61\x40\x61"
               ."\x63\x75\x2e\x65\x64\x75\x3e\x20\x31\x36\x30\x37"
               ."\x34\x38\x39\x31\x35\x34\x20\x2d\x30\x36\x30\x30"
               ."\x0a\x63\x6f\x6d\x6d\x69\x74\x74\x65\x72\x20\x52"
               ."\x6f\x67\x65\x72\x20\x47\x65\x65\x20\x3c\x72\x70"
               ."\x67\x31\x31\x61\x40\x61\x63\x75\x2e\x65\x64\x75"
               ."\x3e\x20\x31\x36\x30\x37\x34\x38\x39\x31\x35\x34"
               ."\x20\x2d\x30\x36\x30\x30\x0a\x0a\x4d\x6f\x76\x65"
               ."\x20\x63\x75\x73\x74\x6f\x6d\x20\x7a\x65\x6e\x64"
               ."\x5f\x6f\x62\x6a\x65\x63\x74\x5f\x68\x61\x6e\x64"
               ."\x6c\x65\x72\x73\x2c\x69\x6e\x69\x74\x20\x74\x6f"
               ."\x20\x70\x68\x70\x5f\x7a\x65\x6e\x64\x5f\x6f\x62"
               ."\x6a\x65\x63\x74\x0a\x0a\x54\x68\x69\x73\x20\x63"
               ."\x6f\x6d\x6d\x69\x74\x20\x61\x6c\x73\x6f\x20\x65"
               ."\x6e\x73\x75\x72\x65\x73\x20\x74\x68\x61\x74\x20"
               ."\x74\x68\x65\x20\x70\x68\x70\x5f\x7a\x65\x6e\x64"
               ."\x5f\x6f\x62\x6a\x65\x63\x74\x3c\x74\x3e\x20\x69"
               ."\x73\x20\x61\x6c\x77\x61\x79\x73\x20\x69\x6e\x0a"
               ."\x73\x74\x61\x6e\x64\x61\x72\x64\x20\x6c\x61\x79"
               ."\x6f\x75\x74\x2c\x20\x77\x68\x69\x63\x68\x20\x69"
               ."\x73\x20\x6e\x65\x63\x65\x73\x73\x61\x72\x79\x20"
               ."\x69\x6e\x20\x6f\x72\x64\x65\x72\x20\x74\x6f\x20"
               ."\x64\x65\x72\x69\x76\x65\x20\x69\x74\x20\x66\x72"
               ."\x6f\x6d\x20\x74\x68\x65\x0a\x7a\x65\x6e\x64\x5f"
               ."\x6f\x62\x6a\x65\x63\x74\x2e\x0a";
        $type = GIT_OBJ_COMMIT;
        $result = git_odb_hash($data,$type);

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_odb_hashfile
     */
    public function testHashfile() {
        $contents = "\x74\x72\x65\x65\x20\x37\x64\x64\x62\x36\x36\x66"
                   ."\x30\x38\x62\x34\x31\x66\x34\x63\x36\x66\x35\x65"
                   ."\x63\x61\x66\x35\x36\x32\x38\x63\x39\x33\x61\x65"
                   ."\x31\x64\x66\x36\x34\x32\x36\x33\x33\x0a\x70\x61"
                   ."\x72\x65\x6e\x74\x20\x65\x62\x66\x64\x33\x39\x39"
                   ."\x64\x36\x36\x37\x31\x30\x32\x38\x30\x39\x35\x37"
                   ."\x32\x37\x35\x33\x30\x30\x34\x32\x62\x36\x62\x30"
                   ."\x37\x61\x35\x65\x38\x64\x61\x65\x61\x0a\x61\x75"
                   ."\x74\x68\x6f\x72\x20\x52\x6f\x67\x65\x72\x20\x47"
                   ."\x65\x65\x20\x3c\x72\x70\x67\x31\x31\x61\x40\x61"
                   ."\x63\x75\x2e\x65\x64\x75\x3e\x20\x31\x36\x30\x37"
                   ."\x34\x38\x39\x31\x35\x34\x20\x2d\x30\x36\x30\x30"
                   ."\x0a\x63\x6f\x6d\x6d\x69\x74\x74\x65\x72\x20\x52"
                   ."\x6f\x67\x65\x72\x20\x47\x65\x65\x20\x3c\x72\x70"
                   ."\x67\x31\x31\x61\x40\x61\x63\x75\x2e\x65\x64\x75"
                   ."\x3e\x20\x31\x36\x30\x37\x34\x38\x39\x31\x35\x34"
                   ."\x20\x2d\x30\x36\x30\x30\x0a\x0a\x4d\x6f\x76\x65"
                   ."\x20\x63\x75\x73\x74\x6f\x6d\x20\x7a\x65\x6e\x64"
                   ."\x5f\x6f\x62\x6a\x65\x63\x74\x5f\x68\x61\x6e\x64"
                   ."\x6c\x65\x72\x73\x2c\x69\x6e\x69\x74\x20\x74\x6f"
                   ."\x20\x70\x68\x70\x5f\x7a\x65\x6e\x64\x5f\x6f\x62"
                   ."\x6a\x65\x63\x74\x0a\x0a\x54\x68\x69\x73\x20\x63"
                   ."\x6f\x6d\x6d\x69\x74\x20\x61\x6c\x73\x6f\x20\x65"
                   ."\x6e\x73\x75\x72\x65\x73\x20\x74\x68\x61\x74\x20"
                   ."\x74\x68\x65\x20\x70\x68\x70\x5f\x7a\x65\x6e\x64"
                   ."\x5f\x6f\x62\x6a\x65\x63\x74\x3c\x74\x3e\x20\x69"
                   ."\x73\x20\x61\x6c\x77\x61\x79\x73\x20\x69\x6e\x0a"
                   ."\x73\x74\x61\x6e\x64\x61\x72\x64\x20\x6c\x61\x79"
                   ."\x6f\x75\x74\x2c\x20\x77\x68\x69\x63\x68\x20\x69"
                   ."\x73\x20\x6e\x65\x63\x65\x73\x73\x61\x72\x79\x20"
                   ."\x69\x6e\x20\x6f\x72\x64\x65\x72\x20\x74\x6f\x20"
                   ."\x64\x65\x72\x69\x76\x65\x20\x69\x74\x20\x66\x72"
                   ."\x6f\x6d\x20\x74\x68\x65\x0a\x7a\x65\x6e\x64\x5f"
                   ."\x6f\x62\x6a\x65\x63\x74\x2e\x0a";
        static::makeDirectory('odb-test-files');

        $path = static::makeFile($contents,'odb-test-files','testHashfile');
        $type = GIT_OBJ_COMMIT;
        $result = git_odb_hashfile($path,$type);

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_odb_num_backends
     */
    public function testNumBackends() {
        $odb = static::getRepoOdb();
        $result = git_odb_num_backends($odb);

        $this->assertIsInt($result);
    }

    /**
     * @phpGitTest git_odb_open
     */
    public function testOpen() {
        $objectsDir = static::makeDirectory('loose-3');
        $result = git_odb_open($objectsDir);

        $this->assertResourceHasType($result,'git_odb');

        return $result;
    }

    // TODO: Use backend that can stream directly.
    // /**
    //  * @phpGitTest git_odb_open_rstream
    //  */
    // public function testOpenRstream() {
    //     $odb = static::getRepoOdb();
    //     $oid = 'faf545194b3df246b2b80ce44369371ec9fe2e68';
    //     $result = git_odb_open_rstream($size,$type,$odb,$oid);

    //     $this->assertInstanceOf(\GitODBStream_Internal::class,$result);

    //     return $result;
    // }

    // TODO: Pending testOpenRstream()
    // /**
    //  * @depends testOpenRstream
    //  * @phpGitTest git_odb_stream_read
    //  */
    // public function testStreamRead($stream) {

    // }

    /**
     * @depends testOpen
     * @phpGitTest git_odb_open_wstream
     */
    public function testOpenWstream($odb) {
        $size = 404;
        $type = GIT_OBJ_COMMIT;
        $result = git_odb_open_wstream($odb,$size,$type);

        $this->assertInstanceOf(\GitODBStream_Internal::class,$result);

        return $result;
    }

    /**
     * @depends testOpenWstream
     * @phpGitTest git_odb_stream_write
     */
    public function testStreamWrite($stream) {
        $data = "\x74\x72\x65\x65\x20\x37\x64\x64\x62\x36\x36\x66"
               ."\x30\x38\x62\x34\x31\x66\x34\x63\x36\x66\x35\x65"
               ."\x63\x61\x66\x35\x36\x32\x38\x63\x39\x33\x61\x65"
               ."\x31\x64\x66\x36\x34\x32\x36\x33\x33\x0a\x70\x61"
               ."\x72\x65\x6e\x74\x20\x65\x62\x66\x64\x33\x39\x39"
               ."\x64\x36\x36\x37\x31\x30\x32\x38\x30\x39\x35\x37"
               ."\x32\x37\x35\x33\x30\x30\x34\x32\x62\x36\x62\x30"
               ."\x37\x61\x35\x65\x38\x64\x61\x65\x61\x0a\x61\x75"
               ."\x74\x68\x6f\x72\x20\x52\x6f\x67\x65\x72\x20\x47"
               ."\x65\x65\x20\x3c\x72\x70\x67\x31\x31\x61\x40\x61"
               ."\x63\x75\x2e\x65\x64\x75\x3e\x20\x31\x36\x30\x37"
               ."\x34\x38\x39\x31\x35\x34\x20\x2d\x30\x36\x30\x30"
               ."\x0a\x63\x6f\x6d\x6d\x69\x74\x74\x65\x72\x20\x52"
               ."\x6f\x67\x65\x72\x20\x47\x65\x65\x20\x3c\x72\x70"
               ."\x67\x31\x31\x61\x40\x61\x63\x75\x2e\x65\x64\x75"
               ."\x3e\x20\x31\x36\x30\x37\x34\x38\x39\x31\x35\x34"
               ."\x20\x2d\x30\x36\x30\x30\x0a\x0a\x4d\x6f\x76\x65"
               ."\x20\x63\x75\x73\x74\x6f\x6d\x20\x7a\x65\x6e\x64"
               ."\x5f\x6f\x62\x6a\x65\x63\x74\x5f\x68\x61\x6e\x64"
               ."\x6c\x65\x72\x73\x2c\x69\x6e\x69\x74\x20\x74\x6f"
               ."\x20\x70\x68\x70\x5f\x7a\x65\x6e\x64\x5f\x6f\x62"
               ."\x6a\x65\x63\x74\x0a\x0a\x54\x68\x69\x73\x20\x63"
               ."\x6f\x6d\x6d\x69\x74\x20\x61\x6c\x73\x6f\x20\x65"
               ."\x6e\x73\x75\x72\x65\x73\x20\x74\x68\x61\x74\x20"
               ."\x74\x68\x65\x20\x70\x68\x70\x5f\x7a\x65\x6e\x64"
               ."\x5f\x6f\x62\x6a\x65\x63\x74\x3c\x74\x3e\x20\x69"
               ."\x73\x20\x61\x6c\x77\x61\x79\x73\x20\x69\x6e\x0a"
               ."\x73\x74\x61\x6e\x64\x61\x72\x64\x20\x6c\x61\x79"
               ."\x6f\x75\x74\x2c\x20\x77\x68\x69\x63\x68\x20\x69"
               ."\x73\x20\x6e\x65\x63\x65\x73\x73\x61\x72\x79\x20"
               ."\x69\x6e\x20\x6f\x72\x64\x65\x72\x20\x74\x6f\x20"
               ."\x64\x65\x72\x69\x76\x65\x20\x69\x74\x20\x66\x72"
               ."\x6f\x6d\x20\x74\x68\x65\x0a\x7a\x65\x6e\x64\x5f"
               ."\x6f\x62\x6a\x65\x63\x74\x2e\x0a";
        $result = git_odb_stream_write($stream,$data);

        $this->assertNull($result);

        return $stream;
    }

    /**
     * @depends testStreamWrite
     * @phpGitTest git_odb_stream_finalize_write
     */
    public function testStreamFinalizeWrite($stream) {
        $result = git_odb_stream_finalize_write($stream);

        $this->assertIsString($result);

        return $stream;
    }

    /**
     * @phpGitTest git_odb_read
     */
    public function testRead() {
        $odb = static::getRepoOdb();
        $oid = 'faf545194b3df246b2b80ce44369371ec9fe2e68';
        $result = git_odb_read($odb,$oid);

        $this->assertResourceHasType($result,'git_odb_object');

        return $result;
    }

    /**
     * @depends testRead
     * @phpGitTest git_odb_object_data
     */
    public function testOdbObjectData($object) {
        $result = git_odb_object_data($object);

        $this->assertIsString($result);
    }

    /**
     * @depends testRead
     * @phpGitTest git_odb_object_id
     */
    public function testOdbObjectId($object) {
        $result = git_odb_object_id($object);

        $this->assertIsString($result);
    }

    /**
     * @depends testRead
     * @phpGitTest git_odb_object_dup
     */
    public function testOdbObjectDup($object) {
        $result = git_odb_object_dup($object);

        $this->assertResourceHasType($result,'git_odb_object');

        return $result;
    }

    /**
     * @depends testOdbObjectDup
     * @phpGitTest git_odb_object_free
     */
    public function testOdbObjectFree($object) {
        $result = git_odb_object_free($object);

        $this->assertNull($result);
        $this->assertResourceHasType($object,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_odb_object[A-Za-z0-9_]* resource/'
        );

        git_odb_object_data($object);
    }

    /**
     * @depends testRead
     * @phpGitTest git_odb_object_size
     */
    public function testOdbObjectSize($object) {
        $result = git_odb_object_size($object);

        $this->assertIsInt($result);
    }

    /**
     * @depends testRead
     * @phpGitTest git_odb_object_type
     */
    public function testOdbObjectType($object) {
        $result = git_odb_object_type($object);

        $this->assertIsInt($result);
    }

    /**
     * @phpGitTest git_odb_read_header
     */
    public function testReadHeader() {
        $odb = static::getRepoOdb();
        $oid = 'faf545194b3df246b2b80ce44369371ec9fe2e68';
        $result = git_odb_read_header($type,$odb,$oid);

        $this->assertIsInt($result);
        $this->assertIsInt($type);
    }

    /**
     * @phpGitTest git_odb_read_prefix
     */
    public function testReadPrefix() {
        $odb = static::getRepoOdb();
        $oid = 'faf5451';
        $result = git_odb_read_prefix($odb,$oid);

        $this->assertResourceHasType($result,'git_odb_object');
    }

    /**
     * @phpGitTest git_odb_refresh
     */
    public function testRefresh() {
        $odb = static::getRepoOdb();
        $result = git_odb_refresh($odb);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_odb_write
     */
    public function testWrite() {
        $odb = static::getRepoOdb();
        $data = "\x74\x72\x65\x65\x20\x37\x64\x64\x62\x36\x36\x66"
               ."\x30\x38\x62\x34\x31\x66\x34\x63\x36\x66\x35\x65"
               ."\x63\x61\x66\x35\x36\x32\x38\x63\x39\x33\x61\x65"
               ."\x31\x64\x66\x36\x34\x32\x36\x33\x33\x0a\x70\x61"
               ."\x72\x65\x6e\x74\x20\x65\x62\x66\x64\x33\x39\x39"
               ."\x64\x36\x36\x37\x31\x30\x32\x38\x30\x39\x35\x37"
               ."\x32\x37\x35\x33\x30\x30\x34\x32\x62\x36\x62\x30"
               ."\x37\x61\x35\x65\x38\x64\x61\x65\x61\x0a\x61\x75"
               ."\x74\x68\x6f\x72\x20\x52\x6f\x67\x65\x72\x20\x47"
               ."\x65\x65\x20\x3c\x72\x70\x67\x31\x31\x61\x40\x61"
               ."\x63\x75\x2e\x65\x64\x75\x3e\x20\x31\x36\x30\x37"
               ."\x34\x38\x39\x31\x35\x34\x20\x2d\x30\x36\x30\x30"
               ."\x0a\x63\x6f\x6d\x6d\x69\x74\x74\x65\x72\x20\x52"
               ."\x6f\x67\x65\x72\x20\x47\x65\x65\x20\x3c\x72\x70"
               ."\x67\x31\x31\x61\x40\x61\x63\x75\x2e\x65\x64\x75"
               ."\x3e\x20\x31\x36\x30\x37\x34\x38\x39\x31\x35\x34"
               ."\x20\x2d\x30\x36\x30\x30\x0a\x0a\x4d\x6f\x76\x65"
               ."\x20\x63\x75\x73\x74\x6f\x6d\x20\x7a\x65\x6e\x64"
               ."\x5f\x6f\x62\x6a\x65\x63\x74\x5f\x68\x61\x6e\x64"
               ."\x6c\x65\x72\x73\x2c\x69\x6e\x69\x74\x20\x74\x6f"
               ."\x20\x70\x68\x70\x5f\x7a\x65\x6e\x64\x5f\x6f\x62"
               ."\x6a\x65\x63\x74\x0a\x0a\x54\x68\x69\x73\x20\x63"
               ."\x6f\x6d\x6d\x69\x74\x20\x61\x6c\x73\x6f\x20\x65"
               ."\x6e\x73\x75\x72\x65\x73\x20\x74\x68\x61\x74\x20"
               ."\x74\x68\x65\x20\x70\x68\x70\x5f\x7a\x65\x6e\x64"
               ."\x5f\x6f\x62\x6a\x65\x63\x74\x3c\x74\x3e\x20\x69"
               ."\x73\x20\x61\x6c\x77\x61\x79\x73\x20\x69\x6e\x0a"
               ."\x73\x74\x61\x6e\x64\x61\x72\x64\x20\x6c\x61\x79"
               ."\x6f\x75\x74\x2c\x20\x77\x68\x69\x63\x68\x20\x69"
               ."\x73\x20\x6e\x65\x63\x65\x73\x73\x61\x72\x79\x20"
               ."\x69\x6e\x20\x6f\x72\x64\x65\x72\x20\x74\x6f\x20"
               ."\x64\x65\x72\x69\x76\x65\x20\x69\x74\x20\x66\x72"
               ."\x6f\x6d\x20\x74\x68\x65\x0a\x7a\x65\x6e\x64\x5f"
               ."\x6f\x62\x6a\x65\x63\x74\x2e\x0a";
        $type = GIT_OBJ_COMMIT;
        $result = git_odb_write($odb,$data,$type);

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_odb_write_pack
     */
    public function testWritePack() {
        $odb = static::getRepoOdb();
        $processCb = function($stats,$payload) {

        };
        $payload = new CallbackPayload;
        $result = git_odb_write_pack($odb,$processCb,$payload);

        $this->assertInstanceOf(\GitODBWritepack_Internal::class,$result);
    }

    protected static function getRepoOdb() {
        static $odb;

        if (!isset($odb)) {
            $repo = static::getRepository();
            $odb = git_repository_odb($repo);
        }

        return $odb;
    }
}
