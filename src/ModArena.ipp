#include "Arena.h"
#include <stdlib.h>

namespace ADS
{
    template<typename T>
    ArenaPtr<T> ModArena::alloc(size_t amount)
    {
        assert(amount > 0);
        auto[address, index] = findFreeAddress(amount * sizeof(T));

        if(!address) return {};

        m_mem_info.insert(m_mem_info.begin() + index, MemBlockInfo{address, address + amount * sizeof(T)});
        return &m_mem_info[index];
    }
}
