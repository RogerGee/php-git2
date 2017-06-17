/*
 * php-odb-stream.cpp
 *
 * This file is a part of php-git2.
 */

#include "php-object.h"
using namespace php_git2;

// Helper macros

#define EXTRACT_STREAM(stream) \
    reinterpret_cast<php_odb_stream_object::git_odb_stream_php*>(stream)

#define EXTRACT_THISOBJ(stream) \
    EXTRACT_STREAM(stream)->thisobj

// Class method entries
static PHP_METHOD(GitODBStream,read);
static PHP_METHOD(GitODBStream,write);
static PHP_METHOD(GitODBStream,finalize_write);
static PHP_METHOD(GitODBStream,free);
zend_function_entry php_git2::odb_stream_methods[] = {
    PHP_ME(GitODBStream,read,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBStream,write,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBStream,finalize_write,NULL,ZEND_ACC_PUBLIC)
    PHP_ME(GitODBStream,free,NULL,ZEND_ACC_PUBLIC)
    PHP_FE_END
};

// Make function implementation

void php_git2::php_git2_make_odb_stream(zval* zp,git_odb_stream* stream,bool isstd TSRMLS_DC)
{
    php_odb_stream_object* obj;
    zend_class_entry* ce = php_git2::class_entry[php_git2_odb_stream_obj];

    // Create object zval.
    object_init_ex(zp,ce);
    obj = reinterpret_cast<php_odb_stream_object*>(zend_objects_get_address(zp TSRMLS_CC));

    // Assign the stream handle.
    obj->stream = stream;
    obj->isstd = isstd;
}

// Implementation of php_odb_stream_object

/*static*/ zend_object_handlers php_odb_stream_object::handlers;
php_odb_stream_object::php_odb_stream_object(zend_class_entry* ce TSRMLS_DC):
    stream(nullptr), isstd(true), zts(TSRMLS_C)
{
    zend_object_std_init(&base,ce TSRMLS_CC);
    object_properties_init(&base,ce);
}

php_odb_stream_object::~php_odb_stream_object()
{
    if (stream != nullptr) {
        // The stream is standard if it was created with high level
        // functions. To prevent memory leaks we must call the high-level
        // git_odb_stream_free().
        if (isstd) {
            git_odb_stream_free(stream);
        }
        else {
            // Otherwise we assume the object was created outside of the
            // standard way and should not be freed using the higher-level
            // function. This is the case when PHP userspace directly calls a
            // stream method on a backend object. In this case the
            // git_odb_stream never was created by the high level functions and
            // just the backend functions.
            stream->free(stream);
        }
    }

    zend_object_std_dtor(&base ZTS_MEMBER_CC(zts));
}

void php_odb_stream_object::create_custom_stream(zval* zobj,unsigned int mode)
{
    // NOTE: the zval should be any zval that points to an object with 'this' as
    // its backing (i.e. result of zend_objects_get_address()). This is really
    // only used by the implementation to obtain class entry info for the class
    // that was used to create the object.

    // Free any existing stream.
    if (stream != nullptr) {
        if (isstd) {
            git_odb_stream_free(stream);
        }
        else {
            stream->free(stream);
        }
    }

    // Create new custom stream.
    stream = new (emalloc(sizeof(git_odb_stream_php))) git_odb_stream_php(zobj,mode);
    isstd = false;
}

/*static*/ void php_odb_stream_object::init(zend_class_entry* ce)
{
    // Register properties.
    zend_declare_property_long(ce,"mode",sizeof("mode")-1,0,ZEND_ACC_PUBLIC);
    zend_declare_property_long(ce,"declared_size",sizeof("declared_size")-1,0,
        ZEND_ACC_PUBLIC);
    zend_declare_property_long(ce,"received_bytes",sizeof("received_bytes")-1,0,
        ZEND_ACC_PUBLIC);
}

/*static*/ int php_odb_stream_object::read(git_odb_stream *stream,char *buffer,size_t len)
{
    zval fname;
    zval retval;
    zval* zlen;
    zval* params[1];
    zval* thisobj = EXTRACT_THISOBJ(stream);
    int result = GIT_OK;

    // We only need to lookup the object backing to get at the ZTS globals.
#ifdef ZTS
    php_odb_stream_object* object;
    object = LOOKUP_OBJECT(php_odb_backend_object,thisobj);
#endif

    // Initialize zvals.
    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    MAKE_STD_ZVAL(zlen);

    // Assign length to parameter zval.
    ZVAL_LONG(zlen,len);
    params[0] = zlen;

    // Assign method name.
    ZVAL_STRING(&fname,"read",1);

    // Call userspace method implementation of stream operation. The user
    // should have overridden the method in a derived class.
    if (call_user_function(NULL,&thisobj,&fname,&retval,1,params
            ZTS_MEMBER_CC(object->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }
    else {
        // Return values to caller. We copy the data into the git2-provided
        // buffer, making sure not to overflow.
        size_t n;
        convert_to_string(&retval);
        n = Z_STRLEN(retval);
        if (n > len) {
            n = len;
        }
        memcpy(buffer,Z_STRVAL(retval),n);
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zlen);
    return result;
}

/*static*/ int php_odb_stream_object::write(git_odb_stream *stream,const char *buffer,size_t len)
{
    zval fname;
    zval retval;
    zval* zbuf;
    zval* params[1];
    zval* thisobj = EXTRACT_THISOBJ(stream);
    int result = GIT_OK;

    // We only need to lookup the object backing to get at the ZTS globals.
#ifdef ZTS
    php_odb_stream_object* object;
    object = LOOKUP_OBJECT(php_odb_backend_object,thisobj);
#endif

    // Initialize zvals.
    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    MAKE_STD_ZVAL(zbuf);

    // Assign buffer to zval to pass into method call.
    ZVAL_STRINGL(zbuf,buffer,len,1);
    params[0] = zbuf;

    // Assign method name.
    ZVAL_STRING(&fname,"write",1);

    // Call userspace method implementation of stream operation. The user
    // should have overridden the method in a derived class.
    if (call_user_function(NULL,&thisobj,&fname,&retval,1,params
            ZTS_MEMBER_CC(object->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zbuf);
    return result;
}

/*static*/ int php_odb_stream_object::finalize_write(git_odb_stream *stream,const git_oid *oid)
{
    zval fname;
    zval retval;
    zval* zbuf;
    zval* params[1];
    zval* thisobj = EXTRACT_THISOBJ(stream);
    int result = GIT_OK;
    char buffer[GIT_OID_HEXSZ+1];

    // We only need to lookup the object backing to get at the ZTS globals.
#ifdef ZTS
    php_odb_stream_object* object;
    object = LOOKUP_OBJECT(php_odb_backend_object,thisobj);
#endif

    // Initialize zvals.
    INIT_ZVAL(fname);
    INIT_ZVAL(retval);
    MAKE_STD_ZVAL(zbuf);

    // Assign OID to parameter zval.
    git_oid_tostr(buffer,sizeof(buffer),oid);
    ZVAL_STRINGL(zbuf,buffer,GIT_OID_HEXSZ,1);
    params[0] = zbuf;

    // Assign method name.
    ZVAL_STRING(&fname,"finalize_write",1);

    // Call userspace method implementation of stream operation. The user
    // should have overridden the method in a derived class.
    if (call_user_function(NULL,&thisobj,&fname,&retval,1,params
            ZTS_MEMBER_CC(object->zts)) == FAILURE)
    {
        result = GIT_ERROR;
    }

    // Cleanup zvals.
    zval_dtor(&fname);
    zval_dtor(&retval);
    zval_ptr_dtor(&zbuf);
    return result;
}

/*static*/ void php_odb_stream_object::free(git_odb_stream *stream)
{
    // First mark the custom stream as having been deleted so as to avoid
    // infinite recursion. Then explicitly call the destructor on the custom
    // stream. Finally free the block of memory that holds the custom stream.

    git_odb_stream_php* wrapper = EXTRACT_STREAM(stream);
    php_odb_stream_object* obj = LOOKUP_OBJECT(php_odb_stream_object,wrapper->thisobj);
    obj->stream = nullptr;
    wrapper->~git_odb_stream_php();
    efree(stream);
}

// php_odb_stream::git_odb_stream_php

php_odb_stream_object::
git_odb_stream_php::git_odb_stream_php(zval* zv,unsigned int mode)
{
    // Blank out the base class (which is the structure defined in the git2
    // headers).
    memset(this,0,sizeof(struct git_odb_stream));

    // Copy the object zval so that we have a new zval we can track that refers
    // to the same specified object. We keep this zval alive as long as the
    // git_odb_stream is alive.
    MAKE_STD_ZVAL(thisobj);
    ZVAL_ZVAL(thisobj,zv,1,0);

    // Get the class entry for the (hopefully) derived class type.
    zend_class_entry* ce = Z_OBJCE_P(thisobj);

    // Set mode.

    // Make sure the class provided overridden methods needed for the requested
    // mode. If not we raise a fatal error.
    if (mode == GIT_STREAM_RDONLY) {
        if (!is_method_overridden(ce,"read",sizeof("read"))) {
            php_error(E_ERROR,"Custom GitODBStream must override read()");
            return;
        }
    }
    else if (mode == GIT_STREAM_WRONLY) {
        if (!is_method_overridden(ce,"write",sizeof("write"))) {
            php_error(E_ERROR,"Custom GitODBStream must override write()");
            return;
        }
        if (!is_method_overridden(ce,"finalize_write",sizeof("finalize_write"))) {
            php_error(E_ERROR,"Custom GitODBStream must override finalize_write()");
            return;
        }
    }

    // Every custom stream must have all the functions available.
    read = php_odb_stream_object::read;
    write = php_odb_stream_object::write;
    finalize_write = php_odb_stream_object::finalize_write;
    free = php_odb_stream_object::free;
}

php_odb_stream_object::
git_odb_stream_php::~git_odb_stream_php()
{
    // Free object zval.
    zval_ptr_dtor(&thisobj);
}

// Implementation of class methods

PHP_METHOD(GitODBStream,read)
{
    /* NOTE: it isn't actually clear how useful this method (and its wrapper,
     * git_odb_stream_read) really are. The documentation seems to indicate that
     * they are not to be used. Plus it is not obvious how to obtain the number
     * of bytes read. For now these functions will return a string zval with the
     * length of the allocated buffer.
     */

    int retval;
    char* buffer;
    long bufsz;
    php_odb_stream_object* object = LOOKUP_OBJECT(php_odb_stream_object,getThis());

    // Stream must be created and the function must be implemented.
    if (object->stream == nullptr || object->stream->read == nullptr) {
        php_error(E_ERROR,"GitODBStream::read(): method is not available");
        return;
    }

    // Parse parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"l",&bufsz) == FAILURE) {
        return;
    }
    if (bufsz <= 0) {
        bufsz = 4096;
    }

    // Call underlying function.
    try {
        buffer = (char*)emalloc(bufsz);
        retval = object->stream->read(object->stream,buffer,bufsz);
        if (retval < 0) {
            php_git2::git_error();
        }
    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }
        return;
    }

    RETVAL_STRINGL(buffer,bufsz,0);
}

PHP_METHOD(GitODBStream,write)
{
    int retval;
    char* buffer;
    int bufsz;
    php_odb_stream_object* object = LOOKUP_OBJECT(php_odb_stream_object,getThis());

    // Stream must be created and the function must be implemented.
    if (object->stream == nullptr || object->stream->write == nullptr) {
        php_error(E_ERROR,"GitODBStream::write(): method is not available");
        return;
    }

    // Parse parameters.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s",&buffer,&bufsz) == FAILURE) {
        return;
    }

    // Call underlying function.
    try {
        retval = object->stream->write(object->stream,buffer,bufsz);
        if (retval < 0) {
            php_git2::git_error();
        }
    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }
        return;
    }
}

PHP_METHOD(GitODBStream,finalize_write)
{
    int retval;
    char* input;
    int input_len;
    git_oid oid;
    php_odb_stream_object* object = LOOKUP_OBJECT(php_odb_stream_object,getThis());

    // Stream must be created and the function must be implemented.
    if (object->stream == nullptr || object->stream->finalize_write == nullptr) {
        php_error(E_ERROR,"GitODBStream::finalize_write(): method is not available");
        return;
    }

    // Read OID parameter from function arguments.
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"s",&input,&input_len) == FAILURE) {
        return;
    }

    // Convert OID string to binary structure.
    convert_oid_fromstr(&oid,input,input_len);

    // Call underlying function.
    try {
        retval = object->stream->finalize_write(object->stream,&oid);
        if (retval < 0) {
            php_git2::git_error();
        }
    } catch (php_git2::php_git2_exception_base& ex) {
        if (ex.what() != nullptr) {
            zend_throw_exception(nullptr,ex.what(),ex.code TSRMLS_CC);
        }
        return;
    }
}

PHP_METHOD(GitODBStream,free)
{
    // Do nothing: do not allow the user to free in this way. The git_odb_stream
    // will be freed later on when PHP destroys the GitODBStream object.
}

/*
 * Local Variables:
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
