
/**
 * @file lexer.h
 * @brief คลาสสำหรับแปลงโค้ดเป็น token
 */

#ifndef AI_LANGUAGE_LEXER_H
#define AI_LANGUAGE_LEXER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

namespace ai_language {

/**
 * @enum TokenType
 * @brief ประเภทของ token ต่างๆ
 */
enum class TokenType {
    // คำสั่งพื้นฐาน
    START,
    LOAD,
    CLEAN,
    SPLIT,
    TRAIN,
    EVALUATE,
    PREDICT,
    SAVE,
    SHOW,
    VISUALIZE,
    PLOT,
    
    // ประเภทการเรียนรู้
    ML,
    DL,
    RL,
    
    // คำสำคัญอื่นๆ
    DATASET,
    MODEL,
    METRIC,
    PATH,
    TYPE,
    WITH,
    EPOCHS,
    LAYERS,
    
    // ตัวดำเนินการและตัวแบ่ง
    EQUAL,
    COMMA,
    COLON,
    
    // ข้อมูล
    IDENTIFIER,
    STRING,
    NUMBER,
    
    // อื่นๆ
    COMMENT,
    WHITESPACE,
    NEWLINE,
    UNKNOWN,
    END_OF_FILE
};

/**
 * @struct Token
 * @brief โครงสร้างข้อมูลที่เก็บ token
 */
struct Token {
    TokenType type;  ///< ประเภทของ token
    std::string value;  ///< ค่าของ token
    int line;  ///< บรรทัดที่พบ token
    int column;  ///< ตำแหน่งในบรรทัดที่พบ token
    
    Token(TokenType type, const std::string& value, int line, int column)
        : type(type), value(value), line(line), column(column) {}
        
    // สำหรับแสดงผลใน debug
    std::string toString() const;
};

/**
 * @class Lexer
 * @brief คลาสสำหรับแปลงโค้ดเป็น token
 */
class Lexer {
public:
    /**
     * @brief คอนสตรักเตอร์
     */
    Lexer();
    
    /**
     * @brief ตั้งค่าฟังก์ชันสำหรับแสดงข้อผิดพลาด
     * @param handler ฟังก์ชันสำหรับแสดงข้อผิดพลาด
     */
    void setErrorHandler(std::function<void(const std::string&)> handler);
    
    /**
     * @brief แปลงโค้ดเป็น token
     * @param source โค้ดที่ต้องการแปลง
     * @return std::vector<Token> รายการของ token
     */
    std::vector<Token> tokenize(const std::string& source);
    
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
    std::string m_source;  ///< โค้ดที่กำลังแปลง
    int m_start;  ///< ตำแหน่งเริ่มต้นของ token ปัจจุบัน
    int m_current;  ///< ตำแหน่งปัจจุบัน
    int m_line;  ///< บรรทัดปัจจุบัน
    int m_column;  ///< ตำแหน่งในบรรทัดปัจจุบัน
    bool m_hasError;  ///< ตัวแปรบอกว่ามีข้อผิดพลาดหรือไม่
    std::vector<Token> m_tokens;  ///< รายการของ token ที่แยกได้
    std::function<void(const std::string&)> m_errorHandler;  ///< ฟังก์ชันสำหรับแสดงข้อผิดพลาด
    
    // แม็ปสำหรับแปลงคำสำคัญเป็น TokenType
    std::unordered_map<std::string, TokenType> m_keywords;
    
    // เมธอดช่วยเหลือ
    bool isAtEnd() const;
    char advance();
    char peek() const;
    char peekNext() const;
    bool match(char expected);
    void addToken(TokenType type);
    void addToken(TokenType type, const std::string& value);
    bool isDigit(char c) const;
    bool isAlpha(char c) const;
    bool isAlphaNumeric(char c) const;
    void scanToken();
    void scanString();
    void scanNumber();
    void scanIdentifier();
    void scanComment();
    void error(const std::string& message);
};

} // namespace ai_language

#endif // AI_LANGUAGE_LEXER_H
