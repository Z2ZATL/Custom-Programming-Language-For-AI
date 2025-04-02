#include "../include/RLInterpreter.h"
#include <random>
#include <iomanip>
#include <fstream>
#include <thread>
#include <chrono>
#include <sys/stat.h>
#include <sstream>

namespace ai_language {

RLInterpreter::RLInterpreter() {
    projectType = "RL";
}

RLInterpreter::~RLInterpreter() {
}

void RLInterpreter::setDefaultParameters() {
    // ล้างค่าเดิม
    parameters.clear();

    // กำหนดค่าเริ่มต้นสำหรับ RL
    parameters["learning_rate"] = 0.1;
    parameters["discount_factor"] = 0.9;
    parameters["exploration_rate"] = 0.1;
    parameters["episodes"] = 1000;
}

void RLInterpreter::handleStartCommand() {
    hasStarted = true;
    std::cout << GREEN << "Program started" << RESET << std::endl;
}

void RLInterpreter::handleCreateCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (args.size() < 1) {
        std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'create [type]' หรือ 'create model [model_name]'" << RESET << std::endl;
        return;
    }

    if (args[0] == "RL") {
        // สร้างโปรเจค
        if (hasCreatedProject) {
            std::cerr << YELLOW << "คำเตือน: กำลังสร้างโปรเจคใหม่ ข้อมูลเดิมจะถูกรีเซ็ต" << RESET << std::endl;
        }

        projectType = "RL";
        hasCreatedProject = true;
        hasLoadedData = false;
        hasCreatedModel = false;
        hasTrainedModel = false;
        hasSavedModel = false;

        // ตั้งค่าพารามิเตอร์เริ่มต้น
        setDefaultParameters();

        std::cout << GREEN << "Project created: Reinforcement Learning" << RESET << std::endl;
    } else if (args[0] == "model") {
        if (!hasCreatedProject) {
            std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create RL' ก่อน" << RESET << std::endl;
            return;
        }

        if (!hasLoadedData) {
            std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'load dataset' หรือ 'load environment' ก่อน" << RESET << std::endl;
            return;
        }

        if (args.size() < 2) {
            std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'create model [model_name]'" << RESET << std::endl;
            return;
        }

        handleModelCreation(args[1]);
    } else {
        std::cerr << RED << "ข้อผิดพลาด: ประเภท '" << args[0] << "' ไม่ถูกต้อง ต้องเป็น 'RL' หรือ 'model'" << RESET << std::endl;
    }
}

void RLInterpreter::handleModelCreation(const std::string& modelName) {
    if (hasCreatedModel) {
        std::cerr << YELLOW << "คำเตือน: กำลังแทนที่โมเดลเดิม (" << modelType << ") ด้วยโมเดลใหม่" << RESET << std::endl;
    }
    modelType = modelName;

    // ตรวจสอบชื่อโมเดลที่มีวงเล็บหรือเครื่องหมายพิเศษ
    if (modelName.find('(') != std::string::npos || modelName.find(')') != std::string::npos ||
        modelName.find('!') != std::string::npos || modelName.find('@') != std::string::npos ||
        modelName.find('#') != std::string::npos || modelName.find('$') != std::string::npos) {
        std::cerr << RED << "ข้อผิดพลาด: ชื่อโมเดลมีเครื่องหมายพิเศษที่ไม่รองรับ" << RESET << std::endl;
        return;
    }

    // ตรวจสอบว่ารองรับโมเดลหรือไม่
    if (modelName != "QLearning" && modelName != "SARSA" && modelName != "DQN" &&
        modelName != "A3C" && modelName != "PPO" && modelName != "DDPG") {
        std::cerr << RED << "ข้อผิดพลาด: ไม่รองรับโมเดล '" << modelName << "' สำหรับ Reinforcement Learning" << RESET << std::endl;
        std::cerr << RED << "โมเดลที่รองรับ: QLearning, SARSA, DQN, A3C, PPO, DDPG" << RESET << std::endl;
        return;
    }

    hasCreatedModel = true;
    hasTrainedModel = false;

    std::cout << GREEN << "Model created: " << modelType << RESET << std::endl;
}

void RLInterpreter::handleSetCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedProject) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create RL' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedModel) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create model' ก่อน" << RESET << std::endl;
        return;
    }

    if (args.size() < 2) {
        std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'set [parameter] [value]'" << RESET << std::endl;
        return;
    }

    std::string parameter = args[0];
    std::string valueStr = args[1];

    // ตรวจสอบชนิดพารามิเตอร์
    if (parameter == "learning_rate" || parameter == "discount_factor" ||
        parameter == "exploration_rate" || parameter == "episodes") {
        try {
            double value = std::stod(valueStr);

            // ตรวจสอบขอบเขตค่าพารามิเตอร์
            if (parameter == "learning_rate" && (value <= 0.0 || value > 1.0)) {
                std::cerr << RED << "ข้อผิดพลาด: learning_rate ต้องอยู่ในช่วง (0, 1]" << RESET << std::endl;
                return;
            } else if (parameter == "discount_factor" && (value < 0.0 || value > 1.0)) {
                std::cerr << RED << "ข้อผิดพลาด: discount_factor ต้องอยู่ในช่วง [0, 1]" << RESET << std::endl;
                return;
            } else if (parameter == "exploration_rate" && (value < 0.0 || value > 1.0)) {
                std::cerr << RED << "ข้อผิดพลาด: exploration_rate ต้องอยู่ในช่วง [0, 1]" << RESET << std::endl;
                return;
            } else if (parameter == "episodes" && (value <= 0 || value > 100000)) {
                std::cerr << RED << "ข้อผิดพลาด: episodes ต้องอยู่ในช่วง (0, 100000]" << RESET << std::endl;
                return;
            }

            parameters[parameter] = value;
            std::cout << GREEN << "Parameter set: " << parameter << " = " << value << RESET << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cerr << RED << "ข้อผิดพลาด: ค่า '" << valueStr << "' ไม่ใช่ตัวเลข" << RESET << std::endl;
        }
    } else if (parameter == "algorithm") {
        if (valueStr == "q_learning" || valueStr == "Q-Learning") {
            parameters["algorithm"] = 0;
        } else if (valueStr == "sarsa" || valueStr == "SARSA") {
            parameters["algorithm"] = 1;
        } else if (valueStr == "monte_carlo" || valueStr == "Monte Carlo") {
            parameters["algorithm"] = 2;
        } else {
            std::cerr << RED << "ข้อผิดพลาด: algorithm ไม่รองรับ '" << valueStr << "'" << RESET << std::endl;
            std::cerr << RED << "algorithm ที่รองรับ: Q-Learning, SARSA, Monte Carlo" << RESET << std::endl;
            return;
        }
        std::cout << GREEN << "Parameter set: " << parameter << " = " << valueStr << RESET << std::endl;
    } else if (parameter == "policy") {
        if (valueStr == "epsilon_greedy" || valueStr == "Epsilon-Greedy") {
            parameters["policy"] = 0;
        } else if (valueStr == "softmax" || valueStr == "Softmax") {
            parameters["policy"] = 1;
        } else if (valueStr == "ucb" || valueStr == "UCB") {
            parameters["policy"] = 2;
        } else {
            std::cerr << RED << "ข้อผิดพลาด: policy ไม่รองรับ '" << valueStr << "'" << RESET << std::endl;
            std::cerr << RED << "policy ที่รองรับ: Epsilon-Greedy, Softmax, UCB" << RESET << std::endl;
            return;
        }
        std::cout << GREEN << "Parameter set: " << parameter << " = " << valueStr << RESET << std::endl;
    } else {
        std::cerr << RED << "ข้อผิดพลาด: พารามิเตอร์ '" << parameter << "' ไม่รองรับสำหรับโมเดล " << modelType << RESET << std::endl;
    }
}

void RLInterpreter::handleLoadCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedProject) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create RL' ก่อน" << RESET << std::endl;
        return;
    }

    if (args.size() < 1) {
        std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'load dataset \"[filename]\"', 'load environment \"[filename]\"' หรือ 'load model \"[filename]\"'" << RESET << std::endl;
        return;
    }

    if (args[0] == "dataset" || args[0] == "environment") {
        if (args.size() < 2) {
            std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'load " << args[0] << " \"[filename]\"'" << RESET << std::endl;
            return;
        }

        std::string filename = args[1];
        // ลบเครื่องหมายคำพูด
        if (filename.front() == '"' && filename.back() == '"') {
            filename = filename.substr(1, filename.length() - 2);
        }

        // ตรวจสอบว่าไฟล์มีอยู่จริงหรือไม่
        std::ifstream file(filename);
        if (!file) {
            std::cerr << RED << "ข้อผิดพลาด: ไม่พบไฟล์ '" << filename << "'" << RESET << std::endl;
            return;
        }

        datasetFilename = filename;
        hasLoadedData = true;

        std::cout << GREEN << (args[0] == "dataset" ? "Dataset" : "Environment") << " loaded successfully" << RESET << std::endl;
    } else if (args[0] == "model") {
        if (args.size() < 2) {
            std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'load model \"[filename]\"'" << RESET << std::endl;
            return;
        }

        std::string filename = args[1];
        // ลบเครื่องหมายคำพูด
        if (filename.front() == '"' && filename.back() == '"') {
            filename = filename.substr(1, filename.length() - 2);
        }

        // ตรวจสอบว่าไฟล์มีอยู่จริงหรือไม่
        std::ifstream file(filename);
        if (!file) {
            std::cerr << RED << "ข้อผิดพลาด: ไม่พบไฟล์ '" << filename << "'" << RESET << std::endl;
            return;
        }

        // สมมติว่าโมเดลถูกโหลดสำเร็จ
        hasCreatedModel = true;
        hasTrainedModel = true;
        modelType = "LoadedModel";

        std::cout << GREEN << "Model loaded successfully" << RESET << std::endl;
    } else {
        std::cerr << RED << "ข้อผิดพลาด: คำสั่ง 'load' ต้องตามด้วย 'dataset', 'environment' หรือ 'model'" << RESET << std::endl;
    }
}

void RLInterpreter::handleTrainCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedProject) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create RL' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasLoadedData) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'load dataset' หรือ 'load environment' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedModel) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create model' ก่อน" << RESET << std::endl;
        return;
    }

    if (args.size() < 1 || args[0] != "model") {
        std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'train model'" << RESET << std::endl;
        return;
    }

    if (hasTrainedModel) {
        std::cerr << RED << "ข้อผิดพลาด: โมเดลนี้ได้ฝึกไปแล้ว ถ้าต้องการฝึกใหม่ให้สร้างโมเดลใหม่" << RESET << std::endl;
        return;
    }

    // ตรวจสอบค่าพารามิเตอร์
    std::stringstream warningMsg;
    bool hasWarning = false;

    if (parameters["learning_rate"] <= 0 || parameters["learning_rate"] > 1) {
        warningMsg << "  - learning_rate มีค่าไม่ถูกต้อง (" << parameters["learning_rate"] << ") ใช้ค่าเริ่มต้น 0.1" << std::endl;
        parameters["learning_rate"] = 0.1;
        hasWarning = true;
    }

    if (parameters["discount_factor"] < 0 || parameters["discount_factor"] > 1) {
        warningMsg << "  - discount_factor มีค่าไม่ถูกต้อง (" << parameters["discount_factor"] << ") ใช้ค่าเริ่มต้น 0.9" << std::endl;
        parameters["discount_factor"] = 0.9;
        hasWarning = true;
    }

    if (parameters["exploration_rate"] < 0 || parameters["exploration_rate"] > 1) {
        warningMsg << "  - exploration_rate มีค่าไม่ถูกต้อง (" << parameters["exploration_rate"] << ") ใช้ค่าเริ่มต้น 0.1" << std::endl;
        parameters["exploration_rate"] = 0.1;
        hasWarning = true;
    }

    if (parameters["episodes"] <= 0 || parameters["episodes"] > 100000) {
        warningMsg << "  - episodes มีค่าไม่ถูกต้อง (" << parameters["episodes"] << ") ใช้ค่าเริ่มต้น 1000" << std::endl;
        parameters["episodes"] = 1000;
        hasWarning = true;
    }

    if (hasWarning) {
        std::cerr << YELLOW << "คำเตือนการฝึกโมเดล:" << std::endl << warningMsg.str() << RESET;
    }

    // แสดงการฝึกโมเดล
    std::cout << BLUE << "Training model with parameters:" << RESET << std::endl;
    for (const auto& param : parameters) {
        std::cout << BLUE << "  - " << param.first << ": " << param.second << RESET << std::endl;
    }

    // จำลองการฝึกโมเดล RL
    int totalEpisodes = parameters["episodes"];
    for (int i = 1; i <= totalEpisodes; ++i) {
        // Simulate training process...replace with actual training loop
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        double reward = (double)i * 10; //Simulate reward
        double avgReward = (double)i * 5; //Simulate average reward
        displayTrainingProgress(i, totalEpisodes, reward, avgReward);
    }
    std::cout << std::endl;

    hasTrainedModel = true;
    std::cout << GREEN << "Training complete" << RESET << std::endl;
}

void RLInterpreter::handleShowCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedProject) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create RL' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedModel) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create model' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasTrainedModel) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'train model' ก่อน" << RESET << std::endl;
        return;
    }

    if (args.size() < 1) {
        std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'show [metric]'" << RESET << std::endl;
        return;
    }

    std::string metric = args[0];
    if (metric == "performance") {
        // สร้างผลลัพธ์สุ่ม
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> reward_dis(-100.0, 500.0);
        std::uniform_int_distribution<> step_dis(50, 200);
        std::uniform_real_distribution<> time_dis(10.0, 100.0);

        double avg_reward = reward_dis(gen);
        int avg_steps = step_dis(gen);
        double training_time = time_dis(gen);

        std::cout << BLUE << "Model Performance:" << RESET << std::endl;
        std::cout << "┌───────────────────────────────┬─────────────┐" << std::endl;
        std::cout << "│ " << std::setw(27) << std::left << "Metric" << " │ " << std::setw(11) << std::right << "Value" << " │" << std::endl;
        std::cout << "├───────────────────────────────┼─────────────┤" << std::endl;
        std::cout << "│ " << std::setw(27) << std::left << "Average Reward" << " │ " << std::setw(11) << std::right << std::fixed << std::setprecision(2) << avg_reward << " │" << std::endl;
        std::cout << "│ " << std::setw(27) << std::left << "Average Steps per Episode" << " │ " << std::setw(11) << std::right << avg_steps << " │" << std::endl;
        std::cout << "│ " << std::setw(27) << std::left << "Training Time" << " │ " << std::setw(9) << std::right << std::fixed << std::setprecision(2) << training_time << " s │" << std::endl;
        std::cout << "└───────────────────────────────┴─────────────┘" << std::endl;
    } else if (metric == "policy") {
        std::cout << BLUE << "Policy Visualization:" << RESET << std::endl;
        std::cout << "┌─────────────────────────────────────────────────────┐" << std::endl;
        std::cout << "│                                                     │" << std::endl;
        std::cout << "│  " << MAGENTA << "State-Action Map" << RESET << "                                │" << std::endl;
        std::cout << "│                                                     │" << std::endl;
        std::cout << "│  " << CYAN << "State 1:  Action 3 (0.78)  Action 1 (0.15)  Action 2 (0.07)" << RESET << " │" << std::endl;
        std::cout << "│  " << CYAN << "State 2:  Action 2 (0.65)  Action 3 (0.25)  Action 1 (0.10)" << RESET << " │" << std::endl;
        std::cout << "│  " << CYAN << "State 3:  Action 1 (0.90)  Action 2 (0.07)  Action 3 (0.03)" << RESET << " │" << std::endl;
        std::cout << "│  " << CYAN << "State 4:  Action 3 (0.55)  Action 1 (0.30)  Action 2 (0.15)" << RESET << " │" << std::endl;
        std::cout << "│  " << CYAN << "State 5:  Action 2 (0.45)  Action 1 (0.40)  Action 3 (0.15)" << RESET << " │" << std::endl;
        std::cout << "│                                                     │" << std::endl;
        std::cout << "└─────────────────────────────────────────────────────┘" << std::endl;
    } else if (metric == "q_values") {
        std::cout << BLUE << "Q-Value Table:" << RESET << std::endl;
        std::cout << "┌───────────┬──────────┬──────────┬──────────┬──────────┐" << std::endl;
        std::cout << "│ " << std::setw(9) << std::left << "State/Act" << " │ " << std::setw(8) << std::right << "Action 1" << " │ " << std::setw(8) << std::right << "Action 2" << " │ " << std::setw(8) << std::right << "Action 3" << " │ " << std::setw(8) << std::right << "Action 4" << " │" << std::endl;
        std::cout << "├───────────┼──────────┼──────────┼──────────┼──────────┤" << std::endl;

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> q_val_dis(-10.0, 100.0);

        for (int i = 1; i <= 5; i++) {
            std::cout << "│ " << std::setw(9) << std::left << "State " + std::to_string(i) << " │ ";
            for (int j = 1; j <= 4; j++) {
                double q_value = q_val_dis(gen);
                std::cout << std::setw(8) << std::right << std::fixed << std::setprecision(2) << q_value << " │ ";
            }
            std::cout << std::endl;
        }
        std::cout << "└───────────┴──────────┴──────────┴──────────┴──────────┘" << std::endl;
    } else if (metric == "rewards") {
        std::cout << BLUE << "Reward Progress:" << RESET << std::endl;
        std::cout << "┌─────────────────────────────────────────────────────┐" << std::endl;
        std::cout << "│                                                     │" << std::endl;
        std::cout << "│  " << GREEN << "Cumulative Reward Over Episodes" << RESET << "                  │" << std::endl;
        std::cout << "│                                                     │" << std::endl;
        std::cout << "│  500 │                                     " << CYAN << "****" << RESET << "   │" << std::endl;
        std::cout << "│      │                               " << CYAN << "***" << RESET << "        │" << std::endl;
        std::cout << "│  400 │                          " << CYAN << "***" << RESET << "             │" << std::endl;
        std::cout << "│      │                     " << CYAN << "***" << RESET << "                  │" << std::endl;
        std::cout << "│  300 │               " << CYAN << "****" << RESET << "                       │" << std::endl;
        std::cout << "│      │         " << CYAN << "****" << RESET << "                             │" << std::endl;
        std::cout << "│  200 │    " << CYAN << "***" << RESET << "                                   │" << std::endl;
        std::cout << "│      │" << CYAN << "***" << RESET << "                                       │" << std::endl;
        std::cout << "│  100 │────────────────────────────────────────     │" << std::endl;
        std::cout << "│      0     200    400    600    800    1000         │" << std::endl;
        std::cout << "│                       Episodes                      │" << std::endl;
        std::cout << "└─────────────────────────────────────────────────────┘" << std::endl;
    } else {
        std::cerr << RED << "ข้อผิดพลาด: metric '" << metric << "' ไม่รองรับ" << RESET << std::endl;
        std::cerr << RED << "metric ที่รองรับ: performance, policy, q_values, rewards" << RESET << std::endl;
    }
}

void RLInterpreter::handleSaveCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedProject) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create RL' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedModel) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create model' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasTrainedModel) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'train model' ก่อน" << RESET << std::endl;
        return;
    }

    if (args.size() < 2 || args[0] != "model") {
        std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'save model \"[filename]\"'" << RESET << std::endl;
        return;
    }

    std::string filename = args[1];

    // ลบเครื่องหมายคำพูด
    if (filename.front() == '"' && filename.back() == '"') {
        filename = filename.substr(1, filename.length() - 2);
    }

    // ตรวจสอบและเพิ่มนามสกุลไฟล์ถ้าไม่มี
    std::string extension;
    size_t dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos) {
        // ถ้าไม่มีนามสกุล ให้เพิ่มตามประเภทโมเดล
        extension = ".rlmodel";
        filename += extension;
    }

    // สร้างโฟลเดอร์ Program test/model ถ้ายังไม่มี
    std::string modelsDir = "Program test/model";
    struct stat st;
    if (stat(modelsDir.c_str(), &st) != 0) {
        // หากไม่มีโฟลเดอร์ ให้สร้างขึ้น
        std::string command = "mkdir -p \"" + modelsDir + "\"";
        int result = system(command.c_str());
        if (result != 0) {
            std::cerr << RED << "ข้อผิดพลาด: ไม่สามารถสร้างโฟลเดอร์ " << modelsDir << RESET << std::endl;
            return;
        }
    }

    std::string fullPath = modelsDir + "/" + filename;
    std::ofstream file(fullPath);
    if (!file) {
        std::cerr << RED << "ข้อผิดพลาด: ไม่สามารถบันทึกไฟล์ " << fullPath << RESET << std::endl;
        return;
    }

    // บันทึกข้อมูลโมเดลลงในไฟล์
    file << "Model Type: " << modelType << std::endl;
    file << "Parameters:" << std::endl;
    for (const auto& param : parameters) {
        file << "  " << param.first << ": " << param.second << std::endl;
    }
    file << "Trained: Yes" << std::endl;
    file << "Dataset: " << datasetFilename << std::endl;

    file.close();
    hasSavedModel = true;
    std::cout << GREEN << "Model saved to " << fullPath << RESET << std::endl;
}

void RLInterpreter::handleRunSimulationCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedProject) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create RL' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedModel) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create model' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasTrainedModel) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'train model' ก่อน" << RESET << std::endl;
        return;
    }

    if (args.size() < 1) {
        std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'run simulation [episodes]'" << RESET << std::endl;
        return;
    }

    int episodes = 1;
    if (args.size() >= 2) {
        try {
            episodes = std::stoi(args[1]);
            if (episodes <= 0) {
                std::cerr << RED << "ข้อผิดพลาด: จำนวน episodes ต้องมากกว่า 0" << RESET << std::endl;
                return;
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << RED << "ข้อผิดพลาด: จำนวน episodes ต้องเป็นตัวเลขจำนวนเต็มบวก" << RESET << std::endl;
            return;
        }
    }

    std::cout << GREEN << "Running " << episodes << " simulation episode(s)..." << RESET << std::endl;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> reward_dis(-10.0, 100.0);
    std::uniform_int_distribution<> step_dis(5, 20);

    for (int e = 1; e <= episodes; e++) {
        int steps = step_dis(gen);
        std::cout << BLUE << "Episode " << e << ":" << RESET << std::endl;

        double total_reward = 0.0;
        for (int s = 1; s <= steps; s++) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            int state = s % 5 + 1;
            int action = (s * 2) % 4 + 1;
            double reward = reward_dis(gen);
            total_reward += reward;

            std::cout << "  Step " << s << ": State " << state
                      << " -> Action " << action
                      << " -> Reward " << std::fixed << std::setprecision(2) << reward << std::endl;
        }

        std::cout << GREEN << "  Episode " << e << " complete. Total steps: " << steps
                  << ", Total reward: " << std::fixed << std::setprecision(2) << total_reward << RESET << std::endl;
        std::cout << std::endl;
    }

    std::cout << GREEN << "Simulation complete." << RESET << std::endl;
}

void RLInterpreter::handleEvaluateCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedProject) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create RL' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedModel) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create model' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasTrainedModel) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'train model' ก่อน" << RESET << std::endl;
        return;
    }

    if (args.size() < 1|| args[0] != "model") {
        std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'evaluate model'" << RESET << std::endl;
        return;
    }

    // สร้างค่าสุ่มสำหรับการประเมิน
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> reward_dis(50.0, 500.0);
    std::uniform_int_distribution<> step_dis(50, 150);
    std::uniform_real_distribution<> success_dis(0.6, 0.95);

    double avg_reward = reward_dis(gen);
    int avg_steps = step_dis(gen);
    double success_rate = success_dis(gen);

    std::cout << BLUE << "Model Evaluation Results:" << RESET << std::endl;
    std::cout << "┌───────────────────────────┬─────────────┐" << std::endl;
    std::cout << "│ " << std::setw(25) << std::left << "Metric" << " │ " << std::setw(11) << std::right << "Value" << " │" << std::endl;
    std::cout << "├───────────────────────────┼─────────────┤" << std::endl;
    std::cout << "│ " << std::setw(25) << std::left << "Average Reward" << " │ " << std::setw(11) << std::right << std::fixed << std::setprecision(2) << avg_reward << " │" << std::endl;
    std::cout << "│ " << std::setw(25) << std::left << "Average Steps" << " │ " << std::setw(11) << std::right << avg_steps << " │" << std::endl;
    std::cout << "│ " << std::setw(25) << std::left << "Success Rate" << " │ " << std::setw(9) << std::right << std::fixed << std::setprecision(2) << (success_rate * 100) << " % │" << std::endl;
    std::cout << "└───────────────────────────┴─────────────┘" << std::endl;
}

void RLInterpreter::handleVisualizeCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedProject) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create RL' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedModel) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create model' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasTrainedModel) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'train model' ก่อน" << RESET << std::endl;
        return;
    }

    if (args.size() < 1) {
        std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'visualize [type]'" << RESET << std::endl;
        return;
    }

    std::string type = args[0];

    if (type == "policy") {
        std::cout << BLUE << "Policy Visualization for " << modelType << ":" << RESET << std::endl;
        std::cout << "┌─────────────────────────────────────────────────────┐" << std::endl;
        std::cout << "│                                                     │" << std::endl;
        std::cout << "│  " << MAGENTA << "State-Action Policy Map" << RESET << "                         │" << std::endl;
        std::cout << "│                                                     │" << std::endl;
        std::cout << "│  " << CYAN << "State 1:  Action 3 (0.78)  Action 1 (0.15)  Action 2 (0.07)" << RESET << " │" << std::endl;
        std::cout << "│  " << CYAN << "State 2:  Action 2 (0.65)  Action 3 (0.25)  Action 1 (0.10)" << RESET << " │" << std::endl;
        std::cout << "│  " << CYAN << "State 3:  Action 1 (0.90)  Action 2 (0.07)  Action 3 (0.03)" << RESET << " │" << std::endl;
        std::cout << "│  " << CYAN << "State 4:  Action 3 (0.55)  Action 1 (0.30)  Action 2 (0.15)" << RESET << " │" << std::endl;
        std::cout << "│  " << CYAN << "State 5:  Action 2 (0.45)  Action 1 (0.40)  Action 3 (0.15)" << RESET << " │" << std::endl;
        std::cout << "│                                                     │" << std::endl;
        std::cout << "└─────────────────────────────────────────────────────┘" << std::endl;
    } else if (type == "value_function") {
        std::cout << BLUE << "Value Function Visualization for " << modelType << ":" << RESET << std::endl;
        std::cout << "┌─────────────────────────────────────────────────────┐" << std::endl;
        std::cout << "│                                                     │" << std::endl;
        std::cout << "│  " << MAGENTA << "State Value Map" << RESET << "                                  │" << std::endl;
        std::cout << "│                                                     │" << std::endl;
        std::cout << "│  100 │                                     " << CYAN << "****" << RESET << "   │" << std::endl;
        std::cout << "│      │                               " << CYAN << "***" << RESET << "        │" << std::endl;
        std::cout << "│   80 │                          " << CYAN << "***" << RESET << "             │" << std::endl;
        std::cout << "│      │                     " << CYAN << "***" << RESET << "                  │" << std::endl;
        std::cout << "│   60 │               " << CYAN << "****" << RESET << "                       │" << std::endl;
        std::cout << "│      │         " << CYAN << "****" << RESET << "                             │" << std::endl;
        std::cout << "│   40 │    " << CYAN << "***" << RESET << "                                   │" << std::endl;
        std::cout << "│      │" << CYAN << "***" << RESET << "                                       │" << std::endl;
        std::cout << "│   20 │────────────────────────────────────────     │" << std::endl;
        std::cout << "│       1     2     3     4     5     6     7        │" << std::endl;
        std::cout << "│                       States                        │" << std::endl;
        std::cout << "│                                                     │" << std::endl;
        std::cout << "└─────────────────────────────────────────────────────┘" << std::endl;
    } else if (type == "learning_curve") {
        std::cout << BLUE << "Learning Curve Visualization for " << modelType << ":" << RESET << std::endl;
        std::cout << "┌─────────────────────────────────────────────────────┐" << std::endl;
        std::cout << "│                                                     │" << std::endl;
        std::cout << "│  " << MAGENTA << "Learning Curve (Reward vs Episodes)" << RESET << "              │" << std::endl;
        std::cout << "│                                                     │" << std::endl;
        std::cout << "│  500 │                                   " << CYAN << "········" << RESET << " │" << std::endl;
        std::cout << "│      │                           " << CYAN << "······" << RESET << "          │" << std::endl;
        std::cout << "│  400 │                      " << CYAN << "·····" << RESET << "                │" << std::endl;
        std::cout << "│      │                 " << CYAN << "·····" << RESET << "                     │" << std::endl;
        std::cout << "│  300 │            " << CYAN << "·····" << RESET << "                          │" << std::endl;
        std::cout << "│      │        " << CYAN << "····" << RESET << "                               │" << std::endl;
        std::cout << "│  200 │     " << CYAN << "···" << RESET << "                                   │" << std::endl;
        std::cout << "│      │  " << CYAN << "···" << RESET << "                                      │" << std::endl;
        std::cout << "│  100 │" << CYAN << "··" << RESET << "                                         │" << std::endl;
        std::cout << "│       0     200    400    600    800    1000        │" << std::endl;
        std::cout << "│                       Episodes                      │" << std::endl;
        std::cout << "│                                                     │" << std::endl;
        std::cout << "└─────────────────────────────────────────────────────┘" << std::endl;
    } else if (type == "environment") {
        std::cout << BLUE << "Environment Visualization for " << modelType << ":" << RESET << std::endl;
        std::cout << "┌─────────────────────────────────────────────────────┐" << std::endl;
        std::cout << "│                                                     │" << std::endl;
        std::cout << "│  " << MAGENTA << "Environment Grid Map" << RESET << "                             │" << std::endl;
        std::cout << "│                                                     │" << std::endl;
        std::cout << "│   ┌─────┬─────┬─────┬─────┬─────┐                  │" << std::endl;
        std::cout << "│   │     │     │     │     │     │                  │" << std::endl;
        std::cout << "│   │  S  │     │     │     │  G  │                  │" << std::endl;
        std::cout << "│   │     │     │     │     │     │                  │" << std::endl;
        std::cout << "│   ├─────┼─────┼─────┼─────┼─────┤                  │" << std::endl;
        std::cout << "│   │     │     │     │     │     │                  │" << std::endl;
        std::cout << "│   │     │  " << RED << "X" << RESET << "  │     │     │     │                  │" << std::endl;
        std::cout << "│   │     │     │     │     │     │                  │" << std::endl;
        std::cout << "│   ├─────┼─────┼─────┼─────┼─────┤                  │" << std::endl;
        std::cout << "│   │     │     │     │     │     │                  │" << std::endl;
        std::cout << "│   │     │  " << RED << "X" << RESET << "  │     │     │     │                  │" << std::endl;
        std::cout << "│   │     │     │     │     │     │                  │" << std::endl;
        std::cout << "│   ├─────┼─────┼─────┼─────┼─────┤                  │" << std::endl;
        std::cout << "│   │     │     │     │     │     │                  │" << std::endl;
        std::cout << "│   │     │     │  " << RED << "X" << RESET << "  │     │     │                  │" << std::endl;
        std::cout << "│   │     │     │     │     │     │                  │" << std::endl;
        std::cout << "│   ├─────┼─────┼─────┼─────┼─────┤                  │" << std::endl;
        std::cout << "│   │     │     │     │     │     │                  │" << std::endl;
        std::cout << "│   │     │     │     │     │     │                  │" << std::endl;
        std::cout << "│   │     │     │     │     │     │                  │" << std::endl;
        std::cout << "│   └─────┴─────┴─────┴─────┴─────┘                  │" << std::endl;
        std::cout << "│                                                     │" << std::endl;
        std::cout << "│   Legend:  S = Start  G = Goal  " << RED << "X" << RESET << " = Obstacle        │" << std::endl;
        std::cout << "│                                                     │" << std::endl;
        std::cout << "└─────────────────────────────────────────────────────┘" << std::endl;
    } else {
        std::cerr << RED << "ข้อผิดพลาด: ประเภทการแสดงผล '" << type << "' ไม่รองรับ" << RESET << std::endl;
        std::cerr << RED << "ประเภทการแสดงผลที่รองรับ: policy, value_function, learning_curve, environment" << RESET << std::endl;
    }
}

std::vector<std::string> RLInterpreter::tokenizeLine(const std::string& line) {
    std::vector<std::string> tokens;
    std::string token;
    bool inQuotes = false;
    for (const char c : line) {
        if (c == '"') {
            inQuotes = !inQuotes;
            token += c;
        } else if (c == ' ' && !inQuotes) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token += c;
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}

void RLInterpreter::interpretLine(const std::string& line) {
    // ข้ามบรรทัดว่างและคอมเมนต์
    if (line.empty() || line[0] == '#') {
        return;
    }

    // แยกคำสั่งและพารามิเตอร์
    std::vector<std::string> tokens = tokenizeLine(line);
    if (tokens.empty()) {
        return;
    }

    std::string command = tokens[0];
    std::vector<std::string> args(tokens.begin() + 1, tokens.end());

    // ประมวลผลคำสั่ง
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
    } else if (command == "run" && args.size() > 0 && args[0] == "simulation") {
        std::vector<std::string> simArgs(args.begin() + 1, args.end());
        handleRunSimulationCommand(simArgs);
    } else if (command == "visualize") {
        handleVisualizeCommand(args);
    } else if (command == "help") {
        std::cout << "คำสั่งที่รองรับสำหรับ Reinforcement Learning:" << std::endl;
        std::cout << "  start                          - เริ่มใช้งานระบบ" << std::endl;
        std::cout << "  create RL                      - สร้างโปรเจค Reinforcement Learning" << std::endl;
        std::cout << "  load dataset \"[filename]\"      - โหลดไฟล์ข้อมูล" << std::endl;
        std::cout << "  load environment \"[filename]\"  - โหลดสภาพแวดล้อม" << std::endl;
        std::cout << "  create model [model_type]      - สร้างโมเดล (QLearning, SARSA, DQN, etc.)" << std::endl;
        std::cout << "  set [parameter] [value]        - ตั้งค่าพารามิเตอร์ (learning_rate, discount_factor, etc.)" << std::endl;
        std::cout << "  train model                    - ฝึกโมเดล" << std::endl;
        std::cout << "  evaluate model                 - ประเมินโมเดล" << std::endl;
        std::cout << "  show performance               - แสดงประสิทธิภาพของโมเดล" << std::endl;
        std::cout << "  show policy                    - แสดงนโยบายของโมเดล" << std::endl;
        std::cout << "  show q_values                  - แสดงตาราง Q-values" << std::endl;
        std::cout << "  show rewards                   - แสดงกราฟรางวัลสะสม" << std::endl;
        std::cout << "  run simulation [episodes]      - จำลองการเล่นเกม" << std::endl;
        std::cout << "  save model \"[filename]\"        - บันทึกโมเดล" << std::endl;
        std::cout << "  load model \"[filename]\"        - โหลดโมเดลที่บันทึกไว้" << std::endl;
        std::cout << "  visualize [type]               - แสดงผลเชิงภาพ (policy, value_function, learning_curve, environment)" << std::endl;
    } else {
        std::cerr << RED << "ข้อผิดพลาด: ไม่รู้จักคำสั่ง '" << command << "'" << RESET << std::endl;
    }
}

void RLInterpreter::displayTrainingProgress(int episode, int totalEpisodes, double reward, double avgReward) {
    // คำนวณเปอร์เซ็นต์ความคืบหน้า
    const int progress = static_cast<int>((static_cast<double>(episode) / totalEpisodes) * 100);
    const int barPosition = progress / 2;

    // ใช้ stringstream เพื่อลดการเรียก I/O หลายครั้ง
    std::stringstream progressBar;
    progressBar << "\rTraining progress: [";

    // สร้างแถบความคืบหน้าที่มีประสิทธิภาพมากขึ้น
    for (int i = 0; i < 50; i++) {
        if (i < barPosition) {
            progressBar << "=";
        } else if (i == barPosition) {
            progressBar << ">";
        } else {
            progressBar << " ";
        }
    }

    // ตั้งค่าความละเอียดตัวเลขเพียงครั้งเดียว
    progressBar << "] " << progress << "% ";
    progressBar << "Episode: " << episode << "/" << totalEpisodes << " ";
    progressBar << std::fixed << std::setprecision(2);
    progressBar << "Reward: " << reward << " ";
    progressBar << "Avg Reward: " << avgReward;

    // แสดงผลเพียงครั้งเดียว
    std::cout << progressBar.str() << std::flush;
}

} // namespace ai_language