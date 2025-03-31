
#ifndef RL_INTERPRETER_H
#define RL_INTERPRETER_H

#include "BaseInterpreter.h"

namespace ai_language {

class RLInterpreter : public BaseInterpreter {
protected:
    void setDefaultParameters() override;

public:
    RLInterpreter();
    ~RLInterpreter() override;

    // ฟังก์ชันสำหรับคำสั่งพื้นฐาน
    void handleStartCommand() override;
    void handleCreateCommand(const std::vector<std::string>& args) override;
    void handleLoadCommand(const std::vector<std::string>& args) override;
    void handleSetCommand(const std::vector<std::string>& args) override;
    void handleTrainCommand(const std::vector<std::string>& args) override;
    void handleShowCommand(const std::vector<std::string>& args) override;
    void handleSaveCommand(const std::vector<std::string>& args) override;
    void handleHelpCommand() override;

    // ฟังก์ชันเฉพาะสำหรับ Reinforcement Learning
    void handleModelCreation(const std::string& modelName);
    void handleEnvironmentSetup();
    void handleEpisodeSimulation();
    void handleRewardVisualization();
};

} // namespace ai_language

#endif // RL_INTERPRETER_H
