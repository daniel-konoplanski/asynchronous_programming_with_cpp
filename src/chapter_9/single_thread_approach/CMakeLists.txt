add_executable(single_thread_approach main.cpp)

target_link_libraries(single_thread_approach PRIVATE Boost::asio Boost::system)
