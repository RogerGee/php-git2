<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryTestCase;

/**
 * @phpGitRemoved git_worktree_add_init_options
 * @phpGitRemoved git_worktree_add_options_init
 * @phpGitRemoved git_worktree_prune_init_options
 * @phpGitRemoved git_worktree_prune_options_init
 */
final class WorktreeTest extends RepositoryTestCase {
    /**
     * @phpGitTest git_worktree_add
     */
    public function testAdd() {
        $repo = static::getRepository();
        $name = 'worktree1';
        $path = static::makePath('w-1');
        $opts = null;
        $result = git_worktree_add($repo,$name,$path,$opts);

        $this->assertResourceHasType($result,'git_worktree');

        return $result;
    }

    /**
     * @phpGitTest git_worktree_add
     */
    public function testAdd_WithOptions() {
        $repo = static::getRepository();
        $name = 'worktree2';
        $path = static::makePath('w-2');
        $opts = [
            'lock' => false,
            'ref' => git_reference_lookup($repo,'refs/remotes/origin/goodbye'),
            'checkout_options' => [
                'disable_filters' => true,
            ],
        ];
        $result = git_worktree_add($repo,$name,$path,$opts);

        $this->assertResourceHasType($result,'git_worktree');

        return $result;
    }

    /**
     * @depends testAdd_WithOptions
     * @phpGitTest git_worktree_free
     */
    public function testFree($worktree) {
        $result = git_worktree_free($worktree);

        $this->assertNull($result);
        $this->assertResourceHasType($worktree,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_worktree[A-Za-z0-9_]* resource/'
        );

        git_worktree_path($worktree);
    }

    /**
     * @depends testAdd
     * @phpGitTest git_worktree_is_locked
     */
    public function testIsLocked($worktree) {
        $result = git_worktree_is_locked($reason,$worktree);

        $this->assertIsBool($result);
        $this->assertIsString($reason);
    }

    /**
     * @depends testAdd
     * @phpGitTest git_worktree_is_prunable
     */
    public function testIsPrunable($worktree) {
        $opts = null;
        $result = git_worktree_is_prunable($worktree,$opts);

        $this->assertIsBool($result);
    }

    /**
     * @depends testAdd
     * @phpGitTest git_worktree_is_prunable
     */
    public function testIsPrunable_WithOptions($worktree) {
        $opts = [
            'flags' => GIT_WORKTREE_PRUNE_VALID | GIT_WORKTREE_PRUNE_LOCKED,
        ];
        $result = git_worktree_is_prunable($worktree,$opts);

        $this->assertIsBool($result);
    }

    /**
     * @phpGitTest git_worktree_list
     */
    public function testList() {
        $repo = static::getRepository();
        $result = git_worktree_list($repo);

        $this->assertIsArray($result);
    }

    /**
     * @depends testAdd
     * @phpGitTest git_worktree_lock
     */
    public function testLock($worktree) {
        $reason = 'just cuz';
        $result = git_worktree_lock($worktree,$reason);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_worktree_lookup
     */
    public function testLookup() {
        $repo = static::getRepository();
        $name = 'worktree3';
        $path = static::makePath('w-3');
        $opts = null;
        git_worktree_add($repo,$name,$path,$opts);
        $result = git_worktree_lookup($repo,$name);

        $this->assertResourceHasType($result,'git_worktree');

        return $result;
    }

    /**
     * @phpGitTest git_worktree_lookup
     */
    public function testLookup_NotFound() {
        $this->expectException(\Git2Exception::class);
        //$this->expectExceptionCode(GIT_ENOTFOUND);

        $repo = static::getRepository();
        $name = 'worktree300';
        $result = git_worktree_lookup($repo,$name);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_worktree_name
     */
    public function testName($worktree) {
        $result = git_worktree_name($worktree);

        $this->assertIsString($result);
    }

    /**
     * @depends testAdd
     * @phpGitTest git_worktree_open_from_repository
     */
    public function testOpenFromRepository($worktree) {
        $repo = git_repository_open(git_worktree_path($worktree));
        $result = git_worktree_open_from_repository($repo);

        $this->assertResourceHasType($result,'git_worktree');
    }

    /**
     * @depends testLookup
     * @phpGitTest git_worktree_name
     */
    public function testPath($worktree) {
        $result = git_worktree_name($worktree);

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_worktree_prune
     */
    public function testPrune() {
        // Create worktree, then delete its directory on disk prior to pruning.
        $repo = static::getRepository();
        $name = 'worktree4';
        $path = static::makePath('w-4');
        $opts = null;
        $worktree = git_worktree_add($repo,$name,$path,$opts);
        static::rmdirRecursive($path,static::makePath());

        $opts = null;
        $result = git_worktree_prune($worktree,$opts);

        $this->assertNull($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_worktree_prune
     */
    public function testPrune_WithOptions($worktree) {
        $opts = [
            'flags' => GIT_WORKTREE_PRUNE_VALID | GIT_WORKTREE_PRUNE_LOCKED,
        ];
        $result = git_worktree_prune($worktree,$opts);

        $this->assertNull($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_worktree_unlock
     */
    public function testUnlock($worktree) {
        $result = git_worktree_unlock($worktree);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_worktree_validate
     */
    public function testValidate() {
        $repo = static::getRepository();
        $name = 'worktree5';
        $path = static::makePath('w-5');
        $opts = null;
        $worktree = git_worktree_add($repo,$name,$path,$opts);

        $result = git_worktree_validate($worktree);

        $this->assertNull($result);
    }
}
