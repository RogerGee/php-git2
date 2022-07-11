<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryBareTestCase;
use PhpGit2\Callback\CallbackPayload;
use PhpGit2\Callback\CallbackReturnValue;

final class TreeTest extends RepositoryBareTestCase {
    /**
     * @phpGitTest git_tree_lookup
     */
    public function testLookup() {
        $repo = static::getRepository();
        $id = '9be15d459a1881c4c656de467fa45376b6886087';
        $result = git_tree_lookup($repo,$id);

        $this->assertResourceHasType($result,'git_tree');

        return $result;
    }

    // /**
    //  * @depends testLookup
    //  * @phpGitTest git_tree_create_updated
    //  */
    // public function testCreateUpdated($tree) {

    // }

    /**
     * @depends testLookup
     * @phpGitTest git_tree_dup
     */
    public function testDup($tree) {
        $result = git_tree_dup($tree);

        $this->assertResourceHasType($result,'git_tree');
    }

    /**
     * @depends testLookup
     * @phpGitTest git_tree_entry_byid
     */
    public function testEntryByid($tree) {
        $id = '82a21601e6135604b75a969a3c338ab827bc4d35';
        $result = git_tree_entry_byid($tree,$id);

        $this->assertResourceHasType($result,'git_tree_entry');

        return $result;
    }

    /**
     * @depends testEntryByid
     * @phpGitTest git_tree_entry_dup
     */
    public function testEntryDup($entry) {
        $result = git_tree_entry_dup($entry);

        $this->assertResourceHasType($result,'git_tree_entry');
    }

    /**
     * @depends testEntryByid
     * @phpGitTest git_tree_entry_filemode
     */
    public function testEntryFilemode($entry) {
        $result = git_tree_entry_filemode($entry);

        $this->assertIsInt($result);
    }

    /**
     * @depends testEntryByid
     * @phpGitTest git_tree_entry_filemode_raw
     */
    public function testEntryFilemodeRaw($entry) {
        $result = git_tree_entry_filemode_raw($entry);

        $this->assertIsInt($result);
    }

    /**
     * @depends testEntryByid
     * @phpGitTest git_tree_entry_id
     */
    public function testEntryId($entry) {
        $result = git_tree_entry_id($entry);

        $this->assertIsString($result);
    }

    /**
     * @depends testEntryByid
     * @phpGitTest git_tree_entry_name
     */
    public function testEntryName($entry) {
        $result = git_tree_entry_id($entry);

        $this->assertIsString($result);
    }

    /**
     * @depends testEntryByid
     * @phpGitTest git_tree_entry_to_object
     */
    public function testEntryToObject($entry) {
        $repo = static::getRepository();
        $result = git_tree_entry_to_object($repo,$entry);

        $this->assertResourceHasType($result,'git_object');
    }

    /**
     * @depends testEntryByid
     * @phpGitTest git_tree_entry_type
     */
    public function testEntryType($entry) {
        $result = git_tree_entry_type($entry);

        $this->assertIsInt($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_tree_entry_byid
     */
    public function testEntryByid_NULL($tree) {
        $id = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
        $result = git_tree_entry_byid($tree,$id);

        $this->assertNull($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_tree_entry_byid
     */
    public function testEntryByindex($tree) {
        $idx = 0;
        $result = git_tree_entry_byindex($tree,$idx);

        $this->assertResourceHasType($result,'git_tree_entry');

        return $result;
    }

    /**
     * @depends testLookup
     * @phpGitTest git_tree_entry_byid
     */
    public function testEntryByindex_NULL($tree) {
        $idx = 333;
        $result = git_tree_entry_byindex($tree,$idx);

        $this->assertNull($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_tree_entry_byname
     */
    public function testEntryByname($tree) {
        $name = 'hello.c';
        $result = git_tree_entry_byname($tree,$name);

        $this->assertResourceHasType($result,'git_tree_entry');

        return $result;
    }

    /**
     * @depends testLookup
     * @phpGitTest git_tree_entry_byname
     */
    public function testEntryByname_NULL($tree) {
        $name = 'abcdefghijklmnopqrstuvwxyz';
        $result = git_tree_entry_byname($tree,$name);

        $this->assertNull($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_tree_entry_bypath
     */
    public function testEntryBypath($tree) {
        $path = 'blob';
        $result = git_tree_entry_bypath($tree,$path);

        $this->assertResourceHasType($result,'git_tree_entry');

        return $result;
    }

    /**
     * @depends testEntryBypath
     * @phpGitTest git_tree_entry_free
     */
    public function testEntryFree($entry) {
        $result = git_tree_entry_free($entry);

        $this->assertNull($result);
        $this->assertResourceHasType($entry,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_tree_entry[A-Za-z0-9_]* resource/'
        );

        git_tree_entry_id($entry);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_tree_entry_bypath
     */
    public function testEntryBypath_ENOTFOUND($tree) {
        $this->expectException(\Exception::class);
        $this->expectExceptionMessage('does not exist in the given tree');
        $this->expectExceptionCode(GIT_ENOTFOUND);

        $path = 'i/do/not/exist';
        $result = git_tree_entry_bypath($tree,$path);
    }

    /**
     * @depends testEntryByid
     * @depends testEntryByname
     * @phpGitTest git_tree_entry_cmp
     */
    public function testEntryCmp($e1,$e2) {
        $result = git_tree_entry_cmp($e1,$e2);

        $this->assertIsInt($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_tree_entrycount
     */
    public function testEntrycount($tree) {
        $result = git_tree_entrycount($tree);

        $this->assertIsInt($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_tree_id
     */
    public function testId($tree) {
        $result = git_tree_id($tree);

        $this->assertIsString($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_tree_owner
     */
    public function testOwner($tree) {
        $result = git_tree_owner($tree);

        $this->assertResourceHasType($result,'git_repository');
    }

    /**
     * @depends testLookup
     * @phpGitTest git_tree_walk
     */
    public function testWalk($tree) {
        $n = 0;

        $mode = 0;
        $callback = function($root,$entry,$payload) use(&$n) {
            $n += 1;
            $this->assertIsString($root);
            $this->assertResourceHasType($entry,'git_tree_entry');
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $payload = new CallbackPayload;
        $result = git_tree_walk($tree,$mode,$callback,$payload);

        $this->assertGreaterThan(0,$n);
        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_tree_lookup_prefix
     */
    public function testLookupPrefix() {
        $repo = static::getRepository();
        $id = 'b962d9';
        $result = git_tree_lookup_prefix($repo,$id);

        $this->assertResourceHasType($result,'git_tree');

        return $result;
    }

    /**
     * @depends testLookupPrefix
     * @phpGitTest git_tree_free
     */
    public function testFree($tree) {
        $result = git_tree_free($tree);

        $this->assertNull($result);
        $this->assertResourceHasType($tree,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_tree[A-Za-z0-9_]* resource/'
        );

        git_tree_id($tree);
    }
}
