<?php

namespace PhpGit2\Test;

use Exception;
use PhpGit2\TestCase;

final class TraceTest extends TestCase {
    /**
     * @phpGitTest git_trace_set
     */
    public function testTrace() {
        $level = GIT_TRACE_ERROR;
        $callback = function($level,$msg) {
            // We aren't testing this callback yet...
        };

        try {
            $result = git_trace_set($level,$callback);
            $this->assertNull($result);

        } catch (Exception $ex) {
            $expectedMessage = 'This version of libgit2 was not built with tracing.';
            $this->assertStringContainsString($expectedMessage,$ex->getMessage());
        }
    }
}
