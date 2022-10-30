<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryTestCase;

final class AnnotatedTest extends RepositoryTestCase {
    /**
     * @phpGitTest git_annotated_commit_from_fetchhead
     */
    public function testFromFetchhead() {
        $repo = static::getRepository();
        $branchName = 'master';
        $remoteUrl = 'repos/general.git';
        $id = '5019e8cd0ed3afed8afb6717776a388817a4a914';
        $result = git_annotated_commit_from_fetchhead($repo,$branchName,$remoteUrl,$id);

        $this->assertResourceHasType($result,'git_annotated_commit');

        return $result;
    }

    /**
     * @depends testFromFetchhead
     * @phpGitTest git_annotated_commit_free
     */
    public function testFree($commit) {
        $result = git_annotated_commit_free($commit);

        $this->assertNull($result);
        $this->assertResourceHasType($commit,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_annotated_commit[A-Za-z0-9_]* resource/'
        );

        git_annotated_commit_id($commit);
    }

    /**
     * @phpGitTest git_annotated_commit_from_ref
     */
    public function testFromRef() {
        $repo = static::getRepository();
        $ref = git_reference_lookup($repo,'refs/remotes/origin/i1');
        $result = git_annotated_commit_from_ref($repo,$ref);

        $this->assertResourceHasType($result,'git_annotated_commit');
    }

    /**
     * @phpGitTest git_annotated_commit_from_revspec
     */
    public function testFromRevspec() {
        $repo = static::getRepository();
        $revspec = '5019e8c';
        $result = git_annotated_commit_from_revspec($repo,$revspec);

        $this->assertResourceHasType($result,'git_annotated_commit');
    }

    /**
     * @phpGitTest git_annotated_commit_lookup
     */
    public function testLookup() {
        $repo = static::getRepository();
        $id = '5019e8cd0ed3afed8afb6717776a388817a4a914';
        $result = git_annotated_commit_lookup($repo,$id);

        $this->assertResourceHasType($result,'git_annotated_commit');

        return $result;
    }

    /**
     * @depends testLookup
     * @phpGitTest git_annotated_commit_id
     */
    public function testId($commit) {
        $result = git_annotated_commit_id($commit);

        $this->assertIsString($result);
    }
}
