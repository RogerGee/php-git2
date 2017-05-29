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

    // Provide a base class for git2_resource that can dynamically dispatch
    // calls to various instantiations of git2_resource.
    class git2_resource_base
    {
    public:
        git2_resource_base():
            parent(nullptr)
        {
        }

        git2_resource_base* get_parent()
        {
            return parent;
        }

        virtual bool free_handle() = 0;

    protected:
        // A reference to a parent resource object. This allows the resource to
        // define a single dependency for its lifetime.
        git2_resource_base* parent;
    };

    // Encapsulate resource structure and basic operations.
    template<typename git_type>
    class git2_resource:
        public git2_resource_base
    {
        typedef void (*resource_dstor)(git2_resource*);
    public:
        typedef git_type* git2_type;
        typedef const git_type* const_git2_type;

        git2_resource(bool isOwner = true):
            handle(nullptr), isowned(isOwner), ref(1)
        {
        }

        ~git2_resource()
        {
            // The destructor should be specialized for the git_type used by the
            // particular template class instantiation.
            throw php_git2_exception(
                "resource type was not implemented correctly: no destructor provided");
        }

        git2_type get_handle()
        {
            return handle;
        }
        const_git2_type get_handle() const
        {
            return handle;
        }

        git2_type* get_handle_byref()
        {
            return &handle;
        }
        const_git2_type* get_handle_byref() const
        {
            return const_cast<const_git2_type*>(&handle);
        }

        void release()
        {
            handle = nullptr;
        }

        bool is_owned() const
        {
            return isowned;
        }

        void up_ref()
        {
            ref += 1;
        }
        void down_ref()
        {
            ref -= 1;
        }

        template<typename T>
        void set_parent(git2_resource<T>* resource)
        {
            if (resource != nullptr) {
                // Update ref count to parent and set parent.
                resource->up_ref();
                parent = resource;
            }
        }

        bool free_handle()
        {
            // Call the destructor to free the handle. We only call the
            // destructor if the handle exists, we own the handle and the ref
            // count is fully decremented. Return true if the object is ready to
            // be cleaned up (i.e. all references have been decremented).

            if (--ref <= 0) {
                if (handle != nullptr && isowned) {
                    this->~git2_resource();
                    handle = nullptr;
                }
                return true;
            }
            return false;
        }

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
            git2_resource* self = reinterpret_cast<git2_resource*>(rsrc->ptr);
            free_recursive(self);
        }

        static void free_recursive(git2_resource_base* self)
        {
            // We must free the git2 handle. If the handle was freed, then free
            // the object itself. Recursively delete any parent.

            if (self->free_handle()) {
                // If we have a parent resource, attempt to free it as well.
                git2_resource_base* parent = self->get_parent();
                if (parent != nullptr) {
                    free_recursive(parent);
                }

                efree(self);
            }
        }

        // This is a pointer to the libgit2 type that the object wraps as a PHP
        // resource. These types are opaque and require explicit deletion by
        // libgit2 routines.
        git2_type handle;

        // Indicates whether or not the handle may be freed by this object.
        bool isowned;

        // A reference counter for the handle. If the counter reaches zero when
        // a resource is destroyed we can free the object.
        int ref;
    };
    template<typename git_type>
    int git2_resource<git_type>::le = 0;

    // Provide a custom derivation for handling resources that do not own their
    // underlying handles. This is useful for helping PHP userspace not bork
    // git2 by freeing things incorrectly.
    template<typename git_type>
    class git2_resource_nofree:
        public git2_resource<git_type>
    {
    public:
        git2_resource_nofree():
            git2_resource<git_type>(false)
        {
        }
    };

    // Provide a function for creating resource objects. This must exist outside
    // of the git2_resource<T> class so that we can allocate derived objects.
    template<typename GitResource>
    GitResource* php_git2_create_resource()
    {
        GitResource* obj;
        obj = new (emalloc(sizeof(GitResource))) GitResource;
        return obj;
    }

    // Provide a function for registering any number of resource types. The
    // template arguments each represent a git2 type that is tracked via a PHP
    // resource type.
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
