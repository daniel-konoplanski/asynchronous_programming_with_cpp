#include "timer/Timer.hpp"

#include <iostream>
#include <memory>
#include <syncstream>
#include <thread>

using namespace std::chrono_literals;

void func()
{
    std::osyncstream(std::cout) << "Doing some stuff in the callback 2\n";
}

int main()
{
    auto timer = timer::Timer([]() { std::osyncstream(std::cout) << "Doing some stuff in the callback\n"; });
    timer.start(500ms);

    std::this_thread::sleep_for(5s);

    timer.stop();

    std::this_thread::sleep_for(2s);

    auto timer2 = timer::Timer(func);

    return 0;
}