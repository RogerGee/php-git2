/*
 * php-type.cpp
 *
 * Copyright (C) Roger P. Gee
 */

#include "php-type.h"

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

    php_array_base::parse_impl(zvp,argno);
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
