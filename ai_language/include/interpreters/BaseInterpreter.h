#ifndef BASE_INTERPRETER_H
#define BASE_INTERPRETER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <map>

// ANSI Color Codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

namespace ai_language {

class BaseInterpreter {
public:
    BaseInterpreter();
    virtual ~BaseInterpreter();

    // Main method to interpret a line of code
    void interpretLine(const std::string& line);
    
    // Common methods for all interpreters
    virtual void setDefaultParameters() = 0;
    virtual void handleStartCommand() = 0;
    virtual void handleCreateCommand(const std::vector<std::string>& args) = 0;
    virtual void handleLoadCommand(const std::vector<std::string>& args) = 0;
    virtual void handleSetCommand(const std::vector<std::string>& args) = 0;
    
    // Safe mode access
    void setSafeMode(bool enabled);
    bool getSafeMode() const;

    virtual void handleTrainCommand(const std::vector<std::string>& args) = 0;
    virtual void handleEvaluateCommand(const std::vector<std::string>& args) = 0;
    virtual void handleShowCommand(const std::vector<std::string>& args) = 0;
    virtual void handleSaveCommand(const std::vector<std::string>& args) = 0;
    virtual void handleHelpCommand() = 0;
    virtual void handleAddCommand(const std::vector<std::string>& args) = 0;
    virtual void handlePlotCommand(const std::vector<std::string>& parts) = 0;
    virtual void handleInspectCommand(const std::vector<std::string>& args) = 0;
    virtual void handleValidateCommand(const std::vector<std::string>& args) = 0;
    virtual void handlePreprocessCommand(const std::vector<std::string>& args) = 0;
    virtual void handleSplitDatasetCommand(const std::vector<std::string>& args) = 0;

    // Method to interpret a file
    void interpretFile(const std::string& filename);

    virtual void handlePredictCommand(const std::vector<std::string>& args) = 0;
    virtual void handleListModelsCommand() = 0;
    virtual void handleDeleteModelCommand(const std::vector<std::string>& args) = 0;
    virtual void handleCompareModelsCommand() = 0;
    virtual void handleCheckStatusCommand() = 0;
    virtual void handleDebugCommand(const std::vector<std::string>& args) = 0;
    virtual void handleCrossValidateCommand(const std::vector<std::string>& args) = 0;
    virtual void handleExportResultsCommand(const std::vector<std::string>& args) = 0;
    virtual void handleScheduleTrainingCommand(const std::vector<std::string>& args) = 0;

    // Utility methods
    std::string getCurrentDateTime();
    std::vector<std::string> tokenizeLine(const std::string& line);

public:
    // Utility methods that should be accessible from outside
    virtual void showVersion();
    virtual void showHelp();
    virtual void showTime();
    virtual void listModels();

protected:
    // Common variables
    bool hasStarted;
    bool hasCreated;
    bool hasLoaded;
    bool hasModel;
    bool safeMode; // ถ้าเป็น true จะรอการยืนยันก่อนทำงานคำสั่ง
    
    // ตรวจสอบโหมดความปลอดภัย
    bool isSafeMode() const;

    bool hasTrained;
    bool isDebugging;
    std::string modelType = "";
    std::unordered_map<std::string, double> parameters;
    std::unordered_map<std::string, std::string> modelInfo;
    int timezone; // Timezone offset from UTC

    virtual void predict(const std::map<std::string, std::string>& params);
    virtual void showMetric(const std::string& metric);
    virtual void showModelInfo();

    // Helper methods
    bool isExitCommand(const std::string& command);
};

} // namespace ai_language

#endif // BASE_INTERPRETER_H