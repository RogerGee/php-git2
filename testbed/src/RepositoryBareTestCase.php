<?php

namespace PhpGit2;

class RepositoryBareTestCase extends TestCase {
    static $__repo;

    public static function setUpBeforeClass() : void {
        parent::setUpBeforeClass();

        $src = 'repos/general.git';
        $dst = static::makePath('repo.git');
        $basePath = static::makePath();

        static::copyRecursive($src,$dst,$basePath);

        self::$__repo = null;
    }

    protected static function getRepository() {
        if (!isset(self::$__repo)) {
            $path = static::makePath('repo.git');
            self::$__repo = git_repository_open_bare($path);
        }

        return self::$__repo;
    }
}
