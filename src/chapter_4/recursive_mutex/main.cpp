#include <cstdint>
#include <iostream>
#include <mutex>
#include <print>
#include <syncstream>
#include <thread>

std::recursive_mutex mtx{};

void func(uint32_t start, uint32_t threadId)
{
    if (start == 0) return;

    std::lock_guard<std::recursive_mutex> lg{ mtx };
    // std::print("func called: {}, from thread: {}\n", start, threadId);
    std::cout << "func called: " << start << ", from thread: " << threadId << "\n";

    func(--start, threadId);
}

int main()
{
    auto jt1 = std::jthread(&func, 10, 1);
    auto jt2 = std::jthread(&func, 10, 2);

    return 0;
}
