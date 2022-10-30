<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryBareTestCase;

final class BranchTest extends RepositoryBareTestCase {
    /**
     * @phpGitTest git_branch_create
     */
    public function testCreate() {
        $repo = static::getRepository();
        $branchName = 'created-branch';
        $target = git_commit_lookup(
            $repo,
            '5019e8cd0ed3afed8afb6717776a388817a4a914'
        );
        $force = false;
        $result = git_branch_create($repo,$branchName,$target,$force);

        $this->assertResourceHasType($result,'git_reference');

        return $result;
    }

    /**
     * @phpGitTest git_branch_create_from_annotated
     */
    public function testCreateFromAnnotated() {
        $repo = static::getRepository();
        $branchName = 'created-branch-annotated';
        $commit = git_annotated_commit_from_ref(
            $repo,
            git_reference_lookup($repo,'refs/heads/i1')
        );
        $force = false;
        $result = git_branch_create_from_annotated($repo,$branchName,$commit,$force);

        $this->assertResourceHasType($result,'git_reference');

        return $result;
    }

    /**
     * @depends testCreateFromAnnotated
     * @phpGitTest git_branch_delete
     */
    public function testDelete($branch) {
        $result = git_branch_delete($branch);

        $this->assertNull($result);
    }

    /**
     * @depends testCreate
     * @phpGitTest git_branch_is_head
     */
    public function testIsHead($branch) {
        $result = git_branch_is_head($branch);

        $this->assertIsBool($result);
    }

    /**
     * @depends testNext
     * @phpGitTest git_branch_iterator_free
     */
    public function testIteratorFree($iter) {
        $result = git_branch_iterator_free($iter);

        $this->assertNull($result);
        $this->assertResourceHasType($iter,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_branch_iterator[A-Za-z0-9_]* resource/'
        );

        git_branch_next($_,$iter);
    }

    /**
     * @phpGitTest git_branch_iterator_new
     */
    public function testIteratorNew() {
        $repo = static::getRepository();
        $listFlags = GIT_BRANCH_ALL;
        $result = git_branch_iterator_new($repo,$listFlags);

        $this->assertResourceHasType($result,'git_branch_iterator');

        return $result;
    }

    /**
     * @phpGitTest git_branch_lookup
     */
    public function testLookup() {
        $repo = static::getRepository();
        $branchName = 'master';
        $branchType = GIT_BRANCH_LOCAL;
        $result = git_branch_lookup($repo,$branchName,$branchType);

        $this->assertResourceHasType($result,'git_reference');

        return $result;
    }

    /**
     * @depends testCreate
     * @phpGitTest git_branch_move
     */
    public function testMove($branch) {
        $newBranchName = 'moved-branch';
        $force = true;
        $result = git_branch_move($branch,$newBranchName,$force);

        $this->assertResourceHasType($result,'git_reference');
    }

    /**
     * @depends testCreate
     * @phpGitTest git_branch_name
     */
    public function testName($branch) {
        $result = git_branch_name($branch);

        $this->assertIsString($result);
    }

    /**
     * @depends testIteratorNew
     * @phpGitTest git_branch_next
     */
    public function testNext($iter) {
        $result = git_branch_next($outType,$iter);

        $this->assertIsInt($outType);
        $this->assertResourceHasType($result,'git_reference');

        return $iter;
    }

    /**
     * @depends testCreate
     * @phpGitTest git_branch_set_upstream
     */
    public function testSetUpstream($branch) {
        $upstreamName = 'master';
        $result = git_branch_set_upstream($branch,$upstreamName);

        $this->assertNull($result);

        return $branch;
    }

    /**
     * @depends testSetUpstream
     * @phpGitTest git_branch_upstream
     */
    public function testUpstream($branch) {
        $result = git_branch_upstream($branch);

        $this->assertResourceHasType($result,'git_reference');
    }

    /**
     * @depends testLookup
     * @phpGitTest git_branch_upstream
     */
    public function testUpstream_NotFound($branch) {
        $result = git_branch_upstream($branch);

        $this->assertFalse($result);
    }
}
