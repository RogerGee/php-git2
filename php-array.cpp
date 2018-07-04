/*
 * php-array.cpp
 *
 * This file is a part of php-git2.
 */

#include "php-array.h"
using namespace php_git2;

array_wrapper::array_wrapper(zval* zarray):
    ht(Z_ARRVAL_P(zarray)), zvp(nullptr), usesTmp(false)
{
    // Give 'tmp' an initial value so that we have it in a known state
    // initially.
    INIT_ZVAL(tmp);
}

array_wrapper::~array_wrapper()
{
    zval_dtor(&tmp);
}

bool array_wrapper::query(const char* key,size_t keysz)
{
    usesTmp = false;
    if (zend_hash_find(ht,key,keysz,(void**)&zvp) != FAILURE) {
        return true;
    }

    zvp = nullptr;
    return false;
}

bool array_wrapper::query(int index)
{
    usesTmp = false;
    if (zend_hash_index_find(ht,index,(void**)&zvp) != FAILURE) {
        return true;
    }

    zvp = nullptr;
    return false;
}

const char* array_wrapper::get_string() const
{
    if (found()) {
        zval* zv = copy_if_not_type(IS_STRING);

        return Z_STRVAL_P(zv);
    }

    return nullptr;
}

const char* array_wrapper::get_string_nullable() const
{
    if (found() && type() != IS_NULL) {
        zval* zv = copy_if_not_type(IS_STRING);

        return Z_STRVAL_P(zv);
    }

    return nullptr;
}

int array_wrapper::get_string_length() const
{
    if (found() && type() != IS_NULL) {
        zval* zv = copy_if_not_type(IS_STRING);

        return Z_STRLEN_P(zv);
    }

    return 0;
}

long array_wrapper::get_long() const
{
    if (found()) {
        zval* zv = copy_if_not_type(IS_LONG);

        return Z_LVAL_P(zv);
    }

    return 0;
}

zval* array_wrapper::get_zval() const
{
    if (found()) {
        return *zvp;
    }

    zval_dtor(&tmp);
    ZVAL_NULL(&tmp);
    return &tmp;
}

void array_wrapper::get_oid(git_oid* out) const
{
    if (found()) {
        const char* src;
        int srclen;
        zval* zv = copy_if_not_type(IS_STRING);

        src = Z_STRVAL_P(zv);
        srclen = Z_STRLEN_P(zv);
        convert_oid_fromstr(out,src,srclen);
    }
}

zval* array_wrapper::copy_if_not_type(int type) const
{
    if (usesTmp) {
        return &tmp;
    }

    zval* zv = *zvp;

    if (Z_TYPE_P(zv) != type) {
        // Use the temporary zval to store the converted, copied zval.
        zv = &tmp;
        zval_dtor(zv);

        // Copy the zval into the temporary zval.
        ZVAL_COPY_VALUE(zv,*zvp);
        zval_copy_ctor(zv);

        // Convert the copied zval.
        convert_to_explicit_type(zv,type);

        // Flag that we're using the temporary zval so we won't have to repeat
        // this.
        usesTmp = true;
    }

    return zv;
}
