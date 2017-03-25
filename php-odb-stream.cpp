/*
 * php-odb-stream.cpp
 *
 * This file is a part of php-git2.
 */

#include "php-object.h"
using namespace php_git2;

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

void php_git2::php_git2_make_odb_stream(zval* zp,git_odb_stream* stream TSRMLS_DC)
{
    php_odb_stream_object* obj;
    zend_class_entry* ce = php_git2::class_entry[php_git2_odb_stream_obj];

    // Create object zval.
    object_init_ex(zp,ce);
    obj = reinterpret_cast<php_odb_stream_object*>(zend_objects_get_address(zp TSRMLS_CC));

    // Assign the stream handle.
    obj->stream = stream;
}

// Implementation of php_odb_stream_object

/*static*/ zend_object_handlers php_odb_stream_object::handlers;
php_odb_stream_object::php_odb_stream_object(zend_class_entry* ce TSRMLS_DC):
    stream(nullptr), zts(TSRMLS_C)
{
    zend_object_std_init(&base,ce TSRMLS_CC);
    object_properties_init(&base,ce);
}

php_odb_stream_object::~php_odb_stream_object()
{
    if (stream != nullptr) {
        stream->free(stream);
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
            git_error();
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
            git_error();
        }
    } catch (php_git2_exception ex) {
        zend_throw_exception(nullptr,ex.what(),0 TSRMLS_CC);
        return;
    }
}

PHP_METHOD(GitODBStream,finalize_write)
{
    int retval;
    git_oid oid;
    char buf[GIT_OID_HEXSZ + 1];
    php_odb_stream_object* object = LOOKUP_OBJECT(php_odb_stream_object,getThis());

    // Stream must be created and the function must be implemented.
    if (object->stream == nullptr || object->stream->finalize_write == nullptr) {
        php_error(E_ERROR,"GitODBStream::finalize_write(): method is not available");
        return;
    }

    // Call underlying function.
    try {
        retval = object->stream->finalize_write(object->stream,&oid);
        if (retval < 0) {
            git_error();
        }
    } catch (php_git2_exception ex) {
        zend_throw_exception(nullptr,ex.what(),0 TSRMLS_CC);
        return;
    }

    // Convert OID to hex string and return it.
    git_oid_tostr(buf,sizeof(buf),&oid);
    RETVAL_STRING(buf,1);
}

PHP_METHOD(GitODBStream,free)
{
    php_odb_stream_object* object = LOOKUP_OBJECT(php_odb_stream_object,getThis());

    // Stream must be created and the function must be implemented.
    if (object->stream == nullptr || object->stream->finalize_write == nullptr) {
        php_error(E_ERROR,"GitODBStream::free(): method is not available");
        return;
    }

    // Call underlying function. Make sure to mark the object as having been
    // destroyed.
    object->stream->free(object->stream);
    object->stream = nullptr;
}
