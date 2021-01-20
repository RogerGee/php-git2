/*
 * refdb.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_REFDB_H
#define PHPGIT2_REFDB_H

namespace php_git2
{
    // Explicitly specialize git2_resource destructor for git_repository.

    template<> php_git_refdb::~git2_resource()
    {
        git_refdb_free(handle);
    }

    // Provide type for passing/returning git_refdb_backends.

    class php_git_refdb_backend:
        public php_object<php_refdb_backend_object>
    {
    public:
        // Make this object a connector to a php_resource that looks up a
        // php_git_refdb resource wrapper object.
        using connect_t = php_resource<php_git_refdb>;
        using target_t = git_refdb_backend*;

        php_git_refdb_backend(connect_t& conn):
            ownerWrapper(conn)
        {
        }

        git_refdb_backend* byval_git2()
        {
            php_refdb_backend_object* object = get_storage();

            // If the object doesn't have a backing, then we create a custom
            // one.
            if (object->backend == nullptr) {
                object->create_custom_backend(get_value());
            }

            // If the object has a custom backing then it is (presumably)
            // already set on a refdb. (The same is true if the owner is already
            // set.)
            else if (object->kind != php_refdb_backend_object::user
                || object->owner != nullptr)
            {
                throw php_git2_exception("The refdb backend is already set on a refdb");
            }

            // Otherwise the object is a user backend and we make it into a
            // conventional backend.
            else {
                object->create_conventional_backend(ownerWrapper.get_object());
            }

            return object->backend;
        }

    private:
        connect_t& ownerWrapper;
    };

    class php_git_refdb_backend_ref
    {
    public:
        php_git_refdb_backend_ref():
            backend(nullptr)
        {
        }

        git_refdb_backend** byval_git2()
        {
            return &backend;
        }

        void ret(zval* return_value)
        {
            php_git2_make_refdb_backend(return_value,backend,nullptr);
        }

    private:
        git_refdb_backend* backend;
    };

    using php_git_refdb_backend_connector = connector_wrapper<php_git_refdb_backend>;

} // namespace php_git2

// Function templates:

static constexpr auto ZIF_GIT_REFDB_BACKEND_FS = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_refdb_backend**,
        git_repository*>::func<git_refdb_backend_fs>,
    php_git2::local_pack<
        php_git2::php_git_refdb_backend_ref,
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_GIT_REFDB_COMPRESS = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_refdb*>::func<git_refdb_compress>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_refdb>
        >
    >;

static constexpr auto ZIF_GIT_REFDB_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_refdb>
        >
    >;

static constexpr auto ZIF_GIT_REFDB_NEW = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_refdb**,
        git_repository*>::func<git_refdb_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_refdb>,
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    php_git2::sequence<0,1>, // Make the git_refdb dependent on the repository.
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_GIT_REFDB_OPEN = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_refdb**,
        git_repository*>::func<git_refdb_open>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_refdb>,
        php_git2::php_resource<php_git2::php_git_repository>
        >,
    php_git2::sequence<0,1>, // Make the git_refdb dependent on the repository.
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>
    >;

static constexpr auto ZIF_GIT_REFDB_SET_BACKEND = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_refdb*,
        git_refdb_backend*>::func<git_refdb_set_backend>,
    php_git2::local_pack<
        php_git2::php_git_refdb_backend_connector,
        php_git2::php_resource<php_git2::php_git_refdb>
        >,
    -1,
    php_git2::sequence<1,0>,
    php_git2::sequence<1,0>
    >;

// Function entries:

#define GIT_REFDB_FE                                                    \
    PHP_GIT2_FE(git_refdb_backend_fs,ZIF_GIT_REFDB_BACKEND_FS,NULL)     \
    PHP_GIT2_FE(git_refdb_compress,ZIF_GIT_REFDB_COMPRESS,NULL)         \
    PHP_GIT2_FE(git_refdb_free,ZIF_GIT_REFDB_FREE,NULL)                 \
    PHP_GIT2_FE(git_refdb_new,ZIF_GIT_REFDB_NEW,NULL)                   \
    PHP_GIT2_FE(git_refdb_open,ZIF_GIT_REFDB_OPEN,NULL)                 \
    PHP_GIT2_FE(git_refdb_set_backend,ZIF_GIT_REFDB_SET_BACKEND,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
