#include "../include/MLInterpreter.h"
#include <random>
#include <iomanip>
#include <fstream>
#include <thread>
#include <chrono>

namespace ai_language {

MLInterpreter::MLInterpreter() {
    projectType = "ML";
    useScikitLearn = false; // Default to not using scikit-learn
}

MLInterpreter::~MLInterpreter() {
}

void MLInterpreter::setDefaultParameters() {
    // ล้างค่าเดิม
    parameters.clear();

    // กำหนดค่าเริ่มต้นสำหรับ ML
    parameters["learning_rate"] = 0.01;
    parameters["epochs"] = 50;
    parameters["batch_size"] = 32;
}

void MLInterpreter::handleStartCommand() {
    hasStarted = true;
    std::cout << GREEN << "Program started" << RESET << std::endl;
}

void MLInterpreter::handleCreateCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (args.size() < 1) {
        std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'create [type]' หรือ 'create model [model_name]'" << RESET << std::endl;
        return;
    }

    if (args[0] == "ML") {
        // สร้างโปรเจค
        if (hasCreatedProject) {
            std::cerr << YELLOW << "คำเตือน: กำลังสร้างโปรเจคใหม่ ข้อมูลเดิมจะถูกรีเซ็ต" << RESET << std::endl;
        }

        projectType = args[0];
        hasCreatedProject = true;
        hasLoadedData = false;
        hasCreatedModel = false;
        hasTrainedModel = false;
        hasShowedAccuracy = false;
        hasSavedModel = false;

        // ตั้งค่าพารามิเตอร์เริ่มต้น
        setDefaultParameters();

        std::cout << GREEN << "Project created: Machine Learning" << RESET << std::endl;
    } else if (args[0] == "model") {
        if (!hasCreatedProject) {
            std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create [type]' ก่อน" << RESET << std::endl;
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
        std::cerr << RED << "ข้อผิดพลาด: ประเภท '" << args[0] << "' ไม่ถูกต้อง ต้องเป็น 'ML' หรือ 'model'" << RESET << std::endl;
    }
}

void MLInterpreter::handleModelCreation(const std::string& modelName) {
    if (hasCreatedModel) {
        std::cerr << YELLOW << "คำเตือน: กำลังแทนที่โมเดลเดิม (" << modelType << ") ด้วยโมเดลใหม่" << RESET << std::endl;
    }
    modelType = modelName;

    // ตรวจสอบชื่อโมเดลที่มีวงเล็บหรือเครื่องหมายพิเศษ
    bool hasInvalidChars = false;
    for (char c : modelType) {
        if (c == '(' || c == ')' || c == '!' || c == '@' || c == '#' || c == '$' || 
            c == '%' || c == '^' || c == '&' || c == '*' || c == '+' || c == '=') {
            hasInvalidChars = true;
            break;
        }
    }

    if (hasInvalidChars) {
        std::cerr << RED << "ข้อผิดพลาด: ชื่อโมเดลมีอักขระพิเศษที่ไม่ได้รับอนุญาต" << RESET << std::endl;
        return;
    }

    // ตรวจสอบชื่อโมเดลที่รองรับ
    std::vector<std::string> supportedModels = {"LinearRegression", "LogisticRegression", "RandomForest", "SVM", "DecisionTree", "KNN"};

    bool isSupported = false;
    for (const auto& model : supportedModels) {
        if (modelType == model) {
            isSupported = true;
            break;
        }
    }

    if (!isSupported) {
        std::cerr << RED << "ข้อผิดพลาด: โมเดล '" << modelType << "' ไม่รองรับในโปรเจค " << projectType << RESET << std::endl;
        std::cerr << YELLOW << "โมเดลที่รองรับในโปรเจค " << projectType << " ได้แก่: ";
        for (size_t i = 0; i < supportedModels.size(); ++i) {
            std::cerr << supportedModels[i];
            if (i < supportedModels.size() - 1) {
                std::cerr << ", ";
            }
        }
        std::cerr << RESET << std::endl;
        return;
    }

    hasCreatedModel = true;
    hasTrainedModel = false;
    hasShowedAccuracy = false;
    hasSavedModel = false;

    std::cout << GREEN << "Model created: " << modelType << RESET << std::endl;
}

void MLInterpreter::handleLoadCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedProject) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create [type]' ก่อน" << RESET << std::endl;
        return;
    }

    if (args.size() < 2) {
        std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'load dataset \"[filename]\"' หรือ 'load model \"[filename]\"'" << RESET << std::endl;
        return;
    }

    if (args[0] == "dataset") {
        // โหลดข้อมูล
        if (args.size() < 2) {
            std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'load dataset \"[filename]\"'" << RESET << std::endl;
            return;
        }

        // แยกชื่อไฟล์จากเครื่องหมายคำพูด
        std::string filename = args[1];
        if (filename.size() >= 2 && filename.front() == '"' && filename.back() == '"') {
            filename = filename.substr(1, filename.size() - 2);
        }

        // ตรวจสอบการมีอยู่ของไฟล์
        struct stat buffer;
        if (stat(filename.c_str(), &buffer) != 0) {
            std::cerr << RED << "ข้อผิดพลาด: ไฟล์ " << filename << " ไม่พบ" << RESET << std::endl;
            return;
        }

        datasetPath = filename;
        hasLoadedData = true;
        hasCreatedModel = false;
        hasTrainedModel = false;
        hasShowedAccuracy = false;
        hasSavedModel = false;

        std::cout << GREEN << "Dataset loaded successfully" << RESET << std::endl;
    } else if (args[0] == "model") {
        // โหลดโมเดล
        if (!hasLoadedData) {
            std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'load dataset' ก่อน" << RESET << std::endl;
            return;
        }

        if (args.size() < 2) {
            std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'load model \"[filename]\"'" << RESET << std::endl;
            return;
        }

        // แยกชื่อไฟล์จากเครื่องหมายคำพูด
        std::string filename = args[1];
        if (filename.size() >= 2 && filename.front() == '"' && filename.back() == '"') {
            filename = filename.substr(1, filename.size() - 2);
        }

        // ตรวจสอบการมีอยู่ของไฟล์
        std::ifstream file(filename);
        if (!file) {
            std::cerr << RED << "ข้อผิดพลาด: ไฟล์โมเดล " << filename << " ไม่พบ" << RESET << std::endl;
            return;
        }
        file.close();

        modelType = "LoadedModel";
        hasCreatedModel = true;
        hasTrainedModel = true; // Assuming loaded models are trained
        hasShowedAccuracy = false;
        hasSavedModel = false;

        std::cout << GREEN << "Model loaded successfully: " << filename << RESET << std::endl;
    } else {
        std::cerr << RED << "ข้อผิดพลาด: ประเภทข้อมูล '" << args[0] << "' ไม่ถูกต้อง ต้องเป็น 'dataset' หรือ 'model'" << RESET << std::endl;
    }
}

void MLInterpreter::handleSetCommand(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cerr << RED << "ข้อผิดพลาด: คำสั่ง 'set' ต้องมีอย่างน้อย 2 อาร์กิวเมนต์ (ชื่อและค่า)" << RESET << std::endl;
        return;
    }

    std::string paramName = args[0];
    std::string paramValue = args[1];

    // ตรวจสอบการตั้งค่าการใช้ scikit-learn
    if (paramName == "use_scikit" || paramName == "use_scikit_learn") {
        if (paramValue == "true" || paramValue == "1" || paramValue == "yes") {
            useScikitLearn = true;
            std::cout << GREEN << "เปิดใช้งานการเชื่อมต่อกับ scikit-learn" << RESET << std::endl;
            // ตรวจสอบว่ามี scikit-learn หรือไม่
            scikitConnector.checkPythonAndScikitLearn();
        } else {
            useScikitLearn = false;
            std::cout << GREEN << "ปิดใช้งานการเชื่อมต่อกับ scikit-learn" << RESET << std::endl;
        }
        return;
    }

    try {
        // แปลงค่าเป็นตัวเลข
        double numericValue = std::stod(paramValue);
        parameters[paramName] = numericValue;
        std::cout << GREEN << "ตั้งค่า " << paramName << " = " << numericValue << RESET << std::endl;
    } catch (const std::invalid_argument&) {
        std::cerr << RED << "ข้อผิดพลาด: ค่า '" << paramValue << "' ไม่ใช่ตัวเลขที่ถูกต้อง" << RESET << std::endl;
    }
}

void MLInterpreter::handleTrainCommand(const std::vector<std::string>& args) {
    if (!hasStarted || projectType.empty()) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' และ 'create' ก่อน" << RESET << std::endl;
        return;
    }

    if (datasetPath.empty()) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องโหลดข้อมูลก่อนด้วยคำสั่ง 'load dataset'" << RESET << std::endl;
        return;
    }

    if (modelType.empty()) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องสร้างโมเดลก่อนด้วยคำสั่ง 'create model'" << RESET << std::endl;
        return;
    }

    if (args.size() > 0 && args[0] != "model") {
        std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'train model'" << RESET << std::endl;
        return;
    }

    std::cout << BLUE << "กำลังเทรนโมเดล " << modelType << " บนข้อมูลจาก " << datasetPath << RESET << std::endl;

    // ตรวจสอบว่าใช้ scikit-learn หรือไม่
    if (useScikitLearn) {
        std::cout << YELLOW << "กำลังใช้ scikit-learn สำหรับการเทรนโมเดล..." << RESET << std::endl;
        // แปลง unordered_map เป็น map ที่ ScikitLearnConnector คาดหวัง
        std::map<std::string, double> paramsMap(parameters.begin(), parameters.end());
        scikitConnector.trainModel(modelType, datasetPath, paramsMap);
        hasTrainedModel = true;
        return;
    }

    // จำลองการเทรนโมเดลด้วย AI Language ปกติ
    std::cout << YELLOW << "เริ่มการเทรนโมเดล..." << RESET << std::endl;

    // แสดงพารามิเตอร์
    std::cout << "พารามิเตอร์ที่ใช้:" << std::endl;
    for (const auto& param : parameters) {
        std::cout << "  - " << param.first << ": " << param.second << std::endl;
    }

    // สร้างการจำลองการเทรนที่ใช้เวลา
    int epochs = static_cast<int>(parameters["epochs"]);
    for (int i = 1; i <= epochs; i++) {
        if (i % 10 == 0 || i == 1 || i == epochs) {
            double progress = static_cast<double>(i) / epochs * 100.0;
            double fakeLoss = 1.0 / (i + 1) + 0.1;
            double fakeAccuracy = 1.0 - fakeLoss;

            std::cout << "  Epoch " << i << "/" << epochs;
            std::cout << " - loss: " << std::fixed << std::setprecision(4) << fakeLoss;
            std::cout << " - accuracy: " << std::fixed << std::setprecision(4) << fakeAccuracy;
            std::cout << " (" << std::fixed << std::setprecision(1) << progress << "%)" << std::endl;

            // ถ้ามี epochs มาก ให้หลับเล็กน้อย เพื่อไม่ให้เร็วเกินไป
            if (epochs > 20) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }

    std::cout << GREEN << "เทรนโมเดลเสร็จสมบูรณ์" << RESET << std::endl;

    hasTrainedModel = true;
}

void MLInterpreter::handleShowCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedProject) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create [type]' ก่อน" << RESET << std::endl;
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

    if (!hasTrainedModel) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'train model' ก่อน" << RESET << std::endl;
        return;
    }

    if (args.size() < 1) {
        std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'show [result_type]'" << RESET << std::endl;
        return;
    }

    std::string resultType = args[0];

    if (resultType == "accuracy") {
        // สุ่มค่าความแม่นยำระหว่าง 80.0 - 99.9
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(80.0, 99.9);
        double accuracy = dis(gen);

        std::cout << GREEN << "Accuracy: " << std::fixed << std::setprecision(2) << accuracy << "%" << RESET << std::endl;
        hasShowedAccuracy = true;
    } else if (resultType == "loss") {
        // สุ่มค่า loss ระหว่าง 0.01 - 0.5
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.01, 0.5);
        double loss = dis(gen);

        std::cout << GREEN << "Loss: " << std::fixed << std::setprecision(4) << loss << RESET << std::endl;
    } else if (resultType == "graph" || resultType == "data") {
        std::cout << GREEN << "Visualization displayed." << RESET << std::endl;

        // แสดง ASCII art graph ง่ายๆ
        std::cout << BLUE << "    ^" << std::endl;
        std::cout << "    |" << std::endl;
        std::cout << "    |   *" << std::endl;
        std::cout << "    |     *" << std::endl;
        std::cout << "    |       *" << std::endl;
        std::cout << "    |         *" << std::endl;
        std::cout << "    |           *" << std::endl;
        std::cout << "    |              *" << std::endl;
        std::cout << "    |                 *" << std::endl;
        std::cout << "    +-------------------------------->" << RESET << std::endl;
    } else {
        std::cerr << YELLOW << "คำเตือน: ประเภทผลลัพธ์ '" << resultType << "' ไม่รองรับ ใช้ 'accuracy', 'loss' หรือ 'graph'" << RESET << std::endl;
    }
}

void MLInterpreter::handleSaveCommand(const std::vector<std::string>& args) {
    if (!hasStarted) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
        return;
    }

    if (!hasCreatedProject) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create [type]' ก่อน" << RESET << std::endl;
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

    if (!hasTrainedModel) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'train model' ก่อน" << RESET << std::endl;
        return;
    }

    if (args.size() < 2 || args[0] != "model") {
        std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'save model \"[filename]\"'" << RESET << std::endl;
        return;
    }

    std::string filename = args[1];

    // ลบเครื่องหมายคำพูดออก
    if (filename.front() == '"' && filename.back() == '"') {
        filename = filename.substr(1, filename.length() - 2);
    }

    // ตรวจสอบและเพิ่มนามสกุลไฟล์ถ้าไม่มี
    std::string extension;
    size_t dotPos = filename.find_last_of('.');
    if (dotPos == std::string::npos) {
        // ถ้าไม่มีนามสกุล ให้เพิ่มตามประเภทโมเดล
        extension = ".mlmodel";
        filename += extension;
    }

    // สร้างโฟลเดอร์ Program test/model ถ้ายังไม่มี
    std::string modelsDir = "Program test/model";
    struct stat st;
    if (stat(modelsDir.c_str(), &st) != 0) {
        // สร้างโฟลเดอร์ถ้าไม่มี
        #ifdef _WIN32
        if (system("mkdir -p \"Program test/model\"") != 0) {
            std::cerr << RED << "ข้อผิดพลาด: ไม่สามารถสร้างโฟลเดอร์ \"Program test/model\"" << RESET << std::endl;
        }
        #else
        if (system("mkdir -p \"Program test/model\"") != 0) {
            std::cerr << RED << "ข้อผิดพลาด: ไม่สามารถสร้างโฟลเดอร์ \"Program test/model\"" << RESET << std::endl;
        }
        #endif
    }

    // จัดการเส้นทางไฟล์
    if (filename.find('/') == std::string::npos && filename.find('\\') == std::string::npos) {
        // ถ้าไม่ได้ระบุเส้นทาง ให้เก็บในโฟลเดอร์ Program test/model
        filename = modelsDir + "/" + filename;
    }

    // เขียนไฟล์จริง (เติมข้อมูลจำลอง)
    std::ofstream outfile(filename);
    if (outfile.is_open()) {
        outfile << "// " << projectType << " Model: " << modelType << std::endl;
        outfile << "// Created: " << getCurrentDateTime() << std::endl;
        outfile << "// Parameters:" << std::endl;
        for (const auto& param : parameters) {
            outfile << "//   " << param.first << ": " << param.second << std::endl;
        }
        outfile << std::endl;
        outfile << "// Model data (binary/serialized) would be here in a real implementation" << std::endl;
        outfile.close();

        std::cout << GREEN << "บันทึกโมเดลไปที่ " << filename << " สำเร็จ" << RESET << std::endl;
        hasSavedModel = true;
    } else {
        std::cerr << RED << "ข้อผิดพลาด: ไม่สามารถเขียนไฟล์ " << filename << " ได้" << RESET << std::endl;
    }
}

void MLInterpreter::handleDataVisualization() const {
    std::cout << GREEN << "Visualization:" << RESET << std::endl;

    // แสดง ASCII art scatter plot ง่ายๆ
    std::cout << BLUE << "    ^" << std::endl;
    std::cout << "    |  * *" << std::endl;
    std::cout << "    |    *  *" << std::endl;
    std::cout << "    | *    *" << std::endl;
    std::cout << "    |     *   *" << std::endl;
    std::cout << "    |  *     *" << std::endl;
    std::cout << "    |       *  *" << std::endl;
    std::cout << "    |  *  *    *" << std::endl;
    std::cout << "    |           *" << std::endl;
    std::cout << "    +-------------------------------->" << RESET << std::endl;
}

void MLInterpreter::handleModelEvaluation() {
    if (!hasTrainedModel) {
        std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'train model' ก่อน" << RESET << std::endl;
        return;
    }

    // สุ่มค่าสำหรับเมทริกต่างๆ
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> acc_dis(80.0, 99.9);
    std::uniform_real_distribution<> loss_dis(0.01, 0.5);
    std::uniform_real_distribution<> f1_dis(0.7, 0.99);

    double accuracy = acc_dis(gen);
    double loss = loss_dis(gen);
    double precision = acc_dis(gen);
    double recall = acc_dis(gen);
    double f1_score = f1_dis(gen);

    std::cout << GREEN << "Model Evaluation:" << RESET << std::endl;
    std::cout << BLUE << "  Accuracy: " << std::fixed << std::setprecision(2) << accuracy << "%" << RESET << std::endl;
    std::cout << BLUE << "  Loss: " << std::fixed << std::setprecision(4) << loss << RESET << std::endl;
    std::cout << BLUE << "  Precision: " << std::fixed << std::setprecision(2) << precision << "%" << RESET << std::endl;
    std::cout << BLUE << "  Recall: " << std::fixed << std::setprecision(2) << recall << "%" << RESET << std::endl;
    std::cout << BLUE << "  F1 Score: " << std::fixed << std::setprecision(2) << f1_score << RESET << std::endl;
}

void MLInterpreter::handleHelpCommand() {
    std::cout << CYAN << "คำสั่งที่ใช้ได้ใน Machine Learning:" << RESET << std::endl;
    std::cout << "  start                       - เริ่มต้นโปรแกรม" << std::endl;
    std::cout << "  create ML                   - สร้างโปรเจค ML" << std::endl;
    std::cout << "  load dataset \"[filename]\"    - โหลดข้อมูล" << std::endl;
    std::cout << "  create model [model_name]   - สร้างโมเดล (LinearRegression, LogisticRegression, RandomForest, SVM, DecisionTree, KNN)" << std::endl;
    std::cout << "  set [parameter] [value]     - ตั้งค่าพารามิเตอร์" << std::endl;
    std::cout << "  set use_scikit_learn [true/false] - ใช้ scikit-learn สำหรับเทรนโมเดล" << std::endl; // Added scikit-learn option
    std::cout << "  set timezone [value]        - ตั้งค่าเขตเวลา (UTC-12 ถึง UTC+14)" << std::endl;
    std::cout << "  train model                 - ฝึกโมเดเดล" << std::endl;
    std::cout << "  show accuracy/loss/graph    - แสดงผลลัพธ์" << std::endl;
    std::cout << "  evaluate model              - ประเมินประสิทธิภาพโมเดล" << std::endl;
    std::cout << "  visualize data              - แสดงภาพข้อมูล" << std::endl;
    std::cout << "  save model \"[filename]\"     - บันทึกโมเดล" << std::endl;
    std::cout << "  load model \"[filename]\"     - โหลดโมเดล" << std::endl;
    std::cout << "  end                         - สิ้นสุดโปรแกรม" << std::endl;
}

std::string MLInterpreter::getCurrentDateTime() const {
        auto now = std::chrono::system_clock::now();
        auto time_t_now = std::chrono::system_clock::to_time_t(now);

        // ปรับเวลาตาม timezone ที่ผู้ใช้กำหนด
        time_t_now += userTimezoneOffset * 3600;

        std::tm* tm_now = std::gmtime(&time_t_now);

        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_now);
        return std::string(buffer);
    }

} // namespace ai_language