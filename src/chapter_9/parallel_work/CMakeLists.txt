add_executable(parallel_work main.cpp)

target_link_libraries(parallel_work PRIVATE Boost::asio Boost::system)
