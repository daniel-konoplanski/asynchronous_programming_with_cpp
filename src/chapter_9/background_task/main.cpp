#include <boost/asio.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/system/detail/error_code.hpp>

#include <print>
#include <thread>

using namespace std::chrono_literals;

void backgroundTask(boost::asio::io_context& ioContext)
{
    std::this_thread::sleep_for(2s);

    std::print("Posting background task\n");

    auto performWork = []() {
        std::this_thread::sleep_for(1s);
        std::print("Doing some work\n");
        std::this_thread::sleep_for(1s);
    };

    boost::asio::post(ioContext, performWork);
}

int main()
{
    boost::asio::io_context ioContext{};

    auto workGuard = boost::asio::make_work_guard(ioContext);

    backgroundTask(ioContext);
    ioContext.run();

    workGuard.reset();

    return 0;
}
