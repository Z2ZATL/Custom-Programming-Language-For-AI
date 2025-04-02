#ifndef BASE_INTERPRETER_H
#define BASE_INTERPRETER_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <iomanip>
#include <thread>
#include <chrono>
#include <sys/stat.h>

// กำหนดสีสำหรับข้อความในคอนโซล
const std::string RESET = "\033[0m";
const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string MAGENTA = "\033[35m";
const std::string CYAN = "\033[36m";

namespace ai_language {

class BaseInterpreter {
protected:
    bool hasStarted = false;
    bool hasCreatedProject = false;
    std::string projectType = "";
    bool hasLoadedData = false;
    std::string datasetPath = "";
    bool hasCreatedModel = false;
    std::string modelType = "";
    bool hasTrainedModel = false;
    bool hasShowedAccuracy = false;
    bool hasSavedModel = false;

    // พารามิเตอร์สำหรับการฝึกโมเดล
    std::unordered_map<std::string, double> parameters;

    // เวลาของ timezone ของผู้ใช้ (ชั่วโมง)
    int userTimezoneOffset = 7; // ค่าเริ่มต้นคือ UTC+7 (ประเทศไทย)

    // Helper function to get current date and time adjusted for user timezone
    std::string getCurrentDateTime() const;

    // ตั้งค่าพารามิเตอร์เริ่มต้น
    virtual void setDefaultParameters() = 0;

public:
    BaseInterpreter() {}
    virtual ~BaseInterpreter() {}

    void interpretFile(const std::string& filename);
    void interpretLine(const std::string& line);

    // ฟังก์ชันสำหรับคำสั่งพื้นฐาน
    virtual void handleStartCommand() = 0;
    virtual void handleCreateCommand(const std::vector<std::string>& args) = 0;
    virtual void handleLoadCommand(const std::vector<std::string>& args) = 0;
    virtual void handleSetCommand(const std::vector<std::string>& args) = 0;
    virtual void handleAddCommand(const std::vector<std::string>& args) = 0;
    virtual void handleTrainCommand(const std::vector<std::string>& args) = 0;
    virtual void handleEvaluateCommand(const std::vector<std::string>& args) = 0;
    virtual void handleShowCommand(const std::vector<std::string>& args) = 0;
    virtual void handleSaveCommand(const std::vector<std::string>& args) = 0;
    virtual void handleHelpCommand() = 0;

    // ฟังก์ชันสำหรับการตรวจสอบสถานะปัจจุบัน
    virtual bool isProjectStarted() const { return hasStarted; }
    virtual bool isProjectCreated() const { return hasCreatedProject; }
};

} // namespace ai_language

#endif // BASE_INTERPRETER_H