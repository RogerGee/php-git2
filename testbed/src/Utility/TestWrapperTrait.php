<?php

namespace PhpGit2\Utility;

use PhpGit2\TestCase;

trait TestWrapperTrait {
    private $unit;
    private $called = [];

    public function __construct(TestCase $unit) {
        $this->unit = $unit;
    }

    public function wasCalled(string $methodName) : bool {
        return in_array($methodName,$this->called);
    }
}
