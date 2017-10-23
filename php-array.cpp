/*
 * php-array.cpp
 *
 * This file is a part of php-git2.
 */

#include "php-array.h"
using namespace php_git2;

array_wrapper::array_wrapper(zval* zarray):
    ht(Z_ARRVAL_P(zarray)), zvp(nullptr)
{
    INIT_ZVAL(tmp);
}

array_wrapper::~array_wrapper()
{
    zval_dtor(&tmp);
}

bool array_wrapper::query(const char* key,size_t keysz)
{
    if (zend_hash_find(ht,key,keysz,(void**)&zvp) != FAILURE) {
        return true;
    }

    zvp = nullptr;
    return false;
}

bool array_wrapper::query(int index)
{
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
    zval* zv = *zvp;

    if (Z_TYPE_P(zv) != type) {
        zv = &tmp;
        zval_dtor(zv);
        ZVAL_COPY_VALUE(zv,*zvp);
        zval_copy_ctor(zv);

        convert_to_explicit_type(zv,type);
    }

    return zv;
}
