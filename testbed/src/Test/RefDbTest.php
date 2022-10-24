<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryBareTestCase;

final class RefDbTest extends RepositoryBareTestCase {
    /**
     * @phpGitTest git_refdb_backend_fs
     */
    public function testBackendFs() {
        $repo = static::getRepository();
        $result = git_refdb_backend_fs($repo);

        $this->assertInstanceOf(\GitRefDBBackend_Internal::class,$result);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_refdb_compress
     */
    public function testCompress($refdb) {
        $result = git_refdb_compress($refdb);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_refdb_free
     */
    public function testFree() {
        $repo = static::getRepository();
        $refdb = git_refdb_new($repo);

        $result = git_refdb_free($refdb);

        $this->assertNull($result);
        $this->assertResourceHasType($refdb,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_refdb[A-Za-z0-9_]* resource/'
        );

        git_refdb_compress($refdb);
    }

    /**
     * @phpGitTest git_refdb_new
     */
    public function testNew() {
        $repo = static::getRepository();
        $result = git_refdb_new($repo);

        $this->assertResourceHasType($result,'git_refdb');
    }

    /**
     * @phpGitTest git_refdb_open
     */
    public function testOpen() {
        $repo = static::getRepository();
        $result = git_refdb_open($repo);

        $this->assertResourceHasType($result,'git_refdb');

        return $result;
    }
}
