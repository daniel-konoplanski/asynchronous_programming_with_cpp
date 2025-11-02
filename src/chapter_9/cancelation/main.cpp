#include <boost/asio.hpp>
#include <iostream>

using namespace std::chrono_literals;

void handle_timeout(const boost::system::error_code& ec)
{
    if (ec == boost::asio::error::operation_aborted)
    {
        std::cout << "Timer canceled.\n";
    }
    else if (!ec)
    {
        std::cout << "Timer expired.\n";
    }
    else
    {
        std::cout << "Error: " << ec.message() << std::endl;
    }
}

int main()
{
    boost::asio::io_context   io{};
    boost::asio::steady_timer timer(io, 5s);
    timer.async_wait(handle_timeout);
    std::this_thread::sleep_for(2s);
    timer.cancel();
    io.run();

    return 0;
}
