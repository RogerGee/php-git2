/*
 * config-git2.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_CONFIG_H
#define PHPGIT2_CONFIG_H
extern "C" {
#include <git2/sys/config.h>
}

namespace php_git2
{

    // Explicitly specialize git2_resource destructor for git_config.
    template<> php_git_config::~git2_resource()
    {
        git_config_free(handle);
    }

    // Provide a type for mapping a git_config_entry to a PHP array. One version
    // does not free while the other frees.

    class php_git_config_entry
    {
    public:
        php_git_config_entry():
            entry(nullptr)
        {
        }

        git_config_entry** byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            return &entry;
        }

        void ret(zval* return_value)
        {
            if (entry == nullptr) {
                RETURN_NULL();
            }

            array_init(return_value);
            add_assoc_string(return_value,"name",(char*)entry->name,1);
            add_assoc_string(return_value,"value",(char*)entry->value,1);
            add_assoc_long(return_value,"level",entry->level);
        }
    protected:
        git_config_entry* get_entry() const
        {
            return entry;
        }
    private:
        git_config_entry* entry;
    };

    class php_git_config_entry_freeing:
        public php_git_config_entry
    {
    public:
        ~php_git_config_entry_freeing()
        {
            git_config_entry* ent = get_entry();

            if (ent != nullptr) {
                git_config_entry_free(ent);
            }
        }
    };

} // namespace php_git2

// Functions:

static constexpr auto ZIF_GIT_CONFIG_NEW = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config**>::func<git_config_new>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_config>
        >,
    1,
    php_git2::sequence<>,
    php_git2::sequence<0>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_GIT_CONFIG_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_config>
        >
    >;

static constexpr auto ZIF_GIT_CONFIG_OPEN_DEFAULT = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config**>::func<git_config_open_default>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_config>
        >,
    1,
    php_git2::sequence<>,
    php_git2::sequence<0>,
    php_git2::sequence<0>
    >;

static constexpr auto ZIF_GIT_CONFIG_OPEN_GLOBAL = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config**,
        git_config*>::func<git_config_open_global>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_config>,
        php_git2::php_resource<php_git2::php_git_config>
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

static constexpr auto ZIF_GIT_CONFIG_OPEN_LEVEL = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config**,
        const git_config*,
        git_config_level_t>::func<git_config_open_level>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_config>,
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_long_cast<git_config_level_t>
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
    >;

static constexpr auto ZIF_GIT_CONFIG_OPEN_ONDISK = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config**,
        const char*>::func<git_config_open_ondisk>,
    php_git2::local_pack<
        php_git2::php_resource_ref<php_git2::php_git_config>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

static constexpr auto ZIF_GIT_CONFIG_PARSE_BOOL = zif_php_git2_function_rethandler<
    php_git2::func_wrapper<
        int,
        int*,
        const char*>::func<git_config_parse_bool>,
    php_git2::local_pack<
        php_git2::php_long_ref<int>,
        php_git2::php_string
        >,
    php_git2::php_convert_boolean_rethandler<int,0>,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

static constexpr auto ZIF_GIT_CONFIG_PARSE_INT32 = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        int32_t*,
        const char*>::func<git_config_parse_int32>,
    php_git2::local_pack<
        php_git2::php_long_ref<int32_t>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

static constexpr auto ZIF_GIT_CONFIG_PARSE_INT64 = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        int64_t*,
        const char*>::func<git_config_parse_int64>,
    php_git2::local_pack<
        php_git2::php_long_ref<int64_t>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

static constexpr auto ZIF_GIT_CONFIG_PARSE_PATH = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        const char*>::func<git_config_parse_path>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1>,
    php_git2::sequence<0,1>,
    php_git2::sequence<0,0>
    >;

static constexpr auto ZIF_GIT_CONFIG_SET_BOOL = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config*,
        const char*,
        int>::func<git_config_set_bool>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string,
        php_git2::php_bool
        >
    >;

static constexpr auto ZIF_GIT_CONFIG_SET_INT32 = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config*,
        const char*,
        int32_t>::func<git_config_set_int32>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string,
        php_git2::php_long_cast<int32_t>
        >
    >;

static constexpr auto ZIF_GIT_CONFIG_SET_INT64 = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config*,
        const char*,
        int64_t>::func<git_config_set_int64>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string,
        php_git2::php_long_cast<int64_t>
        >
    >;

static constexpr auto ZIF_GIT_CONFIG_SET_MULTIVAR = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config*,
        const char*,
        const char*,
        const char*>::func<git_config_set_multivar>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string,
        php_git2::php_string,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_GIT_CONFIG_SET_STRING = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config*,
        const char*,
        const char*>::func<git_config_set_string>,
    php_git2::local_pack<
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string,
        php_git2::php_string
        >
    >;

static constexpr auto ZIF_GIT_CONFIG_GET_ENTRY = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_config_entry**,
        const git_config*,
        const char*>::func<git_config_get_entry>,
    php_git2::local_pack<
        php_git2::php_git_config_entry_freeing,
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
    >;

static constexpr auto ZIF_GIT_CONFIG_GET_INT32 = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        int32_t*,
        const git_config*,
        const char*>::func<git_config_get_int32>,
    php_git2::local_pack<
        php_git2::php_long_ref<int32_t>,
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
    >;

static constexpr auto ZIF_GIT_CONFIG_GET_INT64 = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        int64_t*,
        const git_config*,
        const char*>::func<git_config_get_int64>,
    php_git2::local_pack<
        php_git2::php_long_ref<int64_t>,
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
    >;

static constexpr auto ZIF_GIT_CONFIG_GET_PATH = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        const git_config*,
        const char*>::func<git_config_get_path>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
    >;

static constexpr auto ZIF_GIT_CONFIG_GET_STRING = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        const char**,
        const git_config*,
        const char*>::func<git_config_get_string>,
    php_git2::local_pack<
        php_git2::php_string_ref,
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
    >;

static constexpr auto ZIF_GIT_CONFIG_GET_STRING_BUF = zif_php_git2_function<
    php_git2::func_wrapper<
        int,
        git_buf*,
        const git_config*,
        const char*>::func<git_config_get_string_buf>,
    php_git2::local_pack<
        php_git2::php_git_buf,
        php_git2::php_resource<php_git2::php_git_config>,
        php_git2::php_string
        >,
    1,
    php_git2::sequence<1,2>,
    php_git2::sequence<0,1,2>,
    php_git2::sequence<0,0,1>
    >;

// Function Entries:

#define GIT_CONFIG_FE                                                   \
    PHP_GIT2_FE(git_config_new,ZIF_GIT_CONFIG_NEW,NULL)                 \
    PHP_GIT2_FE(git_config_free,ZIF_GIT_CONFIG_FREE,NULL)               \
    PHP_GIT2_FE(git_config_open_default,ZIF_GIT_CONFIG_OPEN_DEFAULT,NULL) \
    PHP_GIT2_FE(git_config_open_global,ZIF_GIT_CONFIG_OPEN_GLOBAL,NULL) \
    PHP_GIT2_FE(git_config_open_level,ZIF_GIT_CONFIG_OPEN_LEVEL,NULL)   \
    PHP_GIT2_FE(git_config_open_ondisk,ZIF_GIT_CONFIG_OPEN_ONDISK,NULL) \
    PHP_GIT2_FE(git_config_parse_bool,ZIF_GIT_CONFIG_PARSE_BOOL,NULL)   \
    PHP_GIT2_FE(git_config_parse_int32,ZIF_GIT_CONFIG_PARSE_INT64,NULL) \
    PHP_GIT2_FE(git_config_parse_int64,ZIF_GIT_CONFIG_PARSE_INT32,NULL) \
    PHP_GIT2_FE(git_config_parse_path,ZIF_GIT_CONFIG_PARSE_PATH,NULL)   \
    PHP_GIT2_FE(git_config_set_bool,ZIF_GIT_CONFIG_SET_BOOL,NULL)       \
    PHP_GIT2_FE(git_config_set_int32,ZIF_GIT_CONFIG_SET_INT32,NULL)     \
    PHP_GIT2_FE(git_config_set_int64,ZIF_GIT_CONFIG_SET_INT64,NULL)     \
    PHP_GIT2_FE(git_config_set_multivar,ZIF_GIT_CONFIG_SET_MULTIVAR,NULL) \
    PHP_GIT2_FE(git_config_set_string,ZIF_GIT_CONFIG_SET_STRING,NULL)   \
    PHP_GIT2_FE(git_config_get_entry,ZIF_GIT_CONFIG_GET_ENTRY,NULL)     \
    PHP_GIT2_FE(git_config_get_int32,ZIF_GIT_CONFIG_GET_INT32,NULL)     \
    PHP_GIT2_FE(git_config_get_int64,ZIF_GIT_CONFIG_GET_INT64,NULL)     \
    PHP_GIT2_FE(git_config_get_path,ZIF_GIT_CONFIG_GET_PATH,NULL)       \
    PHP_GIT2_FE(git_config_get_string,ZIF_GIT_CONFIG_GET_STRING,NULL)   \
    PHP_GIT2_FE(git_config_get_string_buf,ZIF_GIT_CONFIG_GET_STRING_BUF,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
