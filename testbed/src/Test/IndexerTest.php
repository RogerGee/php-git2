<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryBareTestCase;
use PhpGit2\Callback\CallbackPayload;

final class IndexerTest extends RepositoryBareTestCase {
    /**
     * @phpGitTest git_indexer_new
     */
    public function testNew() {
        $path = static::makeDirectory('indexer1');
        $mode = 0600;
        $odb = null;
        $progressCb = function($stats,$payload) use(&$n_calls) {
            if ($n_calls == 0) {
                $this->assertIsArray($stats);
                $this->assertInstanceOf(CallbackPayload::class,$payload);
            }

            $n_calls += 1;
        };
        $payload = new CallbackPayload;
        $result = git_indexer_new($path,$mode,$odb,$progressCb,$payload);

        $this->assertResourceHasType($result,'git_indexer');

        return $result;
    }

    /**
     * @depends testNew
     * @phpGitTest git_indexer_append
     */
    public function testAppend($indexer) {
        $idx = $indexer;
        $data = "\x50\x41\x43\x4b\x00\x00\x00\x02\x00\x00\x00\x03\x96\x0a\x78"
               ."\x9c\x95\xcb\xc1\x0d\xc2\x30\x0c\x00\xc0\x7f\xa6\xf0\x02\x20"
               ."\x9b\x24\x6e\x23\x21\x04\x2f\xfe\x6c\x90\xd8\x4e\xa9\x44\xd5"
               ."\x2a\x4a\xf6\x87\x15\xb8\xff\xf5\x66\x06\x93\x70\x16\xd5\x10"
               ."\x65\x2e\x5c\x13\x79\x29\x9e\xaa\xd7\x12\x2a\xfa\x64\x93\xc7"
               ."\x3a\x2b\xb2\xba\x3c\xfa\x7b\x6f\xf0\xda\x17\x6b\xf0\xfc\xcd"
               ."\x6b\x3b\x16\xa2\x7c\xcf\x32\xce\xa6\xe3\x06\xc4\xec\x39\xc4"
               ."\x0b\x25\x38\x61\x44\x74\xb2\x6f\xdb\xda\xbb\xfd\xb7\xdc\x43"
               ."\x15\x34\x77\x83\xba\x7e\xcc\x7d\x01\x4b\xcd\x30\xa5\xb0\x02"
               ."\x78\x9c\xf3\xcd\xcf\x53\x08\x4e\x2d\x50\x30\xb4\x54\x30\x34"
               ."\xb0\x32\x31\xb0\x32\x34\x50\x08\xf0\x55\x70\x76\x09\x51\x30"
               ."\x32\x30\x32\xe2\x02\x00\x86\xa5\x07\x5f\xa0\x02\x78\x9c\x33"
               ."\x34\x30\x30\x33\x31\x51\x48\x49\x2c\x49\x65\x78\xb2\x67\x65"
               ."\x8a\x90\x70\xcf\xfc\xe4\xab\x41\xd7\x92\xea\xf6\x36\xaf\x33"
               ."\x90\xfe\x0c\x00\xba\xe7\x0d\x57\x93\xef\x3e\x25\x96\x13\xa1"
               ."\x7b\x52\xd5\x38\x11\x57\xe4\xaa\x24\x74\xa4\x8f\x9a";
        $result = git_indexer_append($idx,$data);

        $this->assertNull($result);

        return $indexer;
    }

    /**
     * @depends testAppend
     * @phpGitTest git_indexer_commit
     */
    public function testCommit($indexer) {
        $idx = $indexer;
        $result = git_indexer_commit($idx);

        $this->assertNull($result);

        return $indexer;
    }

    /**
     * @depends testCommit
     * @phpGitTest git_indexer_hash
     */
    public function testHash($indexer) {
        $idx = $indexer;
        $result = git_indexer_hash($idx);

        $this->assertIsString($result);

        return $indexer;
    }

    /**
     * @depends testHash
     * @phpGitTest git2_indexer_stats
     */
    public function testGit2Stats($indexer) {
        $result = git2_indexer_stats($indexer);

        $this->assertIsArray($result);

        return $indexer;
    }

    /**
     * @depends testGit2Stats
     * @phpGitTest git_indexer_free
     */
    public function testFree($indexer) {
        $idx = $indexer;
        $result = git_indexer_free($idx);

        $this->assertNull($result);
        $this->assertResourceHasType($idx,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_indexer[A-Za-z0-9_]* resource/'
        );

        git_indexer_hash($idx);
    }
}
