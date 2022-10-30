<?php

namespace PhpGit2;

class RepositoryTestCase extends TestCase {
    public static function setUpBeforeClass() : void {
        parent::setUpBeforeClass();

        $opts = [
            'bare' => false,
            'local' => GIT_CLONE_LOCAL,
            'checkout_branch' => 'master'
        ];

        $path = static::makePath('repo');
        $repo = git_clone("repos/general.git",$path,$opts);
        git_repository_free($repo);
    }

    protected static function getRepository() {
        static $repo;
        if (!isset($repo)) {
            $path = static::makePath('repo');
            $repo = git_repository_open($path);
        }

        return $repo;
    }
}
