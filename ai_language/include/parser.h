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
#include <unordered_map>
#include <stdexcept>

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
    Token token; // Token ที่เกี่ยวข้องกับคำสั่งนี้

    Statement() : token(TokenType::IDENTIFIER, "", 0, 0) {}
    Statement(const Token& token) : token(token) {}

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


// ข้อยกเว้นสำหรับการแจ้งข้อผิดพลาดในการแปลความหมาย
class ParserError : public std::runtime_error {
public:
    explicit ParserError(const std::string& message) : std::runtime_error(message) {}
};

// คำสั่ง create <project_type>
class CreateProjectStatement : public Statement {
public:
    std::string projectType;  // ML, DL, หรือ RL
    std::string toString() const override;
    void execute() const override;
};

// คำสั่ง create model <model_name>
class CreateModelStatement : public Statement {
public:
    std::string modelName;
    std::unordered_map<std::string, std::string> parameters;
    std::string toString() const override;
    void execute() const override;
};

// คำสั่ง load dataset <dataset_path>
class LoadDatasetStatement : public Statement {
public:
    std::string dataPath;
    std::string dataType;
    std::string toString() const override;
    void execute() const override;
};

// คำสั่ง load model <model_path>
class LoadModelStatement : public Statement {
public:
    std::string modelPath;
    std::string toString() const override;
    void execute() const override;
};

// คำสั่ง load environment <environment_path>
class LoadEnvironmentStatement : public Statement {
public:
    std::string environmentPath;
    std::string toString() const override;
    void execute() const override;
};

// คำสั่ง set <param_name> <param_value>
class SetParameterStatement : public Statement {
public:
    std::string paramName;
    std::string paramValue;
    std::string toString() const override;
    void execute() const override;
};

// คำสั่ง train model
class TrainModelStatement : public Statement {
public:
    std::string toString() const override;
    void execute() const override;
};

// คำสั่ง evaluate model
class EvaluateModelStatement : public Statement {
public:
    std::string toString() const override;
    void execute() const override;
};

// คำสั่ง show <metric_type>
class ShowMetricStatement : public Statement {
public:
    std::string metricType;  // accuracy, loss, performance, หรือ graph
    std::string toString() const override;
    void execute() const override;
};

// คำสั่ง save model <save_path>
class SaveModelStatement : public Statement {
public:
    std::string savePath;
    std::string toString() const override;
    void execute() const override;
};

/**
 * @class AddLayerStatement
 * @brief คลาสสำหรับคำสั่ง add layer
 */
class AddLayerStatement : public Statement {
public:
    AddLayerStatement(const std::string& layerType, const std::map<std::string, std::string>& params);
    std::string toString() const override;
    void execute() const override;

private:
    std::string layerType;  ///< ประเภทของเลเยอร์
    std::map<std::string, std::string> params;  ///< พารามิเตอร์สำหรับตั้งค่าเลเยอร์
};


// คำสั่ง predict <input>
class PredictStatement : public Statement {
private:
    std::map<std::string, std::string> params;

public:
    PredictStatement(const std::map<std::string, std::string>& params);
public:
    std::string predictInput;
    std::unordered_map<std::string, std::string> parameters;
    std::string toString() const override;
    void execute() const override;
};

// คำสั่ง end
class EndStatement : public Statement {
public:
    std::string toString() const override;
    void execute() const override;
};

/**
 * @class Parser
 * @brief คลาสสำหรับแปลง token เป็น AST
 */
class Parser {
private:
    std::vector<Token> m_tokens;
    size_t m_current;
    bool m_hasError;
    std::string m_errorMsg;
    std::function<void(const std::string&)> m_errorHandler;

    // โครงสร้างข้อมูลเพื่อเก็บข้อมูลตั้งค่า (configuration)
    struct {
        bool expectType = true;    // คาดหวังคำสั่ง type หรือไม่
        bool expectPath = true;    // คาดหวังคำสั่ง path หรือไม่
    } m_config;

    std::shared_ptr<Statement> declaration();
    std::shared_ptr<Statement> statement();
    std::shared_ptr<Statement> startStatement();
    std::shared_ptr<Statement> createStatement();
    std::shared_ptr<Statement> loadStatement();
    std::shared_ptr<Statement> splitStatement();
    std::shared_ptr<Statement> trainStatement();
    std::shared_ptr<Statement> evaluateStatement();
    std::shared_ptr<Statement> showStatement();
    // Removed duplicate declaration of visualizeStatement()
    std::shared_ptr<Statement> plotStatement();
    std::shared_ptr<Statement> predictStatement();
    std::shared_ptr<Statement> saveStatement();
    std::shared_ptr<Statement> endStatement();
    std::shared_ptr<Statement> addLayerStatement();
    std::shared_ptr<Statement> setStatement();

    // สำหรับการพัฒนาเวอร์ชันใหม่
    std::unique_ptr<Statement> parseStatement();
    std::unique_ptr<Statement> parseStartStatement();
    std::unique_ptr<Statement> parseCreateStatement();
    std::unique_ptr<Statement> parseLoadStatement();
    std::unique_ptr<Statement> parseSetStatement();
    std::unique_ptr<Statement> parseTrainStatement();
    std::unique_ptr<Statement> parseEvaluateStatement();
    std::unique_ptr<Statement> parseShowStatement();
    std::unique_ptr<Statement> parseSaveStatement();
    std::unique_ptr<Statement> parseAddLayerStatement();
    std::unique_ptr<Statement> parsePredictStatement();
    std::unique_ptr<Statement> parseEndStatement();

    // เพิ่มฟังก์ชันสำหรับตรวจสอบจุดสิ้นสุดของคำสั่ง
    bool isAtEndOfStatement();
    void consumeEndOfStatement();

    // ดูว่า token ปัจจุบันเป็นประเภทที่คาดหวังหรือไม่ ถ้าใช่ จะเลื่อน token pointer ไปข้างหน้า
    bool match(TokenType type);
    bool match(const std::vector<TokenType>& types);

    // ตรวจสอบประเภทของ token ปัจจุบันโดยไม่เลื่อน token pointer
    bool check(TokenType type) const;

    // เลื่อน token pointer ไปข้างหน้าและคืนค่า token ก่อนหน้า
    Token advance();

    // ตรวจสอบว่าถึงจุดสิ้นสุดของโค้ดหรือไม่
    bool isAtEnd() const;

    // คืนค่า token ปัจจุบันโดยไม่เลื่อน token pointer
    Token peek() const;

    // คืนค่า token ก่อนหน้าโดยไม่เลื่อน token pointer
    Token previous() const;

    // ทำให้ token ปัจจุบันเป็นประเภทที่คาดหวัง ถ้าไม่ใช่ จะรายงานข้อผิดพลาด
    Token consume(TokenType type, const std::string& message);

    // รายงานข้อผิดพลาดที่ token ปัจจุบัน
    void error(const Token& token, const std::string& message);
    void error(const std::string& message);

    // ข้ามไปถึงคำสั่งถัดไปเมื่อเกิดข้อผิดพลาด
    void synchronize();

    // แปลงพารามิเตอร์แบบ key-value
    std::map<std::string, std::string> parseParameters();

    // ข้ามเครื่องหมายขึ้นบรรทัดใหม่ (ถ้ามี)
    void consumeNewlines();

    // เพิ่มประกาศฟังก์ชันที่ใช้ในไฟล์ parser.cpp แต่ไม่ได้ประกาศใน parser.h
    void resetError();
    std::shared_ptr<Statement> cleanStatement();
    std::shared_ptr<Statement> visualizeStatement();


public:
    Parser();
    Parser(const std::vector<Token>& tokens);

    // ตั้งค่าตัวจัดการข้อผิดพลาด
    void setErrorHandler(std::function<void(const std::string&)> handler);

    // ตรวจสอบว่ามีข้อผิดพลาดหรือไม่
    bool hasError() const;

    // สำหรับเวอร์ชันใหม่
    std::string getErrorMessage() const;
    std::vector<std::unique_ptr<Statement>> parse();

    // แปลง token เป็น AST และคืนค่าเป็นโครงสร้างต้นไม้
    std::shared_ptr<Program> parse(const std::vector<Token>& tokens);
    public:
    void resetError();
private:
};

/**
 * @brief ฟังก์ชันช่วยเหลือสำหรับแปลงคำสั่งและพารามิเตอร์
 */
std::map<std::string, std::string> parseParams(const std::string& paramString);

} // namespace ai_language

#endif // AI_LANGUAGE_PARSER_H