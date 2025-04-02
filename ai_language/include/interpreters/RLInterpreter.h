
/**
 * @file RLInterpreter.h
 * @brief Interpreter สำหรับ Reinforcement Learning
 */

#ifndef AI_LANGUAGE_RL_INTERPRETER_H
#define AI_LANGUAGE_RL_INTERPRETER_H

#include "BaseInterpreter.h"
#include <string>

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
};

} // namespace ai_language

#endif // AI_LANGUAGE_RL_INTERPRETER_H
