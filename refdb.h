/*
 * refdb.h
 *
 * This file is a part of php-git2.
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
        public php_value_base,
        private php_zts_base
    {
    public:
        // Make this object a connector to a php_resource that looks up a
        // php_git_refdb resource wrapper object.
        using connect_t = php_resource<php_git_refdb>;
        typedef git_refdb_backend* target_t;

        php_git_refdb_backend(connect_t& conn TSRMLS_DC):
            php_zts_base(TSRMLS_C), ownerWrapper(conn)
        {
        }

        git_refdb_backend* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            // Make sure the zval is an object of or derived from class
            // GitRefDBBackend.
            if (Z_TYPE_P(value) != IS_OBJECT
                || !is_subclass_of(Z_OBJCE_P(value),
                        class_entry[php_git2_refdb_backend_obj]))
            {
                error("GitRefDBBackend",argno);
            }

            // Extract the git_odb_backend from the object zval.
            php_refdb_backend_object* object;
            object = reinterpret_cast<php_refdb_backend_object*>(zend_objects_get_address(value TSRMLS_CC));

            // If the object doesn't have a backing, then we create a custom
            // one.
            if (object->backend == nullptr) {
                object->create_custom_backend(value,ownerWrapper.get_object());
            }
            else if (object->owner != nullptr) {
                throw php_git2_exception("The refdb backend is already owned by a refdb");
            }
            else {
                object->owner = ownerWrapper.get_object();
            }

            return object->backend;
        }

    private:
        connect_t& ownerWrapper;
    };

    class php_git_refdb_backend_ref:
        private php_zts_base
    {
    public:
        php_git_refdb_backend_ref(TSRMLS_D):
            php_zts_base(TSRMLS_C), backend(nullptr)
        {
        }

        git_refdb_backend** byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            return &backend;
        }

        void ret(zval* return_value)
        {
            php_git2_make_refdb_backend(return_value,backend,nullptr TSRMLS_CC);
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
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
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
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
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
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
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
