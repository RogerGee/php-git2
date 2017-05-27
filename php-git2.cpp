/*
 * php-git2.cpp
 *
 * This file is a part of php-git2.
 */

#include "php-git2.h"
#include "php-function.h"
#include "php-callback.h"
#include "php-object.h"
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
#include <cstdio>
#include <cstdarg>
using namespace std;
using namespace php_git2;

ZEND_DECLARE_MODULE_GLOBALS(git2)

// Extension setup functions defined in this unit.
static PHP_GINIT_FUNCTION(git2);
static PHP_GSHUTDOWN_FUNCTION(git2);
static PHP_MINIT_FUNCTION(git2);
static PHP_RINIT_FUNCTION(git2);
static PHP_MINFO_FUNCTION(git2);
static PHP_MSHUTDOWN_FUNCTION(git2);
static PHP_RSHUTDOWN_FUNCTION(git2);

// Exported extension functions defined in this unit.
PHP_FUNCTION(git_libgit2_version);
PHP_FUNCTION(git2_version);

// Functions exported by this extension into PHP.
static zend_function_entry php_git2_functions[] = {
    // Functions that do not directly wrap libgit2 exports:
    PHP_FE(git2_version,NULL)

    // General libgit2 functions:
    PHP_FE(git_libgit2_version,NULL)
    PHP_GIT2_FE(git_libgit2_features,
        (zif_php_git2_function<
            func_wrapper<int>::func<git_libgit2_features>,
            local_pack<>,
            0 >),
        NULL)

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
    PHP_FE_END
};

// Module entry table.
zend_module_entry git2_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_GIT2_EXTNAME,
    php_git2_functions,
    PHP_MINIT(git2),
    PHP_MSHUTDOWN(git2),
    PHP_RINIT(git2),
    PHP_RSHUTDOWN(git2),
    PHP_MINFO(git2),
#if ZEND_MODULE_API_NO >= 20010901
    PHP_GIT2_EXTVER,
#endif
    PHP_MODULE_GLOBALS(git2),
    PHP_GINIT(git2),
    PHP_GSHUTDOWN(git2),
    NULL,
    STANDARD_MODULE_PROPERTIES_EX
};

#ifdef COMPILE_DL_GIT2
ZEND_GET_MODULE(git2)
#endif

// Implementation of internal functions.

PHP_GINIT_FUNCTION(git2)
{
}

PHP_GSHUTDOWN_FUNCTION(git2)
{
}

#define PHP_GIT2_CONSTANT(name) \
    REGISTER_LONG_CONSTANT(#name,name,CONST_CS|CONST_PERSISTENT)

PHP_MINIT_FUNCTION(git2)
{
    // Initialize git2 library.
    git_libgit2_init();

    // Call the function to register all resource types. Whenever a resource
    // type is added, the libgit2 data type name should be added to the list of
    // template parameters.
    php_git2_define_resource_types<
        git_repository,
        git_reference,
        git_object,
        git_revwalk,
        git_packbuilder,
        git_indexer,
        git_odb,
        git_odb_object,
        git_commit,
        git_blob,
        git_tree,
        git_tree_entry >(module_number);

    // Register all classes provided by this extension.
    php_git2_register_classes(TSRMLS_C);

    // Register libgit2 constants:

    // GIT_*
    PHP_GIT2_CONSTANT(GIT_OK);
    PHP_GIT2_CONSTANT(GIT_ERROR);

    // GIT_FEATURE_*
    PHP_GIT2_CONSTANT(GIT_FEATURE_THREADS);
    PHP_GIT2_CONSTANT(GIT_FEATURE_HTTPS);
    PHP_GIT2_CONSTANT(GIT_FEATURE_SSH);
    PHP_GIT2_CONSTANT(GIT_FEATURE_NSEC);

    // GIT_OBJ_*
    PHP_GIT2_CONSTANT(GIT_OBJ_ANY);
    PHP_GIT2_CONSTANT(GIT_OBJ_BAD);
    PHP_GIT2_CONSTANT(GIT_OBJ__EXT1);
    PHP_GIT2_CONSTANT(GIT_OBJ_COMMIT);
    PHP_GIT2_CONSTANT(GIT_OBJ_TREE);
    PHP_GIT2_CONSTANT(GIT_OBJ_BLOB);
    PHP_GIT2_CONSTANT(GIT_OBJ_TAG);
    PHP_GIT2_CONSTANT(GIT_OBJ__EXT2);
    PHP_GIT2_CONSTANT(GIT_OBJ_OFS_DELTA);
    PHP_GIT2_CONSTANT(GIT_OBJ_REF_DELTA);

    // GIT_SORT_*
    PHP_GIT2_CONSTANT(GIT_SORT_NONE);
    PHP_GIT2_CONSTANT(GIT_SORT_TOPOLOGICAL);
    PHP_GIT2_CONSTANT(GIT_SORT_TIME);
    PHP_GIT2_CONSTANT(GIT_SORT_REVERSE);

    // GIT_PATH_LIST_SEPARATOR
    PHP_GIT2_CONSTANT(GIT_PATH_LIST_SEPARATOR);

    // GIT_FILEMODE_*
    PHP_GIT2_CONSTANT(GIT_FILEMODE_UNREADABLE);
    PHP_GIT2_CONSTANT(GIT_FILEMODE_TREE);
    PHP_GIT2_CONSTANT(GIT_FILEMODE_BLOB);
    PHP_GIT2_CONSTANT(GIT_FILEMODE_BLOB_EXECUTABLE);
    PHP_GIT2_CONSTANT(GIT_FILEMODE_LINK);
    PHP_GIT2_CONSTANT(GIT_FILEMODE_COMMIT);

    return SUCCESS;
}

PHP_RINIT_FUNCTION(git2)
{
    return SUCCESS;
}

PHP_MINFO_FUNCTION(git2)
{
    char buf[128];
    int major, minor, rev;

    git_libgit2_version(&major,&minor,&rev);
    snprintf(buf,sizeof(buf),"%d.%d.%d",major,minor,rev);

    php_info_print_table_start();
    php_info_print_table_row(2,PHP_GIT2_EXTNAME,"enabled");
    php_info_print_table_row(2,"extension version",PHP_GIT2_EXTVER);
    php_info_print_table_row(2,"libgit2 version",buf);
    php_info_print_table_end();

    DISPLAY_INI_ENTRIES();
}

PHP_MSHUTDOWN_FUNCTION(git2)
{
    // Deinitialize libgit2. At this point, all resources should have been
    // freed. This means they would call their destructors and all libgit2
    // memory should be freed.
    git_libgit2_shutdown();

    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(git2)
{
    return SUCCESS;
}

PHP_FUNCTION(git_libgit2_version)
{
    char buf[128];
    int major, minor, rev;

    git_libgit2_version(&major,&minor,&rev);
    snprintf(buf,sizeof(buf),"%d.%d.%d",major,minor,rev);

    RETURN_STRING(buf,1);
}

PHP_FUNCTION(git2_version)
{
    char buf[128];
    int major, minor, rev;

    git_libgit2_version(&major,&minor,&rev);
    snprintf(buf,sizeof(buf),"%s %s (libgit2 %d.%d.%d)",PHP_GIT2_EXTNAME,PHP_GIT2_EXTVER,
        major,minor,rev);

    RETURN_STRING(buf,1);
}

// php_git2_exception

php_git2_exception::php_git2_exception(const char* format, ...)
    : message(4096,0)
{
    va_list args;
    va_start(args,format);
    vsnprintf(&message[0],message.size(),format,args);
    va_end(args);
}

// php_git2::git_error()

void php_git2::git_error()
{
    const ::git_error* err = giterr_last();
    if (err == nullptr) {
        throw php_git2_exception("libgit2 no error?");
    }
    throw php_git2_exception("libgit2 error: (%d): %s",err->klass,err->message);
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
