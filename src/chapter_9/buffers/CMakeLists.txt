add_executable(buffers main.cpp)

target_link_libraries(buffers PRIVATE Boost::asio Boost::system)
