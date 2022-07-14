<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryTestCase;
use PhpGit2\Callback\CallbackPayload;

final class IndexTest extends RepositoryTestCase {
    /**
     * @phpGitTest git_index_add
     */
    public function testAdd() {
        $path = static::makeFile('Hello, Override!','repo','hello.c');
        //$statbuf = stat($path);

        $index = static::getRepoIndex();
        $entry = [
            'id' => '82a21601e6135604b75a969a3c338ab827bc4d35',
            'path' => 'hello.c',
            'mode' => GIT_FILEMODE_BLOB,
        ]/* + $statbuf*/;
        $result = git_index_add($index,$entry);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_index_add_all
     */
    public function testAddAll() {
        static::makeFile('a','repo','add.all.a.txt');
        static::makeFile('b','repo','add.all.b.txt');
        static::makeFile('c','repo','add.all.c.txt');

        $n = 0;

        $index = static::getRepoIndex();
        $pathspec = ['*.txt'];
        $flags = 0;
        $callback = function($path,$matchedPathspec,$payload) use(&$n) {
            $n += 1;
            $this->assertIsString($path);
            $this->assertIsString($matchedPathspec);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $payload = new CallbackPayload;
        $result = git_index_add_all($index,$pathspec,$flags,$callback,$payload);

        $this->assertGreaterThan(0,$n);
        $this->assertIsBool($result);
    }

    /**
     * @phpGitTest git_index_add_bypath
     */
    public function testAddBypath() {
        $index = static::getRepoIndex();
        $localPath = 'add.bypath.txt';
        static::makeFile('Contents of new file','repo',$localPath);
        $result = git_index_add_bypath($index,$localPath);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_index_add_frombuffer
     */
    public function testAddFrombuffer() {
        $index = static::getRepoIndex();
        $entry = [
            'path' => 'blob',
            'mode' => GIT_FILEMODE_BLOB,
        ];
        $buffer = 'Blob contents from a buffer';
        $result = git_index_add_frombuffer($index,$entry,$buffer);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_index_caps
     */
    public function testCaps() {
        $index = static::getRepoIndex();
        $result = git_index_caps($index);

        $this->assertIsInt($result);
    }

    /**
     * @phpGitTest git_index_checksum
     */
    public function testChecksum() {
        $index = static::getRepoIndex();
        $result = git_index_checksum($index);

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_index_clear
     */
    public function testClear() {
        $index = static::getRepoIndex();
        $result = git_index_clear($index);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_index_conflict_add
     */
    public function testConflictAdd() {
        $index = static::getRepoIndex();
        $ancestor = null;
        $ours = [
            'id' => '3dfab429769dc235b3a6b773a415b57aafa5cd56',
            'path' => 'bye.c',
            'mode' => GIT_FILEMODE_BLOB,
        ];
        $theirs = [
            'id' => '220ac4aec4d93fc4ba56acdfa72a7faaf69c5686',
            'path' => 'bye.c',
            'mode' => GIT_FILEMODE_BLOB,
        ];
        static::setStage($ours,2);
        static::setStage($theirs,3);
        $result = git_index_conflict_add($index,$ancestor,$ours,$theirs);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_index_conflict_get
     */
    public function testConflictGet() {
        static::addConflict();

        $index = static::getRepoIndex();
        $path = 'bye.c';
        $result = git_index_conflict_get($ancestor,$ours,$theirs,$index,$path);

        $this->assertNull($ancestor);
        $this->assertIsArray($ours);
        $this->assertIsArray($theirs);
        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_index_conflict_iterator_new
     */
    public function testConflictIteratorNew() {
        static::addConflict();

        $index = static::getRepoIndex();
        $result = git_index_conflict_iterator_new($index);

        $this->assertResourceHasType($result,'git_index_conflict_iterator');

        return $result;
    }

    /**
     * @depends testConflictIteratorNew
     * @phpGitTest git_index_conflict_next
     */
    public function testConflictNext($iterator) {
        $result = git_index_conflict_next($ancestor,$ours,$theirs,$iterator);

        $this->assertNull($ancestor);
        $this->assertIsArray($ours);
        $this->assertIsArray($theirs);
        $this->assertIsBool($result);

        return $iterator;
    }

    /**
     * @depends testConflictNext
     * @phpGitTest git_index_conflict_iterator_free
     */
    public function testConflictIteratorFree($iterator) {
        $result = git_index_conflict_iterator_free($iterator);

        $this->assertNull($result);
        $this->assertResourceHasType($iterator,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_index_conflict_iterator[A-Za-z0-9_]* resource/'
        );

        git_index_conflict_next($a,$b,$c,$iterator);
    }

    /**
     * @phpGitTest git_index_conflict_remove
     */
    public function testConflictRemove() {
        static::addConflict();

        $index = static::getRepoIndex();
        $path = 'bye.c';
        $result = git_index_conflict_remove($index,$path);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_index_conflict_cleanup
     */
    public function testConflictCleanup() {
        $index = static::getRepoIndex();
        $result = git_index_conflict_cleanup($index);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_index_entrycount
     */
    public function testEntrycount() {
        $index = static::getRepoIndex();
        $result = git_index_entrycount($index);

        $this->assertIsInt($result);
    }

    /**
     * @phpGitTest git_index_find
     */
    public function testFind() {
        $index = static::getRepoIndex();
        $path = 'find.entry';
        static::addEntry($path);
        $result = git_index_find($index,$path);

        $this->assertIsInt($result);
    }

    /**
     * @phpGitTest git_index_find
     */
    public function testFind_ENOTFOUND() {
        $this->expectException(\Exception::class);
        $this->expectExceptionCode(GIT_ENOTFOUND);

        $index = static::getRepoIndex();
        $path = 'find.entry.doesnotexist';
        $result = git_index_find($index,$path);
    }

    /**
     * @phpGitTest git_index_find_prefix
     */
    public function testFindPrefix() {
        $index = static::getRepoIndex();
        $path = 'find.prefix.entry';
        static::addEntry($path);
        $prefix = 'find.';
        $result = git_index_find_prefix($index,$prefix);

        $this->assertIsInt($result);
    }

    /**
     * @phpGitTest git_index_get_byindex
     */
    public function testGetByindex() {
        static::addEntry('get.byindex.entry');

        $index = static::getRepoIndex();
        $n = 0;
        $result = git_index_get_byindex($index,$n);

        $this->assertIsArray($result);
    }

    /**
     * @phpGitTest git_index_get_byindex
     */
    public function testGetByindex_False() {
        $index = static::getRepoIndex();
        $n = 999999;
        $result = git_index_get_byindex($index,$n);

        $this->assertFalse($result);
    }

    /**
     * @phpGitTest git_index_get_bypath
     */
    public function testGetByPath() {
        $index = static::getRepoIndex();
        $path = 'get.bypath.entry';
        static::addEntry($path);
        $stage = 0;
        $result = git_index_get_bypath($index,$path,$stage);

        $this->assertIsArray($result);

        return $result;
    }

    /**
     * @depends testGetByPath
     * @phpGitTest git_index_entry_is_conflict
     */
    public function testEntry($entry) {
        $result = git_index_entry_is_conflict($entry);

        $this->assertIsBool($result);
    }

    /**
     * @depends testGetByPath
     * @phpGitTest git_index_entry_stage
     */
    public function testEntryStage($entry) {
        $result = git_index_entry_stage($entry);

        $this->assertIsInt($result);
    }

    /**
     * @phpGitTest git_index_get_bypath
     */
    public function testGetByPath_False() {
        $index = static::getRepoIndex();
        $path = 'get.bypath.entry.doesnotexist';
        $stage = 0;
        $result = git_index_get_bypath($index,$path,$stage);

        $this->assertFalse($result);
    }

    /**
     * @phpGitTest git_index_has_conflicts
     */
    public function testHasConflicts() {
        $index = static::getRepoIndex();
        $result = git_index_has_conflicts($index);

        $this->assertIsBool($result);
    }

    /**
     * @phpGitTest git_index_owner
     */
    public function testOwner() {
        $index = static::getRepoIndex();
        $result = git_index_owner($index);

        $this->assertResourceHasType($result,'git_repository');
    }

    /**
     * @phpGitTest git_index_path
     */
    public function testPath() {
        $index = static::getRepoIndex();
        $result = git_index_path($index);

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_index_read
     */
    public function testRead() {
        $index = static::getRepoIndex();
        $force = false;
        $result = git_index_read($index,$force);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_index_remove
     */
    public function testRemove() {
        $index = static::getRepoIndex();
        $path = 'remove.entry';
        static::addEntry($path);
        $stage = 0;
        $result = git_index_remove($index,$path,$stage);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_index_remove_all
     */
    public function testRemoveAll() {
        static::addEntry('remove.all.1.entry');
        static::addEntry('remove.all.2.entry');
        static::addEntry('remove.all.3.entry');

        $n = 0;

        $index = static::getRepoIndex();
        $pathspec = ['remove.all.[0-9].entry'];
        $callback = function($path,$matchedPathspec,$payload) use(&$n) {
            $n += 1;
            $this->assertIsString($path);
            $this->assertIsString($matchedPathspec);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
            return 0;
        };
        $payload = new CallbackPayload;
        $result = git_index_remove_all($index,$pathspec,$callback,$payload);

        $this->assertGreaterThan(0,$n);
        $this->assertTrue($result);
    }

    /**
     * @phpGitTest git_index_remove_all
     */
    public function testRemoveAll_Abort() {
        static::addEntry('remove.all.abort.1.entry');
        static::addEntry('remove.all.abort.2.entry');
        static::addEntry('remove.all.abort.3.entry');

        $n = -1;

        $index = static::getRepoIndex();
        $pathspec = ['remove.all.abort.*'];
        $callback = function($path,$matchedPathspec,$payload) use(&$n) {
            $n += 1;
            $this->assertIsString($path);
            $this->assertIsString($matchedPathspec);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
            return -1;
        };
        $payload = new CallbackPayload;
        $result = git_index_remove_all($index,$pathspec,$callback,$payload);

        $this->assertEquals(0,$n);
        $this->assertFalse($result);
    }

    /**
     * @phpGitTest git_index_remove_bypath
     */
    public function testRemoveBypath() {
        $path = 'remove.bypath.entry';
        static::addEntry($path);

        $index = static::getRepoIndex();
        $result = git_index_remove_bypath($index,$path);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_index_remove_directory
     */
    public function testRemoveDirectory() {
        $index = static::getRepoIndex();
        $dir = 'index.remove.directory';
        static::makeDirectory('repo',$dir);
        static::addEntry("$dir/a.entry");
        static::addEntry("$dir/b.entry");
        $stage = 0;
        $result = git_index_remove_directory($index,$dir,$stage);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_index_update_all
     */
    public function testUpdateAll() {
        static::addEntry('hello.c');
        static::makeFile('void main() { hi(); }','repo','hello.c');

        $n = 0;

        $index = static::getRepoIndex();
        $pathspec = ['*.c'];
        $callback = function($path,$matchedPathspec,$payload) use(&$n) {
            $n += 1;
            $this->assertIsString($path);
            $this->assertIsString($matchedPathspec);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
            return 0;
        };
        $payload = new CallbackPayload;
        $result = git_index_update_all($index,$pathspec,$callback,$payload);

        $this->assertGreaterThan(0,$n);
        $this->assertTrue($result);
    }

    /**
     * @phpGitTest git_index_update_all
     */
    public function testUpdateAll_Abort() {
        static::addEntry('hello.m');
        static::makeFile('void main() { hi(); }','repo','hello.m');

        $n = -1;

        $index = static::getRepoIndex();
        $pathspec = ['*.m'];
        $callback = function($path,$matchedPathspec,$payload) use(&$n) {
            $n += 1;
            $this->assertIsString($path);
            $this->assertIsString($matchedPathspec);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
            return -1;
        };
        $payload = new CallbackPayload;
        $result = git_index_update_all($index,$pathspec,$callback,$payload);

        $this->assertEquals(0,$n);
        $this->assertFalse($result);
    }

    /**
     * @phpGitTest git_index_version
     */
    public function testVersion() {
        $index = static::getRepoIndex();
        $result = git_index_version($index);

        $this->assertIsInt($result);
    }

    /**
     * @phpGitTest git_index_write_tree
     */
    public function testWriteTree() {
        $index = static::getRepoIndex();
        $oid = git_index_write_tree($index);

        $this->assertIsString($oid);
    }

    /**
     * @phpGitTest git_index_write_tree_to
     */
    public function testWriteTreeTo() {
        $newRepoPath = static::makeDirectory('write-tree-to.repo');

        $index = static::getRepoIndex();
        $repo = git_repository_init($newRepoPath,false);
        $oid = git_index_write_tree_to($index,$repo);

        $this->assertIsString($oid);
    }

    /**
     * @phpGitTest git_index_new
     */
    public function testNew() {
        $result = git_index_new();

        $this->assertResourceHasType($result,'git_index');

        return $result;
    }

    /**
     * @depends testNew
     * @phpGitTest git_index_free
     */
    public function testFree($index) {
        $result = git_index_free($index);

        $this->assertNull($result);
        $this->assertResourceHasType($index,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_index[A-Za-z0-9_]* resource/'
        );

        git_index_version($index);
    }

    /**
     * @phpGitTest git_index_open
     */
    public function testOpen() {
        $indexPath = static::makePath('index.test');
        $result = git_index_open($indexPath);

        $this->assertResourceHasType($result,'git_index');

        return $result;
    }

    /**
     * @depends testOpen
     * @phpGitTest git_index_set_caps
     */
    public function testSetCaps($index) {
        $caps = GIT_INDEXCAP_IGNORE_CASE;
        $result = git_index_set_caps($index,$caps);

        $this->assertNull($result);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_index_set_version
     */
    public function testSetVersion($index) {
        $version = 3;
        $result = git_index_set_version($index,$version);

        $this->assertNull($result);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_index_read_tree
     */
    public function testReadTree($index) {
        $repo = static::getRepository();
        $tree = git_tree_lookup($repo,'9be15d459a1881c4c656de467fa45376b6886087');
        $result = git_index_read_tree($index,$tree);

        $this->assertNull($result);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_index_write
     */
    public function testWrite($index) {
        $result = git_index_write($index);

        $this->assertNull($result);
    }

    protected static function getRepoIndex() {
        static $index;

        if (!isset($index)) {
            $repo = static::getRepository();
            $index = git_repository_index($repo);
        }

        return $index;
    }

    protected static function setStage(&$entry,int $stage) : void {
        if (!isset($entry['flags'])) {
            $entry['flags'] = 0;
        }

        $entry['flags'] = ($entry['flags'] & ~GIT_IDXENTRY_STAGEMASK)
                        | (($stage & 0x03) << GIT_IDXENTRY_STAGESHIFT);
    }

    protected static function addEntry(string $localPath) : void {
        $index = static::getRepoIndex();
        static::makeFile("Contents of index entry '$localPath'",'repo',$localPath);
        git_index_add_bypath($index,$localPath);
    }

    protected static function addConflict() : void {
        $index = static::getRepoIndex();
        $ancestor = null;
        $ours = [
            'id' => '3dfab429769dc235b3a6b773a415b57aafa5cd56',
            'path' => 'bye.c',
            'mode' => GIT_FILEMODE_BLOB,
        ];
        $theirs = [
            'id' => '220ac4aec4d93fc4ba56acdfa72a7faaf69c5686',
            'path' => 'bye.c',
            'mode' => GIT_FILEMODE_BLOB,
        ];
        static::setStage($ours,2);
        static::setStage($theirs,3);
        git_index_conflict_add($index,$ancestor,$ours,$theirs);
    }
}
