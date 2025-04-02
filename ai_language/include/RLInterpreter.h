#ifndef RL_INTERPRETER_H
#define RL_INTERPRETER_H

#include "BaseInterpreter.h"

namespace ai_language {

class RLInterpreter : public BaseInterpreter {
private:
    // ตัวแปรสำหรับเก็บประเภทโมเดล
    std::string modelType;
    std::string datasetFilename; // เพิ่มตัวแปรสำหรับเก็บชื่อไฟล์ข้อมูล
    
    // Utility methods
    std::vector<std::string> tokenizeLine(const std::string& line);

public:
    RLInterpreter();
    ~RLInterpreter() override;

    // Override methods from BaseInterpreter
    void interpretLine(const std::string& line);

    // เมธอดสำหรับการตั้งค่าพารามิเตอร์เริ่มต้น
    void setDefaultParameters() override;

    // เมธอดสำหรับการดำเนินการตามประเภทของคำสั่ง
    void handleStartCommand() override;
    void handleCreateCommand(const std::vector<std::string>& args) override;
    void handleLoadCommand(const std::vector<std::string>& args) override;
    void handleSetCommand(const std::vector<std::string>& args) override;
    void handleTrainCommand(const std::vector<std::string>& args) override;
    void handleSaveCommand(const std::vector<std::string>& args) override;
    void handleShowCommand(const std::vector<std::string>& args) override;
    void handleVisualizeCommand(const std::vector<std::string>& args);
    void handleHelpCommand() override;
    void handleRunSimulationCommand(const std::vector<std::string>& args);
    void handleEvaluateCommand(const std::vector<std::string>& args);

    // เมธอดเพิ่มเติมสำหรับ RL
    void handleModelCreation(const std::string& modelName);
    void displayTrainingProgress(int episode, int totalEpisodes, double reward, double avgReward);
    void displayModel(const std::string& metric) const;
};

} // namespace ai_language

#endif // RL_INTERPRETER_H