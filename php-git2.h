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
#include <ext/standard/php_smart_str.h>
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
  bool propagateFatalError;
  bool requestActive;
ZEND_END_MODULE_GLOBALS(git2)
ZEND_EXTERN_MODULE_GLOBALS(git2)

#ifdef ZTS
#include "TSRM.h"
#define GIT2_G(v)  TSRMG(git2_globals_id,zend_git2_globals*,v)
#else
#define GIT2_G(v)  (git2_globals.v)
#endif

// Provide macros for handling functionality for threaded PHP builds.

#ifdef ZTS
#define ZTS_CONSTRUCTOR(type) type(TSRMLS_D) {}
#define ZTS_CONSTRUCTOR_WITH_BASE(type,base) type(TSRMLS_D): base(TSRMLS_C) {}
#define ZTS_MEMBER_C(obj) obj.TSRMLS_C
#define ZTS_MEMBER_CC(obj) , obj.TSRMLS_C
#define ZTS_MEMBER_PC(obj) obj->TSRMLS_C
#define ZTS_MEMBER_PCC(obj) , obj->TSRMLS_C
#define ZTS_MEMBER_EXTRACT(obj) TSRMLS_D = obj.TSRMLS_C

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
#define ZTS_MEMBER_EXTRACT(obj)

// Make ZTS_CTOR insert nothing. It cannot be "( )" because C++ resolves
// ambiguity in parsing 'T X()' in favor of a function declaration.
#define ZTS_CTOR

#endif

#define UNUSED(var) ((void)var)

#define add_assoc_const_string(zv,key,str)                  \
    add_assoc_string_ex(zv,key,sizeof(key),(char*)str,1)

// Create a macro for managing bailout context regions.

#define BAILOUT_ENTER_REGION(ctx)                   \
    ctx.enterRegion( SETJMP(ctx.jumpbuf()) == 0 )

// Create custom git error values. These values must go outside of the range of
// errors defined in the libgit2 "errors.h" header.

#define GIT_EPHPRANGE_START   -30000

#define GIT_EPHPFATAL         -30000
#define GIT_EPHPEXPROP        -30001
#define GIT_EPHPFATALPROP     -30002

namespace php_git2
{
    // List of all functions provided in the php-git2-fe compilation unit.
    extern zend_function_entry functions[];

    // Provide a class to manage passing copies of ZTS handles around.

    class php_zts_base
    {
#ifdef ZTS
    protected:
        php_zts_base(TSRMLS_D):
            TSRMLS_C(TSRMLS_C)
        {
        }

    public:
        TSRMLS_D;
#endif
    };

    class php_zts_base_fetched
    {
#ifdef ZTS
    protected:
        php_zts_base_fetched()
        {
            TSRMLS_FETCH();
            this->TSRMLS_C = TSRMLS_C;
        }

    public:
        TSRMLS_D;
#endif
    };

    class php_zts_member:
        public php_zts_base
    {
#ifdef ZTS
    public:
        php_zts_member(void*** zts):
            php_zts_base(zts)
        {
        }
#endif
    };


    // Provide types for tracking bailouts and manipulating PHP bailout jump
    // buffers.

    class php_bailer:
        private php_zts_base
    {
    public:
        php_bailer(TSRMLS_D):
            php_zts_base(TSRMLS_C), flag(false)
        {
        }

        ~php_bailer()
        {
            if (flag || GIT2_G(propagateFatalError)) {
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

    class php_bailout_context:
        private php_zts_base
    {
    public:
        php_bailout_context(php_bailer& theBailer TSRMLS_DC):
            php_zts_base(TSRMLS_C), original(EG(bailout)), bailer(theBailer)
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
        virtual void handle(TSRMLS_D) const noexcept = 0;

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

        virtual void handle(TSRMLS_D) const noexcept
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
        // Return NULL pointer to indicate that the exception propagates
        // through.
        virtual const char* what() const noexcept
        {
            return nullptr;
        }

        virtual void handle(TSRMLS_D) const noexcept
        {
        }
    };

    class php_git2_fatal_propagated:
        public php_git2_exception_base
    {
    public:
        // Return NULL pointer to indicate that the exception propagates
        // through.
        virtual const char* what() const noexcept
        {
            return nullptr;
        }

        virtual void handle(TSRMLS_D) const noexcept
        {
            // Bailout since an error is already set and was propagated through.
            php_bailer::bailout();
        }
    };

    // Provide an exception type for generating fatal PHP errors.

    class php_git2_fatal_exception:
        public php_git2_exception__with_message
    {
    public:
        php_git2_fatal_exception(const char* format, ...);

        virtual void handle(TSRMLS_D) const noexcept
        {
            // Generate a fatal PHP error.
            php_error(E_ERROR,"%s",what());
        }
    };

    // Provide a type to facilitate converting PHP exceptions.

    class php_exception_wrapper:
        private php_zts_base
    {
    public:
        php_exception_wrapper(TSRMLS_D):
            php_zts_base(TSRMLS_C), zex(EG(exception))
        {
        }

        bool has_exception() const
        {
            return zex != nullptr;
        }

        void handle()
        {
            zend_clear_exception(TSRMLS_C);
        }

        void set_giterr() const;
        void throw_php_git2_exception() const;
    private:
        php_exception_wrapper(const php_exception_wrapper&) = delete;
        php_exception_wrapper& operator =(const php_exception_wrapper&) = delete;

        zval* zex;
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

    // Provide a function used for issuing a PHP warning from the latest libgit2
    // error. The error state is cleared after the error is reported. This
    // function does NOT throw.

    void git_warning(int code,const char* prefix = nullptr);

    // Provide a wrapper around giterr_set_str that allows printf-style format
    // strings.

    void php_git2_giterr_set(int errorClass,const char* format, ...);

    // Function to register constants defined in php-constants.cpp.

    void php_git2_register_constants(int module_number TSRMLS_DC);

    // Functions to create/initialize module globals.

    void php_git2_globals_ctor(zend_git2_globals* gbls TSRMLS_DC);
    void php_git2_globals_dtor(zend_git2_globals* gbls TSRMLS_DC);
    void php_git2_globals_init(TSRMLS_D);
    void php_git2_globals_request_init(TSRMLS_D);
    void php_git2_globals_request_shutdown(TSRMLS_D);

    // Helper functions for converting git2 values to PHP values.

    int convert_oid_fromstr(git_oid* dest,const char* src,int srclen);
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
