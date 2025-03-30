
/**
 * @file interpreter.h
 * @brief คลาสสำหรับแปลและทำงานตามคำสั่งในภาษา AI
 */

#ifndef AI_LANGUAGE_INTERPRETER_H
#define AI_LANGUAGE_INTERPRETER_H

#include "lexer.h"
#include "parser.h"
#include <map>
#include <string>
#include <functional>
#include <memory>
#include <vector>

namespace ai_language {

/**
 * @struct Environment
 * @brief โครงสร้างข้อมูลสำหรับเก็บสภาพแวดล้อมการทำงาน
 */
struct Environment {
    std::string projectType;  ///< ประเภทโปรเจกต์ (ML, DL, RL)
    bool dataLoaded;  ///< ตัวแปรบอกว่าได้โหลดข้อมูลแล้วหรือไม่
    bool dataCleaned;  ///< ตัวแปรบอกว่าได้ทำความสะอาดข้อมูลแล้วหรือไม่
    bool dataSplit;  ///< ตัวแปรบอกว่าได้แบ่งข้อมูลแล้วหรือไม่
    bool modelTrained;  ///< ตัวแปรบอกว่าได้ฝึกโมเดลแล้วหรือไม่
    bool modelEvaluated;  ///< ตัวแปรบอกว่าได้ประเมินผลโมเดลแล้วหรือไม่
    bool modelSaved;  ///< ตัวแปรบอกว่าได้บันทึกโมเดลแล้วหรือไม่
    std::string datasetPath;  ///< ที่อยู่ของไฟล์ข้อมูล
    std::string datasetType;  ///< ประเภทของไฟล์ข้อมูล
    std::string modelPath;  ///< ที่อยู่ของไฟล์โมเดล
    std::map<std::string, double> metrics;  ///< เมทริกซ์ต่างๆ ของโมเดล
    std::map<std::string, std::string> variables;  ///< ตัวแปรที่ผู้ใช้กำหนด
    
    /**
     * @brief Check if a statement is complete (for interactive mode)
     * @param statement The statement to check
     * @return True if the statement is complete, false if it needs more input
     */
    bool isCompleteStatement(const std::string& statement) const;
    
    Environment()
        : dataLoaded(false), dataCleaned(false), dataSplit(false),
          modelTrained(false), modelEvaluated(false), modelSaved(false) {}
};

/**
 * @class Interpreter
 * @brief คลาสสำหรับแปลและทำงานตามคำสั่งในภาษา AI
 */
class Interpreter {
public:
    /**
     * @brief คอนสตรักเตอร์
     */
    Interpreter();
    
    /**
     * @brief ตั้งค่าฟังก์ชันสำหรับแสดงผลลัพธ์
     * @param handler ฟังก์ชันสำหรับแสดงผลลัพธ์
     */
    void setOutputHandler(std::function<void(const std::string&)> handler);
    
    /**
     * @brief ตั้งค่าฟังก์ชันสำหรับแสดงข้อผิดพลาด
     * @param handler ฟังก์ชันสำหรับแสดงข้อผิดพลาด
     */
    void setErrorHandler(std::function<void(const std::string&)> handler);
    
    /**
     * @brief แปลและทำงานตามโค้ด
     * @param source โค้ดที่ต้องการแปล
     */
    void interpret(const std::string& source);
    
    /**
     * @brief ตรวจสอบว่ามีข้อผิดพลาดหรือไม่
     * @return bool true ถ้ามีข้อผิดพลาด, false ถ้าไม่มี
     */
    bool hasError() const;
    
    /**
     * @brief รีเซ็ตสถานะข้อผิดพลาด
     */
    void resetError();
    
    /**
     * @brief ดำเนินการตามคำสั่ง
     * @param command คำสั่งที่ต้องการดำเนินการ
     * @param params พารามิเตอร์ของคำสั่ง
     */
    void executeCommand(const std::string& command, const std::map<std::string, std::string>& params = {});
    
private:
    Lexer m_lexer;  ///< ตัวแยกคำสำหรับแปลงโค้ดเป็น token
    Parser m_parser;  ///< ตัวแยกวิเคราะห์สำหรับแปลง token เป็น AST
    bool m_hasError;  ///< ตัวแปรบอกว่ามีข้อผิดพลาดหรือไม่
    Environment m_environment;  ///< สภาพแวดล้อมการทำงาน
    std::function<void(const std::string&)> m_outputHandler;  ///< ฟังก์ชันสำหรับแสดงผลลัพธ์
    std::function<void(const std::string&)> m_errorHandler;  ///< ฟังก์ชันสำหรับแสดงข้อผิดพลาด
    
    /**
     * @brief ดำเนินการเมื่อต้องการเริ่มโปรเจกต์ใหม่
     * @param params พารามิเตอร์ของคำสั่ง
     */
    void handleStartCommand(const std::map<std::string, std::string>& params);
    
    /**
     * @brief ดำเนินการเมื่อต้องการโหลดข้อมูล
     * @param params พารามิเตอร์ของคำสั่ง
     */
    void handleLoadCommand(const std::map<std::string, std::string>& params);
    
    /**
     * @brief ดำเนินการเมื่อต้องการทำความสะอาดข้อมูล
     * @param params พารามิเตอร์ของคำสั่ง
     */
    void handleCleanCommand(const std::map<std::string, std::string>& params);
    
    /**
     * @brief ดำเนินการเมื่อต้องการแบ่งข้อมูล
     * @param params พารามิเตอร์ของคำสั่ง
     */
    void handleSplitCommand(const std::map<std::string, std::string>& params);
    
    /**
     * @brief ดำเนินการเมื่อต้องการฝึกโมเดล
     * @param params พารามิเตอร์ของคำสั่ง
     */
    void handleTrainCommand(const std::map<std::string, std::string>& params);
    
    /**
     * @brief ดำเนินการเมื่อต้องการประเมินผลโมเดล
     * @param params พารามิเตอร์ของคำสั่ง
     */
    void handleEvaluateCommand(const std::map<std::string, std::string>& params);
    
    /**
     * @brief ดำเนินการเมื่อต้องการทำนายผลลัพธ์
     * @param params พารามิเตอร์ของคำสั่ง
     */
    void handlePredictCommand(const std::map<std::string, std::string>& params);
    
    /**
     * @brief ดำเนินการเมื่อต้องการบันทึกโมเดล
     * @param params พารามิเตอร์ของคำสั่ง
     */
    void handleSaveCommand(const std::map<std::string, std::string>& params);
    
    /**
     * @brief ดำเนินการเมื่อต้องการแสดงข้อมูล
     * @param params พารามิเตอร์ของคำสั่ง
     */
    void handleShowCommand(const std::map<std::string, std::string>& params);
    
    /**
     * @brief ดำเนินการเมื่อต้องการแสดงภาพ
     * @param params พารามิเตอร์ของคำสั่ง
     */
    void handleVisualizeCommand(const std::map<std::string, std::string>& params);
    
    /**
     * @brief ดำเนินการเมื่อต้องการพล็อตกราฟ
     * @param params พารามิเตอร์ของคำสั่ง
     */
    void handlePlotCommand(const std::map<std::string, std::string>& params);
};

} // namespace ai_language

#endif // AI_LANGUAGE_INTERPRETER_H
#ifndef AI_LANGUAGE_INTERPRETER_H
#define AI_LANGUAGE_INTERPRETER_H

#include "parser.h"
#include <functional>

namespace ai_language {
    // placeholder
    // Full implementation will be added later
} // namespace ai_language

#endif // AI_LANGUAGE_INTERPRETER_H
