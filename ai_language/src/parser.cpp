/**
 * @file parser.cpp
 * @brief การดำเนินการของ Parser สำหรับแปลง token เป็น AST
 */

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <memory>
#include <typeinfo>

#include "parser.h"

using namespace ai_language;

namespace ai_language {

// Implementation of Statement classes

StartStatement::StartStatement(const std::string& learningType) 
    : learningType(learningType) {}

std::string StartStatement::toString() const {
    return "StartStatement(" + learningType + ")";
}

void StartStatement::execute() const {
    std::cout << "> start" << std::endl;

    if (learningType == "ML" || learningType == "ml") {
        std::cout << "เริ่มต้นโปรเจกต์ Machine Learning" << std::endl;
    } else if (learningType == "DL" || learningType == "dl") {
        std::cout << "เริ่มต้นโปรเจกต์ Deep Learning" << std::endl;
    } else if (learningType == "RL" || learningType == "rl") {
        std::cout << "เริ่มต้นโปรเจกต์ Reinforcement Learning" << std::endl;
    } else {
        std::cout << "ไม่รู้จักประเภทการเรียนรู้: " << learningType << std::endl;
    }
}

LoadStatement::LoadStatement(const std::string& filename, const std::string& fileType) 
    : filename(filename), fileType(fileType) {}

std::string LoadStatement::toString() const {
    return "LoadStatement(filename=" + filename + ", type=" + fileType + ")";
}

void LoadStatement::execute() const {
    std::cout << "> load dataset filename " << filename << " type " << fileType << std::endl;
    std::cout << "กำลังโหลดข้อมูลจากไฟล์: " << filename << std::endl;
}

CleanStatement::CleanStatement(const std::map<std::string, std::string>& params) 
    : params(params) {}

std::string CleanStatement::toString() const {
    std::stringstream ss;
    ss << "CleanStatement(";

    for (const auto& pair : params) {
        ss << pair.first << "=" << pair.second << ", ";
    }

    ss << ")";
    return ss.str();
}

void CleanStatement::execute() const {
    std::cout << "> clean" << std::endl;
    std::cout << "กำลังทำความสะอาดข้อมูล" << std::endl;

    for (const auto& pair : params) {
        std::cout << "  พารามิเตอร์: " << pair.first << " = " << pair.second << std::endl;
    }
}

SplitStatement::SplitStatement(const std::map<std::string, std::string>& params) 
    : params(params) {}

std::string SplitStatement::toString() const {
    std::stringstream ss;
    ss << "SplitStatement(";

    for (const auto& pair : params) {
        ss << pair.first << "=" << pair.second << ", ";
    }

    ss << ")";
    return ss.str();
}

void SplitStatement::execute() const {
    std::cout << "> split" << std::endl;
    std::cout << "กำลังแบ่งข้อมูล" << std::endl;

    for (const auto& pair : params) {
        std::cout << "  พารามิเตอร์: " << pair.first << " = " << pair.second << std::endl;
    }
}

TrainStatement::TrainStatement(const std::map<std::string, std::string>& params) 
    : params(params) {}

std::string TrainStatement::toString() const {
    std::stringstream ss;
    ss << "TrainStatement(";

    for (const auto& pair : params) {
        ss << pair.first << "=" << pair.second << ", ";
    }

    ss << ")";
    return ss.str();
}

void TrainStatement::execute() const {
    std::cout << "> train";

    if (params.find("epochs") != params.end()) {
        std::cout << " epochs " << params.at("epochs");
    }

    std::cout << std::endl;
    std::cout << "กำลังฝึกโมเดล" << std::endl;

    for (const auto& pair : params) {
        if (pair.first != "epochs") {
            std::cout << "  พารามิเตอร์: " << pair.first << " = " << pair.second << std::endl;
        }
    }
}

EvaluateStatement::EvaluateStatement(const std::map<std::string, std::string>& params) 
    : params(params) {}

std::string EvaluateStatement::toString() const {
    std::stringstream ss;
    ss << "EvaluateStatement(";

    for (const auto& pair : params) {
        ss << pair.first << "=" << pair.second << ", ";
    }

    ss << ")";
    return ss.str();
}

void EvaluateStatement::execute() const {
    std::cout << "> evaluate" << std::endl;
    std::cout << "กำลังประเมินผลโมเดล" << std::endl;

    for (const auto& pair : params) {
        std::cout << "  พารามิเตอร์: " << pair.first << " = " << pair.second << std::endl;
    }
}

PredictStatement::PredictStatement(const std::map<std::string, std::string>& params) 
    : params(params) {}

std::string PredictStatement::toString() const {
    std::stringstream ss;
    ss << "PredictStatement(";

    for (const auto& pair : params) {
        ss << pair.first << "=" << pair.second << ", ";
    }

    ss << ")";
    return ss.str();
}

void PredictStatement::execute() const {
    std::cout << "> predict" << std::endl;
    std::cout << "กำลังทำนายผลลัพธ์" << std::endl;

    for (const auto& pair : params) {
        std::cout << "  พารามิเตอร์: " << pair.first << " = " << pair.second << std::endl;
    }
}

SaveStatement::SaveStatement(const std::map<std::string, std::string>& params) 
    : params(params) {}

std::string SaveStatement::toString() const {
    std::stringstream ss;
    ss << "SaveStatement(";

    for (const auto& pair : params) {
        ss << pair.first << "=" << pair.second << ", ";
    }

    ss << ")";
    return ss.str();
}

void SaveStatement::execute() const {
    std::cout << "> save";

    if (params.find("path") != params.end()) {
        std::cout << " path " << params.at("path");
    }

    std::cout << std::endl;

    if (params.find("path") != params.end()) {
        std::cout << "กำลังบันทึกโมเดลไปยัง: " << params.at("path") << std::endl;
    } else {
        std::cout << "กำลังบันทึกโมเดล" << std::endl;
    }
}

ShowStatement::ShowStatement(const std::string& target, const std::map<std::string, std::string>& params) 
    : target(target), params(params) {}

std::string ShowStatement::toString() const {
    std::stringstream ss;
    ss << "ShowStatement(target=" << target << ", params={";

    for (const auto& pair : params) {
        ss << pair.first << "=" << pair.second << ", ";
    }

    ss << "})";
    return ss.str();
}

void ShowStatement::execute() const {
    std::cout << "> show " << target;

    if (params.find("metric") != params.end()) {
        std::cout << " " << params.at("metric");
    }

    std::cout << std::endl;

    if (target == "metric") {
        std::cout << "กำลังแสดงเมตริก: " << params.at("metric") << std::endl;
    } else {
        std::cout << "กำลังแสดง: " << target << std::endl;
    }
}

VisualizeStatement::VisualizeStatement(const std::string& target, const std::map<std::string, std::string>& params) 
    : target(target), params(params) {}

std::string VisualizeStatement::toString() const {
    std::stringstream ss;
    ss << "VisualizeStatement(target=" << target << ", params={";

    for (const auto& pair : params) {
        ss << pair.first << "=" << pair.second << ", ";
    }

    ss << "})";
    return ss.str();
}

void VisualizeStatement::execute() const {
    std::cout << "> visualize " << target << std::endl;
    std::cout << "กำลังแสดงภาพ: " << target << std::endl;

    for (const auto& pair : params) {
        std::cout << "  พารามิเตอร์: " << pair.first << " = " << pair.second << std::endl;
    }
}

PlotStatement::PlotStatement(const std::string& target, const std::map<std::string, std::string>& params) 
    : target(target), params(params) {}

std::string PlotStatement::toString() const {
    std::stringstream ss;
    ss << "PlotStatement(target=" << target << ", params={";

    for (const auto& pair : params) {
        ss << pair.first << "=" << pair.second << ", ";
    }

    ss << "})";
    return ss.str();
}

void PlotStatement::execute() const {
    std::cout << "> plot " << target << std::endl;
    std::cout << "กำลังพล็อต: " << target << std::endl;

    for (const auto& pair : params) {
        std::cout << "  พารามิเตอร์: " << pair.first << " = " << pair.second << std::endl;
    }
}

Program::Program(const std::vector<std::shared_ptr<Statement>>& statements) 
    : statements(statements) {}

void Program::execute() const {
    for (const auto& statement : statements) {
        statement->execute();
    }
}

// Implementation of Parser

Parser::Parser() 
    : m_current(0), m_hasError(false) {
    // ตั้งค่าฟังก์ชันสำหรับแสดงข้อผิดพลาดเริ่มต้น
    m_errorHandler = [](const std::string& message) {
        std::cerr << "Parser Error: " << message << std::endl;
    };
}

void Parser::setErrorHandler(std::function<void(const std::string&)> handler) {
    m_errorHandler = handler;
}

std::shared_ptr<Program> Parser::parse(const std::vector<Token>& tokens) {
    // รีเซ็ตสถานะ
    m_tokens = tokens;
    m_current = 0;
    m_hasError = false;

    std::vector<std::shared_ptr<Statement>> statements;

    // แปลงคำสั่งจนกว่าจะเป็นจุดสิ้นสุดโค้ด
    while (!isAtEnd()) {
        try {
            // ข้ามบรรทัดว่าง
            consumeNewlines();

            // ถ้าถึงจุดสิ้นสุดโค้ดแล้ว ออกจากลูป
            if (isAtEnd()) break;

            // แปลงคำสั่ง
            statements.push_back(declaration());

            // ข้ามบรรทัดว่าง
            consumeNewlines();
        } catch (const std::exception& e) {
            // จัดการกับข้อผิดพลาด
            synchronize();
        }
    }

    return std::make_shared<Program>(statements);
}

bool Parser::hasError() const {
    return m_hasError;
}

void Parser::resetError() {
    m_hasError = false;
}

bool Parser::isAtEnd() const {
    return peek().type == TokenType::END;
}

Token Parser::peek() const {
    return m_tokens[m_current];
}

Token Parser::previous() const {
    return m_tokens[m_current - 1];
}

Token Parser::advance() {
    if (!isAtEnd()) m_current++;
    return previous();
}

bool Parser::check(TokenType type) const {
    if (isAtEnd()) return false;
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::match(const std::vector<TokenType>& types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

void Parser::consumeNewlines() {
    // Using line breaks instead of NEWLINE token
    while (peek().type == TokenType::END || peek().type == TokenType::COMMENT) {
        // เพียงแค่ข้ามไป
        advance();
    }
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();

    error(peek(), message);
    throw std::runtime_error(message);
}

std::shared_ptr<Statement> Parser::declaration() {
    return statement();
}

std::shared_ptr<Statement> Parser::statement() {
    if (match(TokenType::START)) {
        return startStatement();
    } else if (match(TokenType::LOAD)) {
        return loadStatement();
    } else if (match(TokenType::CLEAN)) {
        return cleanStatement();
    } else if (match(TokenType::SPLIT)) {
        return splitStatement();
    } else if (match(TokenType::TRAIN)) {
        return trainStatement();
    } else if (match(TokenType::EVALUATE)) {
        return evaluateStatement();
    } else if (match(TokenType::EVALUATE)) {  // Use EVALUATE instead of PREDICT
        return predictStatement();
    } else if (match(TokenType::SAVE)) {
        return saveStatement();
    } else if (match(TokenType::SHOW)) {
        return showStatement();
    } else if (match(TokenType::VISUALIZE)) {
        return visualizeStatement();
    } else if (match(TokenType::PLOT)) {
        return plotStatement();
    }

    error(peek(), "คาดหวังคำสั่ง");
    throw std::runtime_error("คาดหวังคำสั่ง");
}

std::shared_ptr<Statement> Parser::startStatement() {
    // start create <ประเภท>
    if (match(TokenType::IDENTIFIER) && previous().value == "create") {
        // ML, DL, RL would be identifiers in our lexer
        Token modelType = consume(TokenType::IDENTIFIER, "คาดหวังประเภทโมเดล (ML, DL, RL)");
        std::string modelTypeStr = modelType.value;
        return std::make_shared<StartStatement>(previous().value);
    } else if (match(TokenType::IDENTIFIER)) {
        return std::make_shared<StartStatement>(previous().value);
    }

    error(peek(), "คาดหวัง 'create' ตามด้วยประเภทการเรียนรู้ (ML, DL, RL)");
    throw std::runtime_error("คาดหวัง 'create' ตามด้วยประเภทการเรียนรู้");
}

std::shared_ptr<Statement> Parser::loadStatement() {
    // load dataset "<ที่อยู่ไฟล์>" type "<ประเภทไฟล์>"
    if (match(TokenType::DATA)) {
        std::string filename;
        std::string fileType;

        if (match(TokenType::IDENTIFIER) && previous().value == "filename") {
            if (match(TokenType::STRING) || match(TokenType::IDENTIFIER)) {
                filename = previous().value;
            } else {
                error(peek(), "คาดหวังชื่อไฟล์");
                throw std::runtime_error("คาดหวังชื่อไฟล์");
            }
        } else {
            error(peek(), "คาดหวัง 'filename'");
            throw std::runtime_error("คาดหวัง 'filename'");
        }

        if (match(TokenType::TYPE)) {
            if (match(TokenType::STRING) || match(TokenType::IDENTIFIER)) {
                fileType = previous().value;
            } else {
                error(peek(), "คาดหวังประเภทไฟล์");
                throw std::runtime_error("คาดหวังประเภทไฟล์");
            }
        } else {
            error(peek(), "คาดหวัง 'type'");
            throw std::runtime_error("คาดหวัง 'type'");
        }

        return std::make_shared<LoadStatement>(filename, fileType);
    }

    error(peek(), "คาดหวัง 'dataset'");
    throw std::runtime_error("คาดหวัง 'dataset'");
}

std::shared_ptr<Statement> Parser::cleanStatement() {
    // clean [with <พารามิเตอร์>]
    std::map<std::string, std::string> params;

    if (match(TokenType::WITH)) {
        params = parseParameters();
    }

    return std::make_shared<CleanStatement>(params);
}

std::shared_ptr<Statement> Parser::splitStatement() {
    // split [with <พารามิเตอร์>]
    std::map<std::string, std::string> params;

    if (match(TokenType::WITH)) {
        params = parseParameters();
    }

    return std::make_shared<SplitStatement>(params);
}

std::shared_ptr<Statement> Parser::trainStatement() {
    // train [epochs <จำนวน>] [with <พารามิเตอร์>]
    std::map<std::string, std::string> params;

    if (match(TokenType::EPOCHS)) {
        if (match(TokenType::NUMBER)) {
            params["epochs"] = previous().value;
        } else {
            error(peek(), "คาดหวังจำนวน epochs");
            throw std::runtime_error("คาดหวังจำนวน epochs");
        }
    }

    if (match(TokenType::WITH)) {
        std::map<std::string, std::string> withParams = parseParameters();
        params.insert(withParams.begin(), withParams.end());
    }

    return std::make_shared<TrainStatement>(params);
}

std::shared_ptr<Statement> Parser::evaluateStatement() {
    // evaluate [with <พารามิเตอร์>]
    std::map<std::string, std::string> params;

    if (match(TokenType::WITH)) {
        params = parseParameters();
    }

    return std::make_shared<EvaluateStatement>(params);
}

std::shared_ptr<Statement> Parser::predictStatement() {
    // predict [with <พารามิเตอร์>]
    std::map<std::string, std::string> params;

    if (match(TokenType::WITH)) {
        params = parseParameters();
    }

    return std::make_shared<PredictStatement>(params);
}

std::shared_ptr<Statement> Parser::saveStatement() {
    // save [path <ชื่อไฟล์>] [with <พารามิเตอร์>]
    std::map<std::string, std::string> params;

    if (match(TokenType::PATH)) {
        if (match(TokenType::STRING) || match(TokenType::IDENTIFIER)) {
            params["path"] = previous().value;
        } else {
            error(peek(), "คาดหวังชื่อไฟล์");
            throw std::runtime_error("คาดหวังชื่อไฟล์");
        }
    }

    if (match(TokenType::WITH)) {
        std::map<std::string, std::string> withParams = parseParameters();
        params.insert(withParams.begin(), withParams.end());
    }

    return std::make_shared<SaveStatement>(params);
}

std::shared_ptr<Statement> Parser::showStatement() {
    // show <เป้าหมาย> [<รายละเอียด>] [with <พารามิเตอร์>]
    if (match(TokenType::METRIC)) {
        std::map<std::string, std::string> params;

        if (match(TokenType::IDENTIFIER)) {
            params["metric"] = previous().value;
        } else {
            error(peek(), "คาดหวังชื่อเมทริก");
            throw std::runtime_error("คาดหวังชื่อเมทริก");
        }

        if (match(TokenType::WITH)) {
            std::map<std::string, std::string> withParams = parseParameters();
            params.insert(withParams.begin(), withParams.end());
        }

        return std::make_shared<ShowStatement>("metric", params);
    } else if (match(TokenType::MODEL)) {
        std::map<std::string, std::string> params;

        if (match(TokenType::WITH)) {
            params = parseParameters();
        }

        return std::make_shared<ShowStatement>("model", params);
    } else if (match(TokenType::IDENTIFIER)) {
        std::string target = previous().value;
        std::map<std::string, std::string> params;

        if (match(TokenType::WITH)) {
            params = parseParameters();
        }

        return std::make_shared<ShowStatement>(target, params);
    }

    error(peek(), "คาดหวังเป้าหมายที่ต้องการแสดง (metric, model)");
    throw std::runtime_error("คาดหวังเป้าหมายที่ต้องการแสดง");
}

std::shared_ptr<Statement> Parser::visualizeStatement() {
    // visualize <เป้าหมาย> [with <พารามิเตอร์>]
    if (match(TokenType::IDENTIFIER)) {
        std::string target = previous().value;
        std::map<std::string, std::string> params;

        if (match(TokenType::WITH)) {
            params = parseParameters();
        }

        return std::make_shared<VisualizeStatement>(target, params);
    }

    error(peek(), "คาดหวังเป้าหมายที่ต้องการแสดงภาพ");
    throw std::runtime_error("คาดหวังเป้าหมายที่ต้องการแสดงภาพ");
}

std::shared_ptr<Statement> Parser::plotStatement() {
    // plot <เป้าหมาย> [with <พารามิเตอร์>]
    if (match(TokenType::IDENTIFIER)) {
        std::string target = previous().value;
        std::map<std::string, std::string> params;

        if (match(TokenType::WITH)) {
            params = parseParameters();
        }

        return std::make_shared<PlotStatement>(target, params);
    }

    error(peek(), "คาดหวังเป้าหมายที่ต้องการพล็อต");
    throw std::runtime_error("คาดหวังเป้าหมายที่ต้องการพล็อต");
}

std::map<std::string, std::string> Parser::parseParameters() {
    std::map<std::string, std::string> params;

    while (match(TokenType::IDENTIFIER)) {
        std::string paramName = previous().value;

        if (match(TokenType::EQUAL)) {
            // <ชื่อ> = <ค่า>
            if (match(TokenType::NUMBER) || match(TokenType::STRING) || match(TokenType::IDENTIFIER)) {
                params[paramName] = previous().value;
            } else {
                error(peek(), "คาดหวังค่าพารามิเตอร์");
                throw std::runtime_error("คาดหวังค่าพารามิเตอร์");
            }
        } else {
            // <ชื่อ> (เป็นธง)
            params[paramName] = "true";
        }
    }

    return params;
}

void Parser::error(const std::string& message) {
    error(peek(), message);
}

void Parser::error(const Token& token, const std::string& message) {
    std::stringstream ss;

    if (token.type == TokenType::END) {
        ss << "บรรทัดที่ " << token.line << ": ข้อผิดพลาดที่จุดสิ้นสุดโค้ด: " << message;
    } else {
        ss << "บรรทัดที่ " << token.line << " ตำแหน่ง " << token.column << ": ";
        ss << "ข้อผิดพลาดที่ '" << token.value << "': " << message;
    }

    if (m_errorHandler) {
        m_errorHandler(ss.str());
    }

    m_hasError = true;
}

void Parser::synchronize() {
    advance();

    while (!isAtEnd()) {
        if (previous().type == TokenType::END) return;

        switch (peek().type) {
            case TokenType::START:
            case TokenType::LOAD:
            case TokenType::CLEAN:
            case TokenType::SPLIT:
            case TokenType::TRAIN:
            case TokenType::EVALUATE:
            case TokenType::SAVE:
            case TokenType::SHOW:
            case TokenType::VISUALIZE:
            case TokenType::PLOT:
                return;
            default:
                break;
        }

        advance();
    }
}

std::map<std::string, std::string> parseParams(const std::string& paramString) {
    std::map<std::string, std::string> params;
    std::istringstream iss(paramString);
    std::string token;

    while (iss >> token) {
        size_t pos = token.find('=');
        if (pos != std::string::npos) {
            std::string key = token.substr(0, pos);
            std::string value = token.substr(pos + 1);

            // ถ้าค่าเป็น "..." ให้ตัด " ออก
            if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
                value = value.substr(1, value.size() - 2);
            }

            params[key] = value;
        } else {
            // กรณีเป็นพารามิเตอร์แบบธง
            params[token] = "true";
        }
    }

    return params;
}

} // namespace ai_language
/**
 * @file parser.cpp
 * @brief การทำงานของ Parser สำหรับแปลงความหมายของ Token เป็นโครงสร้างข้อมูล AST
 */

#include "../include/parser.h"
#include <iostream>
#include <cassert>

namespace ai_language {

Parser::Parser(const std::vector<Token>& tokens)
    : m_tokens(tokens), m_current(0), m_hasError(false), m_errorMsg("") {
}

void Parser::setErrorHandler(std::function<void(const std::string&)> handler) {
    m_errorHandler = handler;
}

bool Parser::hasError() const {
    return m_hasError;
}

std::string Parser::getErrorMessage() const {
    return m_errorMsg;
}

std::vector<std::unique_ptr<Statement>> Parser::parse() {
    std::vector<std::unique_ptr<Statement>> statements;
    
    while (!isAtEnd()) {
        try {
            auto stmt = parseStatement();
            if (stmt) {
                statements.push_back(std::move(stmt));
            }
        } catch (const ParserError& e) {
            m_hasError = true;
            m_errorMsg = e.what();
            
            if (m_errorHandler) {
                m_errorHandler(e.what());
            }
            
            // ข้ามไปถึงคำสั่งถัดไปเพื่อดำเนินการต่อ
            synchronize();
        }
    }
    
    return statements;
}

std::unique_ptr<Statement> Parser::parseStatement() {
    if (match(TokenType::START)) {
        return parseStartStatement();
    } else if (match(TokenType::CREATE)) {
        return parseCreateStatement();
    } else if (match(TokenType::LOAD)) {
        return parseLoadStatement();
    } else if (match(TokenType::SET)) {
        return parseSetStatement();
    } else if (match(TokenType::TRAIN)) {
        return parseTrainStatement();
    } else if (match(TokenType::EVALUATE)) {
        return parseEvaluateStatement();
    } else if (match(TokenType::SHOW)) {
        return parseShowStatement();
    } else if (match(TokenType::SAVE)) {
        return parseSaveStatement();
    } else if (match(TokenType::ADD)) {
        return parseAddLayerStatement();
    } else if (match(TokenType::PREDICT)) {
        return parsePredictStatement();
    } else if (match(TokenType::END_KEYWORD)) {
        return parseEndStatement();
    } else {
        throw ParserError("คำสั่งไม่รองรับ: " + peek().value + " ที่บรรทัด " + std::to_string(peek().line));
    }
    
    return nullptr;
}

std::unique_ptr<Statement> Parser::parseStartStatement() {
    // start คำสั่งไม่ต้องมีพารามิเตอร์เพิ่มเติม
    auto stmt = std::make_unique<StartStatement>();
    stmt->token = previous();
    consumeEndOfStatement();
    return stmt;
}

std::unique_ptr<Statement> Parser::parseCreateStatement() {
    if (match(TokenType::MODEL)) {
        // create model <model_name>
        auto token = previous();
        std::string modelName;
        
        if (match(TokenType::IDENTIFIER)) {
            modelName = previous().value;
        } else {
            throw ParserError("คาดว่าจะเป็นชื่อโมเดล ที่บรรทัด " + std::to_string(peek().line));
        }
        
        auto stmt = std::make_unique<CreateModelStatement>();
        stmt->token = token;
        stmt->modelName = modelName;
        
        // พารามิเตอร์เพิ่มเติมของโมเดล (ถ้ามี)
        while (!isAtEndOfStatement() && !isAtEnd()) {
            if (match(TokenType::IDENTIFIER)) {
                std::string paramName = previous().value;
                std::string paramValue;
                
                if (match(TokenType::IDENTIFIER) || match(TokenType::NUMBER) || match(TokenType::STRING)) {
                    paramValue = previous().value;
                } else {
                    throw ParserError("คาดว่าจะเป็นค่าพารามิเตอร์ ที่บรรทัด " + std::to_string(peek().line));
                }
                
                stmt->parameters[paramName] = paramValue;
            } else {
                // ถ้าไม่ใช่ identifier ก็ถือว่าจบพารามิเตอร์
                break;
            }
        }
        
        consumeEndOfStatement();
        return stmt;
    } else if (match(TokenType::ML_TYPE) || match(TokenType::DL_TYPE) || match(TokenType::RL_TYPE) || 
              (peek().type == TokenType::IDENTIFIER && 
              (peek().value == "ML" || peek().value == "DL" || peek().value == "RL"))) {
        // create ML|DL|RL
        auto token = previous();
        std::string projectType;
        
        if (token.type == TokenType::IDENTIFIER) {
            projectType = token.value;
        } else {
            projectType = token.value;
        }
        
        auto stmt = std::make_unique<CreateProjectStatement>();
        stmt->token = token;
        stmt->projectType = projectType;
        
        consumeEndOfStatement();
        return stmt;
    } else {
        throw ParserError("คำสั่ง 'create' ไม่ถูกต้อง ต้องตามด้วย 'model', 'ML', 'DL', หรือ 'RL' ที่บรรทัด " + std::to_string(peek().line));
    }
    
    return nullptr;
}

std::unique_ptr<Statement> Parser::parseLoadStatement() {
    if (match(TokenType::DATASET) || match(TokenType::IDENTIFIER)) {
        // load dataset "path/to/data.csv" [type "csv"]
        auto token = previous();
        std::string dataPath;
        
        if (!match(TokenType::STRING)) {
            throw ParserError("คาดว่าจะเป็นพาธของข้อมูลในรูปแบบสตริง ที่บรรทัด " + std::to_string(peek().line));
        }
        
        dataPath = previous().value;
        
        auto stmt = std::make_unique<LoadDatasetStatement>();
        stmt->token = token;
        stmt->dataPath = dataPath;
        
        // พารามิเตอร์เพิ่มเติม เช่น type
        while (!isAtEndOfStatement() && !isAtEnd()) {
            if (match(TokenType::TYPE) || (match(TokenType::IDENTIFIER) && previous().value == "type")) {
                if (!match(TokenType::STRING)) {
                    throw ParserError("คาดว่าจะเป็นประเภทข้อมูลในรูปแบบสตริง ที่บรรทัด " + std::to_string(peek().line));
                }
                stmt->dataType = previous().value;
            } else {
                // ถ้าไม่ใช่พารามิเตอร์ที่รองรับ ให้ข้ามไป
                advance();
            }
        }
        
        consumeEndOfStatement();
        return stmt;
    } else if (match(TokenType::MODEL)) {
        // load model "path/to/model.dat"
        auto token = previous();
        
        if (!match(TokenType::STRING)) {
            throw ParserError("คาดว่าจะเป็นพาธของโมเดลในรูปแบบสตริง ที่บรรทัด " + std::to_string(peek().line));
        }
        
        std::string modelPath = previous().value;
        
        auto stmt = std::make_unique<LoadModelStatement>();
        stmt->token = token;
        stmt->modelPath = modelPath;
        
        consumeEndOfStatement();
        return stmt;
    } else if (match(TokenType::ENVIRONMENT) || 
              (match(TokenType::IDENTIFIER) && previous().value == "environment")) {
        // load environment "path/to/env.json"
        auto token = previous();
        
        if (!match(TokenType::STRING)) {
            throw ParserError("คาดว่าจะเป็นพาธของสภาพแวดล้อมในรูปแบบสตริง ที่บรรทัด " + std::to_string(peek().line));
        }
        
        std::string envPath = previous().value;
        
        auto stmt = std::make_unique<LoadEnvironmentStatement>();
        stmt->token = token;
        stmt->environmentPath = envPath;
        
        consumeEndOfStatement();
        return stmt;
    } else {
        throw ParserError("คำสั่ง 'load' ไม่ถูกต้อง ต้องตามด้วย 'dataset', 'model', หรือ 'environment' ที่บรรทัด " + std::to_string(peek().line));
    }
    
    return nullptr;
}

std::unique_ptr<Statement> Parser::parseSetStatement() {
    auto token = previous();
    std::string paramName;
    
    if (match(TokenType::IDENTIFIER) || 
        match(TokenType::LEARNING_RATE) || 
        match(TokenType::BATCH_SIZE) || 
        match(TokenType::EPOCHS) || 
        match(TokenType::TIMEZONE) ||
        match(TokenType::DISCOUNT_FACTOR)) {
        
        paramName = previous().value;
    } else {
        throw ParserError("คาดว่าจะเป็นชื่อพารามิเตอร์ ที่บรรทัด " + std::to_string(peek().line));
    }
    
    // รับค่าพารามิเตอร์
    if (!match(TokenType::NUMBER) && !match(TokenType::STRING) && !match(TokenType::IDENTIFIER)) {
        throw ParserError("คาดว่าจะเป็นค่าพารามิเตอร์ ที่บรรทัด " + std::to_string(peek().line));
    }
    
    std::string paramValue = previous().value;
    
    auto stmt = std::make_unique<SetParameterStatement>();
    stmt->token = token;
    stmt->paramName = paramName;
    stmt->paramValue = paramValue;
    
    consumeEndOfStatement();
    return stmt;
}

std::unique_ptr<Statement> Parser::parseTrainStatement() {
    if (match(TokenType::MODEL) || (match(TokenType::IDENTIFIER) && previous().value == "model")) {
        // train model
        auto token = previous();
        
        auto stmt = std::make_unique<TrainModelStatement>();
        stmt->token = token;
        
        consumeEndOfStatement();
        return stmt;
    } else {
        throw ParserError("คำสั่ง 'train' ไม่ถูกต้อง ต้องตามด้วย 'model' ที่บรรทัด " + std::to_string(peek().line));
    }
    
    return nullptr;
}

std::unique_ptr<Statement> Parser::parseEvaluateStatement() {
    if (match(TokenType::MODEL) || (match(TokenType::IDENTIFIER) && previous().value == "model")) {
        // evaluate model
        auto token = previous();
        
        auto stmt = std::make_unique<EvaluateModelStatement>();
        stmt->token = token;
        
        consumeEndOfStatement();
        return stmt;
    } else {
        throw ParserError("คำสั่ง 'evaluate' ไม่ถูกต้อง ต้องตามด้วย 'model' ที่บรรทัด " + std::to_string(peek().line));
    }
    
    return nullptr;
}

std::unique_ptr<Statement> Parser::parseShowStatement() {
    auto token = previous();
    std::string metricType;
    
    if (match(TokenType::ACCURACY) || match(TokenType::LOSS) || 
        match(TokenType::PERFORMANCE) || match(TokenType::GRAPH) ||
        (match(TokenType::IDENTIFIER) && 
         (previous().value == "accuracy" || previous().value == "loss" ||
          previous().value == "performance" || previous().value == "graph"))) {
        
        metricType = previous().value;
    } else {
        throw ParserError("คำสั่ง 'show' ไม่ถูกต้อง ต้องตามด้วย 'accuracy', 'loss', 'performance', หรือ 'graph' ที่บรรทัด " + std::to_string(peek().line));
    }
    
    auto stmt = std::make_unique<ShowMetricStatement>();
    stmt->token = token;
    stmt->metricType = metricType;
    
    consumeEndOfStatement();
    return stmt;
}

std::unique_ptr<Statement> Parser::parseSaveStatement() {
    if (match(TokenType::MODEL) || (match(TokenType::IDENTIFIER) && previous().value == "model")) {
        // save model "path/to/save.dat"
        auto token = previous();
        
        if (!match(TokenType::STRING)) {
            throw ParserError("คาดว่าจะเป็นพาธสำหรับบันทึกโมเดลในรูปแบบสตริง ที่บรรทัด " + std::to_string(peek().line));
        }
        
        std::string savePath = previous().value;
        
        auto stmt = std::make_unique<SaveModelStatement>();
        stmt->token = token;
        stmt->savePath = savePath;
        
        consumeEndOfStatement();
        return stmt;
    } else {
        throw ParserError("คำสั่ง 'save' ไม่ถูกต้อง ต้องตามด้วย 'model' ที่บรรทัด " + std::to_string(peek().line));
    }
    
    return nullptr;
}

std::unique_ptr<Statement> Parser::parseAddLayerStatement() {
    if (match(TokenType::LAYER) || (match(TokenType::IDENTIFIER) && previous().value == "layer")) {
        // add layer [layer_type] [parameters...]
        auto token = previous();
        
        if (!match(TokenType::IDENTIFIER)) {
            throw ParserError("คาดว่าจะเป็นประเภทของเลเยอร์ ที่บรรทัด " + std::to_string(peek().line));
        }
        
        std::string layerType = previous().value;
        
        auto stmt = std::make_unique<AddLayerStatement>();
        stmt->token = token;
        stmt->layerType = layerType;
        
        // พารามิเตอร์ของเลเยอร์
        while (!isAtEndOfStatement() && !isAtEnd()) {
            if (match(TokenType::IDENTIFIER)) {
                std::string paramName = previous().value;
                
                if (!match(TokenType::NUMBER) && !match(TokenType::STRING) && !match(TokenType::IDENTIFIER)) {
                    throw ParserError("คาดว่าจะเป็นค่าพารามิเตอร์สำหรับเลเยอร์ ที่บรรทัด " + std::to_string(peek().line));
                }
                
                std::string paramValue = previous().value;
                stmt->parameters[paramName] = paramValue;
            } else if (match(TokenType::NUMBER) || match(TokenType::STRING)) {
                // กรณีที่ไม่มีชื่อพารามิเตอร์ แต่มีค่าโดยตรง
                std::string value = previous().value;
                stmt->orderedParams.push_back(value);
            } else {
                // ถ้าไม่ใช่ identifier, number หรือ string ก็ถือว่าจบพารามิเตอร์
                break;
            }
        }
        
        consumeEndOfStatement();
        return stmt;
    } else {
        throw ParserError("คำสั่ง 'add' ไม่ถูกต้อง ต้องตามด้วย 'layer' ที่บรรทัด " + std::to_string(peek().line));
    }
    
    return nullptr;
}

std::unique_ptr<Statement> Parser::parsePredictStatement() {
    // predict [parameters...]
    auto token = previous();
    
    auto stmt = std::make_unique<PredictStatement>();
    stmt->token = token;
    
    // พารามิเตอร์สำหรับการทำนาย
    while (!isAtEndOfStatement() && !isAtEnd()) {
        if (match(TokenType::IDENTIFIER)) {
            std::string paramName = previous().value;
            
            if (!match(TokenType::NUMBER) && !match(TokenType::STRING) && !match(TokenType::IDENTIFIER)) {
                throw ParserError("คาดว่าจะเป็นค่าพารามิเตอร์สำหรับการทำนาย ที่บรรทัด " + std::to_string(peek().line));
            }
            
            std::string paramValue = previous().value;
            stmt->parameters[paramName] = paramValue;
        } else if (match(TokenType::STRING)) {
            // กรณีที่เป็นข้อความสำหรับทำนายโดยตรง
            stmt->predictInput = previous().value;
        } else {
            // ถ้าไม่ใช่ identifier หรือ string ก็ถือว่าจบพารามิเตอร์
            break;
        }
    }
    
    consumeEndOfStatement();
    return stmt;
}

std::unique_ptr<Statement> Parser::parseEndStatement() {
    // end
    auto token = previous();
    
    auto stmt = std::make_unique<EndStatement>();
    stmt->token = token;
    
    consumeEndOfStatement();
    return stmt;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) {
        return advance();
    }
    
    throw ParserError(message + " ที่บรรทัด " + std::to_string(peek().line));
}

void Parser::consumeEndOfStatement() {
    // ในภาษา AI Language แต่ละคำสั่งจบด้วยการขึ้นบรรทัดใหม่หรือสิ้นสุดไฟล์
    // ไม่ต้องทำอะไรเพิ่มเติม เพราะเราแยกคำสั่งตามบรรทัดอยู่แล้ว
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) {
        return false;
    }
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) {
        m_current++;
    }
    return previous();
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::END || m_current >= m_tokens.size();
}

bool Parser::isAtEndOfStatement() {
    return isAtEnd() || peek().type == TokenType::NEWLINE;
}

Token Parser::peek() {
    if (m_current >= m_tokens.size()) {
        // สร้าง token ปลอมเป็น END ถ้าเกินขอบเขต
        Token endToken;
        endToken.type = TokenType::END;
        endToken.value = "END";
        endToken.line = m_tokens.empty() ? 1 : m_tokens.back().line;
        endToken.column = m_tokens.empty() ? 1 : m_tokens.back().column;
        return endToken;
    }
    return m_tokens[m_current];
}

Token Parser::previous() {
    assert(m_current > 0);
    return m_tokens[m_current - 1];
}

void Parser::synchronize() {
    advance();
    
    while (!isAtEnd()) {
        // ข้ามไปจนกว่าจะเจอคำสั่งสำคัญ (เช่น start, create, load, ฯลฯ)
        if (previous().type == TokenType::NEWLINE) {
            return;
        }
        
        switch (peek().type) {
            case TokenType::START:
            case TokenType::CREATE:
            case TokenType::LOAD:
            case TokenType::SET:
            case TokenType::TRAIN:
            case TokenType::EVALUATE:
            case TokenType::SHOW:
            case TokenType::SAVE:
            case TokenType::ADD:
                return;
            default:
                break;
        }
        
        advance();
    }
}

} // namespace ai_language
