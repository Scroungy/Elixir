#pragma once

#include "initializer_list.h"

namespace std
{
    template <typename T, u32 C>
    class FixedArray
    {
        using val_type = T;
        using reference = T &;
        using pointer = T *;
        val_type _data[C]{};
        u32 _size = 0;
        u32 _length = C;

    public:
        FixedArray()
        {
            assert(C > 0); // Length should be at least 1
        }

        FixedArray(const FixedArray<T, C> &other) : _data(other.data()), _size(other.size()) {}

        FixedArray(FixedArray<T, C> &&other) : _data(other.data()), _size(other.size())
        {
            other.nullify();
        }

        FixedArray(std::initializer_list<T> list) noexcept : _data(new val_type[list.size()]), _size(0)
        {
            assert(list.size() <= C);
            for (T &i : list)
            {
                _data.emplace_back(i);
            }
        }

        ~FixedArray() {}

        reference operator[](u32 i) const
        {
            assert(i < _size);
            return _data[i];
        }
        val_type operator[](u32 i)
        {
            assert(i < _size);
            return _data[i];
        }

        template <typename... Args>
        reference emplace_back(Args &&...args)
        {
            if (_size == _length)
            {
                return _data[_size - 1]; // Just return last element
            }
            return _data[_size++] = val_type{forward<Args>(args)...}; // Construct element in place, return and increment
        }

        reference push_back(reference data)
        {
            if (_size == _length)
            {
                return _data[_size - 1]; // Just return last element
            }
            _data[_size] = move(data); // Assign element
            return _data[_size++];     // Return element then increment
        }

        reference replace(u32 index, const reference data)
        {
            assert(index < _size);     // Index should be less than current active size
            _data[index].~T();         // Decon element at index
            _data[index] = move(data); // Move new data into element
            return _data[index];       // Return new element
        }

        reference replace_with_back(u32 index)
        {
            assert(index < _size);                 // Index should be less than current active size
            _data[index].~T();                     // Decon element at index
            _data[index] = move(_data[_size - 1]); // Move last element into this one
            _size--;
            return _data[index];
        }

        val_type pop_back()
        {
            assert(_size > 0);                // Size should be larger than 0
            val_type temp = _data[_size - 1]; // Create a copy element to return later
            _data[_size - 1].~T();            // Deconstruct element in array
            _size--;                          // Decrement size
            return temp;                      // Return copy
        }

        pointer data() const { return _data; } // Return pointer to array

        reference at(u32 index) const
        {
            assert(index < _size); // Index should be less than currently active size
            return _data[index];   // Return element at index
        }

        pointer find(T value)
        {
            for (T &elem : _data)
            {
                if (elem == value)
                {
                    return elem;
                }
            }
            return nullptr;
        }

        reference front() const
        {
            assert(_size > 0); // Size should be larger than 0
            return _data[0];   // Return first element
        }

        reference back()
        {
            assert(_size > 0);       // Size should be larger than 0
            return _data[_size - 1]; // Return last element
        }

        u32 size() const { return _size; } // Return number of elements

        u32 length() const { return _length; } // Return length of array

        void clear()
        {
            // Decon each element
            for (u32 i = 0; i < _size; i++)
            {
                _data[i].~T();
            }
            _size = 0; // Reset size
        }

        void nullify()
        {
            _data = nullptr;
            _size = 0;
        }
    };
}