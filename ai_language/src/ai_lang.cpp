#include "../include/interpreters/InterpreterFactory.h"
#include "../include/interpreters/BaseInterpreter.h"
#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <iomanip>
#include <thread>
#include <chrono>
#include <sys/stat.h> // for stat

// The color constants are already defined in BaseInterpreter.h

namespace ai_language {


void printUsage() {
    std::cout << "วิธีใช้: ai_lang [options] [filename]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -i, --interactive    เข้าสู่โหมดโต้ตอบ" << std::endl;
    std::cout << "  -h, --help          แสดงวิธีใช้" << std::endl;
}

void runInteractiveMode() {
    std::cout << "DEBUG: Starting interactive mode" << std::endl << std::flush;
    std::cout << "=== โหมดโต้ตอบของภาษา AI ===" << std::endl << std::flush;
    std::cout << "พิมพ์คำสั่งและกด Enter เพื่อดำเนินการ (พิมพ์ 'exit' เพื่อออก)" << std::endl << std::flush;
    std::cout << "รองรับคำสั่งหลายบรรทัด - พิมพ์ '\\' แล้วกด Enter เพื่อพิมพ์ต่อในบรรทัดถัดไป" << std::endl << std::flush;
    std::cout << "พิมพ์ ';;' เพื่อดำเนินการทั้งหมด" << std::endl << std::endl << std::flush;

    // แสดงคำแนะนำหรือตัวอย่างคำสั่ง
    std::cout << "ตัวอย่างคำสั่ง:" << std::endl;
    std::cout << "  start" << std::endl;
    std::cout << "  create ML                        # สร้างโปรเจค Machine Learning" << std::endl;
    std::cout << "  create DL                        # สร้างโปรเจค Deep Learning" << std::endl;
    std::cout << "  create RL                        # สร้างโปรเจค Reinforcement Learning" << std::endl;
    std::cout << "  load dataset \"datasets/linear_data.csv\" type \"csv\"  # โหลดข้อมูลสำหรับ ML" << std::endl;
    std::cout << "  load dataset \"datasets/iris.csv\" type \"csv\"         # โหลดข้อมูลสำหรับ DL" << std::endl;
    std::cout << "  load environment \"datasets/environment.json\"         # โหลดสภาพแวดล้อมสำหรับ RL" << std::endl;
    std::cout << "  create model LinearRegression    # สร้างโมเดล LinearRegression" << std::endl;
    std::cout << "  create model NeuralNetwork       # สร้างโมเดล Neural Network" << std::endl;
    std::cout << "  create model QLearning           # สร้างโมเดล Q-Learning" << std::endl;
    std::cout << "  set learning_rate 0.01           # ตั้งค่า learning rate" << std::endl;
    std::cout << "  set epochs 100                   # ตั้งค่าจำนวนรอบการเรียนรู้" << std::endl;
    std::cout << "  train model                      # ฝึกโมเดล" << std::endl;
    std::cout << "  evaluate model                   # ประเมินประสิทธิภาพโมเดล" << std::endl;
    std::cout << "  show accuracy                    # แสดงความแม่นยำของโมเดล" << std::endl;
    std::cout << "  save model \"model_name.dat\"      # บันทึกโมเดล" << std::endl << std::endl;

    std::cout << "ตัวอย่างคำสั่งหลายบรรทัด:" << std::endl;
    std::cout << "  start \\" << std::endl;
    std::cout << "  create ML \\" << std::endl;
    std::cout << "  load dataset \"datasets/linear_data.csv\" \\" << std::endl;
    std::cout << "  create model LinearRegression ;;" << std::endl << std::endl;

    std::cout << "คำแนะนำเพิ่มเติม: ดูไฟล์ docs/guides/USAGE_GUIDE.md สำหรับรายละเอียดเพิ่มเติม" << std::endl;
    std::cout << "หรือดูตัวอย่างไฟล์ในโฟลเดอร์ examples/" << std::endl << std::endl;

    std::cout << "ข้อมูลตัวอย่างอยู่ในโฟลเดอร์ datasets/" << std::endl;
    std::cout << "  - linear_data.csv      # สำหรับ Linear Regression" << std::endl;
    std::cout << "  - classification_data.csv # สำหรับ Classification" << std::endl;
    std::cout << "  - iris.csv             # สำหรับ Neural Network" << std::endl;
    std::cout << "  - images/              # สำหรับ CNN" << std::endl;
    std::cout << "  - environment.json     # สำหรับ Reinforcement Learning" << std::endl;
    std::cout << "  - config.json          # สำหรับการตั้งค่า RL" << std::endl << std::endl;

    // เริ่มต้นด้วย MLInterpreter เป็นค่าเริ่มต้น
    std::unique_ptr<BaseInterpreter> interpreter = InterpreterFactory::createInterpreter("ML");
    std::string multiline = "";
    std::string line;
    std::string currentType = "ML";

    // รับคำสั่งจากผู้ใช้จนกว่าจะพิมพ์ 'exit'
    while (true) {
        if (multiline.empty()) {
            std::cout << "AI> " << std::flush;
        } else {
            std::cout << "... " << std::flush;
        }

        std::getline(std::cin, line);

        if (line == "exit" && multiline.empty()) {
            break;
        }

        // ตรวจสอบการป้อนหลายบรรทัด
        if (!line.empty() && line.back() == '\\') {
            // ลบเครื่องหมาย \ และเก็บคำสั่งไว้
            line.pop_back();
            multiline += line + "\n";
            continue;
        }

        // เพิ่มบรรทัดปัจจุบันเข้าไปในคำสั่งหลายบรรทัด
        multiline += line;

        // ตรวจสอบว่ามีการเปลี่ยนประเภท AI หรือไม่
        if (multiline.find("create ML") != std::string::npos && currentType != "ML") {
            interpreter = InterpreterFactory::createInterpreter("ML");
            currentType = "ML";
        } else if (multiline.find("create DL") != std::string::npos && currentType != "DL") {
            interpreter = InterpreterFactory::createInterpreter("DL");
            currentType = "DL";
        } else if (multiline.find("create RL") != std::string::npos && currentType != "RL") {
            interpreter = InterpreterFactory::createInterpreter("RL");
            currentType = "RL";
        }

        // ตรวจสอบคำสั่งพิเศษ
        if (line == "list models" || line == "list") {
            interpreter->listModels();
            continue;
        } else if (line == "show version") {
            interpreter->showVersion();
            continue;
        } else if (line == "show help" || line == "help") {
            interpreter->showHelp();
            continue;
        } else if (line == "show time" || line == "time") {
            interpreter->showTime();
            continue;
        } else if (line == "exit" || line == "quit") {
            std::cout << "ออกจากโปรแกรม" << std::endl;
            break;
        } else if (line == "clear" || line == "cls") {
            std::cout << "\033[2J\033[1;1H"; // คำสั่งล้างหน้าจอใน terminal
            continue;
        }
        
        // ตรวจสอบว่าเป็นการสิ้นสุดคำสั่งหลายบรรทัดหรือไม่
        if (multiline.find(";;") != std::string::npos || line.find(";;") != std::string::npos) {
            // ลบเครื่องหมาย ;; ออก
            size_t pos = multiline.find(";;");
            if (pos != std::string::npos) {
                multiline.erase(pos, 2);
            }

            // แยกคำสั่งและประมวลผลทีละคำสั่ง
            std::istringstream stream(multiline);
            std::string command;

            while (std::getline(stream, command)) {
                if (!command.empty() && command.front() != '#') {
                    interpreter->interpretLine(command);
                }
            }

            multiline = "";
        } else if (multiline.find(";;") == std::string::npos && line.find(";;") == std::string::npos) {
            // ถ้าไม่มีเครื่องหมาย ;; ในบรรทัดเดียว ให้ประมวลผลทันที
            if (!multiline.empty() && multiline.front() != '#') {
                interpreter->interpretLine(multiline);
            }
            multiline = "";
        }
    }
}

} // end namespace ai_language

int main(int argc, char* argv[]) {
    std::cout << "DEBUG: Starting ai_lang with " << argc << " arguments" << std::endl;
    
    // ตรวจสอบ arguments
    if (argc == 1) {
        ai_language::printUsage();
        return 0;
    }

    std::string arg = argv[1];
    std::cout << "DEBUG: First argument is " << arg << std::endl;
    
    if (arg == "-h" || arg == "--help") {
        ai_language::printUsage();
        return 0;
    } else if (arg == "-i" || arg == "--interactive") {
        std::cout << "DEBUG: About to enter interactive mode" << std::endl << std::flush;
        ai_language::runInteractiveMode();
        std::cout << "DEBUG: Interactive mode completed" << std::endl;
        return 0;
    } else {
        // ประมวลผลไฟล์
        // สร้าง interpreter ตามประเภทที่พบในไฟล์
        std::unique_ptr<ai_language::BaseInterpreter> interpreter = ai_language::InterpreterFactory::createInterpreterFromFile(arg);
        interpreter->interpretFile(arg);
    }

    return 0;
}