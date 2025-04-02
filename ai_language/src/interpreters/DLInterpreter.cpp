// interpreters/DLInterpreter.cpp
#include "../../include/interpreters/DLInterpreter.h"
#include <iostream>
#include <sys/stat.h>
#include <chrono>
#include <cstdlib> // Added for setenv

namespace ai_language {

void DLInterpreter::interpret() {
    // โค้ดสำหรับการแปลภาษา DL
    std::cout << "กำลังดำเนินการกับโมเดล Deep Learning..." << std::endl;
    std::cout << "กำลังสร้างเครือข่ายประสาทเทียม (Neural Network)..." << std::endl;

    // ตัวอย่างการดำเนินการที่เกี่ยวข้องกับ Deep Learning โดยเฉพาะ
    std::cout << "การตั้งค่าเครือข่าย Deep Learning เสร็จสิ้น" << std::endl;
    std::cout << "เริ่มต้นการเทรนโมเดล..." << std::endl;
}

void DLInterpreter::setDefaultParameters() {
    // ตั้งค่าพารามิเตอร์เริ่มต้นสำหรับ Deep Learning
    parameters["learning_rate"] = 0.001;
    parameters["batch_size"] = 32;
    parameters["epochs"] = 50;
    parameters["dropout"] = 0.2;
    parameters["hidden_layers"] = 3;
    parameters["neurons_per_layer"] = 128;
}

void DLInterpreter::handleStartCommand() {
    std::cout << CYAN << "เริ่มต้นการใช้งาน Deep Learning Interpreter..." << RESET << std::endl;
    hasStarted = true;
}

void DLInterpreter::handleCreateCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cout << RED << "กรุณาใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (args.size() < 1) {
        std::cout << RED << "กรุณาระบุชนิดของโมเดล" << RESET << std::endl;
        return;
    }

    std::string modelType = args[0];
    std::cout << GREEN << "กำลังสร้างโมเดล Deep Learning ประเภท: " << modelType << RESET << std::endl;

    if (modelType == "CNN") {
        std::cout << BLUE << "สร้างโมเดล Convolutional Neural Network" << RESET << std::endl;
    } else if (modelType == "RNN") {
        std::cout << BLUE << "สร้างโมเดล Recurrent Neural Network" << RESET << std::endl;
    } else if (modelType == "LSTM") {
        std::cout << BLUE << "สร้างโมเดล Long Short-Term Memory" << RESET << std::endl;
    } else if (modelType == "Transformer") {
        std::cout << BLUE << "สร้างโมเดล Transformer" << RESET << std::endl;
    } else {
        std::cout << BLUE << "สร้างโมเดล Neural Network ทั่วไป" << RESET << std::endl;
    }

    // เคลียร์ Layer เก่าเมื่อสร้างโมเดลใหม่
    layers.clear();

    hasCreatedModel = true;
    this->modelType = modelType;
}

// เพิ่มฟังก์ชันใหม่เพื่อรองรับคำสั่ง add layer
void DLInterpreter::handleAddCommand(const std::vector<std::string>& args) {
    if (!hasCreatedModel) {
        std::cout << RED << "กรุณาสร้างโมเดลก่อนด้วยคำสั่ง 'create'" << RESET << std::endl;
        return;
    }

    if (args.size() < 2 || args[0] != "layer") {
        std::cout << RED << "รูปแบบคำสั่งไม่ถูกต้อง ตัวอย่าง: add layer input 784" << RESET << std::endl;
        return;
    }

    std::string layerType = args[1];
    std::string layerInfo;

    if (layerType == "input") {
        if (args.size() < 3) {
            std::cout << RED << "รูปแบบคำสั่งไม่ถูกต้อง สำหรับ input layer: add layer input size" << RESET << std::endl;
            return;
        }

        int inputSize = std::stoi(args[2]);

        // รองรับทั้งรูปแบบเก่า (width, height, channels) และรูปแบบใหม่ (size)
        if (args.size() >= 5) {
            // รูปแบบเก่า: add layer input width height channels
            int width = inputSize;
            int height = std::stoi(args[3]);
            int channels = std::stoi(args[4]);
            layerInfo = "input:" + std::to_string(width) + "x" + std::to_string(height) + "x" + std::to_string(channels);
        } else {
            // รูปแบบใหม่: add layer input size
            layerInfo = "input:" + std::to_string(inputSize) + ":linear";
        }

    } else if (layerType == "convolutional" || layerType == "conv") {
        if (args.size() < 5) {
            std::cout << RED << "รูปแบบคำสั่งไม่ถูกต้อง สำหรับ convolutional layer: add layer conv filters kernel_size activation" << RESET << std::endl;
            return;
        }
        int filters = std::stoi(args[2]);
        int kernelSize = std::stoi(args[3]);
        std::string activation = args.size() > 5 && args[4] == "activation" ? args[5] : "relu";
        if (activation.front() == '"') activation = activation.substr(1, activation.size()-2);
        layerInfo = "conv:" + std::to_string(filters) + ":" + std::to_string(kernelSize) + ":" + activation;

    } else if (layerType == "max_pooling" || layerType == "pool") {
        if (args.size() < 4) {
            std::cout << RED << "รูปแบบคำสั่งไม่ถูกต้อง สำหรับ pooling layer: add layer pool size_x size_y" << RESET << std::endl;
            return;
        }
        int sizeX = std::stoi(args[2]);
        int sizeY = std::stoi(args[3]);
        layerInfo = "pool:" + std::to_string(sizeX) + "x" + std::to_string(sizeY);

    } else if (layerType == "flatten") {
        layerInfo = "flatten";

    } else if (layerType == "hidden" || layerType == "dense") {
        if (args.size() < 3) {
            std::cout << RED << "รูปแบบคำสั่งไม่ถูกต้อง สำหรับ hidden layer: add layer hidden neurons [activation function]" << RESET << std::endl;
            return;
        }
        int neurons = std::stoi(args[2]);
        std::string activation = args.size() > 4 && args[3] == "activation" ? args[4] : "relu";
        if (activation.front() == '"') activation = activation.substr(1, activation.size()-2);
        layerInfo = "hidden:" + std::to_string(neurons) + ":" + activation;

    } else if (layerType == "output") {
        if (args.size() < 3) {
            std::cout << RED << "รูปแบบคำสั่งไม่ถูกต้อง สำหรับ output layer: add layer output neurons [activation function]" << RESET << std::endl;
            return;
        }
        int neurons = std::stoi(args[2]);
        std::string activation = args.size() > 4 && args[3] == "activation" ? args[4] : "softmax";
        if (activation.front() == '"') activation = activation.substr(1, activation.size()-2);
        layerInfo = "output:" + std::to_string(neurons) + ":" + activation;

    } else if (layerType == "dropout") {
        if (args.size() < 3) {
            std::cout << RED << "รูปแบบคำสั่งไม่ถูกต้อง สำหรับ dropout layer: add layer dropout rate" << RESET << std::endl;
            return;
        }
        float rate = std::stof(args[2]);
        layerInfo = "dropout:" + std::to_string(rate);
    } else {
        std::cout << RED << "ไม่รู้จักประเภทของ layer: " << layerType << RESET << std::endl;
        return;
    }

    layers.push_back(layerInfo);
    std::cout << GREEN << "เพิ่ม " << layerType << " layer สำเร็จ" << RESET << std::endl;
}

void DLInterpreter::handleLoadCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cout << RED << "กรุณาใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (args.size() < 1) {
        std::cout << RED << "กรุณาระบุที่อยู่ของข้อมูล" << RESET << std::endl;
        return;
    }

    datasetPath = args[0];
    std::cout << GREEN << "กำลังโหลดข้อมูลจาก: " << datasetPath << RESET << std::endl;

    // จำลองการโหลดข้อมูล
    std::cout << BLUE << "กำลังเตรียมข้อมูลสำหรับ Deep Learning..." << RESET << std::endl;
    std::cout << BLUE << "กำลังทำ Data Preprocessing..." << RESET << std::endl;
    std::cout << BLUE << "กำลังทำ Data Augmentation..." << RESET << std::endl;

    hasLoadedData = true;
}

void DLInterpreter::handleSetCommand(const std::vector<std::string>& args) {
    if (!hasCreatedModel) {
        std::cout << RED << "กรุณาสร้างโมเดลก่อนด้วยคำสั่ง 'create'" << RESET << std::endl;
        return;
    }

    if (args.size() < 2) {
        std::cout << RED << "กรุณาระบุพารามิเตอร์และค่า" << RESET << std::endl;
        return;
    }

    std::string paramName = args[0];
    double paramValue;

    try {
        paramValue = std::stod(args[1]);
    } catch (const std::exception& e) {
        std::cout << RED << "ค่าพารามิเตอร์ต้องเป็นตัวเลข" << RESET << std::endl;
        return;
    }

    parameters[paramName] = paramValue;
    std::cout << GREEN << "ตั้งค่า " << paramName << " = " << paramValue << RESET << std::endl;
}

void DLInterpreter::handleTrainCommand(const std::vector<std::string>& args) {
    if (!hasCreatedModel) {
        std::cout << RED << "กรุณาสร้างโมเดลก่อนด้วยคำสั่ง 'create'" << RESET << std::endl;
        return;
    }

    if (!hasLoadedData) {
        std::cout << RED << "กรุณาโหลดข้อมูลก่อนด้วยคำสั่ง 'load'" << RESET << std::endl;
        return;
    }

    std::cout << GREEN << "กำลังเทรนโมเดล " << modelType << "..." << RESET << std::endl;
    std::cout << BLUE << "จำนวน Epochs: " << parameters["epochs"] << RESET << std::endl;
    std::cout << BLUE << "Learning Rate: " << parameters["learning_rate"] << RESET << std::endl;
    std::cout << BLUE << "Batch Size: " << parameters["batch_size"] << RESET << std::endl;

    // จำลองการเทรนโมเดล
    for (int epoch = 1; epoch <= 3; epoch++) {
        std::cout << YELLOW << "Epoch " << epoch << "/" << parameters["epochs"] << " - ";
        std::cout << "Loss: " << 0.5 / epoch << " - Accuracy: " << 0.7 + epoch * 0.1 << RESET << std::endl;
    }

    hasTrainedModel = true;
}

void DLInterpreter::handleEvaluateCommand(const std::vector<std::string>& args) {
    if (!hasTrainedModel) {
        std::cout << RED << "กรุณาเทรนโมเดลก่อนด้วยคำสั่ง 'train'" << RESET << std::endl;
        return;
    }

    if (args.empty() || (args.size() >= 1 && args[0] == "model")) {
        std::cout << GREEN << "กำลังประเมินผลโมเดล " << modelType << "..." << RESET << std::endl;
        std::cout << BLUE << "ความแม่นยำบนชุดข้อมูลทดสอบ: 0.92" << RESET << std::endl;
        std::cout << BLUE << "ค่า Loss บนชุดข้อมูลทดสอบ: 0.08" << RESET << std::endl;
    } else {
        std::cout << RED << "รูปแบบคำสั่งไม่ถูกต้อง ตัวอย่าง: evaluate model" << RESET << std::endl;
    }
}

void DLInterpreter::handleShowCommand(const std::vector<std::string>& args) {
    if (!hasTrainedModel) {
        std::cout << RED << "กรุณาเทรนโมเดลก่อนด้วยคำสั่ง 'train'" << RESET << std::endl;
        return;
    }

    if (args.size() < 1) {
        std::cout << RED << "กรุณาระบุข้อมูลที่ต้องการแสดง" << RESET << std::endl;
        return;
    }

    std::string showType = args[0];

    if (showType == "accuracy") {
        std::cout << GREEN << "ความแม่นยำของโมเดล: 0.89" << RESET << std::endl;
    } else if (showType == "loss") {
        std::cout << GREEN << "ค่า Loss: 0.134" << RESET << std::endl;
    } else if (showType == "graph") {
        std::cout << GREEN << "กำลังสร้างกราฟผลการเทรนโมเดล " << modelType << "..." << RESET << std::endl;

        // กำหนดเส้นทางสำหรับเก็บไฟล์กราฟ (ใช้โฟลเดอร์ที่มีอยู่แล้ว)
        std::string dataDir = "ai_language/Program test/Data";

        // จำลองข้อมูลการเทรนสำหรับสร้างกราฟ
        std::string csvPath = dataDir + "/dl_learning_curves_data.csv";
        std::ofstream csvFile(csvPath);
        if (csvFile.is_open()) {
            csvFile << "epoch,accuracy,loss\n";
            for (int i = 1; i <= int(parameters["epochs"]); i++) {
                float progress = i / float(parameters["epochs"]);
                float accuracy = 0.65f + 0.3f * progress;
                float loss = 0.82f - 0.77f * progress;
                csvFile << i << "," << accuracy << "," << loss << "\n";
            }
            csvFile.close();
        }

        // ใช้ Python script เพื่อสร้างกราฟเหมือนใน MLInterpreter
        std::string pythonCommand = "cd ai_language && python3 src/utils/plot_generator.py \"" + csvPath + "\" \"" + dataDir + "/dl_learning_curves.png\" \"Learning Curves for " + modelType + " Model\" 2>&1";

        // เรียกใช้ Python script เพื่อสร้างกราฟ
        FILE* pipe = popen(pythonCommand.c_str(), "r");
        if (!pipe) {
            std::cout << RED << "เกิดข้อผิดพลาดในการสร้างกราฟ" << RESET << std::endl;
            return;
        }

        char buffer[128];
        std::string scriptOutput = "";
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                scriptOutput += buffer;
        }
        pclose(pipe);

        if (scriptOutput.find("Error") != std::string::npos) {
            std::cout << RED << scriptOutput << RESET << std::endl;
        } else {
            std::cout << BLUE << "ข้อมูลสรุปการเทรนโมเดล:" << RESET << std::endl;
            std::cout << BLUE << "------------------------" << RESET << std::endl;
            std::cout << BLUE << "Loss ลดลงจาก 0.82 เหลือ 0.05 ตลอด " << parameters["epochs"] << " epochs" << RESET << std::endl;
            std::cout << BLUE << "Accuracy เพิ่มขึ้นจาก 0.65 เป็น 0.95 ตลอด " << parameters["epochs"] << " epochs" << RESET << std::endl;
            std::cout << BLUE << "------------------------" << RESET << std::endl;

            std::cout << GREEN << "ข้อมูลได้รับการบันทึกเป็นไฟล์ CSV: " << csvPath << RESET << std::endl;
            std::cout << GREEN << "กราฟถูกสร้างและบันทึกเป็นไฟล์ PNG: " << dataDir << "/dl_learning_curves.png" << RESET << std::endl;
            std::cout << GREEN << "To view the graph, open the PNG file in an image viewer" << RESET << std::endl;
        }
    } else if (showType == "performance" || showType == "metrics") {
        std::cout << GREEN << "ผลการวัดประสิทธิภาพของโมเดล " << modelType << ":" << RESET << std::endl;
        std::cout << BLUE << "ความแม่นยำ (Accuracy): 0.89" << RESET << std::endl;
        std::cout << BLUE << "ค่า Loss: 0.134" << RESET << std::endl;
        std::cout << BLUE << "Precision: 0.91" << RESET << std::endl;
        std::cout << BLUE << "Recall: 0.87" << RESET << std::endl;
        std::cout << BLUE << "F1 Score: 0.89" << RESET << std::endl;
    } else if (showType == "model") {
        std::cout << GREEN << "โครงสร้างโมเดล " << modelType << ":" << RESET << std::endl;

        if (layers.empty()) {
            // ถ้ายังไม่มีการกำหนด layer ใช้ค่าเริ่มต้น
            std::cout << BLUE << "- Input Layer: 784 neurons" << RESET << std::endl;
            std::cout << BLUE << "- Hidden Layer 1: " << parameters["neurons_per_layer"] << " neurons, Activation: ReLU" << RESET << std::endl;
            std::cout << BLUE << "- Hidden Layer 2: " << parameters["neurons_per_layer"] / 2 << " neurons, Activation: ReLU" << RESET << std::endl;
            std::cout << BLUE << "- Output Layer: 10 neurons, Activation: Softmax" << RESET << std::endl;
        } else {
            // แสดง layer ที่ผู้ใช้กำหนด
            for (size_t i = 0; i < layers.size(); i++) {
                std::string layerInfo = layers[i];
                size_t firstColon = layerInfo.find(':');
                size_t secondColon = layerInfo.find(':', firstColon + 1);

                std::string layerType = layerInfo.substr(0, firstColon);
                int neurons = std::stoi(layerInfo.substr(firstColon + 1, secondColon - firstColon - 1));
                std::string activation = layerInfo.substr(secondColon + 1);

                std::cout << BLUE << "- ";
                if (layerType == "input") {
                    std::cout << "Input Layer: ";
                } else if (layerType == "hidden") {
                    std::cout << "Hidden Layer " << i << ": ";
                } else if (layerType == "output") {
                    std::cout << "Output Layer: ";
                } else {
                    std::cout << layerType << " Layer: ";
                }

                std::cout << neurons << " neurons";
                if (activation != "linear") {
                    std::cout << ", Activation: " << activation;
                }
                std::cout << RESET << std::endl;
            }
        }
    } else {
        std::cout << RED << "ไม่รู้จักคำสั่ง show ประเภท: " << showType << RESET << std::endl;
    }

    hasShowedAccuracy = true;
}

void DLInterpreter::handleSaveCommand(const std::vector<std::string>& args) {
    if (!hasTrainedModel) {
        std::cout << RED << "กรุณาเทรนโมเดลก่อนด้วยคำสั่ง 'train'" << RESET << std::endl;
        return;
    }

    std::string savePath = "Program test/model/dl_model.dlmodel";
    if (args.size() >= 1) {
        std::string fileName = args[0];
        // ถ้ามีการระบุเส้นทางที่ไม่ได้ขึ้นต้นด้วย / หรือ ./ ให้เพิ่มเส้นทาง default
        if (fileName[0] != '/' && (fileName.size() < 2 || fileName.substr(0, 2) != "./")) {
            savePath = "Program test/model/" + fileName;
        } else {
            savePath = fileName;
        }
        
        // เพิ่มนามสกุลไฟล์ .dlmodel ถ้าไม่มีการระบุนามสกุล
        if (savePath.find('.') == std::string::npos) {
            savePath += ".dlmodel";
        }
    }

    // ตรวจสอบเส้นทางว่ามีโฟลเดอร์หรือไม่
    size_t lastSlash = savePath.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        std::string directory = savePath.substr(0, lastSlash);
        // ตรวจสอบว่าโฟลเดอร์มีอยู่หรือไม่
        struct stat buffer;
        if (stat(directory.c_str(), &buffer) != 0) {
            // โฟลเดอร์ไม่มีอยู่ ให้สร้าง
            std::string command = "mkdir -p \"" + directory + "\"";
            int result = system(command.c_str());
            if (result != 0) {
                std::cout << RED << "เกิดข้อผิดพลาดในการสร้างโฟลเดอร์: " << directory << RESET << std::endl;
                return;
            }
        }
    }

    std::cout << GREEN << "กำลังบันทึกโมเดล " << modelType << " ไปที่ " << savePath << RESET << std::endl;

    // จำลองการบันทึกโมเดลโดยการสร้างไฟล์
    std::ofstream modelFile(savePath);
    if (modelFile.is_open()) {
        // ตั้งค่า time zone เป็น +7 (ประเทศไทย)
        setenv("TZ", "Asia/Bangkok", 1);
        tzset(); // อัปเดต time zone

        // สร้างข้อมูลเวลาปัจจุบัน
        auto now = std::chrono::system_clock::now();
        std::time_t current_time = std::chrono::system_clock::to_time_t(now);
        std::string timestamp = std::ctime(&current_time);
        // ลบ newline character ที่ ctime เพิ่มมาโดยอัตโนมัติ
        if (!timestamp.empty() && timestamp[timestamp.length()-1] == '\n') {
            timestamp.erase(timestamp.length()-1);
        }

        modelFile << "# DL Model saved from AI Language\n";
        modelFile << "model_type: " << modelType << "\n";
        modelFile << "learning_rate: " << parameters["learning_rate"] << "\n";
        modelFile << "epochs: " << parameters["epochs"] << "\n";
        modelFile << "accuracy: 0.95\n";
        modelFile << "create_time: " << timestamp << "\n\n";

        modelFile << "# Layers: " << layers.size() << "\n";
        for (const auto& layer : layers) {
            modelFile << "layer: " << layer << "\n";
        }
        modelFile << "\n# Parameters:\n";
        for (const auto& param : parameters) {
            modelFile << param.first << ": " << param.second << "\n";
        }

        modelFile.close();
        std::cout << GREEN << "โมเดลถูกบันทึกสำเร็จที่ '" << savePath << "'" << RESET << std::endl;
    } else {
        std::cout << RED << "เกิดข้อผิดพลาดในการบันทึกโมเดล: ไม่สามารถเปิดไฟล์ " << savePath << " ได้" << RESET << std::endl;
    }

    hasSavedModel = true;
}

void DLInterpreter::handleHelpCommand() {
    std::cout << CYAN << "===== คำสั่งของ Deep Learning Interpreter =====" << RESET << std::endl;
    std::cout << GREEN << "start" << RESET << " - เริ่มต้นการใช้งาน" << std::endl;
    std::cout << GREEN << "create [model_type]" << RESET << " - สร้างโมเดล (CNN, RNN, LSTM, Transformer)" << std::endl;
    std::cout << GREEN << "load [dataset_path]" << RESET << " - โหลดข้อมูล" << std::endl;
    std::cout << GREEN << "set [param_name] [value]" << RESET << " - ตั้งค่าพารามิเตอร์" << std::endl;
    std::cout << GREEN << "train" << RESET << " - เทรนโมเดล" << std::endl;
    std::cout << GREEN << "show [accuracy|loss|model]" << RESET << " - แสดงข้อมูลของโมเดล" << std::endl;
    std::cout << GREEN << "save [file_path]" << RESET << " - บันทึกโมเดล" << std::endl;
    std::cout << GREEN << "help" << RESET << " - แสดงคำสั่งที่รองรับ" << std::endl;
    std::cout << GREEN << "add layer [layer_type] [neurons] [activation]" << RESET << " - เพิ่ม layer" << std::endl; // Added help for add command
}

} // namespace ai_language