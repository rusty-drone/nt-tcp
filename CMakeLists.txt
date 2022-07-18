cmake_minimum_required(VERSION 3.10)

# set the project name
project(nt)

set(CMAKE_CXX_STANDARD 11)

# add the executable
add_executable(nt grapher.h grapher.cpp tcp.cpp)

target_link_libraries(nt wiringPi)
