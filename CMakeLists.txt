cmake_minimum_required(VERSION 3.10)
project(DataAnalyzer VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(BUILD_TESTS "Build the test suite" ON)
option(USE_EIGEN "Use Eigen library for linear algebra" ON)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Find required packages
find_package(Threads REQUIRED)
if(USE_EIGEN)
    find_package(Eigen3 QUIET)
    if(NOT EIGEN3_FOUND)
        message(STATUS "Eigen3 not found. Downloading...")
        include(FetchContent)
        FetchContent_Declare(
            eigen
            GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
            GIT_TAG 3.4.0
        )
        FetchContent_MakeAvailable(eigen)
        add_compile_definitions(USE_EIGEN)
    else()
        include_directories(${EIGEN3_INCLUDE_DIR})
        add_compile_definitions(USE_EIGEN)
    endif()
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Source files
set(SOURCES
    src/DataManager.cpp
    src/DataProcessor.cpp
    src/ErrorHandler.cpp
    src/Logger.cpp
    src/Model.cpp
    src/Utils.cpp
)

# Main executable
add_executable(data_analyzer src/main.cpp ${SOURCES})
target_link_libraries(data_analyzer PRIVATE Threads::Threads)

# Install rules
install(TARGETS data_analyzer DESTINATION bin)
install(DIRECTORY include/ DESTINATION include/data_analyzer)

# Testing
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Output configuration summary
message(STATUS "")
message(STATUS "Configuration summary:")
message(STATUS "  C++ standard     : ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build tests      : ${BUILD_TESTS}")
message(STATUS "  Use Eigen        : ${USE_EIGEN}")
message(STATUS "")
