add_executable(background_task main.cpp)

target_link_libraries(background_task PRIVATE Boost::asio Boost::system)
