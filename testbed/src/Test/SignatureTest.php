<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryTestCase;

final class SignatureTest extends RepositoryTestCase {
    /**
     * @phpGitTest git_signature_default
     */
    public function testDefault() {
        $repo = static::getRepository();
        $result = git_signature_default($repo);

        $this->assertResourceHasType($result,'git_signature');
    }

    /**
     * @phpGitTest git_signature_new
     */
    public function testNew() {
        $dt = new \DateTime;
        $tzone = $dt->getTimezone();
        assert($tzone !== false);

        $name = 'Example Person';
        $email = 'person@example.com';
        $time = $dt->getTimestamp();
        $offset = intdiv($tzone->getOffset($dt),60);
        $result = git_signature_new($name,$email,$time,$offset);

        $this->assertResourceHasType($result,'git_signature');

        return $result;
    }

    /**
     * @depends testNew
     * @phpGitTest git_signature_dup
     */
    public function testDup($signature) {
        $sig = $signature;
        $result = git_signature_dup($sig);

        $this->assertResourceHasType($result,'git_signature');

        return $result;
    }

    /**
     * @depends testDup
     * @phpGitTest git_signature_free
     */
    public function testFree($signature) {
        $result = git_signature_free($signature);

        $this->assertNull($result);
        $this->assertResourceHasType($signature,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_signature[A-Za-z0-9_]* resource/'
        );

        git_signature_dup($signature);
    }

    /**
     * @phpGitTest git_signature_from_buffer
     */
    public function testFromBuffer() {
        $buf = 'Test User <test@test.tt> 1461698487 +0200';
        $result = git_signature_from_buffer($buf);

        $this->assertResourceHasType($result,'git_signature');
    }

    /**
     * @phpGitTest git_signature_now
     */
    public function testNow() {
        $name = 'Example Person';
        $email = 'person@example.com';
        $result = git_signature_now($name,$email);

        $this->assertResourceHasType($result,'git_signature');
    }
}
