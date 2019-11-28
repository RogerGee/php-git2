<?php

require_once 'test_base.php';

/**
 * Create a custom ODB writepack. This writepack wraps an indexer (which is the
 * default writepack essentially) to index the packfile and extracts objects
 * into the parent ODB backend.
 */
class PHPTestWritepack extends GitODBWritepack {
    private $path;
    private $indexer;

    public function __construct($odb,$callback,$payload) {
        // A custom writepack ALWAYS has an ODB backend. But, it is not attached
        // until after the constructor runs.
        //assert($this->backend instanceof GitODBBackend);

        $this->path = testbed_path('tmp/writepack',true);
        $this->indexer = git_indexer_new($this->path,0600,$odb,$callback,$payload);
    }

    /**
     * Implements GitODBWritepack::append().
     */
    public function append($data,array &$stats) {
        // Stream the packfile to temporary location.
        git_indexer_append($this->indexer,$data);
        $this->updateStats($stats);
    }

    /**
     * Implements GitODBWritepack::commit().
     */
    public function commit(&$stats) {
        git_indexer_commit($this->indexer);
        $hash = git_indexer_hash($this->indexer);
        $this->updateStats($stats);

        testbed_unit('PHPTestWritepack.stats.final',$stats);

        // Extract each object from the pack and insert it into our backend.
        $indexFile = $this->path . DIRECTORY_SEPARATOR . "pack-$hash.idx";
        $src = git_odb_backend_one_pack($indexFile);
        $src->for_each(function($oid,$src) {
            $data = $src->read($type,$oid);
            $stream = git_odb_open_wstream($this->backend->odb,strlen($data),$type);
            $stream->write($data);
            $stream->finalize_write($oid);

        }, $src);
    }

    private function updateStats(array &$stats) {
        testbed_unit('PHPTestWritepack.stats.before',$stats);
        $newStats = git2_indexer_stats($this->indexer);
        foreach (array_keys($stats) as $key) {
            $stats[$key] = $newStats[$key];
        }
        testbed_unit('PHPTestWritepack.stats.after',$newStats);
    }
}
