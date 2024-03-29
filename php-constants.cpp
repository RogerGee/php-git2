/*
 * php-constants.cpp
 *
 * Copyright (C) Roger P. Gee
 */

#include "php-git2.h"
extern "C" {
#include <git2/sys/merge.h>
}
using namespace php_git2;

// Simplify constant registration.

#define PHP_GIT2_CONSTANT(name)                                     \
    REGISTER_LONG_CONSTANT(#name,name,CONST_CS|CONST_PERSISTENT)

#define PHP_GIT2_CONSTANT_S(name)                                   \
    REGISTER_STRING_CONSTANT(#name,const_cast<char*>(name),CONST_CS|CONST_PERSISTENT)

void php_git2::php_git2_register_constants(int module_number)
{
    // GIT_*
    PHP_GIT2_CONSTANT(GIT_ERROR);
    PHP_GIT2_CONSTANT(GIT_ITEROVER);
    PHP_GIT2_CONSTANT(GIT_OK);
    PHP_GIT2_CONSTANT(GIT_PASSTHROUGH);
    PHP_GIT2_CONSTANT(GIT_RETRY);

    // GIT_E*
    PHP_GIT2_CONSTANT(GIT_EAMBIGUOUS);
    PHP_GIT2_CONSTANT(GIT_EAPPLIED);
    PHP_GIT2_CONSTANT(GIT_EAPPLYFAIL);
    PHP_GIT2_CONSTANT(GIT_EAUTH);
    PHP_GIT2_CONSTANT(GIT_EBAREREPO);
    PHP_GIT2_CONSTANT(GIT_EBUFS);
    PHP_GIT2_CONSTANT(GIT_ECERTIFICATE);
    PHP_GIT2_CONSTANT(GIT_ECONFLICT);
    PHP_GIT2_CONSTANT(GIT_EDIRECTORY);
    PHP_GIT2_CONSTANT(GIT_EEOF);
    PHP_GIT2_CONSTANT(GIT_EEXISTS);
    PHP_GIT2_CONSTANT(GIT_EINDEXDIRTY);
    PHP_GIT2_CONSTANT(GIT_EINVALID);
    PHP_GIT2_CONSTANT(GIT_EINVALIDSPEC);
    PHP_GIT2_CONSTANT(GIT_ELOCKED);
    PHP_GIT2_CONSTANT(GIT_EMERGECONFLICT);
    PHP_GIT2_CONSTANT(GIT_EMISMATCH);
    PHP_GIT2_CONSTANT(GIT_EMODIFIED);
    PHP_GIT2_CONSTANT(GIT_ENONFASTFORWARD);
    PHP_GIT2_CONSTANT(GIT_ENOTFOUND);
    PHP_GIT2_CONSTANT(GIT_EOWNER);
    PHP_GIT2_CONSTANT(GIT_EPEEL);
    PHP_GIT2_CONSTANT(GIT_EUNBORNBRANCH);
    PHP_GIT2_CONSTANT(GIT_EUNCOMMITTED);
    PHP_GIT2_CONSTANT(GIT_EUNMERGED);
    PHP_GIT2_CONSTANT(GIT_EUSER);

    // GIT_ERROR_*
	PHP_GIT2_CONSTANT(GIT_ERROR_NONE);
	PHP_GIT2_CONSTANT(GIT_ERROR_NOMEMORY);
	PHP_GIT2_CONSTANT(GIT_ERROR_OS);
	PHP_GIT2_CONSTANT(GIT_ERROR_INVALID);
	PHP_GIT2_CONSTANT(GIT_ERROR_REFERENCE);
	PHP_GIT2_CONSTANT(GIT_ERROR_ZLIB);
	PHP_GIT2_CONSTANT(GIT_ERROR_REPOSITORY);
	PHP_GIT2_CONSTANT(GIT_ERROR_CONFIG);
	PHP_GIT2_CONSTANT(GIT_ERROR_REGEX);
	PHP_GIT2_CONSTANT(GIT_ERROR_ODB);
	PHP_GIT2_CONSTANT(GIT_ERROR_INDEX);
	PHP_GIT2_CONSTANT(GIT_ERROR_OBJECT);
	PHP_GIT2_CONSTANT(GIT_ERROR_NET);
	PHP_GIT2_CONSTANT(GIT_ERROR_TAG);
	PHP_GIT2_CONSTANT(GIT_ERROR_TREE);
	PHP_GIT2_CONSTANT(GIT_ERROR_INDEXER);
	PHP_GIT2_CONSTANT(GIT_ERROR_SSL);
	PHP_GIT2_CONSTANT(GIT_ERROR_SUBMODULE);
	PHP_GIT2_CONSTANT(GIT_ERROR_THREAD);
	PHP_GIT2_CONSTANT(GIT_ERROR_STASH);
	PHP_GIT2_CONSTANT(GIT_ERROR_CHECKOUT);
	PHP_GIT2_CONSTANT(GIT_ERROR_FETCHHEAD);
	PHP_GIT2_CONSTANT(GIT_ERROR_MERGE);
	PHP_GIT2_CONSTANT(GIT_ERROR_SSH);
	PHP_GIT2_CONSTANT(GIT_ERROR_FILTER);
	PHP_GIT2_CONSTANT(GIT_ERROR_REVERT);
	PHP_GIT2_CONSTANT(GIT_ERROR_CALLBACK);
	PHP_GIT2_CONSTANT(GIT_ERROR_CHERRYPICK);
	PHP_GIT2_CONSTANT(GIT_ERROR_DESCRIBE);
	PHP_GIT2_CONSTANT(GIT_ERROR_REBASE);
	PHP_GIT2_CONSTANT(GIT_ERROR_FILESYSTEM);
	PHP_GIT2_CONSTANT(GIT_ERROR_PATCH);
	PHP_GIT2_CONSTANT(GIT_ERROR_WORKTREE);
	PHP_GIT2_CONSTANT(GIT_ERROR_SHA);
	PHP_GIT2_CONSTANT(GIT_ERROR_HTTP);
	PHP_GIT2_CONSTANT(GIT_ERROR_INTERNAL);

    // GIT_FEATURE_*
    PHP_GIT2_CONSTANT(GIT_FEATURE_HTTPS);
    PHP_GIT2_CONSTANT(GIT_FEATURE_NSEC);
    PHP_GIT2_CONSTANT(GIT_FEATURE_SSH);
    PHP_GIT2_CONSTANT(GIT_FEATURE_THREADS);

    // GIT_OBJ_*
    PHP_GIT2_CONSTANT(GIT_OBJ_ANY);
    PHP_GIT2_CONSTANT(GIT_OBJ_BAD);
    PHP_GIT2_CONSTANT(GIT_OBJ_BLOB);
    PHP_GIT2_CONSTANT(GIT_OBJ_COMMIT);
    PHP_GIT2_CONSTANT(GIT_OBJ_OFS_DELTA);
    PHP_GIT2_CONSTANT(GIT_OBJ_REF_DELTA);
    PHP_GIT2_CONSTANT(GIT_OBJ_TAG);
    PHP_GIT2_CONSTANT(GIT_OBJ_TREE);
    PHP_GIT2_CONSTANT(GIT_OBJ__EXT1);
    PHP_GIT2_CONSTANT(GIT_OBJ__EXT2);

    // GIT_SORT_*
    PHP_GIT2_CONSTANT(GIT_SORT_NONE);
    PHP_GIT2_CONSTANT(GIT_SORT_REVERSE);
    PHP_GIT2_CONSTANT(GIT_SORT_TIME);
    PHP_GIT2_CONSTANT(GIT_SORT_TOPOLOGICAL);

    // GIT_PATH_LIST_SEPARATOR
    PHP_GIT2_CONSTANT(GIT_PATH_LIST_SEPARATOR);

    // GIT_FILEMODE_*
    PHP_GIT2_CONSTANT(GIT_FILEMODE_BLOB);
    PHP_GIT2_CONSTANT(GIT_FILEMODE_BLOB_EXECUTABLE);
    PHP_GIT2_CONSTANT(GIT_FILEMODE_COMMIT);
    PHP_GIT2_CONSTANT(GIT_FILEMODE_LINK);
    PHP_GIT2_CONSTANT(GIT_FILEMODE_TREE);
    PHP_GIT2_CONSTANT(GIT_FILEMODE_UNREADABLE);

    // GIT_TREEWALK_*
    PHP_GIT2_CONSTANT(GIT_TREEWALK_POST);
    PHP_GIT2_CONSTANT(GIT_TREEWALK_PRE);

    // GIT_BLAME_*
    PHP_GIT2_CONSTANT(GIT_BLAME_FIRST_PARENT);
    PHP_GIT2_CONSTANT(GIT_BLAME_IGNORE_WHITESPACE);
    PHP_GIT2_CONSTANT(GIT_BLAME_NORMAL);
    PHP_GIT2_CONSTANT(GIT_BLAME_TRACK_COPIES_ANY_COMMIT_COPIES);
    PHP_GIT2_CONSTANT(GIT_BLAME_TRACK_COPIES_SAME_COMMIT_COPIES);
    PHP_GIT2_CONSTANT(GIT_BLAME_TRACK_COPIES_SAME_COMMIT_MOVES);
    PHP_GIT2_CONSTANT(GIT_BLAME_TRACK_COPIES_SAME_FILE);
    PHP_GIT2_CONSTANT(GIT_BLAME_USE_MAILMAP);

    // GIT_REVPARSE_*
    PHP_GIT2_CONSTANT(GIT_REVPARSE_MERGE_BASE);
    PHP_GIT2_CONSTANT(GIT_REVPARSE_RANGE);
    PHP_GIT2_CONSTANT(GIT_REVPARSE_SINGLE);

    // GIT_BRANCH_*
    PHP_GIT2_CONSTANT(GIT_BRANCH_ALL);
    PHP_GIT2_CONSTANT(GIT_BRANCH_LOCAL);
    PHP_GIT2_CONSTANT(GIT_BRANCH_REMOTE);

    // GIT_REF_FORMAT_*
    PHP_GIT2_CONSTANT(GIT_REF_FORMAT_ALLOW_ONELEVEL);
    PHP_GIT2_CONSTANT(GIT_REF_FORMAT_NORMAL);
    PHP_GIT2_CONSTANT(GIT_REF_FORMAT_REFSPEC_PATTERN);
    PHP_GIT2_CONSTANT(GIT_REF_FORMAT_REFSPEC_SHORTHAND);

    // GIT_REF_*
    PHP_GIT2_CONSTANT(GIT_REF_INVALID);
    PHP_GIT2_CONSTANT(GIT_REF_LISTALL);
    PHP_GIT2_CONSTANT(GIT_REF_OID);
    PHP_GIT2_CONSTANT(GIT_REF_SYMBOLIC);

    // GIT_REPOSITORY_INIT_*
    PHP_GIT2_CONSTANT(GIT_REPOSITORY_INIT_BARE);
    PHP_GIT2_CONSTANT(GIT_REPOSITORY_INIT_EXTERNAL_TEMPLATE);
    PHP_GIT2_CONSTANT(GIT_REPOSITORY_INIT_MKDIR);
    PHP_GIT2_CONSTANT(GIT_REPOSITORY_INIT_MKPATH);
    PHP_GIT2_CONSTANT(GIT_REPOSITORY_INIT_NO_DOTGIT_DIR);
    PHP_GIT2_CONSTANT(GIT_REPOSITORY_INIT_NO_REINIT);
    PHP_GIT2_CONSTANT(GIT_REPOSITORY_INIT_RELATIVE_GITLINK);
    PHP_GIT2_CONSTANT(GIT_REPOSITORY_INIT_SHARED_ALL);
    PHP_GIT2_CONSTANT(GIT_REPOSITORY_INIT_SHARED_GROUP);
    PHP_GIT2_CONSTANT(GIT_REPOSITORY_INIT_SHARED_UMASK);

    // GIT_REPOSITORY_OPEN_*
    PHP_GIT2_CONSTANT(GIT_REPOSITORY_OPEN_BARE);
    PHP_GIT2_CONSTANT(GIT_REPOSITORY_OPEN_CROSS_FS);
    PHP_GIT2_CONSTANT(GIT_REPOSITORY_OPEN_FROM_ENV);
    PHP_GIT2_CONSTANT(GIT_REPOSITORY_OPEN_NO_DOTGIT);
    PHP_GIT2_CONSTANT(GIT_REPOSITORY_OPEN_NO_SEARCH);

    // GIT_CONFIG_*
    PHP_GIT2_CONSTANT(GIT_CONFIG_HIGHEST_LEVEL);
    PHP_GIT2_CONSTANT(GIT_CONFIG_LEVEL_APP);
    PHP_GIT2_CONSTANT(GIT_CONFIG_LEVEL_GLOBAL);
    PHP_GIT2_CONSTANT(GIT_CONFIG_LEVEL_LOCAL);
    PHP_GIT2_CONSTANT(GIT_CONFIG_LEVEL_PROGRAMDATA);
    PHP_GIT2_CONSTANT(GIT_CONFIG_LEVEL_SYSTEM);
    PHP_GIT2_CONSTANT(GIT_CONFIG_LEVEL_XDG);

    // GIT_CLONE_LOCAL_*
    PHP_GIT2_CONSTANT(GIT_CLONE_LOCAL_AUTO);
    PHP_GIT2_CONSTANT(GIT_CLONE_LOCAL);
    PHP_GIT2_CONSTANT(GIT_CLONE_NO_LOCAL);
    PHP_GIT2_CONSTANT(GIT_CLONE_LOCAL_NO_LINKS);

    // GIT_CHECKOUT_*
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_ALLOW_CONFLICTS);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_CONFLICT_STYLE_DIFF3);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_CONFLICT_STYLE_MERGE);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_CONFLICT_STYLE_ZDIFF3);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_DISABLE_PATHSPEC_MATCH);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_DONT_OVERWRITE_IGNORED);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_DONT_REMOVE_EXISTING);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_DONT_UPDATE_INDEX);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_DONT_WRITE_INDEX);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_DRY_RUN);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_FORCE);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_NONE);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_NO_REFRESH);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_NOTIFY_ALL);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_NOTIFY_CONFLICT);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_NOTIFY_DIRTY);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_NOTIFY_IGNORED);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_NOTIFY_NONE);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_NOTIFY_UNTRACKED);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_NOTIFY_UPDATED);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_OPTIONS_VERSION);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_RECREATE_MISSING);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_REMOVE_IGNORED);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_REMOVE_UNTRACKED);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_SAFE);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_SKIP_LOCKED_DIRECTORIES);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_SKIP_UNMERGED);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_UPDATE_ONLY);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_UPDATE_SUBMODULES);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_UPDATE_SUBMODULES_IF_CHANGED);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_USE_OURS);
    PHP_GIT2_CONSTANT(GIT_CHECKOUT_USE_THEIRS);

    // GIT_CONFIGMAP_*
    PHP_GIT2_CONSTANT(GIT_CONFIGMAP_FALSE);
    PHP_GIT2_CONSTANT(GIT_CONFIGMAP_TRUE);
    PHP_GIT2_CONSTANT(GIT_CONFIGMAP_INT32);
    PHP_GIT2_CONSTANT(GIT_CONFIGMAP_STRING);

    // GIT_DIFF_*
    PHP_GIT2_CONSTANT(GIT_DIFF_DISABLE_PATHSPEC_MATCH);
    PHP_GIT2_CONSTANT(GIT_DIFF_ENABLE_FAST_UNTRACKED_DIRS);
    PHP_GIT2_CONSTANT(GIT_DIFF_FLAG_BINARY);
    PHP_GIT2_CONSTANT(GIT_DIFF_FLAG_EXISTS);
    PHP_GIT2_CONSTANT(GIT_DIFF_FLAG_NOT_BINARY);
    PHP_GIT2_CONSTANT(GIT_DIFF_FLAG_VALID_ID);
    PHP_GIT2_CONSTANT(GIT_DIFF_FLAG_VALID_SIZE);
    PHP_GIT2_CONSTANT(GIT_DIFF_FORCE_BINARY);
    PHP_GIT2_CONSTANT(GIT_DIFF_FORCE_TEXT);
    PHP_GIT2_CONSTANT(GIT_DIFF_IGNORE_BLANK_LINES);
    PHP_GIT2_CONSTANT(GIT_DIFF_IGNORE_CASE);
    PHP_GIT2_CONSTANT(GIT_DIFF_IGNORE_FILEMODE);
    PHP_GIT2_CONSTANT(GIT_DIFF_IGNORE_SUBMODULES);
    PHP_GIT2_CONSTANT(GIT_DIFF_IGNORE_WHITESPACE);
    PHP_GIT2_CONSTANT(GIT_DIFF_IGNORE_WHITESPACE_CHANGE);
    PHP_GIT2_CONSTANT(GIT_DIFF_IGNORE_WHITESPACE_EOL);
    PHP_GIT2_CONSTANT(GIT_DIFF_INCLUDE_CASECHANGE);
    PHP_GIT2_CONSTANT(GIT_DIFF_INCLUDE_IGNORED);
    PHP_GIT2_CONSTANT(GIT_DIFF_INCLUDE_TYPECHANGE);
    PHP_GIT2_CONSTANT(GIT_DIFF_INCLUDE_TYPECHANGE_TREES);
    PHP_GIT2_CONSTANT(GIT_DIFF_INCLUDE_UNMODIFIED);
    PHP_GIT2_CONSTANT(GIT_DIFF_INCLUDE_UNREADABLE);
    PHP_GIT2_CONSTANT(GIT_DIFF_INCLUDE_UNREADABLE_AS_UNTRACKED);
    PHP_GIT2_CONSTANT(GIT_DIFF_INCLUDE_UNTRACKED);
    PHP_GIT2_CONSTANT(GIT_DIFF_INDENT_HEURISTIC);
    PHP_GIT2_CONSTANT(GIT_DIFF_MINIMAL);
    PHP_GIT2_CONSTANT(GIT_DIFF_NORMAL);
    PHP_GIT2_CONSTANT(GIT_DIFF_PATIENCE);
    PHP_GIT2_CONSTANT(GIT_DIFF_RECURSE_IGNORED_DIRS);
    PHP_GIT2_CONSTANT(GIT_DIFF_RECURSE_UNTRACKED_DIRS);
    PHP_GIT2_CONSTANT(GIT_DIFF_REVERSE);
    PHP_GIT2_CONSTANT(GIT_DIFF_SHOW_BINARY);
    PHP_GIT2_CONSTANT(GIT_DIFF_SHOW_UNMODIFIED);
    PHP_GIT2_CONSTANT(GIT_DIFF_SHOW_UNTRACKED_CONTENT);
    PHP_GIT2_CONSTANT(GIT_DIFF_SKIP_BINARY_CHECK);
    PHP_GIT2_CONSTANT(GIT_DIFF_UPDATE_INDEX);

    // GIT_DIFF_BINARY_*
    PHP_GIT2_CONSTANT(GIT_DIFF_BINARY_DELTA);
    PHP_GIT2_CONSTANT(GIT_DIFF_BINARY_LITERAL);
    PHP_GIT2_CONSTANT(GIT_DIFF_BINARY_NONE);

    // GIT_DIFF_LINE_*
    PHP_GIT2_CONSTANT(GIT_DIFF_LINE_ADDITION);
    PHP_GIT2_CONSTANT(GIT_DIFF_LINE_ADD_EOFNL);
    PHP_GIT2_CONSTANT(GIT_DIFF_LINE_BINARY);
    PHP_GIT2_CONSTANT(GIT_DIFF_LINE_CONTEXT);
    PHP_GIT2_CONSTANT(GIT_DIFF_LINE_CONTEXT_EOFNL);
    PHP_GIT2_CONSTANT(GIT_DIFF_LINE_DELETION);
    PHP_GIT2_CONSTANT(GIT_DIFF_LINE_DEL_EOFNL);
    PHP_GIT2_CONSTANT(GIT_DIFF_LINE_FILE_HDR);
    PHP_GIT2_CONSTANT(GIT_DIFF_LINE_HUNK_HDR);

    // GIT_DIFF_FORMAT_EMAIL_*
    PHP_GIT2_CONSTANT(GIT_DIFF_FORMAT_EMAIL_NONE);
    PHP_GIT2_CONSTANT(GIT_DIFF_FORMAT_EMAIL_EXCLUDE_SUBJECT_PATCH_MARKER);

    // GIT_DIFF_FORMAT_*
    PHP_GIT2_CONSTANT(GIT_DIFF_FORMAT_NAME_ONLY);
    PHP_GIT2_CONSTANT(GIT_DIFF_FORMAT_NAME_STATUS);
    PHP_GIT2_CONSTANT(GIT_DIFF_FORMAT_PATCH);
    PHP_GIT2_CONSTANT(GIT_DIFF_FORMAT_PATCH_HEADER);
    PHP_GIT2_CONSTANT(GIT_DIFF_FORMAT_PATCH_ID);
    PHP_GIT2_CONSTANT(GIT_DIFF_FORMAT_RAW);

    // GIT_DIFF_STATS_*
    PHP_GIT2_CONSTANT(GIT_DIFF_STATS_FULL);
    PHP_GIT2_CONSTANT(GIT_DIFF_STATS_INCLUDE_SUMMARY);
    PHP_GIT2_CONSTANT(GIT_DIFF_STATS_NONE);
    PHP_GIT2_CONSTANT(GIT_DIFF_STATS_NUMBER);
    PHP_GIT2_CONSTANT(GIT_DIFF_STATS_SHORT);

    // GIT_DELTA_*
    PHP_GIT2_CONSTANT(GIT_DELTA_ADDED);
    PHP_GIT2_CONSTANT(GIT_DELTA_CONFLICTED);
    PHP_GIT2_CONSTANT(GIT_DELTA_COPIED);
    PHP_GIT2_CONSTANT(GIT_DELTA_DELETED);
    PHP_GIT2_CONSTANT(GIT_DELTA_IGNORED);
    PHP_GIT2_CONSTANT(GIT_DELTA_MODIFIED);
    PHP_GIT2_CONSTANT(GIT_DELTA_RENAMED);
    PHP_GIT2_CONSTANT(GIT_DELTA_TYPECHANGE);
    PHP_GIT2_CONSTANT(GIT_DELTA_UNMODIFIED);
    PHP_GIT2_CONSTANT(GIT_DELTA_UNREADABLE);
    PHP_GIT2_CONSTANT(GIT_DELTA_UNTRACKED);

    // GIT_IDXENTRY_*
    PHP_GIT2_CONSTANT(GIT_IDXENTRY_ADDED);
    PHP_GIT2_CONSTANT(GIT_IDXENTRY_CONFLICTED);
    PHP_GIT2_CONSTANT(GIT_IDXENTRY_EXTENDED);
    PHP_GIT2_CONSTANT(GIT_IDXENTRY_EXTENDED2);
    PHP_GIT2_CONSTANT(GIT_IDXENTRY_EXTENDED_FLAGS);
    PHP_GIT2_CONSTANT(GIT_IDXENTRY_HASHED);
    PHP_GIT2_CONSTANT(GIT_IDXENTRY_INTENT_TO_ADD);
    PHP_GIT2_CONSTANT(GIT_IDXENTRY_NAMEMASK);
    PHP_GIT2_CONSTANT(GIT_IDXENTRY_NEW_SKIP_WORKTREE);
    PHP_GIT2_CONSTANT(GIT_IDXENTRY_REMOVE);
    PHP_GIT2_CONSTANT(GIT_IDXENTRY_SKIP_WORKTREE);
    PHP_GIT2_CONSTANT(GIT_IDXENTRY_STAGEMASK);
    PHP_GIT2_CONSTANT(GIT_IDXENTRY_STAGESHIFT);
    PHP_GIT2_CONSTANT(GIT_IDXENTRY_UNHASHED);
    PHP_GIT2_CONSTANT(GIT_IDXENTRY_UNPACKED);
    PHP_GIT2_CONSTANT(GIT_IDXENTRY_UPDATE);
    PHP_GIT2_CONSTANT(GIT_IDXENTRY_UPTODATE);
    PHP_GIT2_CONSTANT(GIT_IDXENTRY_VALID);
    PHP_GIT2_CONSTANT(GIT_IDXENTRY_WT_REMOVE);

    // GIT_INDEXCAP_*
    PHP_GIT2_CONSTANT(GIT_INDEXCAP_FROM_OWNER);
    PHP_GIT2_CONSTANT(GIT_INDEXCAP_IGNORE_CASE);
    PHP_GIT2_CONSTANT(GIT_INDEXCAP_NO_FILEMODE);
    PHP_GIT2_CONSTANT(GIT_INDEXCAP_NO_SYMLINKS);

    // GIT_INDEX_ADD_*
    PHP_GIT2_CONSTANT(GIT_INDEX_ADD_CHECK_PATHSPEC);
    PHP_GIT2_CONSTANT(GIT_INDEX_ADD_DEFAULT);
    PHP_GIT2_CONSTANT(GIT_INDEX_ADD_DISABLE_PATHSPEC_MATCH);
    PHP_GIT2_CONSTANT(GIT_INDEX_ADD_FORCE);

    // GIT_TRACE_*
    PHP_GIT2_CONSTANT(GIT_TRACE_DEBUG);
    PHP_GIT2_CONSTANT(GIT_TRACE_ERROR);
    PHP_GIT2_CONSTANT(GIT_TRACE_FATAL);
    PHP_GIT2_CONSTANT(GIT_TRACE_INFO);
    PHP_GIT2_CONSTANT(GIT_TRACE_NONE);
    PHP_GIT2_CONSTANT(GIT_TRACE_TRACE);
    PHP_GIT2_CONSTANT(GIT_TRACE_WARN);

    // GIT_ATTR_*
    PHP_GIT2_CONSTANT(GIT_ATTR_CHECK_FILE_THEN_INDEX);
    PHP_GIT2_CONSTANT(GIT_ATTR_CHECK_INDEX_ONLY);
    PHP_GIT2_CONSTANT(GIT_ATTR_CHECK_INDEX_THEN_FILE);
    PHP_GIT2_CONSTANT(GIT_ATTR_CHECK_NO_SYSTEM);
    PHP_GIT2_CONSTANT(GIT_ATTR_FALSE_T);
    PHP_GIT2_CONSTANT(GIT_ATTR_TRUE_T);
    PHP_GIT2_CONSTANT(GIT_ATTR_UNSPECIFIED_T);
    PHP_GIT2_CONSTANT(GIT_ATTR_VALUE_T);

    // GIT_STATUS_OPT_*
    PHP_GIT2_CONSTANT(GIT_STATUS_OPT_DEFAULTS);
    PHP_GIT2_CONSTANT(GIT_STATUS_OPT_DISABLE_PATHSPEC_MATCH);
    PHP_GIT2_CONSTANT(GIT_STATUS_OPT_EXCLUDE_SUBMODULES);
    PHP_GIT2_CONSTANT(GIT_STATUS_OPT_INCLUDE_IGNORED);
    PHP_GIT2_CONSTANT(GIT_STATUS_OPT_INCLUDE_UNMODIFIED);
    PHP_GIT2_CONSTANT(GIT_STATUS_OPT_INCLUDE_UNREADABLE);
    PHP_GIT2_CONSTANT(GIT_STATUS_OPT_INCLUDE_UNREADABLE_AS_UNTRACKED);
    PHP_GIT2_CONSTANT(GIT_STATUS_OPT_INCLUDE_UNTRACKED);
    PHP_GIT2_CONSTANT(GIT_STATUS_OPT_NO_REFRESH);
    PHP_GIT2_CONSTANT(GIT_STATUS_OPT_RECURSE_IGNORED_DIRS);
    PHP_GIT2_CONSTANT(GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS);
    PHP_GIT2_CONSTANT(GIT_STATUS_OPT_RENAMES_FROM_REWRITES);
    PHP_GIT2_CONSTANT(GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX);
    PHP_GIT2_CONSTANT(GIT_STATUS_OPT_RENAMES_INDEX_TO_WORKDIR);
    PHP_GIT2_CONSTANT(GIT_STATUS_OPT_SORT_CASE_INSENSITIVELY);
    PHP_GIT2_CONSTANT(GIT_STATUS_OPT_SORT_CASE_SENSITIVELY);
    PHP_GIT2_CONSTANT(GIT_STATUS_OPT_UPDATE_INDEX);

    // GIT_STATUS_SHOW_*
    PHP_GIT2_CONSTANT(GIT_STATUS_SHOW_INDEX_AND_WORKDIR);
    PHP_GIT2_CONSTANT(GIT_STATUS_SHOW_INDEX_ONLY);
    PHP_GIT2_CONSTANT(GIT_STATUS_SHOW_WORKDIR_ONLY);

    // GIT_STATUS_*
    PHP_GIT2_CONSTANT(GIT_STATUS_CONFLICTED);
    PHP_GIT2_CONSTANT(GIT_STATUS_CURRENT);
    PHP_GIT2_CONSTANT(GIT_STATUS_IGNORED);
    PHP_GIT2_CONSTANT(GIT_STATUS_INDEX_DELETED);
    PHP_GIT2_CONSTANT(GIT_STATUS_INDEX_MODIFIED);
    PHP_GIT2_CONSTANT(GIT_STATUS_INDEX_NEW);
    PHP_GIT2_CONSTANT(GIT_STATUS_INDEX_RENAMED);
    PHP_GIT2_CONSTANT(GIT_STATUS_INDEX_TYPECHANGE);
    PHP_GIT2_CONSTANT(GIT_STATUS_WT_DELETED);
    PHP_GIT2_CONSTANT(GIT_STATUS_WT_MODIFIED);
    PHP_GIT2_CONSTANT(GIT_STATUS_WT_NEW);
    PHP_GIT2_CONSTANT(GIT_STATUS_WT_RENAMED);
    PHP_GIT2_CONSTANT(GIT_STATUS_WT_TYPECHANGE);
    PHP_GIT2_CONSTANT(GIT_STATUS_WT_UNREADABLE);

    // GIT_MERGE_*
    PHP_GIT2_CONSTANT(GIT_MERGE_ANALYSIS_FASTFORWARD);
    PHP_GIT2_CONSTANT(GIT_MERGE_ANALYSIS_NONE);
    PHP_GIT2_CONSTANT(GIT_MERGE_ANALYSIS_NORMAL);
    PHP_GIT2_CONSTANT(GIT_MERGE_ANALYSIS_UNBORN);
    PHP_GIT2_CONSTANT(GIT_MERGE_ANALYSIS_UP_TO_DATE);
    PHP_GIT2_CONSTANT(GIT_MERGE_FAIL_ON_CONFLICT);
    PHP_GIT2_CONSTANT(GIT_MERGE_FILE_ACCEPT_CONFLICTS);
    PHP_GIT2_CONSTANT(GIT_MERGE_FILE_DEFAULT);
    PHP_GIT2_CONSTANT(GIT_MERGE_FILE_DIFF_MINIMAL);
    PHP_GIT2_CONSTANT(GIT_MERGE_FILE_DIFF_PATIENCE);
    PHP_GIT2_CONSTANT(GIT_MERGE_FILE_FAVOR_NORMAL);
    PHP_GIT2_CONSTANT(GIT_MERGE_FILE_FAVOR_OURS);
    PHP_GIT2_CONSTANT(GIT_MERGE_FILE_FAVOR_THEIRS);
    PHP_GIT2_CONSTANT(GIT_MERGE_FILE_FAVOR_UNION);
    PHP_GIT2_CONSTANT(GIT_MERGE_FILE_IGNORE_WHITESPACE);
    PHP_GIT2_CONSTANT(GIT_MERGE_FILE_IGNORE_WHITESPACE_CHANGE);
    PHP_GIT2_CONSTANT(GIT_MERGE_FILE_IGNORE_WHITESPACE_EOL);
    PHP_GIT2_CONSTANT(GIT_MERGE_FILE_INPUT_VERSION);
    PHP_GIT2_CONSTANT(GIT_MERGE_FILE_OPTIONS_VERSION);
    PHP_GIT2_CONSTANT(GIT_MERGE_FILE_SIMPLIFY_ALNUM);
    PHP_GIT2_CONSTANT(GIT_MERGE_FILE_STYLE_DIFF3);
    PHP_GIT2_CONSTANT(GIT_MERGE_FILE_STYLE_MERGE);
    PHP_GIT2_CONSTANT(GIT_MERGE_FILE_STYLE_ZDIFF3);
    PHP_GIT2_CONSTANT(GIT_MERGE_FIND_RENAMES);
    PHP_GIT2_CONSTANT(GIT_MERGE_NO_RECURSIVE);
    PHP_GIT2_CONSTANT(GIT_MERGE_VIRTUAL_BASE);
    PHP_GIT2_CONSTANT(GIT_MERGE_PREFERENCE_FASTFORWARD_ONLY);
    PHP_GIT2_CONSTANT(GIT_MERGE_PREFERENCE_NONE);
    PHP_GIT2_CONSTANT(GIT_MERGE_PREFERENCE_NO_FASTFORWARD);
    PHP_GIT2_CONSTANT(GIT_MERGE_SKIP_REUC);
    PHP_GIT2_CONSTANT_S(GIT_MERGE_DRIVER_BINARY);
    PHP_GIT2_CONSTANT_S(GIT_MERGE_DRIVER_TEXT);
    PHP_GIT2_CONSTANT_S(GIT_MERGE_DRIVER_UNION);

    // GIT_DESCRIBE_*
    PHP_GIT2_CONSTANT(GIT_DESCRIBE_ALL);
    PHP_GIT2_CONSTANT(GIT_DESCRIBE_DEFAULT);
    PHP_GIT2_CONSTANT(GIT_DESCRIBE_DEFAULT_ABBREVIATED_SIZE);
    PHP_GIT2_CONSTANT(GIT_DESCRIBE_DEFAULT_MAX_CANDIDATES_TAGS);
    PHP_GIT2_CONSTANT(GIT_DESCRIBE_TAGS);

    // GIT_RESET_*
    PHP_GIT2_CONSTANT(GIT_RESET_HARD);
    PHP_GIT2_CONSTANT(GIT_RESET_MIXED);
    PHP_GIT2_CONSTANT(GIT_RESET_SOFT);

    // GIT_STASH_*
    PHP_GIT2_CONSTANT(GIT_STASH_DEFAULT);
    PHP_GIT2_CONSTANT(GIT_STASH_INCLUDE_IGNORED);
    PHP_GIT2_CONSTANT(GIT_STASH_INCLUDE_UNTRACKED);
    PHP_GIT2_CONSTANT(GIT_STASH_KEEP_INDEX);

    // GIT_STASH_APPLY_*
    PHP_GIT2_CONSTANT(GIT_STASH_APPLY_DEFAULT);
    PHP_GIT2_CONSTANT(GIT_STASH_APPLY_REINSTATE_INDEX);

    // GIT_STASH_APPLY_PROGRESS_*
    PHP_GIT2_CONSTANT(GIT_STASH_APPLY_PROGRESS_ANALYZE_INDEX);
    PHP_GIT2_CONSTANT(GIT_STASH_APPLY_PROGRESS_ANALYZE_MODIFIED);
    PHP_GIT2_CONSTANT(GIT_STASH_APPLY_PROGRESS_ANALYZE_UNTRACKED);
    PHP_GIT2_CONSTANT(GIT_STASH_APPLY_PROGRESS_CHECKOUT_MODIFIED);
    PHP_GIT2_CONSTANT(GIT_STASH_APPLY_PROGRESS_CHECKOUT_UNTRACKED);
    PHP_GIT2_CONSTANT(GIT_STASH_APPLY_PROGRESS_DONE);
    PHP_GIT2_CONSTANT(GIT_STASH_APPLY_PROGRESS_LOADING_STASH);
    PHP_GIT2_CONSTANT(GIT_STASH_APPLY_PROGRESS_NONE);

    // GIT_CERT_*
    PHP_GIT2_CONSTANT(GIT_CERT_HOSTKEY_LIBSSH2);
    PHP_GIT2_CONSTANT(GIT_CERT_NONE);
    PHP_GIT2_CONSTANT(GIT_CERT_SSH_MD5);
    PHP_GIT2_CONSTANT(GIT_CERT_SSH_RAW);
    PHP_GIT2_CONSTANT(GIT_CERT_SSH_RAW_TYPE_DSS);
    PHP_GIT2_CONSTANT(GIT_CERT_SSH_RAW_TYPE_KEY_ECDSA_256);
    PHP_GIT2_CONSTANT(GIT_CERT_SSH_RAW_TYPE_KEY_ECDSA_384);
    PHP_GIT2_CONSTANT(GIT_CERT_SSH_RAW_TYPE_KEY_ECDSA_521);
    PHP_GIT2_CONSTANT(GIT_CERT_SSH_RAW_TYPE_KEY_ED25519);
    PHP_GIT2_CONSTANT(GIT_CERT_SSH_RAW_TYPE_RSA);
    PHP_GIT2_CONSTANT(GIT_CERT_SSH_RAW_TYPE_UNKNOWN);
    PHP_GIT2_CONSTANT(GIT_CERT_SSH_SHA1);
    PHP_GIT2_CONSTANT(GIT_CERT_SSH_SHA256);
    PHP_GIT2_CONSTANT(GIT_CERT_STRARRAY);
    PHP_GIT2_CONSTANT(GIT_CERT_X509);

    // GIT_PROXY_*
    PHP_GIT2_CONSTANT(GIT_PROXY_AUTO);
    PHP_GIT2_CONSTANT(GIT_PROXY_NONE);
    PHP_GIT2_CONSTANT(GIT_PROXY_SPECIFIED);

    // GIT_REMOTE_DOWNLOAD_TAGS_*
    PHP_GIT2_CONSTANT(GIT_REMOTE_DOWNLOAD_TAGS_ALL);
    PHP_GIT2_CONSTANT(GIT_REMOTE_DOWNLOAD_TAGS_AUTO);
    PHP_GIT2_CONSTANT(GIT_REMOTE_DOWNLOAD_TAGS_NONE);
    PHP_GIT2_CONSTANT(GIT_REMOTE_DOWNLOAD_TAGS_UNSPECIFIED);

    // GIT_FETCH_*
    PHP_GIT2_CONSTANT(GIT_FETCH_NO_PRUNE);
    PHP_GIT2_CONSTANT(GIT_FETCH_PRUNE);
    PHP_GIT2_CONSTANT(GIT_FETCH_PRUNE_UNSPECIFIED);

    // GIT_CREDTYPE_*
    PHP_GIT2_CONSTANT(GIT_CREDTYPE_DEFAULT);
    PHP_GIT2_CONSTANT(GIT_CREDTYPE_SSH_CUSTOM);
    PHP_GIT2_CONSTANT(GIT_CREDTYPE_SSH_INTERACTIVE);
    PHP_GIT2_CONSTANT(GIT_CREDTYPE_SSH_KEY);
    PHP_GIT2_CONSTANT(GIT_CREDTYPE_SSH_MEMORY);
    PHP_GIT2_CONSTANT(GIT_CREDTYPE_USERNAME);
    PHP_GIT2_CONSTANT(GIT_CREDTYPE_USERPASS_PLAINTEXT);

    // GIT_DIRECTION_*
    PHP_GIT2_CONSTANT(GIT_DIRECTION_FETCH);
    PHP_GIT2_CONSTANT(GIT_DIRECTION_PUSH);

    // GIT_SUBMODULE_*
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_IGNORE_ALL);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_IGNORE_DIRTY);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_IGNORE_NONE);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_IGNORE_UNSPECIFIED);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_IGNORE_UNTRACKED);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_RECURSE_NO);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_RECURSE_ONDEMAND);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_RECURSE_YES);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_STATUS_INDEX_ADDED);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_STATUS_INDEX_DELETED);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_STATUS_INDEX_MODIFIED);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_STATUS_IN_CONFIG);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_STATUS_IN_HEAD);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_STATUS_IN_INDEX);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_STATUS_IN_WD);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_STATUS_WD_ADDED);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_STATUS_WD_DELETED);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_STATUS_WD_INDEX_MODIFIED);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_STATUS_WD_MODIFIED);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_STATUS_WD_UNINITIALIZED);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_STATUS_WD_UNTRACKED);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_STATUS_WD_WD_MODIFIED);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_STATUS__INDEX_FLAGS);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_STATUS__IN_FLAGS);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_STATUS__WD_FLAGS);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_UPDATE_CHECKOUT);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_UPDATE_DEFAULT);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_UPDATE_MERGE);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_UPDATE_NONE);
    PHP_GIT2_CONSTANT(GIT_SUBMODULE_UPDATE_REBASE);

    // GIT_WORKTREE_PRUNE_*
    PHP_GIT2_CONSTANT(GIT_WORKTREE_PRUNE_LOCKED);
    PHP_GIT2_CONSTANT(GIT_WORKTREE_PRUNE_VALID);
    PHP_GIT2_CONSTANT(GIT_WORKTREE_PRUNE_WORKING_TREE);
}
