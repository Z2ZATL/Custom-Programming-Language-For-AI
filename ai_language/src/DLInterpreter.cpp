
#include "../include/DLInterpreter.h"
#include <random>
#include <iomanip>
#include <fstream>
#include <thread>
#include <chrono>
#include <sys/stat.h>

namespace ai_language {

DLInterpreter::DLInterpreter() {
    projectType = "DL";
}

DLInterpreter::~DLInterpreter() {
}

void DLInterpreter::setDefaultParameters() {
    // ล้างค่าเดิม
    parameters.clear();
    
    // กำหนดค่าเริ่มต้นสำหรับ DL
    parameters["learning_rate"] = 0.001;
    parameters["epochs"] = 100;
    parameters["batch_size"] = 32;
    parameters["dropout"] = 0.2;
    parameters["optimizer"] = 1; // 1 = Adam
}

void DLInterpreter::handleStartCommand() {
    hasStarted = true;
    std::cout << GREEN << "Program started" << RESET << std::endl;
}

void DLInterpreter::handleCreateCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (args.size() < 1) {
        std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'create [type]' หรือ 'create model [model_name]'" << RESET << std::endl;
        return;
    }

    if (args[0] == "DL") {
        // สร้างโปรเจค
        if (hasCreatedProject) {
            std::cerr << YELLOW << "คำเตือน: กำลังสร้างโปรเจคใหม่ ข้อมูลเดิมจะถูกรีเซ็ต" << RESET << std::endl;
        }

        projectType = "DL";
        hasCreatedProject = true;
        hasLoadedData = false;
        hasCreatedModel = false;
        hasTrainedModel = false;
        hasSavedModel = false;

        // ตั้งค่าพารามิเตอร์เริ่มต้น
        setDefaultParameters();

        std::cout << GREEN << "Project created: Deep Learning" << RESET << std::endl;
    } else if (args[0] == "model") {
        if (!hasCreatedProject) {
            std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create DL' ก่อน" << RESET << std::endl;
            return;
        }

        if (!hasLoadedData) {
            std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'load dataset' ก่อน" << RESET << std::endl;
            return;
        }

        if (args.size() < 2) {
            std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'create model [model_name]'" << RESET << std::endl;
            return;
        }

        handleModelCreation(args[1]);
    } else {
        std::cerr << RED << "ข้อผิดพลาด: ประเภท '" << args[0] << "' ไม่ถูกต้อง ต้องเป็น 'DL' หรือ 'model'" << RESET << std::endl;
    }
}

void DLInterpreter::handleModelCreation(const std::string& modelName) {
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
    if (modelName != "NeuralNetwork" && modelName != "CNN" && modelName != "RNN" && 
        modelName != "LSTM" && modelName != "GRU" && modelName != "Transformer") {
        std::cerr << RED << "ข้อผิดพลาด: ไม่รองรับโมเดล '" << modelName << "' สำหรับ Deep Learning" << RESET << std::endl;
        std::cerr << RED << "โมเดลที่รองรับ: NeuralNetwork, CNN, RNN, LSTM, GRU, Transformer" << RESET << std::endl;
        return;
    }

    hasCreatedModel = true;
    hasTrainedModel = false;
    
    std::cout << GREEN << "Model created: " << modelType << RESET << std::endl;
}

void DLInterpreter::handleSetCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedProject) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create DL' ก่อน" << RESET << std::endl;
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
    if (parameter == "learning_rate" || parameter == "epochs" || parameter == "batch_size" || parameter == "dropout") {
        try {
            double value = std::stod(valueStr);

            // ตรวจสอบขอบเขตค่าพารามิเตอร์
            if (parameter == "learning_rate" && (value <= 0.0 || value > 1.0)) {
                std::cerr << RED << "ข้อผิดพลาด: learning_rate ต้องอยู่ในช่วง (0, 1]" << RESET << std::endl;
                return;
            } else if (parameter == "epochs" && (value <= 0 || value > 10000)) {
                std::cerr << RED << "ข้อผิดพลาด: epochs ต้องอยู่ในช่วง (0, 10000]" << RESET << std::endl;
                return;
            } else if (parameter == "batch_size" && (value <= 0 || value > 1000)) {
                std::cerr << RED << "ข้อผิดพลาด: batch_size ต้องอยู่ในช่วง (0, 1000]" << RESET << std::endl;
                return;
            } else if (parameter == "dropout" && (value < 0.0 || value >= 1.0)) {
                std::cerr << RED << "ข้อผิดพลาด: dropout ต้องอยู่ในช่วง [0, 1)" << RESET << std::endl;
                return;
            }

            parameters[parameter] = value;
            std::cout << GREEN << "Parameter set: " << parameter << " = " << value << RESET << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cerr << RED << "ข้อผิดพลาด: ค่า '" << valueStr << "' ไม่ใช่ตัวเลข" << RESET << std::endl;
        }
    } else if (parameter == "optimizer") {
        if (valueStr == "sgd" || valueStr == "SGD") {
            parameters["optimizer"] = 0;
        } else if (valueStr == "adam" || valueStr == "Adam") {
            parameters["optimizer"] = 1;
        } else if (valueStr == "rmsprop" || valueStr == "RMSprop") {
            parameters["optimizer"] = 2;
        } else {
            std::cerr << RED << "ข้อผิดพลาด: optimizer ไม่รองรับ '" << valueStr << "'" << RESET << std::endl;
            std::cerr << RED << "optimizer ที่รองรับ: SGD, Adam, RMSprop" << RESET << std::endl;
            return;
        }
        std::cout << GREEN << "Parameter set: " << parameter << " = " << valueStr << RESET << std::endl;
    } else if (parameter == "activation") {
        if (valueStr == "relu" || valueStr == "ReLU") {
            parameters["activation"] = 0;
        } else if (valueStr == "sigmoid" || valueStr == "Sigmoid") {
            parameters["activation"] = 1;
        } else if (valueStr == "tanh" || valueStr == "Tanh") {
            parameters["activation"] = 2;
        } else if (valueStr == "softmax" || valueStr == "Softmax") {
            parameters["activation"] = 3;
        } else {
            std::cerr << RED << "ข้อผิดพลาด: activation ไม่รองรับ '" << valueStr << "'" << RESET << std::endl;
            std::cerr << RED << "activation ที่รองรับ: ReLU, Sigmoid, Tanh, Softmax" << RESET << std::endl;
            return;
        }
        std::cout << GREEN << "Parameter set: " << parameter << " = " << valueStr << RESET << std::endl;
    } else {
        std::cerr << RED << "ข้อผิดพลาด: พารามิเตอร์ '" << parameter << "' ไม่รองรับสำหรับโมเดล " << modelType << RESET << std::endl;
    }
}

void DLInterpreter::handleAddLayerCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedProject) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create DL' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedModel) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create model' ก่อน" << RESET << std::endl;
        return;
    }

    if (args.size() < 2) {
        std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'add layer [type] [units] [options]'" << RESET << std::endl;
        return;
    }

    std::string layerType = args[0];
    if (layerType == "input") {
        if (args.size() < 2) {
            std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'add layer input [units]'" << RESET << std::endl;
            return;
        }
        try {
            int units = std::stoi(args[1]);
            std::cout << GREEN << "Added input layer with " << units << " units" << RESET << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cerr << RED << "ข้อผิดพลาด: จำนวน units ต้องเป็นตัวเลขจำนวนเต็ม" << RESET << std::endl;
        }
    } else if (layerType == "hidden") {
        if (args.size() < 2) {
            std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'add layer hidden [units] [options]'" << RESET << std::endl;
            return;
        }
        try {
            int units = std::stoi(args[1]);
            std::string activation = "relu";
            if (args.size() >= 4 && args[2] == "activation") {
                activation = args[3];
                // ลบเครื่องหมายคำพูด
                if (activation.front() == '"' && activation.back() == '"') {
                    activation = activation.substr(1, activation.length() - 2);
                }
            }
            std::cout << GREEN << "Added hidden layer with " << units << " units and " << activation << " activation" << RESET << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cerr << RED << "ข้อผิดพลาด: จำนวน units ต้องเป็นตัวเลขจำนวนเต็ม" << RESET << std::endl;
        }
    } else if (layerType == "output") {
        if (args.size() < 2) {
            std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'add layer output [units] [options]'" << RESET << std::endl;
            return;
        }
        try {
            int units = std::stoi(args[1]);
            std::string activation = "softmax";
            if (args.size() >= 4 && args[2] == "activation") {
                activation = args[3];
                // ลบเครื่องหมายคำพูด
                if (activation.front() == '"' && activation.back() == '"') {
                    activation = activation.substr(1, activation.length() - 2);
                }
            }
            std::cout << GREEN << "Added output layer with " << units << " units and " << activation << " activation" << RESET << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cerr << RED << "ข้อผิดพลาด: จำนวน units ต้องเป็นตัวเลขจำนวนเต็ม" << RESET << std::endl;
        }
    } else if (layerType == "dropout") {
        if (args.size() < 2) {
            std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'add layer dropout [rate]'" << RESET << std::endl;
            return;
        }
        try {
            double rate = std::stod(args[1]);
            if (rate < 0.0 || rate >= 1.0) {
                std::cerr << RED << "ข้อผิดพลาด: dropout rate ต้องอยู่ในช่วง [0, 1)" << RESET << std::endl;
                return;
            }
            std::cout << GREEN << "Added dropout layer with rate " << rate << RESET << std::endl;
        } catch (const std::invalid_argument& e) {
            std::cerr << RED << "ข้อผิดพลาด: dropout rate ต้องเป็นตัวเลข" << RESET << std::endl;
        }
    } else {
        std::cerr << RED << "ข้อผิดพลาด: ประเภทเลเยอร์ '" << layerType << "' ไม่รองรับ" << RESET << std::endl;
        std::cerr << RED << "ประเภทเลเยอร์ที่รองรับ: input, hidden, output, dropout" << RESET << std::endl;
    }
}

void DLInterpreter::handleLoadCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedProject) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create DL' ก่อน" << RESET << std::endl;
        return;
    }

    if (args.size() < 1) {
        std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'load dataset \"[filename]\"' หรือ 'load model \"[filename]\"'" << RESET << std::endl;
        return;
    }

    if (args[0] == "dataset") {
        if (args.size() < 2) {
            std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'load dataset \"[filename]\"'" << RESET << std::endl;
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
        
        std::cout << GREEN << "Dataset loaded successfully" << RESET << std::endl;
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
        std::cerr << RED << "ข้อผิดพลาด: คำสั่ง 'load' ต้องตามด้วย 'dataset' หรือ 'model'" << RESET << std::endl;
    }
}

void DLInterpreter::handleTrainCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedProject) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create DL' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasLoadedData) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'load dataset' ก่อน" << RESET << std::endl;
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
        warningMsg << "  - learning_rate มีค่าไม่ถูกต้อง (" << parameters["learning_rate"] << ") ใช้ค่าเริ่มต้น 0.001" << std::endl;
        parameters["learning_rate"] = 0.001;
        hasWarning = true;
    }

    if (parameters["epochs"] <= 0 || parameters["epochs"] > 10000) {
        warningMsg << "  - epochs มีค่าไม่ถูกต้อง (" << parameters["epochs"] << ") ใช้ค่าเริ่มต้น 100" << std::endl;
        parameters["epochs"] = 100;
        hasWarning = true;
    }

    if (parameters["batch_size"] <= 0 || parameters["batch_size"] > 1000) {
        warningMsg << "  - batch_size มีค่าไม่ถูกต้อง (" << parameters["batch_size"] << ") ใช้ค่าเริ่มต้น 32" << std::endl;
        parameters["batch_size"] = 32;
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

    // จำลองการฝึกโมเดล
    std::cout << "Training";
    for (int i = 0; i < 10; i++) { // Deep Learning ใช้เวลานานกว่า ML
        std::cout << "." << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    std::cout << std::endl;

    hasTrainedModel = true;
    std::cout << GREEN << "Training complete" << RESET << std::endl;
}

void DLInterpreter::handleShowCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedProject) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create DL' ก่อน" << RESET << std::endl;
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
    if (metric == "accuracy") {
        // สร้างค่าความแม่นยำสุ่ม
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.7, 0.99);
        double accuracy = dis(gen);

        std::cout << "Model accuracy: " << std::fixed << std::setprecision(4) << accuracy << std::endl;
    } else if (metric == "loss") {
        // สร้างค่า loss สุ่ม
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.01, 0.5);
        double loss = dis(gen);

        std::cout << "Model loss: " << std::fixed << std::setprecision(4) << loss << std::endl;
    } else if (metric == "performance") {
        // แสดงประสิทธิภาพโมเดลแบบสุ่ม
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> acc_dis(0.7, 0.99);
        std::uniform_real_distribution<> loss_dis(0.01, 0.5);
        std::uniform_real_distribution<> time_dis(10.0, 100.0);
        
        double accuracy = acc_dis(gen);
        double loss = loss_dis(gen);
        double training_time = time_dis(gen);

        std::cout << "Model performance:" << std::endl;
        std::cout << "  - Accuracy: " << std::fixed << std::setprecision(4) << accuracy << std::endl;
        std::cout << "  - Loss: " << std::fixed << std::setprecision(4) << loss << std::endl;
        std::cout << "  - Training time: " << std::fixed << std::setprecision(2) << training_time << " seconds" << std::endl;
    } else {
        std::cerr << RED << "ข้อผิดพลาด: metric '" << metric << "' ไม่รองรับ" << RESET << std::endl;
        std::cerr << RED << "metric ที่รองรับ: accuracy, loss, performance" << RESET << std::endl;
    }
}

void DLInterpreter::handleSaveCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedProject) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create DL' ก่อน" << RESET << std::endl;
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
        extension = ".dlmodel";
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

void DLInterpreter::handlePlotCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedProject) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create DL' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedModel) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create model' ก่อน" << RESET << std::endl;
        return;
    }

    if (args.size() < 1 || args[0] != "model") {
        std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'plot model'" << RESET << std::endl;
        return;
    }

    std::cout << BLUE << "Model Architecture Plot:" << RESET << std::endl;
    std::cout << "┌─────────────────────────────────────────┐" << std::endl;
    std::cout << "│ " << MAGENTA << std::setw(40) << std::left << modelType << RESET << " │" << std::endl;
    std::cout << "├─────────────────────────────────────────┤" << std::endl;
    std::cout << "│ " << CYAN << std::setw(40) << std::left << "Input Layer" << RESET << " │" << std::endl;
    std::cout << "│   └─> " << std::setw(35) << std::left << "Units: 4" << " │" << std::endl;
    std::cout << "├─────────────────────────────────────────┤" << std::endl;
    std::cout << "│ " << CYAN << std::setw(40) << std::left << "Hidden Layer 1" << RESET << " │" << std::endl;
    std::cout << "│   ├─> " << std::setw(35) << std::left << "Units: 8" << " │" << std::endl;
    std::cout << "│   └─> " << std::setw(35) << std::left << "Activation: ReLU" << " │" << std::endl;
    std::cout << "├─────────────────────────────────────────┤" << std::endl;
    std::cout << "│ " << CYAN << std::setw(40) << std::left << "Dropout Layer" << RESET << " │" << std::endl;
    std::cout << "│   └─> " << std::setw(35) << std::left << "Rate: 0.2" << " │" << std::endl;
    std::cout << "├─────────────────────────────────────────┤" << std::endl;
    std::cout << "│ " << CYAN << std::setw(40) << std::left << "Hidden Layer 2" << RESET << " │" << std::endl;
    std::cout << "│   ├─> " << std::setw(35) << std::left << "Units: 4" << " │" << std::endl;
    std::cout << "│   └─> " << std::setw(35) << std::left << "Activation: ReLU" << " │" << std::endl;
    std::cout << "├─────────────────────────────────────────┤" << std::endl;
    std::cout << "│ " << CYAN << std::setw(40) << std::left << "Output Layer" << RESET << " │" << std::endl;
    std::cout << "│   ├─> " << std::setw(35) << std::left << "Units: 3" << " │" << std::endl;
    std::cout << "│   └─> " << std::setw(35) << std::left << "Activation: Softmax" << " │" << std::endl;
    std::cout << "└─────────────────────────────────────────┘" << std::endl;
}

void DLInterpreter::handleEvaluateCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedProject) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create DL' ก่อน" << RESET << std::endl;
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

    if (args.size() < 1 || args[0] != "model") {
        std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'evaluate model'" << RESET << std::endl;
        return;
    }

    // สร้างค่าสุ่มสำหรับการประเมิน
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> acc_dis(0.7, 0.99);
    std::uniform_real_distribution<> loss_dis(0.01, 0.5);
    std::uniform_real_distribution<> f1_dis(0.65, 0.98);
    std::uniform_real_distribution<> recall_dis(0.6, 0.95);
    std::uniform_real_distribution<> precision_dis(0.7, 0.98);
    
    double accuracy = acc_dis(gen);
    double loss = loss_dis(gen);
    double f1_score = f1_dis(gen);
    double recall = recall_dis(gen);
    double precision = precision_dis(gen);

    std::cout << BLUE << "Model Evaluation Results:" << RESET << std::endl;
    std::cout << "┌─────────────────────────────┬───────────┐" << std::endl;
    std::cout << "│ " << std::setw(25) << std::left << "Metric" << " │ " << std::setw(9) << std::right << "Value" << " │" << std::endl;
    std::cout << "├─────────────────────────────┼───────────┤" << std::endl;
    std::cout << "│ " << std::setw(25) << std::left << "Accuracy" << " │ " << std::setw(9) << std::right << std::fixed << std::setprecision(4) << accuracy << " │" << std::endl;
    std::cout << "│ " << std::setw(25) << std::left << "Loss" << " │ " << std::setw(9) << std::right << std::fixed << std::setprecision(4) << loss << " │" << std::endl;
    std::cout << "│ " << std::setw(25) << std::left << "F1 Score" << " │ " << std::setw(9) << std::right << std::fixed << std::setprecision(4) << f1_score << " │" << std::endl;
    std::cout << "│ " << std::setw(25) << std::left << "Recall" << " │ " << std::setw(9) << std::right << std::fixed << std::setprecision(4) << recall << " │" << std::endl;
    std::cout << "│ " << std::setw(25) << std::left << "Precision" << " │ " << std::setw(9) << std::right << std::fixed << std::setprecision(4) << precision << " │" << std::endl;
    std::cout << "└─────────────────────────────┴───────────┘" << std::endl;
}

void DLInterpreter::handleVisualizeCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedProject) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create DL' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasLoadedData) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'load dataset' ก่อน" << RESET << std::endl;
        return;
    }

    if (args.size() < 1 || args[0] != "data") {
        std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'visualize data'" << RESET << std::endl;
        return;
    }

    std::cout << BLUE << "Data Visualization:" << RESET << std::endl;
    std::cout << "┌─────────────────────────────────────────────────────┐" << std::endl;
    std::cout << "│                                                     │" << std::endl;
    std::cout << "│  " << MAGENTA << "Feature Distribution" << RESET << "                              │" << std::endl;
    std::cout << "│                                                     │" << std::endl;
    std::cout << "│  " << GREEN << "***                                             " << RESET << "  │" << std::endl;
    std::cout << "│  " << GREEN << "*****                                           " << RESET << "  │" << std::endl;
    std::cout << "│  " << GREEN << "*******                     *****               " << RESET << "  │" << std::endl;
    std::cout << "│  " << GREEN << "********                   *******              " << RESET << "  │" << std::endl;
    std::cout << "│  " << GREEN << "**********     ***        *********             " << RESET << "  │" << std::endl;
    std::cout << "│  " << CYAN << "************  *****      ***********            " << RESET << "  │" << std::endl;
    std::cout << "│  " << CYAN << "Feature 1    Feature 2   Feature 3              " << RESET << "  │" << std::endl;
    std::cout << "│                                                     │" << std::endl;
    std::cout << "└─────────────────────────────────────────────────────┘" << std::endl;
}

void DLInterpreter::interpretLine(const std::string& line) {
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
    } else if (command == "add" && args.size() > 0 && args[0] == "layer") {
        std::vector<std::string> layerArgs(args.begin() + 1, args.end());
        handleAddLayerCommand(layerArgs);
    } else if (command == "train") {
        handleTrainCommand(args);
    } else if (command == "evaluate") {
        handleEvaluateCommand(args);
    } else if (command == "show") {
        handleShowCommand(args);
    } else if (command == "save") {
        handleSaveCommand(args);
    } else if (command == "plot") {
        handlePlotCommand(args);
    } else if (command == "visualize") {
        handleVisualizeCommand(args);
    } else if (command == "help") {
        std::cout << "คำสั่งที่รองรับสำหรับ Deep Learning:" << std::endl;
        std::cout << "  start                          - เริ่มใช้งานระบบ" << std::endl;
        std::cout << "  create DL                      - สร้างโปรเจค Deep Learning" << std::endl;
        std::cout << "  load dataset \"[filename]\"      - โหลดไฟล์ข้อมูล" << std::endl;
        std::cout << "  create model [model_type]      - สร้างโมเดล (NeuralNetwork, CNN, RNN, LSTM, GRU, Transformer)" << std::endl;
        std::cout << "  add layer input [units]        - เพิ่มเลเยอร์อินพุต" << std::endl;
        std::cout << "  add layer hidden [units] activation \"[activation]\"  - เพิ่มเลเยอร์ซ่อน พร้อม activation function" << std::endl;
        std::cout << "  add layer dropout [rate]       - เพิ่มเลเยอร์ dropout" << std::endl;
        std::cout << "  add layer output [units] activation \"[activation]\"  - เพิ่มเลเยอร์เอาต์พุต พร้อม activation function" << std::endl;
        std::cout << "  set [parameter] [value]        - ตั้งค่าพารามิเตอร์ (learning_rate, epochs, batch_size)" << std::endl;
        std::cout << "  train model                    - ฝึกโมเดล" << std::endl;
        std::cout << "  evaluate model                 - ประเมินโมเดล" << std::endl;
        std::cout << "  show accuracy                  - แสดงความแม่นยำของโมเดล" << std::endl;
        std::cout << "  show loss                      - แสดงค่า loss ของโมเดล" << std::endl;
        std::cout << "  show performance               - แสดงประสิทธิภาพทั้งหมดของโมเดล" << std::endl;
        std::cout << "  plot model                     - แสดงโครงสร้างโมเดล" << std::endl;
        std::cout << "  visualize data                 - แสดงข้อมูลในรูปแบบกราฟ" << std::endl;
        std::cout << "  save model \"[filename]\"        - บันทึกโมเดล" << std::endl;
        std::cout << "  load model \"[filename]\"        - โหลดโมเดลที่บันทึกไว้" << std::endl;
    } else {
        std::cerr << RED << "ข้อผิดพลาด: ไม่รู้จักคำสั่ง '" << command << "'" << RESET << std::endl;
    }
}

} // namespace ai_language
