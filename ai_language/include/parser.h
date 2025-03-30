
/**
 * @file parser.h
 * @brief คลาสสำหรับแปลง token เป็น AST
 */

#ifndef AI_LANGUAGE_PARSER_H
#define AI_LANGUAGE_PARSER_H

#include "lexer.h"
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <functional>

namespace ai_language {

/**
 * @class Expression
 * @brief คลาสนามธรรมสำหรับโครงสร้างนิพจน์ใน AST
 */
class Expression {
public:
    virtual ~Expression() = default;
    virtual std::string toString() const = 0;
};

/**
 * @class Statement
 * @brief คลาสนามธรรมสำหรับโครงสร้างคำสั่งใน AST
 */
class Statement {
public:
    virtual ~Statement() = default;
    virtual std::string toString() const = 0;
    virtual void execute() const = 0;
};

/**
 * @class StartStatement
 * @brief คลาสสำหรับคำสั่ง start create <ประเภท>
 */
class StartStatement : public Statement {
public:
    StartStatement(const std::string& learningType);
    std::string toString() const override;
    void execute() const override;
    
private:
    std::string learningType;  ///< ประเภทการเรียนรู้ (ML, DL, RL)
};

/**
 * @class LoadStatement
 * @brief คลาสสำหรับคำสั่ง load dataset
 */
class LoadStatement : public Statement {
public:
    LoadStatement(const std::string& filename, const std::string& fileType);
    std::string toString() const override;
    void execute() const override;
    
private:
    std::string filename;  ///< ชื่อไฟล์
    std::string fileType;  ///< ประเภทไฟล์ (csv, json)
};

/**
 * @class CleanStatement
 * @brief คลาสสำหรับคำสั่ง clean
 */
class CleanStatement : public Statement {
public:
    CleanStatement(const std::map<std::string, std::string>& params = {});
    std::string toString() const override;
    void execute() const override;
    
private:
    std::map<std::string, std::string> params;  ///< พารามิเตอร์เพิ่มเติม
};

/**
 * @class SplitStatement
 * @brief คลาสสำหรับคำสั่ง split
 */
class SplitStatement : public Statement {
public:
    SplitStatement(const std::map<std::string, std::string>& params = {});
    std::string toString() const override;
    void execute() const override;
    
private:
    std::map<std::string, std::string> params;  ///< พารามิเตอร์เพิ่มเติม
};

/**
 * @class TrainStatement
 * @brief คลาสสำหรับคำสั่ง train
 */
class TrainStatement : public Statement {
public:
    TrainStatement(const std::map<std::string, std::string>& params = {});
    std::string toString() const override;
    void execute() const override;
    
private:
    std::map<std::string, std::string> params;  ///< พารามิเตอร์เพิ่มเติม
};

/**
 * @class EvaluateStatement
 * @brief คลาสสำหรับคำสั่ง evaluate
 */
class EvaluateStatement : public Statement {
public:
    EvaluateStatement(const std::map<std::string, std::string>& params = {});
    std::string toString() const override;
    void execute() const override;
    
private:
    std::map<std::string, std::string> params;  ///< พารามิเตอร์เพิ่มเติม
};

/**
 * @class PredictStatement
 * @brief คลาสสำหรับคำสั่ง predict
 */
class PredictStatement : public Statement {
public:
    PredictStatement(const std::map<std::string, std::string>& params = {});
    std::string toString() const override;
    void execute() const override;
    
private:
    std::map<std::string, std::string> params;  ///< พารามิเตอร์เพิ่มเติม
};

/**
 * @class SaveStatement
 * @brief คลาสสำหรับคำสั่ง save
 */
class SaveStatement : public Statement {
public:
    SaveStatement(const std::map<std::string, std::string>& params = {});
    std::string toString() const override;
    void execute() const override;
    
private:
    std::map<std::string, std::string> params;  ///< พารามิเตอร์เพิ่มเติม
};

/**
 * @class ShowStatement
 * @brief คลาสสำหรับคำสั่ง show
 */
class ShowStatement : public Statement {
public:
    ShowStatement(const std::string& target, const std::map<std::string, std::string>& params = {});
    std::string toString() const override;
    void execute() const override;
    
private:
    std::string target;  ///< เป้าหมายที่ต้องการแสดง (model, metric)
    std::map<std::string, std::string> params;  ///< พารามิเตอร์เพิ่มเติม
};

/**
 * @class VisualizeStatement
 * @brief คลาสสำหรับคำสั่ง visualize
 */
class VisualizeStatement : public Statement {
public:
    VisualizeStatement(const std::string& target, const std::map<std::string, std::string>& params = {});
    std::string toString() const override;
    void execute() const override;
    
private:
    std::string target;  ///< เป้าหมายที่ต้องการแสดงภาพ (data, model)
    std::map<std::string, std::string> params;  ///< พารามิเตอร์เพิ่มเติม
};

/**
 * @class PlotStatement
 * @brief คลาสสำหรับคำสั่ง plot
 */
class PlotStatement : public Statement {
public:
    PlotStatement(const std::string& target, const std::map<std::string, std::string>& params = {});
    std::string toString() const override;
    void execute() const override;
    
private:
    std::string target;  ///< เป้าหมายที่ต้องการพล็อต (data, result)
    std::map<std::string, std::string> params;  ///< พารามิเตอร์เพิ่มเติม
};

/**
 * @class Program
 * @brief คลาสสำหรับเก็บคำสั่งทั้งหมดในโปรแกรม
 */
class Program {
public:
    Program(const std::vector<std::shared_ptr<Statement>>& statements);
    void execute() const;
    
private:
    std::vector<std::shared_ptr<Statement>> statements;  ///< รายการของคำสั่ง
};

/**
 * @class Parser
 * @brief คลาสสำหรับแปลง token เป็น AST
 */
class Parser {
public:
    /**
     * @brief คอนสตรักเตอร์
     */
    Parser();
    
    /**
     * @brief ตั้งค่าฟังก์ชันสำหรับแสดงข้อผิดพลาด
     * @param handler ฟังก์ชันสำหรับแสดงข้อผิดพลาด
     */
    void setErrorHandler(std::function<void(const std::string&)> handler);
    
    /**
     * @brief แปลง token เป็น AST
     * @param tokens รายการของ token
     * @return std::shared_ptr<Program> โปรแกรมที่แปลงแล้ว
     */
    std::shared_ptr<Program> parse(const std::vector<Token>& tokens);
    
    /**
     * @brief ตรวจสอบว่ามีข้อผิดพลาดหรือไม่
     * @return bool true ถ้ามีข้อผิดพลาด, false ถ้าไม่มี
     */
    bool hasError() const;
    
    /**
     * @brief รีเซ็ตสถานะข้อผิดพลาด
     */
    void resetError();
    
private:
    // ตัวแปรสถานะ
    std::vector<Token> m_tokens;  ///< รายการของ token
    int m_current;  ///< ตำแหน่งปัจจุบัน
    bool m_hasError;  ///< ตัวแปรบอกว่ามีข้อผิดพลาดหรือไม่
    std::function<void(const std::string&)> m_errorHandler;  ///< ฟังก์ชันสำหรับแสดงข้อผิดพลาด
    
    // เมธอดช่วยเหลือ
    bool isAtEnd() const;
    Token peek() const;
    Token previous() const;
    Token advance();
    bool check(TokenType type) const;
    bool match(TokenType type);
    bool match(const std::vector<TokenType>& types);
    void consumeNewlines();
    Token consume(TokenType type, const std::string& message);
    
    // เมธอดสำหรับแปลงคำสั่งต่างๆ
    std::shared_ptr<Statement> declaration();
    std::shared_ptr<Statement> statement();
    std::shared_ptr<Statement> startStatement();
    std::shared_ptr<Statement> loadStatement();
    std::shared_ptr<Statement> cleanStatement();
    std::shared_ptr<Statement> splitStatement();
    std::shared_ptr<Statement> trainStatement();
    std::shared_ptr<Statement> evaluateStatement();
    std::shared_ptr<Statement> predictStatement();
    std::shared_ptr<Statement> saveStatement();
    std::shared_ptr<Statement> showStatement();
    std::shared_ptr<Statement> visualizeStatement();
    std::shared_ptr<Statement> plotStatement();
    
    // เมธอดสำหรับแปลงพารามิเตอร์
    std::map<std::string, std::string> parseParameters();
    
    // เมธอดสำหรับจัดการข้อผิดพลาด
    void error(const std::string& message);
    void error(const Token& token, const std::string& message);
    void synchronize();
};

/**
 * @brief ฟังก์ชันช่วยเหลือสำหรับแปลงคำสั่งและพารามิเตอร์
 */
std::map<std::string, std::string> parseParams(const std::string& paramString);

} // namespace ai_language

#endif // AI_LANGUAGE_PARSER_H
#ifndef AI_LANGUAGE_PARSER_H
#define AI_LANGUAGE_PARSER_H

#include "lexer.h"

namespace ai_language {
    // placeholder
    // Full implementation will be added later
} // namespace ai_language

#endif // AI_LANGUAGE_PARSER_H
