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

void showHelp() {
    std::cout << "=== AI Language Help ===" << std::endl;
    std::cout << "Available commands:" << std::endl;
    std::cout << "  start                           # Start a new AI project" << std::endl;
    std::cout << "  create <type>                   # Create ML, DL, or RL project" << std::endl;
    std::cout << "  load dataset <path> [type]      # Load dataset from file" << std::endl;
    std::cout << "  create model <model_type>       # Create a specific model" << std::endl;
    std::cout << "  set <param> <value>             # Set parameter value" << std::endl;
    std::cout << "  train model                     # Train the model" << std::endl;
    std::cout << "  evaluate model                  # Evaluate model performance" << std::endl;
    std::cout << "  save model <path>               # Save model to file" << std::endl;
    std::cout << "  show <metric|info>              # Show metrics or model info" << std::endl;
    std::cout << "  predict <input>                 # Make predictions" << std::endl;
    std::cout << "  safe on                         # Enable safe mode (requires confirmation before executing commands)" << std::endl;
    std::cout << "  safe off                        # Disable safe mode" << std::endl;
    std::cout << "For more details, see docs/guides/USAGE_GUIDE.md" << std::endl;
}


void runInteractiveMode() {
    std::cout << "DEBUG: Starting interactive mode" << std::endl << std::flush;
    std::cout << "\n\n=== โหมดโต้ตอบของภาษา AI ===" << std::endl << std::flush;
    std::cout << "พิมพ์คำสั่งและกด Enter เพื่อดำเนินการ (พิมพ์ 'exit' เพื่อออก)" << std::endl << std::flush;
    std::cout << "รองรับคำสั่งหลายบรรทัด - พิมพ์ '\\' แล้วกด Enter เพื่อพิมพ์ต่อในบรรทัดถัดไป" << std::endl;
    std::cout << "พิมพ์ ';;' เพื่อดำเนินการทั้งหมด" << std::endl;
    std::cout << "คำสั่งพิเศษ: safe on เพื่อเปิดโหมดความปลอดภัย (ต้องยืนยันก่อนทำงานคำสั่ง), safe off เพื่อปิด" << std::endl;

    // แสดงคำแนะนำหรือตัวอย่างคำสั่ง
    std::cout << CYAN << "ตัวอย่างคำสั่ง:" << RESET << std::endl;
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

    std::cout << CYAN << "ตัวอย่างคำสั่งหลายบรรทัด:" << RESET << std::endl;
    std::cout << "  start \\" << std::endl;
    std::cout << "  create ML \\" << std::endl;
    std::cout << "  load dataset \"datasets/linear_data.csv\" \\" << std::endl;
    std::cout << "  create model LinearRegression ;;" << std::endl << std::endl;


    std::cout << std::endl;

    // Create a pointer for interpreter but don't initialize it yet
    std::unique_ptr<BaseInterpreter> interpreter = nullptr;
    std::string currentType = "";

    std::string multiline = "";
    std::string line;

    // รับคำสั่งจากผู้ใช้จนกว่าจะพิมพ์ 'exit'
    while (true) {
        try {
            if (multiline.empty()) {
                std::cout << GREEN << "AI> " << RESET << std::flush;
            } else {
                std::cout << YELLOW << "... " << RESET << std::flush;
            }

            if (!std::getline(std::cin, line)) {
                std::cout << RED << "ERROR: Input stream closed" << RESET << std::endl << std::flush;
                break;
            }

            if (line.empty()) {
                // กรณีมีการกด Enter โดยไม่มีข้อความ ให้ทำการเริ่มการรับข้อมูลใหม่
                continue;
            }

            std::cout << "DEBUG: Got input: '" << line << "'" << std::endl << std::flush;

            if ((line == "exit" || line == "quit") && multiline.empty()) {
                std::cout << "ออกจากโปรแกรม" << std::endl;
                break;
            }

            // ตรวจสอบคำสั่งพิเศษที่ต้องประมวลผลทันที
            if (multiline.empty()) {
                if (line == "list models" || line == "list") {
                    std::cout << "Available models:" << std::endl;
                    std::cout << "  ML models: LinearRegression, LogisticRegression, RandomForest, SVM, KNN" << std::endl;
                    std::cout << "  DL models: NeuralNetwork, CNN, RNN, LSTM, GRU, Transformer" << std::endl;
                    std::cout << "  RL models: QLearning, DQN, PPO, A2C, DDQN" << std::endl;
                    continue;
                } else if (line == "show version") {
                    std::cout << "AI Language Version: 1.0.0" << std::endl;
                    std::cout << "Build Date: " << __DATE__ << " " << __TIME__ << std::endl;
                    continue;
                } else if (line == "show help" || line == "help") {
                    showHelp();
                    continue;
                } else if (line == "show time" || line == "time") {
                    std::time_t now = std::time(nullptr);
                    std::cout << "Current time: " << std::ctime(&now);
                    continue;
                } else if (line == "exit" || line == "quit") {
                    std::cout << "ออกจากโปรแกรม" << std::endl;
                    break;
                } else if (line == "clear" || line == "cls") {
                    std::cout << "\033[2J\033[1;1H"; // คำสั่งล้างหน้าจอใน terminal
                    continue;
                } else if (line == "safe on") {
                    if (interpreter) interpreter->setSafeMode(true);
                    else std::cout << YELLOW << "Please create an interpreter first." << RESET << std::endl;
                    continue;
                } else if (line == "safe off") {
                    if (interpreter) interpreter->setSafeMode(false);
                    else std::cout << YELLOW << "Please create an interpreter first." << RESET << std::endl;
                    continue;
                }
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
                std::cout << "DEBUG: Switching to ML interpreter" << std::endl << std::flush;
                try {
                    auto newInterpreter = InterpreterFactory::createInterpreter("ML");
                    if (newInterpreter) {
                        interpreter = std::move(newInterpreter);
                        currentType = "ML";
                        std::cout << "สลับมาใช้ Machine Learning interpreter" << std::endl;
                    } else {
                        std::cerr << RED << "ERROR: Failed to create ML interpreter (nullptr returned)" << RESET << std::endl << std::flush;
                    }
                } catch (const std::exception& e) {
                    std::cerr << RED << "ERROR: Failed to create ML interpreter: " << e.what() << RESET << std::endl << std::flush;
                }
            } else if (multiline.find("create DL") != std::string::npos && currentType != "DL") {
                std::cout << "DEBUG: Switching to DL interpreter" << std::endl << std::flush;
                try {
                    auto newInterpreter = InterpreterFactory::createInterpreter("DL");
                    if (newInterpreter) {
                        interpreter = std::move(newInterpreter);
                        currentType = "DL";
                        std::cout << "สลับมาใช้ Deep Learning interpreter" << std::endl;
                    } else {
                        std::cerr << RED << "ERROR: Failed to create DL interpreter (nullptr returned)" << RESET << std::endl << std::flush;
                    }
                } catch (const std::exception& e) {
                    std::cerr << RED << "ERROR: Failed to create DL interpreter: " << e.what() << RESET << std::endl << std::flush;
                }
            } else if (multiline.find("create RL") != std::string::npos && currentType != "RL") {
                std::cout << "DEBUG: Switching to RL interpreter" << std::endl << std::flush;
                try {
                    auto newInterpreter = InterpreterFactory::createInterpreter("RL");
                    if (newInterpreter) {
                        interpreter = std::move(newInterpreter);
                        currentType = "RL";
                        std::cout << "สลับมาใช้ Reinforcement Learning interpreter" << std::endl;
                    } else {
                        std::cerr << RED << "ERROR: Failed to create RL interpreter (nullptr returned)" << RESET << std::endl << std::flush;
                    }
                } catch (const std::exception& e) {
                    std::cerr << RED << "ERROR: Failed to create RL interpreter: " << e.what() << RESET << std::endl << std::flush;
                }
            }

            // ตรวจสอบว่าเป็นการสิ้นสุดคำสั่งหลายบรรทัดหรือไม่
            if (multiline.find(";;") != std::string::npos) {
                std::cout << "DEBUG: Processing multi-line command" << std::endl << std::flush;
                std::cout << CYAN << "กำลังประมวลผลคำสั่งหลายบรรทัด..." << RESET << std::endl;

                // ลบเครื่องหมาย ;; ออก
                size_t pos = multiline.find(";;");
                if (pos != std::string::npos) {
                    multiline.erase(pos, 2);
                }

                // แยกคำสั่งและประมวลผลทีละคำสั่ง
                std::istringstream stream(multiline);
                std::string command;
                int commandCount = 0;

                while (std::getline(stream, command)) {
                    if (!command.empty() && command.front() != '#') {
                        commandCount++;
                        std::cout << "DEBUG: Interpreting command: '" << command << "'" << std::endl << std::flush;
                        std::cout << YELLOW << "(" << commandCount << ") " << RESET << command << std::endl;
                        // ตรวจสอบคำสั่ง exit ให้ดีขึ้น
                        std::string trimmedCmd = command;
                        // ตัดช่องว่างที่อาจมีอยู่
                        trimmedCmd.erase(0, trimmedCmd.find_first_not_of(" \t\n\r\f\v"));
                        trimmedCmd.erase(trimmedCmd.find_last_not_of(" \t\n\r\f\v") + 1);

                        if (trimmedCmd == "exit" || trimmedCmd == "quit") {
                            std::cout << "ออกจากโปรแกรม" << std::endl;
                            break;
                        }
                        if (interpreter && interpreter->getSafeMode()) {
                            std::string confirmation;
                            std::cout << "ยืนยันการทำงานคำสั่ง (y/n): ";
                            std::cin >> confirmation;
                            if (confirmation != "y") {
                                std::cout << "คำสั่งถูกยกเลิก" << std::endl;
                                continue;
                            }
                        }
                        if (interpreter) interpreter->interpretLine(command);
                        std::cout << std::endl; // เพิ่มบรรทัดว่างระหว่างผลลัพธ์ของแต่ละคำสั่ง
                    }
                }

                std::cout << GREEN << "การประมวลผลคำสั่งหลายบรรทัดเสร็จสิ้น" << RESET << std::endl;
                multiline = "";
            } else {
                // ถ้าไม่มีเครื่องหมาย ;; ในบรรทัดเดียว ให้ประมวลผลทันที
                if (!multiline.empty() && multiline.front() != '#') {
                    std::cout << "DEBUG: Interpreting single command: '" << multiline << "'" << std::endl << std::flush;
                    // ตรวจสอบคำสั่ง exit ให้ดีขึ้น
                    std::string trimmedCmd = multiline;
                    // ตัดช่องว่างที่อาจมีอยู่
                    trimmedCmd.erase(0, trimmedCmd.find_first_not_of(" \t\n\r\f\v"));
                    trimmedCmd.erase(trimmedCmd.find_last_not_of(" \t\n\r\f\v") + 1);

                    if (trimmedCmd == "exit" || trimmedCmd == "quit") {
                        std::cout << "ออกจากโปรแกรม" << std::endl;
                        break;
                    }
                    if (interpreter && interpreter->getSafeMode()) {
                        std::string confirmation;
                        std::cout << "ยืนยันการทำงานคำสั่ง (y/n): ";
                        std::cin >> confirmation;
                        if (confirmation != "y") {
                            std::cout << "คำสั่งถูกยกเลิก" << std::endl;
                            continue;
                        }
                    }
                    if (interpreter) interpreter->interpretLine(multiline);
                }
                multiline = "";
            }
        } catch (const std::exception& e) {
            std::cerr << RED << "ERROR in interactive loop: " << e.what() << RESET << std::endl << std::flush;
            multiline = ""; // รีเซ็ตคำสั่งหลายบรรทัดเพื่อป้องกันปัญหา
        } catch (...) {
            std::cerr << RED << "UNKNOWN ERROR in interactive loop" << RESET << std::endl << std::flush;
            multiline = ""; // รีเซ็ตคำสั่งหลายบรรทัดเพื่อป้องกันปัญหา
        }
    }
}

} // end namespace ai_language

int main(int argc, char* argv[]) {
    std::cout << "DEBUG: Starting ai_lang with " << argc << " arguments" << std::endl << std::flush;

    // ตรวจสอบ arguments
    if (argc == 1) {
        std::cout << "No arguments provided, entering interactive mode..." << std::endl << std::flush;
        ai_language::runInteractiveMode();
        return 0;
    }

    std::string arg = argv[1];
    std::cout << "DEBUG: First argument is " << arg << std::endl << std::flush;

    if (arg == "-h" || arg == "--help") {
        ai_language::printUsage();
        return 0;
    } else if (arg == "-i" || arg == "--interactive") {
        std::cout << "DEBUG: About to enter interactive mode" << std::endl << std::flush;
        try {
            ai_language::runInteractiveMode();
            std::cout << "DEBUG: Interactive mode completed normally" << std::endl << std::flush;
        } catch (const std::exception& e) {
            std::cerr << "ERROR in interactive mode: " << e.what() << std::endl << std::flush;
            return 1;
        } catch (...) {
            std::cerr << "UNKNOWN ERROR in interactive mode" << std::endl << std::flush;
            return 1;
        }
        return 0;
    } else {
        // ประมวลผลไฟล์
        try {
            // สร้าง interpreter ตามประเภทที่พบในไฟล์
            std::unique_ptr<ai_language::BaseInterpreter> interpreter = ai_language::InterpreterFactory::createInterpreterFromFile(arg);
            interpreter->interpretFile(arg);
        } catch (const std::exception& e) {
            std::cerr << "ERROR processing file: " << e.what() << std::endl << std::flush;
            return 1;
        } catch (...) {
            std::cerr << "UNKNOWN ERROR processing file" << std::endl << std::flush;
            return 1;
        }
    }

    return 0;
}