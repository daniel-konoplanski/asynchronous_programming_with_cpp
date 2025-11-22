#include <boost/cobalt.hpp>
#include <boost/cobalt/promise.hpp>
#include <cstdint>
#include <print>

boost::cobalt::promise<void> producer(boost::cobalt::channel<int32_t>& channel)
{
    std::println("producer called");

    for (int32_t i{}; i < 10; ++i)
    {
        // std::println("wiritng {} to channel", i);
        co_await channel.write(i);
    }

    std::println("closing channel");

    channel.close();

    co_return;
}

boost::cobalt::main co_main(int, char**)
{
    auto channel = boost::cobalt::channel<int32_t>();
    auto prod    = producer(channel);

    while (channel.is_open())
    {
        auto value = co_await channel.read();
        std::println("Value produced: {}", value);
    }

    co_await prod;

    co_return {};
}
