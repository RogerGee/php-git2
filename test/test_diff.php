<?php

namespace Git2Test\Diff;

require_once 'test_base.php';

function diff_notify_callback($diff,array $delta,$matchedPathspec,$payload) {
    echo "---DIFF_NOTIFY---\n";
    var_dump($diff);
    var_dump($delta);
    var_dump($matchedPathspec);
    var_dump($payload);
}

function diff_progress_callback($diff,$oldPath,$newPath,$payload) {
    echo "---DIFF_PROGRESS---\n";
    var_dump($diff);
    var_dump($oldPath);
    var_dump($newPath);
    var_dump($payload);
}

function diff_file_callback($delta,$progress,$payload) {
    echo "---DIFF_FILE---\n";
    var_dump($delta);
    var_dump($progress);
    var_dump($payload);
}

function diff_binary_callback($delta,$binary,$payload) {
    echo "---DIFF_BINARY---\n";
    var_dump($delta);
    var_dump($binary);
    var_dump($payload);
}

function diff_hunk_callback($delta,$hunk,$payload) {
    echo "---DIFF_HUNK---\n";
    var_dump($delta);
    var_dump($hunk);
    var_dump($payload);
}

function diff_line_callback($delta,$hunk,$line,$payload) {
    echo "---DIFF_LINE---\n";
    var_dump($delta);
    var_dump($hunk);
    var_dump($line);
    var_dump($payload);
}

function accumulate_diff_by_line($delta,$hunk,$line,$ctx) {
    if ($hunk['header'] != $ctx->lastHunkKey) {
        echo $hunk['header'];
        $ctx->lastHunkKey = $hunk['header'];
        $ctx->lastHunk = $hunk;
    }
    echo chr($line['origin']) . $line['content'];
}

function test_blob_to_buffer() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $blob = git_blob_lookup($repo,'063a11c4dc16ffaa1469b5f29ce83903f6cac77e');
    $content = git_blob_rawcontent($blob);

    $lines = explode("\n",$content);
    for ($i = count($lines)/2;$i < intval(count($lines) * 0.575);++$i) {
        unset($lines[$i]);
    }
    $content = implode("\n",$lines);

    // Run for testing callbacks.
    git_diff_blob_to_buffer(
        $blob,
        "/path/to/sourceFile",
        $content,
        "/path/to/newFile",
        null,
        'Git2Test\Diff\diff_file_callback',
        'Git2Test\Diff\diff_binary_callback',
        'Git2Test\Diff\diff_hunk_callback',
        'Git2Test\Diff\diff_line_callback',
        33);

    // Run to get the diff.

    $options = array(
        'old_prefix' => 'OLD_THING',
        'new_prefix' => 'NEW_THING',
        'context_lines' => 10,
        // NOTE: these callbacks aren't called by git_diff_blob_to_buffer
        'notify_cb' => 'Git2Test\Diff\diff_notify_callback',
        'progress_cb' => 'Git2Test\Diff\diff_progress_callback',
        'payload' => 55,
    );

    $ctx = new \stdClass;
    $ctx->lastHunkKey = null;
    $ctx->lastHunk = array();

    git_diff_blob_to_buffer(
        $blob,
        "/path/to/sourceFile",
        $content,
        "/path/to/newFile",
        $options,
        null,
        null,
        null,
        'Git2Test\Diff\accumulate_diff_by_line',
        $ctx);
}

function test_from_buffer() {
    $buf = base64_decode('ZGlmZiAtLWdpdCBhL2JpbmRpbmdzIGIvYmluZGluZ3MKaW5kZXggMT'
                         .'VjZTcyNi4uNmNhZjc2NyAxMDA2NDQKLS0tIGEvYmluZGluZ3MKKys'
                         .'rIGIvYmluZGluZ3MKQEAgLTE5NSw3ICsxOTUsNyBAQCBMZWdlbmQ6'
                         .'CiAuIGdpdF9kaWZmX21lcmdlCiAuIGdpdF9kaWZmX251bV9kZWx0Y'
                         .'XMKIC4gZ2l0X2RpZmZfbnVtX2RlbHRhc19vZl90eXBlCi0uIGdpdF'
                         .'9kaWZmX3ByaW50CisrIGdpdF9kaWZmX3ByaW50CiAuIGdpdF9kaWZ'
                         .'mX3ByaW50X2NhbGxiYWNrX190b19idWYKIC4gZ2l0X2RpZmZfcHJp'
                         .'bnRfY2FsbGJhY2tfX3RvX2ZpbGVfaGFuZGxlCiAuIGdpdF9kaWZmX'
                         .'3N0YXRzX2RlbGV0aW9ucwo=');

    $diff = git_diff_from_buffer($buf);

    $printer = 'Git2Test\Diff\accumulate_diff_by_line';
    $ctx = new \stdClass;
    $ctx->lastHunkKey = null;
    $ctx->lastHunk = array();

    // There is a bug in 0.25.1 that makes this fail. See issue at
    // https://github.com/libgit2/libgit2/issues/4158. This was fixed for
    // 0.26.0.

    //git_diff_print($diff,GIT_DIFF_FORMAT_PATCH,$printer,$ctx);

    var_dump(git_diff_get_delta($diff,0));
    //var_dump(git_diff_get_perfdata($diff)); // This fails due to same bug.
    var_dump(git_diff_is_sorted_icase($diff));
}

function test_tree() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $commitNew = git_commit_lookup($repo,'9f20494a73fba9122db1ab1e48ef57fa69c9c071');
    $commitOld = git_commit_lookup($repo,'dd34fc05733652126a0f3d4494c328f215890248');

    $treeNew = git_commit_tree($commitNew);
    $treeOld = git_commit_tree($commitOld);

    $diff = git_diff_tree_to_tree($repo,$treeOld,$treeNew,null);

    $buf = git_diff_to_buf($diff,GIT_DIFF_FORMAT_PATCH);
    var_dump($buf);
    var_dump(git_diff_num_deltas($diff));
    var_dump(git_diff_num_deltas_of_type($diff,GIT_DELTA_ADDED));
    var_dump(git_diff_num_deltas_of_type($diff,GIT_DELTA_MODIFIED));
}

function test_stats() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $commitNew = git_commit_lookup($repo,'9f20494a73fba9122db1ab1e48ef57fa69c9c071');
    $commitOld = git_commit_lookup($repo,'dd34fc05733652126a0f3d4494c328f215890248');

    $treeNew = git_commit_tree($commitNew);
    $treeOld = git_commit_tree($commitOld);

    $diff = git_diff_tree_to_tree($repo,$treeOld,$treeNew,null);
    $stats = git_diff_get_stats($diff);
    var_dump(git_diff_stats_insertions($stats));
    var_dump(git_diff_stats_deletions($stats));
    var_dump(git_diff_stats_files_changed($stats));
    echo git_diff_stats_to_buf($stats,GIT_DIFF_STATS_FULL,80);
}

function test_format_email() {
    $repo = git_repository_open_bare(testbed_get_repo_path());
    $commitNew = git_commit_lookup($repo,'ee5dcbece3fa22760dbe5aca1f0df3a5c1226406');
    $commitOld = git_commit_lookup($repo,'1a51b105507e293050d6967b4d9ec1ba49ed4e83');

    $treeNew = git_commit_tree($commitNew);
    $treeOld = git_commit_tree($commitOld);
    $diff = git_diff_tree_to_tree($repo,$treeOld,$treeNew,null);

    // Test with array signature.
    $opts = array(
        'flags' => GIT_DIFF_FORMAT_EMAIL_NONE,
        'patch_no' => 1,
        'total_patches' => 1,
        'id' => str_repeat('a',40),
        'summary' => 'I did some stuff...',
        'body' => 'I must be vague about the stuff that I did...',
        'author' => array(
            'name' => 'Roger Gee',
            'email' => 'roger@example.com',
            'when.time' => time(),
            'when.offset' => -60 * 6,
        ),
    );

    $buf = git_diff_format_email($diff,$opts);
    var_dump($buf);

    // Test with resource signature.
    $opts = array(
        'flags' => GIT_DIFF_FORMAT_EMAIL_NONE,
        'patch_no' => 2,
        'total_patches' => 2,
        'id' => str_repeat('b',40),
        'summary' => 'I did some more stuff...',
        'body' => 'I must be vague about the additional stuff that I did...',
        'author' => git_signature_new(
            'Buddy Bookworm','buddy@tulsalibrary.org',time(),-123),
    );

    $buf = git_diff_format_email($diff,$opts);
    var_dump($buf);
}

function test_misc() {
    var_dump(git_diff_status_char(GIT_DELTA_ADDED));
    var_dump(git_diff_status_char(GIT_DELTA_IGNORED));
}

testbed_test('Diff/blob-to-buffer','Git2Test\Diff\test_blob_to_buffer');
testbed_test('Diff/from-buffer','Git2Test\Diff\test_from_buffer');
testbed_test('Diff/tree','Git2Test\Diff\test_tree');
testbed_test('Diff/stats','Git2Test\Diff\test_stats');
testbed_test('Diff/format-email','Git2Test\Diff\test_format_email');
testbed_test('Diff/misc','Git2Test\Diff\test_misc');
