#include "php-git2.h"
#include "repository.h"
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
    PHP_FE(git_libgit2_version,NULL)
    PHP_FE(git2_version,NULL)
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

PHP_MINIT_FUNCTION(git2)
{
    // Call the function to register all resource types. Whenever a resource
    // type is added, the libgit2 data type name should be added to the list of
    // template parameters.
    php_git2_define_resource_types<
        git_repository >(module_number);

    php_git2_define_resource_types(module_number);

    return SUCCESS;
}

PHP_RINIT_FUNCTION(git2)
{
    git_libgit2_init();
    return SUCCESS;
}

PHP_MINFO_FUNCTION(git2)
{
    char buf[128];
    int major, minor, rev;

    git_libgit2_version(&major,&minor,&rev);
    snprintf(buf,sizeof(buf),"%d.%d.%d",major,minor,rev);

    php_info_print_table_start();
    php_info_print_table_header(2,PHP_GIT2_EXTNAME,"enabled");
    php_info_print_table_header(2,"extension version",PHP_GIT2_EXTVER);
    php_info_print_table_header(2,"libgit2 version",buf);
    php_info_print_table_end();

    DISPLAY_INI_ENTRIES();
}

PHP_MSHUTDOWN_FUNCTION(git2)
{
    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(git2)
{
    git_libgit2_shutdown();
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

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
