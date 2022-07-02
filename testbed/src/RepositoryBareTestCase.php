<?php

namespace PhpGit2;

class RepositoryBareTestCase extends TestCase {
    public static function setUpBeforeClass() : void {
        parent::setUpBeforeClass();

        $opts = [
            'bare' => true,
            'local' => GIT_CLONE_LOCAL,
            'checkout_branch' => 'master'
        ];

        $path = static::makePath('repo.git');
        $repo = git_clone("repos/general.git",$path,$opts);
        git_repository_free($repo);
    }
}
