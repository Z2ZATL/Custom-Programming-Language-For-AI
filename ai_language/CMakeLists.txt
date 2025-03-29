cmake_minimum_required(VERSION 3.10)
project(AILanguage VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ตัวเลือกสำหรับ build ทดสอบ
option(BUILD_TESTS "Build test programs" OFF)

# ตั้งชื่อสำหรับไฟล์ output
set(AI_LANG_BINARY ai_lang)

# สร้าง library
add_library(ai_language_lib
    src/lexer.cpp
    src/parser.cpp
    src/interpreter.cpp
)

target_include_directories(ai_language_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# สร้าง executable
add_executable(${AI_LANG_BINARY} ai_lang.cpp)

target_link_libraries(${AI_LANG_BINARY} PRIVATE
    ai_language_lib
)

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
    COMMAND ${AI_LANG_BINARY}
    DEPENDS ${AI_LANG_BINARY}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)