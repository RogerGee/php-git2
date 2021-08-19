/*
 * php-git2.h
 *
 * Copyright (C) Roger P. Gee
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
#include <ext/spl/spl_exceptions.h>
#include <ext/standard/php_var.h>
#include <ext/standard/php_string.h>
#include <ext/standard/php_incomplete_class.h>
#include <ext/standard/info.h>
#include <ext/standard/php_array.h>
#undef lookup
}

#define PHP_GIT2_EXTNAME "git2"
#define PHP_GIT2_EXTVER  "1.0.0-php7-dev"

// Include the libgit2 headers. They should be installed on the system already.
extern "C" {
#include <git2.h>
#include <git2/trace.h>
#include <git2/sys/diff.h>
}

// Include any C/C++ standard libraries.
#include <string>
#include <exception>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <cstdarg>

// Module globals

ZEND_BEGIN_MODULE_GLOBALS(git2)
  bool propagateError;
  bool requestActive;
ZEND_END_MODULE_GLOBALS(git2)
ZEND_EXTERN_MODULE_GLOBALS(git2)

#ifdef ZTS
#include "TSRM.h"
#define GIT2_G(v)  TSRMG(git2_globals_id,zend_git2_globals*,v)
#else
#define GIT2_G(v)  (git2_globals.v)
#endif

#define UNUSED(var) ((void)var)

// Create a macro for managing bailout context regions.

#define BAILOUT_ENTER_REGION(ctx)                   \
    ctx.enterRegion( SETJMP(ctx.jumpbuf()) == 0 )

// Create custom git error values. These values must go outside of the range of
// errors defined in the libgit2 "errors.h" header.

#define GIT_EPHP_RANGE_START    -30000

#define GIT_EPHP                -30000
#define GIT_EPHP_ERROR          -30001
#define GIT_EPHP_PROP           -30002
#define GIT_EPHP_PROP_BAILOUT   -30003

namespace php_git2
{
    // List of all functions provided in the php-git2-fe compilation unit.
    extern zend_function_entry functions[];

    // Provide types for tracking bailouts and manipulating PHP bailout jump
    // buffers.

    class php_bailer
    {
    public:
        php_bailer():
            flag(false)
        {
        }

        ~php_bailer()
        {
            if (flag || GIT2_G(propagateError)) {
                bailout();
            }
        }

        void set()
        {
            flag = true;
        }

        void handled()
        {
            flag = false;
        }

        static void bailout()
        {
            _zend_bailout(const_cast<char*>(__FILE__),__LINE__);
        }

    private:
        bool flag;
    };

    class php_bailout_context
    {
    public:
        php_bailout_context(php_bailer& theBailer):
            original(EG(bailout)), bailer(theBailer)
        {
            EG(bailout) = &current;
        }

        ~php_bailout_context()
        {
            EG(bailout) = original;
        }

        JMP_BUF& jumpbuf()
        {
            return current;
        }

        bool enterRegion(bool result)
        {
            if (!result) {
                bailer.set();
            }

            return result;
        }

    private:
        php_bailout_context(const php_bailout_context&) = delete;
        php_bailout_context& operator =(const php_bailout_context&) = delete;

        JMP_BUF* original;
        JMP_BUF current;
        php_bailer& bailer;
    };

    // Provide a base exception type for identifying exceptions we care to
    // handle.

    class php_git2_exception_base:
        public std::exception
    {
    public:
        php_git2_exception_base():
            code(0)
        {
        }

        // Member function that handles the exception in some well-defined way
        // according to the exception type.
        virtual void handle() const noexcept = 0;

        // The error code to report to userspace.
        int code;
    };

    class php_git2_exception__with_message:
        public php_git2_exception_base
    {
    public:
        php_git2_exception__with_message();
        php_git2_exception__with_message(const php_git2_exception__with_message& inst);
        ~php_git2_exception__with_message();

        virtual const char* what() const noexcept
        {
            return buf->buf;
        }

        php_git2_exception__with_message& operator =(const php_git2_exception__with_message& inst);

    protected:
        void set_message(const char* format,va_list args)
        {
            vsnprintf(buf->buf,buf->size,format,args);
        }

    private:
        struct buffer
        {
            buffer():
                size(1024), ref(1)
            {
                buf = reinterpret_cast<char*>(emalloc(size));
            }

            ~buffer()
            {
                efree(buf);
            }

            char* buf;
            size_t size;
            int ref;
        };

        buffer* buf;
    };

    // Provide an exception type to be thrown by git2 function wrappers when
    // they fail. This will allow userspace to handle these errors as PHP
    // exceptions.

    class php_git2_exception:
        public php_git2_exception__with_message
    {
    public:
        php_git2_exception(const char* format, ...);

        virtual void handle() const noexcept
        {
            // Transform the exception into a PHP exception.
            zend_throw_exception(nullptr,what(),code);
        }
    };

    // Provide exception types to be thrown when PHP exceptions/errors are to be
    // propagated through.

    class php_git2_propagated_exception:
        public php_git2_exception_base
    {
    public:
        virtual const char* what() const noexcept
        {
            // Return NULL pointer to indicate that the exception propagates
            // through.
            return nullptr;
        }

        virtual void handle() const noexcept
        {
        }
    };

    class php_git2_propagated_bailout_exception:
        public php_git2_propagated_exception
    {
    public:
        virtual void handle() const noexcept
        {
            php_bailer::bailout();
        }
    };

    // Provide an exception type for generating error exceptions.

    class php_git2_error_exception:
        public php_git2_exception__with_message
    {
    public:
        php_git2_error_exception(const char* format, ...);

        virtual void handle() const noexcept
        {
            zend_throw_exception(nullptr,what(),0);
        }
    };

    // Provide a type to facilitate converting PHP exceptions.

    class php_exception_wrapper
    {
    public:
        php_exception_wrapper()
        {
            zend_object* ob = EG(exception);
            if (ob != nullptr) {
                ZVAL_OBJ(&zex,ob);
            }
            else {
                ZVAL_UNDEF(&zex);
            }
        }

        const char* get_message() const;

        bool has_exception() const
        {
            return Z_TYPE(zex) != IS_UNDEF;
        }

        void handle()
        {
            zend_clear_exception();
            ZVAL_UNDEF(&zex);
        }

        void set_giterr() const;
        void throw_error() const;

    private:
        php_exception_wrapper(const php_exception_wrapper&) = delete;
        php_exception_wrapper& operator =(const php_exception_wrapper&) = delete;

        zval zex;
    };

    // Provide a function to handle a generic libgit2 error. It is generic for
    // any possible return type. However we specialize it for int to handle all
    // cases. We want to preserve the numeric return code so we can expose it to
    // userspace.
    //
    // NOTE: this function throws a php_git2_exception_base object.

    template<typename T>
    void git_error(T t)
    {
        // Default to GIT_ERROR if the return_type is not 'int'.

        git_error<int>(GIT_ERROR);
    }

    template<>
    void git_error(int code);

    // Provide a wrapper around giterr_set_str that allows printf-style format
    // strings.

    void php_git2_giterr_set(int errorClass,const char* format, ...);

    // Function to register constants defined in php-constants.cpp.

    void php_git2_register_constants(int module_number);

    // Functions to create/initialize module globals.

    void php_git2_globals_ctor(zend_git2_globals* gbls);
    void php_git2_globals_dtor(zend_git2_globals* gbls);
    void php_git2_globals_init();
    void php_git2_globals_request_init();
    void php_git2_globals_request_shutdown();

    // Helper functions for converting git2 values to PHP values.

    int convert_oid_fromstr(git_oid* dest,const char* src,size_t srclen);
    void convert_oid(zval* zv,const git_oid* oid);
    void convert_oid_prefix(zval* zv,const git_oid* prefix,size_t len);
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
    void convert_index_entry(zval* zv,const git_index_entry* ent);
    void convert_index_time(zval* zv,const git_index_time* tv);
    void convert_status_entry(zval* zv,const git_status_entry *ent);
    void convert_merge_file_result(zval* zv,const git_merge_file_result* res);
    void convert_reflog_entry(zval* zv,const git_reflog_entry* ent);
    void convert_reflog(zval* zv,const git_reflog* log);
    void convert_rebase_operation(zval* zv,const git_rebase_operation* oper);
    void convert_cert(zval* zv,const git_cert* cert);
    void convert_push_update(zval* zv,const git_push_update* up);
    void convert_remote_head(zval* zv,const git_remote_head* head);

    // Helper functions for converting PHP values to git2 values.

    git_signature* convert_signature(zval* zv);
    int convert_transfer_progress(git_transfer_progress& stats,zval* zv);

} // namespace php_git2

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
