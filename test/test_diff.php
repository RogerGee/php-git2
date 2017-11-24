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

testbed_test('Diff/blob-to-buffer','Git2Test\Diff\test_blob_to_buffer');
