#include "../include/parser.h"
#include <stdexcept>

namespace ai_language {

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}

std::vector<std::unique_ptr<Statement>> Parser::parse() {
    std::vector<std::unique_ptr<Statement>> statements;
    
    while (!isAtEnd()) {
        try {
            auto stmt = parseStatement();
            if (stmt) {
                statements.push_back(std::move(stmt));
            }
        } catch (const std::exception& e) {
            // รายงานข้อผิดพลาด
            std::cerr << "Parse error: " << e.what() << std::endl;
            
            // Skip to the next statement or end
            while (!isAtEnd() && peek().type != TokenType::END) {
                advance();
            }
        }
    }
    
    return statements;
}

bool Parser::isAtEnd() const {
    return current >= tokens.size() || tokens[current].type == TokenType::END;
}

Token Parser::advance() {
    if (!isAtEnd()) {
        return tokens[current++];
    }
    return tokens[current];
}

Token Parser::peek() const {
    if (isAtEnd()) {
        return Token(TokenType::END);
    }
    return tokens[current];
}

Token Parser::previous() const {
    return tokens[current > 0 ? current - 1 : 0];
}

bool Parser::match(TokenType type) {
    if (isAtEnd()) return false;
    if (peek().type != type) return false;
    
    advance();
    return true;
}

std::unique_ptr<Statement> Parser::parseStatement() {
    TokenType tokenType = peek().type;
    
    switch (tokenType) {
        case TokenType::START:
            return parseStartStatement();
        case TokenType::LOAD:
            return parseLoadStatement();
        case TokenType::CLEAN:
            return parseCleanStatement();
        case TokenType::SPLIT:
            return parseSplitStatement();
        case TokenType::CREATE:
            return parseCreateStatement();
        case TokenType::TRAIN:
            return parseTrainStatement();
        case TokenType::EVALUATE:
            return parseEvaluateStatement();
        case TokenType::SHOW:
            return parseShowStatement();
        case TokenType::SAVE:
            return parseSaveStatement();
        case TokenType::VISUALIZE:
            return parseVisualizeStatement();
        case TokenType::PLOT:
            return parsePlotStatement();
        case TokenType::ADD:
            return parseAddLayerStatement();
        case TokenType::COMPILE:
            return parseCompileStatement();
        case TokenType::PREDICT:
            return parsePredictStatement();
        case TokenType::CONFIGURE:
            return parseConfigureStatement();
        case TokenType::COMPARE:
            return parseCompareStatement();
        default:
            // Skip unknown token
            advance();
            return nullptr;
    }
}

std::unique_ptr<Statement> Parser::parseStartStatement() {
    // "start create ML/DL/RL"
    advance(); // Consume "start"
    
    if (!match(TokenType::CREATE)) {
        throw std::runtime_error("Expected 'create' after 'start'");
    }
    
    if (peek().type != TokenType::IDENTIFIER) {
        throw std::runtime_error("Expected learning type (ML/DL/RL) after 'create'");
    }
    
    std::string learningType = advance().value;
    return std::make_unique<StartStatement>(learningType);
}

std::unique_ptr<Statement> Parser::parseLoadStatement() {
    // "load dataset "file.csv" [type "csv"]
    advance(); // Consume "load"
    
    if (peek().type != TokenType::IDENTIFIER && peek().value != "dataset") {
        throw std::runtime_error("Expected 'dataset' after 'load'");
    }
    
    advance(); // Consume "dataset"
    
    if (peek().type != TokenType::STRING) {
        throw std::runtime_error("Expected filename in quotes");
    }
    
    std::string filename = advance().value;
    std::string type;
    
    // Optional type
    if (match(TokenType::IDENTIFIER) && previous().value == "type") {
        if (peek().type != TokenType::STRING) {
            throw std::runtime_error("Expected type in quotes");
        }
        type = advance().value;
    }
    
    return std::make_unique<LoadStatement>(filename, type);
}

// เมธอดอื่นๆ ที่ยังไม่ได้ใช้ เช่น parseCleanStatement, parseSplitStatement, ...
// จะถูกเพิ่มในอนาคตเมื่อจำเป็น

std::unique_ptr<Statement> Parser::parseCleanStatement() {
    advance(); // Skip for now
    return nullptr;
}

std::unique_ptr<Statement> Parser::parseSplitStatement() {
    advance(); // Skip for now
    return nullptr;
}

std::unique_ptr<Statement> Parser::parseCreateStatement() {
    advance(); // Skip for now
    return nullptr;
}

std::unique_ptr<Statement> Parser::parseTrainStatement() {
    advance(); // Skip for now
    return nullptr;
}

std::unique_ptr<Statement> Parser::parseEvaluateStatement() {
    advance(); // Skip for now
    return nullptr;
}

std::unique_ptr<Statement> Parser::parseShowStatement() {
    advance(); // Skip for now
    return nullptr;
}

std::unique_ptr<Statement> Parser::parseSaveStatement() {
    advance(); // Skip for now
    return nullptr;
}

std::unique_ptr<Statement> Parser::parseVisualizeStatement() {
    advance(); // Skip for now
    return nullptr;
}

std::unique_ptr<Statement> Parser::parsePlotStatement() {
    advance(); // Skip for now
    return nullptr;
}

std::unique_ptr<Statement> Parser::parseAddLayerStatement() {
    advance(); // Skip for now
    return nullptr;
}

std::unique_ptr<Statement> Parser::parseCompileStatement() {
    advance(); // Skip for now
    return nullptr;
}

std::unique_ptr<Statement> Parser::parsePredictStatement() {
    advance(); // Skip for now
    return nullptr;
}

std::unique_ptr<Statement> Parser::parseConfigureStatement() {
    advance(); // Skip for now
    return nullptr;
}

std::unique_ptr<Statement> Parser::parseCompareStatement() {
    advance(); // Skip for now
    return nullptr;
}

std::unordered_map<std::string, std::string> Parser::parseParameters() {
    std::unordered_map<std::string, std::string> params;
    
    while (!isAtEnd() && peek().type == TokenType::IDENTIFIER) {
        std::string paramName = advance().value;
        
        // ตรวจสอบว่ามี value หรือไม่
        if (peek().type == TokenType::STRING || peek().type == TokenType::NUMBER || 
            peek().type == TokenType::IDENTIFIER) {
            std::string paramValue = advance().value;
            params[paramName] = paramValue;
        } else {
            // พารามิเตอร์ที่ไม่มีค่า (boolean flag)
            params[paramName] = "true";
        }
    }
    
    return params;
}

// Implementation of Statement classes

StartStatement::StartStatement(const std::string& learningType) : learningType(learningType) {}

void StartStatement::execute() const {
    std::cout << "เริ่มต้นการสร้างโปรเจกต์ประเภท: " << learningType << std::endl;
    
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

LoadStatement::LoadStatement(const std::string& filename, const std::string& type) 
    : filename(filename), type(type) {}

void LoadStatement::execute() const {
    std::cout << "กำลังโหลดข้อมูลจากไฟล์: " << filename << std::endl;
    if (!type.empty()) {
        std::cout << "ประเภทไฟล์: " << type << std::endl;
    }
}

} // namespace ai_language