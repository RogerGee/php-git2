<?php

namespace PhpGit2;

class RepositoryTestCase extends TestCase {
    static private $__repo;

    public static function setUpBeforeClass() : void {
        parent::setUpBeforeClass();

        $opts = [
            'bare' => false,
            'local' => GIT_CLONE_LOCAL,
            'checkout_branch' => 'master'
        ];

        $path = static::makePath('repo');
        git_clone("repos/general.git",$path,$opts);
        self::$__repo = null;
    }

    protected static function getRepository() {
        if (!isset(self::$__repo)) {
            $path = static::makePath('repo');
            self::$__repo = git_repository_open($path);
        }

        return self::$__repo;
    }
}
