#ifndef AI_LANGUAGE_ML_INTERPRETER_H
#define AI_LANGUAGE_ML_INTERPRETER_H

#include "BaseInterpreter.h"
#include "../connectors/ScikitLearnConnector.h"
#include <map>
#include <string>

namespace ai_language {

class MLInterpreter : public BaseInterpreter {
private:
    bool hasStarted;
    bool hasLoadedData;
    bool hasCreatedModel;
    bool hasTrained;
    bool hasShowedAccuracy;
    bool hasEvaluated;
    std::string modelType;
    std::map<std::string, double> parameters;

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
};

} // namespace ai_language

#endif // AI_LANGUAGE_ML_INTERPRETER_H