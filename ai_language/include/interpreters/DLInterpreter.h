#ifndef AI_LANGUAGE_DLINTERPRETER_H
#define AI_LANGUAGE_DLINTERPRETER_H

#include "BaseInterpreter.h"
#include <string>
#include <map>
#include <vector>

namespace ai_language {

class DLInterpreter : public BaseInterpreter {
private:
    std::vector<std::string> layers; // Store neural network layers
    std::map<std::string, double> parameters;
    std::string datasetPath;

public:
    void interpret() override;
    void setDefaultParameters() override;
    void addLayer(const std::string& layerType, const std::map<std::string, std::string>& params);
    void createModel(const std::string& modelType) override; //Added override
    // Override virtual functions from BaseInterpreter
    void handleLoadCommand(const std::vector<std::string>& args) override; 
    void handleSetCommand(const std::vector<std::string>& args) override;
    void handleAddCommand(const std::vector<std::string>& args) override;
    void handleTrainCommand(const std::vector<std::string>& args) override;
    void handleEvaluateCommand(const std::vector<std::string>& args) override;
    void handleShowCommand(const std::vector<std::string>& args) override;
    void handleSaveCommand(const std::vector<std::string>& args) override;
    void handleHelpCommand() override;

    // Implement remaining pure virtual functions.  These are placeholders.
    void handlePlotCommand(const std::vector<std::string>& parts) override { std::cerr << "Not implemented yet" << std::endl; }
    void handleInspectCommand(const std::vector<std::string>& args) override { std::cerr << "Not implemented yet" << std::endl; }
    void handleValidateCommand(const std::vector<std::string>& args) override { std::cerr << "Not implemented yet" << std::endl; }
    void handlePreprocessCommand(const std::vector<std::string>& args) override { std::cerr << "Not implemented yet" << std::endl; }
    void handleSplitDatasetCommand(const std::vector<std::string>& args) override { std::cerr << "Not implemented yet" << std::endl; }
    void handlePredictCommand(const std::vector<std::string>& args) override { std::cerr << "Not implemented yet" << std::endl; }
    void handleListModelsCommand() override { std::cerr << "Not implemented yet" << std::endl; }
    void handleDeleteModelCommand(const std::vector<std::string>& args) override { std::cerr << "Not implemented yet" << std::endl; }
    void handleCompareModelsCommand() override { std::cerr << "Not implemented yet" << std::endl; }
    void handleCheckStatusCommand() override { std::cerr << "Not implemented yet" << std::endl; }
    void handleDebugCommand(const std::vector<std::string>& args) override { std::cerr << "Not implemented yet" << std::endl; }
    void handleCrossValidateCommand(const std::vector<std::string>& args) override { std::cerr << "Not implemented yet" << std::endl; }
    void handleExportResultsCommand(const std::vector<std::string>& args) override { std::cerr << "Not implemented yet" << std::endl; }
    void handleScheduleTrainingCommand(const std::vector<std::string>& args) override { std::cerr << "Not implemented yet" << std::endl; }
    void handleCreateCommand(const std::vector<std::string>& args) override; //Added override
    void handleStartCommand() override {std::cerr << "Not implemented yet" << std::endl;}; //Added override


};

} // namespace ai_language

#endif // AI_LANGUAGE_DLINTERPRETER_H