<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryTestCase;

/**
 * @phpGitRemoved git_cherrypick_init_options
 */
final class CherrypickTest extends RepositoryTestCase {
    /**
     * @phpGitTest git_cherrypick
     */
    public function testCherrypick() {
        $repo = static::getRepository();
        $commit = git_commit_lookup($repo,'7f5f4d7f51d18271093f0b30c86d038c7696fc5b');
        $options = [
            'mainline' => 0,
        ];
        $result = git_cherrypick($repo,$commit,$options);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_cherrypick_commit
     */
    public function testCommit() {
        $repo = static::getRepository();
        $cherrypickCommit = git_commit_lookup($repo,'7f5f4d7f51d18271093f0b30c86d038c7696fc5b');
        $ourCommit = git_commit_lookup($repo,'5019e8cd0ed3afed8afb6717776a388817a4a914');
        $mainline = 0;
        $mergeOptions = [];
        $result = git_cherrypick_commit($repo,$cherrypickCommit,$ourCommit,$mainline,$mergeOptions);

        $this->assertResourceHasType($result,'git_index');
    }
}
