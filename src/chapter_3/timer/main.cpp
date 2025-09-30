#include "timer/Timer.hpp"

#include <iostream>
#include <syncstream>
#include <thread>

using namespace std::chrono_literals;

int main()
{
    auto timer = timer::Timer([]() { std::osyncstream(std::cout) << "Doing some stuff in the callback\n"; });
    timer.start(500ms);

    std::this_thread::sleep_for(5s);

    timer.stop();

    std::this_thread::sleep_for(2s);

    return 0;
}