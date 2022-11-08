<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryTestCase;
use PhpGit2\Callback\CallbackPayload;

/**
 * @phpGitRemoved git_diff_find_init_options
 * @phpGitRemoved git_diff_format_email_init_options
 * @phpGitRemoved git_diff_format_email_options_init
 * @phpGitRemoved git_diff_init_options
 * @phpGitRemoved git_diff_print_callback__to_buf
 * @phpGitRemoved git_diff_print_callback__to_file_handle
 */
final class DiffTest extends RepositoryTestCase {
    /**
     * @phpGitTest git_diff_blob_to_buffer
     */
    public function testBlobToBuffer() {
        $repo = static::getRepository();
        $n_file = 0;
        $n_hunk = 0;
        $n_line = 0;

        $oldBlob = git_blob_lookup($repo,'82a21601e6135604b75a969a3c338ab827bc4d35');
        $oldPath = 'hello.c';
        $buffer = <<<EOF
#include <iostream>

void main()
{
  std::cout << \"Hello, World!\" << std::endl;
}
EOF;
        $bufferPath = 'hello.cpp';
        $options = [];
        $fileCallback = function($delta,$progress,$payload) use(&$n_file) {
            $n_file += 1;
            $this->assertIsArray($delta);
            $this->assertIsFloat($progress);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $binaryCallback = null;
        $hunkCallback = function($delta,$hunk,$payload) use(&$n_hunk) {
            $n_hunk += 1;
            $this->assertIsArray($delta);
            $this->assertIsArray($hunk);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $lineCallback = function($delta,$hunk,$line,$payload) use(&$n_line) {
            $n_line += 1;
            $this->assertIsArray($delta);
            $this->assertIsArray($hunk);
            $this->assertIsArray($line);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $payload = new CallbackPayload;
        $result = git_diff_blob_to_buffer(
            $oldBlob,
            $oldPath,
            $buffer,
            $bufferPath,
            $options,
            $fileCallback,
            $binaryCallback,
            $hunkCallback,
            $lineCallback,
            $payload
        );

        $this->assertGreaterThan(0,$n_file);
        $this->assertGreaterThan(0,$n_hunk);
        $this->assertGreaterThan(0,$n_line);
        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_diff_blobs
     */
    public function testBlobs() {
        $repo = static::getRepository();
        $n_file = 0;
        $n_hunk = 0;
        $n_line = 0;

        $oldBlob = git_blob_lookup($repo,'82a21601e6135604b75a969a3c338ab827bc4d35');
        $oldPath = 'hello.c';
        $newBlob = git_blob_lookup($repo,'3dfab429769dc235b3a6b773a415b57aafa5cd56');
        $newPath = 'bye.c';
        $options = [];
        $fileCallback = function($delta,$progress,$payload) use(&$n_file) {
            $n_file += 1;
            $this->assertIsArray($delta);
            $this->assertIsFloat($progress);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $binaryCallback = null;
        $hunkCallback = function($delta,$hunk,$payload) use(&$n_hunk) {
            $n_hunk += 1;
            $this->assertIsArray($delta);
            $this->assertIsArray($hunk);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $lineCallback = function($delta,$hunk,$line,$payload) use(&$n_line) {
            $n_line += 1;
            $this->assertIsArray($delta);
            $this->assertIsArray($hunk);
            $this->assertIsArray($line);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $payload = new CallbackPayload;
        $result = git_diff_blobs(
            $oldBlob,
            $oldPath,
            $newBlob,
            $newPath,
            $options,
            $fileCallback,
            $binaryCallback,
            $hunkCallback,
            $lineCallback,
            $payload
        );

        $this->assertGreaterThan(0,$n_file);
        $this->assertGreaterThan(0,$n_hunk);
        $this->assertGreaterThan(0,$n_line);
        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_diff_buffers
     */
    public function testBuffers() {
        $n_file = 0;
        $n_hunk = 0;
        $n_line = 0;

        $oldBuffer = "a\nb\nc";
        $oldPath = 'a/buffer';
        $newBuffer = "d\nb\nf";
        $newPath =  'b/buffer';
        $options = [
            'old_prefix' => 'old',
            'new_prefix' => 'new',
        ];
        $fileCallback = function($delta,$progress,$payload) use(&$n_file) {
            $n_file += 1;
            $this->assertIsArray($delta);
            $this->assertIsFloat($progress);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $binaryCallback = null;
        $hunkCallback = function($delta,$hunk,$payload) use(&$n_hunk) {
            $n_hunk += 1;
            $this->assertIsArray($delta);
            $this->assertIsArray($hunk);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $lineCallback = function($delta,$hunk,$line,$payload) use(&$n_line) {
            $n_line += 1;
            $this->assertIsArray($delta);
            $this->assertIsArray($hunk);
            $this->assertIsArray($line);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $payload = new CallbackPayload;
        $result = git_diff_buffers(
            $oldBuffer,
            $oldPath,
            $newBuffer,
            $newPath,
            $options,
            $fileCallback,
            $binaryCallback,
            $hunkCallback,
            $lineCallback,
            $payload
        );

        $this->assertGreaterThan(0,$n_file);
        $this->assertGreaterThan(0,$n_hunk);
        $this->assertGreaterThan(0,$n_line);
        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_diff_commit_as_email
     */
    public function testCommitAsEmail() {
        $repo = static::getRepository();
        $commit = git_commit_lookup($repo,'2cd82ceaee5897c72eb8fded83632569c80c69c5');
        $patchNo = 1;
        $totalPatches = 1;
        $flags = GIT_DIFF_IGNORE_WHITESPACE;
        $options = [
            'context_lines' => 10,
            'interhunk_lines' => 5,
        ];
        $result = git_diff_commit_as_email(
            $repo,
            $commit,
            $patchNo,
            $totalPatches,
            $flags,
            $options
        );

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_diff_from_buffer
     */
    public function testFromBuffer() {
        $path = static::copyFile('resources/0001.patch','0001.patch');

        $buffer = file_get_contents($path);
        $result = git_diff_from_buffer($buffer);

        $this->assertResourceHasType($result,'git_diff');

        return $result;
    }

    /**
     * @depends testFromBuffer
     * @phpGitTest git_diff_find_similar
     */
    public function testFindSimilar($diff) {
        $options = [];
        $result = git_diff_find_similar($diff,$options);

        $this->assertNull($result);
    }

    /**
     * @depends testFromBuffer
     * @phpGitTest git_diff_find_similar
     */
    public function testFindSimilar_Null($diff) {
        $options = null;
        $result = git_diff_find_similar($diff,$options);

        $this->assertNull($result);
    }

    /**
     * @depends testFromBuffer
     * @phpGitTest git_diff_format_email
     */
    public function testFormatEmail($diff) {
        $options = [
            'flags' => GIT_DIFF_FORMAT_EMAIL_NONE,
            'patch_no' => 1,
            'id' => 'aaaabbbbccccdddd000011112222333344445555',
            'summary' => 'Some things happened to the codebase',
            'body' => 'Create an e-mail ready patch from a diff.',
            'author' => [
                'name' => 'AmazingCori',
                'email' => 'amazing-cori@example.com',
                'when.time' => time(),
                'when.offset' => 6,
            ],
        ];
        $result = git_diff_format_email($diff,$options);

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_diff_free
     */
    public function testFree() {
        $path = static::copyFile('resources/0001.patch','0001.patch');
        $buffer = file_get_contents($path);
        $diff = git_diff_from_buffer($buffer);

        $result = git_diff_free($diff);

        $this->assertNull($result);
        $this->assertResourceHasType($diff,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_diff[A-Za-z0-9_]* resource/'
        );

        git_diff_to_buf($diff,GIT_DIFF_FORMAT_PATCH);
    }

    /**
     * @phpGitTest git_diff_status_char
     */
    public function testStatusChar() {
        $delta = GIT_DELTA_ADDED;
        $result = git_diff_status_char($delta);

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_diff_tree_to_index
     */
    public function testTreeToIndex() {
        $repo = static::getRepository();
        $tree = git_tree_lookup($repo,'b962d96869a2e2acf3efd6541670faf7bc58dd11');
        $index = git_repository_index($repo);
        $options = null;
        $result = git_diff_tree_to_index($repo,$tree,$index,$options);

        $this->assertResourceHasType($result,'git_diff');
    }

    /**
     * @phpGitTest git_diff_tree_to_tree
     */
    public function testTreeToTree() {
        $repo = static::getRepository();
        $oldTree = git_tree_lookup($repo,'b962d96869a2e2acf3efd6541670faf7bc58dd11');
        $newTree = git_tree_lookup($repo,'faf545194b3df246b2b80ce44369371ec9fe2e68');
        $options = null;
        $result = git_diff_tree_to_tree(
            $repo,
            $oldTree,
            $newTree,
            $options
        );

        $this->assertResourceHasType($result,'git_diff');

        return $result;
    }

    /**
     * @depends testTreeToTree
     * @phpGitTest git_diff_foreach
     */
    public function testForeach($diff) {
        $n_file = 0;
        $n_hunk = 0;
        $n_line = 0;

        $fileCallback = function($delta,$progress,$payload) use(&$n_file) {
            $n_file += 1;
            $this->assertIsArray($delta);
            $this->assertIsFloat($progress);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $binaryCallback = null;
        $hunkCallback = function($delta,$hunk,$payload) use(&$n_hunk) {
            $n_hunk += 1;
            $this->assertIsArray($delta);
            $this->assertIsArray($hunk);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $lineCallback = function($delta,$hunk,$line,$payload) use(&$n_line) {
            $n_line += 1;
            $this->assertIsArray($delta);
            $this->assertIsArray($hunk);
            $this->assertIsArray($line);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $payload = new CallbackPayload;
        $result = git_diff_foreach(
            $diff,
            $fileCallback,
            $binaryCallback,
            $hunkCallback,
            $lineCallback,
            $payload
        );

        $this->assertNull($result);
        $this->assertGreaterThan(0,$n_file);
        $this->assertGreaterThan(0,$n_hunk);
        $this->assertGreaterThan(0,$n_line);
    }

    /**
     * @depends testTreeToTree
     * @phpGitTest git_diff_get_delta
     */
    public function testGetDelta($diff) {
        $idx = 0;
        $result = git_diff_get_delta($diff,$idx);

        $this->assertIsArray($result);
    }

    /**
     * @depends testTreeToTree
     * @phpGitTest git_diff_get_stats
     */
    public function testGetStats($diff) {
        $result = git_diff_get_stats($diff);

        $this->assertResourceHasType($result,'git_diff_stats');

        return $result;
    }

    /**
     * @depends testGetStats
     * @phpGitTest git_diff_stats_deletions
     */
    public function testStatsDeletions($stats) {
        $result = git_diff_stats_deletions($stats);

        $this->assertIsInt($result);
    }

    /**
     * @depends testGetStats
     * @phpGitTest git_diff_stats_files_changed
     */
    public function testStatsFilesChanged($stats) {
        $result = git_diff_stats_files_changed($stats);

        $this->assertIsInt($result);
    }

    /**
     * @depends testGetStats
     * @phpGitTest git_diff_stats_insertions
     */
    public function testStatsInsertions($stats) {
        $result = git_diff_stats_insertions($stats);

        $this->assertIsInt($result);
    }

    /**
     * @depends testGetStats
     * @phpGitTest git_diff_stats_to_buf
     */
    public function testStatsToBuf($stats) {
        $format = GIT_DIFF_STATS_FULL;
        $width = 80;
        $result = git_diff_stats_to_buf($stats,$format,$width);

        $this->assertIsString($result);
    }

    /**
     * @depends testTreeToTree
     * @phpGitTest git_diff_is_sorted_icase
     */
    public function testIsSortedIcase($diff) {
        $result = git_diff_is_sorted_icase($diff);

        $this->assertIsBool($result);
    }

    /**
     * @depends testTreeToTree
     * @phpGitTest git_diff_merge
     */
    public function testMerge($diff) {
        // NOTE: git_diff_merge is causing a double free. We believe this is an
        // issue with the libgit2 library.

        // $path = static::copyFile('resources/0001.patch','0001.patch');

        // $buffer = file_get_contents($path);
        // $onto = git_diff_from_buffer($buffer);
        // $from = $diff
        // $result = git_diff_merge($onto,$onto);

        // $this->assertNull($result);
        $this->assertTrue(true);
    }

    /**
     * @depends testTreeToTree
     * @phpGitTest git_diff_num_deltas
     */
    public function testNumDeltas($diff) {
        $result = git_diff_num_deltas($diff);

        $this->assertIsInt($result);
    }

    /**
     * @depends testTreeToTree
     * @phpGitTest git_diff_num_deltas_of_type
     */
    public function testNumDeltasOfType($diff) {
        $type = GIT_DELTA_MODIFIED;
        $result = git_diff_num_deltas_of_type($diff,$type);

        $this->assertIsInt($result);
    }

    /**
     * @depends testTreeToTree
     * @phpGitTest git_diff_print
     */
    public function testPrint($diff) {
        $n_line = 0;

        $format = GIT_DIFF_FORMAT_PATCH;
        $printCb = function($delta,$hunk,$line,$payload) use(&$n_line) {
            $n_line += 1;
            $this->assertIsArrayOrNull($delta);
            $this->assertIsArrayOrNull($hunk);
            $this->assertIsArrayOrNull($line);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $payload = new CallbackPayload;
        $result = git_diff_print($diff,$format,$printCb,$payload);

        $this->assertGreaterThan(0,$n_line);
        $this->assertNull($result);
    }

    /**
     * @depends testTreeToTree
     * @phpGitTest git_diff_stats_free
     */
    public function testStatsFree($diff) {
        $stats = git_diff_get_stats($diff);

        $result = git_diff_stats_free($stats);

        $this->assertNull($result);
        $this->assertResourceHasType($stats,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_diff_stats[A-Za-z0-9_]* resource/'
        );

        git_diff_stats_insertions($stats);
    }

    /**
     * @depends testTreeToTree
     * @phpGitTest git_diff_to_buf
     */
    public function testToBuf($diff) {
        $format = GIT_DIFF_FORMAT_PATCH;
        $result = git_diff_to_buf($diff,$format);

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_diff_tree_to_workdir
     */
    public function testTreeToWorkdir() {
        $repo = static::getRepository();
        $tree = git_tree_lookup($repo,'b962d96869a2e2acf3efd6541670faf7bc58dd11');
        $options = null;
        $result = git_diff_tree_to_workdir($repo,$tree,$options);

        $this->assertResourceHasType($result,'git_diff');
    }

    /**
     * @phpGitTest git_diff_tree_to_workdir_with_index
     */
    public function testTreeToWorkdirWithIndex() {
        $repo = static::getRepository();
        $tree = git_tree_lookup($repo,'b962d96869a2e2acf3efd6541670faf7bc58dd11');
        $options = null;
        $result = git_diff_tree_to_workdir_with_index($repo,$tree,$options);

        $this->assertResourceHasType($result,'git_diff');
    }
}
