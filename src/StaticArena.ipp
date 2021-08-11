#include "Arena.h"
#include <iostream>

namespace ADS
{
    template<typename T>
    T* StaticArena::alloc(size_t amount)
    {
        byte* ptr = findFreeAddress(amount * sizeof(T));

        if (!ptr) return nullptr;

        std::cout << (size_t)m_arena << ',' << (size_t)ptr << ',' << (size_t)(ptr - m_arena) << ',' << m_arena_size << '\n';
        *((size_t*)ptr) = amount * sizeof(T);
        ptr += sizeof(size_t);

        // set memory to 0 if it is not initialized as such.
        if constexpr (byte() != 0)
            memset(ptr, 0, amount * sizeof(T));

        return (T*)ptr;
    }

    template<typename T>
    size_t StaticArena::length(T* address)
    {
        assert(isValid((byte*)address));

        size_t size = ptrSize((byte*)address);

        return size / sizeof(T);
    }
};
