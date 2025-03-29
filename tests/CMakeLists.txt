# Tests CMakeLists.txt
include(FetchContent)

# Download and configure Google Test
# Use the archive URL instead of git to avoid git-related issues
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.zip
)
FetchContent_MakeAvailable(googletest)

# Set variables for test targets
set(TEST_TARGETS
    test_data_manager
    test_data_processor
    test_model
    test_utils
)

# Add test executables
add_executable(test_data_manager TestDataManager.cpp)
add_executable(test_data_processor TestDataProcessor.cpp)
add_executable(test_model TestModel.cpp)
add_executable(test_utils TestUtils.cpp)

# Configure each test executable
foreach(target ${TEST_TARGETS})
    target_link_libraries(${target} 
        PRIVATE 
        gtest 
        gtest_main 
        Threads::Threads
        data_analyzer_lib
    )
    
    add_test(NAME ${target} COMMAND ${target})
    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests
    )
endforeach()

# Create a convenience target to run all tests
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose
    DEPENDS ${TEST_TARGETS}
)

# Create a static library for the test code to use
add_library(data_analyzer_lib STATIC
    ${CMAKE_SOURCE_DIR}/src/DataManager.cpp
    ${CMAKE_SOURCE_DIR}/src/DataProcessor.cpp
    ${CMAKE_SOURCE_DIR}/src/ErrorHandler.cpp
    ${CMAKE_SOURCE_DIR}/src/Logger.cpp
    ${CMAKE_SOURCE_DIR}/src/Model.cpp
    ${CMAKE_SOURCE_DIR}/src/Utils.cpp
)

target_include_directories(data_analyzer_lib PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(data_analyzer_lib PRIVATE Threads::Threads)

# Output summary
message(STATUS "Test configuration:")
message(STATUS "  Google Test version: 1.16.0")
message(STATUS "  Test targets: ${TEST_TARGETS}")
