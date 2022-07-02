<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryTestCase;
use PhpGit2\Misc\CallbackPayload;
use PhpGit2\Misc\CallbackReturnValue;

/**
 * Performs tests on 'repository' instances for a non-bare, non-empty git
 * repository.
 */
final class RepositoryTest extends RepositoryTestCase {
    /**
     * @phpGitTest git_repository_open
     */
    public function testOpen() {
        $path = static::makePath('repo');
        $repo = git_repository_open($path);

        $this->assertResourceHasType($repo,'git_repository');

        return $repo;
    }

    /**
     * @phpGitTest git_repository_open_exit
     */
    public function testOpenExt() {
        $path = static::makePath('repo');
        $flags = GIT_REPOSITORY_OPEN_NO_SEARCH;
        $ceilingDirs = implode(GIT_PATH_LIST_SEPARATOR,['/']);
        $repo = git_repository_open_ext($path,$flags,$ceilingDirs);

        $this->assertResourceHasType($repo,'git_repository');

        git_repository_free($repo);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_config
     */
    public function testConfig($repo) {
        $config = git_repository_config($repo);

        $this->assertResourceHasType($config,'git_config');
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_config_snapshot
     */
    public function testConfigSnapshot($repo) {
        $config = git_repository_config_snapshot($repo);

        $this->assertResourceHasType($config,'git_config');
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_detach_head
     */
    public function testDetachHead($repo) {
        $result = git_repository_detach_head($repo);

        $this->assertIsInt($result);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_fetchhead_foreach
     */
    public function testFetchheadForeach($repo) {
        $callback = function($refName,$remoteUrl,$oid,$isMerge,$payload) {
            $this->assertIsString($refName);
            $this->assertIsString($remoteUrl);
            $this->assertIsString($oid);
            $this->assertIsBool($isMerge);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
            return new CallbackReturnValue;
        };
        $payload = new CallbackPayload;
        $result = git_repository_fetchhead_foreach($repo,$callback,$payload);

        $this->assertNull($result);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_get_namespace
     */
    public function testGetNamespace($repo) {
        $namespace = git_repository_get_namespace($repo);

        $this->assertIsStringOrIsNull($namespace);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_hashfile
     */
    public function testHashfile($repo) {
        $path = static::makePath('test.file');
        $file = fopen($path,'w');
        fwrite($file,"Test file contents");
        fclose($file);

        $hash = git_repository_hashfile($repo,$path,GIT_OBJ_BLOB,'');

        $this->assertIsString($hash);
        $this->assertEquals(strlen($hash),40);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_head
     */
    public function testHead($repo) {
        $head = git_repository_head($repo);

        $this->assertResourceHasType($head,'git_reference');
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_head_detached
     */
    public function testHeadDetached($repo) {
        $result = git_repository_head_detached($repo);

        $this->assertIsBool($result);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_head_unborn
     */
    public function testHeadUnborn($repo) {
        $result = git_repository_head_unborn($repo);

        $this->assertIsBool($result);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_ident
     */
    public function testIdent_Unset($repo) {
        $email = 0;
        $name = git_repository_ident($email,$repo);

        $this->assertIsStringOrIsNull($name);
        $this->assertIsStringOrIsNull($email);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_index
     */
    public function testIndex($repo) {
        $index = git_repository_index($repo);

        $this->assertResourceHasType($index,'git_index');
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_is_bare
     */
    public function testIsBare($repo) {
        $result = git_repository_is_bare($repo);

        $this->assertIsBool($result);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_is_empty
     */
    public function testIsEmpty($repo) {
        $result = git_repository_is_empty($repo);

        $this->assertIsBool($result);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_is_shallow
     */
    public function testIsShallow($repo) {
        $result = git_repository_is_shallow($repo);

        $this->assertIsBool($result);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_message
     */
    public function testMessage($repo) {
        $result = git_repository_message($repo);

        // $message is false when no merge
        $this->assertIsBool($result);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_odb
     */
    public function testOdb($repo) {
        $odb = git_repository_odb($repo);

        $this->assertResourceHasType($odb,'git_odb');
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_path
     */
    public function testPath($repo) {
        $expectedPath = static::makePath('repo','.git');
        $path = git_repository_path($repo);

        $this->assertIsString($path);
        $this->assertEquals(rtrim($path,DIRECTORY_SEPARATOR),$expectedPath);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_refdb
     */
    public function testRefDb($repo) {
        $refdb = git_repository_refdb($repo);

        $this->assertResourceHasType($refdb,'git_refdb');
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_set_head
     */
    public function testSetHead($repo) {
        $result = git_repository_set_head($repo,'refs/heads/goodbye');

        $this->assertNull($result);

        $this->expectException(\Exception::class);
        $this->expectExceptionMessage("The given reference name 'goodbye' is not valid");

        git_repository_set_head($repo,'goodbye');
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_set_head_detached
     */
    public function testSetHeadDetached($repo) {
        $commitish = '8e886830f83cf4e16ade0b8476d433e69d514f48';
        $result = git_repository_set_head_detached($repo,$commitish);

        $this->assertIsBool($result);
        $this->assertTrue($result);

        $badCommitish = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
        $result = git_repository_set_head_detached($repo,$badCommitish);

        $this->assertIsBool($result);
        $this->assertFalse($result);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_set_head_detached_from_annotated
     */
    public function testSetHeadDetachedFromAnnotated($repo) {
        $commitId = '8e886830f83cf4e16ade0b8476d433e69d514f48';
        $annotated = git_annotated_commit_lookup($repo,$commitId);
        $result = git_repository_set_head_detached_from_annotated($repo,$annotated);

        $this->assertIsBool($result);
        $this->assertTrue($result);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_set_ident
     */
    public function testSetIdent($repo) {
        $name = 'Test User';
        $email = 'test@example.com';
        $result = git_repository_set_ident($repo,$name,$email);

        $this->assertNull($result);

        return $repo;
    }

    /**
     * @depends testSetIdent
     * @phpGitTest git_repository_ident
     */
    public function testIdent($repo) {
        $rname = git_repository_ident($remail,$repo);

        $this->assertIsString($rname);
        $this->assertIsString($remail);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_set_namespace
     */
    public function testSetNamespace($repo) {
        $result = git_repository_set_namespace($repo,'test_ns');

        $this->assertNull($result);

        return $repo;
    }

    /**
     * @depends testSetNamespace
     * @phpGitTest git_repository_get_namespace
     */
    public function testGetNamespace_Set($repo) {
        $ns = git_repository_get_namespace($repo);

        $this->assertIsString($ns);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_set_workdir
     */
    public function testSetWorkdir($repo) {
        $workDir = static::makePath('workdir');
        mkdir($workDir);
        $result = git_repository_set_workdir($repo,$workDir,true);

        $this->assertNull($result);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_state
     */
    public function testState($repo) {
        $result = git_repository_state($repo);

        $this->assertIsInt($result);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_state_cleanup
     */
    public function testStateCleanup($repo) {
        $result = git_repository_state_cleanup($repo);

        $this->assertNull($result);
    }

    /**
     * @depends testOpen
     * @phpGitTest git_repository_workdir
     */
    public function testWorkdir($repo) {
        $result = git_repository_workdir($repo);

        $this->assertIsString($result);
    }
}
