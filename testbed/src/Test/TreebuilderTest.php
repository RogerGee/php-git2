<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryTestCase;
use PhpGit2\Callback\CallbackPayload;

final class TreebuilderTest extends RepositoryTestCase {
    /**
     * @phpGitTest git_treebuilder_new
     */
    public function testNew() {
        $repo = static::getRepository();
        $tree = null;
        $result = git_treebuilder_new($repo,$tree);

        $this->assertResourceHasType($result,'git_treebuilder');
    }

    /**
     * @phpGitTest git_treebuilder_new
     */
    public function testNew_WithTree() {
        $repo = static::getRepository();
        $tree = git_tree_lookup(
            $repo,
            '9be15d459a1881c4c656de467fa45376b6886087'
        );
        $result = git_treebuilder_new($repo,$tree);

        $this->assertResourceHasType($result,'git_treebuilder');

        return $result;
    }

    /**
     * @depends testNew_WithTree
     * @phpGitTest git_treebuilder_entrycount
     */
    public function testEntrycount($bld) {
        $result = git_treebuilder_entrycount($bld);

        $this->assertIsInt($result);
    }

    /**
     * @depends testNew_WithTree
     * @phpGitTest git_treebuilder_filter
     */
    public function testFilter($bld) {
        $n_calls = 0;

        $filter = function($entry,$payload) use(&$n_calls) {
            $n_calls += 1;
            $this->assertResourceHasType($entry,'git_tree_entry');
            $this->assertInstanceOf(CallbackPayload::class,$payload);
            return true;
        };
        $payload = new CallbackPayload;
        $result = git_treebuilder_filter($bld,$filter,$payload);

        $this->assertGreaterThan(0,$n_calls);
        $this->assertNull($result);
    }

    /**
     * @depends testNew_WithTree
     * @phpGitTest git_treebuilder_get
     */
    public function testGet($bld) {
        $filename = 'hello.c';
        $result = git_treebuilder_get($bld,$filename);

        $this->assertResourceHasType($result,'git_tree_entry');
    }

    /**
     * @depends testNew_WithTree
     * @phpGitTest git_treebuilder_remove
     */
    public function testRemove($bld) {
        $filename = 'blob';
        $result = git_treebuilder_remove($bld,$filename);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_treebuilder_clear
     */
    public function testClear() {
        $bld = git_treebuilder_new(
            static::getRepository(),
            null
        );
        $result = git_treebuilder_clear($bld);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_treebuilder_free
     */
    public function testFree() {
        $bld = git_treebuilder_new(
            static::getRepository(),
            null
        );

        $result = git_treebuilder_free($bld);

        $this->assertNull($result);
        $this->assertResourceHasType($bld,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_treebuilder[A-Za-z0-9_]* resource/'
        );

        git_treebuilder_clear($bld);
    }

    /**
     * @phpGitTest git_treebuilder_get
     */
    public function testGet_Empty() {
        $bld = git_treebuilder_new(
            static::getRepository(),
            null
        );
        $filename = 'file.txt';
        $result = git_treebuilder_get($bld,$filename);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_treebuilder_insert
     */
    public function testInsert() {
        $repo = static::getRepository();
        $contents = 'A new file to insert.';
        $blobId = git_blob_create_frombuffer($repo,$contents);

        $bld = git_treebuilder_new($repo,null);
        $filename = 'asdf.txt';
        $id = $blobId;
        $filemode = 0100644;
        $result = git_treebuilder_insert($bld,$filename,$id,$filemode);

        $this->assertResourceHasType($result,'git_tree_entry');

        return $bld;
    }
    /**
     * @depends testInsert
     * @phpGitTest git_treebuilder_write
     */
    public function testWrite($bld) {
        $result = git_treebuilder_write($bld);

        $this->assertIsString($result);
    }
}
