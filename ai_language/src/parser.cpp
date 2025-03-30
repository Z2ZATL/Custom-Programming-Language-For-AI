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
#include "../include/parser.h"

namespace ai_language {
    // placeholder 
    // Implementation will be added later
}
