<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryMergingTestCase;
use PhpGit2\Misc\CallbackReturnValue;
use PhpGit2\Misc\CallbackPayload;

/**
 * Performs tests on 'git_repository' functionality for a repository that is
 * undergoing a merge.
 */
final class RepositoryMergingTest extends RepositoryMergingTestCase {
    /**
     * @phpGitTest git_repository_open
     */
    public function testOpen() {
        $path = static::makePath('repo-merging');
        $repo = git_repository_open($path);

        $this->assertResourceHasType($repo,'git_repository');

        return $repo;
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_mergehead_foreach
     */
    public function testMergeheadForeach($repo) {
        $callback = function($oid,$payload) {
            $this->assertIsString($oid);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
            return new CallbackReturnValue;
        };
        $payload = new CallbackPayload;
        $result = git_repository_mergehead_foreach($repo,$callback,$payload);

        $this->assertNull($result);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_message
     */
    public function testMessage($repo) {
        $result = git_repository_message($repo);

        $this->assertIsString($result);

        return $repo;
    }

    /**
     * @depends testMessage
     * @phpGitTest git_repository_message_remove
     */
    public function testMessageRemove($repo) {
        $result = git_repository_message_remove($repo);

        $this->assertNull($result);
    }
}
