#include <array>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/system/detail/error_code.hpp>
#include <cstddef>
#include <istream>
#include <print>
#include <string>
#include <vector>
#include <version>

using namespace std::chrono_literals;

using tcp = boost::asio::ip::tcp;

constexpr uint32_t port{ 1234 };

void handle_client(tcp::socket& socket)
{
    constexpr size_t size_buffer = 5;
    boost::asio::streambuf buf1, buf2;
    std::array<boost::asio::mutable_buffer, 2> buffers {
        buf1.prepare(size_buffer), buf2.prepare(size_buffer)
    };

    boost::system::error_code ec{};

    size_t bytes_recv = socket.read_some(buffers, ec);

    if (ec)
    {
        std::print("Error on receive {}\n", ec.message());
        return;
    }

    std::print("Received {} bytes\n", bytes_recv);
    buf1.commit(5);
    buf2.commit(5);

    std::istream is1(&buf1);
    std::istream is2(&buf2);

    std::string data1, data2;
    is1 >> data1, is2 >> data2;

    std::print("Buffer 1: {}\n", data1);
    std::print("Buffer 2: {}\n", data2);
}

int main()
{
    boost::asio::io_context io{};
    auto tcp_acceptor = tcp::acceptor(io, tcp::endpoint(tcp::v4(), port));

    std::print("Server is running on port {}...\n", port);

    while (true)
    {
        std::print("cp1\n");
        tcp::socket socket(io);
        std::print("cp2\n");
        tcp_acceptor.accept(socket);

        std::print("Client connected with address {}\n", socket.remote_endpoint().address().to_string());
        handle_client(socket);
        std::print("Client disconnected\n");
    }

    return 0;
}