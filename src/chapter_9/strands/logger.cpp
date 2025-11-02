
#include <boost/asio.hpp>

#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <fstream>
#include <thread>

class Logger
{
public:
    Logger(boost::asio::io_context& io, const std::string& filename)
        : io_(io)
        , strand_(io)
        , file_(filename, std::ios::out | std::ios::app)
    {
        if (!file_.is_open())
        {
            throw std::runtime_error("Failed to open log file");
        }
    }

    void log(const std::string message)
    {
        boost::asio::post(
            io_,
            strand_.wrap(
                [this, msg = std::move(message)]() mutable
                {
                    do_log(std::move(msg));
                }));
    }

    ~Logger()
    {
        if (file_.is_open()) file_.close();
    }

private:
    void do_log(const std::string message)
    {
        file_ << message << '\n';
    }

private:
    boost::asio::io_context&        io_;
    boost::asio::io_context::strand strand_;
    std::ofstream                   file_;
};

int main()
{
    boost::asio::io_context io{};

    Logger logger(io, "logfile.log");

    auto log_msg = [&logger](const auto msg)
    {
        return [&logger, msg]()
        {
            logger.log(msg);
        };
    };

    auto t1 = std::jthread(log_msg("Hello from t1"));
    auto t2 = std::jthread(log_msg("Hello from t2"));

    io.run();

    return 0;
}
