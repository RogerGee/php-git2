/*
 * indexer.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_INDEXER_H
#define PHPGIT2_INDEXER_H

namespace php_git2
{
    // Specialize resource type destructor.

    template<> php_git_indexer::~git2_resource()
    {
        git_indexer_free(handle);
    }

    class php_git_indexer_with_stats;

    // Simply the typename for an indexer's asynchronous transfer progress
    // callback.

    using indexer_progress_callback_async = php_callback_async<
        php_git_indexer_with_stats,
        php_callback_sync_nullable
        >;

    // Provide subclass of git2_resource for indexer. This is necessary so we
    // can store git_transfer_progress and callback instances alongside the
    // opaque git2 handle within the resource.

    class php_git_indexer_with_stats:
        public php_git_indexer
    {
        friend class php_git_indexer_connector;
        friend indexer_progress_callback_async;
    public:
        php_git_indexer_with_stats()
        {
            memset(&stats,0,sizeof(git_transfer_progress));
        }

        virtual ~php_git_indexer_with_stats()
        {
            // Free callback structure.
            if (cb != nullptr) {
                cb->~php_callback_sync();
                efree(cb);
            }
        }

        git_transfer_progress* get_stats()
        {
            return &stats;
        }
        const git_transfer_progress* get_stats() const
        {
            return &stats;
        }

    private:
        git_transfer_progress stats;
        php_callback_sync* cb;
    };

    class php_git_indexer_connector
    {
    public:
        using connect_t = php_resource<php_git_indexer_with_stats>;
        typedef git_transfer_progress* target_t;

        php_git_indexer_connector(connect_t& obj):
            conn(obj)
        {
        }

        target_t byval_git2()
        {
            return &conn.get_object()->stats;
        }

    private:
        connect_t& conn;
    };

} // namespace php_git2

// Functions:

static constexpr auto ZIF_GIT_INDEXER_NEW = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_indexer**,
        const char*,
        unsigned int,
        git_odb*,
        git_transfer_progress_cb,
        void*>::func<git_indexer_new>,
    php_git2::local_pack<
        php_git2::php_callback_handler_nullable_async<
            php_git2::transfer_progress_callback,
            php_git2::indexer_progress_callback_async
            >,
        php_git2::connector_wrapper<php_git2::indexer_progress_callback_async>,
        php_git2::php_resource_ref<php_git2::php_git_indexer_with_stats>,
        php_git2::php_string,
        php_git2::php_long,
        php_git2::php_resource_nullable<php_git2::php_git_odb>
        >,
    php_git2::sequence<2,5>,
    3,
    php_git2::sequence<3,4,5,1,1>, // pass callback in twice
    php_git2::sequence<2,3,4,5,0,1>
    >;

static constexpr auto ZIF_GIT_INDEXER_HASH = zif_php_git2_function<
    php_git2::func_wrapper<
        const git_oid*,
        const git_indexer*>::func<git_indexer_hash>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_indexer>
        >,
    0
    >;

static constexpr auto ZIF_GIT_INDEXER_APPEND = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_indexer*,
        const void*,
        size_t,
        git_transfer_progress*>::func<git_indexer_append>,
    php_git2::local_pack<
        php_git2::connector_wrapper<php_git2::php_git_indexer_connector>,
        php_git2::php_resource<php_git2::php_git_indexer_with_stats>,
        php_git2::connector_wrapper<php_git2::php_string_length_connector<size_t> >,
        php_git2::php_string
        >,
    -1,
    php_git2::sequence<1,3>,
    php_git2::sequence<1,3,2,0>
    >;

static constexpr auto ZIF_GIT_INDEXER_COMMIT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_indexer*,
        git_transfer_progress*>::func<git_indexer_commit>,
    php_git2::local_pack<
        php_git2::connector_wrapper<php_git2::php_git_indexer_connector>,
        php_git2::php_resource<php_git2::php_git_indexer_with_stats>
        >,
    -1,
    php_git2::sequence<1>,
    php_git2::sequence<1,0>
    >;

static constexpr auto ZIF_GIT_INDEXER_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_indexer>
        >
    >;

// Create a non-standard function for obtaining the stats on the indexer. This
// is safe because all created indexer resource objects have
// git_transfer_progress members.
static PHP_FUNCTION(git2_indexer_stats)
{
    php_git2::php_bailer bailer ZTS_CTOR;

    {
        git_transfer_progress* stats;
        php_git2::php_resource<php_git2::php_git_indexer_with_stats> indexer ZTS_CTOR;
        php_git2::php_bailout_context ctx(bailer TSRMLS_CC);

        if (BAILOUT_ENTER_REGION(ctx)) {
            zval* zvp;
            if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC,"z",&zvp) == FAILURE) {
                return;
            }

            indexer.parse(zvp,1);

            try {
                stats = indexer.get_object(1)->get_stats();
            } catch (php_git2::php_git2_exception_base& ex) {
                php_git2::php_bailout_context ctx2(bailer TSRMLS_CC);

                if (BAILOUT_ENTER_REGION(ctx2)) {
                    ex.handle(TSRMLS_C);
                }

                return;
            }

            php_git2::convert_transfer_progress(return_value,stats);
        }
    }
}

// Function Entries:

#define GIT_INDEXER_FE                                          \
    PHP_GIT2_FE(git_indexer_new,ZIF_GIT_INDEXER_NEW,NULL)       \
    PHP_GIT2_FE(git_indexer_hash,ZIF_GIT_INDEXER_HASH,NULL)     \
    PHP_GIT2_FE(git_indexer_append,ZIF_GIT_INDEXER_APPEND,NULL) \
    PHP_GIT2_FE(git_indexer_commit,ZIF_GIT_INDEXER_COMMIT,NULL) \
    PHP_GIT2_FE(git_indexer_free,ZIF_GIT_INDEXER_FREE,NULL)     \
    PHP_FE(git2_indexer_stats,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
