<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryBareTestCase;

/**
 * @phpGitRemoved git_remote_init_callbacks
 */
final class RemoteTest extends RepositoryBareTestCase {
    /**
     * @phpGitTest git_remote_create
     */
    public function testCreate() {
        $repo = static::getRepository();
        $name = 'fake-remote-1';
        $url = 'https://example.com/repos/alpha.git';
        $result = git_remote_create($repo,$name,$url);

        $this->assertResourceHasType($result,'git_remote');

        return $result;
    }

    /**
     * @depends testCreate
     * @phpGitTest git_remote_add_fetch
     */
    public function testAddFetch($_) {
        $repo = static::getRepository();
        $remote = 'fake-remote-1';
        $refspec = '+refs/heads/*:refs/remotes/fake-remote-1/*';
        $result = git_remote_add_fetch($repo,$remote,$refspec);

        $this->assertNull($result);
    }

    /**
     * @depends testCreate
     * @phpGitTest git_remote_add_push
     */
    public function testAddPush($_) {
        $repo = static::getRepository();
        $remote = 'fake-remote-1';
        $refspec = '+refs/heads/*:refs/remotes/fake-remote-1/*';
        $result = git_remote_add_push($repo,$remote,$refspec);

        $this->assertNull($result);
    }

    /**
     * @depends testCreate
     * @phpGitTest git_remote_pushurl
     * @phpGitTest git_remote_set_pushurl
     */
    public function testPushurl($remote) {
        // NOTE: The push URL only returns non-NULL if it has been explicitly
        // set. I guess the standard remote URL is used by default with the
        // option to customize for push via git_remote_set_pushurl.

        $result = git_remote_pushurl($remote);

        $this->assertNull($result);

        // Assign a push URL to test for when push URL is explicitly set.

        $repo = static::getRepository();
        $name = 'fake-remote-1';
        $url = 'https://example.com/repos/alpha.git';
        $result = git_remote_set_pushurl($repo,$name,$url);

        $this->assertNull($result);

        $remote2 = git_remote_lookup($repo,$name);
        $result = git_remote_pushurl($remote2);

        $this->assertIsString($result);
    }

    /**
     * @depends testCreate
     * @phpGitTest git_remote_url
     */
    public function testUrl($remote) {
        $result = git_remote_url($remote);

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_remote_create_anonymous
     */
    public function testCreateAnonymous() {
        $repo = static::getRepository();
        $url = 'https://example.com/repos/beta.git';
        $result = git_remote_create_anonymous($repo,$url);

        $this->assertResourceHasType($result,'git_remote');
    }

    /**
     * @phpGitTest git_remote_create_with_fetchspec
     */
    public function testCreateWithFetchspec() {
        $repo = static::getRepository();
        $name = 'fake-remote-2';
        $url = 'https://example.com/repos/gamma.git';
        $fetch = '+refs/heads/*:refs/remotes/fake-remote-2/*';
        $result = git_remote_create_with_fetchspec($repo,$name,$url,$fetch);

        $this->assertResourceHasType($result,'git_remote');

        return $result;
    }

    /**
     * @depends testCreateWithFetchspec
     * @phpGitTest git_remote_delete
     */
    public function testDelete($remote) {
        $repo = static::getRepository($remote);
        $name = 'fake-remote-2';
        $result = git_remote_delete($repo,$name);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_remote_free
     */
    public function testFree() {
        $remote = git_remote_create(
            static::getRepository(),
            'fake-remote-4',
            'https://example.com/repos/delta.git'
        );
        $result = git_remote_free($remote);

        $this->assertNull($result);
        $this->assertResourceHasType($remote,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_remote[A-Za-z0-9_]* resource/'
        );

        git_remote_connected($remote);
    }

    /**
     * @phpGitTest git_remote_is_valid_name
     */
    public function testIsValidName() {
        $name = 'fake-remote-3';
        $result = git_remote_is_valid_name($name);

        $this->assertIsBool($result);
    }

    /**
     * @phpGitTest git_remote_list
     */
    public function testList() {
        $repo = static::getRepository();
        $result = git_remote_list($repo);

        $this->assertIsArray($result);
        $this->assertIsString($result[0]);
    }

    /**
     * @phpGitTest git_remote_lookup
     */
    public function testLookup() {
        $repo = static::getRepository();

        // Add refspecs for future testing.
        git_remote_add_fetch($repo,'origin','+refs/heads/*:refs/remotes/origin/*');
        git_remote_add_push($repo,'origin','+refs/heads/master:refs/heads/origin/master');

        $name = 'origin';
        $result = git_remote_lookup($repo,$name);

        $this->assertResourceHasType($result,'git_remote');

        return $result;
    }

    /**
     * @phpGitTest git_remote_lookup
     */
    public function testLookup_NotFound() {
        $this->expectException(\Exception::class);
        $this->expectExceptionCode(GIT_ENOTFOUND);

        $repo = static::getRepository();
        $name = 'origin-does-not-exist';
        $result = git_remote_lookup($repo,$name);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_remote_autotag
     */
    public function testAutotag($remote) {
        $result = git_remote_autotag($remote);

        $this->assertIsInt($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_remote_connect
     */
    public function testConnect($remote) {
        $direction = GIT_DIRECTION_FETCH;
        $callbacks = null;
        $proxyOpts = null;
        $customHeaders = [];
        $result = git_remote_connect(
            $remote,
            $direction,
            $callbacks,
            $proxyOpts,
            $customHeaders
        );

        $this->assertNull($result);

        return $remote;
    }

    /**
     * @depends testConnect
     * @phpGitTest git_remote_default_branch
     */
    public function testDefaultBranch($remote) {
        $result = git_remote_default_branch($remote);

        $this->assertIsString($result);

        return $remote;
    }

    /**
     * @depends testDefaultBranch
     * @phpGitTest git_remote_ls
     */
    public function testLs($remote) {
        $result = git_remote_ls($remote);

        $this->assertIsArray($result);

        return $remote;
    }

    /**
     * @depends testLs
     * @phpGitTest git_remote_disconnect
     */
    public function testDisconnect($remote) {
        $result = git_remote_disconnect($remote);

        $this->assertNull($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_remote_connected
     */
    public function testConnected($remote) {
        $result = git_remote_connected($remote);

        $this->assertIsBool($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_remote_download
     */
    public function testDownload($remote) {
        $refspecs = null;
        $opts = null;
        $result = git_remote_download($remote,$refspecs,$opts);

        $this->assertNull($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_remote_dup
     */
    public function testDup($remote) {
        $result = git_remote_dup($remote);

        $this->assertResourceHasType($result,'git_remote');
    }

    /**
     * @depends testLookup
     * @phpGitTest git_remote_fetch
     */
    public function testFetch($remote) {
        $refspecs = [
            '+refs/heads/master:refs/remotes/origin/master',
        ];
        $opts = [
            'download_tags' => true,
        ];
        $reflogMessage = 'Fetch master from origin';
        $result = git_remote_fetch($remote,$refspecs,$opts,$reflogMessage);

        $this->assertNull($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_remote_fetch
     */
    public function testFetch_Default($remote) {
        $refspecs = null;
        $opts = null;
        $reflogMessage = null;
        $result = git_remote_fetch($remote,$refspecs,$opts,$reflogMessage);

        $this->assertNull($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_remote_get_fetch_refspecs
     */
    public function testGetFetchRefspecs($remote) {
        $result = git_remote_get_fetch_refspecs($remote);

        $this->assertIsArray($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_remote_get_push_refspecs
     */
    public function testGetPushRefspecs($remote) {
        $result = git_remote_get_push_refspecs($remote);

        $this->assertIsArray($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_remote_get_refspec
     */
    public function testGetRefspec($remote) {
        $n = 1;
        $result = git_remote_get_refspec($remote,$n);

        $this->assertResourceHasType($result,'git_refspec');
    }

    /**
     * @depends testLookup
     * @phpGitTest git_remote_get_refspec
     */
    public function testGetRefspec_OutOfRange($remote) {
        $n = 1000000;
        $result = git_remote_get_refspec($remote,$n);

        $this->assertFalse($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_remote_name
     */
    public function testName($remote) {
        $result = git_remote_name($remote);

        $this->assertIsString($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_remote_owner
     */
    public function testOwner($remote) {
        $result = git_remote_owner($remote);

        $this->assertResourceHasType($result,'git_repository');
    }

    /**
     * @depends testLookup
     * @phpGitTest git_remote_prune
     */
    public function testPrune($remote) {
        $callbacks = null;
        $result = git_remote_prune($remote,$callbacks);

        $this->assertNull($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_remote_prune_refs
     */
    public function testPruneRefs($remote) {
        $result = git_remote_prune_refs($remote);

        $this->assertIsBool($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_remote_refspec_count
     */
    public function testRefspecCount($remote) {
        $result = git_remote_refspec_count($remote);

        $this->assertIsInt($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_remote_stats
     */
    public function testStats($remote) {
        $result = git_remote_stats($remote);

        $this->assertIsArray($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_remote_stop
     */
    public function testStop($remote) {
        $result = git_remote_stop($remote);

        $this->assertNull($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_remote_update_tips
     */
    public function testUpdateTips($remote) {
        $callbacks = null;
        $updateFetchhead = true;
        $downloadTags = true;
        $reflogMessage = 'Update tips test';
        $result = git_remote_update_tips(
            $remote,
            $callbacks,
            $updateFetchhead,
            $downloadTags,
            $reflogMessage
        );

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_remote_push
     */
    public function testPush() {
        // Create a new bare repository to which to push.
        $repo = static::getRepository();
        $path = static::makeDirectory('remote-test-repo-1');
        git_repository_init($path,true);

        $remote = git_remote_create(
            $repo,
            'origin-test-1',
            $path
        );
        $refspecs = [];
        $opts = null;
        $result = git_remote_push($remote,$refspecs,$opts);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_remote_rename
     */
    public function testRename() {
        $repo = static::getRepository();
        $name = 'fake-remote-5';
        $url = 'https://example.com/repos/epsilon.git';
        git_remote_create($repo,$name,$url);

        $newName = 'fake-remote-6';
        $result = git_remote_rename($repo,$name,$newName);

        $this->assertIsArray($result);
    }

    /**
     * @phpGitTest git_remote_set_autotag
     */
    public function testSetAutotag() {
        $repo = static::getRepository();
        $remote = 'origin';
        $value = GIT_REMOTE_DOWNLOAD_TAGS_ALL;
        $result = git_remote_set_autotag($repo,$remote,$value);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_remote_set_url
     */
    public function testSetUrl() {
        $repo = static::getRepository();
        $name = 'fake-remote-7';
        $url = 'https://example.com/repos/zeta.git';
        git_remote_create($repo,$name,$url);

        $newUrl = 'https://example.com/repos/eta.git';
        $result = git_remote_set_url($repo,$name,$newUrl);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_remote_upload
     */
    public function testUpload() {
        // Create a new bare repository to which to push.
        $repo = static::getRepository();
        $path = static::makeDirectory('remote-test-repo-2');
        git_repository_init($path,true);

        $remote = git_remote_create(
            $repo,
            'origin-test-2',
            $path
        );
        $refspecs = [];
        $opts = null;
        $result = git_remote_upload($remote,$refspecs,$opts);

        $this->assertNull($result);
    }
}
