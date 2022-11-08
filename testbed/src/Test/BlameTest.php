<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryBareTestCase;

/**
 * @phpGitRemoved git_blame_init_options
 * @phpGitRemoved git_blame_options_init
 */
final class BlameTest extends RepositoryBareTestCase {
    /**
     * @phpGitTest git_blame_file
     */
    public function testFile() {
        $repo = static::getRepository();
        $path = 'hello.c';
        $options = null;
        $result = git_blame_file($repo,$path,$options);

        $this->assertResourceHasType($result,'git_blame');

        return $result;
    }

    /**
     * @depends testFile
     * @phpGitTest git_blame_buffer
     */
    public function testBuffer($blame) {
        $buffer =<<<EOF
/*
 * hello.c
 */

#include <stdlib.h>
#include <stdio.h>

int main(int argc,char* argv[])
{
    printf("Hello, World!\n");
    return 0;
}
EOF;
        $result = git_blame_buffer($blame,$buffer);

        $this->assertResourceHasType($result,'git_blame');

        return $result;
    }

    /**
     * @depends testBuffer
     * @phpGitTest git_blame_free
     */
    public function testFree($blame) {
        $result = git_blame_free($blame);

        $this->assertNull($result);
        $this->assertResourceHasType($blame,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_blame[A-Za-z0-9_]* resource/'
        );

        git_blame_get_hunk_count($blame);
    }

    /**
     * @depends testFile
     * @phpGitTest git_blame_get_hunk_byindex
     */
    public function testGetHunkByindex($blame) {
        $index = 0;
        $result = git_blame_get_hunk_byindex($blame,$index);

        $this->assertIsArray($result);
    }

    /**
     * @depends testFile
     * @phpGitTest git_blame_get_hunk_byline
     */
    public function testGetHunkByline($blame) {
        $lineno = 1;
        $result = git_blame_get_hunk_byline($blame,$lineno);

        $this->assertIsArray($result);
    }

    /**
     * @depends testFile
     * @phpGitTest git_blame_get_hunk_count
     */
    public function test($blame) {
        $result = git_blame_get_hunk_count($blame);

        $this->assertIsInt($result);
    }
}
