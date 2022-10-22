#pragma once

#include "utility.h"

namespace std
{
    template <class E>
    class initializer_list
    {
        const E *first = nullptr;
        const E *last = nullptr;

    public:
        using val_type = E;
        using reference = const E &;
        using iterator = const E *;

        constexpr initializer_list() noexcept : first(nullptr), last(nullptr) {}
        constexpr initializer_list(const E *first_arg, const E *last_arg) noexcept : first(first_arg), last(last_arg) {}

        constexpr const E *begin() const noexcept { return first; }
        constexpr const E *end() const noexcept { return last; }
        constexpr u64 size() const noexcept { return static_cast<u64>(last - first); }
    };

    template <class E>
    inline constexpr const E *begin(initializer_list<E> list) noexcept { return list.begin(); }
    template <class E>
    inline constexpr const E *end(initializer_list<E> list) noexcept { return list.end(); }
}