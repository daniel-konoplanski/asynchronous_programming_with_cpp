add_executable(cancelation main.cpp)

target_link_libraries(cancelation PRIVATE Boost::asio Boost::system)
