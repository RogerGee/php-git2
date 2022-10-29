<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryTestCase;
use PhpGit2\Callback\CallbackException;
use PhpGit2\Callback\CallbackPayload;

/**
 * @phpGitRemoved git_checkout_init_options
 */
final class CheckoutTest extends RepositoryTestCase {
    /**
     * @phpGitTest git_checkout_head
     */
    public function testHead() {
        $repo = static::getRepository();
        $opts = null;
        $result = git_checkout_head($repo,$opts);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_checkout_index
     */
    public function testIndex() {
        $repo = static::getRepository();
        $index = git_repository_index($repo);
        $opts = null;
        $result = git_checkout_index($repo,$index,$opts);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_checkout_tree
     */
    public function testTree() {
        $n_calls = 0;

        $repo = static::getRepository();
        $treeish = git_object_lookup(
            $repo,
            'f6e24b93681d9e30d08b033c5364b7b816681c80',
            GIT_OBJ_COMMIT
        );
        $opts = [
            'progress_cb' => function(
                $path,
                $completedSteps,
                $totalSteps,
                $payload
                ) use(&$n_calls)
            {
                $n_calls += 1;
                $this->assertIsStringOrNull($path);
                $this->assertIsInt($completedSteps);
                $this->assertIsInt($totalSteps);
                $this->assertInstanceOf(CallbackPayload::class,$payload);
            },
            'progress_payload' => new CallbackPayload,
        ];
        $result = git_checkout_tree($repo,$treeish,$opts);

        $this->assertGreaterThan(0,$n_calls);
        $this->assertNull($result);
    }

    public function testProgressCallbackException() {
        $this->expectException(CallbackException::class);

        $repo = static::getRepository();
        $opts= [
            'progress_cb' => function(
                $path,
                $completedSteps,
                $totalSteps,
                $payload
                ) use(&$n_calls)
            {
                throw new CallbackException;
            },
            'progress_payload' => new CallbackPayload,
        ];
        git_checkout_head($repo,$opts);

        $this->assertTrue(false);
    }
}
