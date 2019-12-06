# config.m4

PHP_ARG_ENABLE(git2, Whether to enable the "git2" extension,
	[  --enable-git2      Enable "git2" extension support])

if test $PHP_GIT2 != "no"; then
    PHP_REQUIRE_CXX()
    PHP_ADD_LIBRARY(stdc++, 1, GIT2_SHARED_LIBADD)

    # Force c++11 mode.
    CXXFLAGS+=" -std=c++11"

    # Here we add the libgit2 library to the build.
    PHP_CHECK_LIBRARY(git2,git_libgit2_version,[
        PHP_ADD_LIBRARY(git2, 1, GIT2_SHARED_LIBADD)
    ],[
        AC_MSG_ERROR([Please install libgit2 on the system])
    ],
    [])

    # Add PHP_RPATHS to extension build via EXTRA_LDFLAGS.
    if test $PHP_RPATHS != ""; then
        PHP_UTILIZE_RPATHS()
        EXTRA_LDFLAGS="$PHP_RPATHS"
        PHP_SUBST([EXTRA_LDFLAGS])
    fi

    PHP_SUBST([CXXFLAGS])
    PHP_SUBST([GIT2_SHARED_LIBADD])

    PHP_NEW_EXTENSION(git2,php-git2.cpp php-git2-fe.cpp php-constants.cpp php-callback.cpp \
        php-object.cpp php-odb-backend.cpp php-odb-backend-internal.cpp php-odb-writepack.cpp  \
        php-odb-writepack-internal.cpp php-odb-stream.cpp php-odb-stream-internal.cpp \
        php-writestream.cpp php-config-backend.cpp php-array.cpp php-closure.cpp \
        php-refdb-backend.cpp php-refdb-backend-internal.cpp,$ext_shared)
fi

#
# Local Variables:
# indent-tabs-mode:nil
# tab-width:4
# End:
#
