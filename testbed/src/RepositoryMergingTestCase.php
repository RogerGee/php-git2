<?php

namespace PhpGit2;

class RepositoryMergingTestCase extends TestCase {
    protected function setUp() : void {
        parent::setUp();

        static::copyNonBareRepoToEnvironment(
            'repos/merging',
            'repo-merging'
        );
    }

    // protected function tearDown() : void {
    //     parent::tearDown();
    // }
}
