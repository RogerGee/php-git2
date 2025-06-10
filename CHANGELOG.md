# Changelog: `php-git2`

## 2.0.1

- Fix deprecations for PHP 8.3 (testbed)
- Fix compatibility issues with libgit2 1.8 and 1.9+

## 2.0.0

- Update extension to `libgit2` version 1
- Add bindings for `git_worktree` functions
- Update `git_reference_cmp` to return `int` instead of `bool`
- Add error class constants
- Allow custom object types to be cloneable
- Fix a few bugs in GitConfigBackend
- Add 'readonly' attribute support to GitConfigBackend to support snapshots
