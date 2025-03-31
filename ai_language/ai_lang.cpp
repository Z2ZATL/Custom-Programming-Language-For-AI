#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <iomanip>
#include <thread>
#include <chrono>
#include <sys/stat.h> // for stat

// กำหนดสีสำหรับข้อความในคอนโซล
const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";

namespace ai_language {

class Interpreter {
private:
    bool hasStarted = false;
    bool hasCreatedProject = false;
    std::string projectType = "";
    bool hasLoadedData = false;
    std::string datasetPath = "";
    bool hasCreatedModel = false;
    std::string modelType = ""; // Added modelName to store model name
    bool hasTrainedModel = false;
    bool hasShowedAccuracy = false;
    bool hasSavedModel = false;

    // พารามิเตอร์สำหรับการฝึกโมเดล
    std::unordered_map<std::string, double> parameters;

    // ค่าพารามิเตอร์เริ่มต้นสำหรับแต่ละประเภท AI
    void setDefaultParameters() {
        // ล้างค่าเดิม
        parameters.clear();

        if (projectType == "ML") {
            parameters["learning_rate"] = 0.01;
            parameters["epochs"] = 50;
            parameters["batch_size"] = 32;
        } else if (projectType == "DL") {
            parameters["learning_rate"] = 0.001;
            parameters["epochs"] = 100;
            parameters["batch_size"] = 32;
        } else if (projectType == "RL") {
            parameters["learning_rate"] = 0.1;
            parameters["episodes"] = 1000;
            parameters["discount_factor"] = 0.9;
        }
    }

    // เวลาของ timezone ของผู้ใช้ (ชั่วโมง)
    int userTimezoneOffset = 7; // ค่าเริ่มต้นคือ UTC+7 (ประเทศไทย)

    // Helper function to get current date and time adjusted for user timezone
    std::string getCurrentDateTime() {
        auto now = std::chrono::system_clock::now();
        auto time_t_now = std::chrono::system_clock::to_time_t(now);

        // ปรับเวลาตาม timezone ของผู้ใช้
        time_t_now += userTimezoneOffset * 3600; // ปรับเวลาตาม offset (ชั่วโมง)

        std::stringstream ss;
        ss << std::put_time(std::gmtime(&time_t_now), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }


public:
    void interpretFile(const std::string& filename) {
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

    void interpretLine(const std::string& line) {
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
            // คำสั่ง start
            hasStarted = true;
            std::cout << GREEN << "Program started" << RESET << std::endl;
        } else if (command == "end") {
            // คำสั่ง end
            std::cout << GREEN << "Program ended" << RESET << std::endl;
        } else if (command == "create") {
            // คำสั่ง create
            if (!hasStarted) {
                std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
                return;
            }

            if (args.size() < 1) {
                std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'create [type]' หรือ 'create model [model_name]'" << RESET << std::endl;
                return;
            }

            if (args[0] == "ML" || args[0] == "DL" || args[0] == "RL") {
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

                std::string projectTypeText;
                if (projectType == "ML") projectTypeText = "Machine Learning";
                else if (projectType == "DL") projectTypeText = "Deep Learning";
                else if (projectType == "RL") projectTypeText = "Reinforcement Learning";

                std::cout << GREEN << "Project created: " << projectTypeText << RESET << std::endl;
            } else if (args[0] == "model") {
                // สร้างโมเดล
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

                if (hasCreatedModel) {
                    std::cerr << YELLOW << "คำเตือน: กำลังแทนที่โมเดลเดิม (" << modelType << ") ด้วยโมเดลใหม่" << RESET << std::endl;
                }
                modelType = args[1];

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
                    std::cerr << YELLOW << "คำเตือน: ชื่อโมเดลมีอักขระพิเศษที่อาจทำให้เกิดปัญหา" << RESET << std::endl;
                }

                // ตรวจสอบชื่อโมเดลที่รองรับ
                std::vector<std::string> supportedModels;
                if (projectType == "ML") {
                    supportedModels = {"LinearRegression", "LogisticRegression", "RandomForest", "SVM", "DecisionTree", "KNN"};
                } else if (projectType == "DL") {
                    supportedModels = {"NeuralNetwork", "CNN", "RNN", "LSTM", "Transformer", "GAN"};
                } else if (projectType == "RL") {
                    supportedModels = {"QLearning", "DQN", "PolicyGradient", "ActorCritic", "DDPG", "A3C"};
                }

                bool isSupported = false;
                for (const auto& model : supportedModels) {
                    if (modelType == model) {
                        isSupported = true;
                        break;
                    }
                }

                if (!isSupported) {
                    std::cerr << YELLOW << "คำเตือน: โมเดล '" << modelType << "' อาจไม่รองรับในโปรเจค " << projectType << RESET << std::endl;
                }

                hasCreatedModel = true;
                hasTrainedModel = false;
                hasShowedAccuracy = false;
                hasSavedModel = false;

                std::cout << GREEN << "Model created: " << modelType << RESET << std::endl;
            } else {
                std::cerr << RED << "ข้อผิดพลาด: ประเภท '" << args[0] << "' ไม่ถูกต้อง ต้องเป็น 'ML', 'DL', 'RL' หรือ 'model'" << RESET << std::endl;
            }
        } else if (command == "load") {
            // คำสั่ง load
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
        } else if (command == "set") {
            // คำสั่ง set
            if (!hasStarted) {
                std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
                return;
            }

            if (!hasCreatedProject) {
                std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create [type]' ก่อน" << RESET << std::endl;
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

            std::string paramName = args[0];
            std::string paramValue = args[1];

            try {
                double numericValue = std::stod(paramValue);

                // ตรวจสอบค่าลบหรือค่าไม่ถูกต้อง
                if (paramName == "learning_rate" && numericValue < 0) {
                    std::cerr << YELLOW << "คำเตือน: ค่า learning_rate ติดลบ (-" << std::abs(numericValue) 
                             << ") อาจทำให้โมเดลไม่ลู่เข้า" << RESET << std::endl;
                } else if (paramName == "epochs" && numericValue <= 0) {
                    std::cerr << YELLOW << "คำเตือน: จำนวน epochs ควรมากกว่า 0" << RESET << std::endl;
                } else if (paramName == "batch_size" && numericValue <= 0) {
                    std::cerr << YELLOW << "คำเตือน: ค่า batch_size ควรมากกว่า 0" << RESET << std::endl;
                }

                // ตั้งค่าพารามิเตอร์
                parameters[paramName] = numericValue;

                std::cout << GREEN << "Parameter set: " << paramName << " = " << numericValue << RESET << std::endl;
            } catch (const std::invalid_argument&) {
                // กรณีค่าไม่ใช่ตัวเลข แต่เป็นพารามิเตอร์พิเศษ
                if (paramName == "timezone" && paramValue.substr(0, 3) == "UTC") {
                    std::cerr << RED << "ข้อผิดพลาด: ไม่รองรับการตั้งค่า timezone ด้วยคำสั่งนี้" << RESET << std::endl;
                    return;
                } else {
                    std::cerr << RED << "ข้อผิดพลาด: ค่า '" << paramValue << "' ไม่ใช่ตัวเลข" << RESET << std::endl;
                }
            } catch (const std::out_of_range&) {
                std::cerr << RED << "ข้อผิดพลาด: ค่า '" << paramValue << "' ใหญ่เกินไป" << RESET << std::endl;
            }
        } else if (command == "train") {
            // คำสั่ง train
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

            if (args.size() < 1 || args[0] != "model") {
                std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'train model'" << RESET << std::endl;
                return;
            }

            if (!hasCreatedModel) {
                std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create model' ก่อน" << RESET << std::endl;
                return;
            }

            // ตรวจสอบค่าพารามิเตอร์ที่ไม่เหมาะสม
            bool hasWarning = false;
            std::stringstream warningMsg;

            if (parameters.count("learning_rate") && parameters["learning_rate"] < 0) {
                warningMsg << "  - learning_rate ติดลบ (" << parameters["learning_rate"] << ") อาจทำให้โมเดลไม่ลู่เข้า" << std::endl;
                hasWarning = true;
            } else if (parameters.count("learning_rate") && parameters["learning_rate"] > 1.0) {
                warningMsg << "  - learning_rate มีค่าสูง (" << parameters["learning_rate"] << ") อาจทำให้โมเดลไม่ลู่เข้า" << std::endl;
                hasWarning = true;
            }

            if (parameters.count("epochs") && parameters["epochs"] <= 0) {
                warningMsg << "  - epochs มีค่าไม่ถูกต้อง (" << parameters["epochs"] << ") ใช้ค่าเริ่มต้น 50" << std::endl;
                parameters["epochs"] = 50;
                hasWarning = true;
            }

            if (parameters.count("batch_size") && parameters["batch_size"] <= 0) {
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
            for (int i = 0; i < 5; i++) {
                std::cout << "." << std::flush;
                std::this_thread::sleep_for(std::chrono::milliseconds(300));
            }
            std::cout << std::endl;

            hasTrainedModel = true;
            std::cout << GREEN << "Training complete" << RESET << std::endl;
        } else if (command == "show") {
            // คำสั่ง show
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
        } else if (command == "save") {
            // คำสั่ง save
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

            // ตรวจสอบและเพิ่มนามสกุลไฟล์ตามประเภทโมเดลถ้าไม่มี
            std::string extension;
            size_t dotPos = filename.find_last_of('.');
            if (dotPos == std::string::npos) {
                // ถ้าไม่มีนามสกุล ให้เพิ่มตามประเภทโมเดล
                if (projectType == "ML") {
                    extension = ".mlmodel";
                } else if (projectType == "DL") {
                    extension = ".dlmodel";
                } else if (projectType == "RL") {
                    extension = ".rlmodel";
                } else if (modelType.find("NLP") != std::string::npos) {
                    extension = ".nlpmodel";
                } else if (modelType.find("CNN") != std::string::npos || 
                           modelType.find("Vision") != std::string::npos) {
                    extension = ".cvmodel";
                } else {
                    extension = ".model";
                }
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
        } else if (command == "visualize") {
            // คำสั่ง visualize
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
        } else if (command == "evaluate") {
            // คำสั่ง evaluate
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

            if (args.size() < 1 || args[0] != "model") {
                std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'evaluate model'" << RESET << std::endl;
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

            if (projectType == "ML" || projectType == "DL") {
                std::cout << BLUE << "  Precision: " << std::fixed << std::setprecision(2) << precision << "%" << RESET << std::endl;
                std::cout << BLUE << "  Recall: " << std::fixed << std::setprecision(2) << recall << "%" << RESET << std::endl;
                std::cout << BLUE << "  F1 Score: " << std::fixed << std::setprecision(2) << f1_score << RESET << std::endl;
            }

            if (projectType == "RL") {
                std::cout << BLUE << "  Average Reward: " << std::fixed << std::setprecision(2) << precision << RESET << std::endl;
                std::cout << BLUE << "  Episodes Completed: 1000" << RESET << std::endl;
                std::cout << BLUE << "  Success Rate: " << std::fixed << std::setprecision(2) << recall << "%" << RESET << std::endl;
            }
        } else if (command == "plot") {
            // คำสั่ง plot
            if (!hasStarted) {
                std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
                return;
            }

            if (!hasCreatedProject) {
                std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create [type]' ก่อน" << RESET << std::endl;
                return;
            }

            if (!hasTrainedModel) {
                std::cerr << RED<< RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'train model' ก่อน" << RESET << std::endl;
                return;
            }

            std::cout << GREEN << "Plotting:" << RESET << std::endl;

            // แสดง ASCII art graph ง่ายๆ
            std::cout << CYAN << "    ^" << std::endl;
            std::cout << "    |            ****" << std::endl;
            std::cout << "    |         ***" << std::endl;
            std::cout << "    |      ***" << std::endl;
            std::cout << "    |    **" << std::endl;
            std::cout << "    |  **" << std::endl;
            std::cout << "    | *" << std::endl;
            std::cout << "    |*" << std::endl;
            std::cout << "    |" << std::endl;
            std::cout << "    +-------------------------------->" << RESET << std::endl;
        } else if (command == "add") {
            // คำสั่ง add (สำหรับ Neural Network)
            if (!hasStarted) {
                std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'start' ก่อน" << RESET << std::endl;
                return;
            }

            if (!hasCreatedProject) {
                std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create [type]' ก่อน" << RESET << std::endl;
                return;
            }

            if (projectType != "DL") {
                std::cerr << RED << "ข้อผิดพลาด: คำสั่ง 'add layer' ใช้ได้เฉพาะกับโปรเจค Deep Learning เท่านั้น" << RESET << std::endl;
                return;
            }

            if (!hasCreatedModel) {
                std::cerr << RED << "ข้อผิดพลาด: ต้องใช้คำสั่ง 'create model' ก่อน" << RESET << std::endl;
                return;
            }

            if (args.size() < 2 || args[0] != "layer") {
                std::cerr << RED << "ข้อผิดพลาด: รูปแบบคำสั่งไม่ถูกต้อง ต้องเป็น 'add layer [layer_type] [parameters]'" << RESET << std::endl;
                return;
            }

            std::string layerType = args[1];
            std::string layerSize = args.size() > 2 ? args[2] : "0";

            std::cout << GREEN << "Layer added: " << layerType << " with size " << layerSize << RESET << std::endl;
        } else if (command == "clean" || command == "split") {
            // คำสั่ง clean/split (สำหรับการจัดการข้อมูล)
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

            if (command == "clean") {
                std::cout << GREEN << "Data cleaned successfully" << RESET << std::endl;
            } else {
                std::cout << GREEN << "Data split successfully" << RESET << std::endl;
            }
        } else if (command == "predict") {
            // คำสั่ง predict
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

            // สุ่มค่าทำนาย
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(0.0, 100.0);
            double prediction = dis(gen);

            std::cout << GREEN << "Prediction: " << std::fixed << std::setprecision(2) << prediction << RESET << std::endl;
        } else if (command == "set" && args.size() > 1 && args[0] == "timezone") {
            // คำสั่ง set timezone
            try {
                int timezone = std::stoi(args[1]);
                if (timezone >= -12 && timezone <= 14) {
                    userTimezoneOffset = timezone;
                    std::cout << GREEN << "ตั้งค่า timezone เป็น UTC" << (timezone >= 0 ? "+" : "") << timezone << " สำเร็จ" << RESET << std::endl;
                } else {
                    std::cerr << RED << "ข้อผิดพลาด: timezone ต้องอยู่ระหว่าง -12 ถึง 14" << RESET << std::endl;
                }
            } catch (const std::invalid_argument&) {
                std::cerr << RED << "ข้อผิดพลาด: รูปแบบ timezone ไม่ถูกต้อง" << RESET << std::endl;
            }
        } else if (command == "help") {
            // คำสั่ง help
            std::cout << CYAN << "คำสั่งที่ใช้ได้:" << RESET << std::endl;
            std::cout << "  start                       - เริ่มต้นโปรแกรม" << std::endl;
            std::cout << "  create ML/DL/RL             - สร้างโปรเจค AI" << std::endl;
            std::cout << "  load dataset \"[filename]\"    - โหลดข้อมูล" << std::endl;
            std::cout << "  create model [model_name]   - สร้างโมเดล" << std::endl;
            std::cout << "  set [parameter] [value]     - ตั้งค่าพารามิเตอร์" << std::endl;
            std::cout << "  set timezone [value]        - ตั้งค่าเขตเวลา (UTC-12 ถึง UTC+14)" << std::endl;
            std::cout << "  train model                 - ฝึกโมเดเดล" << std::endl;
            std::cout << "  show accuracy/loss/graph    - แสดงผลลัพธ์" << std::endl;
            std::cout << "  save model \"[filename]\"     - บันทึกโมเดล" << std::endl;
            std::cout << "  load model \"[filename]\"     - โหลดโมเดล" << std::endl;
            std::cout << "  end                         - สิ้นสุดโปรแกรม" << std::endl;
        } else {
            // คำสั่งไม่รองรับ
            std::cerr << RED << "ข้อผิดพลาด: คำสั่ง '" << command << "' ไม่รองรับ" << RESET << std::endl;
            std::cerr << YELLOW << "ลองใช้คำสั่ง 'help' เพื่อดูคำสั่งที่ใช้ได้" << RESET << std::endl;
        }
    }
};

} // namespace ai_language

void printUsage() {
    std::cout << "วิธีใช้: ai_lang [options] [filename]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -i, --interactive    เข้าสู่โหมดโต้ตอบ" << std::endl;
    std::cout << "  -h, --help          แสดงวิธีใช้" << std::endl;
}

void runInteractiveMode() {
    std::cout << "=== โหมดโต้ตอบของภาษา AI ===" << std::endl;
    std::cout << "พิมพ์คำสั่งและกด Enter เพื่อดำเนินการ (พิมพ์ 'exit' เพื่อออก)" << std::endl;
    std::cout << "รองรับคำสั่งหลายบรรทัด - พิมพ์ '\\' แล้วกด Enter เพื่อพิมพ์ต่อในบรรทัดถัดไป" << std::endl;
    std::cout << "พิมพ์ ';;' เพื่อดำเนินการทั้งหมด" << std::endl << std::endl;

    // แสดงคำแนะนำหรือตัวอย่างคำสั่ง
    std::cout << "ตัวอย่างคำสั่ง:" << std::endl;
    std::cout << "  start" << std::endl;
    std::cout << "  create ML                        # สร้างโปรเจค Machine Learning" << std::endl;
    std::cout << "  create DL                        # สร้างโปรเจค Deep Learning" << std::endl;
    std::cout << "  create RL                        # สร้างโปรเจค Reinforcement Learning" << std::endl;
    std::cout << "  load dataset \"datasets/linear_data.csv\" type \"csv\"  # โหลดข้อมูลสำหรับ ML" << std::endl;
    std::cout << "  load dataset \"datasets/iris.csv\" type \"csv\"         # โหลดข้อมูลสำหรับ DL" << std::endl;
    std::cout << "  load environment \"datasets/environment.json\"         # โหลดสภาพแวดล้อมสำหรับ RL" << std::endl;
    std::cout << "  create model LinearRegression    # สร้างโมเดล LinearRegression" << std::endl;
    std::cout << "  create model NeuralNetwork       # สร้างโมเดล Neural Network" << std::endl;
    std::cout << "  create model QLearning           # สร้างโมเดล Q-Learning" << std::endl;
    std::cout << "  set learning_rate 0.01           # ตั้งค่า learning rate" << std::endl;
    std::cout << "  set epochs 100                   # ตั้งค่าจำนวนรอบการเรียนรู้" << std::endl;
    std::cout << "  train model                      # ฝึกโมเดล" << std::endl;
    std::cout << "  evaluate model                   # ประเมินประสิทธิภาพโมเดล" << std::endl;
    std::cout << "  show accuracy                    # แสดงความแม่นยำของโมเดล" << std::endl;
    std::cout << "  save model \"model_name.dat\"      # บันทึกโมเดล" << std::endl << std::endl;

    std::cout << "ตัวอย่างคำสั่งหลายบรรทัด:" << std::endl;
    std::cout << "  start \\" << std::endl;
    std::cout << "  create ML \\" << std::endl;
    std::cout << "  load dataset \"datasets/linear_data.csv\" \\" << std::endl;
    std::cout << "  create model LinearRegression ;;" << std::endl << std::endl;

    std::cout << "คำแนะนำเพิ่มเติม: ดูไฟล์ USAGE_GUIDE.md สำหรับรายละเอียดเพิ่มเติม" << std::endl;
    std::cout << "หรือดูตัวอย่างไฟล์ในโฟลเดอร์ examples/" << std::endl << std::endl;

    std::cout << "ข้อมูลตัวอย่างอยู่ในโฟลเดอร์ datasets/" << std::endl;
    std::cout << "  - linear_data.csv      # สำหรับ Linear Regression" << std::endl;
    std::cout << "  - classification_data.csv # สำหรับ Classification" << std::endl;
    std::cout << "  - iris.csv             # สำหรับ Neural Network" << std::endl;
    std::cout << "  - images/              # สำหรับ CNN" << std::endl;
    std::cout << "  - environment.json     # สำหรับ Reinforcement Learning" << std::endl;
    std::cout << "  - config.json          # สำหรับการตั้งค่า RL" << std::endl << std::endl;

    ai_language::Interpreter interpreter;
    std::string multiline = "";
    std::string line;

    // รับคำสั่งจากผู้ใช้จนกว่าจะพิมพ์ 'exit'
    while (true) {
        if (multiline.empty()) {
            std::cout << "AI> ";
        } else {
            std::cout << "... ";
        }

        std::getline(std::cin, line);

        if (line == "exit" && multiline.empty()) {
            break;
        }

        // ตรวจสอบการป้อนหลายบรรทัด
        if (!line.empty() && line.back() == '\\') {
            // ลบเครื่องหมาย \ และเก็บคำสั่งไว้
            line.pop_back();
            multiline += line + "\n";
            continue;
        }

        // เพิ่มบรรทัดปัจจุบันเข้าไปในคำสั่งหลายบรรทัด
        multiline += line;

        // ตรวจสอบว่าเป็นการสิ้นสุดคำสั่งหลายบรรทัดหรือไม่
        if (multiline.find(";;") != std::string::npos || line.find(";;") != std::string::npos) {
            // ลบเครื่องหมาย ;; ออก
            size_t pos = multiline.find(";;");
            if (pos != std::string::npos) {
                multiline.erase(pos, 2);
            }

            // แยกคำสั่งและประมวลผลทีละคำสั่ง
            std::istringstream stream(multiline);
            std::string command;

            while (std::getline(stream, command)) {
                if (!command.empty() && command.front() != '#') {
                    interpreter.interpretLine(command);
                }
            }

            multiline = "";
        } else if (multiline.find(";;") == std::string::npos && line.find(";;") == std::string::npos) {
            // ถ้าไม่มีเครื่องหมาย ;; ในบรรทัดเดียว ให้ประมวลผลทันที
            if (!multiline.empty() && multiline.front() != '#') {
                interpreter.interpretLine(multiline);
            }
            multiline = "";
        }
    }
}

int main(int argc, char* argv[]) {
    // ตรวจสอบ arguments
    if (argc == 1) {
        printUsage();
        return 0;
    }

    std::string arg = argv[1];
    if (arg == "-h" || arg == "--help") {
        printUsage();
        return 0;
    } else if (arg == "-i" || arg == "--interactive") {
        runInteractiveMode();
        return 0;
    } else {
        // ประมวลผลไฟล์
        ai_language::Interpreter interpreter;
        interpreter.interpretFile(arg);
    }

    return 0;
}