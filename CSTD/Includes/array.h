#pragma once

#include "initializer_list.h"

namespace std
{
    template <typename T>
    class Array
    {
        using val_type = T;
        using reference = T &;
        using pointer = T *;
        val_type *_data = new val_type[4];
        u32 _size = 0;
        u32 _length = 4;

    public:
        Array() {}

        explicit Array(u32 size) : _data(new val_type[size]), _size(size), _length(size) {}

        Array(const Array<val_type> &other) noexcept : _data(other.data()), _size(other.size()), _length(other.length()) {}

        Array(Array<val_type> &&other) noexcept : _data(other.data()), _size(other.size()), _length(other.length())
        {
            other.nullify();
        }

        template <typename... U>
        Array(const T base, const U... values) noexcept : _data(new val_type[sizeof...(values) + 1]), _size(sizeof...(values) + 1), _length(sizeof...(values) + 1)
        {
            _data[0] = base;
            u64 inc = 1;
            ((_data[inc++] = values), ...);
        }

        // Array(std::initializer_list<T> list)
        // {
        //     for (T &elem : list)
        //     {
        //         _data.emplace_back(elem);
        //     }
        // }

        ~Array() { delete[] _data; }

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

        Array<val_type> &operator=(Array<val_type> &&other) noexcept
        {
            delete[] _data;
            _data = other.data();
            _size = other.size();
            _length = other.length();
            other.nullify();
            return *this;
        }

        Array<val_type> &operator=(const Array<val_type> &other) noexcept
        {
            delete[] _data;
            _data = other.data();
            _size = other.size();
            _length = other.length();
            return *this;
        }

        template <typename... Args>
        reference emplace_back(Args &&...args)
        {
            if (_size == _length)
            {
                pointer newArr = new val_type[_length << 1]; // Create new array twice the size
                for (u32 i = 0; i < _length << 1; i++)
                {
                    newArr[i] = _data[i];
                }
                delete[] _data; // Delete old array
                _data = newArr; // Set pointer to new array
                _length = _length << 1;
            }
            return _data[_size++] = val_type{forward<Args>(args)...};
        }

        reference push_back(reference data)
        {
            if (_size == _length)
            {
                pointer newArr = new val_type[_length << 1]; // Create new array twice the size
                for (u32 i = 0; i < _length << 1; i++)
                {
                    newArr[i] = _data[i];
                }
                delete[] _data; // Delete old array
                _data = newArr; // Set pointer to new array
                _length = _length << 1;
            }
            return _data[_size++] = move(data); // Set last element to data
        }

        reference replace(u32 index, const reference data)
        {
            assert(index < _size);     // Index should be less than size
            _data[index].~T();         // Decon element at index
            _data[index] = move(data); // Move new data into element
            return _data[index];       // Return constructed element
        }

        reference replace_with_back(u32 index)
        {
            assert(index < _size);                 // Index should be less than size
            _data[index].~T();                     // Decon element at index
            _data[index] = move(_data[_size - 1]); // Move end element into index
            _size--;                               // Decrease element count
            return _data[index];                   // Return moved element
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

        pointer find(T value) const
        {
            for (T &elem : _data)
            {
                if (value == elem)
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

        u32 size() const { return _size; } // Return amount of elements in array

        u32 length() const { return _length; } // Return max length of array

        void clear()
        {
            // Decon each element
            for (u32 i = 0; i < _size; i++)
            {
                _data[i].~T();
            }
            _size = 0; // Reset size
        }

        void free()
        {
            delete[] _data; // Deallocate array
            _size = 0;      // Reset size
            _length = 0;    // Reset cap
        }

        void nullify()
        {
            _data = nullptr;
            _size = 0;
            _length = 0;
        }

        void reset(u32 size)
        {
            delete[] _data;             // Deallocate array
            _data = new val_type[size]; // Create array with new size
            _size = 0;                  // Reset element count
            _length = size;             // Set cap to new size
        }
    };
}