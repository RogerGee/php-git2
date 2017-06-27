<?php

namespace Git2Test\Commit;

require_once 'test_base.php';

function test_open() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $commit = git_commit_lookup($repo,'fd211e73b0e7ede926bfbc47026d5ad0bc4b8902');

    $brief = git_commit_summary($commit);
    $body = git_commit_body($commit);
    $time = git_commit_time($commit);
    $offset = git_commit_time_offset($commit);
    $dt = date('Y-m-d H:i:s',$time);

    echo 'Commit ' . git_commit_id($commit) . PHP_EOL;
    echo "  committed on $dt ($offset)\n\n";
    echo "$brief\n\n$body\n";

    $owner = git_commit_owner($commit);
    echo PHP_EOL . 'Owner path:' . PHP_EOL;
    var_dump(git_repository_path($owner));

    echo 'Here\'s who made the commit:' . PHP_EOL;
    var_dump(git2_signature_convert(git_commit_author($commit)));
}

function test_lookup() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $commit = git_commit_lookup($repo,'fd211e73b0e7ede926bfbc47026d5ad0bc4b8902');

    $a = git_commit_nth_gen_ancestor($commit,3);
    var_dump(git_commit_summary($a));

    try {
        $b = git_commit_nth_gen_ancestor($commit,5000000);
    } catch (\Exception $ex) {
        var_dump($ex->getMessage());
        var_dump($ex->getCode());

        if ($ex->getCode() == GIT_ENOTFOUND) {
            echo 'The specified ancestor was not found.' . PHP_EOL;
        }
    }    
}

function test_create() {
    $repo = git_repository_open_bare(testbed_get_repo_path());

    // Use the current HEAD and a static commit as the parents.
    $head = git_reference_name_to_id($repo,'HEAD');
    $parents[] = git_commit_lookup($repo,$head);
    $parents[] = git_commit_lookup($repo,'4c4afe37e0fb4623b25f73f5a8033a92daf76ddf');

    // Create a random reference for this test commit.
    $ref = "refs/heads/myref" . rand(1,1000);

    // Let this commit use an existing tree (for simplicity).
    $tree = git_tree_lookup($repo,'80113d972e694eeef398fc7bbac5023ab7d2a311');

    // Create a bogus author.
    $author = git_signature_now('Bargus Targ','bargus.targ@example.com');

    // Create the commit.
    $id = git_commit_create(
        $repo,
        $ref,
        $author,
        $author,
        null,
        "Commit initial material using a PHP script\n\nThis reverts everything!",
        $tree,
        $parents);

    echo "Created commit with oid=$id.\n";

    // Create same commit but not in the repository.
    $data = git_commit_create_buffer(
        $repo,
        $author,
        $author,
        null,
        "Commit initial material using a PHP script\n\nThis reverts everything!",
        $tree,
        $parents);

    echo "\nCreated commit outside repo with contents:\n";
    var_dump($data);
}

function test_signature() {
    $repo = git_repository_open_bare(testbed_get_repo_path());

    try {
        $signature = git_commit_extract_signature(
            $signedData,
            $repo,
            '4c4afe37e0fb4623b25f73f5a8033a92daf76ddf',
            null);


        var_dump($signature);
        var_dump($signedData);
    } catch (\Exception $ex) {
        echo 'Cannot extract commit signature: ' . $ex->getMessage() . PHP_EOL;
    }
}

function test_create_alternate() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $lambda = function($idx,$repo) {
        $parents = array(
            '4aa38890c5191cc16f50b057263cbfb305ed7ead',
            'e88ad7b6240c422a5d2b9a58d9ced716821fbafa',
        );

        if ($idx >= count($parents)) {
            echo '  payload: ' . $repo . PHP_EOL;
            return null;
        }

        echo "  test_commit_alternate::callback($idx)\n";
        return $parents[$idx];
    };

    // Create a random reference for this test commit.
    $ref = "refs/heads/myref" . rand(1,1000);

    // Create a bogus author.
    $author = git_signature_now('Bargus Targ','bargus.targ@example.com');

    echo 'Creating a commit via callback:' . PHP_EOL;
    $id = git_commit_create_from_callback(
        $repo,
        $ref,
        $author,
        $author,
        null,
        "[stupid] Be the boss using git and PHP for whatever it's worth [1]",
        '80113d972e694eeef398fc7bbac5023ab7d2a311',
        $lambda,
        $repo);
    var_dump($id);

    echo PHP_EOL . 'Creating a commit via OID list' . PHP_EOL;
    $oids = array(
        '4aa38890c5191cc16f50b057263cbfb305ed7ead',
        'e88ad7b6240c422a5d2b9a58d9ced716821fbafa',
    );
    $id = git_commit_create_from_ids(
        $repo,
        "{$ref}_fromid",
        $author,
        $author,
        null,
        "[stupid] Be the boss using git and PHP for whatever it's worth [2]",
        '80113d972e694eeef398fc7bbac5023ab7d2a311',
        $oids);
    var_dump($id);
}

testbed_test('Commit/open','Git2Test\Commit\test_open');
testbed_test('Commit/lookup','Git2Test\Commit\test_lookup');
testbed_test('Commit/create','Git2Test\Commit\test_create');
testbed_test('Commit/signature','Git2Test\Commit\test_signature');
testbed_test('Commit/create_alternate','Git2Test\Commit\test_create_alternate');
