#pragma once

#include <cstdlib>
#include <vector>
#include <tuple>
#include <iostream>
#include <memory>
#include <cassert>

namespace ADS
{
    typedef unsigned char byte;

    class Arena;



    // an arena always having the same memory footprint no matter how many allocations are made.
    // 
    // the only time the footprint is changed is on a call to resize and during initialization.
    // data is not saved when resized and it is not possible to defragment the memory.
    // arena_size does not define how many bytes can be allocated from the arena, 
    // due to the fact that each allocation will have a small section storing the size of the block. (the number of bytes is dependent on size_t)
    // 
    class StaticArena
    {
    public:

        // initializes the arena memory with a specific size.
        StaticArena(size_t arena_size);
        ~StaticArena() { delete[] m_arena; }

        // allocates a memory block of size sizeof(T) * amount inside the arena.
        // returns nullptr if memory allocation failed.
        template<typename T>
        T* alloc(size_t amount = 1);

        // frees the passed address from the arena.
        void free(void* address);

        // returns the number of elements allocated for the address.
        template<typename T>
        size_t length(T* address);

        // returns the number of bytes allocated for the address
        size_t ptrSize(void* address);

        // resizes the size of the memory arena to new_arena_size (in bytes).
        // all data will be cleared on a resize, so using pointers returned by alloc before a resize is undefined behavior.
        void resize(size_t new_arena_size);

        // returns wether the address passed was returned by Alloc and is not freed.
        bool isValid(void* address);

        // flushes every value of every byte in the memory arena to the stream passed.
        void memoryDump(std::ostream& stream)
        {
            for (size_t i = 0; i < m_arena_size; i++)
                stream << (int)m_arena[i] << ' ';
            stream << '\n';
        }


    private:
        // ARENA STRUCTURE DEFINITION:
        // a memory block starts of with an 8/4 byte (depending on architecture) value representing the size of the block, followed by the actual data of the block.
        // MEM_BLOCK = BLOCK_SIZE + DATA...
        // ARENA = MEM_BLOCK + UNUSED_MEM
        // the address returned by alloc will point to the start of MEM and not BLOCK_SIZE.
        // the unused memory in an arena is always zero initialized, thus making it possible to see what parts of the arena are in use.

        byte* m_arena;
        size_t m_arena_size;

    private:

        // finds an adress that has enough memory to store the passed memory block size.
        // returns a nullptr if no address were found.
        byte* findFreeAddress(size_t size);

    };
    

    // a structure containing information about a specific memory block
    struct MemBlockInfo
    {
        byte* start;
        byte* end;

        size_t size() { return (size_t)(end - start); }

        bool operator==(const MemBlockInfo& other) { return start == other.start && end == other.end; }
        bool operator!=(const MemBlockInfo& other) { return !(*this == other); }
    };

    class ModArena;

    // a class containing a pointer.
    // is returned when alloc is called from an ModArena.
    template<typename T>
    class ArenaPtr
    {
        friend ModArena;

        // used to make sure m_pos is valid
        const MemBlockInfo* m_mem_info;

        // stores the position inside the memory block
        T* m_pos;

        ArenaPtr(MemBlockInfo* mem_info) : m_mem_info(mem_info), m_pos((T*)m_mem_info->start) {};
        ArenaPtr() : m_mem_info(nullptr), m_pos(nullptr) {}

    public:

        ArenaPtr(ArenaPtr<T>& other)
            : m_mem_info(other.m_mem_info), m_pos(other.m_pos) {}

        // cast constructor
        template<typename TOther>
        ArenaPtr(ArenaPtr<TOther>& other) : m_mem_info(other.blockInfo()), m_pos((T*)(TOther*)other) {}

        ArenaPtr& operator=(const void* const other)
        {
            assert(m_mem_info);
            assert(other >= m_mem_info->start && other <= m_mem_info->end);
            m_pos = other;
            return *this;
        }

        size_t length() const
        {
            assert(m_mem_info);
            return (m_mem_info->end - m_mem_info->start) / sizeof(T);
        }

        T& operator->() { assert(m_mem_info); return *m_pos; }

        T& operator[](size_t index);
        T operator[](size_t index) const;

        operator T* const() { return m_pos; }
        operator const T* const() const { return m_pos; }

        T& operator*(ArenaPtr& ptr) { return *ptr.m_pos; }

        ArenaPtr& operator++(int);
        ArenaPtr operator++();
        ArenaPtr& operator--(int);
        ArenaPtr operator--();

        ArenaPtr operator+(size_t val);
        ArenaPtr operator-(size_t val);

        ArenaPtr& operator+=(size_t val);
        ArenaPtr& operator-=(size_t val);

        const T* start() const { return (T*)m_mem_info->start; }
        const T* end() const { return (T*)m_mem_info->end; }

        const MemBlockInfo* blockInfo() const { return m_mem_info; }

        // sets the position of the pointer to the start of the memory block.
        // should be called if the arena has been resized or defragmented.
        void reset() { assert(m_mem_info); m_pos = m_mem_info->start; }
    };

    // void specialization for ArenaPtr
    template<>
    class ArenaPtr<void> : public ArenaPtr<char>
    {
    public:
        ArenaPtr(ArenaPtr<void>& other)
            : ArenaPtr<char>(other) {}

        // cast constructor
        template<typename TOther>
        ArenaPtr(ArenaPtr<TOther>& other) : ArenaPtr<char>(other) {}

        void operator[](size_t) = delete;
        void operator->() = delete;
        void operator*() = delete;
    };

    // an arena that has the ability to modify the structure of itself without clearing the data stored.
    //
    // information about the memory block is allocated on the heap, every time an allocation is made.
    // is resizable without clearing the data stored.
    // can defragment the data stored.
    // arena_size does define how many bytes can be allocated from the arena, unlike StaticArena.
    //
    class ModArena
    {
    public:
        // initializes the arena memory with a specific size.
        ModArena(size_t arena_size);
        ~ModArena() { delete[] m_arena; }

        // allocates a memory block of size sizeof(T) * amount inside the arena.
        // returns nullptr if memory allocation failed.
        template<typename T>
        ArenaPtr<T> alloc(size_t amount = 1);

        // frees the passed address from the arena.
        void free(ArenaPtr<void> address);

        // resizes the size of the memory arena to new_arena_size (in bytes).
        // pointers returned by alloc will still be usable if they are reset.
        // memory will be defragmeneted after a resize.
        void resize(size_t new_arena_size);

        // removes unused memory inbetween memory blocks.
        // this operation functions by moving memory blocks next to each other by copying them, so this is an expensive operation.
        void defragment();

        // flushes every value of every byte in the memory arena, to the stream passed.
        void memoryDump(std::ostream& stream)
        {
            for (size_t i = 0; i < m_arena_size; i++)
                stream << (int)m_arena[i] << ' ';
            stream << '\n';
        }

    private:

        // ARENA STRUCTURE DEFINITION:
        //
        // the arena itself has no indicator where memory blocks start or end. This is what m_mem_info keeps track of.
        // this means that memory blocks are stored without additional information inside the arena
        // MEM_BLOCK = DATA...
        // ARENA = MEM_BLOCK + UNUSED_MEM
        // MEM_INFO = [MEM_BLOCK_START, MEM_BLOCK_END]...
        // the unused memory in the arena is always zero initialized.

        byte* m_arena;
        size_t m_arena_size;


        std::vector<MemBlockInfo> m_mem_info;

        // finds an adress that has enough memory to store the passed memory block size.
        // returns a nullptr if no address were found.
        std::pair<byte*, size_t> findFreeAddress(size_t size);
    };


    template<typename T>
    using APtr = ArenaPtr<T>;
};

#include "StaticArena.ipp"
#include "ModArena.ipp"
#include "ArenaPtr.ipp"
