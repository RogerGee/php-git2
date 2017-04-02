/*
 * php-odb-stream.cpp
 *
 * This file is a part of php-git2.
 */

#include "php-object.h"
using namespace php_git2;

// Helpers

static void convert_oid_fromstr(git_oid* dest,const char* src,int srclen)
{
    // Use a temporary buffer to hold the OID hex string. We make sure it
    // contains a string with an exact length of 40 characters.
    char buf[GIT_OID_HEXSZ + 1];
    memset(buf,'0',GIT_OID_HEXSZ);
    buf[GIT_OID_HEXSZ] = 0;
    if (srclen > GIT_OID_HEXSZ) {
        srclen = GIT_OID_HEXSZ;
    }
    strncpy(buf,src,srclen);
    git_oid_fromstr(dest,buf);
}

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

/*static*/ void php_odb_stream_object::init(zend_class_entry* ce)
{
    // Register properties.
    zend_declare_property_long(ce,"mode",sizeof("mode")-1,0,ZEND_ACC_PUBLIC);
    zend_declare_property_long(ce,"declared_size",sizeof("declared_size")-1,0,
        ZEND_ACC_PUBLIC);
    zend_declare_property_long(ce,"received_bytes",sizeof("received_bytes")-1,0,
        ZEND_ACC_PUBLIC);
}

// Implementation of class methods

PHP_METHOD(GitODBStream,read)
{
    /* NOTE: it isn't actually clear how useful this method (and its wrapper,
     * git_odb_stream_read) are. The documentation seems to indicate that they
     * are not to be used. Plus it is not obvious how to obtain the number of
     * bytes read. For now these functions will return a string zval with the
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
    } catch (php_git2_exception ex) {
        zend_throw_exception(nullptr,ex.what(),0 TSRMLS_CC);
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
    } catch (php_git2_exception ex) {
        zend_throw_exception(nullptr,ex.what(),0 TSRMLS_CC);
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
    } catch (php_git2_exception ex) {
        zend_throw_exception(nullptr,ex.what(),0 TSRMLS_CC);
        return;
    }
}

PHP_METHOD(GitODBStream,free)
{
    // Do nothing: do not allow the user to free in this way. The git_odb_stream
    // will be freed later on when PHP destroys the GitODBStream object.
}
