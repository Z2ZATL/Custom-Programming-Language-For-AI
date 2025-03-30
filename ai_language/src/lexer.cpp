/**
 * @file lexer.cpp
 * @brief ตัวแยกคำสำหรับภาษา AI
 */

#include "../include/lexer.h"
#include <cctype>

namespace ai_language {

// คำสำคัญของภาษา
std::unordered_map<std::string, TokenType> Lexer::_keywords = {
    {"start", TokenType::START},
    {"load", TokenType::LOAD},
    {"clean", TokenType::CLEAN},
    {"split", TokenType::SPLIT},
    {"train", TokenType::TRAIN},
    {"evaluate", TokenType::EVALUATE},
    {"show", TokenType::SHOW},
    {"save", TokenType::SAVE},
    {"create", TokenType::CREATE},
    {"with", TokenType::WITH},
    {"into", TokenType::INTO},
    {"ratio", TokenType::RATIO},
    {"path", TokenType::PATH},
    {"metric", TokenType::METRIC},
    {"epochs", TokenType::EPOCHS},
    {"type", TokenType::TYPE},
    {"model", TokenType::MODEL},
    {"data", TokenType::DATA},
    {"visualize", TokenType::VISUALIZE},
    {"plot", TokenType::PLOT}
};

Lexer::Lexer(const std::string& source)
    : _source(source) {
}

std::vector<Token> Lexer::tokenize() {
    _tokens.clear();
    _start = 0;
    _current = 0;
    _line = 1;
    _column = 1;
    _hasError = false;
    _errorMsg = "";

    while (!isAtEnd()) {
        // จุดเริ่มต้นของ token ถัดไป
        _start = _current;
        scanToken();
    }

    // เพิ่ม token END ที่จุดสิ้นสุดข้อความ
    _tokens.push_back(Token(TokenType::END, "", _line, _column));
    return _tokens;
}

bool Lexer::isValidChar(char c) const {
    // ตรวจสอบว่าเป็นตัวอักษรที่รองรับหรือไม่
    // รองรับ ASCII และตัวอักษรพิเศษที่ใช้ในภาษาไทย
    return (c >= 32 && c <= 126) || (static_cast<unsigned char>(c) >= 128);
}

void Lexer::scanToken() {
    char c = advance();

    // ตรวจสอบตัวอักษรที่ไม่รองรับ
    if (!isValidChar(c) && c != '\n' && c != '\t' && c != '\r' && c != ' ') {
        std::string errorMsg = "พบตัวอักษรที่ไม่รู้จัก: '" + std::string(1, c) + "'";
        _hasError = true;
        _errorMsg = errorMsg;
        return;
    }

    switch (c) {
        // ข้ามช่องว่าง (whitespace)
        case ' ':
        case '\t':
        case '\r':
            // ไม่ทำอะไร (ข้ามไป)
            break;

        case '\n':
            _line++;
            _column = 1;
            break;

        // ตัวดำเนินการ
        case '=': addToken(TokenType::EQUAL); break;
        case ',': addToken(TokenType::COMMA); break;
        case '.': addToken(TokenType::DOT); break;
        case '+': addToken(TokenType::PLUS); break;
        case '-': addToken(TokenType::MINUS); break;
        case '*': addToken(TokenType::MULTIPLY); break;
        case '/':
            // เช็คว่าเป็นความคิดเห็นหรือไม่
            if (match('/')) {
                scanComment();
            } else {
                addToken(TokenType::DIVIDE);
            }
            break;
        case '%': addToken(TokenType::MODULO); break;

        // ข้อความ (string)
        case '"': scanString(); break;
        case '\'': scanString(); break;

        default:
            // ตัวเลข (number)
            if (isDigit(c)) {
                scanNumber();
            }
            // ตัวแปรหรือคำสำคัญ (identifier, keyword)
            else if (isAlpha(c)) {
                scanIdentifier();
            }
            // ตัวอักษรที่ไม่รู้จัก
            else {
                error("พบตัวอักษรที่ไม่รู้จัก: '" + std::string(1, c) + "'");
                addToken(TokenType::ERROR, std::string(1, c));
            }
            break;
    }
}

void Lexer::scanString() {
    char delimiter = _source[_current - 1]; // เก็บตัวอักษรเริ่มต้นของข้อความ (ได้จาก advance ก่อนหน้านี้)
    std::string value;

    while (peek() != delimiter && !isAtEnd()) {
        if (peek() == '\n') {
            _line++;
            _column = 1;
        }
        value += advance();
    }

    if (isAtEnd()) {
        error("ข้อความไม่สมบูรณ์");
        addToken(TokenType::ERROR, value);
        return;
    }

    // ข้ามตัวอักษรปิดข้อความ
    advance();

    // เพิ่ม token สำหรับข้อความ (ไม่รวมเครื่องหมายคำพูด)
    addToken(TokenType::STRING, value);
}

void Lexer::scanNumber() {
    while (isDigit(peek())) {
        advance();
    }

    // จัดการกับทศนิยม
    if (peek() == '.' && isDigit(peekNext())) {
        // ข้ามจุดทศนิยม
        advance();

        while (isDigit(peek())) {
            advance();
        }
    }

    // เพิ่ม token สำหรับตัวเลข
    std::string value = _source.substr(_start, _current - _start);
    addToken(TokenType::NUMBER, value);
}

void Lexer::scanIdentifier() {
    while (isAlphaNumeric(peek())) {
        advance();
    }

    // ดึงข้อความที่เป็นตัวแปรหรือคำสำคัญ
    std::string text = _source.substr(_start, _current - _start);

    // ตรวจสอบว่าเป็นคำสำคัญหรือไม่
    auto it = _keywords.find(text);
    TokenType type = (it != _keywords.end()) ? it->second : TokenType::IDENTIFIER;

    // เพิ่ม token
    addToken(type, text);
}

void Lexer::scanComment() {
    // ข้ามทุกตัวอักษรจนถึงจุดสิ้นสุดบรรทัด
    while (peek() != '\n' && !isAtEnd()) {
        advance();
    }

    // เพิ่ม token สำหรับความคิดเห็น (ถ้าต้องการเก็บไว้)
    std::string comment = _source.substr(_start + 2, _current - _start - 2);
    addToken(TokenType::COMMENT, comment);
}

char Lexer::advance() {
    _column++;
    return _source[_current++];
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return _source[_current];
}

char Lexer::peekNext() const {
    if (_current + 1 >= _source.length()) return '\0';
    return _source[_current + 1];
}

bool Lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (_source[_current] != expected) return false;

    _current++;
    _column++;
    return true;
}

void Lexer::addToken(TokenType type, const std::string& value) {
    _tokens.push_back(Token(type, value, _line, _column - (_current - _start)));
}

bool Lexer::isAtEnd() const {
    return _current >= _source.length();
}

void Lexer::error(const std::string& message) {
    _hasError = true;
    _errorMsg = "บรรทัด " + std::to_string(_line) + " ตำแหน่ง " + std::to_string(_column) + ": " + message;
}

bool Lexer::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

bool Lexer::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

} // namespace ai_language
#include "../include/lexer.h"

namespace ai_language {

std::unordered_map<std::string, TokenType> Lexer::_keywords = {
    {"start", TokenType::START},
    {"load", TokenType::LOAD},
    {"clean", TokenType::CLEAN},
    {"split", TokenType::SPLIT},
    {"train", TokenType::TRAIN},
    {"evaluate", TokenType::EVALUATE},
    {"show", TokenType::SHOW},
    {"save", TokenType::SAVE},
    {"create", TokenType::CREATE},
    {"with", TokenType::WITH},
    {"into", TokenType::INTO},
    {"ratio", TokenType::RATIO},
    {"path", TokenType::PATH},
    {"metric", TokenType::METRIC},
    {"epochs", TokenType::EPOCHS},
    {"type", TokenType::TYPE},
    {"model", TokenType::MODEL},
    {"data", TokenType::DATA},
    {"visualize", TokenType::VISUALIZE},
    {"plot", TokenType::PLOT}
};

Lexer::Lexer(const std::string& source) : _source(source) {}

std::vector<Token> Lexer::tokenize() {
    while (!isAtEnd()) {
        _start = _current;
        scanToken();
    }
    
    _tokens.push_back(Token(TokenType::END, "", _line, _column));
    return _tokens;
}

char Lexer::advance() {
    _current++;
    _column++;
    return _source[_current - 1];
}

char Lexer::peek() const {
    if (isAtEnd()) return '\0';
    return _source[_current];
}

char Lexer::peekNext() const {
    if (_current + 1 >= _source.length()) return '\0';
    return _source[_current + 1];
}

bool Lexer::isAtEnd() const {
    return _current >= _source.length();
}

void Lexer::addToken(TokenType type, const std::string& value) {
    _tokens.push_back(Token(type, value.empty() ? _source.substr(_start, _current - _start) : value, _line, _column - (_current - _start)));
}

bool Lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (_source[_current] != expected) return false;
    
    _current++;
    _column++;
    return true;
}

void Lexer::scanToken() {
    char c = advance();
    
    switch (c) {
        case ' ':
        case '\r':
        case '\t':
            // ไม่ทำอะไร, ข้ามช่องว่าง
            break;
        case '\n':
            _line++;
            _column = 1;
            break;
        case '=':
            addToken(TokenType::EQUAL);
            break;
        case ',':
            addToken(TokenType::COMMA);
            break;
        case '.':
            addToken(TokenType::DOT);
            break;
        case '+':
            addToken(TokenType::PLUS);
            break;
        case '-':
            addToken(TokenType::MINUS);
            break;
        case '*':
            addToken(TokenType::MULTIPLY);
            break;
        case '/':
            if (match('/')) {
                scanComment();
            } else {
                addToken(TokenType::DIVIDE);
            }
            break;
        case '%':
            addToken(TokenType::MODULO);
            break;
        case '"':
            scanString();
            break;
        case '#':
            scanComment();
            break;
        default:
            if (isDigit(c)) {
                scanNumber();
            } else if (isAlpha(c)) {
                scanIdentifier();
            } else if (!isValidChar(c)) {
                error("อักขระไม่ถูกต้อง: '" + std::string(1, c) + "'");
            }
            break;
    }
}

void Lexer::scanString() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            _line++;
            _column = 1;
        }
        advance();
    }
    
    if (isAtEnd()) {
        error("ข้อความไม่ปิด");
        return;
    }
    
    // ข้ามเครื่องหมาย " ปิด
    advance();
    
    // ตัดเครื่องหมาย " ออกจากข้อความ
    std::string value = _source.substr(_start + 1, _current - _start - 2);
    addToken(TokenType::STRING, value);
}

void Lexer::scanNumber() {
    while (isDigit(peek())) advance();
    
    // ส่วนทศนิยม
    if (peek() == '.' && isDigit(peekNext())) {
        // ข้ามจุด
        advance();
        
        while (isDigit(peek())) advance();
    }
    
    addToken(TokenType::NUMBER);
}

void Lexer::scanIdentifier() {
    while (isAlphaNumeric(peek())) advance();
    
    std::string text = _source.substr(_start, _current - _start);
    TokenType type;
    
    auto it = _keywords.find(text);
    if (it != _keywords.end()) {
        type = it->second;
    } else {
        type = TokenType::IDENTIFIER;
    }
    
    addToken(type);
}

void Lexer::scanComment() {
    // ข้ามจนถึงสิ้นบรรทัด
    while (peek() != '\n' && !isAtEnd()) {
        advance();
    }
    
    addToken(TokenType::COMMENT);
}

void Lexer::error(const std::string& message) {
    _hasError = true;
    _errorMsg = "บรรทัด " + std::to_string(_line) + " ตำแหน่ง " + std::to_string(_column) + ": " + message;
}

bool Lexer::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

bool Lexer::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

bool Lexer::isValidChar(char c) const {
    return isAlphaNumeric(c) || c == ' ' || c == '\t' || c == '\r' || c == '\n' ||
           c == '"' || c == '=' || c == ',' || c == '.' || c == '+' || c == '-' ||
           c == '*' || c == '/' || c == '%' || c == '#';
}

} // namespace ai_language
