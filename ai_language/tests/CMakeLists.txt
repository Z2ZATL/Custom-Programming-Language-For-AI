include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)

add_executable(lexer_test lexer_test.cpp)
target_link_libraries(lexer_test PRIVATE 
    ai_language_lib
    gtest_main
)

add_executable(parser_test parser_test.cpp)
target_link_libraries(parser_test PRIVATE 
    ai_language_lib
    gtest_main
)

add_executable(interpreter_test interpreter_test.cpp)
target_link_libraries(interpreter_test PRIVATE 
    ai_language_lib
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(lexer_test)
gtest_discover_tests(parser_test)
gtest_discover_tests(interpreter_test)