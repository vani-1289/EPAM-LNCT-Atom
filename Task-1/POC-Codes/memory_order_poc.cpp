#include <iostream>
#include <thread>
#include <atomic>

int data = 0;
std::atomic<bool> ready(false);

void producer()
{
    // Step 1: Prepare the data
    data = 42;

    // Step 2: Publish the data using RELEASE
    ready.store(true, std::memory_order_release);
}

void consumer()
{
    // Wait until producer publishes the data
    while (!ready.load(std::memory_order_acquire))
    {
    }

    // Because of acquire-release ordering,
    // data is guaranteed to be visible here.
    std::cout << "Consumer received data = " << data << std::endl;
}

int main()
{
    std::thread t1(producer);
    std::thread t2(consumer);

    t1.join();
    t2.join();

    return 0;
}
