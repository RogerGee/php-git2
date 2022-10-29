<?php

namespace PhpGit2\Test;

use PhpGit2\TestCase;

/**
 * Performs test on 'git_repository' functionality for creating new, empty
 * repositories or for functionality that does not rely on a repository.
 */
final class RepositoryEmptyTest extends TestCase {
    /**
     * @phpGitTest git_repository_init
     */
    public function testInit() {
        $path = static::makePath('repo');

        $this->assertThat($path,$this->logicalNot($this->directoryExists()));

        $repo = git_repository_init($path,false);

        $this->assertDirectoryExists($path);
        $this->assertResourceHasType($repo,'git_repository');

        return $repo;
    }

    /**
     * @phpGitTest git_repository_init
     */
    public function testInitBare() {
        $path = static::makePath('repo.git');

        $this->assertThat($path,$this->logicalNot($this->directoryExists()));

        $repo = git_repository_init($path,true);

        $this->assertDirectoryExists($path);
        $this->assertResourceHasType($repo,'git_repository');

        return $repo;
    }

    /**
     * @phpGitTest git_repository_init_ext
     */
    public function testInitExt() {
        $path = static::makePath('repo-with-opts');
        $workdirPath = static::makePath('repo-with-opts','workdir');
        $options = [
            'flags' => GIT_REPOSITORY_INIT_MKDIR | GIT_REPOSITORY_INIT_MKPATH,
            'workdir_path' => $workdirPath,
        ];
        $result = git_repository_init_ext($path,$options);

        $this->assertDirectoryExists($path);
        $this->assertDirectoryExists($workdirPath);
        $this->assertResourceHasType($result,'git_repository');
    }

    /**
     * @phpGitTest git_repository_new
     */
    public function testNew() {
        $repo = git_repository_new();

        $this->assertResourceHasType($repo,'git_repository');

        return $repo;
    }

    /**
     * @depends testInit
     * @phpGitTest git_repository_fetchhead_foreach
     */
    public function testFetchheadForeach($repo) {
        $this->expectException(\Exception::class);
        $this->expectExceptionMessageMatches('/Could not find .*FETCH_HEAD/');

        $callback = TestCase::$nop;
        $payload = null;
        $result = git_repository_fetchhead_foreach($repo,$callback,$payload);
    }

    /**
     * @phpGitTest git_repository_free
     */
    public function testFree() {
        $repo = git_repository_new();
        git_repository_free($repo);

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_repository[A-Za-z0-9_]* resource/'
        );

        $path = git_repository_path($repo);
    }

    /**
     * @depends testNew
     * @phpGitTest git_repository_head
     */
    public function testHead($repo) {
        $this->expectExceptionMessage(\Exception::class);
        $this->expectExceptionMessage("Reference 'HEAD' not found");
        $head = git_repository_head($repo);
    }

    /**
     * @depends testInit
     * @phpGitTest git_repository_is_empty
     */
    public function testIsEmpty($repo) {
        $result = git_repository_is_empty($repo);
        $this->assertIsBool($result);
        $this->assertTrue($result);
    }

    /**
     * @depends testInitBare
     * @phpGitTest git_repository_is_bare
     */
    public function testIsBare($repo) {
        $result = git_repository_is_bare($repo);
        $this->assertIsBool($result);
        $this->assertTrue($result);
    }
}
