#include "../include/lexer.h"
#include <iostream>
#include <map>

namespace ai_language {

// คำสั่งสำหรับ Keyword mapping
static const std::map<std::string, TokenType> keywords = {
    {"start", TokenType::START},
    {"load", TokenType::LOAD},
    {"clean", TokenType::CLEAN},
    {"split", TokenType::SPLIT},
    {"create", TokenType::CREATE},
    {"train", TokenType::TRAIN},
    {"evaluate", TokenType::EVALUATE},
    {"show", TokenType::SHOW},
    {"save", TokenType::SAVE},
    {"visualize", TokenType::VISUALIZE},
    {"plot", TokenType::PLOT},
    {"with", TokenType::WITH},
    {"for", TokenType::FOR},
    {"into", TokenType::INTO},
    {"and", TokenType::AND},
    {"to", TokenType::TO},
    {"on", TokenType::ON},
    {"ratio", TokenType::RATIO},
    {"add", TokenType::ADD},
    {"layer", TokenType::LAYER},
    {"compile", TokenType::COMPILE},
    {"predict", TokenType::PREDICT},
    {"test", TokenType::TEST},
    {"configure", TokenType::CONFIGURE},
    {"compare", TokenType::COMPARE}
};

Lexer::Lexer(const std::string& input) : input(input), pos(0) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    
    while (pos < input.size()) {
        char c = input[pos];
        
        // ข้าม whitespace
        if (std::isspace(c)) {
            pos++;
            continue;
        }
        
        // ตรวจสอบ string literals
        if (c == '"' || c == '\'') {
            tokens.push_back(parseString());
            continue;
        }
        
        // ตรวจสอบตัวเลข
        if (std::isdigit(c) || (c == '.' && pos + 1 < input.size() && std::isdigit(input[pos + 1]))) {
            tokens.push_back(parseNumber());
            continue;
        }
        
        // ตรวจสอบคำที่เป็น identifier หรือ keyword
        if (std::isalpha(c) || c == '_') {
            tokens.push_back(parseWord());
            continue;
        }
        
        // ไม่รู้จักตัวอักษร
        tokens.push_back(Token(TokenType::UNKNOWN, std::string(1, c)));
        pos++;
    }
    
    tokens.push_back(Token(TokenType::END));
    return tokens;
}

Token Lexer::parseString() {
    char quote = input[pos++]; // เก็บเครื่องหมายคำพูดเริ่มต้น (' หรือ ")
    std::string value;
    
    while (pos < input.size() && input[pos] != quote) {
        // จัดการกรณี escape sequence
        if (input[pos] == '\\' && pos + 1 < input.size()) {
            pos++; // ข้าม backslash
            
            // จัดการ escape sequence ต่างๆ
            switch (input[pos]) {
                case 'n': value += '\n'; break;
                case 't': value += '\t'; break;
                case 'r': value += '\r'; break;
                case '\\': value += '\\'; break;
                case '\'': value += '\''; break;
                case '"': value += '"'; break;
                default: value += input[pos]; break;
            }
        } else {
            value += input[pos];
        }
        pos++;
    }
    
    if (pos < input.size()) {
        pos++; // ข้ามเครื่องหมายคำพูดปิด
    }
    
    return Token(TokenType::STRING, value);
}

Token Lexer::parseNumber() {
    size_t start = pos;
    bool hasDecimal = false;
    
    // จัดการกับเลขจำนวนเต็มหรือทศนิยม
    while (pos < input.size() &&
           (std::isdigit(input[pos]) ||
            (input[pos] == '.' && !hasDecimal && pos + 1 < input.size() && std::isdigit(input[pos + 1])))) {
        if (input[pos] == '.') {
            hasDecimal = true;
        }
        pos++;
    }
    
    return Token(TokenType::NUMBER, input.substr(start, pos - start));
}

Token Lexer::parseWord() {
    size_t start = pos;
    
    while (pos < input.size() && (std::isalnum(input[pos]) || input[pos] == '_')) {
        pos++;
    }
    
    std::string word = input.substr(start, pos - start);
    TokenType type;
    
    if (isKeyword(word, type)) {
        return Token(type, word);
    }
    
    return Token(TokenType::IDENTIFIER, word);
}

bool Lexer::isKeyword(const std::string& word, TokenType& type) {
    auto it = keywords.find(word);
    if (it != keywords.end()) {
        type = it->second;
        return true;
    }
    return false;
}

} // namespace ai_language