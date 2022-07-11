<?php

namespace PhpGit2\Test;

use PhpGit2\TestCase;
use PhpGit2\Callback\CallbackPayload;
use PhpGit2\Callback\CallbackReturnValue;

final class ConfigTest extends TestCase {
    /**
     * @phpGitTest git_config_new
     */
    public function testNew() {
        $result = git_config_new();

        $this->assertResourceHasType($result,'git_config');

        return $result;
    }

    /**
     * @depends testNew
     * @phpGitTest git_config_add_file_ondisk
     */
    public function testAddFileOndisk($config) {
        $path = static::copyFile('resources/git-config.example','git-config');
        $level = GIT_CONFIG_HIGHEST_LEVEL;
        $force = true;
        $result = git_config_add_file_ondisk($config,$path,$level,$force);

        $this->assertNull($result);

        return $config;
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_add_file_ondisk
     */
    public function testAddFileOndisk_EEXISTS($config) {
        $this->expectException(\Exception::class);
        $this->expectExceptionCode(GIT_EEXISTS);

        $path = static::makePath('git-config');
        $level = GIT_CONFIG_HIGHEST_LEVEL;
        $force = false;
        $result = git_config_add_file_ondisk($config,$path,$level,$force);
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_add_file_ondisk
     */
    public function testAddFileOndisk_ENOTFOUND($config) {
        $this->expectException(\Exception::class);
        // NOTE: The documentation says this should return ENOTFOUND but it just
        // returns -1.

        $path = static::makePath('git-config-does-not-exist');
        $level = GIT_CONFIG_HIGHEST_LEVEL;
        $force = false;
        $result = git_config_add_file_ondisk($config,$path,$level,$force);
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_delete_entry
     */
    public function testDeleteEntry($config) {
        $name = 'delete.b';
        $result = git_config_delete_entry($config,$name);

        $this->assertNull($result);
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_delete_multivar
     */
    public function testDeleteMultivar($config) {
        $name = 'delete.m';
        $regexp = 'm[0-2]';
        $result = git_config_delete_multivar($config,$name,$regexp);

        $this->assertNull($result);
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_foreach
     */
    public function testForeach($config) {
        $n = 0;

        $callback = function($entry,$payload) use(&$n) {
            $n += 1;
            $this->assertIsArray($entry);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $payload = new CallbackPayload;
        $result = git_config_foreach($config,$callback,$payload);

        $this->assertGreaterThan(1,$n);
        $this->assertNull($result);
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_foreach
     */
    public function testForeach_StopIteration($config) {
        $n = 0;

        $callback = function($entry,$payload) use(&$n) {
            $n += 1;
            $this->assertIsArray($entry);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
            return true;
        };
        $payload = new CallbackPayload;
        $result = git_config_foreach($config,$callback,$payload);

        $this->assertEquals(1,$n);
        $this->assertNull($result);
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_foreach_match
     */
    public function testForeachMatch($config) {
        $regexp = 'core\.ki.*';
        $callback = function($entry,$payload) use(&$n) {
            $n += 1;
            $this->assertIsArray($entry);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $payload = new CallbackPayload;
        $result = git_config_foreach_match($config,$regexp,$callback,$payload);

        $this->assertGreaterThan(1,$n);
        $this->assertNull($result);
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_foreach_match
     */
    public function testForeachMatch_StopIteration($config) {
        $regexp = 'core\.ki.*';
        $callback = function($entry,$payload) use(&$n) {
            $n += 1;
            $this->assertIsArray($entry);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
            return true;
        };
        $payload = new CallbackPayload;
        $result = git_config_foreach_match($config,$regexp,$callback,$payload);

        $this->assertEquals(1,$n);
        $this->assertNull($result);
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_get_bool
     */
    public function testGetBool($config) {
        $name = 'core.kstring';
        $result = git_config_get_bool($config,$name);

        $this->assertIsBool($result);
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_get_entry
     */
    public function testGetEntry($config) {
        $name = 'core.kint32';
        $result = git_config_get_entry($config,$name);

        $this->assertIsArray($result);
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_get_int32
     */
    public function testGetInt32($config) {
        $name = 'core.kint32';
        $result = git_config_get_int32($config,$name);

        $this->assertIsInt($result);
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_get_int64
     */
    public function testGetInt64($config) {
        $name = 'core.kint64';
        $result = git_config_get_int64($config,$name);

        $this->assertIsInt($result);
        // Ensure the 64-bit value made it through.
        $this->assertGreaterThan(0xffffffff,$result);
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_get_mapped
     */
    public function testGetMapped($config) {
        $name = 'core.kmapped1';
        $maps = [
            [GIT_CVAR_FALSE,null,2],
            [GIT_CVAR_TRUE,null,3],
            [GIT_CVAR_STRING,'value',4],
        ];
        $result = git_config_get_mapped($config,$name,$maps);

        $this->assertIsInt($result);
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_get_multivar_foreach
     */
    public function testGetMultivarForeach($config) {
        $n = 0;

        $name = 'core.kmulti';
        $regexp = null;
        $callback = function($entry,$payload) use(&$n) {
            $n += 1;
            $this->assertIsArray($entry);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
        };
        $payload = new CallbackPayload;
        $result = git_config_get_multivar_foreach($config,$name,$regexp,$callback,$payload);

        $this->assertGreaterThan(1,$n);
        $this->assertNull($result);
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_get_multivar_foreach
     */
    public function testGetMultivarForeach_StopIteration($config) {
        $n = 0;

        $name = 'core.kmulti';
        $regexp = null;
        $callback = function($entry,$payload) use(&$n) {
            $n += 1;
            $this->assertIsArray($entry);
            $this->assertInstanceOf(CallbackPayload::class,$payload);
            return true;
        };
        $payload = new CallbackPayload;
        $result = git_config_get_multivar_foreach($config,$name,$regexp,$callback,$payload);

        $this->assertEquals(1,$n);
        $this->assertNull($result);
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_get_path
     */
    public function testGetPath($config) {
        $name = 'core.kpath';
        $result = git_config_get_path($config,$name);

        $this->assertIsString($result);
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_get_string_buf
     */
    public function testGetStringBuf($config) {
        $name = 'core.kstring';
        $result = git_config_get_string_buf($config,$name);

        $this->assertIsString($result);
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_iterator_glob_new
     */
    public function testIteratorGlobNew($config) {
        $regexp = 'core\.ki.*';
        $result = git_config_iterator_glob_new($config,$regexp);

        $this->assertResourceHasType($result,'git_config_iterator');

        return $result;
    }

    /**
     * @depends testIteratorGlobNew
     * @phpGitTest git_config_iterator_free
     */
    public function testIteratorFree($iterator) {
        $result = git_config_iterator_free($iterator);

        $this->assertNull($result);
        $this->assertResourceHasType($iterator,'Unknown');

        $this->expectError();
        $this->expectWarningMessageMatches(
            '/supplied resource is not a valid [A-Za-z0-9_]*git_config_iterator[A-Za-z0-9_]* resource/'
        );

        git_config_next($iterator);
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_iterator_new
     */
    public function testIteratorNew($config) {
        $result = git_config_iterator_new($config);

        $this->assertResourceHasType($result,'git_config_iterator');

        return $result;
    }

    /**
     * @depends testIteratorNew
     * @phpGitTest git_config_next
     */
    public function testNext($iterator) {
        $result = git_config_next($iterator);

        $this->assertIsArray($result);

        return $result;
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_multivar_iterator_new
     */
    public function testMultivarIteratorNew($config) {
        $name = 'core.kmulti';
        $regexp = 'va.*';
        $result = git_config_multivar_iterator_new($config,$name,$regexp);

        $this->assertResourceHasType($result,'git_config_iterator');
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_open_level
     */
    public function testOpenLevel($config) {
        $result = git_config_open_level($config,GIT_CONFIG_HIGHEST_LEVEL);

        $this->assertResourceHasType($result,'git_config');
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_open_level
     */
    public function testOpenLevel_ENOTFOUND($config) {
        $this->expectException(\Exception::class);
        $this->expectExceptionCode(GIT_ENOTFOUND);

        $result = git_config_open_level($config,GIT_CONFIG_LEVEL_GLOBAL);
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_set_bool
     */
    public function testSetBool($config) {
        $name = 'set.kbool';
        $value = true;
        $result = git_config_set_bool($config,$name,$value);

        $this->assertNull($result);
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_set_int32
     */
    public function testSetInt32($config) {
        $name = 'set.kint32';
        $value = 0xaabbccdd;
        $result = git_config_set_int32($config,$name,$value);

        $this->assertNull($result);
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_set_int64
     */
    public function testSetInt64($config) {
        $name = 'set.kint64';
        $value = 0xaabbccddee;
        $result = git_config_set_int64($config,$name,$value);

        $this->assertNull($result);

        $read = git_config_get_int64($config,$name);

        $this->assertEquals($read,$value);
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_set_multivar
     */
    public function testSetMultivar($config) {
        $name = 'set.kmulti';
        $regexp = 'value1';
        $value = 'value1';
        $result = git_config_set_multivar($config,$name,$regexp,$value);

        $this->assertNull($result);
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_set_string
     */
    public function testSetString($config) {
        $name = 'set.kstring';
        $value = 'nifty_chaum';
        $result = git_config_set_string($config,$name,$value);

        $this->assertNull($result);
    }

    /**
     * @depends testAddFileOndisk
     * @phpGitTest git_config_snapshot
     */
    public function testSnapshot($config) {
        $result = git_config_snapshot($config);

        $this->assertResourceHasType($result,'git_config');

        return $result;
    }

    /**
     * @depends testSnapshot
     * @phpGitTest git_config_get_string
     */
    public function testGetString($config) {
        $name = 'core.kstring';
        $result = git_config_get_string($config,$name);

        $this->assertIsString($result);
    }

    /**
     * @phpGitTest git_config_lookup_map_value
     */
    public function testLookupMapValue() {
        $maps = [
            [GIT_CVAR_STRING,'ab',33],
        ];
        $value = 'ab';
        $result = git_config_lookup_map_value($maps,$value);

        $this->assertIsInt($result);
    }

    /**
     * @phpGitTest git_config_open_default
     */
    public function testOpenDefault() {
        $result = git_config_open_default();

        $this->assertResourceHasType($result,'git_config');

        return $result;
    }

    /**
     * @depends testOpenDefault
     * @phpGitTest git_config_open_global
     */
    public function testOpenGlobal($config) {
        $result = git_config_open_global($config);

        $this->assertResourceHasType($result,'git_config');
    }

    /**
     * @phpGitTest git_config_open_ondisk
     */
    public function testOpenOndisk() {
        $path = static::copyFile('resources/git-config.example','git-config-ondisk');
        $result = git_config_open_ondisk($path);

        $this->assertResourceHasType($result,'git_config');
    }

    /**
     * @phpGitTest git_config_parse_bool
     */
    public function testParseBool() {
        $value = 'yes';
        $result = git_config_parse_bool($value);

        $this->assertIsBool($result);
    }

    /**
     * @phpGitTest git_config_parse_int32
     */
    public function testParseInt32() {
        $value = '1024k';
        $result = git_config_parse_int32($value);

        $this->assertIsInt($result);
    }

    /**
     * @phpGitTest git_config_parse_int64
     */
    public function testParseInt64() {
        $value = '3792360643524008971';
        $result = git_config_parse_int64($value);

        $this->assertIsInt($result);
    }

    /**
     * @phpGitTest git_config_parse_path
     */
    public function testParsePath() {
        $path = '~/code/projects';
        $result = git_config_parse_path($path);

        $this->assertIsString($result);
    }
}
