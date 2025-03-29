#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

// บรรทัดนี้สามารถเปลี่ยนเป็น import โมดูลในอนาคต
// #include "include/interpreter.h"

// ฟังก์ชันสำหรับอ่านไฟล์
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: ไม่สามารถเปิดไฟล์ " << filename << std::endl;
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// ฟังก์ชันสำหรับแบ่งข้อความเป็นบรรทัด
std::vector<std::string> splitLines(const std::string& text) {
    std::vector<std::string> lines;
    std::istringstream stream(text);
    std::string line;
    
    while (std::getline(stream, line)) {
        // ข้ามบรรทัดว่างและคอมเมนต์
        if (line.empty() || line[0] == '#' || line[0] == '/') {
            continue;
        }
        
        // ตัดช่องว่างที่ไม่จำเป็น
        size_t start = line.find_first_not_of(" \t");
        if (start != std::string::npos) {
            size_t end = line.find_last_not_of(" \t");
            line = line.substr(start, end - start + 1);
            lines.push_back(line);
        }
    }
    
    return lines;
}

// ฟังก์ชันสำหรับแยกคำสั่งและพารามิเตอร์
std::pair<std::string, std::map<std::string, std::string>> parseCommand(const std::string& line) {
    std::string command;
    std::map<std::string, std::string> params;
    
    size_t pos = line.find_first_of(" \t");
    if (pos != std::string::npos) {
        command = line.substr(0, pos);
        
        // แยกพารามิเตอร์
        std::string remaining = line.substr(pos + 1);
        
        // ใช้ฟังก์ชันพิเศษสำหรับคำสั่งที่มีรูปแบบเฉพาะ
        if (command == "start" && remaining.find("create") == 0) {
            size_t typePos = remaining.find_first_of(" \t", 7); // ข้าม "create "
            if (typePos != std::string::npos) {
                std::string type = remaining.substr(typePos + 1);
                command = "start create";
                params["type"] = type;
            }
        } else if (command == "load" && remaining.find("dataset") == 0) {
            // ตัดคำว่า "dataset "
            remaining = remaining.substr(8);
            command = "load dataset";
            
            // ดึงชื่อไฟล์ใน quotes
            size_t start = remaining.find('"');
            if (start != std::string::npos) {
                size_t end = remaining.find('"', start + 1);
                if (end != std::string::npos) {
                    params["filename"] = remaining.substr(start + 1, end - start - 1);
                    
                    // ถ้ามี type
                    size_t typePos = remaining.find("type", end);
                    if (typePos != std::string::npos) {
                        size_t typeStart = remaining.find('"', typePos);
                        if (typeStart != std::string::npos) {
                            size_t typeEnd = remaining.find('"', typeStart + 1);
                            if (typeEnd != std::string::npos) {
                                params["type"] = remaining.substr(typeStart + 1, typeEnd - typeStart - 1);
                            }
                        }
                    }
                }
            }
        } else {
            // แยกพารามิเตอร์ทั่วไป
            std::istringstream iss(remaining);
            std::string token;
            std::string currentKey;
            
            while (iss >> token) {
                // ถ้าเป็น string ในเครื่องหมายคำพูด
                if (token.front() == '"' && token.back() != '"') {
                    // เริ่มอ่าน string ที่มีช่องว่าง
                    std::string value = token.substr(1);
                    std::string nextToken;
                    while (iss >> nextToken) {
                        value += " " + nextToken;
                        if (nextToken.back() == '"') {
                            // จบ string
                            value = value.substr(0, value.size() - 1);
                            params[currentKey] = value;
                            currentKey = "";
                            break;
                        }
                    }
                }
                // ถ้าเป็น string ปกติ
                else if (token.front() == '"' && token.back() == '"') {
                    params[currentKey] = token.substr(1, token.size() - 2);
                    currentKey = "";
                }
                // ถ้าเป็นตัวเลข
                else if (std::isdigit(token.front()) || token.front() == '.') {
                    params[currentKey] = token;
                    currentKey = "";
                }
                // ถ้าเป็นชื่อพารามิเตอร์
                else {
                    currentKey = token;
                }
            }
        }
    } else {
        command = line;
    }
    
    return {command, params};
}

// ฟังก์ชันเพื่อจำลองการทำงานของคำสั่ง
void executeCommand(const std::string& command, const std::map<std::string, std::string>& params) {
    std::cout << "\n> " << command;
    for (const auto& [key, value] : params) {
        if (!key.empty()) {
            std::cout << " " << key << " ";
            if (value.find(' ') != std::string::npos) {
                std::cout << "\"" << value << "\"";
            } else {
                std::cout << value;
            }
        }
    }
    std::cout << std::endl;

    // ML Commands
    if (command == "start create") {
        std::cout << "เริ่มต้นการสร้างโปรเจกต์ประเภท: " << params.at("type") << std::endl;
        if (params.at("type") == "ML" || params.at("type") == "ml") {
            std::cout << "เริ่มต้นโปรเจกต์ Machine Learning" << std::endl;
        } else if (params.at("type") == "DL" || params.at("type") == "dl") {
            std::cout << "เริ่มต้นโปรเจกต์ Deep Learning" << std::endl;
        } else if (params.at("type") == "RL" || params.at("type") == "rl") {
            std::cout << "เริ่มต้นโปรเจกต์ Reinforcement Learning" << std::endl;
        }
    } else if (command == "load dataset") {
        if (params.find("filename") != params.end()) {
            std::cout << "กำลังโหลดข้อมูลจากไฟล์: " << params.at("filename") << std::endl;
        }
    } else if (command == "clean data") {
        std::cout << "กำลังทำความสะอาดข้อมูล" << std::endl;
    } else if (command == "split data") {
        std::cout << "กำลังแบ่งข้อมูลสำหรับการฝึกและทดสอบ" << std::endl;
    } else if (command == "visualize data") {
        std::cout << "กำลังแสดงภาพข้อมูล: data" << std::endl;
    } else if (command == "train model") {
        std::cout << "กำลังฝึกโมเดล" << std::endl;
        if (params.find("epochs") != params.end()) {
            std::cout << "พารามิเตอร์: epochs = " << params.at("epochs") << std::endl;
        }
    } else if (command == "evaluate model") {
        std::cout << "กำลังประเมินโมเดลกับข้อมูล: test_data" << std::endl;
    } else if (command == "show") {
        if (params.find("metric") != params.end()) {
            std::cout << "กำลังแสดงเมตริก: " << params.at("metric") << std::endl;
        }
    } else if (command == "save model") {
        if (params.find("path") != params.end()) {
            std::cout << "กำลังบันทึกโมเดลไปยังไฟล์: " << params.at("path") << std::endl;
        }
    } else if (command == "plot") {
        if (params.find("metric") != params.end()) {
            std::cout << "กำลังพล็อตกราฟ: " << params.at("metric") << std::endl;
        }
    } else if (command == "create") {
        if (params.find("model_type") != params.end()) {
            std::cout << "กำลังสร้างโมเดลประเภท: " << params.at("model_type") << std::endl;
        } else if (params.find("neural_network") != params.end()) {
            std::cout << "กำลังสร้างโครงข่ายประสาทเทียม" << std::endl;
        } else if (params.find("environment") != params.end()) {
            std::cout << "กำลังสร้างสภาพแวดล้อมสำหรับ Reinforcement Learning" << std::endl;
        } else if (params.find("agent") != params.end()) {
            std::cout << "กำลังสร้าง agent สำหรับ Reinforcement Learning" << std::endl;
        }
    } else if (command == "visualize") {
        if (params.find("data") != params.end()) {
            std::cout << "กำลังแสดงภาพข้อมูล: " << params.at("data") << std::endl;
        }
    } else if (command == "train agent") {
        std::cout << "กำลังฝึก agent" << std::endl;
        if (params.find("episodes") != params.end()) {
            std::cout << "จำนวน episodes: " << params.at("episodes") << std::endl;
        }
    }
    // ถ้าไม่มีคำสั่งที่รองรับ
    else {
        // ไม่ทำอะไร (สำหรับคำสั่งที่ยังไม่รองรับในเวอร์ชันนี้)
    }
}

int main(int argc, char* argv[]) {
    std::cout << "=== ทดสอบภาษาโปรแกรมสำหรับ AI ===" << std::endl << std::endl;
    
    // ตรวจสอบการป้อนคำสั่ง
    if (argc > 1) {
        std::string filename = argv[1];
        std::string code = readFile(filename);
        
        if (!code.empty()) {
            std::vector<std::string> lines = splitLines(code);
            
            for (const auto& line : lines) {
                auto [command, params] = parseCommand(line);
                executeCommand(command, params);
            }
        }
    } else {
        // โหมดโต้ตอบ (จะพัฒนาในอนาคต)
        std::cout << "โปรดระบุไฟล์ .ai เพื่อรัน (เช่น ./ai_lang example.ai)" << std::endl;
    }
    
    return 0;
}