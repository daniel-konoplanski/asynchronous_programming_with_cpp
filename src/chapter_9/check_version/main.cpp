#include <boost/version.hpp>

#include <iostream>

int main()
{
    #ifdef ASIO_STANDALONE
        #include <asio/version.hpp>
        std::cout << "Standalone Asio, ASIO_VERSION=" << ASIO_VERSION << "\n";
    #else
        std::cout << "Boost, BOOST_VERSION=" << BOOST_VERSION << "\n";
    #endif
}
