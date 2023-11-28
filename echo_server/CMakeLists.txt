cmake_minimum_required(VERSION 3.26)
project(echo_server)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(echo_server main.cpp)

target_include_directories(echo_server PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(echo_server ${Boost_LIBRARIES})
