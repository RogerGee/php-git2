/*
 * diff.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_DIFF_H
#define PHPGIT2_DIFF_H

namespace php_git2
{

    // Specialize resource destructor for git_diff
    template<> inline php_git_diff::~git2_resource()
    {
        git_diff_free(handle);
    }

    // Define type to wrap git_diff_options.

    class php_git_diff_options:
        public php_value_base
    {
    public:
        php_git_diff_options()
        {
            memset(&opts,0,sizeof(git_diff_options));
        }

        const git_diff_options* byval_git2(unsigned argno = std::numeric_limits<unsigned>::max())
        {
            if (value != nullptr && Z_TYPE_P(value) == IS_ARRAY) {
                array_wrapper arr(value);

                GIT2_ARRAY_LOOKUP_LONG(arr,version,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,flags,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,ignore_submodules,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,context_lines,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,interhunk_lines,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,id_abbrev,opts);
                GIT2_ARRAY_LOOKUP_LONG(arr,max_size,opts);
                GIT2_ARRAY_LOOKUP_STRING_NULLABLE(arr,old_prefix,opts);
                GIT2_ARRAY_LOOKUP_STRING_NULLABLE(arr,new_prefix,opts);
                GIT2_ARRAY_LOOKUP_SUBOBJECT(arr,strarray,pathspec,opts);
                GIT2_ARRAY_LOOKUP_CALLBACK(
                    arr,
                    diff_notify_callback,
                    callbacks.notifyCallback,
                    notify_cb,
                    payload,
                    opts);
                GIT2_ARRAY_LOOKUP_CALLBACK(
                    arr,
                    diff_progress_callback,
                    callbacks.progressCallback,
                    progress_cb,
                    payload,
                    opts);

                // Force payload to be pair of callbacks.
                if (opts.notify_cb != nullptr || opts.progress_cb != nullptr) {
                    opts.payload = (void*)&callbacks;
                }

                return &opts;
            }

            return nullptr;
        }

    private:
        git_diff_options opts;
        git_diff_options_callback_info callbacks;
        php_strarray_array strarray;
    };

} // namespace php_git2

// Functions:

static constexpr auto ZIF_GIT_DIFF_FREE = zif_php_git2_function_free<
    php_git2::local_pack<
        php_git2::php_resource_cleanup<php_git2::php_git_diff>
        >
    >;


// Function Entries:

#define GIT_DIFF_FE                                     \
    PHP_GIT2_FE(git_diff_free,ZIF_GIT_DIFF_FREE,NULL)

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
