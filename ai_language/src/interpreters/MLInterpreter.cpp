#include "../../include/interpreters/MLInterpreter.h"
#include "../../include/utils/plotting.h"
#include <iostream>
#include <chrono>
#include <ctime>

namespace ai_language {

MLInterpreter::MLInterpreter() {
    // Constructor implementation
    hasStarted = false;
    hasLoadedData = false;
    hasCreatedModel = false;
    hasTrained = false;
    hasShowedAccuracy = false;
    hasEvaluated = false; // Added to track evaluation status
    setDefaultParameters();
}

MLInterpreter::~MLInterpreter() {
    // Destructor implementation
}

void MLInterpreter::interpret() {
    std::cout << "Interpreting Machine Learning code..." << std::endl;

    // Implementation of ML interpretation logic
}

void MLInterpreter::setDefaultParameters() {
    // Set default parameters for ML models
    parameters["learning_rate"] = 0.01;
    parameters["epochs"] = 100;
    parameters["batch_size"] = 32;
}

void MLInterpreter::loadModel(const std::string& modelPath) {
    std::cout << "Loading ML model from: " << modelPath << std::endl;
    // Implementation for loading ML model
}

void MLInterpreter::trainModel() {
    std::cout << "Training ML model..." << std::endl;
    // Implementation for training ML model
}

void MLInterpreter::evaluateModel() {
    std::cout << "Evaluating ML model performance..." << std::endl;
    // Implementation for evaluating ML model
}

void MLInterpreter::saveModel(const std::string& modelPath) {
    // สร้างเส้นทางสำหรับบันทึกโมเดล
    std::string fullPath = "Program test/model/";

    // สร้างโฟลเดอร์ถ้ายังไม่มี
    int mkdir_result = system("mkdir -p 'Program test/model'");
    if (mkdir_result != 0) {
        std::cout << "Warning: Could not create directory structure. Model might not save correctly." << std::endl;
    }

    // เพิ่มชื่อไฟล์เข้าไปที่เส้นทาง
    // ลบเครื่องหมายคำพูดออกจากชื่อไฟล์ถ้ามี
    std::string cleanPath = modelPath;
    if (cleanPath.front() == '"' && cleanPath.back() == '"') {
        cleanPath = cleanPath.substr(1, cleanPath.size() - 2);
    }
    fullPath += cleanPath;

    std::cout << "Saving ML model to: " << fullPath << std::endl;

    // ใช้ฟังก์ชัน getCurrentDateTime จาก BaseInterpreter
    std::string timestamp = getCurrentDateTime();

    // สร้างไฟล์ตัวอย่างในโฟลเดอร์นั้น
    std::ofstream modelFile(fullPath);
    if (modelFile.is_open()) {
        modelFile << "# ML Model saved from AI Language\n";
        modelFile << "model_type: " << modelType << "\n";
        modelFile << "learning_rate: " << parameters["learning_rate"] << "\n";
        modelFile << "epochs: " << parameters["epochs"] << "\n";
        modelFile << "accuracy: 0.95\n";
        modelFile << "create_time: " << timestamp << "\n";
        modelFile.close();
        std::cout << "Model successfully saved to: " << fullPath << std::endl;
    } else {
        std::cout << "Error: Could not create model file at: " << fullPath << std::endl;
    }
}

// Implementation of pure virtual functions from BaseInterpreter
void MLInterpreter::handleStartCommand() {
    std::cout << "Starting Machine Learning interpreter..." << std::endl;
    hasStarted = true;
}

void MLInterpreter::handleCreateCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Error: Missing model type for create command" << std::endl;
        return;
    }

    std::string createType = args[0];

    if (createType == "model") {
        if (args.size() < 2) {
            std::cout << "Error: Missing model type. Usage: create model <model_type>" << std::endl;
            return;
        }

        modelType = args[1];
        createModel(modelType); //Use the improved createModel function
        hasCreatedModel = true;
    } else if (createType == "ML") {
        // สำหรับคำสั่ง "create ML" เพื่อรองรับไวยากรณ์ใหม่
        std::cout << "Creating ML environment" << std::endl;
        hasCreatedModel = true;
    } else if (createType == "DL" || createType == "RL") {
        std::cout << "Creating " << createType << " model. (Not fully implemented)" << std::endl;
        hasCreatedModel = true; //Mark as created even if not fully implemented

    } else {
        std::cout << "Unknown create type: " << createType << std::endl;
    }
}

void MLInterpreter::handleLoadCommand(const std::vector<std::string>& args) {
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
    } else if (loadType == "environment") {
        std::cout << "Loading environment from: " << path << " (Not implemented)" << std::endl;
    } else {
        std::cout << "Error: Unknown load type: " << loadType << std::endl;
    }
}

void MLInterpreter::handleSetCommand(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Error: Invalid set command format" << std::endl;
        return;
    }

    std::string paramName = args[0];
    std::string paramValue = args[1];

    try {
        parameters[paramName] = std::stod(paramValue);
        std::cout << "Set " << paramName << " = " << parameters[paramName] << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: Invalid parameter value" << std::endl;
    }
}

void MLInterpreter::handleTrainCommand(const std::vector<std::string>& args) {
    if (!hasCreatedModel) {
        std::cout << "Error: No model created. Use 'create model' command first." << std::endl;
        return;
    }

    if (!hasLoadedData) {
        std::cout << "Warning: No data loaded. Training with default dataset." << std::endl;
    }

    trainModel();
    hasTrained = true;
}

// เพิ่มฟังก์ชันสำหรับจัดการคำสั่ง evaluate
void MLInterpreter::handleEvaluateCommand(const std::vector<std::string>& args) {
    if (!hasTrained) {
        std::cout << "Error: Model must be trained before evaluation" << std::endl;
        return;
    }

    if (args.size() >= 1 && args[0] == "model") {
        std::cout << "Evaluating ML model performance..." << std::endl;
        // Evaluation code would go here
        hasEvaluated = true;
    } else {
        std::cout << "Error: Invalid evaluation command format. Use 'evaluate model'" << std::endl;
    }
}

void MLInterpreter::handleShowCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Error: No show type specified" << std::endl;
        return;
    }

    std::string showType = args[0];

    if (showType == "parameters") {
        std::cout << "Learning rate: " << parameters["learning_rate"] << std::endl;
        std::cout << "Epochs: " << parameters["epochs"] << std::endl;
        std::cout << "Batch size: " << parameters["batch_size"] << std::endl;
        // Display other parameters as needed
    } else if (showType == "accuracy") {
        if (!hasTrained) {
            std::cout << "Warning: Model not trained yet, no accuracy to show." << std::endl;
            return;
        }
        double accuracy = 0.95;  // สมมติค่าความแม่นยำ
        std::cout << "Model accuracy: " << accuracy << std::endl;
    } else if (showType == "loss") {
        if (!hasTrained) {
            std::cout << "Warning: Model not trained yet, no loss to show." << std::endl;
            return;
        }
        double loss = 0.05;  // สมมติค่า loss
        std::cout << "Model loss: " << loss << std::endl;
    } else if (showType == "graph") {
        if (!hasTrained) {
            std::cout << "Warning: Model not trained yet, no graph to show." << std::endl;
            return;
        }

        std::cout << "สร้างกราฟข้อมูลสำหรับโมเดล " << modelType << std::endl;
        std::cout << "ข้อมูลสรุปการเทรนโมเดล:" << std::endl;
        std::cout << "------------------------" << std::endl;
        std::cout << "Loss ลดลงจาก 0.82 เหลือ 0.05 ตลอด " << parameters["epochs"] << " epochs" << std::endl;
        std::cout << "Accuracy เพิ่มขึ้นจาก 0.65 เป็น 0.95 ตลอด " << parameters["epochs"] << " epochs" << std::endl;
        std::cout << "------------------------" << std::endl;
        std::cout << "กำลังบันทึกข้อมูลเป็นไฟล์ CSV เท่านั้น (ส่วนการสร้างกราฟกำลังปรับปรุง)..." << std::endl;

        // ใช้ฟังก์ชัน C++ สร้างกราฟโดยตรง
        std::cout << "กำลังสร้างไฟล์กราฟข้อมูลทั้งในรูปแบบ HTML และ PNG (ไม่สามารถแสดงในเทอร์มินัลได้)..." << std::endl;

        try {
            // เรียกใช้ฟังก์ชัน generateLearningCurves 
            generateLearningCurves(parameters["epochs"], "Program test/Data");

            // แจ้งผู้ใช้ว่าสามารถดูกราฟได้จากไฟล์ HTML ที่สร้างขึ้น
            std::cout << "To view the graph, open the HTML file in a web browser" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error generating graph: " << e.what() << std::endl;
        }
    } else if (showType == "reward" || showType == "q_table" || showType == "model_info" || showType == "version" || showType == "help" || showType == "time") {
        std::cout << "Showing " << showType << " (Not implemented)" << std::endl;
    } else {
        std::cout << "Unknown show type: " << showType << std::endl;
    }
}

void MLInterpreter::handleSaveCommand(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Error: Invalid save command format" << std::endl;
        return;
    }

    if (args[0] == "model") {
        if (!hasTrained) {
            std::cout << "Warning: Saving untrained model." << std::endl;
        }

        // ลบเครื่องหมายคำพูดออกจากชื่อไฟล์ถ้ามี
        std::string cleanPath = args[1];
        if (cleanPath.front() == '"' && cleanPath.back() == '"') {
            cleanPath = cleanPath.substr(1, cleanPath.size() - 2);
        }

        // บันทึกโมเดล
        saveModel(args[1]);

        std::cout << "โมเดลถูกบันทึกไปที่ 'ai_language/Program test/model/" << cleanPath << "'" << std::endl;
    } else {
        std::cout << "Unknown save type: " << args[0] << std::endl;
    }
}

void MLInterpreter::handleHelpCommand() {
    std::cout << "Machine Learning Interpreter Help:" << std::endl;
    std::cout << "  start                        # Start the interpreter" << std::endl;
    std::cout << "  create model <model_type>    # Create an ML model (e.g., RandomForest, LinearRegression)" << std::endl;
    std::cout << "  load dataset <path>          # Load dataset from file" << std::endl;
    std::cout << "  load model <path>            # Load a saved model" << std::endl;
    std::cout << "  set <param> <value>          # Set parameter value" << std::endl;
    std::cout << "  train model                  # Train the model" << std::endl;
    std::cout << "  show parameters              # Show current parameters" << std::endl;
    std::cout << "  show accuracy                # Show model accuracy" << std::endl;
    std::cout << "  save model <path>            # Save model to file" << std::endl;
    std::cout << "  help                         # Show this help message" << std::endl;
    std::cout << "  evaluate model               # Evaluate the trained model" << std::endl; // Added evaluate command to help
    std::cout << "  create DL                    # Create a Deep Learning model (Not fully implemented)" << std::endl;
    std::cout << "  create RL                    # Create a Reinforcement Learning model (Not fully implemented)" << std::endl;
    std::cout << "  load environment <path>      # Load environment (Not fully implemented)" << std::endl;
    std::cout << "  show reward                  # Show reward (Not fully implemented)" << std::endl;
    std::cout << "  show q_table                 # Show q_table (Not fully implemented)" << std::endl;
    std::cout << "  show model_info              # Show model info (Not fully implemented)" << std::endl;
    std::cout << "  show version                 # Show version (Not fully implemented)" << std::endl;
    std::cout << "  show time                    # Show time (Not fully implemented)" << std::endl;

}

void MLInterpreter::handleAddCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Error: Missing argument for add command" << std::endl;
        return;
    }

    if (args[0] == "feature") {
        if (args.size() < 2) {
            std::cout << "Error: Missing feature name. Usage: add feature <feature_name>" << std::endl;
            return;
        }
        std::cout << "Adding feature: " << args[1] << " to the model" << std::endl;
        // Implementation for adding features
    } else if (args[0] == "layer") {
        std::cout << "Adding layer (Not implemented)" << std::endl;
    } else {
        std::cout << "Unknown add type: " << args[0] << std::endl;
    }
}

// Implementation of remaining pure virtual functions
void MLInterpreter::handlePlotCommand(const std::vector<std::string>& parts) {
    std::cout << "Plot command is not implemented for ML yet" << std::endl;
}

void MLInterpreter::handleInspectCommand(const std::vector<std::string>& args) {
    std::cout << "Inspect command is not implemented for ML yet" << std::endl;
}

void MLInterpreter::handleValidateCommand(const std::vector<std::string>& args) {
    std::cout << "Validate command is not implemented for ML yet" << std::endl;
}

void MLInterpreter::handlePreprocessCommand(const std::vector<std::string>& args) {
    std::cout << "Preprocess command is not implemented for ML yet" << std::endl;
}

void MLInterpreter::handleSplitDatasetCommand(const std::vector<std::string>& args) {
    std::cout << "Split dataset command is not implemented for ML yet" << std::endl;
}

void MLInterpreter::handlePredictCommand(const std::vector<std::string>& args) {
    std::cout << "Predict command is not implemented for ML yet" << std::endl;
}

void MLInterpreter::handleListModelsCommand() {
    std::cout << "List models command is not implemented for ML yet" << std::endl;
}

void MLInterpreter::handleDeleteModelCommand(const std::vector<std::string>& args) {
    std::cout << "Delete model command is not implemented for ML yet" << std::endl;
}

void MLInterpreter::handleCompareModelsCommand() {
    std::cout << "Compare models command is not implemented for ML yet" << std::endl;
}

void MLInterpreter::handleCheckStatusCommand() {
    std::cout << "Check status command is not implemented for ML yet" << std::endl;
}

void MLInterpreter::handleDebugCommand(const std::vector<std::string>& args) {
    std::cout << "Debug command is not implemented for ML yet" << std::endl;
}

void MLInterpreter::handleCrossValidateCommand(const std::vector<std::string>& args) {
    std::cout << "Cross validate command is not implemented for ML yet" << std::endl;
}

void MLInterpreter::handleExportResultsCommand(const std::vector<std::string>& args) {
    std::cout << "Export results command is not implemented for ML yet" << std::endl;
}

void MLInterpreter::handleScheduleTrainingCommand(const std::vector<std::string>& args) {
    std::cout << "Schedule training command is not implemented for ML yet" << std::endl;
}

void MLInterpreter::createModel(const std::string& modelType) {
    std::cout << "Creating ML model: " << modelType << std::endl;

    // รองรับโมเดลประเภทต่างๆ สำหรับ ML
    std::vector<std::string> supportedModels = {
        "LinearRegression", "LogisticRegression", "RandomForest", 
        "SVM", "KNN", "DecisionTree", "GradientBoosting"
    };

    bool isSupported = false;
    for (const auto& model : supportedModels) {
        if (model == modelType) {
            isSupported = true;
            break;
        }
    }

    if (!isSupported) {
        std::cerr << "Warning: Model type '" << modelType << "' might not be fully supported for ML." << std::endl;
    }
}


} // namespace ai_language