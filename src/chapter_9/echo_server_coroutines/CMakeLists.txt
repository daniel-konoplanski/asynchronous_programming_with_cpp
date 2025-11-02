add_executable(echo_server_coroutines main.cpp)
target_link_libraries(echo_server_coroutines PRIVATE Boost::asio Boost::system)
