#include "Arena.h"

namespace MemArena
{
    StaticArena::StaticArena(size_t arena_size)
        : m_arena(new byte[arena_size]), m_arena_size(arena_size) {}


    void StaticArena::free(void* address)
    {
        assert(isValid((byte*) address));

        size_t block_size = ptrSize((byte*) address);

        memset((byte*) address - sizeof(size_t), 0, block_size + 1);

        address = nullptr;
    }

    void StaticArena::resize(size_t new_size)
    {
        m_arena_size = new_size;
        delete[] m_arena;
        m_arena = new byte[m_arena_size];
    }


    size_t StaticArena::ptrSize(void* address)
    {
        return *((size_t*) address - 1);
    }


    bool StaticArena::isValid(void* address)
    {

        // address is outside arena bounds
        if(address < m_arena || m_arena + m_arena_size < address) return false;

        for(byte* ptr = m_arena; ptr <= address;)
        {
            // get size of memory block
            size_t* block_size;
            block_size = (size_t*) ptr;

            // advance the pointer to the start of the memory block
            ptr += sizeof(size_t);

            // only return true if the two addresses are the same, and there is allocated memory. 
            if(ptr == address && *block_size > 0) return true;
            
            // advance the pointer to the end of the memory block
            ptr += *block_size;
        }

        return false;
    }

    byte* StaticArena::findFreeAddress(size_t size)
    {
        // include size of the memory block size
        size += sizeof(size_t);

        byte* last = m_arena;

        for(byte* ptr = m_arena; ptr < m_arena + m_arena_size;)
        {   
            // if the space between two addresses is more than or equal to the requested size, return the adress to the end of the first memoryblock + 1
            if(ptr - last >= size)
                return last;
            

            // get size of memory block
            size_t* block_size;
            block_size = (size_t*) ptr;

            // advance the pointer to the start of the memory block
            ptr += sizeof(size_t);
            
            // advance the pointer to the end of the memory block
            ptr += *block_size;

            // only store the ptr as last if memory were allocated
            if(*block_size != 0)
                last = ptr;
        }

        // if no memory block were allocated infront of it, use the end of the arena as the start of the next memory block
        if(m_arena + m_arena_size - last >= size)
            return last;
        else
            return nullptr;
    }

}
