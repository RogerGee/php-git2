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

// php_callback_base

php_callback_base::php_callback_base():
    func(nullptr), data(nullptr)
{
}

bool php_callback_base::is_callable() const
{
    if (func != nullptr && Z_TYPE_P(func) != IS_NULL) {
        char* error = nullptr;
        zend_bool retval;
        retval = zend_is_callable_ex(func,NULL,0,NULL,NULL,NULL,&error TSRMLS_CC);
        if (error) {
            efree(error);
        }

        return retval;
    }

    // NOTE: A null zval is considered a valid callable in this
    // extension for any callback.

    return true;
}

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
        params.assign<0>(buf,size,size,cb->data);
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
        params.assign<1>(cb->data);
        php_git2::convert_transfer_progress(zstats,stats);

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
        params.assign<1>(cb->data);
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
        params.assign<0>(root);
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
        params.assign<0>(idx,cb->data);

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
        params.assign<1>(cb->data);

        // Call the userspace callback.
        params.call(cb->func,&retval);
        convert_to_long(&retval);
        r = Z_LVAL(retval);
        zval_dtor(&retval);

        return static_cast<int>(r);
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
        params.assign<0>(name,cb->data);

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
        params.assign<0>((long)stage,(long)current,(long)total,cb->data);

        // Call the userspace callback.
        params.call(cb->func,&retval);
        convert_to_long(&retval);
        r = Z_LVAL(retval);
        zval_dtor(&retval);

        return r;
    }

    return GIT_EUSER;
}

// config_foreach_callback

int config_foreach_callback::callback(const git_config_entry* entry,void* payload)
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

        // NOTE: if this callback returns non-zero the iteration stops. The
        // default return value of NULL will be converted to 0 to provide the
        // default behavior of continuing iteration.

        long r;
        zval* zentry;
        zval retval;
        zval_array<2> params ZTS_CTOR;

        // Convert arguments to PHP values.
        zentry = params[0];
        params.assign<1>(cb->data);
        array_init(zentry);
        add_assoc_string(zentry,"name",(char*)entry->name,1);
        add_assoc_string(zentry,"value",(char*)entry->value,1);
        add_assoc_long(zentry,"level",entry->level);

        // Call the userspace callback.
        params.call(cb->func,&retval);
        convert_to_long(&retval);
        r = Z_LVAL(retval);
        zval_dtor(&retval);

        return r;
    }

    return GIT_EUSER;
}

// tag_foreach_callback

int tag_foreach_callback::callback(const char* name,git_oid* oid,void* payload)
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

        int error;
        zval retval;
        char buf[GIT_OID_HEXSZ+1];
        zval_array<3> params ZTS_CTOR;

        git_oid_tostr(buf,sizeof(buf),oid);
        params.assign<0>(name,buf,cb->data);

        params.call(cb->func,&retval);
        convert_to_long(&retval);
        error = (int)Z_LVAL(retval);
        zval_dtor(&retval);

        return error;
    }

    return GIT_EUSER;
}

// repository_create_callback

int repository_create_callback::callback(git_repository** out,const char* path,int bare,void* payload)
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

        zval retval;
        zval_array<3> params ZTS_CTOR;

        params.assign<0>(path,static_cast<bool>(bare),cb->data);
        params.call(cb->func,&retval);

        // Allow userspace to generate failure by returning null or false.
        if (Z_TYPE(retval) == IS_NULL || (Z_TYPE(retval) == IS_BOOL && !Z_BVAL(retval))) {
            return -1;
        }

        // Make sure a git_repository resource was returned.
        if (Z_TYPE(retval) != IS_RESOURCE) {
            php_error(E_WARNING,"repository_create_callback: must return git_repository resource");
            return -1;
        }

        // Extract resource from return value.
        php_git_repository* resource;
        resource = (php_git_repository*)zend_fetch_resource(
            NULL TSRMLS_CC,
            Z_RESVAL(retval),
            php_git_repository::resource_name(),
            NULL,
            1,
            php_git_repository::resource_le());
        if (resource == nullptr) {
            // NOTE: PHP raises error if resource type didn't match.
            return -1;
        }

        // The repository resource *MUST* be owned (i.e. this resource is
        // responsible for freeing the handle). We assume that the caller is
        // going to return the repository as an owned resource (potentially),
        // and at the very least, the caller is responsible for freeing the repo
        // handle we assign here. Therefore, we cannot have a git_repository
        // handle from a non-owned context (since it could be freed in another
        // context at some future point). Furthermore, we must revoke ownership
        // on this resource to avoid a double free.

        if (!resource->is_owned()) {
            php_error(E_WARNING,"repository_create_callback: cannot return non-owner resource");
            return -1;
        }

        resource->revoke_ownership();
        *out = resource->get_handle();

        return 0;
    }

    return GIT_EUSER;
}

// diff_notify_callback

int diff_notify_callback::callback(
    const git_diff* diff_so_far,
    const git_diff_delta* delta_to_add,
    const char* matched_pathspec,
    void* payload /* git_diff_options_callback_info */)
{
    git_diff_options_callback_info* info;

    info = reinterpret_cast<git_diff_options_callback_info*>(payload);
    if (info != nullptr) {
        php_callback_base* cb = &info->notifyCallback;
        if (Z_TYPE_P(cb->func) == IS_NULL) {
            return 0;
        }

        int result;
        zval retval;
        zval_array<4> params ZTS_CTOR;
        const php_resource_ref<php_git_diff_nofree> diffRes;

        *diffRes.byval_git2() = diff_so_far;
        diffRes.ret(params[0]);
        convert_diff_delta(params[1],delta_to_add);
        params.assign<2>(matched_pathspec,cb->data);
        params.call(cb->func,&retval);

        convert_to_long(&retval);
        result = static_cast<int>(Z_LVAL(retval));
        zval_dtor(&retval);

        return result;
    }

    return GIT_EUSER;
}

// diff_progress_callback

int diff_progress_callback::callback(
    const git_diff* diff_so_far,
    const char* old_path,
    const char* new_path,
    void* payload)
{
    git_diff_options_callback_info* info;

    info = reinterpret_cast<git_diff_options_callback_info*>(payload);
    if (info != nullptr) {
        php_callback_base* cb = &info->progressCallback;
        if (Z_TYPE_P(cb->func) == IS_NULL) {
            return 0;
        }

        int result;
        zval retval;
        zval_array<4> params;
        const php_resource_ref<php_git_diff_nofree> diffRes;

        *diffRes.byval_git2() = diff_so_far;
        diffRes.ret(params[0]);
        params.assign<1>(old_path,new_path,cb->data);
        params.call(cb->func,&retval);

        if (Z_TYPE(retval) != IS_NULL) {
            convert_to_boolean(&retval);
            result = Z_BVAL(retval) ? 0 : -1;
        }
        else {
            result = 0;
        }
        zval_dtor(&retval);

        return result;
    }

    return GIT_EUSER;
}

// diff_file_callback

int diff_file_callback::callback(const git_diff_delta* delta,float progress,void* payload)
{
    git_diff_callback_info* info;

    info = reinterpret_cast<git_diff_callback_info*>(payload);
    if (info != nullptr) {
        php_callback_base* cb = &info->fileCallback;
        if (Z_TYPE_P(cb->func) == IS_NULL) {
            return 0;
        }

        zval retval;
        zval_array<3> params;

        convert_diff_delta(params[0],delta);
        params.assign<1>(progress,info->zpayload);
        params.call(cb->func,&retval);
        zval_dtor(&retval);

        return 0;
    }

    return GIT_EUSER;
}

// diff_binary_callback

int diff_binary_callback::callback(const git_diff_delta* delta,
    const git_diff_binary* binary,
    void* payload)
{
    git_diff_callback_info* info;

    info = reinterpret_cast<git_diff_callback_info*>(payload);
    if (info != nullptr) {
        php_callback_base* cb = &info->binaryCallback;
        if (Z_TYPE_P(cb->func) == IS_NULL) {
            return 0;
        }

        zval retval;
        zval_array<3> params;

        convert_diff_delta(params[0],delta);
        convert_diff_binary(params[1],binary);
        params.assign<2>(info->zpayload);
        params.call(cb->func,&retval);
        zval_dtor(&retval);

        return 0;
    }

    return GIT_EUSER;
}

// diff_hunk_callback

int diff_hunk_callback::callback(const git_diff_delta* delta,
    const git_diff_hunk* hunk,
    void* payload)
{
    git_diff_callback_info* info;

    info = reinterpret_cast<git_diff_callback_info*>(payload);
    if (info != nullptr) {
        php_callback_base* cb = &info->hunkCallback;
        if (Z_TYPE_P(cb->func) == IS_NULL) {
            return 0;
        }

        zval retval;
        zval_array<3> params;

        convert_diff_delta(params[0],delta);
        convert_diff_hunk(params[1],hunk);
        params.assign<2>(info->zpayload);
        params.call(cb->func,&retval);
        zval_dtor(&retval);

        return 0;
    }

    return GIT_EUSER;
}

// diff_line_callback

int diff_line_callback::callback(const git_diff_delta* delta,
    const git_diff_hunk* hunk,
    const git_diff_line* line,
    void* payload)
{
    git_diff_callback_info* info;

    info = reinterpret_cast<git_diff_callback_info*>(payload);
    if (info != nullptr) {
        php_callback_base* cb = &info->lineCallback;
        if (Z_TYPE_P(cb->func) == IS_NULL) {
            return 0;
        }

        zval retval;
        zval_array<4> params;

        convert_diff_delta(params[0],delta);
        convert_diff_hunk(params[1],hunk);
        convert_diff_line(params[2],line);
        params.assign<3>(info->zpayload);
        params.call(cb->func,&retval);
        zval_dtor(&retval);

        return 0;
    }

    return GIT_EUSER;
}

// index_matched_path_callback

int index_matched_path_callback::callback(const char* path,
    const char* matched_pathspec,
    void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);
#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    if (cb != nullptr) {
        if (Z_TYPE_P(cb->func) == IS_NULL) {
            // Normal behavior is to add the item.
            return 0;
        }

        long r;
        zval retval;
        zval_array<3> params ZTS_CTOR;

        params.assign<0>(path,matched_pathspec,cb->data);
        params.call(cb->func,&retval);
        convert_to_long(&retval);
        r = Z_LVAL(retval);
        zval_dtor(&retval);

        return static_cast<int>(r);
    }

    return GIT_EUSER;
}

// revwalk_hide_callback

int revwalk_hide_callback::callback(const git_oid* commit_id,void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);
#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    if (cb != nullptr) {
        if (Z_TYPE_P(cb->func) == IS_NULL) {
            return 0; // not hidden
        }

        long r;
        zval retval;
        zval_array<2> params ZTS_CTOR;
        char buf[GIT_OID_HEXSZ + 1];

        git_oid_tostr(buf,sizeof(buf),commit_id);
        params.assign<0>(buf,cb->data);

        params.call(cb->func,&retval);
        convert_to_long(&retval);
        r = Z_LVAL(retval);
        zval_dtor(&retval);

        return static_cast<int>(r);
    }

    return GIT_EUSER;
}

// trace_callback

void trace_callback::callback(git_trace_level_t level,const char* msg)
{
    // Just print the trace messages.

	static const char* TRACE_LEVELS[] = {
        "GIT_TRACE_NONE",
        "GIT_TRACE_FATAL",
        "GIT_TRACE_ERROR",
        "GIT_TRACE_WARN",
        "GIT_TRACE_INFO",
        "GIT_TRACE_DEBUG",
        "GIT_TRACE_TRACE"
    };

    php_error(E_WARNING,"git2 trace: %s: %s",TRACE_LEVELS[level],msg);
}

// attr_foreach_callback

int attr_foreach_callback::callback(const char* name,const char* value,void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);
#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    if (cb != nullptr) {
        if (Z_TYPE_P(cb->func) == IS_NULL) {
            // Return a non-zero value to stop looping.
            return 1;
        }

        long r;
        zval retval;
        zval_array<3> params ZTS_CTOR;

        // NOTE: 'value' may be nullptr. The zval_array will handle this
        // accordingly.
        params.assign<0>(name,value,cb->data);

        params.call(cb->func,&retval);
        convert_to_long(&retval);
        r = Z_LVAL(retval);
        zval_dtor(&retval);

        return static_cast<int>(r);
    }

    return GIT_ERROR;
}

// status_callback

int status_callback::callback(const char* path,unsigned int status_flags,void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);
#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    if (cb != nullptr) {
        if (Z_TYPE_P(cb->func) == IS_NULL) {
            // Return a non-zero value to stop looping.
            return 1;
        }

        long r;
        zval retval;
        zval_array<3> params ZTS_CTOR;

        params.assign<0>(path,status_flags,cb->data);
        params.call(cb->func,&retval);

        convert_to_long(&retval);
        r = Z_LVAL(retval);
        zval_dtor(&retval);

        return static_cast<int>(r);
    }

    return GIT_ERROR;
}

// note_foreach_callback

int note_foreach_callback::callback(const git_oid* blob_id,
    const git_oid* annotated_object_id,void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);
#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    if (cb != nullptr) {
        if (Z_TYPE_P(cb->func) == IS_NULL) {
            return GIT_OK;
        }

        long r;
        zval retval;
        zval_array<2> params ZTS_CTOR;
        char buf[GIT_OID_HEXSZ + 1];

        git_oid_tostr(buf,sizeof(buf),blob_id);
        ZVAL_STRING(params[0],buf,1);
        git_oid_tostr(buf,sizeof(buf),annotated_object_id);
        ZVAL_STRING(params[1],buf,1);
        params.assign<2>(cb->data);
        params.call(cb->func,&retval);

        convert_to_long(&retval);
        r = Z_LVAL(retval);
        zval_dtor(&retval);

        return static_cast<int>(r);
    }

    return GIT_ERROR;
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
