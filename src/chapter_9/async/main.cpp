#include <boost/asio.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/system/detail/error_code.hpp>

#include <functional>
#include <iostream>
#include <print>
#include <thread>
#include <ranges>

using namespace std::chrono_literals;

int main()
{
    boost::asio::io_context io{};
    boost::asio::steady_timer timer(io, 2s);

    std::function<void(const boost::system::error_code&)> timerHandler;
    timerHandler = [&timer, &timerHandler](const boost::system::error_code& ec)
    {
        if (!ec)
        {
            std::cout << "Handler: Timer expired.\n";
            timer.expires_after(1s);
            timer.async_wait(timerHandler);
        }
        else
        {
            std::cerr << "Handler error: " << ec.message() << std::endl;
        }
    };

    timer.async_wait(timerHandler);

    boost::asio::post(io, []() {

        for (uint32_t i : std::views::iota(1, 1000))
            std::cout << "Doing some stuff in task " << i << "\n";
    });

    io.run();

    return 0;
}