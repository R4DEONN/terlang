FILE(GLOB_RECURSE SRC "*.h" "*.cpp")
add_library(lexer ${SRC})
target_include_directories(lexer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})