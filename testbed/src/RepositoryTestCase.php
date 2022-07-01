<?php

namespace PhpGit2;

class RepositoryTestCase extends TestCase {
    protected function setUp() : void {
        parent::setUp();

        $opts = [
            'bare' => false,
            'local' => GIT_CLONE_LOCAL,
            'checkout_branch' => 'master'
        ];

        $path = $this->makePath('repo');
        $repo = git_clone("repos/general.git",$path,$opts);
        git_repository_free($repo);
    }

    // protected function tearDown() : void {
    //     parent::tearDown();
    // }
}
