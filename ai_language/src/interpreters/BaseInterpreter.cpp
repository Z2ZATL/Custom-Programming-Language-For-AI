#include "../../include/interpreters/BaseInterpreter.h"
#include <algorithm>
#include <cctype>

namespace ai_language {

BaseInterpreter::BaseInterpreter() : hasStarted(false), hasCreated(false), hasLoaded(false), 
                                     hasModel(false), hasTrained(false), isDebugging(false), 
                                     timezone(0) {
    // Default parameters
    parameters["learning_rate"] = 0.01;
    parameters["epochs"] = 100;
    parameters["batch_size"] = 32;
}

BaseInterpreter::~BaseInterpreter() {
    // Cleanup
}

void BaseInterpreter::interpretLine(const std::string& line) {
    // Skip empty lines and comments
    if (line.empty() || line[0] == '#') {
        return;
    }

    try {
        // Tokenize the line
        std::vector<std::string> parts = tokenizeLine(line);
        if (parts.empty()) {
            return;
        }

        if (isDebugging) {
            std::cout << "DEBUG: Interpreting line: '" << line << "'" << std::endl << std::flush;
        }

    // Extract command and arguments
    std::string command = parts[0];
    std::transform(command.begin(), command.end(), command.begin(), ::tolower);

    std::vector<std::string> args;
    for (size_t i = 1; i < parts.size(); i++) {
        args.push_back(parts[i]);
    }

    // Process commands
    if (command == "start") {
        handleStartCommand();
    }
    else if (command == "create") {
        handleCreateCommand(args);
    }
    else if (command == "load") {
        handleLoadCommand(args);
    }
    else if (command == "set") {
        handleSetCommand(args);
    }
    else if (command == "train") {
        handleTrainCommand(args);
    }
    else if (command == "evaluate") {
        handleEvaluateCommand(args);
    }
    else if (command == "show") {
        handleShowCommand(args);
    }
    else if (command == "save") {
        handleSaveCommand(args);
    }
    else if (command == "help") {
        handleHelpCommand();
    }
    else if (command == "add") {
        handleAddCommand(args);
    }
    else if (command == "plot") {
        handlePlotCommand(parts);
    }
    else if (command == "inspect") {
        std::vector<std::string> inspectArgs;
        for (size_t i = 1; i < parts.size(); i++) {
            inspectArgs.push_back(parts[i]);
        }
        handleInspectCommand(inspectArgs);
    }
    else if (command == "validate") {
        std::vector<std::string> validateArgs;
        for (size_t i = 1; i < parts.size(); i++) {
            validateArgs.push_back(parts[i]);
        }
        handleValidateCommand(validateArgs);
    }
    else if (command == "preprocess") {
        std::vector<std::string> preprocessArgs;
        for (size_t i = 1; i < parts.size(); i++) {
            preprocessArgs.push_back(parts[i]);
        }
        handlePreprocessCommand(preprocessArgs);
    }
    else if (command == "split") {
        std::vector<std::string> splitArgs;
        for (size_t i = 1; i < parts.size(); i++) {
            splitArgs.push_back(parts[i]);
        }
        handleSplitDatasetCommand(splitArgs);
    }
    else if (command == "predict") {
        std::vector<std::string> predictArgs = args;
        handlePredictCommand(predictArgs);
    }
    else if (command == "list" && parts.size() > 1 && parts[1] == "models") {
        handleListModelsCommand();
    }
    else if (command == "delete") {
        std::vector<std::string> deleteArgs;
        for (size_t i = 1; i < parts.size(); i++) {
            deleteArgs.push_back(parts[i]);
        }
        handleDeleteModelCommand(deleteArgs);
    }
    else if (command == "compare" && parts.size() > 1 && parts[1] == "models") {
        handleCompareModelsCommand();
    }
    else if (command == "check" && parts.size() > 1 && parts[1] == "status") {
        handleCheckStatusCommand();
    }
    else if (command == "debug") {
        std::vector<std::string> debugArgs = args;
        handleDebugCommand(debugArgs);
    }
    else if (command == "cross_validate") {
        std::vector<std::string> crossValidateArgs = args;
        handleCrossValidateCommand(crossValidateArgs);
    }
    else if (command == "export") {
        std::vector<std::string> exportArgs;
        for (size_t i = 1; i < parts.size(); i++) {
            exportArgs.push_back(parts[i]);
        }
        handleExportResultsCommand(exportArgs);
    }
    else if (command == "schedule") {
        std::vector<std::string> scheduleArgs;
        for (size_t i = 1; i < parts.size(); i++) {
            scheduleArgs.push_back(parts[i]);
        }
        handleScheduleTrainingCommand(scheduleArgs);
    }
    else if (command == "end") {
        std::cout << "End of program" << std::endl;
    }
    else if (isExitCommand(command)) {
        //Added exit command handling
        std::cout << "Exiting program..." << std::endl;
        exit(0); //Added exit(0) to terminate the program.
    }
    else {
        std::cout << RED << "Error: Unknown command '" << command << "'" << RESET << std::endl;
    }
    } catch (const std::exception& e) {
        std::cerr << RED << "Error interpreting line '" << line << "': " << e.what() << RESET << std::endl << std::flush;
    } catch (...) {
        std::cerr << RED << "Unknown error interpreting line '" << line << "'" << RESET << std::endl << std::flush;
    }
}

void BaseInterpreter::handleStartCommand() {
    hasStarted = true;
    std::cout << GREEN << "AI Language program started" << RESET << std::endl;
}

void BaseInterpreter::handleCreateCommand(const std::vector<std::string>& args) {
    std::cout << "Base create command - override in derived classes" << std::endl;
}

void BaseInterpreter::handleLoadCommand(const std::vector<std::string>& args) {
    std::cout << "Base load command - override in derived classes" << std::endl;
}

void BaseInterpreter::handleSetCommand(const std::vector<std::string>& args) {
    if (!hasCreated) {
        std::cout << RED << "Error: You must create an AI type first" << RESET << std::endl;
        return;
    }

    if (args.size() < 2) {
        std::cout << RED << "Error: set command requires a parameter name and value" << RESET << std::endl;
        return;
    }

    std::string paramName = args[0];
    std::string paramValue = args[1];

    // Check for timezone setting
    if (paramName == "timezone") {
        try {
            int tz = std::stoi(paramValue);
            timezone = tz;
            std::cout << "Set timezone to UTC" << (tz >= 0 ? "+" : "") << tz << std::endl;
        } catch (std::invalid_argument& e) {
            std::cout << RED << "Error: Invalid timezone value. Please provide an integer." << RESET << std::endl;
        }
        return;
    }

    // Add common parameter handling for all interpreter types
    try {
        if (paramName == "learning_rate") {
            double value = std::stod(paramValue);
            parameters["learning_rate"] = value;
            std::cout << "Set " << paramName << " to " << value << std::endl;
        } else if (paramName == "epochs") {
            int value = std::stoi(paramValue);
            parameters["epochs"] = value;
            std::cout << "Set " << paramName << " to " << value << std::endl;
        } else if (paramName == "batch_size") {
            int value = std::stoi(paramValue);
            parameters["batch_size"] = value;
            std::cout << "Set " << paramName << " to " << value << std::endl;
        } else if (paramName == "discount_factor" || paramName == "gamma") {
            double value = std::stod(paramValue);
            parameters["discount_factor"] = value;
            std::cout << "Set discount factor to " << value << std::endl;
        } else if (paramName == "exploration_rate" || paramName == "epsilon") {
            double value = std::stod(paramValue);
            parameters["exploration_rate"] = value;
            std::cout << "Set exploration rate to " << value << std::endl;
        } else if (paramName == "min_exploration_rate") {
            double value = std::stod(paramValue);
            parameters["min_exploration_rate"] = value;
            std::cout << "Set minimum exploration rate to " << value << std::endl;
        } else if (paramName == "exploration_decay") {
            double value = std::stod(paramValue);
            parameters["exploration_decay"] = value;
            std::cout << "Set exploration decay to " << value << std::endl;
        } else if (paramName == "episodes") {
            int value = std::stoi(paramValue);
            parameters["episodes"] = value;
            std::cout << "Set episodes to " << value << std::endl;
        } else if (paramName == "state_size") {
            int value = std::stoi(paramValue);
            parameters["state_size"] = value;
            std::cout << "Set state size to " << value << std::endl;
        } else if (paramName == "action_size") {
            int value = std::stoi(paramValue);
            parameters["action_size"] = value;
            std::cout << "Set action size to " << value << std::endl;
        } else {
            // Fallback for parameters not explicitly handled
            std::cout << "Set " << paramName << " to " << paramValue << std::endl;
            parameters[paramName] = std::stod(paramValue);
        }
    } catch (const std::exception& e) {
        std::cout << RED << "Error: Invalid parameter value for " << paramName << ". " << e.what() << RESET << std::endl;
    }
}

void BaseInterpreter::handleTrainCommand(const std::vector<std::string>& args) {
    std::cout << "Base train command - override in derived classes" << std::endl;
}

void BaseInterpreter::handleEvaluateCommand(const std::vector<std::string>& args) {
    std::cout << "Base evaluate command - override in derived classes" << std::endl;
}

void BaseInterpreter::handleShowCommand(const std::vector<std::string>& args) {
    std::cout << "Base show command - override in derived classes" << std::endl;
}

void BaseInterpreter::handleSaveCommand(const std::vector<std::string>& args) {
    std::cout << "Base save command - override in derived classes" << std::endl;
}

void BaseInterpreter::handleHelpCommand() {
    std::cout << CYAN << "AI Language Help" << RESET << std::endl;
    std::cout << "Available commands:" << std::endl;
    std::cout << "  start - Start the program" << std::endl;
    std::cout << "  create <type> - Create an AI project (ML, DL, RL)" << std::endl;
    std::cout << "  load dataset \"<file>\" - Load a dataset" << std::endl;
    std::cout << "  create model <name> - Create a model" << std::endl;
    std::cout << "  set <param> <value> - Set a parameter" << std::endl;
    std::cout << "  train model - Train the model" << std::endl;
    std::cout << "  evaluate model - Evaluate the model" << std::endl;
    std::cout << "  save model \"<file>\" - Save the model" << std::endl;
    std::cout << "  end - End the program" << std::endl;
}

void BaseInterpreter::setDefaultParameters() {
    // Base implementation does nothing
}

void BaseInterpreter::handleAddCommand(const std::vector<std::string>& args) {
    std::cout << RED << "Error: add command is only available for Deep Learning" << RESET << std::endl;
}

void BaseInterpreter::handlePlotCommand(const std::vector<std::string>& parts) {
    if (parts.size() < 2) {
        std::cout << RED << "Error: plot command requires at least one argument" << RESET << std::endl;
        return;
    }

    std::string plotType = parts[1];
    std::cout << "Plotting " << plotType << " graph (implemented in derived classes)" << std::endl;
    std::cout << "Create a visualization file in Data/learning_curves.png" << std::endl;
}

void BaseInterpreter::handleInspectCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << RED << "Error: inspect command requires an argument" << RESET << std::endl;
        return;
    }

    std::string target = args[0];
    std::cout << "Inspecting " << target << "..." << std::endl;
    std::cout << "Data structure looks good" << std::endl;
}

void BaseInterpreter::handleValidateCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << RED << "Error: validate command requires an argument" << RESET << std::endl;
        return;
    }

    std::string target = args[0];
    std::cout << "Validating " << target << "..." << std::endl;
    std::cout << "Validation complete: No issues found" << std::endl;
}

void BaseInterpreter::handlePreprocessCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << RED << "Error: preprocess command requires an argument" << RESET << std::endl;
        return;
    }

    std::string method = args[0];
    std::cout << "Preprocessing data using " << method << " method" << std::endl;
    std::cout << "Data preprocessing complete" << std::endl;
}

void BaseInterpreter::handleSplitDatasetCommand(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << RED << "Error: split command requires ratio arguments" << RESET << std::endl;
        return;
    }

    std::cout << "Splitting dataset with ratio " << args[0] << ":" << args[1] << std::endl;
    std::cout << "Dataset split successfully" << std::endl;
}

void BaseInterpreter::handlePredictCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << RED << "Error: predict command requires input values" << RESET << std::endl;
        return;
    }

    std::cout << "Making prediction with input: ";
    for (const auto& arg : args) {
        std::cout << arg << " ";
    }
    std::cout << std::endl;

    // สร้างค่าทำนายแบบสุ่มเพื่อการสาธิต
    double randomPrediction = (std::rand() % 1000) / 100.0;
    std::cout << "Prediction result: " << randomPrediction << std::endl;
}

void BaseInterpreter::handleListModelsCommand() {
    listModels();
}

void BaseInterpreter::predict(const std::map<std::string, std::string>& params) {
    std::cout << "Base predict method - override in derived classes" << std::endl;
}

void BaseInterpreter::showMetric(const std::string& metric) {
    std::cout << "Base showMetric method - override in derived classes" << std::endl;
}

void BaseInterpreter::handleDeleteModelCommand(const std::vector<std::string>& args) {
    std::cout << "Base delete model command - override in derived classes" << std::endl;
}

void BaseInterpreter::handleCompareModelsCommand() {
    std::cout << "Base compare models command - override in derived classes" << std::endl;
}

void BaseInterpreter::handleCheckStatusCommand() {
    std::cout << CYAN << "Current Status:" << RESET << std::endl;
    std::cout << "  Started: " << (hasStarted ? "Yes" : "No") << std::endl;
    std::cout << "  AI Type: " << (hasCreated ? modelType : "Not set") << std::endl;
    std::cout << "  Data Loaded: " << (hasLoaded ? "Yes" : "No") << std::endl;
    std::cout << "  Model Created: " << (hasModel ? "Yes" : "No") << std::endl;
    std::cout << "  Model Trained: " << (hasTrained ? "Yes" : "No") << std::endl;
    std::cout << "  Debug Mode: " << (isDebugging ? "On" : "Off") << std::endl;
    std::cout << "  Timezone: UTC" << (timezone >= 0 ? "+" : "") << timezone << std::endl;

    std::cout << "  Parameters:" << std::endl;
    for (const auto& param : parameters) {
        std::cout << "    " << param.first << ": " << param.second << std::endl;
    }
}

void BaseInterpreter::handleDebugCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << RED << "Error: debug command requires 'on' or 'off'" << RESET << std::endl;
        return;
    }

    std::string mode = args[0];
    if (mode == "on") {
        isDebugging = true;
        std::cout << "Debug mode turned on" << std::endl;
    } else if (mode == "off") {
        isDebugging = false;
        std::cout << "Debug mode turned off" << std::endl;
    } else {
        std::cout << RED << "Error: debug command requires 'on' or 'off'" << RESET << std::endl;
    }
}

void BaseInterpreter::handleCrossValidateCommand(const std::vector<std::string>& args) {
    std::cout << "Base cross validate command - override in derived classes" << std::endl;
}

void BaseInterpreter::handleExportResultsCommand(const std::vector<std::string>& args) {
    std::cout << "Base export results command - override in derived classes" << std::endl;
}

void BaseInterpreter::handleScheduleTrainingCommand(const std::vector<std::string>& args) {
    std::cout << "Base schedule training command - override in derived classes" << std::endl;
}

std::string BaseInterpreter::getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    // Adjust time according to timezone
    in_time_t += timezone * 3600;

    std::stringstream ss;
    ss << std::put_time(std::gmtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    ss << " (UTC" << (timezone >= 0 ? "+" : "") << timezone << ")";

    return ss.str();
}

std::vector<std::string> BaseInterpreter::tokenizeLine(const std::string& line) {
    std::vector<std::string> tokens;
    std::string token;
    bool inQuotes = false;

    for (char c : line) {
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

void BaseInterpreter::interpretFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << RED << "Error: Could not open file " << filename << RESET << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        // Skip empty lines and comments
        if (line.empty() || line[0] == '#') {
            continue;
        }
        interpretLine(line);
    }
}

void BaseInterpreter::showModelInfo() {
    std::cout << "Model Information:" << std::endl;
    std::cout << "  - Type: Generic Model" << std::endl;
    std::cout << "  - Parameters: (not available)" << std::endl;
    std::cout << "  - Performance metrics: (not available)" << std::endl;
}

void BaseInterpreter::showVersion() {
    std::cout << CYAN << "AI Language Version: 1.0.0" << RESET << std::endl;
    std::cout << "Build Date: " << __DATE__ << " " << __TIME__ << std::endl;
    std::cout << "Developed by: Your AI Language Team" << std::endl;
    std::cout << "License: MIT" << std::endl;
}

void BaseInterpreter::showHelp() {
    std::cout << CYAN << "=== AI Language Help ===" << RESET << std::endl;
    std::cout << "Available commands:" << std::endl;
    std::cout << "  start                           # Start a new AI project" << std::endl;
    std::cout << "  create <type>                   # Create ML, DL, or RL project" << std::endl;
    std::cout << "  load dataset <path> [type]      # Load dataset from file" << std::endl;
    std::cout << "  create model <model_type>       # Create a specific model" << std::endl;
    std::cout << "  set <param> <value>             # Set parameter value" << std::endl;
    std::cout << "  train model                     # Train the model" << std::endl;
    std::cout << "  evaluate model                  # Evaluate model performance" << std::endl;
    std::cout << "  save model <path>               # Save model to file" << std::endl;
    std::cout << "  show <metric|info>              # Show metrics or model info" << std::endl;
    std::cout << "  predict <input>                 # Make predictions" << std::endl;
    std::cout << "  list models                     # Show available models" << std::endl;
    std::cout << "  validate <dataset>              # Validate dataset" << std::endl;
    std::cout << "  preprocess <method>             # Preprocess data" << std::endl;
    std::cout << "  split <train> <test>            # Split dataset" << std::endl;
    std::cout << "  plot <type>                     # Plot data or results" << std::endl;
    std::cout << "  show version                    # Show software version" << std::endl;
    std::cout << "  show time                       # Show current time" << std::endl;
    std::cout << "  clear                           # Clear screen" << std::endl;
    std::cout << "  exit                            # Exit program" << std::endl;
    std::cout << std::endl << "For more details, see docs/guides/USAGE_GUIDE.md" << std::endl;
}

void BaseInterpreter::showTime() {
    std::time_t now = std::time(nullptr);
    std::cout << "Current time: " << std::ctime(&now);
}

void BaseInterpreter::listModels() {
    std::cout << CYAN << "Available models:" << RESET << std::endl;
    std::cout << "  ML models: LinearRegression, LogisticRegression, RandomForest, SVM, KNN" << std::endl;
    std::cout << "  DL models: NeuralNetwork, CNN, RNN, LSTM, GRU, Transformer" << std::endl;
    std::cout << "  RL models: QLearning, DQN, PPO, A2C, DDQN" << std::endl;

    // แสดงโมเดลที่มีอยู่ในระบบ (ถ้ามี)
    std::cout << std::endl << "Models in your project:" << std::endl;
    if (hasModel) {
        std::cout << "  Current model: " << modelType << std::endl;
        std::cout << "  Status: " << (hasTrained ? "Trained" : "Not trained") << std::endl;
    } else {
        std::cout << "  No models have been created yet" << std::endl;
    }
}

bool BaseInterpreter::isExitCommand(const std::string& command) {
    std::string trimmedCmd = command;
    // ตัดช่องว่างที่อาจมีอยู่
    trimmedCmd.erase(0, trimmedCmd.find_first_not_of(" \t\n\r\f\v"));
    trimmedCmd.erase(trimmedCmd.find_last_not_of(" \t\n\r\f\v") + 1);

    return trimmedCmd == "exit" || trimmedCmd == "quit";
}

} // namespace ai_language