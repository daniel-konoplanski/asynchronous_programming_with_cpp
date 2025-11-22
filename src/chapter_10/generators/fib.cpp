#include <cstdint>
#include <iostream>
#include <print>
#include <ranges>

#include <boost/cobalt.hpp>
#include <boost/cobalt/generator.hpp>

boost::cobalt::generator<uint32_t> fib(uint32_t count)
{
    uint32_t a = 0;
    uint32_t b = 1;

    for (uint32_t i : std::ranges::views::iota(0U, count))
    {
        co_yield a;
        uint32_t next = a + b;
        a = b;
        b = next;
    }

    // co_return a;
}

boost::cobalt::main co_main(int, char**)
{
    auto gen = fib(10);

    while (gen)
    {
        std::println("{}", co_await gen);
    }

    co_return {};
}
