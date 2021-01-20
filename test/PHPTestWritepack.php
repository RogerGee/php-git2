<?php

require_once 'test_base.php';
require_once 'PHPProxyWritepack.php';

/**
 * PHPTestWritepack
 *
 * Provides a PHPProxyWritepack that dumps stats updates to log.
 */
class PHPTestWritepack extends PHPProxyWritepack {
    protected function updateStats(array &$stats,$final = false) {
        if (!$final) {
            testbed_dump('PHPTestWritepack.stats.before',$stats);
        }
        parent::updateStats($stats,$final);
        if (!$final) {
            testbed_dump('PHPTestWritepack.stats.after',$newStats);
        }
        else {
            testbed_dump('PHPTestWritepack.stats.final',$stats);
        }
    }
}
