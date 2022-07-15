<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryTestCase;
use PhpGit2\Callback\CallbackPayload;

final class AttrTest extends RepositoryTestCase {
    /**
     * @phpGitTest git_attr_add_macro
     */
    public function testAddMacro() {
        $repo = static::getRepository();
        $name = 'binary';
        $values = '-diff -crlf';
        $result = git_attr_add_macro($repo,$name,$values);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_attr_cache_flush
     */
    public function testCacheFlush() {
        $repo = static::getRepository();
        $result = git_attr_cache_flush($repo);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_attr_foreach
     */
    public function testForeach() {
        $n = 0;

        $repo = static::getRepository();
        $flags = GIT_ATTR_CHECK_FILE_THEN_INDEX;
        $path = 'hello.c';
        $callback = function($name,$value,$payload) use(&$n) {
            $n += 1;
            $this->assertIsString($name);
            $this->assertIsString($value);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $payload = new CallbackPayload;
        $result = git_attr_foreach($repo,$flags,$path,$callback,$payload);

        $this->assertGreaterThan(0,$n);
        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_attr_get
     */
    public function testGet() {
        $repo = static::getRepository();
        $flags = GIT_ATTR_CHECK_FILE_THEN_INDEX;
        $path = 'hello.c';
        $name = 'diff';
        $result = git_attr_get($repo,$flags,$path,$name);

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_attr_value
     */
    public function testValue() {
        $attr = 'diff=cpp';
        $result = git_attr_value($attr);

        $this->assertIsInt($result);
    }
}
