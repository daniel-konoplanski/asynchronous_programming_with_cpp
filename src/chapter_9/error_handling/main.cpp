#include <boost/asio.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/system/detail/error_code.hpp>

#include <boost/system/system_error.hpp>
#include <functional>
#include <iostream>
#include <print>
#include <thread>
#include <ranges>

using namespace std::chrono_literals;

int main()
{
    boost::asio::io_context io{};
    boost::asio::steady_timer timer(io, 3s);

    auto fut = timer.async_wait(boost::asio::use_future);

    std::jthread ioThread([&io]() {
        io.run();
    });

    std::this_thread::sleep_for(1s);
    timer.cancel();
    
    try
    {
        fut.get();
        std::cout << "Timer expired successfully!\n";
    }
    catch (const boost::system::system_error& e)
    {
        std::cout << "Timer failed: " << e.code().message() << '\n';
    }

    ioThread.join();

    return 0;
}
