add_executable(timer_strand timer.cpp)
target_link_libraries(timer_strand PRIVATE Boost::asio Boost::system)

add_executable(logger_strand logger.cpp)
target_link_libraries(logger_strand PRIVATE Boost::asio Boost::system)
