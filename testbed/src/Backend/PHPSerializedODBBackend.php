<?php

namespace PhpGit2\Backend;

class PHPSerializedODBBackend extends \GitODBBackend {
    private $storage = [];

    /**
     * Implements GitODBBackend::read().
     *
     * @param int &$type
     *  Returns the type of the object.
     * @param string $oid
     *  The ID of the object to read.
     *
     * @return string
     *  The object contents.
     */
    public function read(&$type,string $oid) : string|bool {
        if (isset($this->storage[$oid])) {
            $record = $this->storage[$oid];
            $type = $record['t'];
            return $record['d'];
        }

        return false;
    }

    /**
     * Implements GitODBBackend::read_prefix().
     *
     * @param string &$fullOid
     *  Returns the full oid string.
     * @param int &$type
     *  Returns the object type.
     * @param string $abbrevOid
     *  The object prefix ID to lookup.
     *
     * @return string
     *  The object contents.
     */
    public function read_prefix(&$fullOid,&$type,string $abbrevOid) : string|bool {
        $i = 0;
        $keys = array_keys($this->storage);
        while ($i < count($keys)) {
            $oid = $keys[$i++];
            $prefix = substr($oid,0,strlen($abbrevOid));
            if ($prefix == $abbrevOid) {
                $fullOid = $oid;
                $type = $this->storage[$oid]['t'];
                return $this->storage[$oid]['d'];
            }
        }

        return false;
    }

    /**
     * Implements GitODBBackend::read_header().
     *
     * @param int &$size
     *  Returns the content length of the object.
     * @param int &$type
     *  Returns the object type.
     * @param string $oid
     *  The object ID.
     *
     * @return bool
     *  Returns whether the header was found or not.
     */
    public function read_header(&$size,&$type,string $oid) : bool {
        if (!isset($this->storage[$oid])) {
            return false;
        }

        $size = strlen($this->storage[$oid]['d']);
        $type = strlen($this->storage[$oid]['t']);

        return true;
    }

    /**
     * Implements GitODBBackend::write().
     *
     * @param string $oid
     *  The OID of the object to write.
     * @param string $data
     *  The object contents.
     * @param int $type
     *  The type of the objects.
     */
    public function write(string $oid,string $data,int $type) : void {
        $this->storage[$oid] = [
            't' => $type,
            'd' => $data,
        ];
    }

    /**
     * Implements GitODBBackend::exists().
     *
     * @param string $oid
     *  The OID of the object to write.
     *
     * @return bool
     */
    public function exists(string $oid) : bool {
        return isset($this->storage[$oid]);
    }

    /**
     * Implements GitODBBackend::exists_prefix().
     *
     * @param string &$fullOid
     *  Returns the full OID if matched.
     * @param string $prefix
     *  The OID prefix to search.
     *
     * @return bool
     */
    public function exists_prefix(&$fullOid,string $prefix) : bool {
        $i = 0;
        $keys = array_keys($this->storage);
        while ($i < count($keys)) {
            $oid = $keys[$i++];
            $cand = substr($oid,0,strlen($prefix));
            if ($cand == $prefix) {
                $fullOid = $oid;
                return true;
            }
        }

        return false;
    }

    /**
     * Implements GitODBBackend::for_each().
     *
     * @param callable $callback
     *  The callback to execute for each object.
     * @param mixed $payload
     *  The payload to pass through.
     */
    public function for_each(callable $callback,mixed $payload = null) : void {
        foreach (array_keys($this->storage) as $oid) {
            if ($callback($oid,$payload) === false) {
                break;
            }
        }
    }
}
