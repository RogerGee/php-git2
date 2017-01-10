/*
 * git2-resource.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_GIT2_RESOURCE_H
#define PHPGIT2_GIT2_RESOURCE_H
#include "php-git2.h"
#include <new>
#include <typeinfo>

namespace php_git2
{

    // Encapsulate resource structure and basic operations.
    template<typename git_type>
    class git2_resource
    {
        typedef void (*resource_dstor)(git2_resource*);
    public:
        typedef git_type* git2_type;

        git2_resource():
            handle(nullptr)
        {
        }

        virtual ~git2_resource()
        {
            throw php_git2_exception(
                "resource type was not implemented correctly: no destructor provided");
        }

        git2_type get_handle()
        { return handle; }
        git2_type* get_handle_byref()
        { return &handle; }
        void release()
        { handle = nullptr; }

        static void define_resource_type(int moduleNumber)
        {
            le = zend_register_list_destructors_ex(
                destroy_resource, nullptr, typeid(git2_type).name(), moduleNumber);
        }

        static int resource_le()
        { return le; }
        static const char* resource_name()
        { return typeid(git2_type).name(); }
    private:
        static int le;

        static void destroy_resource(zend_rsrc_list_entry* rsrc TSRMLS_DC)
        {
            // We must explicitly call the object's destructor, then free the
            // object itself.

            git2_resource* thisObj = reinterpret_cast<git2_resource*>(rsrc->ptr);

            thisObj->~git2_resource();
            efree(thisObj);
        }

        // This is a pointer to the libgit2 type that the object wraps as a PHP
        // resource. These types are opaque and require explicit deletion by
        // libgit2 routines.
        git2_type handle;
    };
    template<typename git_type>
    int git2_resource<git_type>::le = 0;

    template<typename GitResource>
    GitResource* php_git2_create_resource()
    {
        GitResource* obj;
        obj = new (emalloc(sizeof(GitResource))) GitResource;
        return obj;
    }

    template<typename... Args>
    void php_git2_define_resource_types(int moduleNumber)
    {
        void (*fcs[])(int) = {
            (git2_resource<Args>::define_resource_type)..., nullptr
        };

        int i = 0;
        while (fcs[i] != nullptr) {
            (fcs[i])(moduleNumber);
            i += 1;
        }
    }

} // namespace php_git2

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
