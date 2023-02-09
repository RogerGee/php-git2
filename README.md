# php-git2

This project provides a PHP extension implementing bindings for [`libgit2`](https://github.com/libgit2/libgit2). You can use this library to build native Git tooling directly into your PHP application.

This branch targets PHP 8. See other branches for other PHP major versions. Please note that this project no longer supports versions before PHP 7.

> Note: At this time, PHP 8 support is experimental.

Primary author:

> [Roger Gee](https://github.com/RogerGee)

## Versioning

Since we do not bundle the `libgit2` dependency, it's up to the user to provide the correct version of  `libgit2` when building the extension. The `libgit2` version corresponds indirectly to the version of `php-git2` being built. Consult the following table to determine the correct version of `libgit2` required by `php-git2`:

| `php-git2` | `libgit2` | Notes |
| -------- | ------- | ------- |
| `2.0.0` | `^1.5.0` | `libgit2` API is now stable |
| `1.0.0`  | `0.25.1` | `libgit2` is unstable, so `v1` exclusively uses `0.25.1` |

The entries in the table above denote the `libgit2` version requirement for a particular `php-git2` release. An entry is only added when a new version of `libgit2` is required.

In theory, you should be able to use any version that fits the constraint (and possibly some previous `libgit2` releases within the same major version). The minimum `libgit2` version denoted by the constraint is the one we use to develop and test the release.

### Branches

This project maintains multiple branches for different major versions of PHP. Look for a branch named `phpX/master` for PHP version `X`. Branch `master` will point to the latest `phpX/master` that is supported. Maintenance and development branches are organized similarly (e.g. `php8/develop`).

A release is issued for each PHP version separately. Look for tags organized under PHP version (e.g. `php8/v2.0.0`). A release branch may exist for extended maintenance on a particular major version release (e.g. `php8/v2`).

## API Coverage

The [`docs/coverage.txt`](docs/coverage.txt) file contains a summary of library coverage and a list of all bindings with the implementation status of each binding.

We document API specifics in the [`docs/DOCS.txt`](docs/DOCS.txt) file, including the major differences between the PHP userspace API and the original C library API. Please consult this documentation before using these bindings, as there are some differences that need to be understood.

## Design

Our core design principle is to follow the original, `libgit2` C API as closely as possible while at the same time making the extension behave comfortably in userspace with what PHP developers expect. At times, we add helper functions to facilitate certain tasks in userspace or to optimize an otherwise costly operation (e.g. converting values back-and-forth between userspace and the library).

### Interfaces

- Most of the opaque `libgit2` data types (i.e. handles) are implemented as resources in PHP userspace:
	- This allows the PHP API to closely follow the underlying C API.
- Functions that return a `libgit2` handle via an output parameter in the C API return a resource via the function return value in the PHP API:
	- (e.g. `git_repository_open()` returns a `git_repository` resource).
- Errors are always converted into PHP exceptions
- Custom interface data structures (e.g. backends) are implemented as PHP classes:
	- This allows the developer to implement a subclass that easily implements a custom interface.
	- For example, the `git_odb_backend` structure is a class called `GitODBBackend`.
	- A developer could subclass `GitODBBackend` to provide an alternate storage mechanism for a repository's object database (such as a MySQL or SQLite database)
- Most other data structures are implemented using arrays

### Stability

We want the extension to be stable so that it can be used securely in production code. (Who wouldn't?) To that end, we've added an extra reference counting layer to track resource dependencies. This ensures PHP developers don't blow their feet off when doing something like this:

```php
function get_ref() {
    $repo = git_repository_open('/path/to/repo.git',true);
    $ref = git_reference_lookup($repo,'annot-tag-1');
    return $ref;
}

$ref = get_ref();
$peeled = git_reference_peel($ref,GIT_OBJ_COMMIT);
```

The extension will keep the `git_repository` object alive behind the scenes since a dependency is established between the `git_repository` and the `git_reference`. This allows PHP developers to use the library without worrying about low-level concerns.

### Programming methodology

Most of the extension is designed as inline code in header files. We use C++ metaprogramming constructs to generate extension functions. This approach is great for streamlining redundant tasks, separating the prototype for a binding from its implementation and keeping track of API changes. However, it comes with the small drawback of decreased flexibility when implementing unusual or more custom bindings.

If a binding doesn't "fit the mold" and cannot be implemented using one of the generic binding template function generators, then we recommend the binding be written directly in the header file using the conventional `PHP_FUNCTION` macro.

We write PHP class implementations directly in their own compilation units. The extension provides internal PHP classes for many of the custom storage backends that can be extended in userspace.

## Building

This extension is written in C++, and it takes advantage of modern C++ features. You need to have a relatively recent C++ compiler that supports C++11 metaprogramming features. This extension has been built successfully using GCC and Clang on macOS and Linux. The extension has also been built experimentally for Windows; however, Windows builds are not actively supported.

Build the extension in the normal way using `phpize`, `configure` and `make`.

Run `phpize` to generate the build system:

~~~
$ phpize
~~~

Then `configure` and `make`:

~~~
$ ./configure [--with-git2=/path/to/libgit2 --with-git2-static]
$ make
~~~

Ensure that you configure the correct version of `libgit2` for the extension version you are building (see Versioning for more on this). You can tell the build system where to find `libgit2` via the `--with-git2` option. You can also force the build system to use a static library instead of a shared library with `--with-git2-static`.

**Example: building with static library**

In this example, I've built and installed `libgit2` under `/usr/local/libgit2` as a static library. (Note: the static library must be installed as `lib/libgit2.a` under the distribution. Also, make sure the library was compiled with position-independent code if building a shared extension.) Now I can build `php-git2` using this static library:

~~~
$ ./configure --with-git2=/usr/local/libgit2 --with-git2-static LDFLAGS="-lssh2"
$ make
~~~

Note: I needed to link against `ssh2` since my `libgit2` was built with SSH support.

**Other Considerations**

If you are linking to a shared library, the compiler should embed the library `soname`  into the resulting binary. The embedded name should correspond to the `libgit2` version you have configured. In this way, the loader will always load the correct version at runtime. It is important to ensure the `soname` is specific enough to avoid accidentally loading the wrong version at runtime, in case there are multiple versions installed on the system. If your `libgit2` is in a non-standard location, then the build system should be smart enough to inject the appropriate `rpath` into your binary so that the correct library loads.

When you configure the build, there are a couple of different options you should consider to make the binary useful for your purposes. Add extra compiler options to the `CXXFLAGS` variable to customize your build. For release builds, enable optimizations like so:

~~~
./configure CXXFLAGS='-O3'
~~~

Since the code uses a lot of inline template functions, heavy optimizations will really help improve the performance of the binary (and make it a lot smaller). However, this is not great for debugging since it collapses down function calls. For a development environment, we recommend something like:

~~~
./configure CXXFLAGS='-g -O0 -Wfatal-errors -Wall -Wno-deprecated-register'
~~~

This configuration will produce a huge binary, but it will be easier to trace. If you are developing, we also recommend including the [`Makefile.extra`](Makefile.extra) file (included in the repository) at the bottom of the generated `Makefile`. This will give the build system some extra dependency information that makes life easier. This is done like so:

~~~
-include Makefile.extra
## Bottom of generated Makefile
~~~

## Windows

This project does not officially support Windows at this time. With this said, there shouldn't be anything preventing the extension from building and running on Windows; we just haven't worked out any of the inevitable, platform-specific issues.

## Roadmap

| Item | Status |
| -- | -- |
| Improve unit testing | Complete (October 2022) |
| Update to `libgit2` version 1 | Complete (January 2023) |
| Add support for PHP 8 | In progress |
| Add support for custom `libgit2` memory allocator utilizing PHP's memory allocation functionality | Pending |
| Create phpdoc files to generate documentation site | Pending |
