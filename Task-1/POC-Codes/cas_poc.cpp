#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

std::atomic<int> counter(0);

void incrementCounter()
{
    for (int i = 0; i < 100000; i++)
    {
        int expected = counter.load(std::memory_order_relaxed);

        while (!counter.compare_exchange_weak(
            expected,
            expected + 1,
            std::memory_order_relaxed))
        {
            // CAS failed because another thread changed counter.
            // expected is automatically updated.
        }
    }
}

int main()
{
    const int threadCount = 4;

    std::vector<std::thread> threads;

    for (int i = 0; i < threadCount; i++)
    {
        threads.emplace_back(incrementCounter);
    }

    for (auto& t : threads)
    {
        t.join();
    }

    std::cout << "Expected counter = "
              << threadCount * 100000 << std::endl;

    std::cout << "Actual counter = "
              << counter.load() << std::endl;

    return 0;
}
