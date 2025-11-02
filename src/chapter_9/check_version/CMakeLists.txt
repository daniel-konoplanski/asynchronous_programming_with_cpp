add_executable(check_version main.cpp)
target_link_libraries(check_version PRIVATE Boost::asio Boost::system)
