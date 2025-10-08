#include <cstdint>
#include <mutex>
#include <print>
#include <shared_mutex>
#include <ranges>
#include <thread>

std::shared_mutex mtx{};
using namespace std::chrono_literals;

uint32_t globalVar{ 0 };

void readerFunc()
{
    while (true)
    {
        {
            std::shared_lock<std::shared_mutex> shl{ mtx };
            std::print("Element read: {} from thread: {}\n", globalVar, std::this_thread::get_id());
        }

        std::this_thread::sleep_for(200ms);
    }
}

void writerFunc()
{
    for (uint32_t i : std::views::iota(1, 101))
    {
        {
            std::unique_lock<std::shared_mutex> uql{ mtx };
            globalVar = i;
        }

        std::this_thread::sleep_for(100ms);
    }
}

int main()
{
    auto writer = std::jthread(writerFunc);
    auto reader1 = std::jthread(readerFunc);
    auto reader2 = std::jthread(readerFunc);

    return 0;
}
