/**
 * @file interpreter.cpp
 * @brief การดำเนินการของ Interpreter สำหรับแปลและทำงานตามคำสั่งในภาษา AI
 */

#include "../include/interpreter.h"
#include <iostream>
#include <sstream>
#include <random>

namespace ai_language {

Interpreter::Interpreter() : m_lexer(""), m_hasError(false) {
    // ตั้งค่าฟังก์ชันสำหรับแสดงผลลัพธ์เริ่มต้น
    m_outputHandler = [](const std::string& message) {
        std::cout << message << std::endl;
    };

    // ตั้งค่าฟังก์ชันสำหรับแสดงข้อผิดพลาดเริ่มต้น
    m_errorHandler = [](const std::string& message) {
        std::cerr << "Interpreter Error: " << message << std::endl;
    };

    // ตั้งค่า handler ของ parser
    m_parser.setErrorHandler([this](const std::string& message) {
        m_errorHandler(message);
        m_hasError = true;
    });
}

void Interpreter::setOutputHandler(std::function<void(const std::string&)> handler) {
    m_outputHandler = handler;
}

void Interpreter::setErrorHandler(std::function<void(const std::string&)> handler) {
    m_errorHandler = handler;
}

void Interpreter::interpret(const std::string& source) {
    // รีเซ็ตสถานะข้อผิดพลาด
    resetError();

    try {
        // สร้าง Lexer ใหม่ด้วย source code
        m_lexer = Lexer(source);

        // แปลงโค้ดเป็น token
        std::vector<Token> tokens = m_lexer.tokenize();

        // ตรวจสอบว่ามีข้อผิดพลาดหรือไม่
        if (m_lexer.hasError()) {
            m_errorHandler(m_lexer.getError());
            m_hasError = true;
            return;
        }

        // แปลง token เป็น AST
        std::shared_ptr<Program> program = m_parser.parse(tokens);

        // ตรวจสอบว่ามีข้อผิดพลาดหรือไม่
        if (m_parser.hasError()) {
            m_hasError = true;
            return;
        }

        // ประมวลผล AST
        // TODO: ดำเนินการตาม AST
        m_outputHandler("ดำเนินการแปลภาษาเสร็จสิ้น");

    } catch (const std::exception& e) {
        m_errorHandler(std::string("ข้อผิดพลาดที่ไม่คาดคิด: ") + e.what());
        m_hasError = true;
    }
}

bool Interpreter::hasError() const {
    return m_hasError;
}

void Interpreter::resetError() {
    m_hasError = false;
    m_parser.resetError();
}

void Interpreter::executeCommand(const std::string& command, const std::map<std::string, std::string>& params) {
    try {
        if (command == "start") {
            handleStartCommand(params);
        } else if (command == "load") {
            handleLoadCommand(params);
        } else if (command == "clean") {
            handleCleanCommand(params);
        } else if (command == "split") {
            handleSplitCommand(params);
        } else if (command == "train") {
            handleTrainCommand(params);
        } else if (command == "evaluate") {
            handleEvaluateCommand(params);
        } else if (command == "add") {
            handleAddCommand(params);
        } else if (command == "predict") {
            handlePredictCommand(params);
        } else if (command == "save") {
            handleSaveCommand(params);
        } else if (command == "show") {
            handleShowCommand(params);
        } else if (command == "visualize") {
            handleVisualizeCommand(params);
        } else if (command == "plot") {
            handlePlotCommand(params);
        } else {
            m_errorHandler("ไม่รู้จักคำสั่ง: " + command);
        }
    } catch (const std::exception& e) {
        m_errorHandler("เกิดข้อผิดพลาดขณะทำงานตามคำสั่ง: " + std::string(e.what()));
        m_hasError = true;
    }
}

void Interpreter::handleStartCommand(const std::map<std::string, std::string>& params) {
    // ตั้งค่าประเภทโปรเจกต์
    if (params.find("type") != params.end()) {
        m_environment.projectType = params.at("type");
    } else {
        m_environment.projectType = "ML";  // ค่าเริ่มต้น
    }

    // รีเซ็ตสถานะทั้งหมด
    m_environment.dataLoaded = false;
    m_environment.dataCleaned = false;
    m_environment.dataSplit = false;
    m_environment.modelTrained = false;
    m_environment.modelEvaluated = false;
    m_environment.modelSaved = false;
    m_environment.datasetPath = "";
    m_environment.datasetType = "";
    m_environment.modelPath = "";
    m_environment.metrics.clear();
    m_environment.variables.clear();

    m_outputHandler("เริ่มต้นโปรเจกต์ประเภท: " + m_environment.projectType);
}

void Interpreter::handleLoadCommand(const std::map<std::string, std::string>& params) {
    // ตรวจสอบว่าได้เริ่มต้นโปรเจกต์หรือยัง
    if (m_environment.projectType.empty()) {
        m_errorHandler("โปรดใช้คำสั่ง 'start' เพื่อเริ่มต้นโปรเจกต์ก่อนโหลดข้อมูล");
        return;
    }

    if (params.find("dataset") != params.end()) {
        std::string filename = params.at("dataset");
        m_environment.datasetPath = filename;

        std::string fileType = "csv";  // ค่าเริ่มต้น
        if (params.find("type") != params.end()) {
            fileType = params.at("type");
        }

        m_outputHandler("กำลังโหลดข้อมูลจากไฟล์: " + filename);
        m_environment.dataLoaded = true;
    } else {
        m_errorHandler("ไม่ได้ระบุชื่อไฟล์ข้อมูล");
    }
}

void Interpreter::handleCleanCommand(const std::map<std::string, std::string>& params) {
    // ตรวจสอบว่าได้โหลดข้อมูลแล้วหรือไม่
    if (!m_environment.dataLoaded) {
        m_errorHandler("ต้องโหลดข้อมูลก่อนที่จะทำความสะอาด");
        return;
    }

    m_environment.dataCleaned = true;
    m_outputHandler("กำลังทำความสะอาดข้อมูล");

    // แสดงพารามิเตอร์ที่ใช้
    for (const auto& pair : params) {
        m_outputHandler("  พารามิเตอร์: " + pair.first + " = " + pair.second);
    }
}

void Interpreter::handleSplitCommand(const std::map<std::string, std::string>& params) {
    // ตรวจสอบว่าได้ทำความสะอาดข้อมูลแล้วหรือไม่
    if (!m_environment.dataCleaned) {
        m_errorHandler("ต้องทำความสะอาดข้อมูลก่อนที่จะแบ่ง");
        return;
    }

    m_environment.dataSplit = true;
    m_outputHandler("กำลังแบ่งข้อมูล");

    // แสดงพารามิเตอร์ที่ใช้
    for (const auto& pair : params) {
        m_outputHandler("  พารามิเตอร์: " + pair.first + " = " + pair.second);
    }
}

void Interpreter::handleTrainCommand(const std::map<std::string, std::string>& params) {
    // ตรวจสอบว่าได้แบ่งข้อมูลแล้วหรือไม่
    if (!m_environment.dataSplit) {
        m_errorHandler("ต้องแบ่งข้อมูลก่อนที่จะฝึกโมเดล");
        return;
    }

    m_environment.modelTrained = true;
    m_outputHandler("กำลังฝึกโมเดล");

    // แสดงพารามิเตอร์ที่ใช้
    for (const auto& pair : params) {
        m_outputHandler("  พารามิเตอร์: " + pair.first + " = " + pair.second);
    }
}

void Interpreter::handleEvaluateCommand(const std::map<std::string, std::string>& params) {
    // ตรวจสอบว่าได้ฝึกโมเดลแล้วหรือไม่
    if (!m_environment.modelTrained) {
        m_errorHandler("ต้องฝึกโมเดลก่อนที่จะประเมินผล");
        return;
    }

    m_outputHandler("กำลังประเมินผลโมเดล");
    m_environment.modelEvaluated = true;

    // สร้างเมทริกซ์การประเมินผลแบบสุ่ม
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.7, 0.99);

    m_environment.metrics["accuracy"] = dist(gen);
    m_environment.metrics["precision"] = dist(gen);
    m_environment.metrics["recall"] = dist(gen);
    m_environment.metrics["f1"] = dist(gen);

    // แสดงพารามิเตอร์ที่ใช้
    for (const auto& pair : params) {
        m_outputHandler("  พารามิเตอร์: " + pair.first + " = " + pair.second);
    }
}

void Interpreter::handleAddCommand(const std::map<std::string, std::string>& params) {
    if (params.find("type") == params.end()) {
        m_errorHandler("ต้องระบุ type ของสิ่งที่จะเพิ่ม (เช่น layer)");
        return;
    }
    std::string type = params.at("type");
    if (type == "layer") {
        if (params.find("layer_type") == params.end() || params.find("units") == params.end()) {
            m_errorHandler("ต้องระบุ layer_type และ units สำหรับการเพิ่ม layer");
            return;
        }
        std::string layerType = params.at("layer_type");
        int units = std::stoi(params.at("units"));
        std::string activation = "relu"; // default activation
        if (params.find("activation") != params.end()){
            activation = params.at("activation");
        }
        m_outputHandler("กำลังเพิ่ม layer ประเภท: " + layerType + ", units: " + std::to_string(units) + ", activation: " + activation);
        // Add layer to model here...  This would involve interacting with a model object.
    } else {
        m_errorHandler("ไม่รู้จัก type ที่จะเพิ่ม: " + type);
    }
}


void Interpreter::handlePredictCommand(const std::map<std::string, std::string>& params) {
    // ตรวจสอบว่าได้ฝึกโมเดลแล้วหรือไม่
    if (!m_environment.modelTrained) {
        m_errorHandler("ต้องฝึกโมเดลก่อนที่จะทำนาย");
        return;
    }

    m_outputHandler("กำลังทำนายผลลัพธ์");

    // สร้างผลลัพธ์แบบสุ่ม
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    m_outputHandler("ผลลัพธ์การทำนาย: " + std::to_string(dist(gen)));

    // แสดงพารามิเตอร์ที่ใช้
    for (const auto& pair : params) {
        m_outputHandler("  พารามิเตอร์: " + pair.first + " = " + pair.second);
    }
}

void Interpreter::handleSaveCommand(const std::map<std::string, std::string>& params) {
    // ตรวจสอบว่าได้ฝึกโมเดลแล้วหรือไม่
    if (!m_environment.modelTrained) {
        m_errorHandler("ต้องฝึกโมเดลก่อนที่จะบันทึก");
        return;
    }

    // ตรวจสอบว่ามีพารามิเตอร์ที่จำเป็นหรือไม่
    if (params.find("path") == params.end()) {
        m_errorHandler("ต้องระบุที่อยู่ไฟล์");
        return;
    }

    m_environment.modelPath = params.at("path");
    m_environment.modelSaved = true;

    m_outputHandler("กำลังบันทึกโมเดลไปยัง: " + m_environment.modelPath);
}

void Interpreter::handleShowCommand(const std::map<std::string, std::string>& params) {
    // ตรวจสอบว่ามีพารามิเตอร์ที่จำเป็นหรือไม่
    if (params.find("target") == params.end()) {
        m_errorHandler("ต้องระบุเป้าหมายที่ต้องการแสดง");
        return;
    }

    std::string target = params.at("target");

    if (target == "metric") {
        // ตรวจสอบว่าได้ประเมินผลโมเดลแล้วหรือไม่
        if (!m_environment.modelEvaluated) {
            m_errorHandler("ต้องประเมินผลโมเดลก่อนที่จะแสดงเมทริกซ์");
            return;
        }

        // ตรวจสอบว่ามีพารามิเตอร์ที่จำเป็นหรือไม่
        if (params.find("metric") == params.end()) {
            m_errorHandler("ต้องระบุชื่อเมทริก");
            return;
        }

        std::string metricName = params.at("metric");
        m_outputHandler("กำลังแสดงเมตริก: " + metricName);

        if (m_environment.metrics.find(metricName) != m_environment.metrics.end()) {
            m_outputHandler(metricName + " = " + std::to_string(m_environment.metrics[metricName]));
        } else {
            m_errorHandler("ไม่พบเมทริก: " + metricName);
        }
    } else if (target == "model") {
        // ตรวจสอบว่าได้ฝึกโมเดลแล้วหรือไม่
        if (!m_environment.modelTrained) {
            m_errorHandler("ต้องฝึกโมเดลก่อนที่จะแสดง");
            return;
        }

        m_outputHandler("กำลังแสดงข้อมูลโมเดล");
        m_outputHandler("  ประเภท: " + m_environment.projectType);

        if (m_environment.modelEvaluated) {
            m_outputHandler("  ประสิทธิภาพ:");
            for (const auto& pair : m_environment.metrics) {
                m_outputHandler("    " + pair.first + " = " + std::to_string(pair.second));
            }
        }
    } else {
        m_errorHandler("ไม่รู้จักเป้าหมาย: " + target);
    }
}

void Interpreter::handleVisualizeCommand(const std::map<std::string, std::string>& params) {
    // ตรวจสอบว่ามีพารามิเตอร์ที่จำเป็นหรือไม่
    if (params.find("target") == params.end()) {
        m_errorHandler("ต้องระบุเป้าหมายที่ต้องการแสดงภาพ");
        return;
    }

    std::string target = params.at("target");

    if (target == "data") {
        // ตรวจสอบว่าได้โหลดข้อมูลแล้วหรือไม่
        if (!m_environment.dataLoaded) {
            m_errorHandler("ต้องโหลดข้อมูลก่อนที่จะแสดงภาพ");
            return;
        }

        m_outputHandler("กำลังแสดงภาพข้อมูล");
    } else if (target == "model") {
        // ตรวจสอบว่าได้ฝึกโมเดลแล้วหรือไม่
        if (!m_environment.modelTrained) {
            m_errorHandler("ต้องฝึกโมเดลก่อนที่จะแสดงภาพ");
            return;
        }

        m_outputHandler("กำลังแสดงภาพโมเดล");
    } else {
        m_errorHandler("ไม่รู้จักเป้าหมาย: " + target);
    }
}

void Interpreter::handlePlotCommand(const std::map<std::string, std::string>& params) {
    // ตรวจสอบว่ามีพารามิเตอร์ที่จำเป็นหรือไม่
    if (params.find("target") == params.end()) {
        m_errorHandler("ต้องระบุเป้าหมายที่ต้องการพล็อต");
        return;
    }

    std::string target = params.at("target");

    if (target == "data") {
        // ตรวจสอบว่าได้โหลดข้อมูลแล้วหรือไม่
        if (!m_environment.dataLoaded) {
            m_errorHandler("ต้องโหลดข้อมูลก่อนที่จะพล็อต");
            return;
        }

        m_outputHandler("กำลังพล็อตข้อมูล");
    } else if (target == "result") {
        // ตรวจสอบว่าได้ฝึกโมเดลแล้วหรือไม่
        if (!m_environment.modelTrained) {
            m_errorHandler("ต้องฝึกโมเดลก่อนที่จะพล็อตผลลัพธ์");
            return;
        }

        m_outputHandler("กำลังพล็อตผลลัพธ์");
    } else {
        m_errorHandler("ไม่รู้จักเป้าหมาย: " + target);
    }
}

bool ai_language::Environment::isCompleteStatement(const std::string& statement) const {
    // Basic implementation:
    // Check if the statement is empty or just whitespace
    if (statement.empty() || std::all_of(statement.begin(), statement.end(), [](char c) { return std::isspace(c); })) {
        return false;
    }

    // Count quotes to make sure they're paired
    int quotes = 0;
    for (char c : statement) {
        if (c == '"') {
            quotes++;
        }
    }

    // If odd number of quotes, statement is incomplete
    if (quotes % 2 != 0) {
        return false;
    }

    // Add more complex checks as needed
    // For multi-line statements, block structures, etc.

    return true;
}

} // namespace ai_language