<?php

namespace PhpGit2;

class RepositoryMergingTestCase extends TestCase {
    public static function setUpBeforeClass() : void {
        parent::setUpBeforeClass();

        static::copyNonBareRepoToEnvironment(
            'repos/merging',
            'repo-merging'
        );
    }
}
