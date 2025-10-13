add_executable(multiple_threads_single_io main.cpp)

target_link_libraries(multiple_threads_single_io PRIVATE Boost::asio Boost::system)
