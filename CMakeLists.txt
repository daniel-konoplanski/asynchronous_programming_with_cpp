cmake_minimum_required(VERSION 3.31)

project(default VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add extra warning flags to all targets
# set(WARNING_FLAGS -Wall -Wextra -Wpedantic -Werror)
# add_compile_options(${WARNING_FLAGS})

find_package(boost_asio CONFIG REQUIRED)
find_package(boost_cobalt CONFIG REQUIRED)

add_subdirectory(src)
