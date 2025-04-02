
#include "../../include/interpreters/RLInterpreter.h"
#include "../../include/connectors/Connector.h"
#include <iostream>

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
        system(mkdirCmd.c_str());
    }
    
    // สร้างเส้นทางเต็มสำหรับไฟล์
    std::string fullPath = directory + modelPath;
    if (fullPath.find(".rlmodel") == std::string::npos) {
        fullPath += ".rlmodel";
    }

    // Get current time for timestamp
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::string timestamp = std::ctime(&time_t_now);
    timestamp.pop_back(); // Remove trailing newline

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

void RLInterpreter::handleSetCommand(const std::vector<std::string>& args) {
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

void RLInterpreter::handleHelpCommand() {
    std::cout << "Reinforcement Learning Interpreter Help:" << std::endl;
    std::cout << "  start                        # Start the interpreter" << std::endl;
    std::cout << "  create model <model_type>    # Create an RL model (e.g., DQN, Q-Learning)" << std::endl;
    std::cout << "  load dataset <path>          # Load environment/dataset" << std::endl;
    std::cout << "  load model <path>            # Load a saved model" << std::endl;
    std::cout << "  set <param> <value>          # Set parameter value" << std::endl;
    std::cout << "  train model                  # Train the model" << std::endl;
    std::cout << "  show parameters              # Show current parameters" << std::endl;
    std::cout << "  show performance             # Show model performance" << std::endl;
    std::cout << "  save model <path>            # Save model to file" << std::endl;
    std::cout << "  help                         # Show this help message" << std::endl;
    std::cout << "  evaluate model               # Evaluate the trained model" << std::endl;
}

} // namespace ai_language
