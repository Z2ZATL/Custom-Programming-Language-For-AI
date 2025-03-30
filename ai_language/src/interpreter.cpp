
#include "../include/interpreter.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <filesystem>
#include <algorithm>
#include <random>

namespace ai_language {

Interpreter::Interpreter() 
    : m_hasError(false),
      m_outputHandler([](const std::string& msg) { std::cout << msg << std::endl; }),
      m_errorHandler([](const std::string& msg) { std::cerr << "Error: " << msg << std::endl; }) {
}

void Interpreter::setOutputHandler(OutputHandler handler) {
    m_outputHandler = handler;
}

void Interpreter::setErrorHandler(ErrorHandler handler) {
    m_errorHandler = handler;
}

bool Interpreter::hasError() const {
    return m_hasError;
}

std::string Interpreter::getLastError() const {
    return m_lastError;
}

void Interpreter::interpret(const std::string& source) {
    try {
        m_hasError = false;
        m_lastError = "";
        
        // สร้างสภาวะแวดล้อมการทำงานใหม่
        Environment env;
        
        // แยกบรรทัด
        std::istringstream stream(source);
        std::string line;
        
        while (std::getline(stream, line)) {
            // ข้ามบรรทัดว่างและคอมเมนต์
            if (line.empty() || line[0] == '#') {
                continue;
            }
            
            // ลบช่องว่างด้านหน้าและด้านหลัง
            line = trim(line);
            if (line.empty()) {
                continue;
            }
            
            // แยกคำสั่งและพารามิเตอร์
            auto [command, params] = parseCommand(line);
            
            // ดำเนินการตามคำสั่ง
            executeCommand(command, params, env);
        }
    } catch (const std::exception& e) {
        m_hasError = true;
        m_lastError = e.what();
        m_errorHandler(e.what());
    }
}

std::string Interpreter::trim(const std::string& str) {
    auto start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    
    auto end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

std::pair<std::string, std::map<std::string, std::string>> Interpreter::parseCommand(const std::string& line) {
    std::string command;
    std::map<std::string, std::string> params;
    
    size_t pos = 0;
    std::string token;
    std::vector<std::string> tokens;
    
    // แยกโทเค็นโดยพิจารณา string literals
    bool inQuotes = false;
    std::string currentToken;
    
    for (size_t i = 0; i < line.length(); i++) {
        char c = line[i];
        
        if (c == '"') {
            inQuotes = !inQuotes;
            currentToken += c;
        } else if (c == ' ' && !inQuotes) {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
        } else {
            currentToken += c;
        }
    }
    
    if (!currentToken.empty()) {
        tokens.push_back(currentToken);
    }
    
    // หากไม่มีโทเค็น ให้คืนค่าว่าง
    if (tokens.empty()) {
        return {"", {}};
    }
    
    // โทเค็นแรกเป็นคำสั่ง
    command = tokens[0];
    
    // ตรวจหาคำสั่งรวม เช่น "start create"
    if (tokens.size() > 1 && 
        (command == "start" || command == "load" || command == "save" || 
         command == "show" || command == "train")) {
        command += " " + tokens[1];
        tokens.erase(tokens.begin()); // ลบโทเค็นแรกออกไป
    }
    
    // แปลงโทเค็นที่เหลือเป็นพารามิเตอร์
    for (size_t i = 1; i < tokens.size(); i++) {
        // ตรวจหาคีย์และค่า
        if (i + 1 < tokens.size()) {
            std::string key = tokens[i];
            std::string value = tokens[i + 1];
            
            // ถ้าค่าเป็น string literal ให้ลบ quotes ออก
            if (value.front() == '"' && value.back() == '"') {
                value = value.substr(1, value.length() - 2);
            }
            
            // เพิ่มคีย์และค่าลงใน params
            if (key != "with" && key != "type" && key != "path" && key != "epochs") {
                params[key] = value;
                i++; // ข้ามค่าที่เพิ่งประมวลผล
            } else {
                // คีย์พิเศษ
                if (key == "type") {
                    params["type"] = value;
                    i++;
                } else if (key == "path") {
                    params["path"] = value;
                    i++;
                } else if (key == "epochs") {
                    params["epochs"] = value;
                    i++;
                }
            }
        } else {
            // ถ้าเหลือโทเค็นเดียว ให้ใส่เป็นพารามิเตอร์โดยไม่มีคีย์
            params[""] = tokens[i];
        }
    }
    
    return {command, params};
}

void Interpreter::executeCommand(const std::string& command, 
                               const std::map<std::string, std::string>& params,
                               Environment& env) {
    // แสดงคำสั่งที่กำลังทำงาน
    std::string displayCommand = "\n> " + command;
    for (const auto& [key, value] : params) {
        if (!key.empty()) {
            displayCommand += " " + key + " ";
            if (value.find(' ') != std::string::npos) {
                displayCommand += "\"" + value + "\"";
            } else {
                displayCommand += value;
            }
        }
    }
    m_outputHandler(displayCommand);
    
    // ตรวจคำสั่งและดำเนินการตามนั้น
    if (command == "start") {
        // คำสั่ง start ปกติ
        // ไม่มีการดำเนินการเฉพาะ
    } else if (command == "start create") {
        // คำสั่ง start create <type>
        if (params.find("") != params.end()) {
            std::string type = params.at("");
            env.projectType = type;
            if (type == "ML" || type == "ml") {
                m_outputHandler("เริ่มต้นโปรเจกต์ Machine Learning");
            } else if (type == "DL" || type == "dl") {
                m_outputHandler("เริ่มต้นโปรเจกต์ Deep Learning");
            } else if (type == "RL" || type == "rl") {
                m_outputHandler("เริ่มต้นโปรเจกต์ Reinforcement Learning");
            } else {
                m_outputHandler("เริ่มต้นโปรเจกต์: " + type);
            }
        }
    } else if (command == "load dataset") {
        // คำสั่ง load dataset <filename> type <type>
        std::string filename = "";
        std::string type = "csv"; // ค่าเริ่มต้น
        
        if (params.find("filename") != params.end()) {
            filename = params.at("filename");
        }
        
        if (params.find("type") != params.end()) {
            type = params.at("type");
        }
        
        m_outputHandler("กำลังโหลดข้อมูลจากไฟล์: " + filename);
        env.dataLoaded = true;
        env.datasetPath = filename;
        env.datasetType = type;
        
    } else if (command == "clean") {
        // คำสั่ง clean data
        if (!env.dataLoaded) {
            m_errorHandler("ไม่มีข้อมูลที่โหลดไว้");
            return;
        }
        
        m_outputHandler("กำลังทำความสะอาดข้อมูล");
        env.dataCleaned = true;
        
    } else if (command == "split") {
        // คำสั่ง split data
        if (!env.dataLoaded) {
            m_errorHandler("ไม่มีข้อมูลที่โหลดไว้");
            return;
        }
        
        // ตรวจสอบพารามิเตอร์ของการแบ่งข้อมูล
        double trainRatio = 0.8; // ค่าเริ่มต้น
        if (params.find("ratio") != params.end()) {
            try {
                trainRatio = std::stod(params.at("ratio"));
            } catch (...) {
                m_errorHandler("ค่า ratio ไม่ถูกต้อง");
                return;
            }
        }
        
        m_outputHandler("กำลังแบ่งข้อมูลด้วยอัตราส่วน train:test = " 
                      + std::to_string(trainRatio) + ":" 
                      + std::to_string(1.0 - trainRatio));
        env.dataSplit = true;
        env.trainRatio = trainRatio;
        
    } else if (command == "train") {
        // คำสั่ง train model [epochs <num>]
        if (!env.dataLoaded || !env.dataSplit) {
            m_errorHandler("ต้องโหลดและแบ่งข้อมูลก่อนที่จะฝึกโมเดล");
            return;
        }
        
        int epochs = 10; // ค่าเริ่มต้น
        if (params.find("epochs") != params.end()) {
            try {
                epochs = std::stoi(params.at("epochs"));
            } catch (...) {
                m_errorHandler("จำนวน epochs ไม่ถูกต้อง");
                return;
            }
        }
        
        m_outputHandler("กำลังฝึกโมเดลด้วย " + std::to_string(epochs) + " epochs");
        env.modelTrained = true;
        env.epochs = epochs;
        
        // จำลองการฝึกโมเดล
        m_outputHandler("Epoch 1/" + std::to_string(epochs) + ": loss = 0.9542, accuracy = 0.6231");
        
        if (epochs > 1) {
            m_outputHandler("...");
            m_outputHandler("Epoch " + std::to_string(epochs) + "/" + std::to_string(epochs) + 
                          ": loss = 0.1245, accuracy = 0.9876");
        }
        
    } else if (command == "evaluate") {
        // คำสั่ง evaluate model
        if (!env.modelTrained) {
            m_errorHandler("ต้องฝึกโมเดลก่อนที่จะประเมินผล");
            return;
        }
        
        m_outputHandler("กำลังประเมินผลโมเดล");
        env.modelEvaluated = true;
        
        // สร้างเมทริกซ์การประเมินผลแบบสุ่ม
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dist(0.7, 0.99);
        
        env.metrics["accuracy"] = dist(gen);
        env.metrics["precision"] = dist(gen);
        env.metrics["recall"] = dist(gen);
        env.metrics["f1"] = dist(gen);
        
    } else if (command == "show metric") {
        // คำสั่ง show metric <name>
        if (!env.modelEvaluated) {
            m_errorHandler("ต้องประเมินผลโมเดลก่อนที่จะแสดงเมทริกซ์");
            return;
        }
        
        std::string metricName = "";
        if (params.find("metric") != params.end()) {
            metricName = params.at("metric");
        }
        
        m_outputHandler("กำลังแสดงเมตริก: " + metricName);
        
        if (env.metrics.find(metricName) != env.metrics.end()) {
            m_outputHandler(metricName + " = " + std::to_string(env.metrics[metricName]));
        } else {
            m_errorHandler("ไม่พบเมทริกซ์ชื่อ " + metricName);
        }
        
    } else if (command == "save") {
        // คำสั่ง save model path <path>
        if (!env.modelTrained) {
            m_errorHandler("ต้องฝึกโมเดลก่อนที่จะบันทึก");
            return;
        }
        
        std::string path = "";
        if (params.find("path") != params.end()) {
            path = params.at("path");
        }
        
        if (path.empty()) {
            m_errorHandler("ต้องระบุพาธที่จะบันทึกโมเดล");
            return;
        }
        
        m_outputHandler("กำลังบันทึกโมเดลไปยัง " + path);
        
        // จำลองการบันทึกโมเดล
        std::ofstream modelFile(path);
        if (modelFile.is_open()) {
            modelFile << "AI Language Model Serialized Data\n";
            modelFile << "Type: " << env.projectType << "\n";
            modelFile << "Epochs: " << env.epochs << "\n";
            modelFile << "Accuracy: " << env.metrics["accuracy"] << "\n";
            modelFile.close();
            m_outputHandler("บันทึกโมเดลสำเร็จ");
        } else {
            m_errorHandler("ไม่สามารถบันทึกโมเดลไปยัง " + path);
        }
        
    } else if (command == "visualize") {
        // คำสั่ง visualize data
        if (!env.dataLoaded) {
            m_errorHandler("ต้องโหลดข้อมูลก่อนที่จะสร้างภาพ");
            return;
        }
        
        m_outputHandler("กำลังสร้างภาพข้อมูล");
        
        // จำลองการสร้างภาพ
        m_outputHandler("วิเคราะห์ข้อมูล 1,000 แถวและ 10 คอลัมน์");
        m_outputHandler("สร้างภาพแผนภูมิกระจาย (Scatter Plot) สำหรับคุณลักษณะที่สำคัญที่สุด");
        
    } else if (command == "predict") {
        // คำสั่ง predict using <file>
        if (!env.modelTrained) {
            m_errorHandler("ต้องฝึกโมเดลก่อนที่จะทำนาย");
            return;
        }
        
        std::string file = "";
        if (params.find("using") != params.end()) {
            file = params.at("using");
        }
        
        if (file.empty()) {
            m_errorHandler("ต้องระบุไฟล์ข้อมูลที่จะใช้ทำนาย");
            return;
        }
        
        m_outputHandler("กำลังทำนายโดยใช้ข้อมูลจาก " + file);
        
        // จำลองการทำนาย
        m_outputHandler("ทำนายข้อมูล 100 แถว");
        m_outputHandler("ผลลัพธ์ถูกบันทึกไปที่ 'predictions.csv'");
        
    } else {
        // คำสั่งที่ไม่รู้จัก
        m_errorHandler("ไม่รู้จักคำสั่ง: " + command);
    }
}

} // namespace ai_language
