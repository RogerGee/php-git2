#
# config.m4 - php-git2
#

PHP_ARG_WITH(git2, Whether to include "git2" support,
    [  --with-git2 [=DIR]        Include "git2" extension support; DIR is the install
                          prefix for the libgit2 library])

PHP_ARG_WITH(git2-static, Whether to include "git2" support (static libraries),
    [  --with-git2-static      Force using static libgit2], no, no)

# Here we add the libgit2 library to the build.
if test "$PHP_GIT2" != "no"; then
    # Compile list of directories to search for libgit2.
    GIT2_LIBGIT2_SEARCH_DIRS="/usr/local /usr"
    if test "$PHP_GIT2" != "yes"; then
        GIT2_LIBGIT2_SEARCH_DIRS="$PHP_GIT2 $GIT2_LIBGIT2_SEARCH_DIRS"
    fi

    AC_MSG_CHECKING([for libgit2 git2.h header])
    for i in $GIT2_LIBGIT2_SEARCH_DIRS; do
      test -f $i/include/git2.h && GIT2_LIBGIT2_DIR=$i && break
    done

    if test -z "$GIT2_LIBGIT2_DIR"; then
        AC_MSG_RESULT([not found])
        AC_MSG_ERROR([required libgit2 header git2.h not found])
    fi
    AC_MSG_RESULT([yes])

    if test "$PHP_GIT2_STATIC" != "no"; then
        # Find static archive and add to build.
        AC_MSG_CHECKING([for libgit2 libgit2.a static library])
        if ! test -f "$GIT2_LIBGIT2_DIR/lib/libgit2.a"; then
            AC_MSG_RESULT([not found])
            AC_MSG_ERROR([required libgit2 static library libgit2.a not found])
        fi
        AC_MSG_RESULT([yes])

        PHP_ADD_INCLUDE($GIT2_LIBGIT2_DIR/include)
        LDFLAGS+="$GIT2_LIBGIT2_DIR/lib/libgit2.a"
    else
        # Find shared library and add to build.
        PHP_CHECK_LIBRARY(git2,git_libgit2_version,
        [
            PHP_ADD_INCLUDE($GIT2_LIBGIT2_DIR/include)
            PHP_ADD_LIBRARY_WITH_PATH(git2, $GIT2_LIBGIT2_DIR/lib, GIT2_SHARED_LIBADD)
        ],
        [
            AC_MSG_ERROR([Please install libgit2 on the system])
        ],
        [
          -L$GIT2_LIBGIT2_DIR/lib
        ])
    fi
fi

if test $PHP_GIT2 != "no"; then
    PHP_REQUIRE_CXX()
    PHP_ADD_LIBRARY(stdc++, 1, GIT2_SHARED_LIBADD)

    # Force c++11 mode.
    CXXFLAGS+=" -std=c++11"

    # Add PHP_RPATHS to extension build via EXTRA_LDFLAGS.
    if test "$PHP_RPATHS" != ""; then
        PHP_UTILIZE_RPATHS()
        EXTRA_LDFLAGS="$PHP_RPATHS"
        PHP_SUBST([EXTRA_LDFLAGS])
    fi

    PHP_SUBST([CXXFLAGS])
    PHP_SUBST([GIT2_SHARED_LIBADD])

    PHP_NEW_EXTENSION(git2,php-git2.cpp \
        php-git2-fe.cpp \
        php-function.cpp \
        php-type.cpp \
        php-constants.cpp \
        php-callback.cpp \
        php-object.cpp \
        php-odb-backend.cpp \
        php-odb-backend-internal.cpp \
        php-odb-writepack.cpp \
        php-odb-writepack-internal.cpp \
        php-odb-stream.cpp \
        php-odb-stream-internal.cpp \
        php-writestream.cpp \
        php-config-backend.cpp \
        php-array.cpp \
        php-closure.cpp \
        php-refdb-backend.cpp \
        php-refdb-backend-internal.cpp,$ext_shared)
fi

#
# Local Variables:
# indent-tabs-mode:nil
# tab-width:4
# End:
#
