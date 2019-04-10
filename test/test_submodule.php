<?php

namespace Git2Test\Submodule;

require_once 'test_base.php';

function lookup_test_submodule($repo) {
    try {
        $submodule = git_submodule_lookup($repo,'sub1');

    } catch (\Exception $ex) {
        $cloneURL = testbed_get_repo_path();

        if ($ex->getCode() == GIT_ENOTFOUND) {
            $submodule = git_submodule_add_setup(
                $repo,
                "file://$cloneURL",
                'sub1',
                true);
        }
    }

    return $submodule;
}

function test_create() {
    $repo = git_repository_open(testbed_get_localrepo_path());
    $submodule = lookup_test_submodule($repo);

    testbed_unit('test_create',$submodule);

    git_submodule_free($submodule);
}

function test_checkout() {
    $cloneURL = testbed_get_repo_path();
    $remote = git_repository_open_bare($cloneURL);

    $path = testbed_get_localrepo_path();
    $repo = git_repository_open($path);
    $submodule = lookup_test_submodule($repo);

    // TODO Figure this out...

    // $opts['repository_cb'] = function($path,$bare,$submodule) {
    //     return git_submodule_open($submodule);
    // };
    // $opts['repository_cb_payload'] = $submodule;
    // git_clone("file://$cloneURL","$path/sub1",$opts);

    // git_submodule_add_finalize($submodule);
}

testbed_test('Submodule/test_create','Git2Test\Submodule\test_create');
testbed_test('Submodule/test_checkout','Git2Test\Submodule\test_checkout');
