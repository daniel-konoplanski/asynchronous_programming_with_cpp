add_executable(stream_buffers main.cpp)

target_link_libraries(stream_buffers PRIVATE Boost::asio Boost::system)
