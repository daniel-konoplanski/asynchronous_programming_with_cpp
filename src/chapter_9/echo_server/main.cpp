#include <array>
#include <boost/asio/write_at.hpp>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/detail/error_code.hpp>

using namespace std::chrono_literals;
using boost::asio::ip::tcp;
using IoContextPtr   = std::shared_ptr<boost::asio::io_context>;
using TcpAcceptorPtr = std::shared_ptr<boost::asio::ip::tcp::acceptor>;

class Session : public std::enable_shared_from_this<Session>
{
public:
    Session(tcp::socket socket)
        : socket_(std::move(socket))
    {}

    void start()
    {
        do_read();
    }

private:
    void do_read()
    {
        auto self(shared_from_this());

        socket_.async_read_some(
            boost::asio::buffer(data_, max_length),
            [this, self](boost::system::error_code ec, std::size_t length)
            {
                if (!ec) do_write(length);
            });
    }

    void do_write(uint32_t length)
    {
        auto self{ shared_from_this() };
        boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
        [this, self](boost::system::error_code ec, std::size_t) {
            if (!ec) do_read();
        });
    }

private:
    static constexpr uint32_t max_length{ 1024 };
    tcp::socket socket_;
    std::array<char, max_length> data_;
};

class EchoServer
{
public:
    EchoServer(IoContextPtr io)
        : ioContextPtr_{ std::move(io) }
    {
        tcp::endpoint endpoint(tcp::v4(), port_number);
        acceptor_ = std::make_shared<tcp::acceptor>(tcp::acceptor(*ioContextPtr_, endpoint));

        do_accept();
    }

private:
    void do_accept()
    {
        acceptor_->async_accept([this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec)
            {
                auto session = std::make_shared<Session>(std::move(socket));
                session->start();

                do_accept();
            }
        });
    }

private:
    constexpr static uint32_t port_number{ 1234 };
    IoContextPtr              ioContextPtr_;
    TcpAcceptorPtr            acceptor_;
};

int main()
{
    IoContextPtr io{ std::make_shared<boost::asio::io_context>() };

    std::cout << "Starting server...\n";

    try
    {
        EchoServer server(io);
        io->run();
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }

    std::cout << "Exiting...\n";

    return 0;
}
