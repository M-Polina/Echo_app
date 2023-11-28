
cmake_minimum_required(VERSION 3.26)
project(echo_server)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(echo_client main.cpp)

target_include_directories(echo_client PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(echo_client ${Boost_LIBRARIES})
