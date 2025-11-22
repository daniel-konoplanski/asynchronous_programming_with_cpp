#include <cstdint>
#include <print>

#include <boost/cobalt.hpp>
#include <boost/cobalt/generator.hpp>

boost::cobalt::generator<uint32_t> generate(uint32_t limit)
{
    co_await boost::cobalt::this_coro::initial;

    for (uint32_t i{}; i <= limit; ++i)
    {
        co_yield i;
    }

    co_return {};
}

boost::cobalt::main co_main(int, char**)
{
    auto gen = generate(10);

    while (gen)
    {
        auto number = co_await gen;
        std::println("Got number {}", number);
    }

    co_return {};
}
