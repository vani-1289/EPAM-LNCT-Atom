#include <iostream>
#include <thread>

int data = 0;
bool ready = false;

void producer()
{
    data = 42;
    ready = true;
}

void consumer()
{
    while (!ready)
    {
    }

    std::cout << "Consumer received data = "
              << data << std::endl;
}

int main()
{
    std::thread t1(producer);
    std::thread t2(consumer);

    t1.join();
    t2.join();

    return 0;
}
