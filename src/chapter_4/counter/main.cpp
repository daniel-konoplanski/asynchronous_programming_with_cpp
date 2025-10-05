#include <iostream>
#include <mutex>
#include <thread>

int counter{};
std::mutex mtx{};

int main()
{
    auto func = []
    {
        for (int i = 0; i < 1000000; ++i)
        {
            std::lock_guard<std::mutex> lg{ mtx };
            counter++;
        }
    };

    std::thread t1(func);
    std::thread t2(func);

    t1.join();
    t2.join();

    std::cout << counter << std::endl;

    return 0;
}