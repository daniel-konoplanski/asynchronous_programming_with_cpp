add_executable(signal_slot_cancel main.cpp)

target_link_libraries(signal_slot_cancel PRIVATE Boost::asio Boost::system)
