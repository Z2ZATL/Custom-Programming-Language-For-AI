#include "../../include/interpreters/BaseInterpreter.h"
#include "../../include/interpreters/InterpreterFactory.h"
// ... rest of the code (assuming it exists and needs appropriate include path adjustments) ...
#include "../../include/interpreters/BaseInterpreter.h"
#include "../../include/interpreters/InterpreterFactory.h"
#include <fstream>
#include <sstream>

namespace ai_language {

std::string BaseInterpreter::getCurrentDateTime() const {
    // Set timezone to Thailand (UTC+7)
    setenv("TZ", "Asia/Bangkok", 1);
    tzset();

    // Get current time
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    // Add timezone offset
    time += userTimezoneOffset * 3600;

    // Format time with locale settings
    std::tm* tm_info = localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm_info, "%a %b %d %H:%M:%S %Y");
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

    // สร้างรายการคำสั่งแบบจัดการข้อมูลดิบเพื่อให้ใช้กับคำสั่งบางตัวที่ต้องการข้อมูลเต็ม
    std::vector<std::string> parts;
    std::istringstream fullLineStream(line);
    std::string part;
    while (fullLineStream >> part) {
        parts.push_back(part);
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
    } else if (command == "plot") {
        handlePlotCommand(parts);
    } else if (command == "inspect") {
        if (parts.size() > 1 && parts[1] == "model") {
            std::vector<std::string> inspectArgs(parts.begin() + 2, parts.end());
            handleInspectCommand(inspectArgs);
        } else {
            std::cout << "Error: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'inspect model <model_name> <option>'" << std::endl;
        }
    } else if (command == "validate") {
        if (parts.size() > 1 && parts[1] == "model") {
            std::vector<std::string> validateArgs(parts.begin() + 2, parts.end());
            handleValidateCommand(validateArgs);
        } else {
            std::cout << "Error: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'validate model <dataset_path>'" << std::endl;
        }
    } else if (command == "preprocess") {
        if (parts.size() > 1 && parts[1] == "data") {
            std::vector<std::string> preprocessArgs(parts.begin() + 2, parts.end());
            handlePreprocessCommand(preprocessArgs);
        } else {
            std::cout << "Error: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'preprocess data <operation>'" << std::endl;
        }
    } else if (command == "split") {
        if (parts.size() > 1 && parts[1] == "dataset") {
            std::vector<std::string> splitArgs(parts.begin() + 2, parts.end());
            handleSplitDatasetCommand(splitArgs);
        } else {
            std::cout << "Error: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'split dataset <train_ratio> <test_ratio>'" << std::endl;
        }
    } else if (command == "predict") {
        std::vector<std::string> predictArgs(parts.begin() + 1, parts.end());
        handlePredictCommand(predictArgs);
    } else if (command == "list" && parts.size() > 1 && parts[1] == "models") {
        handleListModelsCommand();
    } else if (command == "delete") {
        if (parts.size() > 1 && parts[1] == "model") {
            std::vector<std::string> deleteArgs(parts.begin() + 2, parts.end());
            handleDeleteModelCommand(deleteArgs);
        } else {
            std::cout << "Error: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'delete model <model_name>'" << std::endl;
        }
    } else if (command == "compare" && parts.size() > 1 && parts[1] == "models") {
        handleCompareModelsCommand();
    } else if (command == "check" && parts.size() > 1 && parts[1] == "status") {
        handleCheckStatusCommand();
    } else if (command == "debug") {
        std::vector<std::string> debugArgs(parts.begin() + 1, parts.end());
        handleDebugCommand(debugArgs);
    } else if (command == "cross_validate") {
        std::vector<std::string> crossValidateArgs(parts.begin() + 1, parts.end());
        handleCrossValidateCommand(crossValidateArgs);
    } else if (command == "export") {
        if (parts.size() > 1 && parts[1] == "results") {
            std::vector<std::string> exportArgs(parts.begin() + 2, parts.end());
            handleExportResultsCommand(exportArgs);
        } else {
            std::cout << "Error: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'export results \"<filename>\"'" << std::endl;
        }
    } else if (command == "schedule") {
        if (parts.size() > 1 && parts[1] == "training") {
            std::vector<std::string> scheduleArgs(parts.begin() + 2, parts.end());
            handleScheduleTrainingCommand(scheduleArgs);
        } else {
            std::cout << "Error: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'schedule training \"<time>\"'" << std::endl;
        }
    } else {
        std::cout << RED << "ไม่รู้จักคำสั่ง: " << command << RESET << std::endl;
    }
}
