
#ifndef DL_INTERPRETER_H
#define DL_INTERPRETER_H

#include "BaseInterpreter.h"
#include <vector>
#include <string>

namespace ai_language {

// โครงสร้างข้อมูลสำหรับเก็บข้อมูลเลเยอร์
struct Layer {
    std::string type;
    int units;
    std::string activation;
    double dropoutRate;
};

class DLInterpreter : public BaseInterpreter {
protected:
    std::vector<Layer> layers;
    void setDefaultParameters() override;

public:
    DLInterpreter();
    ~DLInterpreter() override;

    // ฟังก์ชันสำหรับคำสั่งพื้นฐาน
    void handleStartCommand() override;
    void handleCreateCommand(const std::vector<std::string>& args) override;
    void handleLoadCommand(const std::vector<std::string>& args) override;
    void handleSetCommand(const std::vector<std::string>& args) override;
    void handleTrainCommand(const std::vector<std::string>& args) override;
    void handleShowCommand(const std::vector<std::string>& args) override;
    void handleSaveCommand(const std::vector<std::string>& args) override;
    void handleHelpCommand() override;

    // ฟังก์ชันเฉพาะสำหรับ Deep Learning
    void handleModelCreation(const std::string& modelName);
    void handleAddLayer(const std::vector<std::string>& args);
    void handleDataVisualization();
    void handleModelPlotting();
    void handleModelEvaluation();
};

} // namespace ai_language

#endif // DL_INTERPRETER_H
