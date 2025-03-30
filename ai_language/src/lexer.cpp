
/**
 * @file lexer.cpp
 * @brief การดำเนินการของ Lexer สำหรับแยกโค้ดเป็น token
 */

#include "../include/lexer.h"
#include <iostream>
#include <sstream>

namespace ai_language {

// สำหรับแสดงผลใน debug
std::string Token::toString() const {
    std::stringstream ss;
    ss << "Token(type=";
    
    switch (type) {
        case TokenType::START: ss << "START"; break;
        case TokenType::LOAD: ss << "LOAD"; break;
        case TokenType::CLEAN: ss << "CLEAN"; break;
        case TokenType::SPLIT: ss << "SPLIT"; break;
        case TokenType::TRAIN: ss << "TRAIN"; break;
        case TokenType::EVALUATE: ss << "EVALUATE"; break;
        case TokenType::PREDICT: ss << "PREDICT"; break;
        case TokenType::SAVE: ss << "SAVE"; break;
        case TokenType::SHOW: ss << "SHOW"; break;
        case TokenType::VISUALIZE: ss << "VISUALIZE"; break;
        case TokenType::PLOT: ss << "PLOT"; break;
        
        case TokenType::ML: ss << "ML"; break;
        case TokenType::DL: ss << "DL"; break;
        case TokenType::RL: ss << "RL"; break;
        
        case TokenType::DATASET: ss << "DATASET"; break;
        case TokenType::MODEL: ss << "MODEL"; break;
        case TokenType::METRIC: ss << "METRIC"; break;
        case TokenType::PATH: ss << "PATH"; break;
        case TokenType::TYPE: ss << "TYPE"; break;
        case TokenType::WITH: ss << "WITH"; break;
        case TokenType::EPOCHS: ss << "EPOCHS"; break;
        case TokenType::LAYERS: ss << "LAYERS"; break;
        
        case TokenType::EQUAL: ss << "EQUAL"; break;
        case TokenType::COMMA: ss << "COMMA"; break;
        case TokenType::COLON: ss << "COLON"; break;
        
        case TokenType::IDENTIFIER: ss << "IDENTIFIER"; break;
        case TokenType::STRING: ss << "STRING"; break;
        case TokenType::NUMBER: ss << "NUMBER"; break;
        
        case TokenType::COMMENT: ss << "COMMENT"; break;
        case TokenType::WHITESPACE: ss << "WHITESPACE"; break;
        case TokenType::NEWLINE: ss << "NEWLINE"; break;
        case TokenType::UNKNOWN: ss << "UNKNOWN"; break;
        case TokenType::END_OF_FILE: ss << "END_OF_FILE"; break;
    }
    
    ss << ", value=\"" << value << "\", line=" << line << ", column=" << column << ")";
    return ss.str();
}

Lexer::Lexer() 
    : m_start(0), m_current(0), m_line(1), m_column(1), m_hasError(false) {
    // กำหนดคำสำคัญที่จะถูกแปลงเป็น TokenType เฉพาะ
    m_keywords["start"] = TokenType::START;
    m_keywords["load"] = TokenType::LOAD;
    m_keywords["clean"] = TokenType::CLEAN;
    m_keywords["split"] = TokenType::SPLIT;
    m_keywords["train"] = TokenType::TRAIN;
    m_keywords["evaluate"] = TokenType::EVALUATE;
    m_keywords["predict"] = TokenType::PREDICT;
    m_keywords["save"] = TokenType::SAVE;
    m_keywords["show"] = TokenType::SHOW;
    m_keywords["visualize"] = TokenType::VISUALIZE;
    m_keywords["plot"] = TokenType::PLOT;
    
    m_keywords["ML"] = TokenType::ML;
    m_keywords["ml"] = TokenType::ML;
    m_keywords["DL"] = TokenType::DL;
    m_keywords["dl"] = TokenType::DL;
    m_keywords["RL"] = TokenType::RL;
    m_keywords["rl"] = TokenType::RL;
    
    m_keywords["dataset"] = TokenType::DATASET;
    m_keywords["model"] = TokenType::MODEL;
    m_keywords["metric"] = TokenType::METRIC;
    m_keywords["path"] = TokenType::PATH;
    m_keywords["type"] = TokenType::TYPE;
    m_keywords["with"] = TokenType::WITH;
    m_keywords["epochs"] = TokenType::EPOCHS;
    m_keywords["layers"] = TokenType::LAYERS;
    
    // ตั้งค่าฟังก์ชันสำหรับแสดงข้อผิดพลาดเริ่มต้น
    m_errorHandler = [](const std::string& message) {
        std::cerr << "Lexer Error: " << message << std::endl;
    };
}

void Lexer::setErrorHandler(std::function<void(const std::string&)> handler) {
    m_errorHandler = handler;
}

std::vector<Token> Lexer::tokenize(const std::string& source) {
    // รีเซ็ตสถานะ
    m_source = source;
    m_start = 0;
    m_current = 0;
    m_line = 1;
    m_column = 1;
    m_hasError = false;
    m_tokens.clear();
    
    // วนลูปแยก token จนกว่าจะถึงจุดสิ้นสุดของโค้ด
    while (!isAtEnd()) {
        m_start = m_current;
        scanToken();
    }
    
    // เพิ่ม token END_OF_FILE ปิดท้าย
    m_tokens.push_back(Token(TokenType::END_OF_FILE, "", m_line, m_column));
    
    return m_tokens;
}

bool Lexer::hasError() const {
    return m_hasError;
}

void Lexer::resetError() {
    m_hasError = false;
}

bool Lexer::isAtEnd() const {
    return m_current >= static_cast<int>(m_source.length());
}

char Lexer::advance() {
    m_current++;
    m_column++;
    return m_source[m_current - 1];
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return m_source[m_current];
}

char Lexer::peekNext() const {
    if (m_current + 1 >= static_cast<int>(m_source.length())) return '\0';
    return m_source[m_current + 1];
}

bool Lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (m_source[m_current] != expected) return false;
    
    m_current++;
    m_column++;
    return true;
}

void Lexer::addToken(TokenType type) {
    std::string text = m_source.substr(m_start, m_current - m_start);
    m_tokens.push_back(Token(type, text, m_line, m_column - (m_current - m_start)));
}

void Lexer::addToken(TokenType type, const std::string& value) {
    m_tokens.push_back(Token(type, value, m_line, m_column - (m_current - m_start)));
}

bool Lexer::isDigit(char c) const {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) const {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Lexer::isAlphaNumeric(char c) const {
    return isAlpha(c) || isDigit(c);
}

void Lexer::scanToken() {
    char c = advance();
    
    switch (c) {
        // ข้ามช่องว่างและตัวขึ้นบรรทัดใหม่
        case ' ':
        case '\r':
        case '\t':
            // ข้ามช่องว่างไป
            break;
            
        case '\n':
            // เพิ่มบรรทัดเมื่อเจอตัวขึ้นบรรทัดใหม่
            m_line++;
            m_column = 1;
            addToken(TokenType::NEWLINE);
            break;
            
        // ตัวดำเนินการและตัวแบ่ง
        case '=':
            addToken(TokenType::EQUAL);
            break;
        case ',':
            addToken(TokenType::COMMA);
            break;
        case ':':
            addToken(TokenType::COLON);
            break;
            
        // สตริง
        case '"':
            scanString();
            break;
            
        // คอมเมนต์
        case '#':
            scanComment();
            break;
            
        default:
            if (isDigit(c)) {
                scanNumber();
            } else if (isAlpha(c)) {
                scanIdentifier();
            } else {
                error("ไม่รู้จักตัวอักษร: " + std::string(1, c));
            }
            break;
    }
}

void Lexer::scanString() {
    // ไล่จนกว่าจะเจอเครื่องหมาย " ปิด หรือจบโค้ด
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            m_line++;
            m_column = 1;
        }
        advance();
    }
    
    // ถ้าจบโค้ดแล้วแต่ไม่เจอ " ปิด
    if (isAtEnd()) {
        error("สตริงไม่ได้ปิด");
        return;
    }
    
    // ข้าม " ปิด
    advance();
    
    // ดึงค่าสตริง ตัด " หัวท้ายออก
    std::string value = m_source.substr(m_start + 1, m_current - m_start - 2);
    addToken(TokenType::STRING, value);
}

void Lexer::scanNumber() {
    // ไล่จนกว่าจะไม่ใช่ตัวเลข
    while (isDigit(peek())) advance();
    
    // ตรวจสอบจุดทศนิยม
    if (peek() == '.' && isDigit(peekNext())) {
        // ข้ามจุด
        advance();
        
        // ไล่ตัวเลขหลังจุดทศนิยม
        while (isDigit(peek())) advance();
    }
    
    // ดึงค่าตัวเลข
    std::string number = m_source.substr(m_start, m_current - m_start);
    addToken(TokenType::NUMBER, number);
}

void Lexer::scanIdentifier() {
    // ไล่จนกว่าจะไม่ใช่ตัวอักษรหรือตัวเลข
    while (isAlphaNumeric(peek())) advance();
    
    // ดึงค่าตัวระบุ
    std::string identifier = m_source.substr(m_start, m_current - m_start);
    
    // ตรวจสอบว่าเป็นคำสำคัญหรือไม่
    if (m_keywords.find(identifier) != m_keywords.end()) {
        addToken(m_keywords[identifier], identifier);
    } else {
        addToken(TokenType::IDENTIFIER, identifier);
    }
}

void Lexer::scanComment() {
    // ไล่จนกว่าจะเจอตัวขึ้นบรรทัดใหม่หรือจบโค้ด
    while (peek() != '\n' && !isAtEnd()) {
        advance();
    }
    
    // ดึงค่าคอมเมนต์
    std::string comment = m_source.substr(m_start + 1, m_current - m_start - 1);
    addToken(TokenType::COMMENT, comment);
}

void Lexer::error(const std::string& message) {
    std::stringstream ss;
    ss << "บรรทัดที่ " << m_line << " ตำแหน่ง " << m_column << ": " << message;
    
    if (m_errorHandler) {
        m_errorHandler(ss.str());
    }
    
    m_hasError = true;
}

} // namespace ai_language
