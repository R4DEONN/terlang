cmake_minimum_required(VERSION 3.22)
project(ex)

set(CMAKE_CXX_STANDARD 20)
FILE(GLOB_RECURSE SRC "src/*.h" "src/*.cpp")
add_executable(MachineTransformer main.cpp ${SRC})