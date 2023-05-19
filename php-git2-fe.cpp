/*
 * php-git2-fe.cpp
 *
 * Copyright (C) Roger P. Gee
 */

#include "php-git2.h"
#include "php-function.h"
#include "php-object.h"
#include "php-rethandler.h"
#include "stubs/tmp.h"
#include "repository.h"
#include "reference.h"
#include "object.h"
#include "revwalk.h"
#include "packbuilder.h"
#include "indexer.h"
#include "odb.h"
#include "commit.h"
#include "blob.h"
#include "tree.h"
#include "signature.h"
#include "treebuilder.h"
#include "blame.h"
#include "revparse.h"
#include "annotated.h"
#include "branch.h"
#include "config-git2.h"
#include "checkout.h"
#include "tag.h"
#include "diff.h"
#include "index.h"
#include "trace.h"
#include "ignore.h"
#include "attr.h"
#include "status.h"
#include "cherrypick.h"
#include "merge.h"
#include "note.h"
#include "reflog.h"
#include "refdb.h"
#include "patch.h"
#include "describe.h"
#include "rebase.h"
#include "reset.h"
#include "revert.h"
#include "stash.h"
#include "remote.h"
#include "refspec.h"
#include "cred.h"
#include "clone.h"
#include "submodule.h"
#include "worktree.h"

// Exported extension functions defined in this unit.
static PHP_FUNCTION(git_libgit2_version);
static PHP_FUNCTION(git2_version);

// Functions exported by this extension into PHP.
zend_function_entry php_git2::functions[] = {
    // Functions that do not directly wrap libgit2 exports:
    PHP_FE(git2_version,arginfo_git2_version)

    // General libgit2 functions:
    PHP_FE(git_libgit2_version,arginfo_git_libgit2_version)
    PHP_GIT2_FE_EX(git_libgit2_prerelease,
        (zif_php_git2_function<
            func_wrapper<const char*>::func<git_libgit2_prerelease>,
            local_pack<>,
            0 >),
        arginfo_git_libgit2_prerelease)
    PHP_GIT2_FE_EX(git_libgit2_features,
        (zif_php_git2_function<
            func_wrapper<int>::func<git_libgit2_features>,
            local_pack<>,
            0 >),
        arginfo_git_libgit2_features)

    // Include template specializations for the different library wrappers. The
    // compiler will instantiate these into this compilation unit.
    GIT_REPOSITORY_FE
    GIT_REFERENCE_FE
    GIT_OBJECT_FE
    GIT_REVWALK_FE
    GIT_PACKBUILDER_FE
    GIT_INDEXER_FE
    GIT_ODB_FE
    GIT_COMMIT_FE
    GIT_BLOB_FE
    GIT_TREE_FE
    GIT_SIGNATURE_FE
    GIT_TREEBUILDER_FE
    GIT_BLAME_FE
    GIT_REVPARSE_FE
    GIT_ANNOTATED_FE
    GIT_BRANCH_FE
    GIT_CONFIG_FE
    GIT_CHECKOUT_FE
    GIT_TAG_FE
    GIT_DIFF_FE
    GIT_INDEX_FE
    GIT_TRACE_FE
    GIT_IGNORE_FE
    GIT_ATTR_FE
    GIT_STATUS_FE
    GIT_CHERRYPICK_FE
    GIT_MERGE_FE
    GIT_NOTE_FE
    GIT_REFLOG_FE
    GIT_REFDB_FE
    GIT_PATCH_FE
    GIT_DESCRIBE_FE
    GIT_REBASE_FE
    GIT_RESET_FE
    GIT_REVERT_FE
    GIT_STASH_FE
    GIT_REMOTE_FE
    GIT_REFSPEC_FE
    GIT_CRED_FE
    GIT_CLONE_FE
    GIT_SUBMODULE_FE
    GIT_WORKTREE_FE
    PHP_FE_END
};

PHP_FUNCTION(git_libgit2_version)
{
    char buf[128];
    int major, minor, rev;

    git_libgit2_version(&major,&minor,&rev);
    snprintf(buf,sizeof(buf),"%d.%d.%d",major,minor,rev);

    RETURN_STRING(buf);
}

PHP_FUNCTION(git2_version)
{
    char buf[128];
    int major, minor, rev;
    const char* prerelease;

    git_libgit2_version(&major,&minor,&rev);
    prerelease = git_libgit2_prerelease();
    if (prerelease != nullptr) {
        snprintf(
            buf,
            sizeof(buf),
            "%s %s (libgit2 %d.%d.%d-%s)",
            PHP_GIT2_EXTNAME,
            PHP_GIT2_EXTVER,
            major,
            minor,
            rev,
            prerelease);
    }
    else {
        snprintf(
            buf,
            sizeof(buf),
            "%s %s (libgit2 %d.%d.%d)",
            PHP_GIT2_EXTNAME,
            PHP_GIT2_EXTVER,
            major,
            minor,
            rev);
    }

    RETURN_STRING(buf);
}
