
#include "../../include/interpreters/InterpreterFactory.h"
#include "../../include/interpreters/MLInterpreter.h"
#include "../../include/interpreters/DLInterpreter.h"
#include "../../include/interpreters/RLInterpreter.h"
#include <fstream>
#include <regex>

namespace ai_language {

std::unique_ptr<BaseInterpreter> InterpreterFactory::createInterpreter(const std::string& type) {
    if (type == "ML") {
        return std::make_unique<MLInterpreter>();
    } else if (type == "DL") {
        return std::make_unique<DLInterpreter>();
    } else if (type == "RL") {
        return std::make_unique<RLInterpreter>();
    } else {
        // หากไม่ระบุประเภทหรือประเภทไม่ถูกต้อง ใช้ ML เป็นค่าเริ่มต้น
        return std::make_unique<MLInterpreter>();
    }
}

std::unique_ptr<BaseInterpreter> InterpreterFactory::createInterpreterFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ข้อผิดพลาด: ไม่สามารถเปิดไฟล์ " << filename << std::endl;
        return createInterpreter("ML"); // ใช้ ML เป็นค่าเริ่มต้น
    }

    std::string line;
    std::regex mlPattern("create\\s+ML");
    std::regex dlPattern("create\\s+DL");
    std::regex rlPattern("create\\s+RL");

    while (std::getline(file, line)) {
        // ข้ามบรรทัดว่างและคอมเมนต์
        if (line.empty() || line[0] == '#') {
            continue;
        }

        if (std::regex_search(line, mlPattern)) {
            return createInterpreter("ML");
        } else if (std::regex_search(line, dlPattern)) {
            return createInterpreter("DL");
        } else if (std::regex_search(line, rlPattern)) {
            return createInterpreter("RL");
        }
    }

    // หากไม่พบประเภทที่ระบุในไฟล์ ใช้ ML เป็นค่าเริ่มต้น
    return createInterpreter("ML");
}

} // namespace ai_language
