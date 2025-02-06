cmake_minimum_required(VERSION 3.22)
project(ex)

set(CMAKE_CXX_STANDARD 20)
FILE(GLOB_RECURSE SRC "src/*.h" "src/*.cpp")
add_executable(MachineTransformer main.cpp ${SRC})

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

target_link_libraries(MachineTransformer PRIVATE nlohmann_json::nlohmann_json)
