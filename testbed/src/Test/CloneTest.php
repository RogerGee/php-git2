<?php

namespace PhpGit2\Test;

use PhpGit2\TestCase;

/**
 * @phpGitRemoved git_clone_init_options
 * @phpGitRemoved git_clone_options_init
 */
final class CloneTest extends TestCase {
    /**
     * @phpGitTest git_clone
     */
    public function testClone() {
        $url = "./repos/general.git";
        $localPath = static::makePath('repo1');
        $opts = null;
        $result = git_clone($url,$localPath,$opts);

        $this->assertResourceHasType($result,'git_repository');
    }

    /**
     * @phpGitTest git_clone
     */
    public function testClone_WithOptions() {
        $url = "./repos/general.git";
        $localPath = static::makePath('repo2.git');
        $opts = [
            'bare' => true,
            'local' => GIT_CLONE_LOCAL,
        ];
        $result = git_clone($url,$localPath,$opts);

        $this->assertResourceHasType($result,'git_repository');
    }
}
