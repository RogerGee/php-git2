/*
 * repository.h
 *
 * This file is a part of php-git.
 */

#ifndef PHPGIT2_REPOSITORY_H
#define PHPGIT2_REPOSITORY_H
#include "php-function.h"

namespace php_git2
{
    // Explicitly specialize git2_resource destructor for git_repository.
    template<> git2_resource<git_repository>::~git2_resource()
    { git_repository_free(handle); }

}

#endif
