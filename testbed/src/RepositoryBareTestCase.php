<?php

namespace PhpGit2;

class RepositoryBareTestCase extends TestCase {
    public static function setUpBeforeClass() : void {
        parent::setUpBeforeClass();

        $src = 'repos/general.git';
        $dst = static::makePath('repo.git');
        $basePath = static::makePath();

        static::copyRecursive($src,$dst,$basePath);
    }

    protected static function getRepository() {
        static $repo;
        if (!isset($repo)) {
            $path = static::makePath('repo.git');
            $repo = git_repository_open_bare($path);
        }

        return $repo;
    }
}
