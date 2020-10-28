/*
 * php-function.cpp
 *
 * Copyright (C) Roger P. Gee
 */

#include "php-function.h"
using namespace php_git2;

void php_git2::php_extract_args_impl(int nargs,zval* args,php_value* dest)
{
    int result;

    result = zend_get_parameters_array_ex(nargs,args);
    for (int i = 0;i < nargs;++i) {
        php_value* val = dest[i];

        val->parse(args[i],i+1);
    }

    if (result == FAILURE) {
        php_error(E_ERROR,"Incorrect number of arguments passed to function");
    }
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
