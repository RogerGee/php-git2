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
    if (cb != nullptr) {
        // Special Case: if the callback payload is a stream, we write directly
        // to the stream and avoid the callback altogether.
        if (cb->data != nullptr && Z_TYPE_P(cb->data) == IS_RESOURCE
            && strcmp(zend_rsrc_list_get_rsrc_type(Z_RESVAL_P(cb->data)),"stream") == 0)
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
        zval_array<3> params;
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
        zval_array<2> params;
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

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
