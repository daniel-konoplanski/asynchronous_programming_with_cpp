#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/system/detail/error_code.hpp>
#include <boost/asio.hpp>

#include <print>

using namespace std::chrono_literals;

void onTimeout(const boost::system::error_code& ec)
{
    std::print("Timer timed out with code {}\n", ec.to_string());
}

int main()
{
    boost::asio::io_context ioContext{};
    boost::asio::steady_timer timer{ioContext, 2s};

    timer.async_wait(&onTimeout);
    ioContext.run();

    return 0;
}
