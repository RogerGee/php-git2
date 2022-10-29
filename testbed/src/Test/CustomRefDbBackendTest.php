<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryBareTestCase;
use PhpGit2\Backend\TestRefDbBackend;
use PhpGit2\Callback\CallbackPayload;

final class CustomRefDbBackendTest extends RepositoryBareTestCase {
    public function testGeneral() {
        $repo = git_repository_new();
        $odb = git_odb_new();
        $refdb = git_refdb_new($repo);

        $path = $this->makeDirectory('odb','loose');
        $loose = git_odb_backend_loose($path,5,false,0700,0600);
        git_odb_add_backend($odb,$loose,1);

        $backend = new TestRefDbBackend($this);
        git_refdb_set_backend($refdb,$backend);

        git_repository_set_refdb($repo,$refdb);
        git_repository_set_odb($repo,$odb);

        $builder = git_treebuilder_new($repo,null);
        $treeId = git_treebuilder_write($builder);
        git_treebuilder_free($builder);

        $author = git_signature_now('Testing Framework','testing@example.com');
        $commitId = git_commit_create(
            $repo,
            null,
            $author,
            $author,
            null,
            'This is a test commit for the refdb testing module.',
            git_tree_lookup($repo,$treeId),
            []
        );

        for ($i = 0;$i < 10;++$i) {
            $ref = git_reference_create($repo,"refs/test/r$i",$commitId,true,"");
        }

        $this->assertTrue($backend->wasCalled('write'));

        $ref = git_reference_lookup($repo,'refs/test/r5');

        $this->assertTrue($backend->wasCalled('lookup'));

        $callback = function($ref,$payload) {

        };
        $payload = null;
        git_reference_foreach($repo,$callback,$payload);

        $this->assertTrue($backend->wasCalled('iterator_new'));
        $this->assertTrue($backend->wasCalled('iterator_next'));

        git_reference_delete($ref);

        $this->assertTrue($backend->wasCalled('del'));
    }
}
