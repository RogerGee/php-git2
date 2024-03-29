<?php

namespace PhpGit2;

use RuntimeError;

class TestCase extends \PHPUnit\Framework\TestCase {
    const DIRNAME = '.php-git2-testbed';

    public static $nop = [self::class,'nop'];
    public static function nop(...$args) : void {

    }

    public static function setUpBeforeClass() : void {
        $basePath = self::makePath();
        if (is_dir($basePath)) {
            self::rmdirRecursive($basePath,$basePath);
        }
        mkdir($basePath);
    }

    public static function tearDownAfterClass() : void {
        $basePath = self::makePath();
        self::rmdirRecursive($basePath,$basePath);
    }

    public static function makePath(string ...$parts) : string {
        $tmp = realpath(sys_get_temp_dir());
        $ps = array_merge([$tmp,self::DIRNAME],$parts);
        $ps = array_filter($ps);
        return implode(DIRECTORY_SEPARATOR,$ps);
    }

    public static function makeDirectory(string ...$pathParts) : string {
        $base = self::makePath();
        $dir = self::makePath(...$pathParts);
        assert( is_dir($base) );

        $n = strlen($base);
        while ($n < strlen($dir) && $dir[$n] == DIRECTORY_SEPARATOR) {
            $n += 1;
        }
        while ($n < strlen($dir)) {
            $n = strpos($dir,DIRECTORY_SEPARATOR,$n);
            if ($n === false) {
                $d = $dir;
                $n = strlen($dir);
            }
            else {
                $d = substr($dir,0,$n);
                while ($n < strlen($dir) && $dir[$n] == DIRECTORY_SEPARATOR) {
                    $n += 1;
                }
            }

            if (!is_dir($d)) {
                if (mkdir($d) === false) {
                    throw new RuntimeError("Cannot create testbed directory '$d'");
                }
            }
        }

        return $dir;
    }

    public static function makeFile(string $contents,string ...$pathParts) : string {
        $filePath = self::makePath(...$pathParts);
        $f = fopen($filePath,'w');
        fwrite($f,$contents);
        fclose($f);
        return $filePath;
    }

    public static function makeFileStream(string ...$pathParts) {
        $filePath = self::makePath(...$pathParts);
        $f = fopen($filePath,'w');
        if (!$f) {
            throw new RuntimeError("Cannot open file stream for writing: $filePath");
        }
        return $f;
    }

    public static function copyFile(string $src,string ...$dstParts) : string {
        $dst = self::makePath(...$dstParts);
        if (!copy($src,$dst)) {
            throw new RuntimeError("Failed to copy '$src' to '$dst'");
        }
        return $dst;
    }

    public static function copyNonBareRepoToEnvironment(string $repoNameSrc,string $repoNameDst) {
        $basePath = self::makePath();
        $src = $repoNameSrc;
        $dst = self::makePath($repoNameDst);

        self::copyRecursive($src,$dst,$basePath);

        // Rename the .git subdirectory on the non-bare repo. We had to rename
        // it to commit it into the project.
        $gitted = implode(DIRECTORY_SEPARATOR,[$dst,'.gitted']);
        $git = implode(DIRECTORY_SEPARATOR,[$dst,'.git']);
        rename($gitted,$git);
    }

    public static function copyRecursive(string $pSrc,string $pDst,string $basePath) {
        $src = rtrim($pSrc,DIRECTORY_SEPARATOR);
        $dst = rtrim($pDst,DIRECTORY_SEPARATOR);
        assert(substr($dst,0,strlen($basePath)) == $basePath);

        assert(is_dir($src));
        mkdir($dst);

        $entries = scandir($src);
        assert($entries !== false);

        foreach ($entries as $entry) {
            if ($entry == '.' || $entry == '..') {
                continue;
            }

            $srcEntry = implode(DIRECTORY_SEPARATOR,[$src,$entry]);
            $dstEntry = implode(DIRECTORY_SEPARATOR,[$dst,$entry]);

            if (is_dir($srcEntry)) {
                self::copyRecursive($srcEntry,$dstEntry,$basePath);
            }
            else {
                if (copy($srcEntry,$dstEntry) === false) {
                    throw new RuntimeError("Could not copy file '$srcEntry'");
                }
            }
        }
    }

    public static function rmdirRecursive(string $pPath,string $basePath) {
        $path = rtrim($pPath,DIRECTORY_SEPARATOR);
        assert(substr($path,0,strlen($basePath)) == $basePath);
        assert(is_dir($path));

        $entries = scandir($path);
        assert($entries !== false);

        foreach ($entries as $entry) {
            if ($entry == '.' || $entry == '..') {
                continue;
            }

            $entryPath = implode(DIRECTORY_SEPARATOR,[$path,$entry]);
            if (is_dir($entryPath)) {
                self::rmdirRecursive($entryPath,$basePath);
            }
            else {
                if (unlink($entryPath) === false) {
                    throw new RuntimeError("Could not remove file '$entryPath'");
                }
            }
        }

        if (rmdir($path) === false) {
            throw new RuntimeError("Could not remove directory '$entryPath'");
        }
    }

    /**
     * Asserts that the value is either an array variable or NULL.
     *
     * @param mixed $value
     */
    public function assertIsArrayOrNull($value) {
        $this->assertThat(
            $value,
            $this->logicalOr(
                $this->isNull(),
                $this->isType('array')
            )
        );
    }

    /**
     * Asserts that the value is either a string variable or NULL.
     *
     * @param mixed $value
     */
    public function assertIsStringOrNull($value) {
        $this->assertThat(
            $value,
            $this->logicalOr(
                $this->isNull(),
                $this->isType('string')
            )
        );
    }

    /**
     * Asserts that the resource variable has the indicated resource type.
     *
     * @param resource $resource
     * @param string $resourceType
     *  The resource type name. NOTE: this can be any substring to match within
     *  the full resource type name. We must use substring matching because the
     *  resource names are (at least in part) generated by the c++ compiler.
     */
    public function assertResourceHasType($resource,string $resourceType) {
        $this->assertIsResource($resource);

        $type = get_resource_type($resource);
        $this->assertStringContainsString($resourceType,$type);
    }
}
