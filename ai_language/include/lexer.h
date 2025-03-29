#pragma once

#include <string>
#include <vector>
#include <cctype>
#include <algorithm>

namespace ai_language {

// ประกาศ Token types
enum class TokenType {
    START,          // start create ML/DL/RL
    LOAD,           // load dataset
    CLEAN,          // clean data
    SPLIT,          // split data
    CREATE,         // create model
    TRAIN,          // train model
    EVALUATE,       // evaluate model
    SHOW,           // show metrics
    SAVE,           // save model
    VISUALIZE,      // visualize data
    PLOT,           // plot learning curve
    IDENTIFIER,     // ชื่อตัวแปร, พารามิเตอร์
    STRING,         // ข้อความในเครื่องหมายคำพูด
    NUMBER,         // ตัวเลข
    WITH,           // with (สำหรับระบุพารามิเตอร์)
    FOR,            // for (เช่น train for epochs)
    INTO,           // into (split into)
    AND,            // and (train and test)
    TO,             // to (save to)
    ON,             // on (evaluate on)
    RATIO,          // ratio (สำหรับการ split)
    ADD,            // add (เช่น add layer)
    LAYER,          // layer (สำหรับ neural network)
    COMPILE,        // compile (model)
    PREDICT,        // predict (using model)
    TEST,           // test (agent)
    CONFIGURE,      // configure (agent)
    COMPARE,        // compare (agents)
    UNKNOWN,        // คำสั่งที่ไม่รู้จัก
    END             // จบคำสั่ง
};

// Token
struct Token {
    TokenType type;
    std::string value;

    Token(TokenType type, const std::string& value = "")
        : type(type), value(value) {}
};

// Lexer
class Lexer {
public:
    Lexer(const std::string& input);
    std::vector<Token> tokenize();

private:
    std::string input;
    size_t pos;
    
    Token parseString();
    Token parseNumber();
    Token parseWord();
    bool isKeyword(const std::string& word, TokenType& type);
};

} // namespace ai_language