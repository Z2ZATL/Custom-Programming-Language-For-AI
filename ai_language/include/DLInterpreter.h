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
private:
    // ตัวแปรสำหรับเก็บประเภทโมเดล
    std::string modelType;
    std::string datasetFilename; // เพิ่มตัวแปรสำหรับเก็บชื่อไฟล์ข้อมูล
    
    // Helper function
    std::vector<std::string> tokenizeLine(const std::string& line);

public:
    DLInterpreter();
    ~DLInterpreter() override;

    // เมธอดสำหรับการประมวลผลไฟล์และคำสั่ง
    void interpretFile(const std::string& filename);
    void interpretLine(const std::string& line);

    // เมธอดสำหรับการดำเนินการตามประเภทของคำสั่ง
    void handleStartCommand() override;
    void handleCreateCommand(const std::vector<std::string>& args) override;
    void handleLoadCommand(const std::vector<std::string>& args) override;
    void handleSetCommand(const std::vector<std::string>& args) override;
    void handleTrainCommand(const std::vector<std::string>& args) override;
    void handleShowCommand(const std::vector<std::string>& args) override;
    void handleSaveCommand(const std::vector<std::string>& args) override;

    // เมธอดเพิ่มเติมสำหรับ DL
    void handleAddLayerCommand(const std::vector<std::string>& args);
    void handlePlotCommand(const std::vector<std::string>& args);
    void handleEvaluateCommand(const std::vector<std::string>& args);
    void handleVisualizeCommand(const std::vector<std::string>& args);

    // เมธอดเฉพาะสำหรับ DL
    void setDefaultParameters();
    void handleModelCreation(const std::string& modelName);
    void handleAddLayer(const std::vector<std::string>& args);
    void handleDataVisualization();
    void handleModelPlotting();
    void handleModelEvaluation();
};

} // namespace ai_language

#endif // DL_INTERPRETER_H