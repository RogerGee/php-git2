<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryTestCase;

final class BlobTest extends RepositoryTestCase {
    /**
     * @phpGitTest git_blob_create_frombuffer
     */
    public function testCreateFrombuffer() {
        $repo = static::getRepository();
        $buffer = 'New blob contents frombuffer';
        $result = git_blob_create_frombuffer($repo,$buffer);

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_blob_create_fromdisk
     */
    public function testCreateFromdisk() {
        $repo = static::getRepository();
        $path = static::makeFile(
            'New blob contents fromdisk',
            'blob-create-fromdisk'
        );
        $result = git_blob_create_fromdisk($repo,$path);

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_blob_create_fromstream
     */
    public function testCreateFromstream() {
        $repo = static::getRepository();
        $hintpath = 'file.fromstream.txt';
        $result = git_blob_create_fromstream($repo,$hintpath);

        $this->assertInstanceOf(\GitWritestream::class,$result);

        return $result;
    }

    /**
     * @depends testCreateFromstream
     * @phpGitTest git_blob_create_fromstream_commit
     */
    public function testCreateFromstreamCommit($stream) {
        $writeOut = 'New blob contents fromstream';
        $stream->write($writeOut);

        $result = git_blob_create_fromstream_commit($stream);

        $this->assertIsString($result);

        $repo = static::getRepository();
        $blob = git_blob_lookup($repo,$result);
        $contents = git_blob_rawcontent($blob);

        $this->assertIsString($contents);
        $this->assertEquals($writeOut,$contents);
    }

    /**
     * @phpGitTest git_blob_create_fromworkdir
     */
    public function testCreateFromworkdir() {
        $contents = 'New blob content fromworkdir';
        $fileName = 'file.fromworkdir.txt';
        $filePath = static::makeFile($contents,'repo',$fileName);

        $repo = static::getRepository();
        $relativePath = $fileName;
        $result = git_blob_create_fromworkdir($repo,$relativePath);

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_blob_lookup
     */
    public function testLookup() {
        $repo = static::getRepository();
        $id = '586818c5f169efa6f847f7e21dfd37859871b85f';
        $result = git_blob_lookup($repo,$id);

        $this->assertResourceHasType($result,'git_blob');

        return $result;
    }

    /**
     * @depends testLookup
     * @phpGitTest git_blob_dup
     */
    public function testDup($blob) {
        $result = git_blob_dup($blob);

        $this->assertResourceHasType($result,'git_blob');
    }

    /**
     * @depends testLookup
     * @phpGitTest git_blob_filtered_content
     */
    public function testFilteredContent($blob) {
        $asPath = 'hello.c';
        $checkForBinaryData = false;
        $result = git_blob_filtered_content($blob,$asPath,$checkForBinaryData);

        $this->assertIsString($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_blob_id
     */
    public function testId($blob) {
        $result = git_blob_id($blob);

        $this->assertIsString($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_blob_is_binary
     */
    public function testIsBinary($blob) {
        $result = git_blob_is_binary($blob);

        $this->assertIsBool($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_blob_owner
     */
    public function testOwner($blob) {
        $result = git_blob_owner($blob);

        $this->assertResourceHasType($result,'git_repository');
    }

    /**
     * @depends testLookup
     * @phpGitTest git_blob_rawcontent
     */
    public function testRawcontent($blob) {
        $result = git_blob_rawcontent($blob);

        $this->assertIsString($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_blob_rawsize
     */
    public function testRawsize($blob) {
        $result = git_blob_rawsize($blob);

        $this->assertIsInt($result);
    }

    /**
     * @phpGitTest git_blob_lookup_prefix
     */
    public function testLookupPrefix() {
        $repo = static::getRepository();
        $id = '586818c';
        $result = git_blob_lookup_prefix($repo,$id);

        $this->assertResourceHasType($result,'git_blob');

        return $result;
    }

    /**
     * @depends testLookupPrefix
     * @phpGitTest git_blob_free
     */
    public function testFree($blob) {
        $result = git_blob_free($blob);

        $this->assertNull($result);
        $this->assertResourceHasType($blob,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_blob[A-Za-z0-9_]* resource/'
        );

        git_blob_id($blob);
    }
}
