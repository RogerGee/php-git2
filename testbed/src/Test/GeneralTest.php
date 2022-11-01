?php

namespace PhpGit2\Test;

use PhpGit2\RepositoryTestCase;

/**
 * @phpGitRemoved git_buf_contains_nul
 * @phpGitRemoved git_buf_free
 * @phpGitRemoved git_buf_grow
 * @phpGitRemoved git_buf_is_binary
 * @phpGitRemoved git_buf_set
 * @phpGitRemoved git_hashsig_compare
 * @phpGitRemoved git_hashsig_create
 * @phpGitRemoved git_hashsig_create_fromfile
 * @phpGitRemoved git_hashsig_free
 * @phpGitRemoved git_libgit2_init
 * @phpGitRemoved git_libgit2_opts
 * @phpGitRemoved git_libgit2_shutdown
 * @phpGitRemoved git_oidarray_free
 * @phpGitRemoved git_oid_cmp
 * @phpGitRemoved git_oid_cpy
 * @phpGitRemoved git_oid_equal
 * @phpGitRemoved git_oid_fmt
 * @phpGitRemoved git_oid_fromraw
 * @phpGitRemoved git_oid_fromstr
 * @phpGitRemoved git_oid_fromstrn
 * @phpGitRemoved git_oid_fromstrp
 * @phpGitRemoved git_oid_iszero
 * @phpGitRemoved git_oid_ncmp
 * @phpGitRemoved git_oid_nfmt
 * @phpGitRemoved git_oid_pathfmt
 * @phpGitRemoved git_oid_shorten_add
 * @phpGitRemoved git_oid_shorten_free
 * @phpGitRemoved git_oid_shorten_new
 * @phpGitRemoved git_oid_strcmp
 * @phpGitRemoved git_oid_streq
 * @phpGitRemoved git_oid_tostr
 * @phpGitRemoved git_oid_tostr_s
 * @phpGitRemoved git_openssl_set_locking
 * @phpGitRemoved git_proxy_init_options
 * @phpGitRemoved git_push_init_options
 * @phpGitRemoved git_strarray_copy
 * @phpGitRemoved git_strarray_free
 * @phpGitRemoved git_stream_register_tls
 * @phpGitRemoved git_transaction_commit
 * @phpGitRemoved git_transaction_free
 * @phpGitRemoved git_transaction_lock_ref
 * @phpGitRemoved git_transaction_new
 * @phpGitRemoved git_transaction_remove
 * @phpGitRemoved git_transaction_set_reflog
 * @phpGitRemoved git_transaction_set_symbolic_target
 * @phpGitRemoved git_transaction_set_target
 */
final class GeneralTest extends RepositoryTestCase {
    public function testLifetime() {
        $repo = static::getRepository();

        $ref = git_reference_lookup($repo,'refs/heads/master');
        git_repository_free($repo);

        $name = git_reference_name($ref);

        $this->assertTrue(true);
    }
}
