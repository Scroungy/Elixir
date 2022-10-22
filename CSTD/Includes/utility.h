#pragma once

#include "typedefs.h"
#include <assert.h>

namespace std
{
    template <class T>
    struct remove_reference_impl
    {
        typedef T type;
    };

    template <class T>
    struct remove_reference_impl<T &>
    {
        typedef T type;
    };

    template <class T>
    struct remove_reference_impl<T &&>
    {
        typedef T type;
    };

    template <class T>
    using remove_reference = typename remove_reference_impl<T>::type;

    template <typename T>
    inline remove_reference<T> &&move(T &&arg) noexcept
    {
        return static_cast<remove_reference<T> &&>(arg);
    }

    template <typename T>
    inline T forward(remove_reference<T> &arg) noexcept
    {
        return static_cast<T>(arg);
    }

}
