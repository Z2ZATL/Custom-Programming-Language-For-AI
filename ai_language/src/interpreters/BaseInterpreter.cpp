#include "../../include/interpreters/BaseInterpreter.h"
#include "../../include/interpreters/InterpreterFactory.h"
// ... rest of the code (assuming it exists and needs appropriate include path adjustments) ...
#include "../../include/interpreters/BaseInterpreter.h"
#include "../../include/interpreters/InterpreterFactory.h"
#include <fstream>
#include <sstream>

namespace ai_language {

std::string BaseInterpreter::getCurrentDateTime() const {
    // Get current time
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    // Add timezone offset
    time += userTimezoneOffset * 3600;

    // Format time
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void BaseInterpreter::interpretFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << RED << "ไม่สามารถเปิดไฟล์: " << filename << RESET << std::endl;
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
    // ข้ามบรรทัดว่างและคอมเมนต์
    if (line.empty() || line[0] == '#') {
        return;
    }

    // แยกคำสั่งและพารามิเตอร์
    std::istringstream iss(line);
    std::string command;
    iss >> command;

    std::vector<std::string> args;
    std::string arg;
    while (iss >> arg) {
        args.push_back(arg);
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
    } else if (command == "evaluate") {
        handleEvaluateCommand(args);
    } else if (command == "show") {
        handleShowCommand(args);
    } else if (command == "save") {
        handleSaveCommand(args);
    } else if (command == "help") {
        handleHelpCommand();
    } else if (command == "add") {
        handleAddCommand(args);
    } else {
        std::cout << RED << "ไม่รู้จักคำสั่ง: " << command << RESET << std::endl;
    }
}

} // namespace ai_language