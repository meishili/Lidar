cmake_minimum_required(VERSION 3.10)

project(thread)

add_executable(thread  lidar.cpp main.cpp mean.cpp distance.cpp)

set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

target_link_libraries(thread pthread)