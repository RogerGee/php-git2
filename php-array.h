/*
 * php-array.h
 *
 * Copyright (C) Roger P. Gee
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
            return Z_TYPE_P(zvp);
        }

        bool found() const
        {
            return zvp != nullptr;
        }

        const char* get_string() const;
        const char* get_string_nullable() const;
        int get_string_length() const;
        long get_long() const;
        bool get_bool() const;
        zval* get_zval() const;
        void get_oid(git_oid* out) const;

    private:
        HashTable* ht;
        zval* zvp;
        mutable zval tmp;
        mutable bool usesTmp;

        zval* copy_if_not_type(int type) const;
    };
}

// Define convenience macros for working with array_wrapper instances. These
// should be used in a byval_git2 implementation with parameter named 'argno'.

#define GIT2_ARRAY_LOOKUP_STRING(wrapper,name,var)    \
    if (wrapper.query(#name,sizeof(#name)-1)) {         \
        var.name = wrapper.get_string();              \
    }

#define GIT2_ARRAY_INDEX_STRING(wrapper,index,name,var) \
    if (wrapper.query(index)) {                         \
        var.name = wrapper.get_string();                \
    }

#define GIT2_ARRAY_LOOKUP_STRING_NULLABLE(wrapper,name,var)    \
    if (wrapper.query(#name,sizeof(#name)-1)) {                  \
        var.name = wrapper.get_string_nullable();              \
    }

#define GIT2_ARRAY_INDEX_STRING_NULLABLE(wrapper,index,name,var)    \
    if (wrapper.query(index)) {                                     \
        var.name = wrapper.get_string_nullable();                   \
    }

#define GIT2_ARRAY_LOOKUP_STRING_WITH_LENGTH(wrapper,name,len,var)  \
    if (wrapper.query(#name,sizeof(#name)-1)) {                       \
        var.name = wrapper.get_string();                            \
        var.len = wrapper.get_string_length();                      \
    }

#define GIT2_ARRAY_LOOKUP_LONG(wrapper,name,var)                        \
    if (wrapper.query(#name,sizeof(#name)-1)) {                           \
        var.name = (decltype(var.name))wrapper.get_long();              \
    }

#define GIT2_ARRAY_INDEX_LONG(wrapper,index,name,var)       \
    if (wrapper.query(index)) {                             \
        var.name = (decltype(var.name))wrapper.get_long();  \
    }

#define GIT2_ARRAY_LOOKUP_BOOL(wrapper,name,var)                        \
    if (wrapper.query(#name,sizeof(#name)-1)) {                           \
        var.name = (decltype(var.name))wrapper.get_bool();              \
    }

#define GIT2_ARRAY_INDEX_BOOL(wrapper,index,name,var)       \
    if (wrapper.query(index)) {                             \
        var.name = (decltype(var.name))wrapper.get_bool();  \
    }

#define GIT2_ARRAY_LOOKUP_OID(wrapper,name,var) \
    if (wrapper.query(#name,sizeof(#name)-1)) {   \
        wrapper.get_oid(&var.name);             \
    }

#define GIT2_ARRAY_INDEX_OID(wrapper,index,name,var)  \
    if (wrapper.query(index)) {                       \
        wrapper.get_oid(&var.name);                   \
    }

#define GIT2_ARRAY_LOOKUP_SUBOBJECT(wrapper,obj,name,var)       \
    if (wrapper.query(#name,sizeof(#name)-1)) {                   \
        obj.set_zval(wrapper.get_zval());                       \
        var.name = obj.byval_git2(argno);                       \
    }

#define GIT2_ARRAY_INDEX_SUBOBJECT(wrapper,index,obj,name,var)  \
    if (wrapper.query(index)) {                                 \
        obj.set_zval(wrapper.get_zval());                       \
        var.name = obj.byval_git2(argno);                       \
    }

#define GIT2_ARRAY_LOOKUP_SUBOBJECT_DEREFERENCE(wrapper,obj,name,var)   \
    if (wrapper.query(#name,sizeof(#name)-1)) {                           \
        obj.set_zval(wrapper.get_zval());                               \
        var.name = *obj.byval_git2(argno);                              \
    }

#define GIT2_ARRAY_INDEX_SUBOBJECT_DEREFERENCE(wrapper,index,obj,name,var)  \
    if (wrapper.query(index)) {                                  \
        obj.set_zval(wrapper.get_zval());                        \
        var.name = *obj.byval_git2(argno);                       \
    }

#define GIT2_ARRAY_LOOKUP_RESOURCE(wrapper,type,name,var)               \
    if (wrapper.query(#name,sizeof(#name)-1)) {                           \
        php_resource<type> rr ZTS_CTOR;                                 \
        type r;                                                         \
        zval* zv = wrapper.get_zval();                                  \
        if (Z_TYPE_P(zv) != IS_NULL) {                                  \
            if (Z_TYPE_P(zv) != IS_RESOURCE) {                          \
                error_custom("Expected resource in property " #name,argno); \
            }                                                           \
            rr.set_zval(zv);                                            \
            r = rr.get_object(argno);                                   \
            var.name = r.get_handle();                                  \
        }                                                               \
    }

#define GIT2_ARRAY_INDEX_RESOURCE(wrapper,index,type,name,var)          \
    if (wrapper.query(index)) {                                         \
        php_resource<type> rr ZTS_CTOR;                                 \
        type r;                                                         \
        zval* zv = wrapper.get_zval();                                  \
        if (Z_TYPE_P(zv) != IS_NULL) {                                  \
            if (Z_TYPE_P(zv) != IS_RESOURCE) {                          \
                error_custom("Expected resource in property " #name,argno); \
            }                                                           \
            rr.set_zval(zv);                                            \
            r = rr.get_object(argno);                                   \
            var.name = r.get_handle();                                  \
        }                                                               \
    }

#define GIT2_ARRAY_LOOKUP_CALLBACK(wrapper,callbackWrapper,callbackObject,funcName,payloadName,var) \
    if (wrapper.query(#funcName,sizeof(#funcName)-1)) {                   \
        zval* zfunc, *zdata;                                            \
        zfunc = wrapper.get_zval();                                     \
        if (wrapper.query(#payloadName,sizeof(#payloadName)-1)) {         \
            zdata = wrapper.get_zval();                                 \
        }                                                               \
        else {                                                          \
            ALLOC_INIT_ZVAL(zdata);                                     \
            Z_DELREF_P(zdata);                                          \
        }                                                               \
        callbackObject.set_members(zfunc,zdata);                        \
        var.funcName = callbackWrapper::callback;                       \
        var.payloadName = (void*)&callbackObject;                       \
    }

// NOTE: this variant assumes 'var' is already zeroed out or initialized in some
// way.
#define GIT2_ARRAY_LOOKUP_CALLBACK_NULLABLE(wrapper,callbackWrapper,callbackObject,funcName,payloadName,var) \
    if (wrapper.query(#funcName,sizeof(#funcName)-1)) {                   \
        zval* zfunc, *zdata;                                            \
        zfunc = wrapper.get_zval();                                     \
        if (Z_TYPE_P(zfunc) != IS_NULL) {                               \
            if (wrapper.query(#payloadName,sizeof(#payloadName)-1)) {     \
                zdata = wrapper.get_zval();                             \
            }                                                           \
            else {                                                      \
                ALLOC_INIT_ZVAL(zdata);                                 \
                Z_DELREF_P(zdata);                                      \
            }                                                           \
            callbackObject.set_members(zfunc,zdata);                    \
            var.funcName = callbackWrapper::callback;                   \
            var.payloadName = (void*)&callbackObject;                   \
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
