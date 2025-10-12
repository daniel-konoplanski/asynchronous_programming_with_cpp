add_executable(async main.cpp)

target_link_libraries(async PRIVATE Boost::asio Boost::system)
