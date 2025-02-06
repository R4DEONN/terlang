cmake_minimum_required(VERSION 3.22)
project(LexerApp)

enable_testing()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/bin")

add_subdirectory(src)

add_executable(LexerApp main.cpp)
target_link_libraries(LexerApp PRIVATE lexer)

#add_subdirectory(test)

#add_dependencies(LexerApp run_tests)