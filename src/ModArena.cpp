#include "Arena.h"
#include <stdlib.h>

namespace ADS
{

    ModArena::ModArena(size_t arena_size)
        : m_arena(new byte[arena_size]), m_arena_size(arena_size)
    {
        if constexpr (byte() != 0)
            memset(m_arena, 0, arena_size);
    }

    void ModArena::free(ArenaPtr<void> ptr)
    {
        assert(std::find(m_mem_info.begin(), m_mem_info.end(), *ptr.blockInfo()) != m_mem_info.end());
        ptr.m_mem_info = nullptr;
        ptr.m_pos = nullptr;
    }

    std::pair<byte*, size_t> ModArena::findFreeAddress(size_t size)
    {

        byte* last = m_arena;
        size_t index = 0;

        for (const MemBlockInfo& info : m_mem_info)
        {
            if (size_t(last - info.start) >= size)
                return { last, index };
            else
            {
                last = info.end;
                index++;
            }
        }

        // compare to end of arena instead of memory block infront of last
        if (last - m_arena + m_arena_size >= size) return { last, index };

        return { nullptr, NULL };
    }
};
