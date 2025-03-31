
#include "../include/BaseInterpreter.h"
#include "../include/InterpreterFactory.h"
#include <sstream>

namespace ai_language {

std::string BaseInterpreter::getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);

    // ปรับเวลาตาม timezone ของผู้ใช้
    time_t_now += userTimezoneOffset * 3600; // ปรับเวลาตาม offset (ชั่วโมง)

    std::stringstream ss;
    ss << std::put_time(std::gmtime(&time_t_now), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void BaseInterpreter::interpretFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << RED << "ข้อผิดพลาด: ไม่สามารถเปิดไฟล์ " << filename << RESET << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        // ข้ามบรรทัดว่างและคอมเมนต์
        if (line.empty() || line[0] == '#') {
            continue;
        }

        interpretLine(line);
    }
}

void BaseInterpreter::interpretLine(const std::string& line) {
    // แยกคำสั่งและพารามิเตอร์
    std::vector<std::string> tokens;
    std::string currentToken;
    bool inQuotes = false;

    for (char c : line) {
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

    if (tokens.empty()) {
        return;
    }

    // ประมวลผลคำสั่ง
    std::string command = tokens[0];

    std::vector<std::string> args;
    for (size_t i = 1; i < tokens.size(); ++i) {
        args.push_back(tokens[i]);
    }

    if (command == "start") {
        handleStartCommand();
    } else if (command == "create") {
        handleCreateCommand(args);
    } else if (command == "load") {
        handleLoadCommand(args);
    } else if (command == "set") {
        handleSetCommand(args);
    } else if (command == "train") {
        handleTrainCommand(args);
    } else if (command == "show") {
        handleShowCommand(args);
    } else if (command == "save") {
        handleSaveCommand(args);
    } else if (command == "help") {
        handleHelpCommand();
    } else if (command == "end") {
        std::cout << GREEN << "Program ended" << RESET << std::endl;
    } else {
        // คำสั่งไม่รองรับ
        std::cerr << RED << "ข้อผิดพลาด: คำสั่ง '" << command << "' ไม่รองรับ" << RESET << std::endl;
        std::cerr << YELLOW << "ลองใช้คำสั่ง 'help' เพื่อดูคำสั่งที่ใช้ได้" << RESET << std::endl;
    }
}

} // namespace ai_language
