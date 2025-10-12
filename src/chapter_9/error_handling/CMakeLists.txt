add_executable(error_handling main.cpp)

target_link_libraries(error_handling PRIVATE Boost::asio Boost::system)
