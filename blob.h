/*
 * blob.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_BLOB_H
#define PHPGIT2_BLOB_H

namespace php_git2
{

    // Explicitly specialize git2_resource destructor for git_blob.

    template<> php_git_blob::~git2_resource()
    {
        git_blob_free(handle);
    }

    // Provide a rethandler for git_blob_owner().
    class php_git_blob_owner_rethandler
    {
    public:
        bool ret(
            git_repository* retval,
            zval* return_value,
            local_pack<php_resource<php_git_blob>,
                php_resource_ref<php_git_repository_nofree> >&& pack)
        {
            if (retval == nullptr) {
                return false;
            }

            auto&& blob = pack.get<0>();
            auto&& resource = pack.get<1>();
            *resource.byval_git2() = retval;
            resource.ret(return_value);

            // Make the repository resource dependent on the blob (which is
            // dependent on the original repository resource).
            resource.get_object()->set_parent(blob.get_object());

            return true;
        }
    };

    // Provide a rethandler for git_blob_rawcontent().
    class php_git_blob_rawcontent_rethandler
    {
    public:
        bool ret(
            const void* retval,
            zval* return_value,
            local_pack<php_resource<php_git_blob> >&& pack)
        {
            if (retval != nullptr) {
                // Make a binary string for the return value. The length is
                // obtained from the blob attached to the local_pack.
                size_t length;
                length = git_blob_rawsize(pack.get<0>().get_object(1)->get_handle());
                RETVAL_STRINGL((const char*)retval,length,1);
            }
            else {
                RETVAL_NULL();
            }

            return true;
        }
    };

    // Provide types for getting/returning git_writestream objects.

    class php_git_writestream_byval:
        public php_value_base,
        private php_zts_base
    {
    public:
        php_git_writestream_byval(TSRMLS_D):
            php_zts_base(TSRMLS_C)
        {
        }

        git_writestream* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            // Make sure zval is object derived from GitWritestream.
            if (Z_TYPE_P(value) != IS_OBJECT
                || !is_subclass_of(Z_OBJCE_P(value),
                        class_entry[php_git2_writestream_obj]))
            {
                error("GitWritestream",argno);
            }

            // Extract the git_writestream from the object zval.
            php_writestream_object* object;
            object = reinterpret_cast<php_writestream_object*>(zend_objects_get_address(value TSRMLS_CC));

            if (object->ws == nullptr) {
                throw new php_git2_exception("the writestream has already been closed");
            }

            return object->ws;
        }
    };

    class php_git_writestream_byref:
        private php_zts_base
    {
    public:
        php_git_writestream_byref(TSRMLS_D):
            php_zts_base(TSRMLS_C), ws(nullptr)
        {
        }

        git_writestream** byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            return &ws;
        }

        void ret(zval* return_value)
        {
            // Wrap the git_writestream in an object zval.
            php_git2_make_writestream(return_value,ws TSRMLS_CC);
        }
    private:
        git_writestream* ws;
    };

    // Provide a git_writestream type that releases the git_writestream after
    // passing it to git2.

    class php_git_writestream_release:
        public php_value_base,
        private php_zts_base
    {
    public:
        php_git_writestream_release(TSRMLS_D):
            php_zts_base(TSRMLS_C)
        {
        }

        git_writestream* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            // Make sure zval is object derived from GitWritestream.
            if (Z_TYPE_P(value) != IS_OBJECT
                || !is_subclass_of(Z_OBJCE_P(value),
                        class_entry[php_git2_writestream_obj]))
            {
                error("GitWritestream",argno);
            }

            // Extract the git_writestream from the object zval.
            php_writestream_object* object;
            git_writestream* ws;
            object = reinterpret_cast<php_writestream_object*>(zend_objects_get_address(value TSRMLS_CC));

            if (object->ws == nullptr) {
                throw new php_git2_exception("the writestream has already been closed");
            }
            ws = object->ws;
            object->ws = nullptr;

            return ws;
        }
    };

} // namespace php_git2

#endif

static constexpr auto ZIF_GIT_BLOB_CREATE_FROMBUFFER = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_repository*,
        const void*,
        size_t
        >::func<git_blob_create_frombuffer>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::connector_wrapper<php_git2::php_string_length_connector<size_t> >,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,3>,
    php_git2::sequence<0,1,3,2>,
    php_git2::sequence<0,0,1,0>
    >;

static constexpr auto ZIF_GIT_BLOB_CREATE_FROMDISK = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_repository*,
        const char*
        >::func<git_blob_create_fromdisk>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
    >;

static constexpr auto ZIF_GIT_BLOB_CREATE_FROMWORKDIR = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_repository*,
        const char*
        >::func<git_blob_create_fromworkdir>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
    >;

static constexpr auto ZIF_GIT_BLOB_FILTERED_CONTENT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        git_blob*,
        const char*,
        int
        >::func<git_blob_filtered_content>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_resource<php_git2::php_git_blob>,
        php_git2::php_string,
        php_git2::php_bool
        >,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>,
    php_git2::sequence<0,0,1,2>
    >;

static constexpr auto ZIF_GIT_BLOB_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_blob>
        >
    >;

static constexpr auto ZIF_GIT_BLOB_ID = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        const git_blob*
        >::func<git_blob_id>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_blob>
        >,
    0
    >;

static constexpr auto ZIF_GIT_BLOB_IS_BINARY = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const git_blob*
        >::func<git_blob_is_binary>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_blob>
        >,
    0
    >;

static constexpr auto ZIF_GIT_BLOB_LOOKUP = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_blob**,
        git_repository*,
        const git_oid*
        >::func<git_blob_lookup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_blob>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_git_oid_fromstr
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
    >;

static constexpr auto ZIF_GIT_BLOB_LOOKUP_PREFIX = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_blob**,
        git_repository*,
        const git_oid*,
        size_t
        >::func<git_blob_lookup_prefix>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_blob>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::connector_wrapper<php_git2::php_string_length_connector<
                                        size_t,php_git2::php_git_oid_fromstr> >,
        php_git2::php_git_oid_fromstr
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,3>,
    php_git2::sequence<0,1,3,2>,
    php_git2::sequence<0,0,1,0>
    >;

static constexpr auto ZIF_GIT_BLOB_OWNER = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        git_repository*,
        const git_blob*
        >::func<git_blob_owner>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_blob>,
        php_git2::php_resource_ref<php_git2::php_git_repository_nofree>
        >,
    php_git2::php_git_blob_owner_rethandler,
    php_git2::sequence<0>,
    php_git2::sequence<0>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_GIT_BLOB_RAWCONTENT = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        const void*,
        const git_blob*
        >::func<git_blob_rawcontent>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_blob>
        >,
    php_git2::php_git_blob_rawcontent_rethandler
    >;

static constexpr auto ZIF_GIT_BLOB_RAWSIZE = zif_php_git2_function<
    php_git2::func_wrapper<
        git_off_t,
        const git_blob*
        >::func<git_blob_rawsize>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_blob>
        >,
    0
    >;

static constexpr auto ZIF_GIT_BLOB_DUP = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_blob**,
        git_blob*
        >::func<git_blob_dup>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_blob>,
        php_git2::php_resource<php_git2::php_git_blob>
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

static constexpr auto ZIF_GIT_BLOB_CREATE_FROMSTREAM = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_writestream**,
        git_repository*,
        const char*
        >::func<git_blob_create_fromstream>,
    php_git2::local_pack<
        php_git2::php_git_writestream_byref,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_nullable_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
    >;

static constexpr auto ZIF_GIT_BLOB_CREATE_FROMSTREAM_COMMIT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_oid*,
        git_writestream*
        >::func<git_blob_create_fromstream_commit>,
    php_git2::local_pack<
        php_git2::php_git_oid,
        php_git2::php_git_writestream_release
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

#define GIT_BLOB_FE                                                     \
    PHP_GIT2_FE(git_blob_create_frombuffer,ZIF_GIT_BLOB_CREATE_FROMBUFFER,NULL) \
    PHP_GIT2_FE(git_blob_create_fromdisk,ZIF_GIT_BLOB_CREATE_FROMDISK,NULL) \
    PHP_GIT2_FE(git_blob_create_fromworkdir,ZIF_GIT_BLOB_CREATE_FROMWORKDIR,NULL) \
    PHP_GIT2_FE(git_blob_filtered_content,ZIF_GIT_BLOB_FILTERED_CONTENT,NULL) \
    PHP_GIT2_FE(git_blob_free,ZIF_GIT_BLOB_FREE,NULL)                   \
    PHP_GIT2_FE(git_blob_id,ZIF_GIT_BLOB_ID,NULL)                       \
    PHP_GIT2_FE(git_blob_is_binary,ZIF_GIT_BLOB_IS_BINARY,NULL)         \
    PHP_GIT2_FE(git_blob_lookup,ZIF_GIT_BLOB_LOOKUP,NULL)               \
    PHP_GIT2_FE(git_blob_lookup_prefix,ZIF_GIT_BLOB_LOOKUP_PREFIX,NULL) \
    PHP_GIT2_FE(git_blob_owner,ZIF_GIT_BLOB_OWNER,NULL)                 \
    PHP_GIT2_FE(git_blob_rawcontent,ZIF_GIT_BLOB_RAWCONTENT,NULL)       \
    PHP_GIT2_FE(git_blob_rawsize,ZIF_GIT_BLOB_RAWSIZE,NULL)             \
    PHP_GIT2_FE(git_blob_dup,ZIF_GIT_BLOB_DUP,NULL)                     \
    PHP_GIT2_FE(git_blob_create_fromstream,ZIF_GIT_BLOB_CREATE_FROMSTREAM,NULL) \
    PHP_GIT2_FE(git_blob_create_fromstream_commit,ZIF_GIT_BLOB_CREATE_FROMSTREAM_COMMIT,NULL)

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
