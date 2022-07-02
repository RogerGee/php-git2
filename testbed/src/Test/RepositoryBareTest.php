<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryBareTestCase;

/**
 * Performs tests on 'git_repository' functionality for a bare, non-empty git
 * repository.
 */
final class RepositoryBareTest extends RepositoryBareTestCase {
    /**
     * @phpGitTest git_repository_open_bare
     */
    public function testOpen() {
        $path = static::makePath('repo.git');
        $repo = git_repository_open_bare($path);

        $this->assertResourceHasType($repo,'git_repository');

        return $repo;
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_workdir
     */
    public function testWorkdir($repo) {
        $result = git_repository_workdir($repo);

        $this->assertNull($result);
    }
}
