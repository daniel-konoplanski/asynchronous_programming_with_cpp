#include <chrono>
#include <cstdint>
#include <ostream>
#include <print>

#include <boost/asio/steady_timer.hpp>
#include <boost/asio/this_coro.hpp>
#include <boost/cobalt.hpp>
#include <boost/cobalt/join.hpp>
#include <boost/cobalt/op.hpp>
#include <boost/cobalt/promise.hpp>

using namespace std::chrono_literals;

enum class ResultCode : uint8_t
{
    SUCCESS = 0,
    FAILURE = 1
};

template <>
struct std::formatter<ResultCode> : std::formatter<std::string_view>
{
    auto format(ResultCode r, format_context& ctx) const
    {
        std::string_view name;

        switch (r)
        {
            case ResultCode::SUCCESS: name = "SUCCESS"; break;
            case ResultCode::FAILURE: name = "FAILURE"; break;
            default: name = "Unknown";
        }

        return std::formatter<std::string_view>::format(name, ctx);
    }
};

boost::cobalt::promise<ResultCode> dataProcessing(std::chrono::milliseconds interval)
{
    std::println("Starting timer with interval {}", interval);

    boost::asio::steady_timer tmr{ co_await boost::cobalt::this_coro::executor, interval };
    co_await tmr.async_wait(boost::cobalt::use_op);

    if (interval < 5000ms) co_return ResultCode::SUCCESS;

    co_return ResultCode::FAILURE;
}

boost::cobalt::main co_main(int, char**)
{
    auto result = co_await boost::cobalt::join(
        dataProcessing(1000ms),
        dataProcessing(2000ms),
        dataProcessing(3000ms),
        dataProcessing(4000ms),
        dataProcessing(6000ms));

    auto [r1, r2, r3, r4, r5] = result;

    std::println("Code r1: {}", r1);
    std::println("Code r2: {}", r2);
    std::println("Code r3: {}", r3);
    std::println("Code r4: {}", r4);
    std::println("Code r5: {}", r5);

    co_return {};
}
