/*
 * php-object.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_PHPOBJECT_H
#define PHPGIT2_PHPOBJECT_H
#include "php-callback.h"

namespace php_git2
{
    // Define the custom object types provided by php-git2. The order of the
    // elements in this enumeration maps to the internal order of each type's
    // corresponding class storage structure and thus is highly important.

    enum php_git2_object_t
    {
        php_git2_odb_writepack_obj,
        _php_git2_obj_top_
    };

    // Define zend_object derivations for objects that require an extended
    // backend structure.

    struct php_odb_writepack_object
    {
        php_odb_writepack_object();
        ~php_odb_writepack_object();

        zend_object base;
        git_odb_writepack* writepack;
        git_transfer_progress prog;
        php_callback_sync* cb;

        static zend_object_handlers handlers;
    };

    // Provide a routine to call during MINIT for registering the custom
    // classes.

    void php_git2_register_classes();

    // Provide functions for creating objects.

    void php_git2_make_object(zval* zp,php_git2_object_t type);
    void php_git2_make_odb_writepack(zval* zp,git_odb_writepack* writepack,
        php_callback_sync* cb);
}

#endif
