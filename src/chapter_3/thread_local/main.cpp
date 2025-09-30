#include <cstddef>
#include <thread>
#include <syncstream>
#include <iostream>

thread_local int i{0};

int main(int, char*[])
{
    auto t1 = std::jthread(
        []()
        {
            i = 10;
        });

    t1.join();

    std::osyncstream(std::cout) << "main i: " << i << "\n";

    return 0;
}