cmake_minimum_required(VERSION 3.10)
project(ai_language VERSION 0.1.0 LANGUAGES CXX)

# กำหนดมาตรฐาน C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ไฟล์ source สำหรับ library
set(LIB_SOURCES
    src/lexer.cpp
    src/parser.cpp
    src/interpreter.cpp
)

# ไฟล์ source สำหรับ executable
set(EXEC_SOURCES
    ai_lang.cpp
)

# สร้าง library
add_library(ai_language_lib STATIC ${LIB_SOURCES})
target_include_directories(ai_language_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# สร้าง executable
add_executable(ai_lang ${EXEC_SOURCES})
target_link_libraries(ai_lang PRIVATE ai_language_lib)

# กำหนด output directory
set_target_properties(ai_lang PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

# ตัวเลือกสำหรับ build ทดสอบ
option(BUILD_TESTS "Build test programs" OFF)

# เปิดการทดสอบตามตัวเลือก
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# ตัวเลือกการ build
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# สร้างเป้าหมายสำหรับรัน
add_custom_target(run
    COMMAND ai_lang
    DEPENDS ai_lang
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
cmake_minimum_required(VERSION 3.10)
project(ai_language)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Library source files
set(LIB_SOURCES
    src/lexer.cpp
    src/parser.cpp
    src/interpreter.cpp
)

# Create library
add_library(ai_language_lib STATIC ${LIB_SOURCES})

# Main executable
add_executable(ai_lang ai_lang.cpp)
target_link_libraries(ai_lang ai_language_lib)

# Enable testing
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
