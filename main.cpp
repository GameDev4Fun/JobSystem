#include <array>
#include <chrono>
using namespace std::literals::chrono_literals;
#include <iostream>
#include <thread>
#include <cassert>
#include <algorithm>
#include <random>
#include <execution>
#include "JobSystem.h"

#include "RingBuffer.h"

int gen()
{
    static int i = 0;
    return ++i;
}

int main() {
    JobSystem jobSystem;

    std::vector<uint64_t> data;
    data.resize(1'000'000'000);

    std::mt19937 rng; // default constructed, seeded with fixed seed
    std::generate_n(std::execution::par_unseq,
                    data.begin(), 1'000'000'000, std::ref(rng));

    double sum;

    jobSystem.Dispatch(10, data.size()/10, []()
    {

    });

    for(int i = 0; i < 10; ++i) {
        jobSystem.Execute([]() {
            std::cout << "Test" << std::endl;
            std::this_thread::sleep_for(10s);
        });
    }

    std::cin.ignore();

    return 0;
}
