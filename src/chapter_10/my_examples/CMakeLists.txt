add_executable(my_examples main.cpp)

target_link_libraries(my_examples PRIVATE Boost::asio Boost::system Boost::cobalt)
