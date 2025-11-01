add_executable(signals main.cpp)

target_link_libraries(signals PRIVATE Boost::asio Boost::system)
