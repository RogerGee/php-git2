/*
 * php-rethandler.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_RETHANDLER_H
#define PHPGIT2_RETHANDLER_H
#include "php-type.h"
#include "php-function.h"

namespace php_git2
{

    // Provide a rethandler to use for any integer. This is necessary to handle
    // when negative integers are returned from functions.

    template<typename Numeric>
    class php_numeric_rethandler
    {
    public:
        template<typename... Ts>
        bool ret(Numeric retval,zval* return_value,local_pack<Ts...>&& pack)
        {
            RETVAL_LONG(retval);
            return true;
        }
    };

    template<typename Numeric>
    class php_boolean_rethandler
    {
    public:
        template<typename... Ts>
        bool ret(Numeric retval,zval* return_value,local_pack<Ts...>&& pack)
        {
            RETVAL_BOOL(retval);
            return true;
        }
    };

    // Provide a rethandler for converting a numeric value into a bool.

    template<typename Numeric,unsigned Position>
    class php_convert_boolean_rethandler
    {
    public:
        template<typename T,typename... Ts>
        bool ret(T retval,zval* return_value,local_pack<Ts...>&& pack)
        {
            auto&& value = pack.template get<Position>();
            value.ret(return_value);
            convert_to_boolean(return_value);
            return true;
        }
    };

    // Provide a rethandler to use for setting dependencies. This is modeled for
    // functions that return their owner as a git2 resource.

    template<typename T>
    class php_owner_rethandler
    {
    public:
        using pack_type = local_pack<php_resource<T>,php_resource_ref<php_git_repository_nofree> >;

        bool ret(git_repository* owner,zval* return_value,pack_type&& pack)
        {
            if (owner == nullptr) {
                return false;
            }

            auto&& obj = pack.template get<0>();
            auto&& resource = pack.template get<1>();

            *resource.byval_git2() = owner;
            resource.ret(return_value);

            // Make the repository resource dependent on the object (which is
            // dependent on the original repository resource).
            resource.get_object()->set_parent(obj.get_object());

            return true;
        }   
    };

    // Provide a rethandler for returning a resource.

    template<typename ResourceType,typename Git2Type,unsigned Position = 0>
    class php_resource_rethandler
    {
    public:
        template<typename... Ts>
        bool ret(Git2Type* handle,zval* return_value,local_pack<Ts...>&& pack)
        {
            if (handle == nullptr) {
                return false;
            }

            auto&& obj = pack.template get<Position>();
            const php_resource_ref<ResourceType> resource;

            *resource.byval_git2() = handle;
            resource.ret(return_value);

            // Make the resource dependent on the object (which is dependent on
            // the original resource).
            resource.get_object()->set_parent(obj.get_object());

            return true;
        }
    };

    template<typename ResourceType,typename Git2Type,unsigned Position = 0>
    class php_resource_nullable_rethandler
    {
    public:
        template<typename... Ts>
        bool ret(Git2Type* handle,zval* return_value,local_pack<Ts...>&& pack)
        {
            if (handle == nullptr) {
                ZVAL_NULL(return_value); // just in case
                return true;
            }

            auto&& obj = pack.template get<Position>();
            const php_resource_ref<ResourceType> resource;

            *resource.byval_git2() = handle;
            resource.ret(return_value);

            // Make the resource dependent on the object (which is dependent on
            // the original resource).
            resource.get_object()->set_parent(obj.get_object());

            return true;
        }
    };

    template<typename ResourceType,typename Git2Type>
    class php_resource_nodeps_rethandler
    {
    public:
        template<typename... Ts>
        bool ret(Git2Type* handle,zval* return_value,local_pack<Ts...>&& pack)
        {
            if (handle == nullptr) {
                return false;
            }

            const php_resource_ref<ResourceType> resource;
            *resource.byval_git2() = handle;
            resource.ret(return_value);

            return true;
        }
    };

    // Provide a rethandler for handling the GIT_ENOTFOUND error condition. We
    // provide one for returning the boolean condition, another for returning a
    // pack element and another for returning a resource pack element with
    // dependencies to set.

    class php_boolean_notfound_rethandler
    {
    public:
        template<typename... Ts>
        bool ret(int retval,zval* return_value,local_pack<Ts...>&& pack)
        {
            if (retval != 0) {
                if (retval != GIT_ENOTFOUND) {
                    return false;
                }

                RETVAL_FALSE;
                return true;
            }

            RETVAL_TRUE;
            return true;
        }
    };

    template<unsigned Position>
    class php_notfound_rethandler
    {
    public:
        template<typename... Ts>
        bool ret(int retval,zval* return_value,local_pack<Ts...>&& pack)
        {
            auto&& obj = pack.template get<Position>();

            if (retval != 0) {
                if (retval != GIT_ENOTFOUND) {
                    return false;
                }

                RETVAL_FALSE;
                return true;
            }

            obj.ret(return_value);
            return true;
        }
    };

    template<unsigned Position,typename ResourceDeps>
    class php_resource_notfound_rethandler
    {
    public:
        template<typename... Ts>
        bool ret(int retval,zval* return_value,local_pack<Ts...>&& pack)
        {
            auto&& obj = pack.template get<Position>();

            if (retval != 0) {
                if (retval != GIT_ENOTFOUND) {
                    return false;
                }

                RETVAL_FALSE;
                return true;
            }

            obj.ret(return_value);
            php_set_resource_dependency(std::forward<local_pack<Ts...> >(pack),ResourceDeps());
            return true;
        }
    };

    // Provide a rethandler that returns false when GIT_ITEROVER is
    // reached. Provides a variant for returning a value at 'Position' within
    // the local pack having no dependencies and another variant that returns a
    // resource at 'Position' within the local pack having dependencies
    // 'ResourceDeps'.

    template<unsigned Position>
    class php_iterover_rethandler
    {
    public:
        template<typename... Ts>
        bool ret(int retval,zval* return_value,local_pack<Ts...>&& pack)
        {
            auto&& obj = pack.template get<Position>();

            if (retval != 0) {
                if (retval != GIT_ITEROVER) {
                    return false;
                }

                RETVAL_FALSE;
                return true;
            }

            obj.ret(return_value);
            return true;
        }
    };

    template<unsigned Position,typename ResourceDeps>
    class php_resource_iterover_rethandler
    {
    public:
        template<typename... Ts>
        bool ret(int retval,zval* return_value,local_pack<Ts...>&& pack)
        {
            auto&& obj = pack.template get<Position>();

            if (retval != 0) {
                if (retval != GIT_ITEROVER) {
                    return false;
                }

                RETVAL_FALSE;
                return true;
            }

            obj.ret(return_value);
            php_set_resource_dependency(std::forward<local_pack<Ts...> >(pack),ResourceDeps());
            return true;
        }
    };

} // php_git2

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
