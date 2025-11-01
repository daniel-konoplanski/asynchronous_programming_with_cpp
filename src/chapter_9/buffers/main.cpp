#include <array>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <print>
#include <vector>
#include <version>

using namespace std::chrono_literals;

int main()
{
    char data[1024];
    [[maybe_unused]] auto buffer = boost::asio::buffer(data);

    std::print("buffer size {}\n", buffer.size());
    std::print("buffer data {}\n", buffer.data());

    std::array<char, 128> buff1, buff2;

    auto mbuff1 = boost::asio::buffer(buff1);
    auto mbuff2 = boost::asio::buffer(buff2);

    std::vector<boost::asio::const_buffer> cbuffers{ mbuff1, mbuff2 };

    return 0;
}