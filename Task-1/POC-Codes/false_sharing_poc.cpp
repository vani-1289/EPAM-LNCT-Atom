#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

const long long ITERATIONS = 100000000;

// Two counters may share the same cache line.
struct NormalCounters
{
    std::atomic<long long> a{0};
    std::atomic<long long> b{0};
};

// Force counters onto separate cache lines.
struct PaddedCounters
{
    alignas(64) std::atomic<long long> a{0};
    alignas(64) std::atomic<long long> b{0};
};

void incrementA(std::atomic<long long>& counter)
{
    for (long long i = 0; i < ITERATIONS; i++)
    {
        counter.fetch_add(1, std::memory_order_relaxed);
    }
}

void incrementB(std::atomic<long long>& counter)
{
    for (long long i = 0; i < ITERATIONS; i++)
    {
        counter.fetch_add(1, std::memory_order_relaxed);
    }
}

long long runNormal()
{
    NormalCounters counters;

    auto start = std::chrono::high_resolution_clock::now();

    std::thread t1(incrementA, std::ref(counters.a));
    std::thread t2(incrementB, std::ref(counters.b));

    t1.join();
    t2.join();

    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(
        end - start
    ).count();
}

long long runPadded()
{
    PaddedCounters counters;

    auto start = std::chrono::high_resolution_clock::now();

    std::thread t1(incrementA, std::ref(counters.a));
    std::thread t2(incrementB, std::ref(counters.b));

    t1.join();
    t2.join();

    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(
        end - start
    ).count();
}

int main()
{
    std::cout << "Running false sharing experiment...\n\n";

    long long normalTime = runNormal();
    long long paddedTime = runPadded();

    std::cout << "Normal counters time : "
              << normalTime << " ms\n";

    std::cout << "Padded counters time : "
              << paddedTime << " ms\n";

    if (paddedTime < normalTime)
    {
        std::cout << "\nPadded version was faster.\n";
    }
    else
    {
        std::cout << "\nThis run did not show a speedup.\n";
        std::cout << "Run the experiment several times.\n";
    }

    return 0;
}
