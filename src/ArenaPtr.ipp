#include "Arena.h"

namespace ADS
{
    template<typename T>
    T& ArenaPtr<T>::operator[](size_t index)
    {
        assert(m_mem_info);
        assert((byte*)(m_pos + index) <= m_mem_info->end);
        return m_pos[index];
    }

    template<typename T>
    T ArenaPtr<T>::operator[](size_t index) const 
    {
        assert(m_mem_info);
        assert((byte*)(m_pos + index) <= m_mem_info->end);
        return m_pos[index];
    }

    template<typename T>
    ArenaPtr<T>& ArenaPtr<T>::operator++(int)
    {
        assert(m_mem_info);
        assert((byte*)(m_pos + 1) <= m_mem_info->end);
        m_pos++;

        return *this;
    }

    template<typename T>
    ArenaPtr<T> ArenaPtr<T>::operator++()
    {
        ArenaPtr<T> tmp = m_pos;
        *this++;
        return tmp;
    }

    template<typename T>
    ArenaPtr<T>& ArenaPtr<T>::operator--(int)
    {
        assert((byte*)(m_pos - 1) >= m_mem_info->start);
        m_pos--;

        return *this;
    }

    template<typename T>
    ArenaPtr<T> ArenaPtr<T>::operator--()
    {
        ArenaPtr<T> tmp = m_pos;
        *this--;
        return tmp;
    }


    template<typename T>
    ArenaPtr<T> ArenaPtr<T>::operator+(size_t val)
    {
        ArenaPtr<T> result(*this);
        result += val;
        return result;
    }

    template<typename T>
    ArenaPtr<T> ArenaPtr<T>::operator-(size_t val)
    {
        ArenaPtr<T> result(*this);
        result -= val;
        return result;
    }

    template<typename T>
    ArenaPtr<T>& ArenaPtr<T>::operator+=(size_t val)
    {
        assert(m_mem_info);
        assert((byte*)(m_pos + val) <= m_mem_info->end);
        m_pos += val;
    }
    
    template<typename T>
    ArenaPtr<T>& ArenaPtr<T>::operator-=(size_t val)
    {
        assert(m_mem_info);
        assert((byte*)(m_pos - val) >= m_mem_info->start);
        m_pos -= val;
    }
}
