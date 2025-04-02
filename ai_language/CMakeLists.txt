cmake_minimum_required(VERSION 3.10)
project(ai_language)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# เพิ่มไฟล์ source ทั้งหมด
set(SOURCES
    src/interpreters/BaseInterpreter.cpp
    src/interpreters/MLInterpreter.cpp
    src/interpreters/DLInterpreter.cpp
    src/interpreters/RLInterpreter.cpp
    src/interpreters/InterpreterFactory.cpp
    src/connectors/ScikitLearnConnector.cpp
    src/interpreter.cpp
    src/lexer.cpp
    src/parser.cpp
)

# สร้าง library
add_library(ai_language_lib STATIC ${SOURCES})

# เพิ่ม utils
add_executable(show_time src/utils/show_time.cpp)
target_include_directories(ai_language_lib PUBLIC include)

# สร้าง executable
add_executable(ai_lang src/ai_lang.cpp)
target_link_libraries(ai_lang PRIVATE ai_language_lib)

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