#pragma once

#include <array>
#include <mutex>
#include "SpinLock.h"

template<typename T, size_t Capacity>
class RingBuffer
{
public:
    inline bool push_back(const T& item)
    {
        SpinLock guard(m_lock);
        auto next = (m_head + 1) % Capacity;
        if(next == m_tail) return false;

        m_underlyingData[m_head] = item;
        m_head = next;
        return true;
    }

    inline bool pop_front(T& item)
    {
        SpinLock guard(m_lock);
        if(m_tail == m_head) return false;

        item = m_underlyingData[m_tail];
        m_tail = (m_tail + 1) % Capacity;
        return true;
    }

private:
    std::array<T, Capacity> m_underlyingData;
    size_t m_head = 0;
    size_t m_tail = 0;
    std::atomic_bool m_lock;
};