<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryBareTestCase;
use PhpGit2\Callback\CallbackPayload;

final class PackbuilderTest extends RepositoryBareTestCase {
    private static $packfilePath;

    public static function setUpBeforeClass() : void {
        parent::setUpBeforeClass();

        self::$packfilePath = static::makeDirectory('packfiles');
    }

    /**
     * @phpGitTest git_packbuilder_new
     */
    public function testNew() {
        $repo = static::getRepository();
        $result = git_packbuilder_new($repo);

        $this->assertResourceHasType($result,'git_packbuilder');

        return $result;
    }

    /**
     * @phpGitTest git_packbuilder_free
     */
    public function testFree() {
        $repo = static::getRepository();
        $pb = git_packbuilder_new($repo);

        $result = git_packbuilder_free($pb);

        $this->assertNull($result);
        $this->assertResourceHasType($pb,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_packbuilder[A-Za-z0-9_]* resource/'
        );

        git_packbuilder_written($pb);
    }

    /**
     * @phpGitTest git_packbuilder_set_callbacks
     */
    public function testSetCallbacks() {
        $repo = static::getRepository();
        $pb = git_packbuilder_new($repo);

        $n = 0;

        $progressCb = function($stage,$current,$total,$payload) use(&$n) {
            $n += 1;
            $this->assertIsInt($stage);
            $this->assertIsInt($current);
            $this->assertIsInt($total);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $payload = new CallbackPayload;
        $result = git_packbuilder_set_callbacks($pb,$progressCb,$payload);

        $this->assertNull($result);

        git_packbuilder_insert_commit(
            $pb,
            '5019e8cd0ed3afed8afb6717776a388817a4a914'
        );

        git_packbuilder_write($pb,self::$packfilePath,0600,null,null);

        $this->assertGreaterThan(0,$n);
    }

    /**
     * @depends testNew
     * @phpGitTest git_packbuilder_insert
     */
    public function testInsert($pb) {
        $id = '64db48af90133136eda7414dfd79783a513287a9';
        $name = null;
        $result = git_packbuilder_insert($pb,$id,$name);

        $this->assertNull($result);
    }

    /**
     * @depends testNew
     * @phpGitTest git_packbuilder_insert_commit
     */
    public function testInsertCommit($pb) {
        $id = '5019e8cd0ed3afed8afb6717776a388817a4a914';
        $result = git_packbuilder_insert_commit($pb,$id);

        $this->assertNull($result);
    }

    /**
     * @depends testNew
     * @phpGitTest git_packbuilder_insert_recur
     */
    public function testInsertRecur($pb) {
        $id = '2cd82ceaee5897c72eb8fded83632569c80c69c5';
        $name = 'test';
        $result = git_packbuilder_insert_recur($pb,$id,$name);

        $this->assertNull($result);
    }

    /**
     * @depends testNew
     * @phpGitTest git_packbuilder_insert_tree
     */
    public function testInsertTree($pb) {
        $id = 'b962d96869a2e2acf3efd6541670faf7bc58dd11';
        $result = git_packbuilder_insert_tree($pb,$id);

        $this->assertNull($result);
    }

    /**
     * @depends testNew
     * @phpGitTest git_packbuilder_insert_walk
     */
    public function testInsertWalk($pb) {
        $repo = static::getRepository();
        $walk = git_revwalk_new($repo);
        git_revwalk_push($walk,'7f5f4d7f51d18271093f0b30c86d038c7696fc5b');

        $result = git_packbuilder_insert_walk($pb,$walk);

        $this->assertNull($result);
    }

    /**
     * @depends testNew
     * @phpGitTest git_packbuilder_object_count
     */
    public function testObjectCount($pb) {
        $result = git_packbuilder_object_count($pb);

        $this->assertIsInt($result);
    }

    /**
     * @depends testNew
     * @phpGitTest git_packbuilder_set_threads
     */
    public function testSetThreads($pb) {
        $n = 2;
        $result = git_packbuilder_set_threads($pb,$n);

        $this->assertNull($result);

        return $pb;
    }

    /**
     * @depends testSetThreads
     * @phpGitTest git_packbuilder_write
     */
    public function testWrite($pb) {
        $n = 0;

        $path = self::$packfilePath;
        $mode = 0600;
        $progressCb = function($stats,$payload) use(&$n) {
            $n += 1;
            $this->assertIsArray($stats);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $payload = new CallbackPayload;
        $result = git_packbuilder_write($pb,$path,$mode,$progressCb,$payload);

        $this->assertGreaterThan(0,$n);
        $this->assertNull($result);

        return $pb;
    }

    /**
     * @depends testWrite
     * @phpGitTest git_packbuilder_hash
     */
    public function testHash($pb) {
        $result = git_packbuilder_hash($pb);

        $this->assertIsString($result);
    }

    /**
     * @depends testWrite
     * @phpGitTest git_packbuilder_foreach
     */
    public function testForeach($pb) {
        $n = 0;

        $cb = function($buf,$size,$payload) use(&$n) {
            $n += 1;
            $this->assertIsString($buf);
            $this->assertIsInt($size);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $payload = new CallbackPayload;
        $result = git_packbuilder_foreach($pb,$cb,$payload);

        $this->assertGreaterThan(0,$n);
        $this->assertNull($result);
    }

    /**
     * @depends testWrite
     * @phpGitTest git_packbuilder_foreach
     */
    public function testForeach_Stream($pb) {
        $cb = null;
        $payload = static::makeFileStream('sample.file');
        $result = git_packbuilder_foreach($pb,$cb,$payload);

        $this->assertGreaterThan(0,ftell($payload));
        $this->assertNull($result);
    }

    /**
     * @depends testNew
     * @phpGitTest git_packbuilder_written
     */
    public function testWritten($pb) {
        $result = git_packbuilder_written($pb);

        $this->assertIsInt($result);
    }
}
