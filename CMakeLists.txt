cmake_minimum_required(VERSION 3.24)
project(JobSystem)

set(CMAKE_CXX_STANDARD 17)

add_executable(JobSystem main.cpp JobSystem.h JobSystem.cpp RingBuffer.h SpinLock.h)
