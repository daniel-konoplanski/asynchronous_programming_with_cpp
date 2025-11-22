add_executable(generators main.cpp)
target_link_libraries(generators Boost::cobalt)

add_executable(fib fib.cpp)
target_link_libraries(fib Boost::cobalt)
