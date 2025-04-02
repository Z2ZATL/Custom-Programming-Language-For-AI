#ifndef ML_INTERPRETER_H
#define ML_INTERPRETER_H

#include "BaseInterpreter.h"
#include "ScikitLearnConnector.h"

namespace ai_language {

class MLInterpreter : public BaseInterpreter {
protected:
    void setDefaultParameters() override;

    // เชื่อมต่อกับ scikit-learn
    ScikitLearnConnector scikitConnector;
    bool useScikitLearn = false;

public:
    MLInterpreter();
    ~MLInterpreter() override;

    // ฟังก์ชันสำหรับคำสั่งพื้นฐาน
    void handleStartCommand() override;
    void handleCreateCommand(const std::vector<std::string>& args) override;
    void handleLoadCommand(const std::vector<std::string>& args) override;
    void handleSetCommand(const std::vector<std::string>& args) override;
    void handleTrainCommand(const std::vector<std::string>& args) override;
    void handleShowCommand(const std::vector<std::string>& args) override;
    void handleSaveCommand(const std::vector<std::string>& args) override;
    void handleHelpCommand() override;

    // ฟังก์ชันเฉพาะสำหรับ Machine Learning
    void handleModelCreation(const std::string& modelName);
    void handleDataVisualization() const;
    void handleModelEvaluation();
};

} // namespace ai_language

#endif // ML_INTERPRETER_H