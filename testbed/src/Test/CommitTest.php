<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryBareTestCase;

final class CommitTest extends RepositoryBareTestCase {
    /**
     * @phpGitTest git_commit_create
     */
    public function testCreate() {
        $now = new \DateTime;

        $repo = static::getRepository();
        $updateRef = 'refs/test/commit';
        $author = git_signature_new(
            'compassionate_kirch',
            'compassionate_kirch@example.com',
            $now->getTimestamp(),
            $now->getOffset() / 60
        );
        $committer = git_signature_new(
            'festive_sanderson',
            'festive_sanderson@example.com',
            $now->getTimestamp(),
            $now->getOffset() / 60
        );
        $encoding = 'ascii';
        $message = "This is the summary line\n\nThis is the body paragraph";
        $tree = git_tree_lookup($repo,'b962d96869a2e2acf3efd6541670faf7bc58dd11');
        $parents = [];
        $result = git_commit_create(
            $repo,
            $updateRef,
            $author,
            $committer,
            $encoding,
            $message,
            $tree,
            $parents
        );

        $this->assertIsString($result);

        return git_commit_lookup($repo,$result);
    }

    /**
     * @depends testCreate
     * @phpGitTest git_commit_create_buffer
     */
    public function testCreateBuffer($commit) {
        $now = new \DateTime;

        $repo = static::getRepository();
        $author = git_signature_new(
            'mystifying_hermann',
            'mystifying_hermann@example.com',
            $now->getTimestamp(),
            $now->getOffset() / 60
        );
        $committer = git_signature_new(
            'cranky_faraday',
            'cranky_faraday@example.com',
            $now->getTimestamp(),
            $now->getOffset() / 60
        );
        $encoding = null;
        $message = 'Perform an additional commit';
        $tree = git_tree_lookup($repo,'1feebccba4162cdd7d73f59db70b3f5a9454c850');
        $parents = [$commit];
        $result = git_commit_create_buffer(
            $repo,
            $author,
            $committer,
            $encoding,
            $message,
            $tree,
            $parents
        );

        $this->assertIsString($result);

        return $result;
    }

    /**
     * @depends testCreateBuffer
     * @phpGitTest git_commit_create_with_signature
     */
    public function testCreateWithSignature($buffer) {
        $repo = static::getRepository();
        $signature = 'verybadexampleofasignature';
        $signatureField = null;
        $result = git_commit_create_with_signature(
            $repo,
            $buffer,
            $signature,
            $signatureField
        );

        $this->assertIsString($result);
    }

    /**
     * @depends testCreate
     * @phpGitTest git_commit_body
     */
    public function testBody($commit) {
        $result = git_commit_body($commit);

        $this->assertIsString($result);
    }

    /**
     * @depends testCreate
     * @phpGitTest git_commit_message_encoding
     */
    public function testMessageEncoding($commit) {
        $result = git_commit_message_encoding($commit);

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_commit_extract_signature
     */
    public function testExtractSignature() {
        $repo = static::getRepository();
        $commitId = '0ba6992f3e37531e49ceb230b77537a6439ae08a';
        $field = null;
        $result1 = git_commit_extract_signature($result2,$repo,$commitId,$field);

        $this->assertIsString($result1);
        $this->assertIsString($result2);
    }

    /**
     * @phpGitTest git_commit_extract_signature
     */
    public function testExtractSignature_ENOTFOUND() {
        $this->expectException(\Exception::class);
        $this->expectExceptionCode(GIT_ENOTFOUND);

        $repo = static::getRepository();
        $commitId = '2cd82ceaee5897c72eb8fded83632569c80c69c5';
        $field = null;
        $result1 = git_commit_extract_signature($result2,$repo,$commitId,$field);
    }

    /**
     * @phpGitTest git_commit_lookup
     */
    public function testLookup() {
        $repo = static::getRepository();
        $id = '2cd82ceaee5897c72eb8fded83632569c80c69c5';
        $result = git_commit_lookup($repo,$id);

        $this->assertResourceHasType($result,'git_commit');

        return $result;
    }

    /**
     * @depends testLookup
     * @phpGitTest git_commit_amend
     */
    public function testAmend($commit) {
        $updateRef = null;
        $author = null;
        $committer = null;
        $messageEncoding = null;
        $message = 'This is the message for the amended commit';
        $tree = null;
        $result = git_commit_amend(
            $commit,
            $updateRef,
            $author,
            $committer,
            $messageEncoding,
            $message,
            $tree
        );

        $this->assertIsString($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_commit_author
     */
    public function testAuthor($commit) {
        $result = git_commit_author($commit);

        $this->assertResourceHasType($result,'git_signature');
    }

    /**
     * @depends testLookup
     * @phpGitTest git_commit_body
     */
    public function testBody_Null($commit) {
        $result = git_commit_body($commit);

        $this->assertNull($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_commit_committer
     */
    public function testCommitter($commit) {
        $result = git_commit_committer($commit);

        $this->assertResourceHasType($result,'git_signature');
    }

    /**
     * @depends testLookup
     * @phpGitTest git_commit_dup
     */
    public function testDup($commit) {
        $result = git_commit_dup($commit);

        $this->assertResourceHasType($result,'git_commit');
    }

    /**
     * @depends testLookup
     * @phpGitTest git_commit_header_field
     */
    public function testHeaderField($commit) {
        $field = 'tree';
        $result = git_commit_header_field($commit,$field);

        $this->assertIsString($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_commit_header_field
     */
    public function testHeaderField_ENOTFOUND($commit) {
        $this->expectException(\Exception::class);
        $this->expectExceptionCode(GIT_ENOTFOUND);

        $field = 'badfield';
        $result = git_commit_header_field($commit,$field);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_commit_id
     */
    public function testId($commit) {
        $result = git_commit_id($commit);

        $this->assertIsString($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_commit_message
     */
    public function testMessage($commit) {
        $result = git_commit_message($commit);

        $this->assertIsString($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_commit_message_encoding
     */
    public function testMessageEncoding_Null($commit) {
        $result = git_commit_message_encoding($commit);

        $this->assertNull($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_commit_message_raw
     */
    public function testMessageRaw($commit) {
        $result = git_commit_message_raw($commit);

        $this->assertIsString($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_commit_nth_gen_ancestor
     */
    public function testNthGenAncestor($commit) {
        $n = 1;
        $result = git_commit_nth_gen_ancestor($commit,$n);

        $this->assertResourceHasType($result,'git_commit');
    }

    /**
     * @depends testLookup
     * @phpGitTest git_commit_owner
     */
    public function testOwner($commit) {
        $result = git_commit_owner($commit);

        $this->assertResourceHasType($result,'git_repository');
    }

    /**
     * @depends testLookup
     * @phpGitTest git_commit_parent
     */
    public function testParent($commit) {
        $n = 0;
        $result = git_commit_parent($commit,$n);

        $this->assertResourceHasType($result,'git_commit');
    }

    /**
     * @depends testLookup
     * @phpGitTest git_commit_parent_id
     */
    public function testParentId($commit) {
        $n = 0;
        $result = git_commit_parent_id($commit,$n);

        $this->assertIsString($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_commit_parentcount
     */
    public function testParentcount($commit) {
        $result = git_commit_parentcount($commit);

        $this->assertIsInt($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_commit_raw_header
     */
    public function testRawHeader($commit) {
        $result = git_commit_raw_header($commit);

        $this->assertIsString($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_commit_summary
     */
    public function testSummary($commit) {
        $result = git_commit_summary($commit);

        $this->assertIsString($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_commit_time
     */
    public function testTime($commit) {
        $result = git_commit_time($commit);

        $this->assertIsInt($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_commit_time_offset
     */
    public function testTimeOffset($commit) {
        $result = git_commit_time_offset($commit);

        $this->assertIsInt($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_commit_tree
     */
    public function testTree($commit) {
        $result = git_commit_tree($commit);

        $this->assertResourceHasType($result,'git_tree');
    }

    /**
     * @depends testLookup
     * @phpGitTest git_commit_tree_id
     */
    public function testTreeId($commit) {
        $result = git_commit_tree_id($commit);

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_commit_lookup_prefix
     */
    public function testLookupPrefix() {
        $repo = static::getRepository();
        $id = '2cd82ce';
        $result = git_commit_lookup_prefix($repo,$id);

        $this->assertResourceHasType($result,'git_commit');

        return $result;
    }

    /**
     * @depends testLookupPrefix
     * @phpGitTest git_commit_free
     */
    public function testFree($commit) {
        $result = git_commit_free($commit);

        $this->assertNull($result);
        $this->assertResourceHasType($commit,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_commit[A-Za-z0-9_]* resource/'
        );

        git_commit_id($commit);
    }
}
