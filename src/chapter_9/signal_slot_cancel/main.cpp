#include <boost/asio.hpp>
#include <boost/asio/bind_cancellation_slot.hpp>
#include <boost/asio/cancellation_signal.hpp>
#include <boost/asio/cancellation_type.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/system/detail/error_code.hpp>
#include <print>
#include <thread>

using namespace std::chrono_literals;

int main()
{
    boost::asio::io_context io{};

    boost::asio::steady_timer timer(io, 5s);
    boost::asio::cancellation_signal cancel_signal{};

    auto timeout_callback = [](const boost::system::error_code& ec) {
        if (ec)
        {
            std::print("Timmer finished with status: \"{}\"\n", ec.message());
            return;
        }

        std::print("Timmer finished successfully");
    };

    timer.async_wait(boost::asio::bind_cancellation_slot(cancel_signal.slot(), timeout_callback));

    std::this_thread::sleep_for(2s);
    cancel_signal.emit(boost::asio::cancellation_type::all);

    io.run();

    return 0;
}
