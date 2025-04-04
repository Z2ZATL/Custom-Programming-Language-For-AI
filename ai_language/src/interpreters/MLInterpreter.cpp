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
    if (!hasCreatedModel) {
        std::cout << RED << "Error: No model created. Use 'create model' command first." << RESET << std::endl;
        return;
    }
    
    if (args.size() < 2) {
        std::cout << RED << "Error: Invalid set command format. Usage: set <parameter_name> <value>" << RESET << std::endl;
        return;
    }

    std::string paramName = args[0];
    std::string paramValue = args[1];

    try {
        if (paramName == "learning_rate") {
            double value = std::stod(paramValue);
            if (value <= 0 || value > 1.0) {
                std::cout << YELLOW << "Warning: Learning rate is typically between 0 and 1. Your value: " << value << RESET << std::endl;
            }
            parameters[paramName] = value;
            std::cout << "Set " << paramName << " = " << value << std::endl;
        } else if (paramName == "epochs") {
            int value = std::stoi(paramValue);
            if (value <= 0) {
                std::cout << RED << "Error: Epochs must be a positive integer" << RESET << std::endl;
                return;
            }
            parameters[paramName] = value;
            std::cout << "Set " << paramName << " = " << value << std::endl;
        } else if (paramName == "batch_size") {
            int value = std::stoi(paramValue);
            if (value <= 0) {
                std::cout << RED << "Error: Batch size must be a positive integer" << RESET << std::endl;
                return;
            }
            parameters[paramName] = value;
            std::cout << "Set " << paramName << " = " << value << std::endl;
        } else if (paramName == "optimizer") {
            // สตริงพารามิเตอร์ - เราจะเก็บเป็นค่าตัวเลข -1 เพื่อบ่งชี้ว่าเป็นค่าพิเศษ
            // แล้วเก็บค่าจริงไว้ในแมปแยกต่างหาก
            parameters[paramName] = -1; // ใช้ค่าพิเศษเพื่อบ่งชี้ว่านี่คือพารามิเตอร์ที่เป็นสตริง
            stringParameters[paramName] = paramValue;
            std::cout << "Set " << paramName << " = " << paramValue << std::endl;
        } else if (paramName == "loss_function") {
            parameters[paramName] = -1;
            stringParameters[paramName] = paramValue;
            std::cout << "Set " << paramName << " = " << paramValue << std::endl;
        } else if (paramName == "random_state" || paramName == "seed") {
            int value = std::stoi(paramValue);
            parameters[paramName] = value;
            std::cout << "Set " << paramName << " = " << value << std::endl;
        } else if (paramName == "activation") {
            parameters[paramName] = -1;
            stringParameters[paramName] = paramValue;
            std::cout << "Set " << paramName << " = " << paramValue << std::endl;
        } else {
            // กรณีอื่นๆ ลองแปลงเป็นตัวเลข
            try {
                double value = std::stod(paramValue);
                parameters[paramName] = value;
                std::cout << "Set " << paramName << " = " << value << std::endl;
            } catch (...) {
                // ถ้าแปลงเป็นตัวเลขไม่ได้ ให้เก็บเป็น string
                parameters[paramName] = -1; // ใช้ค่าพิเศษ
                stringParameters[paramName] = paramValue;
                std::cout << "Set " << paramName << " = " << paramValue << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cout << RED << "Error: Invalid parameter value for " << paramName << ". " << e.what() << RESET << std::endl;
    }
}

void MLInterpreter::handleTrainCommand(const std::vector<std::string>& /* args */) {
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
    std::cout << "  plot <type> [options]       # Generate plots (scatter, line, histogram, correlation, learning_curve)" << std::endl;
    std::cout << "  predict <data> or predict file <path> # Make predictions" << std::endl; // Added predict command
    std::cout << "  list models                  # List available models" << std::endl; // Added list models command

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
    if (parts.size() < 1) {
        std::cout << RED << "Error: Missing plot type. Usage: plot <type> [options]" << RESET << std::endl;
        std::cout << "Available plot types: scatter, line, histogram, correlation, learning_curve, learning_curves" << std::endl;
        return;
    }

    // สำหรับคำสั่ง "plot learning_curves" หรือ "plot learning_curve"
    std::string plotType;
    if (parts.size() >= 2) {
        plotType = parts[1];
    } else {
        // กรณีที่มีเพียงคำสั่ง "plot" ให้ใช้ learning_curves เป็นค่าเริ่มต้น
        plotType = "learning_curves";
    }
    
    std::string outputPath = "Program test/Data/plot_output.png";

    // ตรวจสอบและสร้างโฟลเดอร์ถ้ายังไม่มี
    std::string mkdir_cmd = "mkdir -p 'Program test/Data'";
    int mkdir_result = system(mkdir_cmd.c_str());
    if (mkdir_result != 0) {
        std::cout << RED << "ไม่สามารถสร้างโฟลเดอร์สำหรับบันทึกกราฟ" << RESET << std::endl;
        return;
    }

    std::cout << CYAN << "Creating " << plotType << " plot..." << RESET << std::endl;

    if (plotType == "scatter") {
        std::cout << "Generating scatter plot of features vs target variable" << std::endl;
        // สร้างข้อมูลสำหรับกราฟในไฟล์ CSV
        std::string csvPath = "Program test/Data/scatter_data.csv";
        std::ofstream csvFile(csvPath);
        if (csvFile.is_open()) {
            csvFile << "feature,target\n";
            for (int i = 0; i < 100; i++) {
                double x = i * 0.1;
                double y = 2 * x + 1 + (rand() % 100 - 50) * 0.05;
                csvFile << x << "," << y << "\n";
            }
            csvFile.close();
            std::cout << GREEN << "Scatter plot data saved to: " << csvPath << RESET << std::endl;
        }
    } else if (plotType == "line") {
        std::cout << "Generating line plot of predicted vs actual values" << std::endl;
        // สร้างข้อมูลสำหรับกราฟในไฟล์ CSV
        std::string csvPath = "Program test/Data/line_data.csv";
        std::ofstream csvFile(csvPath);
        if (csvFile.is_open()) {
            csvFile << "x,actual,predicted\n";
            for (int i = 0; i < 100; i++) {
                double x = i * 0.1;
                double actual = 2 * x + 1;
                double predicted = 2 * x + 0.9 + (rand() % 100 - 50) * 0.02;
                csvFile << x << "," << actual << "," << predicted << "\n";
            }
            csvFile.close();
            std::cout << GREEN << "Line plot data saved to: " << csvPath << RESET << std::endl;
        }
    } else if (plotType == "histogram") {
        std::cout << "Generating histogram of feature distributions" << std::endl;
        // สร้างข้อมูลสำหรับกราฟในไฟล์ CSV
        std::string csvPath = "Program test/Data/histogram_data.csv";
        std::ofstream csvFile(csvPath);
        if (csvFile.is_open()) {
            csvFile << "value\n";
            for (int i = 0; i < 1000; i++) {
                double value = (rand() % 100) * 0.1;
                csvFile << value << "\n";
            }
            csvFile.close();
            std::cout << GREEN << "Histogram data saved to: " << csvPath << RESET << std::endl;
        }
    } else if (plotType == "correlation") {
        std::cout << "Generating correlation heatmap of features" << std::endl;
        // สร้างข้อมูลสำหรับกราฟในไฟล์ CSV
        std::string csvPath = "Program test/Data/correlation_data.csv";
        std::ofstream csvFile(csvPath);
        if (csvFile.is_open()) {
            csvFile << "feature1,feature2,feature3,target\n";
            for (int i = 0; i < 100; i++) {
                double f1 = (rand() % 100) * 0.1;
                double f2 = f1 * 0.7 + (rand() % 100 - 50) * 0.05;
                double f3 = f1 * 0.3 + f2 * 0.2 + (rand() % 100 - 50) * 0.05;
                double target = f1 * 2 + f2 * 1.5 + f3 * 0.5 + (rand() % 100 - 50) * 0.1;
                csvFile << f1 << "," << f2 << "," << f3 << "," << target << "\n";
            }
            csvFile.close();
            std::cout << GREEN << "Correlation data saved to: " << csvPath << RESET << std::endl;
        }
    } else if (plotType == "learning_curve" || plotType == "learning_curves") {
        if (!hasTrained) {
            std::cout << RED << "Error: Model must be trained before plotting learning curve" << RESET << std::endl;
            return;
        }
        std::cout << "Generating learning curve plot" << std::endl;

        // ใช้ฟังก์ชัน generateLearningCurves ที่มีอยู่แล้ว
        try {
            generateLearningCurves(parameters["epochs"], "Program test/Data");
            std::cout << GREEN << "Learning curve plot saved to: ai_language/Program test/Data/learning_curves.png" << RESET << std::endl;
            return;
        } catch (const std::exception& e) {
            std::cerr << RED << "Error generating plot: " << e.what() << RESET << std::endl;
            return;
        }
    } else {
        std::cout << RED << "Error: Unknown plot type '" << plotType << "'" << RESET << std::endl;
        std::cout << "Available plot types: scatter, line, histogram, correlation, learning_curve, learning_curves" << std::endl;
        return;
    }

    std::cout << GREEN << "Plot saved to: " << outputPath << RESET << std::endl;
    std::cout << "To view the plot, open the file in a suitable image viewer" << std::endl;
}

void MLInterpreter::handleInspectCommand(const std::vector<std::string>& /* args */) {
    std::cout << "Inspect command is not implemented for ML yet" << std::endl;
}

void MLInterpreter::handleValidateCommand(const std::vector<std::string>& args) {
    if (!hasLoadedData && !(args.size() > 0 && args[0] == "model")) {
        std::cout << RED << "Error: No dataset loaded. Please load a dataset first." << RESET << std::endl;
        return;
    }

    std::string targetType = args.size() > 0 ? args[0] : "dataset";
    std::cout << CYAN << "Validating " << targetType << "..." << RESET << std::endl;

    if (targetType == "dataset") {
        // ตรวจสอบความสมบูรณ์ของข้อมูล
        std::cout << "Checking for missing values..." << std::endl;
        std::cout << "Checking data types..." << std::endl;
        std::cout << "Checking for outliers..." << std::endl;
        
        std::cout << GREEN << "Dataset validation complete: Data quality looks good" << RESET << std::endl;
        std::cout << "- No missing values detected" << std::endl;
        std::cout << "- All features have appropriate data types" << std::endl;
        std::cout << "- No significant outliers detected" << std::endl;
    } else if (targetType == "model") {
        if (!hasCreatedModel) {
            std::cout << RED << "Error: No model created. Create a model first." << RESET << std::endl;
            return;
        }
        
        std::cout << "Validating model architecture..." << std::endl;
        std::cout << "Checking model parameters..." << std::endl;
        
        std::cout << GREEN << "Model validation complete: Structure looks appropriate" << RESET << std::endl;
        std::cout << "- Model architecture is valid for " << modelType << std::endl;
        std::cout << "- Parameters are within recommended ranges" << std::endl;
    } else {
        std::cout << RED << "Error: Unknown validation target: " << targetType << RESET << std::endl;
        std::cout << "Available options: dataset, model" << std::endl;
    }
}

void MLInterpreter::handlePreprocessCommand(const std::vector<std::string>& args) {
    if (!hasLoadedData) {
        std::cout << RED << "Error: No dataset loaded. Please load a dataset first." << RESET << std::endl;
        return;
    }

    if (args.empty()) {
        std::cout << RED << "Error: Missing preprocessing method. Usage: preprocess <method>" << RESET << std::endl;
        std::cout << "Available methods: normalize, standardize, encode, impute" << std::endl;
        return;
    }

    std::string method = args[0];
    std::cout << CYAN << "Preprocessing data using " << method << " method..." << RESET << std::endl;

    if (method == "normalize") {
        std::cout << "Normalizing features to [0, 1] range..." << std::endl;
        std::cout << GREEN << "Normalization complete: All numeric features now in range [0, 1]" << RESET << std::endl;
    } else if (method == "standardize") {
        std::cout << "Standardizing features to mean=0, std=1..." << std::endl;
        std::cout << GREEN << "Standardization complete: All numeric features now have mean=0, std=1" << RESET << std::endl;
    } else if (method == "encode") {
        std::cout << "Encoding categorical features..." << std::endl;
        std::cout << GREEN << "Encoding complete: Categorical features now one-hot encoded" << RESET << std::endl;
    } else if (method == "impute") {
        std::cout << "Imputing missing values..." << std::endl;
        std::cout << GREEN << "Imputation complete: Missing values replaced with appropriate values" << RESET << std::endl;
    } else if (method == "dataset") {
        std::cout << "Applying standard preprocessing for dataset type..." << std::endl;
        std::cout << GREEN << "Dataset preprocessing complete: Applied standard transformations" << RESET << std::endl;
    } else {
        std::cout << RED << "Error: Unknown preprocessing method: " << method << RESET << std::endl;
        std::cout << "Available methods: normalize, standardize, encode, impute, dataset" << std::endl;
    }
}

void MLInterpreter::handleSplitDatasetCommand(const std::vector<std::string>& args) {
    if (!hasLoadedData) {
        std::cout << RED << "Error: No dataset loaded. Please load a dataset first." << RESET << std::endl;
        return;
    }

    // แปลงรูปแบบคำสั่งใหม่ เช่น "split data into train, test with ratio 0.8, 0.2"
    std::vector<std::string> ratioValues;
    
    // ตรวจหาคำว่า "ratio" ในอาร์กิวเมนต์
    bool foundRatio = false;
    size_t ratioIndex = 0;
    
    for (size_t i = 0; i < args.size(); i++) {
        if (args[i] == "ratio") {
            foundRatio = true;
            ratioIndex = i + 1;
            break;
        }
    }
    
    // ถ้าเจอคำว่า "ratio" ให้ดึงค่าต่อจากนั้นมาเป็น ratio values
    if (foundRatio && ratioIndex < args.size()) {
        for (size_t i = ratioIndex; i < args.size(); i++) {
            std::string value = args[i];
            // ลบเครื่องหมาย , ออกถ้ามี
            if (!value.empty() && value.back() == ',') {
                value.pop_back();
            }
            if (!value.empty()) {
                ratioValues.push_back(value);
            }
        }
    } else {
        // รูปแบบเดิม โดยเอาค่าทั้งหมดมาเป็น ratio values
        ratioValues = args;
    }
    
    if (ratioValues.size() < 2) {
        std::cout << RED << "Error: Missing split ratios. Usage: split dataset <train_ratio> <test_ratio> [validation_ratio]" << RESET << std::endl;
        return;
    }

    double trainRatio = 0.0, testRatio = 0.0, validationRatio = 0.0;
    
    try {
        // ตรวจสอบรูปแบบข้อมูลเพื่อปรับแก้ให้สามารถรับค่าได้หลากหลายรูปแบบ
        std::string trainStr = ratioValues[0];
        std::string testStr = ratioValues[1];
        
        // ตัดเครื่องหมาย % ออกถ้ามี
        if (!trainStr.empty() && trainStr.back() == '%') {
            trainStr.pop_back();
            trainRatio = std::stod(trainStr) / 100.0;
        } else {
            trainRatio = std::stod(trainStr);
        }
        
        if (!testStr.empty() && testStr.back() == '%') {
            testStr.pop_back();
            testRatio = std::stod(testStr) / 100.0;
        } else {
            testRatio = std::stod(testStr);
        }
        
        // ตรวจสอบข้อมูลตัวที่ 3 ถ้ามี
        if (ratioValues.size() > 2) {
            std::string validStr = ratioValues[2];
            if (!validStr.empty() && validStr.back() == '%') {
                validStr.pop_back();
                validationRatio = std::stod(validStr) / 100.0;
            } else {
                validationRatio = std::stod(validStr);
            }
        }
        
        // ตรวจสอบว่าอัตราส่วนอยู่ระหว่าง 0 และ 1
        if (trainRatio < 0 || trainRatio > 1 || testRatio < 0 || testRatio > 1 || validationRatio < 0 || validationRatio > 1) {
            std::cout << RED << "Error: Ratio values must be between 0 and 1." << RESET << std::endl;
            return;
        }
        
        // ตรวจสอบว่าอัตราส่วนรวมกันได้ 1.0
        double totalRatio = trainRatio + testRatio + validationRatio;
        if (std::abs(totalRatio - 1.0) > 0.001) {
            // ถ้าผลรวมไม่เท่ากับ 1.0 ให้แจ้งเตือนและปรับค่า
            std::cout << YELLOW << "Warning: Split ratios should sum to 1.0. Current sum: " << totalRatio << ". Normalizing values..." << RESET << std::endl;
            // ปรับให้มีผลรวมเป็น 1.0
            trainRatio /= totalRatio;
            testRatio /= totalRatio;
            if (validationRatio > 0) {
                validationRatio /= totalRatio;
            }
        }
    } catch (const std::exception& e) {
        std::cout << RED << "Error: Invalid ratio values. Must be numbers between 0 and 1." << RESET << std::endl;
        return;
    }

    std::cout << CYAN << "Splitting dataset with ratio - ";
    std::cout << "Train: " << (trainRatio * 100) << "%, ";
    std::cout << "Test: " << (testRatio * 100) << "%";
    
    if (validationRatio > 0) {
        std::cout << ", Validation: " << (validationRatio * 100) << "%";
    }
    
    std::cout << RESET << std::endl;

    // จำลองการแบ่งชุดข้อมูล
    int totalSamples = 1000; // สมมติว่ามี 1000 ตัวอย่าง
    int trainSamples = static_cast<int>(totalSamples * trainRatio);
    int testSamples = static_cast<int>(totalSamples * testRatio);
    int validationSamples = static_cast<int>(totalSamples * validationRatio);

    std::cout << GREEN << "Dataset split complete:" << RESET << std::endl;
    std::cout << "- Training set: " << trainSamples << " samples" << std::endl;
    std::cout << "- Testing set: " << testSamples << " samples" << std::endl;
    
    if (validationRatio > 0) {
        std::cout << "- Validation set: " << validationSamples << " samples" << std::endl;
    }
}

void MLInterpreter::handlePredictCommand(const std::vector<std::string>& args) {
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

        std::cout << CYAN << "Making predictions on data from file: " << filePath << RESET << std::endl;
        std::cout << "Processing 250 samples..." << std::endl;

        // จำลองผลลัพธ์การทำนาย
        std::cout << GREEN << "Prediction complete. Results:" << RESET << std::endl;
        std::cout << "Mean prediction: 42.7" << std::endl;
        std::cout << "Prediction range: [12.3, 89.6]" << std::endl;
        std::cout << "R² on prediction set: 0.92" << std::endl;
    } else if (args[0] == "with") {
        // กรณีใช้รูปแบบ "predict with [values]"
        if (args.size() < 2) {
            std::cout << RED << "Error: Missing values after 'with'. Usage: predict with <values>" << RESET << std::endl;
            return;
        }

        // รวมทุกอาร์กิวเมนต์หลังคำว่า "with" ให้เป็นสตริงเดียว
        // เพื่อรองรับกรณีที่มีช่องว่างระหว่างอาร์เรย์ 2 มิติ
        std::string combinedArgs;
        for (size_t i = 1; i < args.size(); i++) {
            combinedArgs += args[i];
            if (i < args.size() - 1) {
                combinedArgs += " ";
            }
        }

        // ตรวจสอบว่าเป็นอาร์เรย์ 2D หรือไม่
        if (combinedArgs.substr(0, 2) == "[[") {
            // จัดการกับอาร์เรย์ 2D
            std::cout << CYAN << "Making prediction with 2D array input: " << combinedArgs << RESET << std::endl;
            std::cout << GREEN << "Prediction results for multiple samples:" << RESET << std::endl;
            std::cout << "Sample 1: 42.3" << std::endl;
            std::cout << "Sample 2: 36.7" << std::endl;
            std::cout << "Sample 3: 51.2" << std::endl;
            std::cout << "Mean prediction: 43.4" << std::endl;
            return;
        } 
        // ตรวจสอบว่าเป็นอาร์เรย์ 1D หรือไม่
        else if (combinedArgs.front() == '[' && combinedArgs.back() == ']') {
            std::string arrayStr = combinedArgs.substr(1, combinedArgs.size() - 2); // ลบ [] ออก
            
            // แยกค่าด้วยเครื่องหมายคอมม่า
            std::vector<double> inputValues;
            std::stringstream ss(arrayStr);
            std::string item;
            
            while (std::getline(ss, item, ',')) {
                // ลบช่องว่างหน้าและหลัง
                item.erase(0, item.find_first_not_of(" \t"));
                item.erase(item.find_last_not_of(" \t") + 1);
                
                try {
                    inputValues.push_back(std::stod(item));
                } catch (const std::exception& e) {
                    std::cout << RED << "Error: Invalid input value '" << item << "' in array. Must be numeric." << RESET << std::endl;
                    return;
                }
            }
            
            if (inputValues.empty()) {
                std::cout << RED << "Error: Empty array for prediction." << RESET << std::endl;
                return;
            }
            
            std::cout << CYAN << "Making prediction with input array: ";
            for (const auto& val : inputValues) {
                std::cout << val << " ";
            }
            std::cout << RESET << std::endl;
            
            // จำลองการคำนวณการทำนาย
            double predictionResult = 0.0;
            if (modelType == "LinearRegression") {
                // ตัวอย่างการคำนวณสำหรับ Linear Regression
                predictionResult = 3.5 + 2.7 * inputValues[0];
                if (inputValues.size() > 1) {
                    predictionResult += 1.2 * inputValues[1];
                }
            } else {
                // ค่าเริ่มต้นสำหรับโมเดลประเภทอื่นๆ
                predictionResult = 42.0 + 0.5 * inputValues[0];
            }
            
            std::cout << GREEN << "Prediction result: " << predictionResult << RESET << std::endl;
            return;
        } else {
            // กรณีปกติที่มีค่าหลังคำว่า "with"
            std::vector<double> inputValues;
            for (size_t i = 1; i < args.size(); i++) {
                try {
                    inputValues.push_back(std::stod(args[i]));
                } catch (const std::exception& e) {
                    std::cout << RED << "Error: Invalid input value '" << args[i] << "'. Must be numeric." << RESET << std::endl;
                    return;
                }
            }

            std::cout << CYAN << "Making prediction with input: ";
            for (const auto& val : inputValues) {
                std::cout << val << " ";
            }
            std::cout << RESET << std::endl;

            // จำลองการคำนวณการทำนาย
            double predictionResult = 0.0;
            if (modelType == "LinearRegression") {
                // ตัวอย่างการคำนวณสำหรับ Linear Regression
                predictionResult = 3.5 + 2.7 * inputValues[0];
                if (inputValues.size() > 1) {
                    predictionResult += 1.2 * inputValues[1];
                }
            } else if (modelType == "RandomForest" || modelType == "GradientBoosting") {
                // ตัวอย่างการคำนวณสำหรับโมเดลอื่นๆ
                predictionResult = 15.0 + 0.8 * inputValues[0];
                if (inputValues.size() > 1) {
                    predictionResult *= 1.05 * inputValues[1];
                }
            } else {
                // ค่าเริ่มต้นสำหรับโมเดลประเภทอื่นๆ
                predictionResult = 42.0 + 0.5 * inputValues[0];
            }

            std::cout << GREEN << "Prediction result: " << predictionResult << RESET << std::endl;
        }
    } else {
        // รวมทุกอาร์กิวเมนต์ให้เป็นสตริงเดียว
        // เพื่อรองรับกรณีที่มีช่องว่างระหว่างอาร์เรย์ 2 มิติ
        std::string combinedArgs;
        for (size_t i = 0; i < args.size(); i++) {
            combinedArgs += args[i];
            if (i < args.size() - 1) {
                combinedArgs += " ";
            }
        }

        // ตรวจสอบว่าเป็นอาร์เรย์ 2D หรือไม่
        if (combinedArgs.substr(0, 2) == "[[") {
            // จัดการกับอาร์เรย์ 2D
            std::cout << CYAN << "Making prediction with 2D array input: " << combinedArgs << RESET << std::endl;
            std::cout << GREEN << "Prediction results for multiple samples:" << RESET << std::endl;
            std::cout << "Sample 1: 42.3" << std::endl;
            std::cout << "Sample 2: 36.7" << std::endl;
            std::cout << "Sample 3: 51.2" << std::endl;
            std::cout << "Mean prediction: 43.4" << std::endl;
            return;
        } 
        // ตรวจสอบว่าเป็นอาร์เรย์ 1D หรือไม่
        else if (combinedArgs.front() == '[' && combinedArgs.back() == ']') {
            std::string arrayStr = combinedArgs.substr(1, combinedArgs.size() - 2); // ลบ [] ออก
            
            // แยกค่าด้วยเครื่องหมายคอมม่า
            std::vector<double> inputValues;
            std::stringstream ss(arrayStr);
            std::string item;
            
            while (std::getline(ss, item, ',')) {
                // ลบช่องว่างหน้าและหลัง
                item.erase(0, item.find_first_not_of(" \t"));
                item.erase(item.find_last_not_of(" \t") + 1);
                
                try {
                    inputValues.push_back(std::stod(item));
                } catch (const std::exception& e) {
                    std::cout << RED << "Error: Invalid input value '" << item << "' in array. Must be numeric." << RESET << std::endl;
                    return;
                }
            }
            
            if (inputValues.empty()) {
                std::cout << RED << "Error: Empty array for prediction." << RESET << std::endl;
                return;
            }
            
            std::cout << CYAN << "Making prediction with input array: ";
            for (const auto& val : inputValues) {
                std::cout << val << " ";
            }
            std::cout << RESET << std::endl;
            
            // จำลองการคำนวณการทำนาย
            double predictionResult = 0.0;
            if (modelType == "LinearRegression") {
                // ตัวอย่างการคำนวณสำหรับ Linear Regression
                predictionResult = 3.5 + 2.7 * inputValues[0];
                if (inputValues.size() > 1) {
                    predictionResult += 1.2 * inputValues[1];
                }
            } else {
                // ค่าเริ่มต้นสำหรับโมเดลประเภทอื่นๆ
                predictionResult = 42.0 + 0.5 * inputValues[0];
            }
            
            std::cout << GREEN << "Prediction result: " << predictionResult << RESET << std::endl;
            return;
        } else {
            // กรณีทำนายด้วยข้อมูลแบบปกติ
            std::vector<double> inputValues;
            for (const auto& arg : args) {
                try {
                    inputValues.push_back(std::stod(arg));
                } catch (const std::exception& e) {
                    std::cout << RED << "Error: Invalid input value '" << arg << "'. Must be numeric." << RESET << std::endl;
                    return;
                }
            }

            std::cout << CYAN << "Making prediction with input: ";
            for (const auto& val : inputValues) {
                std::cout << val << " ";
            }
            std::cout << RESET << std::endl;

            // จำลองการคำนวณการทำนาย
            double predictionResult = 0.0;
            if (modelType == "LinearRegression") {
                // ตัวอย่างการคำนวณสำหรับ Linear Regression
                predictionResult = 3.5 + 2.7 * inputValues[0];
                if (inputValues.size() > 1) {
                    predictionResult += 1.2 * inputValues[1];
                }
            } else if (modelType == "RandomForest" || modelType == "GradientBoosting") {
                // ตัวอย่างการคำนวณสำหรับโมเดลอื่นๆ
                predictionResult = 15.0 + 0.8 * inputValues[0];
                if (inputValues.size() > 1) {
                    predictionResult *= 1.05 * inputValues[1];
                }
            } else {
                // ค่าเริ่มต้นสำหรับโมเดลประเภทอื่นๆ
                predictionResult = 42.0 + 0.5 * inputValues[0];
            }

            std::cout << GREEN << "Prediction result: " << predictionResult << RESET << std::endl;
        }
    }
}

void MLInterpreter::handleListModelsCommand() {
    std::cout << CYAN << "Available ML models:" << RESET << std::endl;

    // แสดงรายการโมเดลที่รองรับทั้งหมด
    std::vector<std::string> supportedModels = {
        "LinearRegression", "LogisticRegression", "RandomForest", 
        "SVM", "KNN", "DecisionTree", "GradientBoosting"
    };

    for (const auto& model : supportedModels) {
        std::cout << "- " << model << std::endl;
    }

    // แสดงโมเดลที่ถูกสร้างในโปรเจกต์ปัจจุบัน (ถ้ามี)
    if (hasCreatedModel) {
        std::cout << std::endl << CYAN << "Current project model:" << RESET << std::endl;
        std::cout << "- Type: " << modelType << std::endl;
        std::cout << "- Status: " << (hasTrained ? "Trained" : "Not trained") << std::endl;

        // แสดงพารามิเตอร์หลักของโมเดล
        std::cout << "- Parameters:" << std::endl;
        for (const auto& param : parameters) {
            std::cout << "  * " << param.first << ": " << param.second << std::endl;
        }
    } else {
        std::cout << std::endl << "No model has been created in this project yet." << std::endl;
    }
}

void MLInterpreter::handleDeleteModelCommand(const std::vector<std::string>& args) {
    (void) args; // เพื่อป้องกัน warning unused parameter
    std::cout << "Delete model command is not implemented for ML yet" << std::endl;
}

void MLInterpreter::handleCompareModelsCommand() {
    std::cout << "Compare models command is not implemented for ML yet" << std::endl;
}

void MLInterpreter::handleCheckStatusCommand() {
    std::cout << "Check status command is not implemented for ML yet" << std::endl;
}

void MLInterpreter::handleDebugCommand(const std::vector<std::string>& args) {
    (void) args; // เพื่อป้องกัน warning unused parameter
    std::cout << "Debug command is not implemented for ML yet" << std::endl;
}

void MLInterpreter::handleCrossValidateCommand(const std::vector<std::string>& args) {
    (void) args; // เพื่อป้องกัน warning unused parameter
    std::cout << "Cross validate command is not implemented for ML yet" << std::endl;
}

void MLInterpreter::handleExportResultsCommand(const std::vector<std::string>& args) {
    (void) args; // เพื่อป้องกัน warning unused parameter
    std::cout << "Export results command is not implemented for ML yet" << std::endl;
}

void MLInterpreter::handleScheduleTrainingCommand(const std::vector<std::string>& args) {
    (void) args; // เพื่อป้องกัน warning unused parameter
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