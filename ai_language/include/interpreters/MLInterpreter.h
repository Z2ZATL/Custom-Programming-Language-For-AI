#ifndef AI_LANGUAGE_ML_INTERPRETER_H
#define AI_LANGUAGE_ML_INTERPRETER_H

#include "BaseInterpreter.h"
#include "../connectors/ScikitLearnConnector.h"
#include <map>
#include <string>

namespace ai_language {

class MLInterpreter : public BaseInterpreter {
private:
    std::string modelType;
    bool hasStarted;
    bool hasLoadedData;
    bool hasCreatedModel;
    bool hasTrained;
    bool hasShowedAccuracy;
    bool hasEvaluated;
    std::map<std::string, double> parameters;
    std::map<std::string, std::string> stringParameters; // เพิ่มแมปสำหรับเก็บพารามิเตอร์ที่เป็นสตริง

public:
    MLInterpreter();
    ~MLInterpreter();

    void interpret();

    // Additional functions
    void setDefaultParameters();
    void loadModel(const std::string& modelPath);
    void trainModel();
    void evaluateModel();
    void saveModel(const std::string& modelPath);
    void createModel(const std::string& modelType);

    // Function to handle commands
    void handleStartCommand() override;
    void handleCreateCommand(const std::vector<std::string>& args) override;
    void handleLoadCommand(const std::vector<std::string>& args) override;
    void handleSetCommand(const std::vector<std::string>& args) override;
    void handleTrainCommand(const std::vector<std::string>& args) override;
    void handleEvaluateCommand(const std::vector<std::string>& args) override;
    void handleShowCommand(const std::vector<std::string>& args) override;
    void handleSaveCommand(const std::vector<std::string>& args) override;
    void handleHelpCommand() override;
    void handleAddCommand(const std::vector<std::string>& args) override;

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

#endif // AI_LANGUAGE_ML_INTERPRETER_H