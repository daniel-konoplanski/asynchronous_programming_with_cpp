#include "timeout/Timeout.hpp"
#include <thread>

using namespace std::chrono_literals;

int main()
{
    auto timeout = timeout::Timeout([]() { std::osyncstream(std::cout) << "Doing some stuff in the callback\n"; });
    timeout.start(2s);

    std::this_thread::sleep_for(1s);

    timeout.handle();

    std::this_thread::sleep_for(4s);

    timeout.stop();

    return 0;
}