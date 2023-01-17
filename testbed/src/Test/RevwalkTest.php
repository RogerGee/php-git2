<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryBareTestCase;
use PhpGit2\Callback\CallbackPayload;

final class RevwalkTest extends RepositoryBareTestCase {
    /**
     * @phpGitTest git_revwalk_add_hide_cb
     */
    public function testAddHideCb() {
        $n_calls = 0;

        $revwalk = git_revwalk_new(static::getRepository());
        $hideCb = function($commit,$payload) use(&$n_calls) {
            $n_calls += 1;
            $this->assertIsString($commit);
            $this->assertInstanceOf(CallbackPayload::class,$payload);

            return ( $commit == 'f6e24b93681d9e30d08b033c5364b7b816681c80' );
        };
        $payload = new CallbackPayload;
        $result = git_revwalk_add_hide_cb($revwalk,$hideCb,$payload);

        $this->assertNull($result);

        git_revwalk_push($revwalk,'64db48af90133136eda7414dfd79783a513287a9');
        git_revwalk_next($revwalk);

        $this->assertGreaterThan(0,$n_calls);
    }

    /**
     * @phpGitTest git_revwalk_hide
     */
    public function testHide() {
        $revwalk = git_revwalk_new(static::getRepository());
        $commitId = 'f6e24b93681d9e30d08b033c5364b7b816681c80';
        $result = git_revwalk_hide($revwalk,$commitId);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_revwalk_hide_glob
     */
    public function testHideGlob() {
        $revwalk = git_revwalk_new(static::getRepository());
        $glob = 'b*';
        $result = git_revwalk_hide_glob($revwalk,$glob);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_revwalk_hide_head
     */
    public function testHideHead() {
        $revwalk = git_revwalk_new(static::getRepository());
        $result = git_revwalk_hide_head($revwalk);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_revwalk_hide_ref
     */
    public function testHideRef() {
        $revwalk = git_revwalk_new(static::getRepository());
        $refname = 'refs/heads/i1';
        $result = git_revwalk_hide_ref($revwalk,$refname);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_revwalk_free
     */
    public function testFree() {
        $revwalk = git_revwalk_new(static::getRepository());

        $result = git_revwalk_free($revwalk);

        $this->assertNull($result);
        $this->assertResourceHasType($revwalk,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_revwalk[A-Za-z0-9_]* resource/'
        );

        git_revwalk_reset($revwalk);
    }

    /**
     * @phpGitTest git_revwalk_new
     */
    public function testNew() {
        $result = git_revwalk_new(static::getRepository());

        $this->assertResourceHasType($result,'git_revwalk');
    }

    /**
     * @phpGitTest git_revwalk_next
     */
    public function testNext() {
        $revwalk = git_revwalk_new(static::getRepository());
        git_revwalk_push($revwalk,'64db48af90133136eda7414dfd79783a513287a9');
        $result = git_revwalk_next($revwalk);

        $this->assertIsString($result);

        // Test iter over return value on new, empty revwalk.
        $revwalk = git_revwalk_new(static::getRepository());
        $result = git_revwalk_next($revwalk);

        $this->assertFalse($result);
    }

    /**
     * @phpGitTest git_revwalk_push
     */
    public function testPush() {
        $revwalk = git_revwalk_new(static::getRepository());
        $id = '64db48af90133136eda7414dfd79783a513287a9';
        $result = git_revwalk_push($revwalk,$id);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_revwalk_push_glob
     */
    public function testPushGlob() {
        $revwalk = git_revwalk_new(static::getRepository());
        $glob = 'refs/heads/i[0-9]';
        $result = git_revwalk_push_glob($revwalk,$glob);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_revwalk_push_head
     */
    public function testPushHead() {
        $revwalk = git_revwalk_new(static::getRepository());
        $result = git_revwalk_push_head($revwalk);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_revwalk_push_range
     */
    public function testPushRange() {
        $revwalk = git_revwalk_new(static::getRepository());
        $range = 'master..master~2';
        $result = git_revwalk_push_range($revwalk,$range);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_revwalk_push_ref
     */
    public function testPushRef() {
        $revwalk = git_revwalk_new(static::getRepository());
        $ref = 'refs/tags/t1';
        $result = git_revwalk_push_ref($revwalk,$ref);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_revwalk_push_ref
     */
    public function testPushRef_NotFound() {
        // For some reason, git_revwalk_push_ref no longer returns GIT_ENOTFOUND
        // but just GIT_ERROR.
        $this->expectException(\Git2Exception::class);
        //$this->expectExceptionCode(GIT_ENOTFOUND);

        $revwalk = git_revwalk_new(static::getRepository());
        $ref = 'refs/tags/idonotexist';
        $result = git_revwalk_push_ref($revwalk,$ref);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_revwalk_repository
     */
    public function testRepository() {
        $revwalk = git_revwalk_new(static::getRepository());
        $result = git_revwalk_repository($revwalk);

        $this->assertResourceHasType($result,'git_repository');
    }

    /**
     * @phpGitTest git_revwalk_reset
     */
    public function testReset() {
        $revwalk = git_revwalk_new(static::getRepository());
        $result = git_revwalk_reset($revwalk);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_revwalk_simplify_first_parent
     */
    public function testSimplifyFirstParent() {
        $revwalk = git_revwalk_new(static::getRepository());
        $result = git_revwalk_simplify_first_parent($revwalk);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_revwalk_sorting
     */
    public function testSorting() {
        $revwalk = git_revwalk_new(static::getRepository());
        $sortMode = GIT_SORT_TOPOLOGICAL;
        $result = git_revwalk_sorting($revwalk,$sortMode);

        $this->assertNull($result);
    }
}
