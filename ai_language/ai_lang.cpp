/**
 * @file ai_lang.cpp
 * @brief Main program for AI Language interpreter
 */

#include "include/interpreter.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

void showUsage(const std::string& programName) {
    std::cout << "=== ทดสอบภาษาโปรแกรมสำหรับ AI ===" << std::endl << std::endl;
    std::cout << "โปรดระบุไฟล์ .ai เพื่อรัน (เช่น " << programName << " example.ai)" << std::endl;
    std::cout << "หรือใช้ -i เพื่อเข้าสู่โหมดโต้ตอบ (เช่น " << programName << " -i)" << std::endl;
}

void runInteractiveMode() {
    std::cout << "=== โหมดโต้ตอบของภาษา AI ===" << std::endl;
    std::cout << "พิมพ์คำสั่งและกด Enter เพื่อดำเนินการ (พิมพ์ 'exit' เพื่อออก)" << std::endl << std::endl;

    ai_language::Interpreter interpreter;
    std::string line;

    while (true) {
        std::cout << "AI> ";
        std::getline(std::cin, line);

        if (line == "exit" || line == "quit") {
            break;
        }

        interpreter.interpret(line);
    }

    std::cout << "ออกจากโปรแกรม" << std::endl;
}

void runFile(const std::string& filename) {
    // ตรวจสอบว่าไฟล์มีอยู่จริง
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ไม่สามารถเปิดไฟล์: " << filename << std::endl;
        return;
    }

    // อ่านเนื้อหาทั้งหมดของไฟล์
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    // สร้าง interpreter และประมวลผลโค้ด
    ai_language::Interpreter interpreter;
    std::cout << "=== ทดสอบภาษาโปรแกรมสำหรับ AI ===" << std::endl << std::endl;
    interpreter.interpret(buffer.str());
}

int main(int argc, char* argv[]) {
    // ถ้าไม่มีพารามิเตอร์ แสดงวิธีใช้
    if (argc < 2) {
        showUsage(argv[0]);
        return 0;
    }

    std::string arg = argv[1];

    // ตรวจสอบโหมดการทำงาน
    if (arg == "-i" || arg == "--interactive") {
        runInteractiveMode();
    } else {
        // สันนิษฐานว่าเป็นชื่อไฟล์
        runFile(arg);
    }

    return 0;
}