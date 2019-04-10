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

int php_git2::php_git2_invoke_callback(zval* func,zval* ret,zend_uint paramCount,zval* params[])
{
    php_bailer bailer;
    php_bailout_context ctx(bailer);
    int result = GIT_OK;

    INIT_ZVAL(*ret);

    if (BAILOUT_ENTER_REGION(ctx)) {
        int retval;

        retval = call_user_function(EG(function_table),NULL,func,ret,paramCount,params TSRMLS_CC);
        if (retval == FAILURE) {
            php_git2_giterr_set(GITERR_INVALID,"Failed to invoke userspace callback");
            result = GIT_EPHPFATAL;
        }
        else {
            php_exception_wrapper ex;

            // Handle case where PHP userspace threw an exception.
            if (ex.has_exception()) {
                ex.set_giterr(TSRMLS_C);
                result = GIT_EPHPEXPROP;
            }
        }
    }
    else {
        // Set a libgit2 error for completeness.
        php_git2_giterr_set(GITERR_INVALID,"PHP reported a fatal error");

        // Allow the fatal error to propogate.
        result = GIT_EPHPFATALPROP;
        bailer.handled();
    }

    return result;

}

// php_callback_base

php_callback_base::php_callback_base(TSRMLS_D):
    php_zts_base(TSRMLS_C), func(nullptr), data(nullptr)
{
}

bool php_callback_base::is_null() const
{
    if (func == nullptr || Z_TYPE_P(func) == IS_NULL) {
        return true;
    }

    return false;
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

    return false;
}

// packbuilder_foreach_callback

/*static*/ int
packbuilder_foreach_callback::callback(void* buf,size_t size,void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    // Special Case: if the callback is null but the payload is a stream, we
    // write directly to the stream.

    if (Z_TYPE_P(cb->func) == IS_NULL
        && cb->data != nullptr
        && Z_TYPE_P(cb->data) == IS_RESOURCE
        && strcmp(zend_rsrc_list_get_rsrc_type(Z_RESVAL_P(cb->data) TSRMLS_CC),"stream") == 0)
    {
        php_stream* stream = nullptr;
        php_stream_from_zval_no_verify(stream,&cb->data);
        if (stream != nullptr) {
            php_stream_write(stream,(const char*)buf,size);
            return GIT_OK;
        }

        giterr_set_str(GITERR_INVALID,"packbuilder_foreach_callback: stream is invalid");
        return GIT_ERROR;
    }

    // Account for when callable is null and do nothing. We cannot anticipate
    // that the callback handler is nullable so we have to handle it here. Since
    // this function is read-only, all we have to do is quit with a success
    // status.

    if (Z_TYPE_P(cb->func) == IS_NULL) {
        return GIT_OK;
    }

    // Otherwise we convert the values to zvals and call PHP userspace.

    int result;
    zval retval;
    zval_array<3> params ZTS_CTOR;

    params.assign<0>(buf,size,size,cb->data);

    result = params.call(cb->func,&retval);
    zval_dtor(&retval);

    return result;
}

// transfer_progress_callback

/*static*/ int
transfer_progress_callback::callback(const git_transfer_progress* stats,void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    // Call the PHP userspace callback. We convert the git_transfer_progress
    // struct to a PHP array.

    int result;
    zval* zstats;
    zval retval;
    zval_array<2> params ZTS_CTOR;

    zstats = params[0];
    params.assign<1>(cb->data);
    php_git2::convert_transfer_progress(zstats,stats);

    result = params.call(cb->func,&retval);

    if (result == 0) {
        if (Z_TYPE(retval) == IS_BOOL && !Z_BVAL(retval)) {
            result = -1;
        }
    }

    zval_dtor(&retval);

    return result;
}

// odb_foreach_callback

/*static*/ int
odb_foreach_callback::callback(const git_oid* oid,void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    // Call the PHP userspace callback. We convert the git_oid struct to a PHP
    // string.

    int result;
    zval retval;
    zval_array<2> params ZTS_CTOR;

    convert_oid(params[0],oid);
    params.assign<1>(cb->data);

    result = params.call(cb->func,&retval);
    zval_dtor(&retval);

    return result;
}

// treewalk_callback

/*static*/ int
treewalk_callback::callback(const char* root,const git_tree_entry* entry,void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    // Call the userspace function.

    int result;
    zval retval;
    zval_array<3> params ZTS_CTOR;
    const php_resource_ref<php_git_tree_entry_nofree> res; // it cannot free

    params.assign<0>(root);
    res.set_object(entry);
    res.ret(params[1]);
    params.assign<2>(std::forward<zval*>(cb->data));

    result = params.call(cb->func,&retval);
    zval_dtor(&retval);

    return result;
}

// commit_parent_callback

/*static*/ const git_oid*
commit_parent_callback::callback(size_t idx,void* payload)
{
    commit_parent_callback::sync_callback* cb;
    cb = reinterpret_cast<commit_parent_callback::sync_callback*>(payload);

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    int result;
    zval retval;
    git_oid* oid;
    zval_array<2> params ZTS_CTOR;
    INIT_ZVAL(retval);

    // Convert arguments to PHP values.
    params.assign<0>(idx,cb->data);

    // Call the userspace callback.
    result = params.call(cb->func,&retval);

    // Handle errors. It is unclear how this callback is to report errors (if at
    // all), so we just report end of operation.
    if (result < 0) {
        // Flag propagated fatal errors globally so they can be issued later on
        // by a bailout context. We do this since libgit2 will NOT report the
        // error to the calling PHP context.
        if (result == GIT_EPHPFATALPROP) {
            GIT2_G(propagateFatalError) = true;
        }

        return nullptr;
    }

    // Returning null means the callback ends.
    if (Z_TYPE(retval) == IS_NULL) {
        return nullptr;
    }

    // Otherwise a string is returned representing the ID of a commit. We
    // convert this to binary and store the result in the git_oid attached to
    // the callback.
    oid = &cb->oidbuf;
    convert_to_string(&retval);
    convert_oid_fromstr(oid,Z_STRVAL(retval),Z_STRLEN(retval));

    zval_dtor(&retval);

    return oid;
}

// reference_foreach_callback

int reference_foreach_callback::callback(git_reference* ref,void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(pc);
#endif

    int result;
    zval retval;
    zval_array<2> params ZTS_CTOR;
    const php_resource_ref<php_git_reference> res;

    // Convert arguments to PHP values.
    *res.byval_git2() = ref;
    res.ret(params[0]);
    params.assign<1>(cb->data);

    // Call the userspace callback.
    result = params.call(cb->func,&retval);
    zval_dtor(&retval);

    return result;
}

// reference_foreach_name_callback

int reference_foreach_name_callback::callback(const char* name,void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(pc);
#endif

    int result;
    zval retval;
    zval_array<2> params ZTS_CTOR;

    // Convert arguments to PHP values.
    params.assign<0>(name,cb->data);

    // Call the userspace callback.
    result = params.call(cb->func,&retval);
    zval_dtor(&retval);

    return result;
}

// packbuilder_progress_callback

int packbuilder_progress_callback::callback(int stage,uint32_t current,uint32_t total,void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    int result;
    zval retval;
    zval_array<4> params ZTS_CTOR;

    // Convert arguments to PHP values.
    params.assign<0>(
        static_cast<long>(stage),
        static_cast<long>(current),
        static_cast<long>(total),
        cb->data);

    // Call the userspace callback.
    result = params.call(cb->func,&retval);
    zval_dtor(&retval);

    return result;
}

// config_foreach_callback

int config_foreach_callback::callback(const git_config_entry* entry,void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    int result;
    bool returnValue;
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
    result = params.call(cb->func,&retval);
    if (result == GIT_OK) {
        convert_to_boolean(&retval);
        returnValue = Z_BVAL(retval);
        zval_dtor(&retval);

        result = returnValue ? 1 : 0;
    }

    return result;
}

// tag_foreach_callback

int tag_foreach_callback::callback(const char* name,git_oid* oid,void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    int result;
    zval retval;
    char buf[GIT_OID_HEXSZ+1];
    zval_array<3> params ZTS_CTOR;

    git_oid_tostr(buf,sizeof(buf),oid);
    params.assign<0>(name,buf,cb->data);

    result = params.call(cb->func,&retval);
    zval_dtor(&retval);

    return result;
}

// repository_create_callback

int repository_create_callback::callback(git_repository** out,const char* path,int bare,void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    int result;
    zval retval;
    zval_array<3> params ZTS_CTOR;

    params.assign<0>(path,static_cast<bool>(bare),cb->data);
    result = params.call(cb->func,&retval);

    // Handle errors from userspace.
    if (result < 0) {
        return result;
    }

    // Allow userspace to generate failure by returning null or false.
    if (Z_TYPE(retval) == IS_NULL || (Z_TYPE(retval) == IS_BOOL && !Z_BVAL(retval))) {
        giterr_set_str(GITERR_INVALID,"repository_create_callback: failed to create repository");
        return GIT_ERROR;
    }

    // Make sure a git_repository resource was returned.
    if (Z_TYPE(retval) != IS_RESOURCE) {
        giterr_set_str(GITERR_INVALID,"repository_create_callback: must return resource");
        return GIT_ERROR;
    }

    // Extract resource from return value.
    php_git_repository* resource;
    resource = (php_git_repository*)zend_fetch_resource(
        NULL TSRMLS_CC,
        Z_RESVAL(retval),
        NULL,
        NULL,
        1,
        php_git_repository::resource_le());
    if (resource == nullptr) {
        // NOTE: PHP prints warning if resource type didn't match.
        giterr_set_str(GITERR_INVALID,"repository_create_callback: must return git_repository resource");
        return GIT_ERROR;
    }

    // The repository resource *MUST* be owned (i.e. this resource is
    // responsible for freeing the handle). We assume that the caller is going
    // to return the repository as an owned resource (potentially), and at the
    // very least, the caller is responsible for freeing the repo handle we
    // assign here. Therefore, we cannot have a git_repository handle from a
    // non-owned context (since it could be freed in another context at some
    // future point). Furthermore, we must revoke ownership on this resource to
    // avoid a double free.

    if (!resource->is_owned()) {
        giterr_set_str(GITERR_INVALID,"repository_create_callback: cannot return non-owner resource");
        return GIT_ERROR;
    }

    resource->revoke_ownership();
    *out = resource->get_handle();

    return GIT_OK;
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

    php_callback_base* cb = &info->notifyCallback;

    int result;
    zval retval;
    zval_array<4> params ZTS_CTOR;
    const php_resource_ref<php_git_diff_nofree> diffRes;

    *diffRes.byval_git2() = diff_so_far;
    diffRes.ret(params[0]);
    convert_diff_delta(params[1],delta_to_add);
    params.assign<2>(matched_pathspec,cb->data);
    result = params.call(cb->func,&retval);

    // Handle errors. It is unclear how this callback is to report errors (if at
    // all), so we just report end of operation.
    if (result < 0) {
        // Flag propagated fatal errors globally so they can be issued later on
        // by a bailout context. We do this since libgit2 will NOT report the
        // error to the calling PHP context.
        if (result == GIT_EPHPFATALPROP) {
            GIT2_G(propagateFatalError) = true;
        }

        return -1;
    }

    convert_to_long(&retval);
    result = static_cast<int>(Z_LVAL(retval));
    zval_dtor(&retval);

    return result;
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

    php_callback_base* cb = &info->progressCallback;

    int result;
    zval retval;
    zval_array<4> params;
    const php_resource_ref<php_git_diff_nofree> diffRes;

    *diffRes.byval_git2() = diff_so_far;
    diffRes.ret(params[0]);
    params.assign<1>(old_path,new_path,cb->data);
    result = params.call(cb->func,&retval);

    // Handle errors. It is unclear how this callback is to report errors (if at
    // all), so we just report end of operation.
    if (result < 0) {
        // Flag propagated fatal errors globally so they can be issued later on
        // by a bailout context. We do this since libgit2 will NOT report the
        // error to the calling PHP context.
        if (result == GIT_EPHPFATALPROP) {
            GIT2_G(propagateFatalError) = true;
        }

        result = 1;
    }
    else {
        if (Z_TYPE(retval) != IS_NULL) {
            convert_to_boolean(&retval);
            result = Z_BVAL(retval) ? 0 : 1;
        }
        else {
            result = 0;
        }
    }

    zval_dtor(&retval);

    return result;
}

// diff_file_callback

int diff_file_callback::callback(const git_diff_delta* delta,float progress,void* payload)
{
    git_diff_callback_info* info;
    info = reinterpret_cast<git_diff_callback_info*>(payload);

    php_callback_base* cb = &info->fileCallback;

    int result;
    zval retval;
    zval_array<3> params;

    convert_diff_delta(params[0],delta);
    params.assign<1>(progress,info->zpayload);
    result = params.call(cb->func,&retval);
    zval_dtor(&retval);

    return result;
}

// diff_binary_callback

int diff_binary_callback::callback(const git_diff_delta* delta,
    const git_diff_binary* binary,
    void* payload)
{
    git_diff_callback_info* info;
    info = reinterpret_cast<git_diff_callback_info*>(payload);

    php_callback_base* cb = &info->binaryCallback;

    int result;
    zval retval;
    zval_array<3> params;

    convert_diff_delta(params[0],delta);
    convert_diff_binary(params[1],binary);
    params.assign<2>(info->zpayload);
    result = params.call(cb->func,&retval);
    zval_dtor(&retval);

    return result;
}

// diff_hunk_callback

int diff_hunk_callback::callback(const git_diff_delta* delta,
    const git_diff_hunk* hunk,
    void* payload)
{
    git_diff_callback_info* info;
    info = reinterpret_cast<git_diff_callback_info*>(payload);

    php_callback_base* cb = &info->hunkCallback;

    int result;
    zval retval;
    zval_array<3> params;

    convert_diff_delta(params[0],delta);
    convert_diff_hunk(params[1],hunk);
    params.assign<2>(info->zpayload);
    result = params.call(cb->func,&retval);
    zval_dtor(&retval);

    return result;
}

// diff_line_callback

int diff_line_callback::callback(const git_diff_delta* delta,
    const git_diff_hunk* hunk,
    const git_diff_line* line,
    void* payload)
{
    git_diff_callback_info* info;
    info = reinterpret_cast<git_diff_callback_info*>(payload);

    php_callback_base* cb = &info->lineCallback;

    int result;
    zval retval;
    zval_array<4> params;

    convert_diff_delta(params[0],delta);
    convert_diff_hunk(params[1],hunk);
    convert_diff_line(params[2],line);
    params.assign<3>(info->zpayload);
    result = params.call(cb->func,&retval);
    zval_dtor(&retval);

    return result;
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

    int result;
    zval retval;
    zval_array<3> params ZTS_CTOR;

    params.assign<0>(path,matched_pathspec,cb->data);
    result = params.call(cb->func,&retval);
    zval_dtor(&retval);

    return result;
}

// revwalk_hide_callback

int revwalk_hide_callback::callback(const git_oid* commit_id,void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    int result;
    zval retval;
    zval_array<2> params ZTS_CTOR;
    char buf[GIT_OID_HEXSZ + 1];

    git_oid_tostr(buf,sizeof(buf),commit_id);
    params.assign<0>(buf,cb->data);

    result = params.call(cb->func,&retval);
    zval_dtor(&retval);

    return result;
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

    int result;
    zval retval;
    zval_array<3> params ZTS_CTOR;

    // NOTE: 'value' may be nullptr. The zval_array will handle this
    // accordingly.
    params.assign<0>(name,value,cb->data);

    result = params.call(cb->func,&retval);
    zval_dtor(&retval);

    return result;
}

// status_callback

int status_callback::callback(const char* path,unsigned int status_flags,void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    int result;
    zval retval;
    zval_array<3> params ZTS_CTOR;

    params.assign<0>(path,status_flags,cb->data);

    result = params.call(cb->func,&retval);
    zval_dtor(&retval);

    return result;
}

// note_foreach_callback

int note_foreach_callback::callback(const git_oid* blob_id,
    const git_oid* annotated_object_id,void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    int result;
    zval retval;
    zval_array<2> params ZTS_CTOR;
    char buf[GIT_OID_HEXSZ + 1];

    git_oid_tostr(buf,sizeof(buf),blob_id);
    ZVAL_STRING(params[0],buf,1);
    git_oid_tostr(buf,sizeof(buf),annotated_object_id);
    ZVAL_STRING(params[1],buf,1);
    params.assign<2>(cb->data);

    result = params.call(cb->func,&retval);
    zval_dtor(&retval);

    return result;
}

// stash_callback

int stash_callback::callback(size_t index,
    const char* message,
    const git_oid* stash_id,
    void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    int result;
    zval retval;
    zval_array<4> params ZTS_CTOR;
    char buf[GIT_OID_HEXSZ + 1];

    params.assign<0>(index,message);
    git_oid_tostr(buf,sizeof(buf),stash_id);
    ZVAL_STRING(params[2],buf,1);
    params.assign<3>(cb->data);

    result = params.call(cb->func,&retval);

    if (result == GIT_OK) {
        if (Z_TYPE(retval) != IS_NULL) {
            convert_to_boolean(&retval);
            result = Z_BVAL(retval) ? 0 : -1;
        }
    }

    zval_dtor(&retval);

    return result;
}

// stash_apply_progress_callback

int stash_apply_progress_callback::callback(
    git_stash_apply_progress_t progress,
    void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    int result;
    zval retval;
    zval_array<2> params ZTS_CTOR;

    params.assign<0>(static_cast<long>(progress),cb->data);
    result = params.call(cb->func,&retval);

    if (result == GIT_OK) {
        if (Z_TYPE(retval) != IS_NULL) {
            convert_to_boolean(&retval);
            result = Z_BVAL(retval) ? 0 : -1;
        }
    }

    zval_dtor(&retval);

    return result;
}

// cred_acquire_callback

int cred_acquire_callback::callback(git_cred** cred,
    const char* url,
    const char* username_from_url,
    unsigned int allowed_types,
    void* payload)
{
    php_callback_base* cb = reinterpret_cast<php_callback_base*>(payload);

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    int result;
    zval retval;
    zval_array<4> params ZTS_CTOR;

    params.assign<0>(url,username_from_url,allowed_types,cb->data);
    result = params.call(cb->func,&retval);

    if (result == GIT_OK) {
        if (Z_TYPE(retval) == IS_RESOURCE) {
            php_git_cred* resource;
            resource = (php_git_cred*)zend_fetch_resource(
                NULL TSRMLS_CC,
                Z_RESVAL(retval),
                NULL,
                NULL,
                1,
                php_git_cred::resource_le());

            if (resource == nullptr) {
                // NOTE: PHP prints warning if resource type didn't match.
                giterr_set_str(GITERR_INVALID,"cred_acquire_callback: must return git_cred resource");
                result = GIT_ERROR;
            }
            else if (!resource->is_owned()) {
                // Check the ownership status for sanity's sake. All git_cred
                // resources should be owned.
                giterr_set_str(GITERR_INVALID,"cred_acquire_callback: cannot return non-owner resource");
                result = GIT_ERROR;
            }
            else {
                // Revoke ownership since it is now up to the library to manage
                // the credential; then assign the credential to the output
                // parameter.
                resource->revoke_ownership();
                *cred = resource->get_handle();
            }
        }
        else {
            result = GIT_PASSTHROUGH;
        }
    }

    zval_dtor(&retval);

    return result;
}

// transport_certificate_check_callback

int transport_certificate_check_callback::callback(git_cert* cert,
    int valid,
    const char* host,
    void* payload)
{
    php_callback_base* cb = reinterpret_cast<php_callback_base*>(payload);

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    int result;
    zval retval;
    zval_array<4> params ZTS_CTOR;

    convert_cert(params[0],cert);
    params.assign<1>(static_cast<bool>(valid),host,cb->data);
    result = params.call(cb->func,&retval);

    if (result == GIT_OK) {
        convert_to_boolean(&retval);
        result = (Z_BVAL(retval)) ? 1 : 0;
    }

    zval_dtor(&retval);

    return result;
}

// remote_transport_message_callback

int remote_transport_message_callback::callback(const char* str,int len,void* payload)
{
    git_remote_callbacks_info* info = reinterpret_cast<git_remote_callbacks_info*>(payload);
    php_callback_base* cb = &info->transportMessageCallback;

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    int result;
    zval retval;
    zval_array<2> params ZTS_CTOR;

    params.assign<0>(static_cast<const void*>(str),static_cast<size_t>(len),cb->data);
    result = params.call(cb->func,&retval);

    if (result == GIT_OK) {
        if (Z_TYPE(retval) != IS_NULL) {
            convert_to_boolean(&retval);
            result = Z_BVAL(retval) ? 0 : -1;
        }
    }

    zval_dtor(&retval);

    return result;
}

// remote_completion_callback

int remote_completion_callback::callback(
    git_remote_completion_type type,
    void* payload)
{
    git_remote_callbacks_info* info = reinterpret_cast<git_remote_callbacks_info*>(payload);
    php_callback_base* cb = &info->transportMessageCallback;

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    int result;
    zval retval;
    zval_array<2> params ZTS_CTOR;

    params.assign<0>(static_cast<long>(type),cb->data);
    result = params.call(cb->func,&retval);

    zval_dtor(&retval);

    return result;
}

// remote_cred_acquire_callback

int remote_cred_acquire_callback::callback(git_cred** cred,
    const char* url,
    const char* username_from_url,
    unsigned int allowed_types,
    void* payload)
{
    git_remote_callbacks_info* info = reinterpret_cast<git_remote_callbacks_info*>(payload);
    php_callback_base* cb = &info->credAcquireCallback;

    // Implement callback in terms of existing implementation.

    return cred_acquire_callback::callback(cred,url,username_from_url,allowed_types,cb);
}

// remote_transport_certificate_check_callback

int remote_transport_certificate_check_callback::callback(git_cert* cert,
    int valid,
    const char* host,
    void* payload)
{
    git_remote_callbacks_info* info = reinterpret_cast<git_remote_callbacks_info*>(payload);
    php_callback_base* cb = &info->transportCertificateCheckCallback;

    // Implement callback in terms of existing implementation.

    return transport_certificate_check_callback::callback(cert,valid,host,cb);
}

// remote_transfer_progress_callback

int remote_transfer_progress_callback::callback(
    const git_transfer_progress* stats,
    void* payload)
{
    git_remote_callbacks_info* info = reinterpret_cast<git_remote_callbacks_info*>(payload);
    php_callback_base* cb = &info->transportCertificateCheckCallback;

    // Implement this callback in terms of the already defined transfer_progress
    // callback.

    return transfer_progress_callback::callback(stats,cb);
}

// remote_update_tips_callback

int remote_update_tips_callback::callback(
    const char* refname,
    const git_oid* a,
    const git_oid* b,
    void* payload)
{
    git_remote_callbacks_info* info = reinterpret_cast<git_remote_callbacks_info*>(payload);
    php_callback_base* cb = &info->updateTipsCallback;

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    int result;
    zval retval;
    zval_array<4> params ZTS_CTOR;

    params.assign<0>(refname);
    convert_oid(params[1],a);
    convert_oid(params[2],b);
    params.assign<3>(cb->data);
    result = params.call(cb->func,&retval);

    zval_dtor(&retval);

    return result;
}

// remote_packbuilder_progress_callback

int remote_packbuilder_progress_callback::callback(
    int stage,
    uint32_t current,
    uint32_t total,
    void* payload)
{
    git_remote_callbacks_info* info = reinterpret_cast<git_remote_callbacks_info*>(payload);
    php_callback_base* cb = &info->packbuilderProgressCallback;

    // Implement this callback in terms of the already defined
    // packbuilder_progress callback.

    return packbuilder_progress_callback::callback(stage,current,total,cb);
}

// remote_push_transfer_progress_callback

int remote_push_transfer_progress_callback::callback(
    unsigned int current,
    unsigned int total,
    size_t bytes,
    void* payload)
{
    git_remote_callbacks_info* info = reinterpret_cast<git_remote_callbacks_info*>(payload);
    php_callback_base* cb = &info->pushTransferProgressCallback;

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    int result;
    zval retval;
    zval_array<4> params ZTS_CTOR;

    params.assign<0>(
        static_cast<long>(current),
        static_cast<long>(total),
        static_cast<long>(bytes),
        cb->data);
    result = params.call(cb->func,&retval);

    zval_dtor(&retval);

    return result;
}

// remote_push_update_reference_callback

int remote_push_update_reference_callback::callback(
    const char* refname,
    const char* status,
    void* payload)
{
    git_remote_callbacks_info* info = reinterpret_cast<git_remote_callbacks_info*>(payload);
    php_callback_base* cb = &info->pushUpdateReferenceCallback;

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    int result;
    zval retval;
    zval_array<3> params ZTS_CTOR;

    params.assign<0>(refname);
    if (status != nullptr) {
        params.assign<1>(status);
    }
    params.assign<2>(cb->data);
    result = params.call(cb->func,&retval);

    zval_dtor(&retval);

    return result;
}

// remote_push_negotiation_callback

int remote_push_negotiation_callback::callback(
    const git_push_update** updates,
    size_t len,
    void *payload)
{
    git_remote_callbacks_info* info = reinterpret_cast<git_remote_callbacks_info*>(payload);
    php_callback_base* cb = &info->pushNegotiationCallback;

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    int result;
    zval retval;
    zval_array<2> params ZTS_CTOR;

    array_init(params[0]);
    for (size_t i = 0;i < len;++i) {
        zval* zv;
        MAKE_STD_ZVAL(zv);
        convert_push_update(zv,updates[i]);

        add_next_index_zval(params[0],zv);
    }

    params.assign<1>(cb->data);
    result = params.call(cb->func,&retval);

    zval_dtor(&retval);

    return result;
}

// proxy_cred_acquire_callback

int proxy_cred_acquire_callback::callback(git_cred** cred,
    const char* url,
    const char* username_from_url,
    unsigned int allowed_types,
    void* payload)
{
    git_proxy_callbacks_info* info = reinterpret_cast<git_proxy_callbacks_info*>(payload);
    php_callback_base* cb = &info->credAcquireCallback;

    // Implement callback in terms of existing implementation.

    return cred_acquire_callback::callback(cred,url,username_from_url,allowed_types,cb);
}

// proxy_transport_certificate_check_callback

int proxy_transport_certificate_check_callback::callback(git_cert* cert,
    int valid,
    const char* host,
    void* payload)
{
    git_proxy_callbacks_info* info = reinterpret_cast<git_proxy_callbacks_info*>(payload);
    php_callback_base* cb = &info->transportCertificateCheckCallback;

    // Implement callback in terms of existing implementation.

    return transport_certificate_check_callback::callback(cert,valid,host,cb);
}

// remote_create_callback

int remote_create_callback::callback(
    git_remote** out,
    git_repository* repo,
    const char* name,
    const char* url,
    void* payload)
{
    php_callback_base* cb = reinterpret_cast<php_callback_base*>(payload);

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    int result;
    zval retval;
    zval_array<4> params ZTS_CTOR;
    php_resource_ref<php_git_repository_nofree> repoResource;

    repoResource.set_object(repo);
    repoResource.ret(params[0]);
    params.assign<1>(name,url,cb->data);
    result = params.call(cb->func,&retval);

    if (result == GIT_OK) {
        if (Z_TYPE(retval) == IS_RESOURCE) {
            php_git_remote* resource;
            resource = (php_git_remote*)zend_fetch_resource(
                NULL TSRMLS_CC,
                Z_RESVAL(retval),
                NULL,
                NULL,
                1,
                php_git_remote::resource_le());

            if (resource == nullptr) {
                // NOTE: PHP prints warning if resource type didn't match.
                giterr_set_str(GITERR_INVALID,"remote_create_callback: must return git_remote resource");
                result = GIT_ERROR;
            }
            else if (!resource->is_owned()) {
                // Check the ownership status for sanity's sake. All git_remote
                // resources created by the callback should be owned.
                giterr_set_str(GITERR_INVALID,"remote_create_callback: cannot return non-owner resource");
                result = GIT_ERROR;
            }
            else {
                // Revoke ownership since it is now up to the library to manage
                // the remoteential; then assign the remoteential to the output
                // parameter.
                resource->revoke_ownership();
                *out = resource->get_handle();
            }
        }
        else {
            convert_to_long(&retval);
            if (Z_LVAL(retval) == 0) {
                giterr_set_str(GITERR_INVALID,"remote_create_callback: invalid return value");
                result = GIT_ERROR;
            }
            else {
                result = Z_LVAL(retval);
            }
        }
    }

    zval_dtor(&retval);

    return result;
}

// repository_fetchhead_foreach_callback

int repository_fetchhead_foreach_callback::callback(
    const char* ref_name,
    const char* remote_url,
    const git_oid* oid,
    unsigned int is_merge,
    void* payload)
{
    php_callback_base* cb = reinterpret_cast<php_callback_base*>(payload);

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    int result;
    zval retval;
    zval_array<5> params ZTS_CTOR;

    params.assign<0>(ref_name,remote_url);
    convert_oid(params[2],oid);
    params.assign<3>(static_cast<bool>(is_merge),cb->data);

    result = params.call(cb->func,&retval);
    zval_dtor(&retval);

    return result;
}

// repository_mergehead_foreach_callback

int repository_mergehead_foreach_callback::callback(
    const git_oid* oid,
    void* payload)
{
    php_callback_base* cb = reinterpret_cast<php_callback_base*>(payload);

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    int result;
    zval retval;
    zval_array<2> params ZTS_CTOR;

    convert_oid(params[0],oid);
    params.assign<1>(cb->data);

    result = params.call(cb->func,&retval);
    zval_dtor(&retval);

    return result;
}

// treebuilder_filter_callback

int treebuilder_filter_callback::callback(
    const git_tree_entry* entry,
    void* payload)
{
    php_callback_base* cb = reinterpret_cast<php_callback_base*>(payload);

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(cb);
#endif

    int result;
    zval retval;
    zval_array<2> params ZTS_CTOR;
    const php_resource_ref<php_git_tree_entry_nofree> res; // it cannot free

    res.set_object(entry);
    res.ret(params[0]);
    params.assign<1>(cb->data);

    result = params.call(cb->func,&retval);
    zval_dtor(&retval);

    return result;
}

int submodule_foreach_callback::callback(
    git_submodule* sm,
    const char* name,
    void* payload)
{
    php_callback_sync* cb = reinterpret_cast<php_callback_sync*>(payload);

#ifdef ZTS
    TSRMLS_D = ZTS_MEMBER_PC(pc);
#endif

    int result;
    zval retval;
    zval_array<3> params ZTS_CTOR;
    const php_resource_ref<php_git_submodule> res;

    // Convert arguments to PHP values.
    *res.byval_git2() = sm;
    res.ret(params[0]);
    params.assign<1>(name,cb->data);

    // Call the userspace callback.
    result = params.call(cb->func,&retval);
    zval_dtor(&retval);

    return result;
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
