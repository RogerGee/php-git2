<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryBareTestCase;

/**
 * @phpGitRemoved git_object__size
 */
final class ObjectTest extends RepositoryBareTestCase {
    /**
     * @phpGitTest git_object_lookup
     */
    public function testLookup() {
        $repo = static::getRepository();
        $id = '586818c5f169efa6f847f7e21dfd37859871b85f';
        $type = GIT_OBJ_BLOB;
        $result = git_object_lookup($repo,$id,$type);

        $this->assertResourceHasType($result,'git_object');

        return $result;
    }

    /**
     * @depends testLookup
     * @phpGitTest git_object_dup
     */
    public function testDup($object) {
        $source = $object;
        $result = git_object_dup($source);

        $this->assertResourceHasType($result,'git_object');
    }

    /**
     * @depends testLookup
     * @phpGitTest git_object_id
     */
    public function testId($object) {
        $result = git_object_id($object);

        $this->assertIsString($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_object_owner
     */
    public function testOwner($object) {
        $result = git_object_owner($object);

        $this->assertResourceHasType($result,'git_repository');
    }

    /**
     * @depends testLookup
     * @phpGitTest git_object_short_id
     */
    public function testShortId($object) {
        $result = git_object_short_id($object);

        $this->assertIsString($result);
    }

    /**
     * @depends testLookup
     * @phpGitTest git_object_type
     */
    public function testType($object) {
        $result = git_object_type($object);

        $this->assertIsInt($result);
    }

    /**
     * @phpGitTest git_object_lookup_bypath
     */
    public function testLookupBypath() {
        $repo = static::getRepository();
        $treeish = git_object_lookup(
            $repo,
            '64db48af90133136eda7414dfd79783a513287a9',
            GIT_OBJ_COMMIT
        );
        $path = 'bye.c';
        $type = GIT_OBJ_BLOB;
        $result = git_object_lookup_bypath($treeish,$path,$type);

        $this->assertResourceHasType($result,'git_object');
    }

    /**
     * @phpGitTest git_object_lookup_prefix
     */
    public function testLookupPrefix() {
        $repo = static::getRepository();
        $id = '586818c';
        $type = GIT_OBJ_BLOB;
        $result = git_object_lookup_prefix($repo,$id,$type);

        $this->assertResourceHasType($result,'git_object');
    }

    /**
     * @phpGitTest git_object_free
     */
    public function testFree() {
        $object = git_object_lookup(
            static::getRepository(),
            '82a21601e6135604b75a969a3c338ab827bc4d35',
            GIT_OBJ_BLOB
        );

        $result = git_object_free($object);

        $this->assertNull($result);
        $this->assertResourceHasType($object,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_object[A-Za-z0-9_]* resource/'
        );

        git_object_id($object);
    }

    /**
     * @phpGitTest git_object_peel
     */
    public function testPeel() {
        $repo = static::getRepository();
        $object = git_object_lookup(
            $repo,
            '64db48af90133136eda7414dfd79783a513287a9',
            GIT_OBJ_COMMIT
        );
        $targetType = GIT_OBJ_TREE;
        $result = git_object_peel($object,$targetType);

        $this->assertResourceHasType($result,'git_object');
    }

    /**
     * @phpGitTest git_object_string2type
     */
    public function testString2type() {
        $str = 'commit';
        $result = git_object_string2type($str);

        $this->assertIsInt($result);
    }

    /**
     * @phpGitTest git_object_type2string
     */
    public function testType2string() {
        $type = GIT_OBJ_COMMIT;
        $result = git_object_type2string($type);

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_object_typeisloose
     */
    public function testTypeisloose() {
        $type = GIT_OBJ_COMMIT;
        $result = git_object_typeisloose($type);

        $this->assertIsBool($result);
    }
}
