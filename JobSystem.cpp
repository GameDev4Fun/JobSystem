#include <thread>

#if WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cassert>

#endif

#include "JobSystem.h"

JobSystem::JobSystem() {
    auto threadNumber = std::thread::hardware_concurrency() - 1;
    threadNumber = std::max(1u, threadNumber);

    for(int i = 0; i < threadNumber; ++i)
    {
        std::thread worker([&](){
            Task job;

            while(true)
            {
                if(m_tasks.pop_front(job))
                {
                    job();
                    m_finishedLabel.fetch_add(1);
                }
                else
                {
                    std::unique_lock lg(m_wakeMutex);
                    m_wakeCondition.wait(lg);
                }
            }
        });

#ifdef WIN32
        HANDLE handle = worker.native_handle();
        DWORD_PTR affinityMask = 1ull << i;
        DWORD_PTR result = SetThreadAffinityMask(handle, affinityMask);
        assert(result > 0);

        SetThreadPriority(handle, THREAD_PRIORITY_HIGHEST);
#endif

        worker.detach();
    }
}

JobSystem::JobSystem(uint16_t threads) {

}

JobSystem::~JobSystem() {

}

void JobSystem::Execute(const Task &task)
{
    m_currentLabel.fetch_add(1);

    while(!m_tasks.push_back(task))
    {
        Poll();
    }

    m_wakeCondition.notify_one();
}

void JobSystem::Dispatch(uint32_t jobCount, uint32_t groupSize, const Task &job) {

}

void JobSystem::Wait()
{
    while(IsBusy())
    {
        Poll();
    }
}

bool JobSystem::IsBusy()
{
    return m_finishedLabel.load() < m_currentLabel.load();
}

void JobSystem::Poll()
{
    m_wakeCondition.notify_one();
    std::this_thread::yield();
}
