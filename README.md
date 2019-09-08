# php-git2

This project provides a PHP extension that exposes bindings for [`libgit2`](https://github.com/libgit2/libgit2). You can use this library to create and manipulate git repositories in a PHP application without having to invoke the `git(1)` command-line tool.

The extension currently targets PHP 5 (see Versioning) and has been tested with PHP 5.4, 5.5 and 5.6. PHP 7 support is in active development.

Primary author:

	Roger Gee <rpg11a@acu.edu>

## Versioning

Since we do not bundle the `libgit2` dependency, it's up to the user to provide the correct version of  `libgit2` required by their version of `php-git2` (either as a shared or static library). If you are dynamic linking on Linux, `php-git2` will embed the `soname` of whatever `libgit2` version you have configured into the resulting binary; in this way it will always load the correct version.

The following table details the versions of `libgit2` required by `php-git2`:

| `php-git2` | `libgit2` |
| -------- | ------- |
| `v1.0.x`  | `v0.25.1` |

We have chosen to require a particular `libgit2` minor version for a particular range of `php-git2` versions. The entries in the table above denote the _first_ in a range of `php-git2` versions to utilize the specified `libgit2` version. Note that `libgit2` updates along minor versions (since its API is currently unstable). Any patch version should work with the extension; however the exact, listed version is what is used in development and testing (and may be retroactively updated if upgraded during development).

A new minor version of `php-git2` is released whenever new API bindings are implemented, new helper functions are added or when internal classes are updated. Bug fixes may be included in minor version releases if they accompany new feature additions; otherwise pure bug fix releases increment the patch version.

The major version will only change if the extension undergoes a major interface change (or if `libgit2` drastically changes its API).

## Branches

This project maintains multiple branches for different major versions of PHP. Look for a branch named `phpX/master` for PHP version `X`. Branch `master` will point to the latest `phpX/master` that is supported.

Maintenance and development branches are organized by major version (e.g. `php5/devel`). If changes apply to multiple major branches, then they may be performed either on a universal maintenance branch or on separate maintenance branches depending on how the changes merge.

A release is issued for each major version separately. Look for tags organized by major version (e.g. `php5/v1.0.0`). A release branch may exist for extended maintenance on a particular release. ZTS/non-ZTS versions are part of the extension configuration when it is built, so they exist under the same release. The following releases are supported at this time:

| PHP | non-ZTS | ZTS
|--|--| -- |
| 5 | yes | yes |
| 7 | no | no |

## API Coverage

The [bindings](bindings) file contains a list of all bindings and documents whether they are covered or not.

We document API specifics in the [`DOCS`](DOCS) file, including the major differences between the PHP userspace API and the original C library API. Please consult this documentation before using these bindings, as there are some differences that need to be understood.

Please make sure to update these files when adding/updating functionality.

## Design

Our core principle is to stick as closely as possible to the original, C `libgit2` API while at the same  time making the extension behave comfortably in userspace with what PHP devs expect. At times we  add some helper functions to facilitate certain tasks in userspace or to optimize an otherwise costly operation (e.g. like converting values back-and-forth between userspace and the library). We also provide several abstractions as PHP classes so that they can be subclassed from userspace (e.g. the `git_odb_backend`  is a class called `GitODBBackend`).

### Stability

We want the extension to be stable so that it can be used securely in production code. To that end  we've added an extra reference counting layer to track resource dependencies. This ensures PHP  programmers don't blow their feet off when doing something like this:

```php
function get_ref() {
    $repo = git_repository_open('/path/to/repo.git',true);
    $ref = git_reference_lookup($repo,'annot-tag-1');
    return $ref;
}

$ref = get_ref();
$peeled = git_reference_peel($ref,GIT_OBJ_COMMIT);
```

The extension will keep the `git_repository` object alive behind the scenes since a dependency is established between the `git_repository` and the `git_reference`. This prevents the extension (and PHP) from crashing when programmers do crazy wack.

### Programming methodology

Most of the extension is designed as inline code in header files. We use C++ metaprogramming constructs to generate extension functions. This approach is great for knocking out redundant tasks,separating the prototype for a binding from its implementation and keeping track of API changes. However it comes with the small drawback of decreased flexibility when modifying a binding.

If a binding doesn't "fit the mold" and cannot be implemented using one of the generic binding template function generators, then we recommend the binding be written directly in the header file using the conventional `PHP_FUNCTION` macro.

We write PHP class implementations directly in their own compilation units since not too much of that code can be generalized. The extension provides internal PHP classes for many of the custom storage backends that can be extended in userspace.

## Building

This extension is written in C++, and it takes advantage of several C++11 features. You need to have a relatively recent C++ compiler that supports C++11 metaprogramming features. This extension has been built successfully using `g++` and `clang` on macOS and Linux. Experimental versions for PHP5 have been built on Windows using MinGW (see section on Windows).

Build the extension in the normal way using `phpize`, `autoconf` and `make`.

Run phpize initially:

	$ phpize

Then `configure` and `make`:

	$ ./configure [options...]
	$ make

Ensure that you configure the correct version of `libgit2` for the current version you are building (see Versioning for more on this). If you want to static link `libgit2` into the extension, install the archive library for `libgit2` in place of the dynamic link one (or alter the compile macros in the `config.m4` file to point at your static archive).

If you are linking against a shared library on Linux and want a little more control over which `libgit2` shared library is loaded, then you can configure `RPATH` to point at the desired `libgit2` binary. This is accomplished via the `PHP_RPATHS` environment variable passed to `configure`. This is useful for when you have a local development build of `libgit2` that you want to use when hacking on `php-git2`.

When you configure the build, there are a couple of different options you should consider to make the binary useful for your particular purpose. Add options to the `CXXFLAGS` variable since the C++ compiler is used for all compilation units. For release builds, enable optimizations like so:

	./configure CXXFLAGS='-O3'

Since the code uses a lot of inline template functions, optimizations will really help improve the performance of the binary (and make it a lot smaller). However this is not great for testing since it collapses function calls down. For a development environment, we recommend something like

	./configure CXXFLAGS='-g -O0 -Wfatal-errors -Wall -Wno-deprecated-register'

This configuration will produce a huge binary, but it will be easier to trace. If you are developing, we also recommend including the [`Makefile.extra`](Makefile.extra) file (included in the repository) at the bottom of the generated `Makefile`. This will give the build system some extra dependency information that makes life easier. This is done like so:

	-include Makefile.extra
	## Bottom of generated Makefile

## Windows

This project does not officially support Windows at this time, due to compiler version requirements for PHP5 and inadequate C++11 support in those compilers. Experimental builds have been built with MinGW that work successfully. However these builds required a lot of hacking and were designed to only be used in _testing_, not production, environments.

If you are building for Windows, or using one of our experimental builds, make sure that your `libgit2` does not static link to the CRT. It needs to dynamic link to the same runtime being used by `php5` and `php-git2`.

In our experimental builds, we were able to successfully build `php-git2.dll` using MinGW (though we had to hack the standard and PHP headers a little). The resulting library linked against `msvcr100.dll`, and we built PHP 5.4 and `git2` with the VC100 tools (which is non-standard).

