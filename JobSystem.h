#pragma once

#include <cstdint>
#include <functional>
#include "RingBuffer.h"

using Task = std::function<void()>;

class JobSystem
{
public:
    JobSystem();
    explicit JobSystem(uint16_t threads);

    ~JobSystem();

    void Execute(const Task& task);

    void Dispatch(uint32_t jobCount, uint32_t groupSize, const Task& job);

    bool IsBusy();

    void Wait();

private:
    RingBuffer<Task, 255> m_tasks;
    std::condition_variable m_wakeCondition;
    std::mutex m_wakeMutex;

    std::atomic_uint64_t m_currentLabel = 0;
    std::atomic_uint64_t m_finishedLabel = 0;

    void Poll();
};