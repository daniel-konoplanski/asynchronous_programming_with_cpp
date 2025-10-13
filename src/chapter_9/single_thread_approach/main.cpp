#include <boost/asio.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/system/detail/error_code.hpp>
#include <print>

using namespace std::chrono_literals;

int main()
{
    boost::asio::io_context io{};

    boost::asio::steady_timer st1(io, 2s);
    boost::asio::steady_timer st2(io, 3s);

    st1.async_wait(
        [](const boost::system::error_code& ec)
        {
            std::print("st1 run out with code {}\n", ec.to_string());
        });

    st2.async_wait(
        [](const boost::system::error_code& ec)
        {
            std::print("st2 run out with code {}\n", ec.to_string());
        });

    io.run();
}
