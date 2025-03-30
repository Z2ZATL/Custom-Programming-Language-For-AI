
/**
 * @file ai_lang.cpp
 * @brief โปรแกรมหลักสำหรับแปลภาษา AI
 */

#include "include/interpreter.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

void showUsage(const std::string& programName) {
    std::cout << "วิธีใช้งาน: " << programName << " [ไฟล์.ai | -i]" << std::endl;
    std::cout << "  ไฟล์.ai      ไฟล์โค้ดภาษา AI ที่ต้องการประมวลผล" << std::endl;
    std::cout << "  -i, --interactive    เข้าสู่โหมดโต้ตอบ" << std::endl;
}

void runInteractiveMode() {
    std::cout << "=== โหมดโต้ตอบของภาษา AI ===" << std::endl;
    std::cout << "พิมพ์คำสั่งและกด Enter เพื่อดำเนินการ (พิมพ์ 'exit' เพื่อออก)" << std::endl << std::endl;

    ai_language::Interpreter interpreter;
    std::string line;
    std::string command;

    // ตั้งค่าฟังก์ชันสำหรับแสดงผลลัพธ์
    interpreter.setOutputHandler([](const std::string& message) {
        std::cout << message << std::endl;
    });
    
    // ตั้งค่าฟังก์ชันสำหรับแสดงข้อผิดพลาด
    interpreter.setErrorHandler([](const std::string& message) {
        std::cerr << "\033[31m" << message << "\033[0m" << std::endl;
    });

    // แสดงคำแนะนำสำหรับการใช้งาน
    std::cout << "ตัวอย่างคำสั่ง:\n";
    std::cout << "  start create ML\n";
    std::cout << "  load dataset \"data.csv\" type \"csv\"\n";
    std::cout << "  clean\n";
    std::cout << "  split\n";
    std::cout << "  train epochs 100\n\n";

    while (true) {
        std::cout << "AI> ";
        std::getline(std::cin, line);

        if (line == "exit" || line == "quit") {
            break;
        }

        // แก้ไขข้อผิดพลาดในการตรวจสอบคำสั่ง start create ML
        if (line.find("start create") == 0) {
            // แยกส่วนคำสั่ง
            std::istringstream iss(line);
            std::string cmd1, cmd2, type;
            iss >> cmd1 >> cmd2 >> type;
            
            if (cmd1 == "start" && cmd2 == "create" && 
                (type == "ML" || type == "DL" || type == "RL")) {
                std::cout << "เริ่มต้นโปรเจกต์ประเภท: " << type << std::endl;
                
                if (type == "ML") {
                    std::cout << "เริ่มต้นโปรเจกต์ Machine Learning" << std::endl;
                } else if (type == "DL") {
                    std::cout << "เริ่มต้นโปรเจกต์ Deep Learning" << std::endl;
                } else if (type == "RL") {
                    std::cout << "เริ่มต้นโปรเจกต์ Reinforcement Learning" << std::endl;
                }
            } else {
                std::cerr << "\033[31mรูปแบบคำสั่งไม่ถูกต้อง ตัวอย่าง: start create ML หรือ start create DL หรือ start create RL\033[0m" << std::endl;
            }
        } else {
            // ประมวลผลคำสั่งด้วย interpreter ปกติ
            interpreter.interpret(line);
        }
    }

    std::cout << "ออกจากโปรแกรม" << std::endl;
}

void runFile(const std::string& filename) {
    // ตรวจสอบว่าไฟล์มีอยู่จริง
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: ไม่สามารถเปิดไฟล์ " << filename << std::endl;
        return;
    }

    // อ่านเนื้อหาทั้งหมดของไฟล์
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    // สร้าง interpreter และตั้งค่าฟังก์ชันสำหรับแสดงผลลัพธ์และข้อผิดพลาด
    ai_language::Interpreter interpreter;
    
    interpreter.setOutputHandler([](const std::string& message) {
        std::cout << message << std::endl;
    });
    
    interpreter.setErrorHandler([](const std::string& message) {
        std::cerr << "\033[31m" << message << "\033[0m" << std::endl;
    });
    
    // ประมวลผลโค้ด
    interpreter.interpret(buffer.str());
}

int main(int argc, char* argv[]) {
    std::cout << "=== ทดสอบภาษาโปรแกรมสำหรับ AI ===" << std::endl << std::endl;
    
    // ถ้าไม่มีพารามิเตอร์ แสดงวิธีใช้
    if (argc < 2) {
        showUsage(argv[0]);
        return 0;
    }

    std::string arg = argv[1];

    // ตรวจสอบโหมดการทำงาน
    if (arg == "-i" || arg == "--interactive") {
        runInteractiveMode();
        return 0;
    } 
    
    // ถ้าไม่ใช่โหมดโต้ตอบ ให้พยายามเปิดไฟล์
    runFile(arg);
    return 0;
}
