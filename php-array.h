/*
 * php-array.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_ARRAY_H
#define PHPGIT2_ARRAY_H
#include "php-git2.h"

namespace php_git2
{

    // Provide a type for wrapping common array operations.

    class array_wrapper
    {
    public:
        array_wrapper(zval* zarray);
        ~array_wrapper();

        bool query(const char* key,size_t keysz);
        bool query(int index);

        int type() const
        {
            return Z_TYPE_PP(zvp);
        }

        bool found() const
        {
            return zvp != nullptr;
        }

        const char* get_string() const;
        const char* get_string_nullable() const;
        long get_long() const;
        zval* get_zval() const;
        void get_oid(git_oid* out) const;
    private:
        HashTable* ht;
        zval** zvp;
        mutable zval tmp;

        zval* copy_if_not_type(int type) const;
    };
}

// Define convenience macros for working with array_wrapper instances.

#define GIT2_ARRAY_LOOKUP_STRING(wrapper,name,var)    \
    if (wrapper.query(#name,sizeof(#name))) {         \
        var.name = wrapper.get_string();              \
    }

#define GIT2_ARRAY_INDEX_STRING(wrapper,index,name,var) \
    if (wrapper.query(index)) {                         \
        var.name = wrapper.get_string();                \
    }

#define GIT2_ARRAY_LOOKUP_STRING_NULLABLE(wrapper,name,var)    \
    if (wrapper.query(#name,sizeof(#name))) {                  \
        var.name = wrapper.get_string_nullable();              \
    }

#define GIT2_ARRAY_INDEX_STRING_NULLABLE(wrapper,index,name,var)    \
    if (wrapper.query(index)) {                                     \
        var.name = wrapper.get_string_nullable();                   \
    }

#define GIT2_ARRAY_LOOKUP_LONG(wrapper,name,var)                        \
    if (wrapper.query(#name,sizeof(#name))) {                           \
        var.name = (decltype(var.name))wrapper.get_long();              \
    }

#define GIT2_ARRAY_INDEX_LONG(wrapper,index,name,var)       \
    if (wrapper.query(index)) {                             \
        var.name = (decltype(var.name))wrapper.get_long();  \
    }

#define GIT2_ARRAY_LOOKUP_OID(wrapper,name,var) \
    if (wrapper.query(#name,sizeof(#name))) {   \
        wrapper.get_oid(&var.name);             \
    }

#define GIT2_ARRAY_INDEX_OID(wrapper,index,name,var)  \
    if (wrapper.query(index)) {                       \
        wrapper.get_oid(&var.name);                   \
    }

#define GIT2_ARRAY_LOOKUP_SUBOBJECT(wrapper,obj,name,var)       \
    if (wrapper.query(#name,sizeof(#name))) {                   \
        obj.set_zval(wrapper.get_zval());                       \
        var.name = obj.byval_git2(argno);                       \
    }

#define GIT2_ARRAY_INDEX_SUBOBJECT(wrapper,index,obj,name,var)  \
    if (wrapper.query(index)) {                                 \
        obj.set_zval(wrapper.get_zval());                       \
        var.name = obj.byval_git2(argno);                       \
    }

#define GIT2_ARRAY_LOOKUP_SUBOBJECT_DEREFERENCE(wrapper,obj,name,var)   \
    if (wrapper.query(#name,sizeof(#name))) {                           \
        obj.set_zval(wrapper.get_zval());                               \
        var.name = *obj.byval_git2(argno);                              \
    }

#define GIT2_ARRAY_INDEX_SUBOBJECT_DEREFERENCE(wrapper,index,obj,name,var)  \
    if (wrapper.query(index)) {                                  \
        obj.set_zval(wrapper.get_zval());                        \
        var.name = *obj.byval_git2(argno);                       \
    }

#define GIT2_ARRAY_LOOKUP_RESOURCE(wrapper,type,name,var)               \
    if (wrapper.query(#name,sizeof(#name))) {                           \
        php_resource<type> rr;                                          \
        type r;                                                         \
        zval* zv = wrapper.get_zval();                                  \
        if (Z_TYPE_P(zv) != IS_NULL) {                                  \
            if (Z_TYPE_P(zv) != IS_RESOURCE) {                          \
                error_custom("expected resource in property " #name,argno); \
            }                                                           \
            rr.set_zval(zv);                                            \
            r = rr.get_object(argno);                                   \
            var.name = r.get_handle();                                  \
        }                                                               \
    }

#define GIT2_ARRAY_INDEX_RESOURCE(wrapper,index,type,name,var)          \
    if (wrapper.query(index)) {                                         \
        php_resource<type> rr;                                          \
        type r;                                                         \
        zval* zv = wrapper.get_zval();                                  \
        if (Z_TYPE_P(zv) != IS_NULL) {                                  \
            if (Z_TYPE_P(zv) != IS_RESOURCE) {                          \
                error_custom("expected resource in property " #name,argno); \
            }                                                           \
            rr.set_zval(zv);                                            \
            r = rr.get_object(argno);                                   \
            var.name = r.get_handle();                                  \
        }                                                               \
    }

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
