<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryBareTestCase;
use PhpGit2\Misc\CallbackPayload;
use PhpGit2\Misc\CallbackReturnValue;

final class ReferenceTest extends RepositoryBareTestCase {
    /**
     * @phpGitTest git_reference_create
     */
    public function testCreate() {
        $repo = static::getRepository();
        $name = 'refs/test/gifted_goodall';
        $oid = '8e886830f83cf4e16ade0b8476d433e69d514f48';
        $force = true;
        $log = "We added the '$name' reference";
        $result = git_reference_create($repo,$name,$oid,$force,$log);

        $this->assertResourceHasType($result,'git_reference');

        return $result;
    }

    /**
     * @depends testCreate
     * @phpGitTest git_reference_has_log
     */
    public function testHasLog($ref) {
        $repo = static::getRepository();
        $result = git_reference_has_log($repo,'refs/test/gifted_goodall');

        $this->assertIsBool($result);
    }

    /**
     * @depends testCreate
     * @phpGitTest git_reference_create
     */
    public function testCreate_EEXISTS($existingRef) {
        $this->expectException(\Exception::class);
        $this->expectExceptionMessage('a reference with that name already exists');
        $this->expectExceptionCode(GIT_EEXISTS);

        $repo = static::getRepository();
        $name = 'refs/test/gifted_goodall';
        $oid = '8e886830f83cf4e16ade0b8476d433e69d514f48';
        $force = false;
        $log = "We added the '$name' reference";
        $result = git_reference_create($repo,$name,$oid,$force,$log);
    }

    /**
     * @depends testCreate
     * @phpGitTest git_reference_create_matching
     */
    public function testCreateMatching($existingRef) {
        $repo = static::getRepository();
        $name = 'refs/test/gifted_goodall';
        $oid = '8e886830f83cf4e16ade0b8476d433e69d514f48';
        $force = true;
        $oidExpected = '8e886830f83cf4e16ade0b8476d433e69d514f48';
        $log = "We updated the '$name' reference";
        $result = git_reference_create_matching($repo,$name,$oid,$force,$oidExpected,$log);

        $this->assertResourceHasType($result,'git_reference');

        return $result;
    }

    /**
     * @depends testCreateMatching
     * @phpGitTest git_reference_create_matching
     */
    public function testCreateMatching_EMODIFIED($existingRef) {
        $this->expectException(\Exception::class);
        $this->expectExceptionMessage('old reference value does not match');
        $this->expectExceptionCode(GIT_EMODIFIED);

        $repo = static::getRepository();
        $name = 'refs/test/gifted_goodall';
        $oid = '8e886830f83cf4e16ade0b8476d433e69d514f48';
        $force = true;
        $oidExpected = 'aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa';
        $log = "We updated the '$name' reference";
        $result = git_reference_create_matching($repo,$name,$oid,$force,$oidExpected,$log);
    }

    /**
     * @depends testCreate
     * @depends testCreateMatching
     * @phpGitTest git_reference_cmp
     */
    public function testCmp($ref1,$ref2) {
        $result = git_reference_cmp($ref1,$ref2);

        $this->assertIsBool($result);
    }

    /**
     * @depends testCreate
     * @phpGitTest git_reference_dup
     */
    public function testDup($ref) {
        $result = git_reference_dup($ref);

        $this->assertResourceHasType($result,'git_reference');
    }

    /**
     * @depends testCreate
     * @phpGitTest git_reference_dwim
     */
    public function testDwim($ref) {
        $repo = static::getRepository();
        $shorthand = 'test/gifted_goodall';
        $result = git_reference_dwim($repo,$shorthand);

        $this->assertResourceHasType($result,'git_reference');

        return $result;
    }

    /**
     * @depends testDwim
     * @phpGitTest git_reference_dwim
     */
    public function testDwim_Error($ref) {
        $this->expectException(\Exception::class);
        $this->expectExceptionMessage('no reference found for shorthand');

        $repo = static::getRepository();
        $shorthand = 'optimistic_noyce';
        $result = git_reference_dwim($repo,$shorthand);
    }

    /**
     * @depends testCreate
     * @phpGitTest git_reference_ensure_log
     */
    public function testEnsureLog($ref) {
        $repo = static::getRepository();
        $refname = 'refs/test/gifted_goodall';
        $result = git_reference_ensure_log($repo,$refname);

        $this->assertNull($result);
    }

    /**
     * @depends testCreate
     * @phpGitTest git_reference_is_branch
     */
    public function testIsBranch($ref) {
        $result = git_reference_is_branch($ref);

        $this->assertIsBool($result);
    }

    /**
     * @depends testCreate
     * @phpGitTest git_reference_is_note
     */
    public function testIsNote($ref) {
        $result = git_reference_is_note($ref);

        $this->assertIsBool($result);
    }

    /**
     * @depends testCreate
     * @phpGitTest git_reference_is_remote
     */
    public function testIsRemote($ref) {
        $result = git_reference_is_remote($ref);

        $this->assertIsBool($result);
    }

    /**
     * @depends testCreate
     * @phpGitTest git_reference_is_tag
     */
    public function testIsTag($ref) {
        $result = git_reference_is_tag($ref);

        $this->assertIsBool($result);
    }

    /**
     * @depends testCreate
     * @phpGitTest git_reference_name
     */
    public function testName($ref) {
        $name = git_reference_name($ref);

        $this->assertIsString($name);
    }

    /**
     * @depends testCreate
     * @phpGitTest git_reference_owner
     */
    public function testOwner($ref) {
        $result = git_reference_owner($ref);

        $this->assertResourceHasType($result,'git_repository');
    }

    /**
     * @depends testCreate
     * @phpGitTest git_reference_shorthand
     */
    public function testShorthand($ref) {
        $result = git_reference_shorthand($ref);

        $this->assertIsString($result);
    }

    /**
     * @depends testCreate
     * @phpGitTest git_reference_target
     */
    public function testTarget($ref) {
        $result = git_reference_target($ref);

        $this->assertIsString($result);
    }

    /**
     * @depends testCreate
     * @phpGitTest git_reference_type
     */
    public function testType($ref) {
        $result = git_reference_type($ref);

        $this->assertIsInt($result);
    }

    /**
     * @phpGitTest git_reference_delete
     */
    public function testDelete() {
        $repo = static::getRepository();
        $name = 'refs/test/hopeful_clarke';
        $oid = '8e886830f83cf4e16ade0b8476d433e69d514f48';
        $force = true;
        $log = "We added the '$name' reference";
        $ref = git_reference_create($repo,$name,$oid,$force,$log);

        $result = git_reference_delete($ref);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_reference_foreach
     */
    public function testForeach() {
        $n = 0;

        $repo = static::getRepository();
        $callback = function($ref,$payload) use(&$n) {
            $n += 1;
            $this->assertResourceHasType($ref,'git_reference');
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $payload = new CallbackPayload;
        $result = git_reference_foreach($repo,$callback,$payload);

        $this->assertGreaterThan(0,$n);
        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_reference_foreach_glob
     */
    public function testForeachGlob() {
        $n = 0;

        $repo = static::getRepository();
        $glob = 'refs/heads/i[0-9]';
        $callback = function($name,$payload) use(&$n) {
            $n += 1;
            $this->assertIsString($name);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $payload = new CallbackPayload;
        $result = git_reference_foreach_glob($repo,$glob,$callback,$payload);

        $this->assertGreaterThan(0,$n);
        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_reference_foreach_name
     */
    public function testForeachName() {
        $n = 0;

        $repo = static::getRepository();
        $callback = function($name,$payload) use(&$n) {
            $n += 1;
            $this->assertIsString($name);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $payload = new CallbackPayload;
        $result = git_reference_foreach_name($repo,$callback,$payload);

        $this->assertGreaterThan(0,$n);
        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_reference_free
     */
    public function testFree() {
        $repo = static::getRepository();
        $name = 'refs/test/flamboyant_feistel';
        $oid = '8e886830f83cf4e16ade0b8476d433e69d514f48';
        $force = true;
        $log = "We added the '$name' reference";
        $ref = git_reference_create($repo,$name,$oid,$force,$log);

        $this->assertResourceHasType($ref,'git_reference');

        $result = git_reference_free($ref);

        $this->assertNull($result);
        $this->assertResourceHasType($ref,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_reference[A-Za-z0-9_]* resource/'
        );

        git_reference_delete($ref);
    }

    /**
     * @phpGitTest git_reference_is_valid_name
     */
    public function testIsValidName() {
        $refname = '~tilde~';
        $result = git_reference_is_valid_name($refname);

        $this->assertIsBool($result);
    }

    /**
     * @phpGitTest git_reference_iterator_free
     */
    public function testIteratorFree() {
        $repo = static::getRepository();
        $iterator = git_reference_iterator_new($repo);

        $this->assertResourceHasType($iterator,'git_reference_iterator');

        $result = git_reference_iterator_free($iterator);

        $this->assertNull($result);
        $this->assertResourceHasType($iterator,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_reference_iterator[A-Za-z0-9_]* resource/'
        );

        $ref = git_reference_next($iterator);
    }

    /**
     * @phpGitTest git_reference_iterator_new
     */
    public function testIteratorNew() {
        $repo = static::getRepository();
        $result = git_reference_iterator_new($repo);

        $this->assertResourceHasType($result,'git_reference_iterator');

        return $result;
    }

    /**
     * @phpGitTest git_reference_iterator_glob_new
     */
    public function testIteratorGlobNew() {
        $repo = static::getRepository();
        $glob = 'refs/heads/i[0-9]';
        $result = git_reference_iterator_glob_new($repo,$glob);

        $this->assertResourceHasType($result,'git_reference_iterator');

        return $result;
    }

    /**
     * @depends testIteratorNew
     * @phpGitTest git_reference_next
     */
    public function testNext($iterator) {
        while (true) {
            $result = git_reference_next($iterator);

            if ($result) {
                $this->assertResourceHasType($result,'git_reference');
            }
            else {
                $this->assertFalse($result);
                break;
            }
        }
    }

    /**
     * @depends testIteratorGlobNew
     * @phpGitTest git_reference_next_name
     */
    public function testNextName($iterator) {
        while (true) {
            $result = git_reference_next_name($iterator);

            if ($result) {
                $this->assertIsString($result);
            }
            else {
                $this->assertFalse($result);
                break;
            }
        }
    }

    /**
     * @phpGitTest git_reference_list
     */
    public function testList() {
        $repo = static::getRepository();
        $result = git_reference_list($repo);

        $this->assertIsArray($result);
        $this->assertIsString($result[0]);
    }

    /**
     * @phpGitTest git_reference_lookup
     */
    public function testLookup() {
        $repo = static::getRepository();
        $result = git_reference_lookup($repo,'refs/heads/i1');

        $this->assertResourceHasType($result,'git_reference');

        return $result;
    }

    /**
     * @depends testLookup
     * @phpGitTest git_reference_set_target
     */
    public function testSetTarget($ref) {
        $id = '2cd82ceaee5897c72eb8fded83632569c80c69c5';
        $logMessage = 'Update branch head i1';
        $result = git_reference_set_target($ref,$id,$logMessage);

        $this->assertResourceHasType($result,'git_reference');
    }

    /**
     * @depends testLookup
     * @phpGitTest git_reference_symbolic_target
     */
    public function testSymbolicTarget_NULL($ref) {
        $result = git_reference_symbolic_target($ref);

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_reference_lookup
     */
    public function testLookup_ENOTFOUND() {
        $this->expectException(\Exception::class);
        $this->expectExceptionMessageMatches("/Reference '.*' not found/");
        $this->expectExceptionCode(GIT_ENOTFOUND);

        $repo = static::getRepository();
        $result = git_reference_lookup($repo,'refs/heads/idonotexist');
    }

    /**
     * @phpGitTest git_reference_normalize_name
     */
    public function testNormalizeName() {
        $name = 'refs//heads////roger';
        $flags = GIT_REF_FORMAT_REFSPEC_PATTERN;

        $result = git_reference_normalize_name($name,$flags);

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_reference_peel
     */
    public function testPeel() {
        $repo = static::getRepository();
        $ref = git_reference_lookup($repo,'refs/tags/t1');

        $result = git_reference_peel($ref,GIT_OBJ_COMMIT);

        $this->assertResourceHasType($result,'git_object');
    }

    /**
     * @phpGitTest git_reference_remove
     */
    public function testRemove() {
        $repo = static::getRepository();
        $result = git_reference_remove($repo,'refs/heads/i7');

        $this->assertNull($result);
    }

    /**
     * @phpGitTest git_reference_remove
     */
    public function testRemove_NotFound() {
        $this->expectException(\Exception::class);
        $this->expectExceptionMessage("Reference 'refs/hdeads/i7' not found");

        $repo = static::getRepository();
        $result = git_reference_remove($repo,'refs/hdeads/i7');
    }

    /**
     * @phpGitTest git_reference_rename
     */
    public function testRename() {
        $oldName = 'i5';
        $newName = 'i55';

        $repo = static::getRepository();
        $ref = git_reference_lookup($repo,"refs/heads/$oldName");
        $newName = "refs/heads/$newName";
        $force = true;
        $logMessage = "Rename branch '$oldName' to $newName";
        $result = git_reference_rename($ref,$newName,$force,$logMessage);

        $this->assertResourceHasType($result,'git_reference');
    }

    /**
     * @phpGitTest git_reference_rename
     */
    public function testRename_EEXISTS() {
        $this->expectException(\Exception::class);
        $this->expectExceptionMessage('a reference with that name already exists');
        $this->expectExceptionCode(GIT_EEXISTS);

        $oldName = 'i2';
        $newName = 'i3';

        $repo = static::getRepository();
        $ref = git_reference_lookup($repo,"refs/heads/$oldName");
        $newName = "refs/heads/$newName";
        $force = false;
        $logMessage = "Rename branch '$oldName' to $newName";
        $result = git_reference_rename($ref,$newName,$force,$logMessage);
    }

    /**
     * @phpGitTest git_reference_symbolic_create
     */
    public function testSymbolicCreate() {
        $repo = static::getRepository();
        $name = 'refs/sym/frosty_meninsky';
        $target = 'refs/heads/i2';
        $force = true;
        $logMessage = 'If you would prefer to receive a printed copy';
        $result = git_reference_symbolic_create($repo,$name,$target,$force,$logMessage);

        $this->assertResourceHasType($result,'git_reference');

        return $result;
    }

    /**
     * @depends testSymbolicCreate
     * @phpGitTest git_reference_symbolic_create
     */
    public function testSymbolicCreate_EEXISTS($ref) {
        $this->expectException(\Exception::class);
        $this->expectExceptionMessage("a reference with that name already exists");
        $this->expectExceptionCode(GIT_EEXISTS);

        $repo = static::getRepository();
        $name = 'refs/sym/frosty_meninsky';
        $target = 'refs/heads/i3';
        $force = false;
        $logMessage = 'Trying to overwrite frosty_meninsky';
        $result = git_reference_symbolic_create($repo,$name,$target,$force,$logMessage);
    }

    /**
     * @depends testSymbolicCreate
     * @phpGitTest git_reference_resolve
     */
    public function testResolve($ref) {
        $result = git_reference_resolve($ref);

        $this->assertResourceHasType($result,'git_reference');
    }

    /**
     * @depends testSymbolicCreate
     * @phpGitTest git_reference_symbolic_target
     */
    public function testSymbolicTarget($ref) {
        $result = git_reference_symbolic_target($ref);

        $this->assertIsString($result);
    }

    /**
     * @depends testSymbolicCreate
     * @phpGitTest git_reference_target
     */
    public function testTarget_NULL($ref) {
        $result = git_reference_target($ref);

        $this->assertNull($result);
    }

    /**
     * @depends testSymbolicCreate
     * @phpGitTest git_reference_symbolic_create_matching
     */
    public function testSymbolicCreateMatching($ref) {
        $repo = static::getRepository();
        $name = 'refs/sym/frosty_meninsky';
        $target = 'refs/heads/i3';
        $force = true;
        $currentValue = 'refs/heads/i2';
        $logMessage = 'If you would prefer to receive a printed copy';
        $result = git_reference_symbolic_create_matching($repo,$name,$target,$force,$currentValue,$logMessage);

        $this->assertResourceHasType($result,'git_reference');

        return $result;
    }

    /**
     * @depends testSymbolicCreateMatching
     * @phpGitTest git_reference_symbolic_create_matching
     */
    public function testSymbolicCreateMatching_EMODIFIED($ref) {
        $this->expectException(\Exception::class);
        $this->expectExceptionMessage('old reference value does not match');
        $this->expectExceptionCode(GIT_EMODIFIED);

        $repo = static::getRepository();
        $name = 'refs/sym/frosty_meninsky';
        $target = 'refs/heads/i1';
        $force = true;
        $currentValue = 'refs/heads/master';
        $logMessage = 'If you would prefer to receive a printed copy';
        $result = git_reference_symbolic_create_matching($repo,$name,$target,$force,$currentValue,$logMessage);
    }

    /**
     * @depends testSymbolicCreateMatching
     * @phpGitTest git_reference_symbolic_set_target
     */
    public function testSymbolicSetTarget($ref) {
        $target = 'refs/heads/i2';
        $logMessage = 'Revert back to original target';
        $result = git_reference_symbolic_set_target($ref,$target,$logMessage);

        $this->assertResourceHasType($result,'git_reference');
    }

    /**
     * @depends testSymbolicCreateMatching
     * @phpGitTest git_reference_symbolic_set_target
     */
    public function testSymbolicSetTarget_EINVALIDSPEC($ref) {
        $this->expectException(\Exception::class);
        $this->expectExceptionMessageMatches("/given reference name '.*' is not valid/");
        $this->expectExceptionCode(GIT_EINVALIDSPEC);

        $target = '~~refs/BAD?/name';
        $logMessage = 'Revert back to original target';
        $result = git_reference_symbolic_set_target($ref,$target,$logMessage);
    }

    /**
     * @phpGitTest git_reference_target_peel
     */
    public function testTargetPeel() {
        $repo = static::getRepository();
        $ref = git_reference_lookup($repo,'refs/tags/t1');

        $result = git_reference_target_peel($ref);

        $this->assertIsString($result);
    }
}
