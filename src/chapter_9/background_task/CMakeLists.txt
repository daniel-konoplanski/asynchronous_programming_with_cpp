add_executable(asio_timer main.cpp)

target_link_libraries(asio_timer PRIVATE Boost::asio Boost::system)
