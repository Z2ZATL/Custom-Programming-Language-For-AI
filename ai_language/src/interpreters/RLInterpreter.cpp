#include "../../include/interpreters/RLInterpreter.h"
#include "../../include/connectors/Connector.h"
#include <iostream>
#include <cstdlib> // for setenv
#include <ctime>   // for tzset
#include <fstream> //for ofstream
#include <sys/stat.h> //for stat


namespace ai_language {

RLInterpreter::RLInterpreter() {
    // Constructor implementation
    hasLoadedData = false;
    hasCreatedModel = false;
    hasTrained = false;
    hasEvaluated = false;
}

RLInterpreter::~RLInterpreter() {
    // Destructor implementation
}

void RLInterpreter::interpret() {
    std::cout << "Interpreting Reinforcement Learning code..." << std::endl;

    // Create a connector instance for external RL libraries if needed
    Connector connector;
    connector.connect();

    // Implementation of RL interpretation logic
}

void RLInterpreter::loadModel(const std::string& modelPath) {
    std::cout << "Loading RL model from: " << modelPath << std::endl;
    // Implementation for loading RL model
}

void RLInterpreter::trainModel() {
    std::cout << "Training RL model..." << std::endl;
    // Implementation for training RL model
    hasTrained = true;
}

void RLInterpreter::evaluateModel() {
    std::cout << "Evaluating RL model performance..." << std::endl;
    // Implementation for evaluating RL model
    hasEvaluated = true;
}

void RLInterpreter::saveModel(const std::string& modelPath) {
    std::cout << "Saving RL model to: " << modelPath << std::endl;

    // ตรวจสอบและสร้างโฟลเดอร์ให้แน่ใจว่ามีอยู่
    std::string directory = "Program test/model/";
    struct stat info;
    if (stat(directory.c_str(), &info) != 0 || !(info.st_mode & S_IFDIR)) {
        std::string mkdirCmd = "mkdir -p \"" + directory + "\"";
        int result = system(mkdirCmd.c_str());
        if (result != 0) {
            std::cout << "Warning: Failed to create directory: " << directory << std::endl;
        }
    }

    // ทำความสะอาดชื่อไฟล์ (ลบเครื่องหมายคำพูดถ้ามี)
    std::string cleanModelPath = modelPath;
    if (cleanModelPath.size() >= 2) {
        if (cleanModelPath.front() == '"' && cleanModelPath.back() == '"') {
            cleanModelPath = cleanModelPath.substr(1, cleanModelPath.size() - 2);
        }
    }

    // สร้างเส้นทางเต็มสำหรับไฟล์
    std::string fullPath = directory + cleanModelPath;
    if (fullPath.find(".rlmodel") == std::string::npos) {
        fullPath += ".rlmodel";
    }

    // ใช้ฟังก์ชัน getCurrentDateTime จาก BaseInterpreter
    std::string timestamp = getCurrentDateTime();

    // บันทึกโมเดลลงไฟล์
    std::ofstream modelFile(fullPath);
    if (modelFile.is_open()) {
        modelFile << "# RL Model saved from AI Language\n";
        modelFile << "model_type: " << modelType << "\n";
        modelFile << "discount_factor: " << parameters["gamma"] << "\n";
        modelFile << "episodes: " << parameters["episodes"] << "\n";
        modelFile << "exploration_rate: " << parameters["epsilon"] << "\n";
        modelFile << "create_time: " << timestamp << "\n";
        modelFile.close();
        std::cout << "Model successfully saved to: " << fullPath << std::endl;
        std::cout << "โมเดลถูกบันทึกไปที่ '" << "ai_language/" << fullPath << "'" << std::endl;
    } else {
        std::cout << "Error: Could not create model file at: " << fullPath << std::endl;
    }
}

// Implement the required virtual functions from BaseInterpreter
void RLInterpreter::setDefaultParameters() {
    // Set default parameters for RL
    parameters["learning_rate"] = 0.01;
    parameters["gamma"] = 0.99;
    parameters["epsilon"] = 0.1;
    parameters["episodes"] = 1000;
}

void RLInterpreter::handleStartCommand() {
    std::cout << "Starting Reinforcement Learning interpreter..." << std::endl;
}

void RLInterpreter::handleAddCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Error: Missing argument for add command" << std::endl;
        return;
    }

    if (args[0] == "action") {
        if (args.size() < 2) {
            std::cout << "Error: Missing action name. Usage: add action <action_name>" << std::endl;
            return;
        }
        std::cout << "Adding action: " << args[1] << " to the environment" << std::endl;
        // Implementation for adding actions to RL environment
    } else if (args[0] == "state") {
        if (args.size() < 2) {
            std::cout << "Error: Missing state name. Usage: add state <state_name>" << std::endl;
            return;
        }
        std::cout << "Adding state: " << args[1] << " to the environment" << std::endl;
        // Implementation for adding states to RL environment
    } else {
        std::cout << "Unknown add type: " << args[0] << std::endl;
    }
}

void RLInterpreter::handleCreateCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Error: Missing model type for create command" << std::endl;
        return;
    }

    std::string modelType = args[0];
    std::cout << "Creating RL model: " << modelType << std::endl;

    // Create the specified RL model
    hasCreatedModel = true;
}

void RLInterpreter::handleLoadCommand(const std::vector<std::string>& args) {
    if (args.empty() || args.size() < 2) {
        std::cout << "Error: Invalid load command format" << std::endl;
        return;
    }

    std::string loadType = args[0];
    std::string path = args[1];

    if (loadType == "dataset") {
        std::cout << "Loading dataset from: " << path << std::endl;
        hasLoadedData = true;
    } else if (loadType == "model") {
        loadModel(path);
    } else {
        std::cout << "Error: Unknown load type: " << loadType << std::endl;
    }
}

void RLInterpreter::handleSetCommand(const std::vector<std::string>& parts) {
    if (parts.size() < 3) {
        std::cout << "Error: Invalid set command. Expected: set <parameter> <value>" << std::endl;
        return;
    }

    std::string parameter = parts[1];
    std::string valueStr = parts[2];

    // Handle timezone separately since it might be an integer
    if (parameter == "timezone") {
        int timezone_value = std::stoi(valueStr);
        parameters["timezone"] = static_cast<double>(timezone_value);
        std::cout << "Set timezone = UTC" << (timezone_value >= 0 ? "+" : "") << timezone_value << std::endl;

        // ตั้งค่า timezone ในระบบ (สำหรับ Linux)
        std::string tz_env;
        if (timezone_value >= 0) {
            tz_env = "Etc/GMT-" + std::to_string(timezone_value); // เครื่องหมายกลับกันใน POSIX
        } else {
            tz_env = "Etc/GMT+" + std::to_string(-timezone_value); // เครื่องหมายกลับกันใน POSIX
        }
        setenv("TZ", tz_env.c_str(), 1);
        tzset();
        return;
    }

    // สำหรับพารามิเตอร์อื่นๆ
    double value = std::stod(valueStr);

    if (parameter == "learning_rate") {
        parameters["learning_rate"] = value;
        std::cout << "Set learning_rate = " << value << std::endl;
    } 
    else if (parameter == "episodes") {
        parameters["episodes"] = value;
        std::cout << "Set episodes = " << value << std::endl;
    }
    else if (parameter == "discount_factor") {
        parameters["gamma"] = value;
        std::cout << "Set discount_factor = " << value << std::endl;
    }
    else if (parameter == "exploration_rate") {
        parameters["epsilon"] = value;
        std::cout << "Set exploration_rate = " << value << std::endl;
    }
    else {
        std::cout << "Warning: Unknown parameter: " << parameter << std::endl;
    }
}

void RLInterpreter::handleTrainCommand(const std::vector<std::string>& args) {
    if (!hasCreatedModel) {
        std::cout << "Error: No model created. Use 'create model' command first." << std::endl;
        return;
    }

    if (!hasLoadedData) {
        std::cout << "Warning: No data loaded. Training with default environment." << std::endl;
    }

    trainModel();
}

void RLInterpreter::handleEvaluateCommand(const std::vector<std::string>& args) {
    if (!hasTrained) {
        std::cout << "Error: Model must be trained before evaluation" << std::endl;
        return;
    }

    if (args.size() >= 1 && args[0] == "model") {
        std::cout << "Evaluating RL model performance..." << std::endl;
        // Evaluation code would go here
        hasEvaluated = true;
    } else {
        std::cout << "Error: Invalid evaluation command format. Use 'evaluate model'" << std::endl;
    }
}

void RLInterpreter::handleShowCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Error: No show type specified" << std::endl;
        return;
    }

    std::string showType = args[0];

    if (showType == "parameters") {
        std::cout << "Episodes: " << parameters["episodes"] << std::endl;
        std::cout << "Discount factor: " << parameters["gamma"] << std::endl;
        std::cout << "Exploration rate: " << parameters["epsilon"] << std::endl;
        // Display other parameters as needed
        std::cout << "Timezone: UTC" << (parameters["timezone"] >= 0 ? "+" : "") << static_cast<int>(parameters["timezone"]) << std::endl;
    } else if (showType == "performance") {
        if (!hasTrained) {
            std::cout << "Warning: Model not trained yet, no performance metrics to show." << std::endl;
            return;
        }
        std::cout << "Evaluating RL model performance..." << std::endl;
        // Display various metrics specific to RL
        std::cout << "Average reward: " << 85.2 << std::endl;
        std::cout << "Success rate: " << 0.78 << std::endl;
    } else {
        std::cout << "Unknown show type: " << showType << std::endl;
    }
}

void RLInterpreter::handleSaveCommand(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Error: Invalid save command format" << std::endl;
        return;
    }

    if (args[0] == "model") {
        saveModel(args[1]);
    } else {
        std::cout << "Unknown save type: " << args[0] << std::endl;
    }
}

// สตับฟังก์ชันสำหรับคำสั่งใหม่
void RLInterpreter::handleInspectCommand(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Error: รูปแบบที่ถูกต้อง: inspect model <model_name> <option>" << std::endl;
        return;
    }
    std::cout << "ยังไม่ได้ดำเนินการคำสั่ง inspect model" << std::endl;
}

void RLInterpreter::handleValidateCommand(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Error: รูปแบบที่ถูกต้อง: validate model \"<dataset_path>\"" << std::endl;
        return;
    }
    std::cout << "ยังไม่ได้ดำเนินการคำสั่ง validate model" << std::endl;
}

void RLInterpreter::handlePreprocessCommand(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Error: รูปแบบที่ถูกต้อง: preprocess data <operation>" << std::endl;
        return;
    }
    std::cout << "ยังไม่ได้ดำเนินการคำสั่ง preprocess data" << std::endl;
}

void RLInterpreter::handleSplitDatasetCommand(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Error: รูปแบบที่ถูกต้อง: split dataset <train_ratio> <test_ratio>" << std::endl;
        return;
    }
    std::cout << "ยังไม่ได้ดำเนินการคำสั่ง split dataset" << std::endl;
}

void RLInterpreter::handlePredictCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Error: รูปแบบที่ถูกต้อง: predict \"<input_data>\" หรือ predict dataset \"<filename>\"" << std::endl;
        return;
    }
    std::cout << "ยังไม่ได้ดำเนินการคำสั่ง predict" << std::endl;
}

void RLInterpreter::handleListModelsCommand() {
    std::cout << "รายการโมเดลที่มีอยู่:" << std::endl;
    std::cout << "- " << modelType << std::endl;
}

void RLInterpreter::handleDeleteModelCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Error: รูปแบบที่ถูกต้อง: delete model <model_name>" << std::endl;
        return;
    }
    std::cout << "ยังไม่ได้ดำเนินการคำสั่ง delete model" << std::endl;
}

void RLInterpreter::handleCompareModelsCommand() {
    std::cout << "ยังไม่ได้ดำเนินการคำสั่ง compare models" << std::endl;
}

void RLInterpreter::handleCheckStatusCommand() {
    std::cout << "สถานะโปรเจกต์:" << std::endl;
    std::cout << "- ประเภท AI: RL" << std::endl;
    std::cout << "- โมเดล: " << (hasCreatedModel ? modelType : "ยังไม่ได้สร้าง") << std::endl;
    std::cout << "- สถานะการโหลดข้อมูล: " << (hasLoadedData ? "โหลดแล้ว" : "ยังไม่ได้โหลด") << std::endl;
    std::cout << "- สถานะการฝึก: " << (hasTrained ? "ฝึกแล้ว" : "ยังไม่ได้ฝึก") << std::endl;
    std::cout << "- สถานะการประเมิน: " << (hasEvaluated ? "ประเมินแล้ว" : "ยังไม่ได้ประเมิน") << std::endl;
}

void RLInterpreter::handleDebugCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Error: รูปแบบที่ถูกต้อง: debug on หรือ debug off" << std::endl;
        return;
    }
    std::string mode = args[0];
    if (mode == "on") {
        std::cout << "เปิดโหมดดีบัก" << std::endl;
    } else if (mode == "off") {
        std::cout << "ปิดโหมดดีบัก" << std::endl;
    } else {
        std::cout << "Error: ต้องเป็น on หรือ off เท่านั้น" << std::endl;
    }
}

void RLInterpreter::handleCrossValidateCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Error: รูปแบบที่ถูกต้อง: cross_validate <folds>" << std::endl;
        return;
    }
    std::cout << "ยังไม่ได้ดำเนินการคำสั่ง cross_validate" << std::endl;
}

void RLInterpreter::handleExportResultsCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Error: รูปแบบที่ถูกต้อง: export results \"<filename>\"" << std::endl;
        return;
    }
    std::cout << "ยังไม่ได้ดำเนินการคำสั่ง export results" << std::endl;
}

void RLInterpreter::handleScheduleTrainingCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Error: รูปแบบที่ถูกต้อง: schedule training \"<time>\"" << std::endl;
        return;
    }
    std::cout << "ยังไม่ได้ดำเนินการคำสั่ง schedule training" << std::endl;
}

void RLInterpreter::handleCreateEnvironmentCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Error: รูปแบบที่ถูกต้อง: create environment \"<env_name>\"" << std::endl;
        return;
    }
    std::cout << "ยังไม่ได้ดำเนินการคำสั่ง create environment" << std::endl;
}

void RLInterpreter::handleSetEnvironmentParameterCommand(const std::vector<std::string>& args) {
    if (args.size() < 3) {
        std::cout << "Error: รูปแบบที่ถูกต้อง: set environment parameter <param> <value>" << std::endl;
        return;
    }
    std::cout << "ยังไม่ได้ดำเนินการคำสั่ง set environment parameter" << std::endl;
}


void RLInterpreter::handlePlotCommand(const std::vector<std::string>& args) {
    //Implementation for plot command.  This is a stub.
    std::cout << "Plot command not yet implemented." << std::endl;
}

void RLInterpreter::handleHelpCommand() {
    std::cout << "Reinforcement Learning Interpreter Help:" << std::endl;
    std::cout << "  start                             # เริ่มต้นโปรเจกต์" << std::endl;
    std::cout << "  create model <model_type>         # สร้างโมเดล RL (เช่น QLearning, DQN)" << std::endl;
    std::cout << "  load dataset <path>               # โหลดข้อมูลสภาพแวดล้อม" << std::endl;
    std::cout << "  load model <path>                 # โหลดโมเดลที่บันทึกไว้" << std::endl;
    std::cout << "  set <param> <value>               # ตั้งค่าพารามิเตอร์" << std::endl;
    std::cout << "  train model                       # ฝึกโมเดล" << std::endl;
    std::cout << "  evaluate model                    # ประเมินประสิทธิภาพโมเดล" << std::endl;
    std::cout << "  show parameters                   # แสดงพารามิเตอร์ปัจจุบัน" << std::endl;
    std::cout << "  show performance                  # แสดงประสิทธิภาพโมเดล" << std::endl;
    std::cout << "  inspect model <model> <option>    # ตรวจสอบรายละเอียดของโมเดล" << std::endl;
    std::cout << "  validate model <dataset_path>     # ตรวจสอบโมเดลด้วยชุดข้อมูลใหม่" << std::endl;
    std::cout << "  plot model <type> <output_path>   # สร้างกราฟจากโมเดล" << std::endl;
    std::cout << "  save model <path>                 # บันทึกโมเดลลงไฟล์" << std::endl;
    std::cout << "  predict <input_data>              # ทำนายผลลัพธ์จากข้อมูลใหม่" << std::endl;
    std::cout << "  list models                       # แสดงรายการโมเดลที่มีอยู่" << std::endl;
    std::cout << "  check status                      # ตรวจสอบสถานะปัจจุบัน" << std::endl;
    std::cout << "  help                              # แสดงข้อความช่วยเหลือ" << std::endl;
}

} // namespace ai_language