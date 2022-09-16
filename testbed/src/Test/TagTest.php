<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryTestCase;
use PhpGit2\Callback\CallbackPayload;

final class TagTest extends RepositoryTestCase {
    /**
     * @phpGitTest git_tag_annotation_create
     */
    public function testAnnotationCreate() {
        $dt = new \DateTime;
        $tzone = $dt->getTimezone();
        assert($tzone !== false);

        $repo = static::getRepository();
        $tagName = 'annot1';
        $target = git_object_lookup(
            $repo,
            '64db48af90133136eda7414dfd79783a513287a9',
            GIT_OBJ_COMMIT
        );
        $tagger = git_signature_new(
            'Test McTester',
            'test@example.com',
            $dt->getTimestamp(),
            intdiv($tzone->getOffset($dt),60)
        );
        $message = 'Created annotated tag via git_tag_annotation_create';
        $result = git_tag_annotation_create(
            $repo,
            $tagName,
            $target,
            $tagger,
            $message
        );

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_tag_create
     */
    public function testCreate() {
        $dt = new \DateTime;
        $tzone = $dt->getTimezone();
        assert($tzone !== false);

        $repo = static::getRepository();
        $tagName = 'annot2';
        $target = git_object_lookup(
            $repo,
            '64db48af90133136eda7414dfd79783a513287a9',
            GIT_OBJ_COMMIT
        );
        $tagger = git_signature_new(
            'Test McTester',
            'test@example.com',
            $dt->getTimestamp(),
            intdiv($tzone->getOffset($dt),60)
        );
        $message = 'Created annotated tag via git_tag_annotation_create';
        $force = false;
        $result = git_tag_create(
            $repo,
            $tagName,
            $target,
            $tagger,
            $message,
            $force
        );

        $this->assertIsString($result);

        return $result;
    }

    /**
     * @phpGitTest git_tag_create_frombuffer
     */
    public function testCreateFrombuffer() {
        $repo = static::getRepository();
        $buffer =<<<EOF
object 64db48af90133136eda7414dfd79783a513287a9
type commit
tag asdf
tagger Roger Gee <rpg11a@acu.edu> 1663343228 -0500

This is a tag with a message.
EOF;
        $force = false;
        $result = git_tag_create_frombuffer($repo,$buffer,$force);

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_tag_create_lightweight
     */
    public function testCreateLightweight() {
        $repo = static::getRepository();
        $tagName = 'tag1';
        $target = git_object_lookup(
            $repo,
            '64db48af90133136eda7414dfd79783a513287a9',
            GIT_OBJ_COMMIT
        );
        $force = false;
        $result = git_tag_create_lightweight($repo,$tagName,$target,$force);

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_tag_delete
     */
    public function testDelete() {
        $repo = static::getRepository();
        $tagName = 'tagToDelete';
        $result = git_tag_delete($repo,$tagName);

        $this->assertNull($result);
    }

    /**
     * @depends testCreate
     * @phpGitTest git_tag_lookup
     */
    public function testLookup($oid) {
        $repo = static::getRepository();
        $id = $oid;
        $result = git_tag_lookup($repo,$id);

        $this->assertResourceHasType($result,'git_tag');

        return $result;
    }

    /**
     * @depends testCreate
     * @phpGitTest git_tag_lookup_prefix
     */
    public function testLookupPrefix($oid) {
        $repo = static::getRepository();
        $id = substr($oid,0,7);
        $result = git_tag_lookup_prefix($repo,$id);

        $this->assertResourceHasType($result,'git_tag');
    }

    /**
     * @depends testLookup
     * @phpGitTest git_tag_dup
     */
    public function testDup($tag) {
        $source = $tag;
        $result = git_tag_dup($source);

        $this->assertResourceHasType($result,'git_tag');

        return $result;
    }

    /**
     * @depends testDup
     * @phpGitTest git_tag_free
     */
    public function testFree($tag) {
        $result = git_tag_free($tag);

        $this->assertNull($result);
        $this->assertResourceHasType($tag,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_tag[A-Za-z0-9_]* resource/'
        );

        git_tag_id($tag);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_tag_id
     */
    public function testId($tag) {
        $result = git_tag_id($tag);

        $this->assertIsString($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_tag_message
     */
    public function testMessage($tag) {
        $result = git_tag_message($tag);

        $this->assertIsString($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_tag_name
     */
    public function testName($tag) {
        $result = git_tag_name($tag);

        $this->assertIsString($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_tag_owner
     */
    public function testOwner($tag) {
        $result = git_tag_owner($tag);

        $this->assertResourceHasType($result,'git_repository');
    }

    /**
     * @depends testLookup
     * @phpGitTest git_tag_peel
     */
    public function testPeel($tag) {
        $result = git_tag_peel($tag);

        $this->assertResourceHasType($result,'git_object');
    }

    /**
     * @depends testLookup
     * @phpGitTest git_tag_tagger
     */
    public function testTagger($tag) {
        $result = git_tag_tagger($tag);

        $this->assertResourceHasType($result,'git_signature');
    }

    /**
     * @depends testLookup
     * @phpGitTest git_tag_target
     */
    public function testTarget($tag) {
        $result = git_tag_target($tag);

        $this->assertResourceHasType($result,'git_object');
    }

    /**
     * @depends testLookup
     * @phpGitTest git_tag_target_id
     */
    public function testTargetId($tag) {
        $result = git_tag_target_id($tag);

        $this->assertIsString($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_tag_target_type
     */
    public function testTargetType($tag) {
        $result = git_tag_target_type($tag);

        $this->assertIsInt($result);
    }

    /**
     * @phpGitTest git_tag_foreach
     */
    public function testForeach() {
        $n_calls = 0;

        $repo = static::getRepository();
        $callback = function($name,$oid,$payload) use(&$n_calls) {
            $n_calls += 1;
            $this->assertIsString($name);
            $this->assertIsString($oid);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $payload = new CallbackPayload;
        $result = git_tag_foreach($repo,$callback,$payload);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_tag_list
     */
    public function testList() {
        $repo = static::getRepository();
        $result = git_tag_list($repo);

        $this->assertIsArray($result);
        $this->assertIsString($result[0]);
    }

    /**
     * @phpGitTest git_tag_list_match
     */
    public function testListMatch() {
        $pattern = 'annot[0-9]';
        $repo = static::getRepository();
        $result = git_tag_list_match($pattern,$repo);

        $this->assertIsArray($result);
        $this->assertIsString($result[0]);
    }
}
