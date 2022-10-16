<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryBareTestCase;
use PhpGit2\Callback\CallbackPayload;

final class NoteTest extends RepositoryBareTestCase {
    /**
     * @phpGitTest git_note_create
     */
    public function testCreate() {
        $dt = new \DateTime;
        $tzone = $dt->getTimezone();
        assert($tzone !== false);

        $repo = static::getRepository();
        $notesRef = null;
        $author = git_signature_new(
            'Example Person',
            'person@example.com',
            $dt->getTimestamp(),
            intdiv($tzone->getOffset($dt),60)
        );
        $committer = $author;
        $oid = '220ac4aec4d93fc4ba56acdfa72a7faaf69c5686';
        $note = 'This is a really nice file.';
        $force = false;
        $result = git_note_create(
            $repo,
            $notesRef,
            $author,
            $committer,
            $oid,
            $note,
            $force);

        $this->assertIsString($result);

        return $result;
    }

    /**
     * @depends testCreate
     * @phpGitTest git_note_read
     */
    public function testRead($noteOid) {
        $repo = static::getRepository();
        $notesRef = null;
        $oid = '220ac4aec4d93fc4ba56acdfa72a7faaf69c5686';
        $result = git_note_read($repo,$notesRef,$oid);

        $this->assertResourceHasType($result,'git_note');

        return $result;
    }

    /**
     * @depends testRead
     * @phpGitTest git_note_author
     */
    public function testAuthor($note) {
        $result = git_note_author($note);

        $this->assertResourceHasType($result,'git_signature');
    }

    /**
     * @depends testRead
     * @phpGitTest git_note_committer
     */
    public function testCommitter($note) {
        $result = git_note_committer($note);

        $this->assertResourceHasType($result,'git_signature');
    }

    /**
     * @depends testRead
     * @phpGitTest git_note_id
     */
    public function testId($note) {
        $result = git_note_id($note);

        $this->assertIsString($result);
    }

    /**
     * @depends testRead
     * @phpGitTest git_note_message
     */
    public function testMessage($note) {
        $result = git_note_message($note);

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_note_foreach
     */
    public function testForeach() {
        $n = 0;

        $repo = static::getRepository();
        $notesRef = null;
        $notecb = function($blobId,$annontatedObjectId,$payload) use(&$n) {
            $n += 1;
            $this->assertIsString($blobId);
            $this->assertIsString($annontatedObjectId);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $payload = new CallbackPayload;
        $result = git_note_foreach($repo,$notesRef,$notecb,$payload);

        $this->assertGreaterThan(0,$n);
        $this->assertNull($result);
    }

    /**
     * @depends testCreate
     * @phpGitTest git_note_free
     */
    public function testFree($noteOid) {
        $repo = static::getRepository();
        $notesRef = null;
        $oid = '220ac4aec4d93fc4ba56acdfa72a7faaf69c5686';
        $note = git_note_read($repo,$notesRef,$oid);

        $result = git_note_free($note);

        $this->assertNull($result);
        $this->assertResourceHasType($note,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_note[A-Za-z0-9_]* resource/'
        );

        git_note_message($note);
    }

    /**
     * @depends testCreate
     * @phpGitTest git_note_iterator_new
     */
    public function testIteratorNew($noteOid) {
        $repo = static::getRepository();
        $notesRef = null;
        $result = git_note_iterator_new($repo,$notesRef);

        // NOTE: for some reason, the type ID for git_note_iterator contains
        // 'git_iterator' and not 'git_note_iterator'.
        $this->assertResourceHasType($result,'git_iterator');

        return $result;
    }

    /**
     * @depends testIteratorNew
     * @phpGitTest git_note_next
     */
    public function testNext($iterator) {
        $result = git_note_next($annotatedId,$iterator);

        $this->assertIsString($result);
        $this->assertIsString($annotatedId);
        $this->assertEquals($annotatedId,'220ac4aec4d93fc4ba56acdfa72a7faaf69c5686');

        return $iterator;
    }

    /**
     * @depends testNext
     * @phpGitTest git_note_iterator_free
     */
    public function testIteratorFree($iterator) {
        $result = git_note_iterator_free($iterator);

        $this->assertNull($result);
        $this->assertResourceHasType($iterator,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_iterator[A-Za-z0-9_]* resource/'
        );

        git_note_next($_,$iterator);
    }

    /**
     * @phpGitTest git_note_remove
     */
    public function testRemove() {
        $dt = new \DateTime;
        $tzone = $dt->getTimezone();
        assert($tzone !== false);

        $repo = static::getRepository();
        $notesRef = null;
        $author = git_signature_new(
            'Other Person',
            'other.person@example.com',
            $dt->getTimestamp(),
            intdiv($tzone->getOffset($dt),60)
        );
        $committer = $author;
        $oid = '82a21601e6135604b75a969a3c338ab827bc4d35';
        $note = 'This is a really nice file, too!';
        $force = false;
        $noteOid = git_note_create(
            $repo,
            $notesRef,
            $author,
            $committer,
            $oid,
            $note,
            $force
        );

        $result = git_note_remove(
            $repo,
            $notesRef,
            $author,
            $committer,
            $oid
        );

        $this->assertNull($result);
    }
}
