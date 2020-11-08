/*
 * php-type.cpp
 *
 * Copyright (C) Roger P. Gee
 */

#include "php-type.h"
#include <cstdio>
#include <cstring>
using namespace php_git2;

// php_parameter

void php_parameter::parse_with_context(zval* zvp,const char* ctx)
{
    try {
        parse_impl(zvp,1);

    } catch (php_git2_propagated_exception&) {
        php_exception_wrapper wex;
        const char* basemsg;

        basemsg = wex.get_message();
        if (basemsg != nullptr) {
            const char* found;
            zend_string* msg;

            found = strstr(basemsg,"parameter 1");
            if (found) {
                int offset;
                int nextOffset;
                int ctxlen = strlen(ctx);
                int baselen = strlen(basemsg);
                int extend = ctxlen - sizeof("parameter 1")-1;

                msg = zend_string_init(basemsg,baselen,0);
                if (extend > 0) {
                    msg = zend_string_extend(msg,baselen + extend,0);
                }
                else if (extend < 0) {
                    msg = zend_string_truncate(msg,baselen + extend,0);
                }

                offset = found - basemsg;
                memcpy(ZSTR_VAL(msg) + offset,ctx,ctxlen);

                nextOffset = offset + sizeof("parameter 1")-1;
                memcpy(ZSTR_VAL(msg) + offset + ctxlen,
                    basemsg + nextOffset,
                    baselen - nextOffset);

                php_git2_exception ex(ZSTR_VAL(msg));
                zend_string_release(msg);
                wex.handle();
                throw ex;
            }

            wex.handle();
            throw php_git2_exception("Invalid value for parameter '%s'",ctx);
        }

        wex.handle();
        throw php_git2_exception("Invalid value for parameter '%s'",ctx);
    }
}

// php_output_parameter

void php_output_parameter::parse_impl(zval* zp,int argno)
{
    // Output parameters must maintain the original zval so that it can be
    // directly modified.
    this->zvp = zvp;
}

// php_value_generic

void php_value_generic::parse_impl(zval* zvp,int)
{
    ZVAL_COPY_VALUE(&value,zvp);
}

// php_resource_base

void php_resource_base::parse_impl(zval* zvp,int argno)
{
    int result;
    zval* dummy;

    result = zend_parse_parameter(ZEND_PARSE_PARAMS_THROW,argno,zvp,"r",&dummy);
    if (result == FAILURE) {
        throw php_git2_propagated_exception();
    }

    ZVAL_COPY_VALUE(&value,zvp);
}

// php_array_base

void php_array_base::parse_impl(zval* zvp,int argno)
{
    int result;
    zval* dummy;

    result = zend_parse_parameter(ZEND_PARSE_PARAMS_THROW,argno,zvp,"a",&dummy);
    if (result == FAILURE) {
        throw php_git2_propagated_exception();
    }

    ZVAL_COPY_VALUE(&value,zvp);
}

// php_option_array

void php_option_array::parse_impl(zval* zvp,int argno)
{
    if (Z_TYPE(value) == IS_NULL) {
        ZVAL_NULL(&value);
        return;
    }

    php_array_base::parse(zvp,argno);
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
