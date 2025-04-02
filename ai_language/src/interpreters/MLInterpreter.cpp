#include "../../include/interpreters/MLInterpreter.h"
#include <iostream>

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
    std::cout << "Saving ML model to: " << modelPath << std::endl;
    // Implementation for saving ML model
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
        std::cout << "Creating ML model: " << modelType << std::endl;
        hasCreatedModel = true;
    } else if (createType == "ML") {
        // สำหรับคำสั่ง "create ML" เพื่อรองรับไวยากรณ์ใหม่
        std::cout << "Creating ML environment" << std::endl;
        hasCreatedModel = true;
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

        saveModel(args[1]);
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
    } else {
        std::cout << "Unknown add type: " << args[0] << std::endl;
    }
}

}

} // namespace ai_language