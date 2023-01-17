<?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryBareTestCase;
use PhpGit2\Backend\TestConfigBackend;

final class CustomConfigBackendTest extends RepositoryBareTestCase {
    /**
     * 
     */
    public function testProperties() {
        $backend = new TestConfigBackend($this);

        // These properties do not exist until the backend instance is attached
        // to a git_config.
        $this->assertTrue(!property_exists($backend,'version'));

        // This property will always exist.
        $this->assertTrue(property_exists($backend,'readonly'));
        $this->assertIsBool($backend->readonly);
        $this->assertFalse($backend->readonly);

        $cfg = git_config_new();
        $backend->readonly = 'yes';
        git_config_add_backend($cfg,$backend,GIT_CONFIG_HIGHEST_LEVEL,null,false);

        // The version property now exists as an integer.
        $this->assertTrue(property_exists($backend,'version'));
        $this->assertIsInt($backend->version);

        // Check that the 'readonly' property was synced and that it was cast to
        // boolean.
        $this->assertTrue(property_exists($backend,'readonly'));
        $this->assertIsBool($backend->readonly);
        $this->assertTrue($backend->readonly);        
    }

    /**
     * 
     */
    public function testWrite() {
        $repo = static::getRepository();
        $cfg = git_config_new();
        git_repository_set_config($repo,$cfg);

        $backend = new TestConfigBackend($this);
        git_config_add_backend($cfg,$backend,GIT_CONFIG_HIGHEST_LEVEL,$repo,false);

        git_config_set_bool($cfg,'test.bool1',true);
        git_config_set_bool($cfg,'test.bool2',false);
        git_config_set_int32($cfg,'test.int321',33);
        git_config_set_int32($cfg,'test.int322',34);
        git_config_set_int64($cfg,'test.int641',287498247);
        git_config_set_int64($cfg,'test.int642',5465123654987);
        git_config_set_string($cfg,'test.string1','lego_monolith');
        git_config_set_string($cfg,'test.string2','monolithic_lego');

        git_config_set_multivar($cfg,'test.mvalue','/value[1-9]+/','value0');

        $this->assertTrue($backend->wasCalled('open'));
        $this->assertTrue($backend->wasCalled('set'));
        $this->assertTrue($backend->wasCalled('set_multivar'));
    }

    /**
     * @depends testWrite
     */
    public function testRead() {
        $repo = static::getRepository();
        $cfg = git_config_new();
        git_repository_set_config($repo,$cfg);

        $backend = new TestConfigBackend($this);
        git_config_add_backend($cfg,$backend,GIT_CONFIG_HIGHEST_LEVEL,$repo,false);

        $cfg = git_config_snapshot($cfg);
        $cloned = TestConfigBackend::getLatest();
        $this->assertTrue($cloned !== $backend);

        $value = git_config_get_bool($cfg,'test.bool1');
        $value = git_config_get_int32($cfg,'test.int322');
        $this->assertEquals(34,$value);
        $value = git_config_get_int64($cfg,'test.int641');
        $value = git_config_get_string($cfg,'test.string2');
        $this->assertEquals('monolithic_lego',$value);

        // Ensure that the initial backend did not get called for the 'get'
        // method. This only should happen to the cloned object.
        $this->assertTrue($backend->wasCalled('open'));
        $this->assertTrue(!$backend->wasCalled('get'));

        $this->assertTrue($cloned->wasCalled('open'));
        $this->assertTrue($cloned->wasCalled('get'));
    }

    /**
     * @depends testRead
     */
    public function testDelete() {
        $repo = static::getRepository();
        $cfg = git_config_new();
        git_repository_set_config($repo,$cfg);

        $backend = new TestConfigBackend($this);
        git_config_add_backend($cfg,$backend,GIT_CONFIG_HIGHEST_LEVEL,$repo,false);

        git_config_delete_entry($cfg,'test.int322');

        $this->assertTrue($backend->wasCalled('open'));
        $this->assertTrue($backend->wasCalled('del'));
    }
}
