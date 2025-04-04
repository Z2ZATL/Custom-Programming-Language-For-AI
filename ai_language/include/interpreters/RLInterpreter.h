/**
 * @file RLInterpreter.h
 * @brief Interpreter สำหรับ Reinforcement Learning
 */

#ifndef AI_LANGUAGE_RL_INTERPRETER_H
#define AI_LANGUAGE_RL_INTERPRETER_H

#include "BaseInterpreter.h"
#include <string>
#include <map>

namespace ai_language {

/**
 * @class RLInterpreter
 * @brief Interpreter สำหรับแปลและทำงานตามคำสั่ง Reinforcement Learning
 */
class RLInterpreter : public BaseInterpreter {
public:
    /**
     * @brief Constructor
     */
    RLInterpreter();

    /**
     * @brief Destructor
     */
    ~RLInterpreter() override;

    /**
     * @brief ฟังก์ชันหลักสำหรับแปลและทำงานตามคำสั่ง
     */
    void interpret();

    /**
     * @brief โหลดโมเดล RL จากไฟล์
     * @param modelPath พาธของไฟล์โมเดล
     */
    void loadModel(const std::string& modelPath);

    /**
     * @brief ฝึกโมเดล RL
     */
    void trainModel();

    /**
     * @brief ประเมินประสิทธิภาพของโมเดล RL
     */
    void evaluateModel();

    /**
     * @brief บันทึกโมเดล RL ลงไฟล์
     * @param modelPath พาธของไฟล์โมเดลที่ต้องการบันทึก
     */
    void saveModel(const std::string& modelPath);

    // Implementation of pure virtual functions from BaseInterpreter
    void setDefaultParameters() override;
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

private:
    std::map<std::string, double> parameters;
    bool hasLoadedData;
    bool hasCreatedModel;
    bool hasTrained;
    bool hasEvaluated;
    
    // เพิ่มฟังก์ชันสำหรับจัดการคำสั่งใหม่
    void handleInspectCommand(const std::vector<std::string>& args);
    void handleValidateCommand(const std::vector<std::string>& args);
    void handlePlotCommand(const std::vector<std::string>& args);
    void handlePreprocessCommand(const std::vector<std::string>& args);
    void handleSplitDatasetCommand(const std::vector<std::string>& args);
    void handlePredictCommand(const std::vector<std::string>& args);
    void handleListModelsCommand();
    void handleDeleteModelCommand(const std::vector<std::string>& args);
    void handleCompareModelsCommand();
    void handleCheckStatusCommand();
    void handleDebugCommand(const std::vector<std::string>& args);
    void handleCrossValidateCommand(const std::vector<std::string>& args);
    void handleExportResultsCommand(const std::vector<std::string>& args);
    void handleScheduleTrainingCommand(const std::vector<std::string>& args);
    void handleCreateEnvironmentCommand(const std::vector<std::string>& args);
    void handleSetEnvironmentParameterCommand(const std::vector<std::string>& args);
    
    // ฟังก์ชันช่วย
    void cleanModelFileName(std::string& modelPath);
};

} // namespace ai_language

#endif // AI_LANGUAGE_RL_INTERPRETER_H