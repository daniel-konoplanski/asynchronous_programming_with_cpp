#include <boost/cobalt.hpp>
#include <boost/cobalt/promise.hpp>
#include <cstdint>
#include <print>
#include <random>

boost::cobalt::promise<int32_t> getRand(int32_t min, int32_t max)
{
    std::println("started getRand");

    std::random_device            rd{};
    std::mt19937                  mt{ rd() };
    std::uniform_int_distribution dist(min, max);

    std::println("blocking getRand");

    co_return dist(mt);
}

boost::cobalt::task<int32_t> getRandLazy(int32_t min, int32_t max)
{
    std::println("started getRand");

    std::random_device            rd{};
    std::mt19937                  mt{ rd() };
    std::uniform_int_distribution dist(min, max);

    std::println("blocking getRand");

    co_return dist(mt);
}


boost::cobalt::main co_main(int, char**)
{
    std::println("step1");

    auto result = getRand(1, 100);

    std::println("step2");

    std::println("Result: {}", co_await result);

    std::println("_____________________");

    std::println("step1");

    auto result2 = getRandLazy(1, 100);

    std::println("step2");

    std::println("Result: {}", co_await result2);

    co_return {};
}
