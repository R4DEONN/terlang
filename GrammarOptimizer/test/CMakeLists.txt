FILE(GLOB_RECURSE SRC "*.cpp")
add_executable(tests ${SRC})
find_package(Catch2 REQUIRED)
target_link_libraries(tests PRIVATE grammar_optimizer Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(tests)

add_custom_target(run_tests
        COMMAND tests
        DEPENDS tests
)