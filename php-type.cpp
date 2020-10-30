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

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
