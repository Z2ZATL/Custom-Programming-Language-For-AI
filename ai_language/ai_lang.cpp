/**
 * @file ai_lang.cpp
 * @brief โปรแกรมหลักสำหรับแปลภาษา AI
 */

#include "include/interpreter.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <unistd.h>  // สำหรับ getcwd
#include <cstring>   // สำหรับ strncpy
#include <algorithm> // for std::transform

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
    std::cout << "ตัวอย่างคำสั่งตามลำดับการทำงาน:\n";
    std::cout << "  1. start\n";
    std::cout << "  2. create ML\n";
    std::cout << "  3. load dataset \"data.csv\"\n";
    std::cout << "  4. create model LinearRegression\n";
    std::cout << "  5. train model\n";
    std::cout << "  6. show accuracy\n\n";

    // ตัวแปรเก็บสถานะของโปรแกรม ต้องทำตามลำดับ
    bool hasStarted = false;
    bool hasCreatedProject = false;
    bool hasLoadedData = false;
    bool hasCreatedModel = false;
    bool hasTrainedModel = false;
    std::string projectType = "";
    std::string modelName = "";

    while (true) {
        std::cout << "AI> ";
        std::getline(std::cin, line);

        if (line == "exit" || line == "quit") {
            break;
        }

        // ตรวจสอบคำสั่งและความถูกต้องตามลำดับขั้นตอน
        if (line == "start") {
            if (hasStarted) {
                std::cerr << "\033[31mError: คำสั่ง 'start' ถูกใช้ไปแล้ว\033[0m" << std::endl;
                continue;
            }
            
            hasStarted = true;
            std::cout << "Program started" << std::endl;
        } 
        else if (line.find("create ") == 0) {
            // คำสั่ง create project หรือ create model
            std::istringstream iss(line);
            std::string cmd, type;
            iss >> cmd >> type;
            
            if (!hasStarted) {
                std::cerr << "\033[31mError: Must start with 'start' command\033[0m" << std::endl;
                continue;
            }
            
            if (type == "ML" || type == "DL" || type == "RL") {
                // create project type
                if (hasCreatedProject) {
                    std::cerr << "\033[31mError: Project type already defined\033[0m" << std::endl;
                    continue;
                }
                
                if (hasLoadedData || hasCreatedModel || hasTrainedModel) {
                    std::cerr << "\033[31mError: Cannot change project type after other operations\033[0m" << std::endl;
                    continue;
                }
                
                projectType = type;
                hasCreatedProject = true;
                
                if (type == "ML") {
                    std::cout << "Project created: Machine Learning" << std::endl;
                } else if (type == "DL") {
                    std::cout << "Project created: Deep Learning" << std::endl;
                } else if (type == "RL") {
                    std::cout << "Project created: Reinforcement Learning" << std::endl;
                }
            } 
            else if (type == "model") {
                // create model
                if (!hasCreatedProject) {
                    std::cerr << "\033[31mError: Must create project type first\033[0m" << std::endl;
                    continue;
                }
                
                if (!hasLoadedData) {
                    std::cerr << "\033[31mError: Must load dataset first\033[0m" << std::endl;
                    continue;
                }
                
                if (hasCreatedModel) {
                    std::cerr << "\033[31mError: Model already created\033[0m" << std::endl;
                    continue;
                }
                
                // รับชื่อโมเดล
                std::string model;
                iss >> model;
                
                if (model.empty()) {
                    std::cerr << "\033[31mError: Model name required\033[0m" << std::endl;
                    continue;
                }
                
                // ตรวจสอบความเข้ากันได้ของโมเดลกับประเภทโปรเจกต์
                bool validModel = false;
                if (projectType == "ML" && 
                    (model == "LinearRegression" || model == "LogisticRegression" || 
                     model == "RandomForest" || model == "SVM" || model == "KNN")) {
                    validModel = true;
                } else if (projectType == "DL" && 
                           (model == "NeuralNetwork" || model == "CNN" || model == "RNN")) {
                    validModel = true;
                } else if (projectType == "RL" && 
                           (model == "QLearning" || model == "DQN")) {
                    validModel = true;
                }
                
                if (!validModel) {
                    std::cerr << "\033[31mError: Model not compatible with project type\033[0m" << std::endl;
                    continue;
                }
                
                modelName = model;
                hasCreatedModel = true;
                std::cout << "Model created: " << model << std::endl;
            } 
            else {
                std::cerr << "\033[31mError: Invalid create command. Use 'create ML/DL/RL' or 'create model <model_name>'\033[0m" << std::endl;
            }
        } 
        else if (line.find("load dataset") == 0) {
            if (!hasStarted) {
                std::cerr << "\033[31mError: Must start with 'start' command\033[0m" << std::endl;
                continue;
            }
            
            if (!hasCreatedProject) {
                std::cerr << "\033[31mError: Must create project type first\033[0m" << std::endl;
                continue;
            }
            
            if (hasLoadedData) {
                std::cerr << "\033[31mError: Dataset already loaded\033[0m" << std::endl;
                continue;
            }
            
            // ตรวจจับคำสั่ง load dataset "filename"
            size_t filenameStart = line.find("\"");
            
            if (filenameStart != std::string::npos) {
                size_t filenameEnd = line.find("\"", filenameStart + 1);
                
                if (filenameEnd != std::string::npos) {
                    std::string filename = line.substr(filenameStart + 1, filenameEnd - filenameStart - 1);
                    
                    // ตรวจสอบนามสกุลไฟล์ว่าเป็น .csv หรือไม่
                    if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".csv") {
                        std::cerr << "\033[31mError: File must be a CSV\033[0m" << std::endl;
                        continue;
                    }
                    
                    // ตรวจสอบว่าไฟล์มีอยู่จริงหรือไม่
                    std::ifstream f(filename);
                    if (!f.good()) {
                        std::cerr << "\033[31mError: File not found\033[0m" << std::endl;
                        continue;
                    }
                    
                    std::cout << "กำลังโหลดข้อมูลจากไฟล์: " << filename << std::endl;
                    std::cout << "Dataset loaded successfully" << std::endl;
                    hasLoadedData = true;
                } else {
                    std::cerr << "\033[31mError: Invalid dataset filename format\033[0m" << std::endl;
                }
            } else {
                std::cerr << "\033[31mError: Missing dataset filename\033[0m" << std::endl;
            }
        } 
        else if (line == "train model") {
            if (!hasStarted) {
                std::cerr << "\033[31mError: Must start with 'start' command\033[0m" << std::endl;
                continue;
            }
            
            if (!hasCreatedProject) {
                std::cerr << "\033[31mError: Must create project type first\033[0m" << std::endl;
                continue;
            }
            
            if (!hasLoadedData) {
                std::cerr << "\033[31mError: Must load dataset first\033[0m" << std::endl;
                continue;
            }
            
            if (!hasCreatedModel) {
                std::cerr << "\033[31mError: Must create model first\033[0m" << std::endl;
                continue;
            }
            
            if (hasTrainedModel) {
                std::cerr << "\033[31mError: Model already trained\033[0m" << std::endl;
                continue;
            }
            
            std::cout << "Training model: " << modelName << "..." << std::endl;
            std::cout << "Training complete" << std::endl;
            hasTrainedModel = true;
        } 
        else if (line == "show accuracy") {
            if (!hasStarted) {
                std::cerr << "\033[31mError: Must start with 'start' command\033[0m" << std::endl;
                continue;
            }
            
            if (!hasCreatedProject) {
                std::cerr << "\033[31mError: Must create project type first\033[0m" << std::endl;
                continue;
            }
            
            if (!hasLoadedData) {
                std::cerr << "\033[31mError: Must load dataset first\033[0m" << std::endl;
                continue;
            }
            
            if (!hasCreatedModel) {
                std::cerr << "\033[31mError: Must create model first\033[0m" << std::endl;
                continue;
            }
            
            if (!hasTrainedModel) {
                std::cerr << "\033[31mError: Model not trained yet\033[0m" << std::endl;
                continue;
            }
            
            // สร้างความแม่นยำสุ่ม
            double accuracy = 80.0 + (std::rand() % 15) + (std::rand() % 100) / 100.0;
            std::cout << "Accuracy: " << accuracy << "%" << std::endl;
        } 
        else {
            // คำสั่งอื่นๆ
            if (line == "exit") {
                std::cout << "ออกจากโปรแกรม" << std::endl;
                break;
            } 
            else if (!line.empty()) {
                std::cerr << "\033[31mError: Unknown command: " << line << "\033[0m" << std::endl;
            }
                    } else if (lowerCmd.find("split") != std::string::npos) {
                        if (!hasCleanedData) {
                            std::cerr << "\033[33mคำเตือน: คุณควรทำความสะอาดข้อมูลด้วยคำสั่ง 'clean' ก่อน\033[0m" << std::endl;
                            valid = false;
                        }
                        hasSplitData = true;
                    } else if (lowerCmd.find("train") != std::string::npos) {
                        if (!hasSplitData) {
                            std::cerr << "\033[33mคำเตือน: คุณควรแบ่งข้อมูลด้วยคำสั่ง 'split' ก่อน\033[0m" << std::endl;
                            valid = false;
                        }
                        hasTrainedModel = true;
                    } else if (lowerCmd.find("evaluate") != std::string::npos) {
                        if (!hasTrainedModel) {
                            std::cerr << "\033[33mคำเตือน: คุณควรฝึกโมเดลด้วยคำสั่ง 'train' ก่อน\033[0m" << std::endl;
                            valid = false;
                        }
                        hasEvaluatedModel = true;
                    } else if (lowerCmd.find("save") != std::string::npos) {
                        if (!hasTrainedModel) {
                            std::cerr << "\033[33mคำเตือน: คุณควรฝึกโมเดลด้วยคำสั่ง 'train' ก่อนที่จะบันทึก\033[0m" << std::endl;
                            valid = false;
                        }
                    }

                    // ประมวลผลคำสั่งถ้าลำดับขั้นตอนถูกต้อง
                    if (valid) {
                        if (command == "clean") {
                            std::cout << "> clean" << std::endl;
                            std::cout << "กำลังทำความสะอาดข้อมูล..." << std::endl;
                            std::cout << "ลบค่า null และแทนที่ด้วยค่าเฉลี่ย" << std::endl;
                            std::cout << "กำจัดค่า outlier" << std::endl;
                            std::cout << "ทำความสะอาดข้อมูลเสร็จสิ้น" << std::endl;
                        } else if (command == "split") {
                            std::cout << "> split" << std::endl;
                            std::cout << "กำลังแบ่งข้อมูลเป็นชุดฝึกและชุดทดสอบ..." << std::endl;
                            std::cout << "แบ่งข้อมูล 80% สำหรับชุดฝึก และ 20% สำหรับชุดทดสอบ" << std::endl;
                        } else if (command.substr(0, 5) == "train") {
                            std::cout << "> train";
                            std::string epochsStr = "100"; // ค่าเริ่มต้น
                            if (command.substr(0, 12) == "train epochs") {
                                epochsStr = command.substr(13);
                                std::cout << " epochs " << epochsStr;
                            }
                            std::cout << std::endl;
                            std::cout << "กำลังฝึกโมเดล Machine Learning..." << std::endl;
                            std::cout << "จำนวน epochs: " << epochsStr << std::endl;
                            std::cout << "โมเดลฝึกเสร็จสิ้น" << std::endl;
                        } else if (command == "evaluate") {
                            std::cout << "> evaluate" << std::endl;
                            std::cout << "กำลังประเมินผลโมเดล..." << std::endl;
                            std::cout << "ความแม่นยำ (accuracy): 0.92" << std::endl;
                            std::cout << "ความแม่นยำเชิงลึก (precision): 0.89" << std::endl;
                            std::cout << "ความไว (recall): 0.94" << std::endl;
                        } else if (command.substr(0, 4) == "show") {
                            std::cout << "> show";
                            if (command.substr(0, 11) == "show metric") {
                                std::string metric = command.substr(12);
                                if (metric.front() == '"' && metric.back() == '"') {
                                    metric = metric.substr(1, metric.length() - 2);
                                }
                                std::cout << " metric " << metric << std::endl;
                                std::cout << "กำลังแสดงเมตริก: " << metric << std::endl;

                                if (metric == "accuracy") {
                                    std::cout << "ความแม่นยำ (accuracy): 0.92" << std::endl;
                                } else if (metric == "precision") {
                                    std::cout << "ความแม่นยำเชิงลึก (precision): 0.89" << std::endl;
                                } else if (metric == "recall") {
                                    std::cout << "ความไว (recall): 0.94" << std::endl;
                                } else if (metric == "f1") {
                                    std::cout << "F1 score: 0.91" << std::endl;
                                } else {
                                    std::cout << "ไม่พบเมตริก: " << metric << std::endl;
                                }
                            } else {
                                std::cout << std::endl;
                            }
                        } else if (command.substr(0, 4) == "save") {
                            std::cout << "> save";
                            if (command.substr(0, 9) == "save path") {
                                std::string path = command.substr(10);
                                if (path.front() == '"' && path.back() == '"') {
                                    path = path.substr(1, path.length() - 2);
                                }
                                std::cout << " path " << path << std::endl;
                                // แสดงพาธเต็มของไฟล์ที่กำลังบันทึก
                                char abs_path[1024];
                                if (path[0] != '/') {  // ถ้าไม่ใช่พาธสัมบูรณ์
                                    char cwd[1024];
                                    if (getcwd(cwd, sizeof(cwd)) != NULL) {
                                        snprintf(abs_path, sizeof(abs_path), "%s/%s", cwd, path.c_str());
                                    } else {
                                        strncpy(abs_path, path.c_str(), sizeof(abs_path));
                                    }
                                } else {
                                    strncpy(abs_path, path.c_str(), sizeof(abs_path));
                                }

                                std::cout << "กำลังบันทึกโมเดลไปที่: " << path << std::endl;
                                std::cout << "พาธเต็ม: " << abs_path << std::endl;

                                // สร้างไฟล์ .pkl จริง
                                std::ofstream file(path.c_str(), std::ios::binary);
                                if (file.is_open()) {
                                    // เขียนข้อมูลโมเดลพื้นฐาน
                                    file << "# AI Language ML Model\n";
                                    file << "MODEL_TYPE=ML\n";
                                    file << "CREATED_TIME=" << time(nullptr) << "\n";
                                    file << "PARAMETERS=learning_rate:0.01,epochs:100\n";

                                    // เขียนค่า weight และ bias สมมติ
                                    file << "WEIGHTS=1.5,2.3,-0.8\n";
                                    file << "BIAS=0.5\n";

                                    file.close();
                                    std::cout << "บันทึกโมเดลสำเร็จ" << std::endl;
                                } else {
                                    std::cerr << "\033[31mเกิดข้อผิดพลาด: ไม่สามารถสร้างไฟล์ " << path << "\033[0m" << std::endl;
                                }
                            } else {
                                std::cout << std::endl;
                            }
                        } else {
                            interpreter.interpret(command);
                        }
                    }

                    // รีเซ็ตคำสั่งหลังจากประมวลผลแล้ว
                    command = "";
                } catch (const std::exception& e) {
                    std::cerr << "เกิดข้อผิดพลาด: " << e.what() << std::endl;
                    command = "";
                }
            }
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

    // ตัวแปรเก็บสถานะของโปรแกรม ต้องทำตามลำดับ
    bool hasStarted = false;
    bool hasCreatedProject = false;
    bool hasLoadedData = false;
    bool hasCreatedModel = false;
    bool hasTrainedModel = false;
    std::string projectType = "";
    std::string modelName = "";

    std::string line;
    int lineNumber = 0;
    
    while (std::getline(file, line)) {
        lineNumber++;
        
        // ข้ามบรรทัดว่างหรือคอมเมนต์
        if (line.empty() || line[0] == '#' || line.find_first_not_of(" \t") == std::string::npos) {
            continue;
        }
        
        std::cout << "> " << line << std::endl;
        
        // ตรวจสอบคำสั่งและความถูกต้องตามลำดับขั้นตอน
        if (line == "start") {
            if (hasStarted) {
                std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": คำสั่ง 'start' ถูกใช้ไปแล้ว\033[0m" << std::endl;
                continue;
            }
            
            hasStarted = true;
            std::cout << "Program started" << std::endl;
        } 
        else if (line.find("create ") == 0) {
            // คำสั่ง create project หรือ create model
            std::istringstream iss(line);
            std::string cmd, type;
            iss >> cmd >> type;
            
            if (!hasStarted) {
                std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Must start with 'start' command\033[0m" << std::endl;
                continue;
            }
            
            if (type == "ML" || type == "DL" || type == "RL") {
                // create project type
                if (hasCreatedProject) {
                    std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Project type already defined\033[0m" << std::endl;
                    continue;
                }
                
                if (hasLoadedData || hasCreatedModel || hasTrainedModel) {
                    std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Cannot change project type after other operations\033[0m" << std::endl;
                    continue;
                }
                
                projectType = type;
                hasCreatedProject = true;
                
                if (type == "ML") {
                    std::cout << "Project created: Machine Learning" << std::endl;
                } else if (type == "DL") {
                    std::cout << "Project created: Deep Learning" << std::endl;
                } else if (type == "RL") {
                    std::cout << "Project created: Reinforcement Learning" << std::endl;
                }
            } 
            else if (type == "model") {
                // create model
                if (!hasCreatedProject) {
                    std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Must create project type first\033[0m" << std::endl;
                    continue;
                }
                
                if (!hasLoadedData) {
                    std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Must load dataset first\033[0m" << std::endl;
                    continue;
                }
                
                if (hasCreatedModel) {
                    std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Model already created\033[0m" << std::endl;
                    continue;
                }
                
                // รับชื่อโมเดล
                std::string model;
                iss >> model;
                
                if (model.empty()) {
                    std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Model name required\033[0m" << std::endl;
                    continue;
                }
                
                // ตรวจสอบความเข้ากันได้ของโมเดลกับประเภทโปรเจกต์
                bool validModel = false;
                if (projectType == "ML" && 
                    (model == "LinearRegression" || model == "LogisticRegression" || 
                     model == "RandomForest" || model == "SVM" || model == "KNN")) {
                    validModel = true;
                } else if (projectType == "DL" && 
                           (model == "NeuralNetwork" || model == "CNN" || model == "RNN")) {
                    validModel = true;
                } else if (projectType == "RL" && 
                           (model == "QLearning" || model == "DQN")) {
                    validModel = true;
                }
                
                if (!validModel) {
                    std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Model not compatible with project type\033[0m" << std::endl;
                    continue;
                }
                
                modelName = model;
                hasCreatedModel = true;
                std::cout << "Model created: " << model << std::endl;
            } 
            else {
                std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Invalid create command. Use 'create ML/DL/RL' or 'create model <model_name>'\033[0m" << std::endl;
            }
        } 
        else if (line.find("load dataset") == 0) {
            if (!hasStarted) {
                std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Must start with 'start' command\033[0m" << std::endl;
                continue;
            }
            
            if (!hasCreatedProject) {
                std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Must create project type first\033[0m" << std::endl;
                continue;
            }
            
            if (hasLoadedData) {
                std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Dataset already loaded\033[0m" << std::endl;
                continue;
            }
            
            // ตรวจจับคำสั่ง load dataset "filename"
            size_t filenameStart = line.find("\"");
            
            if (filenameStart != std::string::npos) {
                size_t filenameEnd = line.find("\"", filenameStart + 1);
                
                if (filenameEnd != std::string::npos) {
                    std::string filename = line.substr(filenameStart + 1, filenameEnd - filenameStart - 1);
                    
                    // ตรวจสอบนามสกุลไฟล์ว่าเป็น .csv หรือไม่
                    if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".csv") {
                        std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": File must be a CSV\033[0m" << std::endl;
                        continue;
                    }
                    
                    // ตรวจสอบว่าไฟล์มีอยู่จริงหรือไม่
                    std::ifstream f(filename);
                    if (!f.good()) {
                        std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": File not found\033[0m" << std::endl;
                        continue;
                    }
                    
                    std::cout << "กำลังโหลดข้อมูลจากไฟล์: " << filename << std::endl;
                    std::cout << "Dataset loaded successfully" << std::endl;
                    hasLoadedData = true;
                } else {
                    std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Invalid dataset filename format\033[0m" << std::endl;
                }
            } else {
                std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Missing dataset filename\033[0m" << std::endl;
            }
        } 
        else if (line == "train model") {
            if (!hasStarted) {
                std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Must start with 'start' command\033[0m" << std::endl;
                continue;
            }
            
            if (!hasCreatedProject) {
                std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Must create project type first\033[0m" << std::endl;
                continue;
            }
            
            if (!hasLoadedData) {
                std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Must load dataset first\033[0m" << std::endl;
                continue;
            }
            
            if (!hasCreatedModel) {
                std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Must create model first\033[0m" << std::endl;
                continue;
            }
            
            if (hasTrainedModel) {
                std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Model already trained\033[0m" << std::endl;
                continue;
            }
            
            std::cout << "Training model: " << modelName << "..." << std::endl;
            std::cout << "Training complete" << std::endl;
            hasTrainedModel = true;
        } 
        else if (line == "show accuracy") {
            if (!hasStarted) {
                std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Must start with 'start' command\033[0m" << std::endl;
                continue;
            }
            
            if (!hasCreatedProject) {
                std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Must create project type first\033[0m" << std::endl;
                continue;
            }
            
            if (!hasLoadedData) {
                std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Must load dataset first\033[0m" << std::endl;
                continue;
            }
            
            if (!hasCreatedModel) {
                std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Must create model first\033[0m" << std::endl;
                continue;
            }
            
            if (!hasTrainedModel) {
                std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Model not trained yet\033[0m" << std::endl;
                continue;
            }
            
            // สร้างความแม่นยำสุ่ม
            double accuracy = 80.0 + (std::rand() % 15) + (std::rand() % 100) / 100.0;
            std::cout << "Accuracy: " << accuracy << "%" << std::endl;
        } 
        else {
            // คำสั่งที่ไม่รู้จัก
            std::cerr << "\033[31mError ที่บรรทัด " << lineNumber << ": Unknown command: " << line << "\033[0m" << std::endl;
        }
    }
    
    file.close();
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