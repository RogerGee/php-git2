/*
 * php-git2.h
 *
 * This file is a part of php-git.
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
#include <zend_exceptions.h>
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
}

// Include any C++ standard libraries.
#include <string>
#include <exception>

// Module globals
ZEND_BEGIN_MODULE_GLOBALS(git2)

ZEND_END_MODULE_GLOBALS(git2)
ZEND_EXTERN_MODULE_GLOBALS(git2)

namespace php_git2
{

    class php_git2_exception:
        public std::exception
    {
    public:
        php_git2_exception(const char* format, ...);

        virtual const char* what() const noexcept
        { return message.c_str(); }
    private:
        std::string message;
    };

} // namespace php_git2

#endif

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
