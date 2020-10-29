/*
 * blame.h
 *
 * Copyright (C) Roger P. Gee
 */

#ifndef PHPGIT2_BLAME_H
#define PHPGIT2_BLAME_H

namespace php_git2
{

    // Explicitly specialize git2_resource destructor for git_blame.

    template<> php_git_blame::~git2_resource()
    {
        git_blame_free(handle);
    }

    // Provide a type for converting a PHP array into a git_blame_options
    // struct.

    class php_git2_blame_options:
        public php_value_base
    {
    public:
        php_git2_blame_options(TSRMLS_D)
        {
            git_blame_init_options(&opts,GIT_BLAME_OPTIONS_VERSION);
        }

        git_blame_options* byval_git2()
        {
            if (value != nullptr && Z_TYPE_P(value) == IS_ARRAY) {
                array_wrapper arr(value);

                GIT2_ARRAY_LOOKUP_LONG(arr,flags,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,min_match_characters,opts);
                GIT2_ARRAY_LOOKUP_OID(arr,newest_commit,opts);
                GIT2_ARRAY_LOOKUP_OID(arr,oldest_commit,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,min_line,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,max_line,opts);
            }

            return &opts;
        }

    private:
        git_blame_options opts;
    };

    // Provide a rethandler for returning git_blame_hunks.

    template<typename... Ts>
    class php_git_blame_hunk_rethandler
    {
    public:
        ZTS_CONSTRUCTOR(php_git_blame_hunk_rethandler)

        bool ret(const git_blame_hunk* hunk,zval* return_value,local_pack<Ts...>& pack)
        {
            if (hunk == nullptr) {
                return false;
            }

            convert_blame_hunk(return_value,hunk);

            return true;
        }
    };

} // php_git2

// Functions:

static constexpr auto ZIF_GIT_BLAME_FILE = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_blame**,
        git_repository*,
        const char*,
        git_blame_options*
        >::func<git_blame_file>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_blame>,
        php_git2::php_resource<php_git2::php_git_repository>,
        php_git2::php_string,
        php_git2::php_git2_blame_options
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,2,3>,
    php_git2::sequence<0,1,2,3>
    >;

static constexpr auto ZIF_GIT_BLAME_GET_HUNK_BYINDEX = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        const git_blame_hunk*,
        git_blame*,
        uint32_t
        >::func<git_blame_get_hunk_byindex>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_blame>,
        php_git2::php_long_cast<size_t>
        >,
    php_git2::php_git_blame_hunk_rethandler<
        php_git2::php_resource<php_git2::php_git_blame>,
        php_git2::php_long_cast<size_t> >
    >;

static constexpr auto ZIF_GIT_BLAME_GET_HUNK_BYLINE = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        const git_blame_hunk*,
        git_blame*,
        size_t
        >::func<git_blame_get_hunk_byline>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_blame>,
        php_git2::php_long_cast<size_t>
        >,
    php_git2::php_git_blame_hunk_rethandler<
        php_git2::php_resource<php_git2::php_git_blame>,
        php_git2::php_long_cast<size_t> >
    >;

static constexpr auto ZIF_GIT_BLAME_GET_HUNK_COUNT = zif_php_git2_function<
    php_git2::func_wrapper<
        uint32_t,
        git_blame*
        >::func<git_blame_get_hunk_count>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_blame>
        >,
    0
    >;

static constexpr auto ZIF_GIT_BLAME_BUFFER = zif_php_git2_function_setdeps<
    php_git2::func_wrapper<
        int,
        git_blame**,
        git_blame*,
        const char*,
        size_t
        >::func<git_blame_buffer>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_blame>,
        php_git2::php_resource<php_git2::php_git_blame>,
        php_git2::connector_wrapper<php_git2::php_string_length_connector<size_t> >,
        php_git2::php_string
        >,
    php_git2::sequence<0,1>,
    1,
    php_git2::sequence<1,3>,
    php_git2::sequence<0,1,3,2>
    >;

static constexpr auto ZIF_GIT_BLAME_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_blame>
        >
    >;

// Function Entries:

#define GIT_BLAME_FE                                                    \
    PHP_GIT2_FE(git_blame_file,ZIF_GIT_BLAME_FILE,NULL)                 \
    PHP_GIT2_FE(git_blame_buffer,ZIF_GIT_BLAME_BUFFER,NULL)             \
    PHP_GIT2_FE(git_blame_get_hunk_byindex,ZIF_GIT_BLAME_GET_HUNK_BYINDEX,NULL) \
    PHP_GIT2_FE(git_blame_get_hunk_byline,ZIF_GIT_BLAME_GET_HUNK_BYLINE,NULL) \
    PHP_GIT2_FE(git_blame_get_hunk_count,ZIF_GIT_BLAME_GET_HUNK_COUNT,NULL) \
    PHP_GIT2_FE(git_blame_free,ZIF_GIT_BLAME_FREE,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
