
#ifndef DL_INTERPRETER_H
#define DL_INTERPRETER_H

#include "BaseInterpreter.h"
#include <map>
#include <cmath>

namespace ai_language {

class DLInterpreter : public BaseInterpreter {
private:
    std::vector<std::string> layers; // Store neural network layers
    std::map<std::string, double> parameters;
    std::string datasetPath;
    
public:
    DLInterpreter() = default;
    ~DLInterpreter() override = default;

    void interpret();

    // Override virtual functions from BaseInterpreter
    void setDefaultParameters() override;
    void handleStartCommand() override;
    void handleCreateCommand(const std::vector<std::string>& args) override;
    void handleLoadCommand(const std::vector<std::string>& args) override;
    void handleSetCommand(const std::vector<std::string>& args) override;
    void handleAddCommand(const std::vector<std::string>& args) override;
    void handleTrainCommand(const std::vector<std::string>& args) override;
    void handleEvaluateCommand(const std::vector<std::string>& args) override;
    void handleShowCommand(const std::vector<std::string>& args) override;
    void handleSaveCommand(const std::vector<std::string>& args) override;
    void handleHelpCommand() override;

    // Implement remaining pure virtual functions
    void handlePlotCommand(const std::vector<std::string>& parts) override;
    void handleInspectCommand(const std::vector<std::string>& args) override;
    void handleValidateCommand(const std::vector<std::string>& args) override;
    void handlePreprocessCommand(const std::vector<std::string>& args) override;
    void handleSplitDatasetCommand(const std::vector<std::string>& args) override;
    void handlePredictCommand(const std::vector<std::string>& args) override;
    void handleListModelsCommand() override;
    void handleDeleteModelCommand(const std::vector<std::string>& args) override;
    void handleCompareModelsCommand() override;
    void handleCheckStatusCommand() override;
    void handleDebugCommand(const std::vector<std::string>& args) override;
    void handleCrossValidateCommand(const std::vector<std::string>& args) override;
    void handleExportResultsCommand(const std::vector<std::string>& args) override;
    void handleScheduleTrainingCommand(const std::vector<std::string>& args) override;
};

} // namespace ai_language

#endif // DL_INTERPRETER_H
