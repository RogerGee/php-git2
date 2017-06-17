/*
 * php-rethandler.h
 *
 * This file is a part of php-git2.
 */

#ifndef PHPGIT2_RETHANDLER_H
#define PHPGIT2_RETHANDLER_H
#include "php-type.h"

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

} // php_git2

#endif

/*
 * Local Variables:
 * mode:c++
 * indent-tabs-mode:nil
 * tab-width:4
 * End:
 */
