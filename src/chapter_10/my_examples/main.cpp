#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/cobalt.hpp>
#include <boost/cobalt/op.hpp>
#include <iostream>
#include <print>

namespace cobalt = boost::cobalt;

cobalt::generator<int> squares(int max)
{
    std::cout << "squares was called\n";
    for (int i = 0; i <= max; ++i)
    {
        std::println("squares getting next value...");
        co_yield i* i;
    }

    co_return {};
}

// cobalt::main co_main(int, char**)
// {
//     std::println("co_main before generator initialized");
//     auto gen = squares(5);  // Nothing runs here yet
//     std::println("co_main after generator initialized");

//     while (gen)  // Check if the generator has more values
//     {
//         std::println("co_main getting next value...");

//         auto val = co_await gen;  // Await to get the next value
//         std::cout << val << " ";
//     }
//     std::cout << "\n";

//     co_return 0;
// }

using namespace std::chrono_literals;

cobalt::main co_main(int, char**)
{
    auto                      exec = co_await cobalt::this_coro::executor;
    boost::asio::steady_timer tmr1{ exec, 1s };
    boost::asio::steady_timer tmr2{ exec, 2s };
    boost::asio::steady_timer tmr3{ exec, 3s };
    boost::asio::steady_timer tmr4{ exec, 4s };

    std::vector ops = { tmr1.async_wait(cobalt::use_op),
                        tmr2.async_wait(cobalt::use_op),
                        tmr3.async_wait(cobalt::use_op),
                        tmr4.async_wait(cobalt::use_op) };

    int count = 1;
    while (!ops.empty())
    {
        auto idx = co_await boost::cobalt::race(ops);
        std::cout << "Timer " << count++ << " finished (original index: " << idx << ")\n";
        ops.erase(ops.begin() + idx);
    }

    co_return 0;
}