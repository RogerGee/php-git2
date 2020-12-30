/*
 * php-function.cpp
 *
 * Copyright (C) Roger P. Gee
 */

#include "php-function.h"
using namespace php_git2;

void php_git2::php_extract_args_impl(
    int result,
    int nargs,
    zval* args[],
    php_parameter* dest[],
    int ngiven)
{
    if (result == FAILURE) {
        throw php_git2_exception("%s() expects exactly %d parameters, %d given",
            get_active_function_name(),
            nargs,
            ngiven);
    }

    for (int i = 0;i < nargs;++i) {
        php_parameter* val = dest[i];
        val->parse(args[i],i+1);
    }
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
