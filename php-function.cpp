/*
 * php-function.cpp
 *
 * Copyright (C) Roger P. Gee
 */

#include "php-function.h"
using namespace php_git2;

void php_git2::php_extract_args_impl(int nargs,php_parameter* dest[],int ngiven)
{
    zval args;
    int result;
    HashPosition pos;

    array_init_size(&args,nargs);
    result = zend_copy_parameters_array(nargs,&args);

    if (result == FAILURE) {
        zval_ptr_dtor(&args);

        throw php_git2_exception("%s() expects exactly %d parameters, %d given",
            get_active_function_name(),
            nargs,
            ngiven);
    }

    zend_hash_internal_pointer_reset_ex(Z_ARRVAL(args),&pos);
    for (int i = 0;i < nargs;++i) {
        dest[i]->parse(zend_hash_get_current_data_ex(Z_ARRVAL(args),&pos),i+1);
        zend_hash_move_forward_ex(Z_ARRVAL(args),&pos);
    }

    zval_ptr_dtor(&args);
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
