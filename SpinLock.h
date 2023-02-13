#pragma once

#include <atomic>

struct SpinLock
{
    SpinLock(std::atomic_bool& flag) : m_flag(flag)
    {
        for(;;)
        {
            if(!m_flag.exchange(true, std::memory_order_acquire))
            {
                break;
            }

            while(m_flag.load(std::memory_order_relaxed))
            {
#if defined(__GNUC__) && !defined(__llvm__)
                __builtin_ia32_pause();
#else
                std::this_thread::yield();
#endif
            }
        }
    }

    ~SpinLock()
    {
        m_flag = false;
    }

private:
    std::atomic_bool& m_flag;
};