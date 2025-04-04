
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

    // Tokenize the line
    std::vector<std::string> parts = tokenizeLine(line);
    if (parts.empty()) {
        return;
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
    else {
        std::cout << RED << "Error: Unknown command '" << command << "'" << RESET << std::endl;
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

    // Handle other parameters (to be implemented in derived classes)
    std::cout << "Set " << paramName << " to " << paramValue << std::endl;
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

void BaseInterpreter::handleAddCommand(const std::vector<std::string>& args) {
    std::cout << RED << "Error: add command is only available for Deep Learning" << RESET << std::endl;
}

void BaseInterpreter::handlePlotCommand(const std::vector<std::string>& parts) {
    std::cout << "Base plot command - override in derived classes" << std::endl;
}

void BaseInterpreter::handleInspectCommand(const std::vector<std::string>& args) {
    std::cout << "Base inspect command - override in derived classes" << std::endl;
}

void BaseInterpreter::handleValidateCommand(const std::vector<std::string>& args) {
    std::cout << "Base validate command - override in derived classes" << std::endl;
}

void BaseInterpreter::handlePreprocessCommand(const std::vector<std::string>& args) {
    std::cout << "Base preprocess command - override in derived classes" << std::endl;
}

void BaseInterpreter::handleSplitDatasetCommand(const std::vector<std::string>& args) {
    std::cout << "Base split dataset command - override in derived classes" << std::endl;
}

void BaseInterpreter::handlePredictCommand(const std::vector<std::string>& args) {
    std::cout << "Base predict command - override in derived classes" << std::endl;
}

void BaseInterpreter::handleListModelsCommand() {
    std::cout << "Base list models command - override in derived classes" << std::endl;
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

} // namespace ai_language
