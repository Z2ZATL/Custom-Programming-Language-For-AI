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

private:
    std::map<std::string, double> parameters;
    bool hasLoadedData;
    bool hasCreatedModel;
    bool hasTrained;
    bool hasEvaluated;
};

} // namespace ai_language

#endif // AI_LANGUAGE_RL_INTERPRETER_H