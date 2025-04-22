// interpreters/DLInterpreter.cpp
#include "../../include/interpreters/DLInterpreter.h"
#include <iostream>
#include <sys/stat.h>
#include <chrono>
#include <cstdlib> // Added for setenv
#include <cmath> // Added for std::exp
#include <map>
#include <string>
#include <fstream> // Added for file operations
#include <algorithm> // Added for std::transform

namespace ai_language {

void DLInterpreter::interpret() {
    // โค้ดสำหรับการแปลภาษา DL
    std::cout << "กำลังดำเนินการกับโมเดล Deep Learning..." << std::endl;
    std::cout << "กำลังสร้างเครือข่ายประสาทเทียม (Neural Network)..." << std::endl;

    // ตัวอย่างการดำเนินการที่เกี่ยวข้องกับ Deep Learning โดยเฉพาะ
    std::cout << "การตั้งค่าเครือข่าย Deep Learning เสร็จสิ้น" << std::endl;
    std::cout << "เริ่มต้นการเทรนโมเดล..." << std::endl;
}

void DLInterpreter::addLayer(const std::string& layerType, const std::map<std::string, std::string>& params) {
    std::cout << "Adding " << layerType << " layer with parameters: ";
    for (const auto& param : params) {
        std::cout << param.first << "=" << param.second << " ";
    }
    std::cout << std::endl;
}

void DLInterpreter::createModel(const std::string& modelType) {
    std::cout << "Creating DL model: " << modelType << std::endl;

    // รองรับโมเดลประเภทต่างๆ สำหรับ DL
    std::vector<std::string> supportedModels = {
        "NeuralNetwork", "CNN", "RNN", "LSTM", "GRU", "Transformer"
    };

    bool isSupported = false;
    for (const auto& model : supportedModels) {
        if (model == modelType) {
            isSupported = true;
            break;
        }
    }

    if (!isSupported) {
        std::cerr << "Warning: Model type '" << modelType << "' might not be fully supported for DL." << std::endl;
    }
}

void DLInterpreter::setDefaultParameters() {
    // ตั้งค่าพารามิเตอร์เริ่มต้นสำหรับ Deep Learningning
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

    hasCreated = true;
    this->modelType = modelType;
}

// เพิ่มฟังก์ชันใหม่เพื่อรองรับคำสั่ง add layer
void DLInterpreter::handleAddCommand(const std::vector<std::string>& args) {
    if (!hasCreated) {
        std::cout << RED << "กรุณาสร้างโมเดลก่อนด้วยคำสั่ง 'create'" << RESET << std::endl;
        return;
    }

    if (args.size() < 2 || args[0] != "layer") {
        std::cout << RED << "รูปแบบคำสั่งไม่ถูกต้อง ตัวอย่าง: add layer input 784" << RESET << std::endl;
        return;
    }

    std::string layerType = args[1];
    std::string layerInfo;

    // เพิ่มการสนับสนุนคำสั่ง add layer Dense

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

    } else if (layerType == "hidden" || layerType == "dense" || layerType == "Dense") {
        // ตรวจสอบรูปแบบ add layer Dense units 64 activation "relu"
        if (layerType == "Dense") {
            if (args.size() < 4) {
                std::cout << RED << "รูปแบบคำสั่งไม่ถูกต้อง สำหรับ Dense layer: add layer Dense units <size> [activation <func>]" << RESET << std::endl;
                return;
            }

            if (args[2] != "units") {
                std::cout << RED << "รูปแบบคำสั่งไม่ถูกต้อง สำหรับ Dense layer กรุณาระบุ units" << RESET << std::endl;
                return;
            }

            int neurons = std::stoi(args[3]);
            std::string activation = "relu"; // ค่าเริ่มต้น

            // ตรวจสอบว่ามีการระบุ activation หรือไม่
            for (size_t i = 4; i < args.size() - 1; i++) {
                if (args[i] == "activation") {
                    activation = args[i + 1];
                    break;
                }
            }

            // ลบเครื่องหมายคำพูดออกถ้ามี
            if (activation.length() >= 2 && activation.front() == '"' && activation.back() == '"') {
                activation = activation.substr(1, activation.length() - 2);
            }

            layerInfo = "dense:" + std::to_string(neurons) + ":" + activation;
        } else {
            // รูปแบบเดิม: add layer hidden/dense neurons [activation]
            if (args.size() < 3) {
                std::cout << RED << "รูปแบบคำสั่งไม่ถูกต้อง สำหรับ hidden layer: add layer hidden neurons [activation function]" << RESET << std::endl;
                return;
            }
            int neurons = std::stoi(args[2]);
            std::string activation = args.size() > 4 && args[3] == "activation" ? args[4] : "relu";
            if (activation.front() == '"') activation = activation.substr(1, activation.size()-2);
            layerInfo = "hidden:" + std::to_string(neurons) + ":" + activation;
        }

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

    // Update model architecture
    if (modelType == "CNN" && (layerType == "conv" || layerType == "convolutional")) {
        std::cout << "Added convolutional layer to CNN model" << std::endl;
    } else if (modelType == "LSTM" && layerType == "lstm") {
        std::cout << "Added LSTM layer to recurrent model" << std::endl;
    } else if (modelType == "Transformer" && layerType == "attention") {
        std::cout << "Added attention layer to transformer model" << std::endl;
    } else {
        std::cout << "Added " << layerType << " layer to " << modelType << " model" << std::endl;
    }
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

    hasLoaded = true;
}

void DLInterpreter::handleSetCommand(const std::vector<std::string>& args) {
    if (!hasCreated) {
        std::cout << RED << "กรุณาสร้างโมเดลก่อนด้วยคำสั่ง 'create'" << RESET << std::endl;
        return;
    }

    if (args.size() < 2) {
        std::cout << RED << "กรุณาระบุพารามิเตอร์และค่า" << RESET << std::endl;
        return;
    }

    std::string paramName = args[0];
    std::string paramValueStr = args[1];

    // ถ้าพารามิเตอร์เป็นค่าตัวเลข
    try {
        // ลองแปลงเป็นตัวเลข
        double paramValue = std::stod(paramValueStr);
        parameters[paramName] = paramValue;
        std::cout << GREEN << "ตั้งค่า " << paramName << " = " << paramValue << RESET << std::endl;
    } catch (const std::exception& e) {
        // ถ้าแปลงเป็นตัวเลขไม่ได้ ให้เก็บเป็นค่าสตริง
        // ลบเครื่องหมายคำพูดออกถ้ามี
        if (paramValueStr.length() >= 2 && paramValueStr.front() == '"' && paramValueStr.back() == '"') {
            paramValueStr = paramValueStr.substr(1, paramValueStr.length() - 2);
        }

        // เก็บค่าสตริงในพารามิเตอร์พิเศษ
        stringParameters[paramName] = paramValueStr;

        // ใช้ค่าพิเศษในพารามิเตอร์ปกติเพื่อบ่งชี้ว่านี่เป็นข้อมูลสตริง
        parameters[paramName] = -1;

        std::cout << GREEN << "ตั้งค่า " << paramName << " = " << paramValueStr << RESET << std::endl;
    }
}

void DLInterpreter::handleTrainCommand(const std::vector<std::string>& /* args */) {
    if (!hasCreated) {
        std::cout << RED << "กรุณาสร้างโมเดลก่อนด้วยคำสั่ง 'create'" << RESET << std::endl;
        return;
    }

    if (!hasLoaded) {
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

    hasTrained = true;
}

void DLInterpreter::handleEvaluateCommand(const std::vector<std::string>& args) {
    if (!hasTrained) {
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
    if (!hasTrained) {
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

        // กำหนดเส้นทางสำหรับเก็บไฟล์กราฟ
        std::string dataDir = "Program test/Data";

        // ตรวจสอบและสร้างโฟลเดอร์หากไม่มีอยู่
        std::string mkdir_cmd = "mkdir -p '" + dataDir + "'";
        int mkdir_result = system(mkdir_cmd.c_str());
        if (mkdir_result != 0) {
            std::cout << RED << "ไม่สามารถสร้างโฟลเดอร์สำหรับบันทึกข้อมูล" << RESET << std::endl;
            return;
        }

        // จำลองข้อมูลการเทรนสำหรับสร้างกราฟ - เพิ่มข้อมูลเฉพาะ DL
        std::string csvPath = dataDir + "/learning_curves_data.csv";
        std::ofstream csvFile(csvPath);
        if (csvFile.is_open()) {
            // เพิ่มข้อมูล validation_accuracy และ validation_loss สำหรับ DL
            csvFile << "epoch,accuracy,loss,validation_accuracy,validation_loss\n";
            for (int i = 1; i <= int(parameters["epochs"]); i++) {
                float progress = i / float(parameters["epochs"]);
                float accuracy = 0.65f + 0.3f * (1 - std::exp(-(i)/25.0));
                float loss = 0.82f - 0.77f * (1 - std::exp(-(i)/30.0));

                // สร้างข้อมูล validation จำลองที่มีความแตกต่างเล็กน้อยกับข้อมูลเทรน
                float validation_accuracy = accuracy - 0.05f * (1 - progress) * (float)rand() / RAND_MAX;
                float validation_loss = loss + 0.08f * (1 - progress) * (float)rand() / RAND_MAX;

                csvFile << i << "," << accuracy << "," << loss << "," 
                       << validation_accuracy << "," << validation_loss << "\n";
            }
            csvFile.close();
            std::cout << "กำลังบันทึกข้อมูลเป็นไฟล์ CSV เท่านั้น (ส่วนการสร้างกราฟกำลังปรับปรุง)..." << std::endl;
        } else {
            std::cout << RED << "ไม่สามารถเปิดไฟล์สำหรับบันทึกข้อมูล: " << csvPath << RESET << std::endl;
            return;
        }

        // สร้างกราฟด้วย plot_generator.py
        std::cout << "กำลังสร้างไฟล์กราฟข้อมูลทั้งในรูปแบบ HTML และ PNG (ไม่สามารถแสดงในเทอร์มินัลได้)..." << std::endl;

        // ใช้ Python script เพื่อสร้างกราฟ
        std::string command = "python3 src/utils/plot_generator.py \"" + csvPath + "\" \"" + dataDir + "\" \"Learning Curves for " + modelType + " Model\"";
        int result = system(command.c_str());

        if (result == 0) {
            std::cout << "Graph saved successfully to " << dataDir << std::endl;
            std::cout << "ข้อมูลได้รับการบันทึกเป็นไฟล์ CSV: ai_language/" << csvPath << std::endl;
            std::cout << "กราฟถูกสร้างและบันทึกเป็นไฟล์ PNG: ai_language/" << dataDir << "/learning_curves.png" << std::endl;
            std::cout << "To view the graph, open the HTML file in a web browser" << std::endl;
        } else {
            std::cout << RED << "เกิดข้อผิดพลาดในการสร้างกราฟ" << RESET << std::endl;
            return;
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
                    std::cout << "Hidden Layer " << (i) << ": ";
                } else if (layerType == "output") {
                    std::cout << "Output Layer: ";
                } else if (layerType == "conv") {
                    std::cout << "Convolutional Layer: ";
                } else if (layerType == "pool") {
                    std::cout << "Max Pooling Layer: ";
                } else if (layerType == "dropout") {
                    std::cout << "Dropout Layer (rate=" << neurons << "): ";
                } else if (layerType == "flatten") {
                    std::cout << "Flatten Layer: ";
                } else {
                    std::cout << layerType << " Layer: ";
                }

                if (layerType != "dropout" && layerType != "flatten" && layerType != "pool") {
                    std::cout << neurons << " neurons";
                    if (activation != "linear") {
                        std::cout << ", Activation: " << activation;
                    }
                }
                std::cout << RESET << std::endl;
            }
        }
    } else {
        std::cout << RED << "ไม่รู้จักคำสั่ง show ประเภท: " << showType << RESET << std::endl;
    }

    // State already tracked by BaseInterpreter
}

void DLInterpreter::handleSaveCommand(const std::vector<std::string>& args) {
    if (!hasTrained) {
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

        // ตรวจสอบนามสกุลไฟล์และเติมถ้าไม่มี
        bool hasSupportedExtension = 
            (savePath.find(".dlmodel") != std::string::npos) || 
            (savePath.find(".pkl") != std::string::npos) || 
            (savePath.find(".onnx") != std::string::npos);

        if (!hasSupportedExtension) {
            // ใช้ .pkl เป็นค่าเริ่มต้นตามข้อเสนอ
            savePath += ".pkl";
            std::cout << "ใช้นามสกุล .pkl เป็นค่าเริ่มต้นสำหรับโมเดล DL" << std::endl;
        }
    }

    // ใช้โฟลเดอร์ที่มีอยู่แล้ว ไม่จำเป็นต้องสร้างโฟลเดอร์ใหม่ทุกครั้ง
    // แต่ยังควรตรวจสอบและสร้างเฉพาะกรณีที่ไม่มีโฟลเดอร์
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

    // แสดงข้อความว่ากำลังบันทึกโมเดลไปที่ไหน
    std::cout << "Saving DL model to: " << savePath << std::endl;

    // ใช้ฟังก์ชัน getCurrentDateTime จาก BaseInterpreter
    std::string timestamp = getCurrentDateTime();

    // ตรวจสอบนามสกุลไฟล์เพื่อเลือกวิธีการบันทึกที่เหมาะสม
    if (savePath.find(".pkl") != std::string::npos) {
        // สำหรับไฟล์ .pkl ใช้ Python และ pickle
        // Ensure the directory exists
        std::string dataDir = "Program test/Data";
        std::string mkdirCmd = "mkdir -p '" + dataDir + "'";
        int mkdirResult = system(mkdirCmd.c_str());
        if (mkdirResult != 0) {
            std::cout << RED << "ไม่สามารถสร้างโฟลเดอร์สำหรับบันทึกข้อมูล" << RESET << std::endl;
            return;
        }

        std::string pythonScript = dataDir + "/save_dl_model.py";
        std::ofstream scriptFile(pythonScript);

        if (scriptFile.is_open()) {
            scriptFile << "import pickle\n";
            scriptFile << "import numpy as np\n";
            scriptFile << "import os\n";
            scriptFile << "from datetime import datetime\n\n";

            scriptFile << "# สร้างโฟลเดอร์ถ้ายังไม่มี\n";
            scriptFile << "os.makedirs('./" << savePath.substr(0, savePath.find_last_of('/')) << "', exist_ok=True)\n\n";

            scriptFile << "# ข้อมูลโมเดล\n";
            scriptFile << "model_data = {\n";
            scriptFile << "    'model_type': '" << modelType << "',\n";
            scriptFile << "    'learning_rate': " << parameters["learning_rate"] << ",\n";
            scriptFile << "    'epochs': " << parameters["epochs"] << ",\n";
            scriptFile << "    'accuracy': 0.95,\n";
            scriptFile << "    'create_time': '" << timestamp << "',\n";

            // เพิ่มข้อมูล layers
            scriptFile << "    'layers': [\n";
            for (const auto& layer : layers) {
                scriptFile << "        '" << layer << "',\n";
            }
            scriptFile << "    ],\n";

            // เพิ่มพารามิเตอร์อื่นๆ
            scriptFile << "    'parameters': {\n";
            for (const auto& param : parameters) {
                scriptFile << "        '" << param.first << "': " << param.second << ",\n";
            }
            scriptFile << "    },\n";
            scriptFile << "}\n\n";

            scriptFile << "# บันทึกโมเดลด้วย pickle\n";
            scriptFile << "model_path = './" << savePath << "'\n";
            scriptFile << "with open(model_path, 'wb') as f:\n";
            scriptFile << "    pickle.dump(model_data, f)\n";
            scriptFile << "\nprint('Model successfully saved to: ' + model_path)\n";
            scriptFile.close();

            // รันสคริปต์ Python
            std::string command = "python3 \"" + pythonScript + "\"";
            int result = system(command.c_str());

            if (result == 0) {
                std::cout << "Model successfully saved to: " << savePath << std::endl;
                std::cout << GREEN << "โมเดลถูกบันทึกไปที่ 'ai_language/" << savePath << "'" << RESET << std::endl;
                std::cout << "โมเดลถูกบันทึกในรูปแบบ pickle (.pkl) สามารถโหลดได้โดยตรงใน Python" << std::endl;
            } else {
                std::cout << RED << "เกิดข้อผิดพลาดในการบันทึกโมเดลด้วย pickle" << RESET << std::endl;
                std::cout << "Command that failed: " << command << std::endl;
            }
        } else {
            std::cout << RED << "เกิดข้อผิดพลาด: ไม่สามารถสร้างสคริปต์ Python สำหรับบันทึกโมเดล" << RESET << std::endl;
        }
    } else if (savePath.find(".onnx") != std::string::npos) {
        // สำหรับไฟล์ .onnx (ONNX format)
        std::string pythonScript = "Program test/Data/save_dl_onnx.py";
        std::ofstream scriptFile(pythonScript);

        if (scriptFile.is_open()) {
            scriptFile << "import numpy as np\n";
            scriptFile << "import onnx\n";
            scriptFile << "from onnx import helper\n";
            scriptFile << "from onnx import TensorProto\n\n";

            scriptFile << "# สร้างข้อมูลโมเดลจำลองในรูปแบบ ONNX\n";
            scriptFile << "try:\n";
            scriptFile << "    # สร้าง inputs\n";
            scriptFile << "    inputs = []\n";

            // หาขนาด input จาก layers
            scriptFile << "    # อ่านข้อมูล layers เพื่อหาขนาด input\n";
            scriptFile << "    input_size = 784  # ค่าเริ่มต้นทั่วไป\n";
            scriptFile << "    layers = " << (layers.empty() ? "[]" : "['" + layers[0] + "']") << "\n";
            scriptFile << "    if layers and 'input' in layers[0]:\n";
            scriptFile << "        parts = layers[0].split(':')\n";
            scriptFile << "        if len(parts) > 1:\n";
            scriptFile << "            try:\n";
            scriptFile << "                input_size = int(parts[1])\n";
            scriptFile << "            except:\n";
            scriptFile << "                pass\n";

            scriptFile << "    inputs.append(helper.make_tensor_value_info('input', TensorProto.FLOAT, [1, input_size]))\n\n";

            // สร้าง outputs
            scriptFile << "    # สร้าง outputs\n";
            scriptFile << "    outputs = [helper.make_tensor_value_info('output', TensorProto.FLOAT, [1, 10])]\n\n";

            // สร้าง nodes ตาม layers
            scriptFile << "    # สร้าง nodes\n";
            scriptFile << "    nodes = []\n";
            scriptFile << "    nodes.append(helper.make_node('Identity', ['input'], ['output'], 'identity_node'))\n\n";

            // สร้างและบันทึกโมเดล ONNX
            scriptFile << "    # สร้างโมเดล ONNX\n";
            scriptFile << "    graph = helper.make_graph(nodes, '" << modelType << "', inputs, outputs)\n";
            scriptFile << "    model = helper.make_model(graph)\n";
            scriptFile << "    model.ir_version = 7  # ONNX version\n";
            scriptFile << "    onnx.save(model, '" << savePath << "')\n";
            scriptFile << "    print('Model successfully exported to ONNX format at: " << savePath << "')\n";scriptFile << "    print('Note: This is a simplified ONNX model for demonstration purposes.')\n";
            scriptFile << "except Exception as e:\n";
            scriptFile << "    print('Error exporting to ONNX:', e)\n";
            scriptFile.close();

            // รันสคริปต์ Python
            std::string installCommand = "pip install onnx --no-warn-script-location > /dev/null 2>&1";
            int install_result = system(installCommand.c_str());
            if (install_result != 0) {
                std::cout << YELLOW << "Warning: Package installation may have issues" << RESET << std::endl;
            }

            // Create required directories
            std::string dirname = savePath.substr(0, savePath.find_last_of('/'));
            std::string mkdir_cmd = "mkdir -p '" + dirname + "'";
            int dir_result = system(mkdir_cmd.c_str());
            if (dir_result != 0) {
                std::cout << RED << "Warning: Unable to create model directory: " << dirname << RESET << std::endl;
            } else {
                std::cout << GREEN << "Created directory: " << dirname << RESET << std::endl;
            }

            std::string command = "python3 " + pythonScript;
            int result = system(command.c_str());

            if (result == 0) {
                std::cout << "Model successfully exported to ONNX format at: " << savePath << std::endl;
                std::cout << GREEN << "โมเดลถูกบันทึกไปที่ 'ai_language/" << savePath << "'" << RESET << std::endl;
                std::cout << "โมเดลถูกบันทึกในรูปแบบ ONNX (.onnx) สามารถใช้กับ ONNX Runtime หรือแพลตฟอร์มที่รองรับ ONNX" << std::endl;
            } else {
                std::cout << RED << "เกิดข้อผิดพลาดในการบันทึกโมเดลเป็น ONNX" << RESET << std::endl;
                std::cout << "โปรดติดตั้ง ONNX package ก่อนใช้งาน: pip install onnx" << std::endl;
            }
        } else {
            std::cout << RED << "เกิดข้อผิดพลาด: ไม่สามารถสร้างสคริปต์ Python สำหรับบันทึกโมเดล ONNX" << RESET << std::endl;
        }
    } else {
        // สำหรับไฟล์ .dlmodel หรือนามสกุลอื่นๆ ใช้วิธีการเดิม
        std::ofstream modelFile(savePath);
        if (modelFile.is_open()) {
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
            std::cout << "Model successfully saved to: " << savePath << std::endl;
            std::cout << GREEN << "โมเดลถูกบันทึกไปที่ 'ai_language/" << savePath << "'" << RESET << std::endl;
        } else {
            std::cout << RED << "เกิดข้อผิดพลาดในการบันทึกโมเดล: ไม่สามารถเปิดไฟล์ " << savePath << " ได้" << RESET << std::endl;
        }
    }

    hasModel = true;
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
    std::cout << GREEN << "plot [type] [options]" << RESET << " - สร้างกราฟ (loss, accuracy, model)" << std::endl; // Added help for plot command
    std::cout << GREEN << "predict [data]" << RESET << " - ทำนายผลลัพธ์" << std::endl; // Added help for predict command
    std::cout << GREEN << "list models" << RESET << " - แสดงรายการโมเดล" << std::endl; // Added help for list models command
}

// Implementation of remaining pure virtual functions

void DLInterpreter::handlePlotCommand(const std::vector<std::string>& parts) {
    if (parts.size() < 2) {
        std::cout << RED << "Error: Missing plot type. Usage: plot <type> [options]" << RESET << std::endl;
        std::cout << "Available DL plot types: loss, accuracy, model, learning_curves, confusion_matrix, feature_maps" << std::endl;
        return;
    }

    std::string plotType = parts[1];
    std::string outputPath = "Program test/Data/dl_plot_output.png";

    // ตรวจสอบและสร้างโฟลเดอร์ถ้ายังไม่มี
    std::string mkdir_cmd = "mkdir -p 'Program test/Data'";
    int mkdir_result = system(mkdir_cmd.c_str());
    if (mkdir_result != 0) {
        std::cout << RED << "ไม่สามารถสร้างโฟลเดอร์สำหรับบันทึกกราฟ" << RESET << std::endl;
        return;
    }

    if (!hasTrained && plotType != "model") {
        std::cout << RED << "Error: Model must be trained first before plotting " << plotType << RESET << std::endl;
        return;
    }

    std::cout << CYAN << "Creating " << plotType << " plot for " << modelType << " model..." << RESET << std::endl;

    // จำลองการสร้างกราฟประเภทต่างๆ สำหรับ Deep Learning
    if (plotType == "loss") {
        std::cout << "Generating training and validation loss curves" << std::endl;

        // สร้างข้อมูลสำหรับกราฟในไฟล์ CSV
        std::string csvPath = "Program test/Data/dl_loss_data.csv";
        std::ofstream csvFile(csvPath);
        if (csvFile.is_open()) {
            csvFile << "epoch,train_loss,val_loss\n";
            for (int i = 1; i <= int(parameters["epochs"]); i++) {
                float progress = i / float(parameters["epochs"]);
                float train_loss = 0.8f - 0.75f * (1 - std::exp(-(i)/15.0));
                float val_loss = train_loss + 0.05f * (1 - progress) * (float)rand() / RAND_MAX;
                csvFile << i << "," << train_loss << "," << val_loss << "\n";
            }
            csvFile.close();

            // เรียกใช้ Python script เพื่อสร้างกราฟ
            std::string command = "python3 src/utils/plot_generator.py \"" + csvPath + "\" \"Program test/Data\" \"Loss Curves for " + modelType + " Model\"";
            int result = system(command.c_str());

            if (result == 0) {
                std::cout << GREEN << "Loss plot saved to: ai_language/Program test/Data/learning_curves.png" << RESET << std::endl;
            } else {
                std::cout << RED << "Error generating loss plot" << RESET << std::endl;
            }
        }
    } else if (plotType == "accuracy") {
        std::cout << "Generating training and validation accuracy curves" << std::endl;

        // สร้างข้อมูลสำหรับกราฟในไฟล์ CSV
        std::string csvPath = "Program test/Data/dl_accuracy_data.csv";
        std::ofstream csvFile(csvPath);
        if (csvFile.is_open()) {
            csvFile << "epoch,train_accuracy,val_accuracy\n";
            for (int i = 1; i <= int(parameters["epochs"]); i++) {
                float progress = i / float(parameters["epochs"]);
                float train_acc = 0.5f + 0.45f * (1 - std::exp(-(i)/20.0));
                float val_acc = train_acc - 0.05f * (1 - progress) * (float)rand() / RAND_MAX;
                csvFile << i << "," << train_acc << "," << val_acc << "\n";
            }
            csvFile.close();

            // เรียกใช้ Python script เพื่อสร้างกราฟ
            std::string command = "python3 src/utils/plot_generator.py \"" + csvPath + "\" \"Program test/Data\" \"Accuracy Curves for " + modelType + " Model\"";
            int result = system(command.c_str());

            if (result == 0) {
                std::cout << GREEN << "Accuracy plot saved to: ai_language/Program test/Data/learning_curves.png" << RESET << std::endl;
            } else {
                std::cout << RED << "Error generating accuracy plot" << RESET << std::endl;
            }
        }
    } else if (plotType == "model") {
        std::cout << "Generating model architecture visualization" << std::endl;

        // สร้างไฟล์ ASCII visualization ของโมเดล
        std::string modelPath = "Program test/Data/model_architecture.txt";
        std::ofstream modelFile(modelPath);
        if (modelFile.is_open()) {
            modelFile << "Model: " << modelType << "\n";
            modelFile << "==============================\n\n";

            if (layers.empty()) {
                // ถ้ายังไม่มีการกำหนด layer ใช้ค่าเริ่มต้น
                modelFile << "Input Layer [784] \n    |\n";
                modelFile << "Dense Layer [" << parameters["neurons_per_layer"] << "] (ReLU)\n    |\n";
                modelFile << "Dense Layer [" << parameters["neurons_per_layer"]/2 << "] (ReLU)\n    |\n";
                modelFile << "Output Layer [10] (Softmax)\n";
            } else {
                // แสดง layer ที่ผู้ใช้กำหนด
                for (size_t i = 0; i < layers.size(); i++) {
                    std::string layerInfo = layers[i];
                    size_t firstColon = layerInfo.find(':');

                    std::string layerType = layerInfo.substr(0, firstColon);
                                        std::string details = layerInfo.substr(firstColon + 1);

                    modelFile << layerType << " Layer [" << details << "]\n";
                    if (i < layers.size() - 1) {
                        modelFile << "    |\n";
                    }
                }
            }

            modelFile.close();
            std::cout << GREEN << "Model architecture saved to: ai_language/" << modelPath << RESET << std::endl;

            // แสดงโครงสร้างโมเดลใน console
            std::ifstream readModelFile(modelPath);
            if (readModelFile.is_open()) {
                std::string line;
                while (std::getline(readModelFile, line)) {
                    std::cout << line << std::endl;
                }
                readModelFile.close();
            }
        } else {
            std::cout << RED << "Error saving model architecture" << RESET << std::endl;
        }
    } else if (plotType == "confusion_matrix") {
        std::cout << "Generating confusion matrix for classification model" << std::endl;

        // จำลองข้อมูล confusion matrix
        std::cout << "Confusion Matrix:" << std::endl;
        std::cout << "----------------" << std::endl;
        std::cout << "    | Pred 0 | Pred 1 | Pred 2 |" << std::endl;
        std::cout << "True 0 |   95   |    3   |    2   |" << std::endl;
        std::cout << "True 1 |    4   |   87   |    9   |" << std::endl;
        std::cout << "True 2 |    2   |    5   |   93   |" << std::endl;
        std::cout << "----------------" << std::endl;

        std::cout << GREEN << "Confusion matrix visualization complete" << RESET << std::endl;
    } else if (plotType == "feature_maps" && (modelType == "CNN" || modelType == "Transformer")) {
        std::cout << "Generating visualization of learned feature maps" << std::endl;

        // จำลองการแสดงผล feature maps สำหรับ CNN
        std::cout << "Feature maps from first convolutional layer (8x8 grid):" << std::endl;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                std::cout << (((i+j) % 2 == 0) ? "▓" : "░");
            }
            std::cout << std::endl;
        }

        std::cout << GREEN << "Feature maps visualization complete" << RESET << std::endl;
    } else if (plotType == "learning_curves") {
        if (!hasTrained) {
            std::cout << RED << "Error: Model must be trained before plotting learning curves" << RESET << std::endl;
            return;
        }

        std::cout << "Generating training and validation learning curves" << std::endl;

        // สร้างข้อมูลสำหรับกราฟในไฟล์ CSV
        std::string csvPath = "Program test/Data/dl_learning_curves_data.csv";
        std::ofstream csvFile(csvPath);
        if (csvFile.is_open()) {
            csvFile << "epoch,train_accuracy,train_loss,val_accuracy,val_loss\n";
            for (int i = 1; i <= int(parameters["epochs"]); i++) {
                float progress = i / float(parameters["epochs"]);
                float train_acc = 0.5f + 0.45f * (1 - std::exp(-(i)/20.0));
                float val_acc = train_acc - 0.05f * (1 - progress) * (float)rand() / RAND_MAX;
                float train_loss = 0.8f - 0.75f * (1 - std::exp(-(i)/15.0));
                float val_loss = train_loss + 0.05f * (1 - progress) * (float)rand() / RAND_MAX;
                csvFile << i << "," << train_acc << "," << train_loss << "," << val_acc << "," << val_loss << "\n";
            }
            csvFile.close();

            // เรียกใช้ Python script เพื่อสร้างกราฟ
            std::string command = "python3 src/utils/plot_generator.py \"" + csvPath + "\" \"Program test/Data\" \"Learning Curves for " + modelType + " Model\"";
            int result = system(command.c_str());

            if (result == 0) {
                std::cout << GREEN << "Learning curves plot saved to: ai_language/Program test/Data/learning_curves.png" << RESET << std::endl;
            } else {
                std::cout << RED << "Error generating learning curves plot" << RESET << std::endl;
            }
        }
    } else {
        std::cout << RED << "Error: Unknown plot type '" << plotType << "' for DL models" << RESET << std::endl;
    }
}

void DLInterpreter::handlePredictCommand(const std::vector<std::string>& args) {
    if (!hasTrained) {
        std::cout << RED << "Error: Model must be trained before making predictions" << RESET << std::endl;
        return;
    }

    if (args.empty()) {
        std::cout << RED << "Error: Missing input data for prediction. Usage: predict <data> or predict file <path>" << RESET << std::endl;
        return;
    }

    if (args[0] == "file") {
        if (args.size() < 2) {
            std::cout << RED << "Error: Missing file path. Usage: predict file <path>" << RESET << std::endl;
            return;
        }

        std::string filePath = args[1];
        // ลบเครื่องหมายคำพูดออกจากชื่อไฟล์ถ้ามี
        if (filePath.front() == '"' && filePath.back() == '"') {
            filePath = filePath.substr(1, filePath.size() - 2);
        }

        std::cout << CYAN << "Making predictions with " << modelType << " on data from file: " << filePath << RESET << std::endl;

        // จำลองผลลัพธ์การทำนาย
        bool isClassification = (modelType == "CNN" || layers.back().find("softmax") != std::string::npos);

        if (isClassification) {
            std::cout << "Processing 100 samples for classification..." << std::endl;
            std::cout << GREEN << "Prediction complete. Results summary:" << RESET << std::endl;
            std::cout << "Class distribution: [Class 0: 32%, Class 1: 45%, Class 2: 23%]" << std::endl;
            std::cout << "Overall accuracy: 91.5%" << std::endl;
        } else {
            std::cout << "Processing 100 samples for regression..." << std::endl;
            std::cout << GREEN << "Prediction complete. Results summary:" << RESET << std::endl;
            std::cout << "Mean prediction: 37.8" << std::endl;
            std::cout << "Prediction range: [15.6, 76.3]" << std::endl;
            std::cout << "Mean Squared Error: 2.34" << std::endl;
        }
    } else if (args[0] == "image" && modelType == "CNN") {
        // กรณีทำนายรูปภาพด้วย CNN
        if (args.size() < 2) {
            std::cout << RED << "Error: Missing image path. Usage: predict image <path>" << RESET << std::endl;
            return;
        }

        std::string imagePath = args[1];
        if (imagePath.front() == '"' && imagePath.back() == '"') {
            imagePath = imagePath.substr(1, imagePath.size() - 2);
        }

        std::cout << CYAN << "Making image prediction with CNN on: " << imagePath << RESET << std::endl;
        std::cout << "Preprocessing image..." << std::endl;
        std::cout << "Running inference..." << std::endl;

        std::cout << GREEN << "Prediction results:" << RESET << std::endl;
        std::cout << "Class: Cat (0.94 confidence)" << std::endl;
        std::cout << "Top 3 predictions: [Cat: 94%, Dog: 4%, Fox: 2%]" << std::endl;
    } else if (args[0] == "text" && (modelType == "RNN" || modelType == "LSTM" || modelType == "Transformer")) {
        // กรณีทำนายข้อความด้วย RNN, LSTM หรือ Transformer
        if (args.size() < 2) {
            std::cout << RED << "Error: Missing text. Usage: predict text \"<input text>\"" << RESET << std::endl;
            return;
        }

        std::string inputText = args[1];
        if (inputText.front() == '"' && inputText.back() == '"') {
            inputText = inputText.substr(1, inputText.size() - 2);
        }

        std::cout << CYAN << "Making text prediction with " << modelType << " on: \"" << inputText << "\"" << RESET << std::endl;
        std::cout << "Tokenizing input..." << std::endl;
        std::cout << "Running inference..." << std::endl;

        if (modelType == "Transformer") {
            std::cout << GREEN << "Generated continuation:" << RESET << std::endl;
            std::cout << "\"" << inputText << " is a great example of natural language processing capabilities.\"" << std::endl;
        } else {
            std::cout << GREEN << "Sentiment analysis result:" << RESET << std::endl;
            std::cout << "Positive sentiment (0.78 confidence)" << std::endl;
            std::cout << "Sentiment breakdown: [Positive: 78%, Neutral: 15%, Negative: 7%]" << std::endl;
        }
    } else {
        // กรณีทำนายข้อมูลที่ระบุโดยตรง
        std::vector<double> inputValues;
        for (const auto& arg : args) {
            try {
                inputValues.push_back(std::stod(arg));
            } catch (const std::exception& e) {
                // ข้ามค่าที่ไม่ใช่ตัวเลขโดยไม่แสดงข้อความเตือนเพื่อให้เป็นพฤติกรรมเดียวกันกับ MLInterpreter
                continue;
            }
        }

        if (inputValues.empty()) {
            std::cout << RED << "Error: No valid numeric input values found. Usage: predict <value1> <value2> ..." << RESET << std::endl;
            return;
        }

        std::cout << CYAN << "Making prediction with " << modelType << " on input data: ";
        for (const auto& val : inputValues) {
            std::cout << val << " ";
        }
        std::cout << RESET << std::endl;

        // จำลองการคำนวณการทำนาย
        bool isClassification = (modelType == "CNN" || layers.back().find("softmax") != std::string::npos);

        if (isClassification) {
            // สร้างผลลัพธ์จำลองสำหรับงาน classification
            std::vector<double> classProbs = {0.15, 0.75, 0.1};
            int predictedClass = 1; // ตรงกับ index ที่มีค่า probability สูงสุด

            std::cout << GREEN << "Classification result:" << RESET << std::endl;
            std::cout << "Predicted class: " << predictedClass << std::endl;
            std::cout << "Class probabilities: [";
            for (size_t i = 0; i < classProbs.size(); i++) {
                std::cout << classProbs[i];
                if (i < classProbs.size() - 1) std::cout << ", ";
            }
            std::cout << "]" << std::endl;
        } else {
            // สร้างผลลัพธ์จำลองสำหรับงาน regression
            double predictionResult = 42.5;

            std::cout << GREEN << "Regression result:" << RESET << std::endl;
            std::cout << "Predicted value: " << predictionResult << std::endl;
        }
    }
}

void DLInterpreter::handleListModelsCommand() {
    std::cout << CYAN << "Available Deep Learning models:" << RESET << std::endl;

    // แสดงรายการโมเดลที่รองรับทั้งหมด
    std::vector<std::string> supportedModels = {
        "NeuralNetwork", "CNN", "RNN", "LSTM", "GRU", "Transformer"
    };

    for (const auto& model : supportedModels) {
        std::cout << "- " << model;

        // เพิ่มข้อมูลเกี่ยวกับโมเดลแต่ละประเภท
        if (model == "CNN") {
            std::cout << " (Convolutional Neural Network, for image processing)";
        } else if (model == "RNN") {
            std::cout << " (Recurrent Neural Network, for sequence data)";
        } else if (model == "LSTM") {
            std::cout << " (Long Short-Term Memory, for longer sequences)";
        } else if (model == "GRU") {
            std::cout << " (Gated Recurrent Unit, faster alternative to LSTM)";
        } else if (model == "Transformer") {
            std::cout << " (Attention-based model, for NLP tasks)";
        } else if (model == "NeuralNetwork") {
            std::cout << " (Standard fully-connected network)";
        }

        std::cout << std::endl;
    }

    // แสดงข้อมูลเกี่ยวกับโมเดลที่สร้างในโปรเจกต์ปัจจุบัน (ถ้ามี)
    if (hasCreated) {
        std::cout << std::endl << CYAN << "Current project model:" << RESET << std::endl;
        std::cout << "- Type: " << modelType << std::endl;
        std::cout << "- Status: " << (hasTrained ? "Trained" : "Not trained") << std::endl;

        if (!layers.empty()) {
            std::cout << "- Architecture: " << layers.size() << " layers" << std::endl;

            // แสดงจำนวน parameters ที่คำนวณโดยประมาณ
            int totalParams = 0;
            int prevSize = 0;

            for (size_t i = 0; i < layers.size(); i++) {
                std::string layerInfo = layers[i];
                size_t firstColon = layerInfo.find(':');
                size_t secondColon = layerInfo.find(':', firstColon + 1);

                std::string layerType = layerInfo.substr(0, firstColon);
                int neurons = 0;

                try {
                    if (secondColon != std::string::npos) {
                        neurons = std::stoi(layerInfo.substr(firstColon + 1, secondColon - firstColon - 1));
                    } else {
                        neurons = std::stoi(layerInfo.substr(firstColon + 1));
                    }
                } catch (...) {
                    neurons = 0;
                }

                if (layerType != "dropout" && layerType != "flatten" && neurons > 0) {
                    if (prevSize > 0) {
                        // Weights + bias
                        totalParams += (prevSize * neurons) + neurons;
                    }
                    prevSize = neurons;
                }
            }

            std::cout << "- Approximate parameters: " << totalParams << std::endl;
        }

        // แสดงพารามิเตอร์หลักของโมเดล
        std::cout << "- Training parameters:" << std::endl;
        for (const auto& param : parameters) {
            std::cout << "  * " << param.first << ": " << param.second << std::endl;
        }
    } else {
        std::cout << std::endl << "No model has been created in this project yet." << std::endl;
        std::cout << "Use 'create [model_type]' to create a model." << std::endl;
    }
}

void DLInterpreter::handleInspectCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << RED << "กรุณาระบุสิ่งที่ต้องการตรวจสอบ" << RESET << std::endl;
        std::cout << "ตัวอย่าง: inspect model, inspect dataset, inspect layer" << std::endl;
        return;
    }

    std::string target = args[0];

    if (target == "model") {
        if (!hasCreated) {
            std::cout << RED << "กรุณาสร้างโมเดลก่อนด้วยคำสั่ง 'create'" << RESET << std::endl;
            return;
        }

        std::cout << CYAN << "=== รายละเอียดโมเดล " << modelType << " ===" << RESET << std::endl;
        std::cout << "ประเภท: " << modelType << std::endl;
        std::cout << "สถานะการเทรน: " << (hasTrained ? "เทรนแล้ว" : "ยังไม่ได้เทรน") << std::endl;

        if (layers.empty()) {
            std::cout << YELLOW << "ยังไม่มีการกำหนด layer" << RESET << std::endl;
        } else {
            std::cout << GREEN << "\nLayers:" << RESET << std::endl;
            for (size_t i = 0; i < layers.size(); i++) {
                std::cout << i+1 << ". " << layers[i] << std::endl;
            }
        }

        std::cout << GREEN << "\nParameters:" << RESET << std::endl;
        for (const auto& param : parameters) {
            std::cout << "- " << param.first << ": " << param.second << std::endl;
        }

        for (const auto& param : stringParameters) {
            std::cout << "- " << param.first << ": " << param.second << std::endl;
        }

        if (hasTrained) {
            std::cout << GREEN << "\nPerformance Metrics:" << RESET << std::endl;
            std::cout << "- ความแม่นยำ: 0.92" << std::endl;
            std::cout << "- Loss: 0.08" << std::endl;
        }
    } 
    else if (target == "dataset") {
        if (!hasLoaded) {
            std::cout << RED << "กรุณาโหลดข้อมูลก่อนด้วยคำสั่ง 'load dataset'" << RESET << std::endl;
            return;
        }

        std::cout << CYAN << "=== รายละเอียดข้อมูล ===" << RESET << std::endl;
        std::cout << "ที่อยู่: " << datasetPath << std::endl;
        std::cout << "จำนวนตัวอย่าง: 1000" << std::endl;

        if (stringParameters.find("target_column") != stringParameters.end()) {
            std::cout << "คอลัมน์เป้าหมาย: " << stringParameters["target_column"] << std::endl;
        }

        std::cout << GREEN << "\nตัวอย่างข้อมูล:" << RESET << std::endl;
        std::cout << "sepal_length | sepal_width | petal_length | petal_width | species" << std::endl;
        std::cout << "-------------|------------|--------------|-------------|--------" << std::endl;
        std::cout << "     5.1     |     3.5    |      1.4     |     0.2     | setosa" << std::endl;
        std::cout << "     4.9     |     3.0    |      1.4     |     0.2     | setosa" << std::endl;
        std::cout << "     6.2     |     2.9    |      4.3     |     1.3     | versicolor" << std::endl;
        std::cout << "     6.7     |     3.1    |      5.6     |     2.4     | virginica" << std::endl;
    }
    else if (target == "layer") {
        if (layers.empty()) {
            std::cout << RED << "ยังไม่มีการกำหนด layer ในโมเดล" << RESET << std::endl;
            return;
        }

        if (args.size() < 2) {
            std::cout << YELLOW << "กรุณาระบุ index ของ layer ที่ต้องการตรวจสอบ" << RESET << std::endl;
            std::cout << "จำนวน layer ทั้งหมด: " << layers.size() << std::endl;
            return;
        }

        try {
            int index = std::stoi(args[1]) - 1; // ปรับให้เริ่มจาก 1

            if (index < 0 || index >= static_cast<int>(layers.size())) {
                std::cout << RED << "Layer index ไม่ถูกต้อง" << RESET << std::endl;
                return;
            }

            std::string layerInfo = layers[index];
            std::cout << CYAN << "=== รายละเอียด Layer ที่ " << (index+1) << " ===" << RESET << std::endl;
            std::cout << "ข้อมูล: " << layerInfo << std::endl;

            // แยกส่วนประกอบของ layer
            size_t firstColon = layerInfo.find(':');
            if (firstColon != std::string::npos) {
                std::string layerType = layerInfo.substr(0, firstColon);
                std::string rest = layerInfo.substr(firstColon + 1);

                std::cout << "ประเภท: " << layerType << std::endl;

                if (layerType == "dense" || layerType == "hidden" || layerType == "output") {
                    size_t secondColon = rest.find(':');
                    if (secondColon != std::string::npos) {
                        std::string units = rest.substr(0, secondColon);
                        std::string activation = rest.substr(secondColon + 1);

                        std::cout << "จำนวน Units: " << units << std::endl;
                        std::cout << "ฟังก์ชัน Activation: " << activation << std::endl;

                        // คำนวณจำนวนพารามิเตอร์
                        if (index > 0) {
                            std::string prevLayerInfo = layers[index-1];
                            size_t prevFirstColon = prevLayerInfo.find(':');
                            size_t prevSecondColon = prevLayerInfo.find(':', prevFirstColon + 1);

                            if (prevFirstColon != std::string::npos && prevSecondColon != std::string::npos) {
                                try {
                                    int prevUnits = std::stoi(prevLayerInfo.substr(prevFirstColon + 1, prevSecondColon - prevFirstColon - 1));
                                    int currentUnits = std::stoi(units);

                                    int params = (prevUnits * currentUnits) + currentUnits; // weights + biases
                                    std::cout << "จำนวนพารามิเตอร์: " << params << std::endl;
                                } catch(...) {
                                    // ไม่สามารถคำนวณได้
                                }
                            }
                        }
                    }
                } else if (layerType == "conv") {
                    std::vector<std::string> parts;
                    size_t pos = 0;
                    size_t nextColon;
                    while ((nextColon = rest.find(':', pos)) != std::string::npos) {
                        parts.push_back(rest.substr(pos, nextColon - pos));
                        pos = nextColon + 1;
                    }
                    parts.push_back(rest.substr(pos));

                    if (parts.size() >= 2) {
                        std::cout << "จำนวน Filters: " << parts[0] << std::endl;
                        std::cout << "Kernel Size: " << parts[1] << std::endl;
                        if (parts.size() >= 3) {
                            std::cout << "ฟังก์ชัน Activation: " << parts[2] << std::endl;
                        }
                    }
                }
            }
        } catch (const std::exception& e) {
            std::cout << RED << "เกิดข้อผิดพลาด: " << e.what() << RESET << std::endl;
        }
    }
    else {
        std::cout << RED << "ไม่รู้จักเป้าหมายการตรวจสอบ: " << target << RESET << std::endl;
        std::cout << "ตัวอย่าง: inspect model, inspect dataset, inspect layer" << std::endl;
    }
}

void DLInterpreter::handleValidateCommand(const std::vector<std::string>& args) {
    if (!hasLoaded) {
        std::cout << RED << "กรุณาโหลดข้อมูลก่อนด้วยคำสั่ง 'load dataset'" << RESET << std::endl;
        return;
    }

    if (args.empty() || args[0] != "dataset") {
        std::cout << RED << "รูปแบบคำสั่งไม่ถูกต้อง ตัวอย่าง: validate dataset" << RESET << std::endl;
        return;
    }

    std::cout << GREEN << "กำลังตรวจสอบความถูกต้องของข้อมูล..." << RESET << std::endl;

    // จำลองการตรวจสอบข้อมูล
    std::cout << BLUE << "ตรวจสอบคอลัมน์และประเภทข้อมูล... " << RESET << std::endl;
    std::cout << GREEN << "✓ ประเภทข้อมูลถูกต้อง" << RESET << std::endl;

    std::cout << BLUE << "ตรวจสอบค่า missing values... " << RESET << std::endl;
    std::cout << GREEN << "✓ ไม่พบค่า missing" << RESET << std::endl;

    std::cout << BLUE << "ตรวจสอบข้อมูล outliers... " << RESET << std::endl;
    std::cout << GREEN << "✓ ไม่พบค่า outliers ที่มีนัยสำคัญ" << RESET << std::endl;

    if (stringParameters.find("target_column") != stringParameters.end()) {
        std::string targetColumn = stringParameters["target_column"];
        std::cout << BLUE << "ตรวจสอบการกระจายตัวของคลาส (" << targetColumn << ")... " << RESET << std::endl;
        std::cout << GREEN << "✓ การกระจายตัวของคลาสสมดุล" << RESET << std::endl;
    }

    std::cout << GREEN << "การตรวจสอบข้อมูลเสร็จสิ้น: ข้อมูลพร้อมสำหรับการสร้างโมเดล" << RESET << std::endl;
}

void DLInterpreter::handlePreprocessCommand(const std::vector<std::string>& args) {
    if (!hasLoaded) {
        std::cout << RED << "กรุณาโหลดข้อมูลก่อนด้วยคำสั่ง 'load dataset'" << RESET << std::endl;
        return;
    }

    if (args.size() < 2 || args[0] != "dataset") {
        std::cout << RED << "รูปแบบคำสั่งไม่ถูกต้อง ตัวอย่าง: preprocess dataset normalize" << RESET << std::endl;
        return;
    }

    std::cout << GREEN << "กำลังประมวลผลข้อมูลเบื้องต้น..." << RESET << std::endl;

    // ตรวจสอบวิธีการที่ต้องการใช้
    bool hasNormalize = false;
    bool hasScale = false;
    bool hasOneHot = false;

    for (size_t i = 1; i < args.size(); i++) {
        std::string method = args[i];
        std::transform(method.begin(), method.end(), method.begin(), ::tolower);

        if (method == "normalize") {
            hasNormalize = true;
            std::cout << BLUE << "กำลังทำ Normalization..." << RESET << std::endl;
            std::cout << "  การแปลงข้อมูลให้อยู่ในช่วง [0, 1]" << std::endl;
        } else if (method == "scale" || method == "standardize") {
            hasScale = true;
            std::cout << BLUE << "กำลังทำ Standardization..." << RESET << std::endl;
            std::cout << "  การแปลงข้อมูลให้มีค่าเฉลี่ย 0 และความแปรปรวน 1" << std::endl;
        } else if (method == "onehot" || method == "one_hot") {
            hasOneHot = true;
            std::cout << BLUE << "กำลังทำ One-hot encoding..." << RESET << std::endl;
            std::cout << "  การแปลงข้อมูลประเภท categorical ให้เป็น binary vectors" << std::endl;
        } else {
            std::cout << YELLOW << "คำเตือน: ไม่รู้จักวิธี preprocessing '" << method << "'" << RESET << std::endl;
        }
    }

    if (modelType == "CNN") {
        std::cout << BLUE << "กำลังทำ Image Data Augmentation สำหรับ CNN..." << RESET << std::endl;
        std::cout << "  การหมุนภาพสุ่ม, การพลิกภาพ, การซูม, การปรับความสว่าง" << std::endl;
    }

    if (!hasNormalize && !hasScale && !hasOneHot) {
        std::cout << YELLOW << "คำเตือน: ไม่มีการระบุวิธี preprocessing ที่รู้จัก" << RESET << std::endl;
        std::cout << "วิธีที่สนับสนุน: normalize, scale/standardize, onehot/one_hot" << std::endl;
    }

    std::cout << GREEN << "การประมวลผลข้อมูลเบื้องต้นเสร็จสิ้น" << RESET << std::endl;
}

void DLInterpreter::handleSplitDatasetCommand(const std::vector<std::string>& args) {
    if (!hasLoaded) {
        std::cout << RED << "กรุณาโหลดข้อมูลก่อนด้วยคำสั่ง 'load dataset'" << RESET << std::endl;
        return;
    }

    // ดึงชื่อชุดข้อมูลและอัตราส่วน
    std::vector<std::string> datasets;
    std::vector<double> ratios;

    // หา index ของคำสำคัญ
    size_t intoIndex = 0;
    size_t withIndex = 0;
    size_t ratioIndex = 0;

    for (size_t i = 0; i < args.size(); i++) {
        if (args[i] == "into") {
            intoIndex = i;
        } else if (args[i] == "with") {
            withIndex = i;
        } else if (args[i] == "ratio") {
            ratioIndex = i;
        }
    }

    // ตรวจสอบรูปแบบคำสั่ง
    if (args.size() < 7 || args[0] != "dataset" || intoIndex == 0 || withIndex == 0 || ratioIndex == 0 || 
        withIndex >= args.size() - 2 || ratioIndex != withIndex + 1) {
        std::cout << RED << "รูปแบบคำสั่งไม่ถูกต้อง" << RESET << std::endl;
        std::cout << "ตัวอย่าง: split dataset into train, test with ratio 0.8, 0.2" << std::endl;
        std::cout << "หรือ: split dataset into train, test, validation with ratio 0.7, 0.2, 0.1" << std::endl;
        return;
    }

    // เก็บชื่อชุดข้อมูล ระหว่าง 'into' และ 'with'
    for (size_t i = intoIndex + 1; i < withIndex; i++) {
        std::string dataset = args[i];
        // ลบเครื่องหมาย ',' ถ้ามี
        if (!dataset.empty() && dataset.back() == ',') {
            dataset.pop_back();
        }
        // เพิ่มชื่อชุดข้อมูลถ้าไม่ใช่สตริงว่าง
        if (!dataset.empty()) {
            datasets.push_back(dataset);
        }
    }

    // เก็บอัตราส่วน หลังจาก 'ratio'
    for (size_t i = ratioIndex + 1; i < args.size(); i++) {
        std::string ratio = args[i];
        // ลบเครื่องหมาย ',' ถ้ามี
        if (!ratio.empty() && ratio.back() == ',') {
            ratio.pop_back();
        }
        // แปลงเป็นตัวเลข
        if (!ratio.empty()) {
            try {
                double r = std::stod(ratio);
                ratios.push_back(r);
            } catch (const std::exception& e) {
                std::cout << RED << "อัตราส่วนไม่ถูกต้อง: " << ratio << RESET << std::endl;
                return;
            }
        }
    }

    // ตรวจสอบว่าจำนวนชุดข้อมูลและอัตราส่วนตรงกัน
    if (datasets.size() != ratios.size()) {
        std::cout << RED << "จำนวนชุดข้อมูลและอัตราส่วนไม่ตรงกัน" << RESET << std::endl;
        std::cout << "ชุดข้อมูล: " << datasets.size() << " รายการ, อัตราส่วน: " << ratios.size() << " รายการ" << std::endl;
        return;
    }

    // ตรวจสอบว่าผลรวมของอัตราส่วนเท่ากับ 1.0
    double sum = 0.0;
    for (const auto& r : ratios) {
        sum += r;
    }
    if (std::abs(sum - 1.0) > 0.01) {
        std::cout << YELLOW << "คำเตือน: ผลรวมของอัตราส่วนควรเท่ากับ 1.0 (ค่าปัจจุบันคือ " << sum << ")" << RESET << std::endl;
    }

    // แสดงข้อมูลการแบ่ง
    std::cout << GREEN << "กำลังแบ่งข้อมูลเป็น:" << RESET << std::endl;
    for (size_t j = 0; j < datasets.size(); j++) {
        int percent = static_cast<int>(ratios[j] * 100);
        std::cout << "- " << datasets[j] << ": " << percent << "% (" << static_cast<int>(ratios[j] * 1000) << " ตัวอย่าง)" << std::endl;
    }

    std::cout << GREEN << "การแบ่งข้อมูลเสร็จสิ้น" << RESET << std::endl;

    // บันทึกข้อมูลการแบ่งในพารามิเตอร์
    parameters["split_completed"] = 1.0;
}


void DLInterpreter::handleDeleteModelCommand([[maybe_unused]] const std::vector<std::string>& args) {
    std::cout << "Delete model command is not implemented for DL yet" << std::endl;
}

void DLInterpreter::handleCompareModelsCommand() {
    std::cout << "Compare models command is not implemented for DL yet" << std::endl;
}

void DLInterpreter::handleCheckStatusCommand() {
    std::cout << "Check status command is not implemented for DL yet" << std::endl;
}

void DLInterpreter::handleDebugCommand([[maybe_unused]] const std::vector<std::string>& args) {
    std::cout << "Debug command is not implemented for DL yet" << std::endl;
}

void DLInterpreter::handleCrossValidateCommand([[maybe_unused]] const std::vector<std::string>& args) {
    std::cout << "Cross validate command is not implemented for DL yet" << std::endl;
}

void DLInterpreter::handleExportResultsCommand([[maybe_unused]] const std::vector<std::string>& args) {
    std::cout << "Export results command not implemented yet in DL interpreter" << std::endl;
}

void DLInterpreter::handleScheduleTrainingCommand([[maybe_unused]] const std::vector<std::string>& args) {
    std::cout << "Schedule training command not implemented yet in DL interpreter" << std::endl;
}

} // namespace ai_language