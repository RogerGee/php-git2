/*
 * php-git2.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_GIT2_H
#define PHPGIT2_GIT2_H

// Include PHP headers.
extern "C" {
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <php.h>
#include <zend.h>
#include <zend_exceptions.h>
#include <zend_interfaces.h>
#include <ext/standard/php_smart_str.h>
#include <ext/spl/spl_exceptions.h>
#include <ext/standard/php_var.h>
#include <ext/standard/php_string.h>
#include <ext/standard/php_incomplete_class.h>
#include <ext/standard/info.h>
#include <ext/standard/php_array.h>
}

#define PHP_GIT2_EXTNAME "git2"
#define PHP_GIT2_EXTVER  "0.0.0"

// Include the libgit2 headers. They should be installed on the system already.
extern "C" {
#include <git2.h>
#include <git2/sys/diff.h>
}

// Include any C/C++ standard libraries.
#include <string>
#include <exception>
#include <cstring>

// Module globals
ZEND_BEGIN_MODULE_GLOBALS(git2)

ZEND_END_MODULE_GLOBALS(git2)
ZEND_EXTERN_MODULE_GLOBALS(git2)

// Provide macros for handling functionality for threaded PHP builds.

#ifdef ZTS
#define ZTS_CONSTRUCTOR(type) type(TSRMLS_D) {}
#define ZTS_CONSTRUCTOR_WITH_BASE(type,base) type(TSRMLS_D): base(TSRMLS_C) {}
#define ZTS_MEMBER_C(obj) obj.TSRMLS_C
#define ZTS_MEMBER_CC(obj) , obj.TSRMLS_C
#define ZTS_MEMBER_PC(obj) obj->TSRMLS_C
#define ZTS_MEMBER_PCC(obj) , obj->TSRMLS_C

// We provide a macro that allows us to optionally pass TSRMLS_C to a
// constructor.
#define ZTS_CTOR (TSRMLS_C)

#else

#define ZTS_CONSTRUCTOR(type)
#define ZTS_CONSTRUCTOR_WITH_BASE(type,base)
#define ZTS_MEMBER_C(obj)
#define ZTS_MEMBER_CC(obj)
#define ZTS_MEMBER_PC(obj)
#define ZTS_MEMBER_PCC(obj)

// Make ZTS_CTOR insert nothing. It cannot be "( )" because C++ resolves
// ambiguity in parsing 'T X()' in favor of a function declaration.
#define ZTS_CTOR

#endif

#define UNUSED(var) ((void)var)

#define add_assoc_const_string(zv,key,str)      \
    add_assoc_string_ex(zv,key,sizeof(key),(char*)str,1)

namespace php_git2
{

    // Provide a base exception type for identifying exceptions we care to
    // handle.

    class php_git2_exception_base:
        public std::exception
    {
    public:
        php_git2_exception_base():
            code(0) {}

        // The error code to report to userspace.
        int code;
    };

    // Provide an exception type to be thrown by git2 function wrappers when
    // they fail. This will allow userspace to handle these errors.

    class php_git2_exception:
        public php_git2_exception_base
    {
    public:
        php_git2_exception(const char* format, ...);

        virtual const char* what() const noexcept
        { return message.c_str(); }
    private:
        std::string message;
    };

    // Provide an exception type to be thrown when a PHP exception is to be
    // propagated through.

    class php_git2_propagated_exception:
        public php_git2_exception_base
    {
    public:
        // Return NULL pointer to indicate that the exception propagates
        // through.
        virtual const char* what() const noexcept
        { return nullptr; }
    };

    // Provide a function to handle a generic libgit2 error. It is generic for
    // any possible return type. However we specialize it for int to handle all
    // cases. We want to preserve the numeric return code so we can expose it to
    // userspace.

    template<typename T>
    void git_error(T t)
    {
        // Default to GIT_ERROR if the return_type is not 'int'.

        git_error<int>(GIT_ERROR);
    }

    template<>
    void git_error(int code);

    // Function to register constants defined in php-constants.cpp.

    void php_git2_register_constants(int module_number TSRMLS_DC);

    // Helper functions for converting git2 values to PHP values.

    void convert_oid_fromstr(git_oid* dest,const char* src,int srclen);
    void convert_transfer_progress(zval* zv,const git_transfer_progress* stats);
    void convert_blame_hunk(zval* zv,const git_blame_hunk* hunk);
    void convert_diff_delta(zval* zv,const git_diff_delta* delta);
    void convert_diff_file(zval* zv,const git_diff_file* file);
    void convert_diff_binary(zval* zv,const git_diff_binary* binary);
    void convert_diff_binary_file(zval* zv,const git_diff_binary_file* file);
    void convert_diff_hunk(zval* zv,const git_diff_hunk* hunk);
    void convert_diff_line(zval* zv,const git_diff_line* line);
    void convert_diff_perfdata(zval* zv,const git_diff_perfdata* perfdata);
    void convert_signature(zval* zv,const git_signature* sig);

    // Helper functions for converting PHP values to git2 values.

    git_signature* convert_signature(zval* zv);

} // namespace php_git2

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
