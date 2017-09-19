/*
 * php-callback.cpp
 *
 * This file is a part of php-git2.
 *
 * This unit provides the out-of-line implementations of the various callback
 * routines used by php-git2.
 */

#include "php-callback.h"
using namespace php_git2;

// packbuilder_foreach_callback

/*static*/ int
packbuilder_foreach_callback::callback(void* buf,size_t size,void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);
#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    if (cb != nullptr) {
        // Special Case: if the callback is null but the payload is a stream, we
        // write directly to the stream.
        if (Z_TYPE_P(cb->func) == IS_NULL && cb->data != nullptr && Z_TYPE_P(cb->data) == IS_RESOURCE
            && strcmp(zend_rsrc_list_get_rsrc_type(Z_RESVAL_P(cb->data) TSRMLS_CC),"stream") == 0)
        {
            php_stream* stream = nullptr;
            php_stream_from_zval_no_verify(stream,&cb->data);
            if (stream != nullptr) {
                php_stream_write(stream,(const char*)buf,size);
                return 0;
            }
        }

        // Null callable is a no-op.
        if (Z_TYPE_P(cb->func) == IS_NULL) {
            return GIT_OK;
        }

        // Otherwise we convert the values to zvals and call PHP userspace.
        long r;
        zval retval;
        zval_array<3> params ZTS_CTOR;
        params.assign<0>(std::forward<const void*>(buf),
            std::forward<size_t>(size),
            std::forward<long>(size),
            std::forward<zval*>(cb->data));
        params.call(cb->func,&retval);
        convert_to_long(&retval);
        r = Z_LVAL(retval);
        zval_dtor(&retval);
        return r;
    }

    return GIT_EUSER;
}

// transfer_progress_callback

/*static*/ int
transfer_progress_callback::callback(const git_transfer_progress* stats,void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);
#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    if (cb != nullptr) {
        // Avoid setup for null callables.
        if (Z_TYPE_P(cb->func) == IS_NULL) {
            return GIT_OK;
        }

        // Call the PHP userspace callback. We convert the git_transfer_progress
        // struct to a PHP array.

        long r;
        zval* zstats;
        zval retval;
        zval_array<2> params ZTS_CTOR;
        zstats = params[0];
        params.assign<1>(std::forward<zval*>(cb->data));
        array_init(zstats);
        add_assoc_long_ex(zstats,"total_objects",14,stats->total_objects);
        add_assoc_long_ex(zstats,"indexed_objects",16,stats->indexed_objects);
        add_assoc_long_ex(zstats,"received_objects",17,stats->received_objects);
        add_assoc_long_ex(zstats,"local_objects",14,stats->local_objects);
        add_assoc_long_ex(zstats,"total_objects",14,stats->total_objects);
        add_assoc_long_ex(zstats,"indexed_deltas",15,stats->indexed_deltas);
        add_assoc_long_ex(zstats,"received_bytes",15,stats->received_bytes);
        params.call(cb->func,&retval);
        convert_to_long(&retval);
        r = Z_LVAL(retval);
        zval_dtor(&retval);
        return r;
    }

    return GIT_EUSER;
}

// odb_foreach_callback

/*static*/ int
odb_foreach_callback::callback(const git_oid* oid,void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);
#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    if (cb != nullptr) {
        // Avoid setup for null callables.
        if (Z_TYPE_P(cb->func) == IS_NULL) {
            return GIT_OK;
        }

        // Call the PHP userspace callback. We convert the git_oid struct to a
        // PHP string.

        long r;
        zval retval;
        zval_array<2> params ZTS_CTOR;
        char buf[GIT_OID_HEXSZ + 1];

        git_oid_tostr(buf,sizeof(buf),oid);
        ZVAL_STRING(params[0],buf,1);
        params.assign<1>(std::forward<zval*>(cb->data));
        params.call(cb->func,&retval);
        convert_to_long(&retval);
        r = Z_LVAL(retval);
        zval_dtor(&retval);

        return r;
    }

    return GIT_EUSER;
}

// treewalk_callback

/*static*/ int
treewalk_callback::callback(const char* root,const git_tree_entry* entry,void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);
#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(pc);
#endif

    if (cb != nullptr) {
        // Account for when callable is null and do nothing.
        if (Z_TYPE_P(cb->func) == IS_NULL) {
            return GIT_OK;
        }

        long r;
        zval retval;
        zval_array<3> params ZTS_CTOR;
        const php_resource_ref<php_git_tree_entry_nofree> res; // it cannot free

        // Convert arguments to PHP values.
        params.assign<0>(std::forward<const char*>(root));
        *res.byval_git2() = entry;
        res.ret(params[1]);
        params.assign<2>(std::forward<zval*>(cb->data));

        // Call the userspace callback.
        params.call(cb->func,&retval);
        convert_to_long(&retval);
        r = Z_LVAL(retval);
        zval_dtor(&retval);

        return r;
    }

    return GIT_EUSER;
}

// commit_parent_callback

/*static*/ const git_oid*
commit_parent_callback::callback(size_t idx,void* payload)
{
    commit_parent_callback::sync_callback* cb
        = reinterpret_cast<commit_parent_callback::sync_callback*>(payload);
#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(pc);
#endif

    if (cb != nullptr) {
        // Account for when callable is null and do nothing.
        if (Z_TYPE_P(cb->func) == IS_NULL) {
            return nullptr;
        }

        zval retval;
        git_oid* oid;
        zval_array<2> params ZTS_CTOR;
        INIT_ZVAL(retval);

        // Convert arguments to PHP values.
        params.assign<0>(std::forward<long>(idx));
        params.assign<1>(std::forward<zval*>(cb->data));

        // Call the userspace callback.
        params.call(cb->func,&retval);

        // Returning null means the callback ends.
        if (Z_TYPE(retval) == IS_NULL) {
            return nullptr;
        }

        // Otherwise a string is returning representing the ID of a commit. We
        // convert this to binary and store the result in the git_oid attached
        // to the callback.
        oid = &cb->oidbuf;
        convert_to_string(&retval);
        convert_oid_fromstr(oid,Z_STRVAL(retval),Z_STRLEN(retval));

        return oid;
    }

    return nullptr;
}

// reference_foreach_callback

int reference_foreach_callback::callback(git_reference* ref,void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);
#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(pc);
#endif

    if (cb != nullptr) {
        // Account for when callable is null and do nothing.
        if (Z_TYPE_P(cb->func) == IS_NULL) {
            return GIT_OK;
        }

        long r;
        zval retval;
        zval_array<2> params ZTS_CTOR;
        const php_resource_ref<php_git_reference> res;

        // Convert arguments to PHP values.
        *res.byval_git2() = ref;
        res.ret(params[0]);
        params.assign<1>(std::forward<zval*>(cb->data));

        // Call the userspace callback.
        params.call(cb->func,&retval);
        convert_to_long(&retval);
        r = Z_LVAL(retval);
        zval_dtor(&retval);

        return r;
    }

    return GIT_EUSER;
}

// reference_foreach_name_callback

int reference_foreach_name_callback::callback(const char* name,void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);
#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(pc);
#endif

    if (cb != nullptr) {
        // Account for when callable is null and do nothing.
        if (Z_TYPE_P(cb->func) == IS_NULL) {
            return GIT_OK;
        }

        long r;
        zval retval;
        zval_array<2> params ZTS_CTOR;

        // Convert arguments to PHP values.
        params.assign<0>(std::forward<const char*>(name));
        params.assign<1>(std::forward<zval*>(cb->data));

        // Call the userspace callback.
        params.call(cb->func,&retval);
        convert_to_long(&retval);
        r = Z_LVAL(retval);
        zval_dtor(&retval);

        return r;
    }

    return GIT_EUSER;
}

// packbuilder_progress_callback

int packbuilder_progress_callback::callback(int stage,uint32_t current,uint32_t total,void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);
#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    if (cb != nullptr) {
        // Avoid setup for null callables.
        if (Z_TYPE_P(cb->func) == IS_NULL) {
            return GIT_OK;
        }

        long r;
        zval retval;
        zval_array<4> params ZTS_CTOR;

        // Convert arguments to PHP values.
        params.assign<0>(std::forward<long>((long)stage));
        params.assign<1>(std::forward<long>((long)current));
        params.assign<2>(std::forward<long>((long)total));
        params.assign<3>(std::forward<zval*>(cb->data));

        // Call the userspace callback.
        params.call(cb->func,&retval);
        convert_to_long(&retval);
        r = Z_LVAL(retval);
        zval_dtor(&retval);

        return r;
    }

    return GIT_EUSER;
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
