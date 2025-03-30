
/**
 * @file lexer.h
 * @brief ตัวแยกคำสำหรับภาษา AI
 */

#ifndef AI_LANGUAGE_LEXER_H
#define AI_LANGUAGE_LEXER_H

#include <string>
#include <vector>
#include <map>
#include <unordered_map>

namespace ai_language {

/**
 * @brief ประเภทของ token
 */
enum class TokenType {
    // คำสำคัญ
    START,      ///< start
    LOAD,       ///< load
    CLEAN,      ///< clean
    SPLIT,      ///< split
    TRAIN,      ///< train
    EVALUATE,   ///< evaluate
    SHOW,       ///< show
    SAVE,       ///< save
    CREATE,     ///< create
    WITH,       ///< with
    INTO,       ///< into
    RATIO,      ///< ratio
    PATH,       ///< path
    METRIC,     ///< metric
    EPOCHS,     ///< epochs
    TYPE,       ///< type
    MODEL,      ///< model
    DATA,       ///< data
    VISUALIZE,  ///< visualize
    PLOT,       ///< plot
    
    // ตัวดำเนินการ
    EQUAL,      ///< =
    COMMA,      ///< ,
    DOT,        ///< .
    PLUS,       ///< +
    MINUS,      ///< -
    MULTIPLY,   ///< *
    DIVIDE,     ///< /
    MODULO,     ///< %
    
    // ค่าและตัวแปร
    IDENTIFIER, ///< ชื่อตัวแปร
    STRING,     ///< ข้อความ
    NUMBER,     ///< ตัวเลข
    
    // อื่นๆ
    COMMENT,    ///< ความคิดเห็น
    END,        ///< สิ้นสุดข้อความ
    ERROR       ///< ข้อผิดพลาด
};

/**
 * @brief โครงสร้างของ token
 */
struct Token {
    TokenType type;       ///< ประเภทของ token
    std::string value;    ///< ค่าของ token
    int line;             ///< บรรทัดที่พบ token
    int column;           ///< ตำแหน่งในบรรทัดที่พบ token
    
    Token(TokenType type, const std::string& value, int line, int column) 
        : type(type), value(value), line(line), column(column) {}
};

/**
 * @brief คลาสสำหรับแยกข้อความเป็น token
 */
class Lexer {
public:
    /**
     * @brief สร้าง Lexer จากข้อความ
     * @param source ข้อความที่ต้องการแยก
     */
    explicit Lexer(const std::string& source);
    
    /**
     * @brief แยกข้อความเป็น token
     * @return รายการของ token
     */
    std::vector<Token> tokenize();
    
    /**
     * @brief ตรวจสอบว่ามีข้อผิดพลาดหรือไม่
     * @return true ถ้ามีข้อผิดพลาด
     */
    bool hasError() const { return _hasError; }
    
    /**
     * @brief ส่งคืนข้อความผิดพลาดล่าสุด
     * @return ข้อความผิดพลาด
     */
    std::string getError() const { return _errorMsg; }
    
private:
    std::string _source;             ///< ข้อความที่ต้องการแยก
    int _start = 0;                  ///< ตำแหน่งเริ่มต้นของ token ปัจจุบัน
    int _current = 0;                ///< ตำแหน่งปัจจุบันที่กำลังอ่าน
    int _line = 1;                   ///< บรรทัดปัจจุบัน
    int _column = 1;                 ///< ตำแหน่งในบรรทัดปัจจุบัน
    bool _hasError = false;          ///< สถานะข้อผิดพลาด
    std::string _errorMsg;           ///< ข้อความผิดพลาด
    
    std::vector<Token> _tokens;      ///< รายการของ token ที่แยกได้
    static std::unordered_map<std::string, TokenType> _keywords; ///< คำสำคัญของภาษา
    
    /**
     * @brief อ่านตัวอักษรถัดไป
     * @return ตัวอักษรถัดไป
     */
    char advance();
    
    /**
     * @brief ดูตัวอักษรถัดไปโดยไม่เลื่อนตำแหน่ง
     * @return ตัวอักษรถัดไป
     */
    char peek() const;
    
    /**
     * @brief ดูตัวอักษรถัดไป 2 ตำแหน่งโดยไม่เลื่อนตำแหน่ง
     * @return ตัวอักษรถัดไป 2 ตำแหน่ง
     */
    char peekNext() const;
    
    /**
     * @brief ตรวจสอบว่าถึงจุดสิ้นสุดข้อความหรือไม่
     * @return true ถ้าถึงจุดสิ้นสุดข้อความ
     */
    bool isAtEnd() const;
    
    /**
     * @brief สร้าง token
     * @param type ประเภทของ token
     * @param value ค่าของ token (ถ้ามี)
     */
    void addToken(TokenType type, const std::string& value = "");
    
    /**
     * @brief ตรวจสอบว่าตัวอักษรตรงกับที่คาดหวังหรือไม่
     * @param expected ตัวอักษรที่คาดหวัง
     * @return true ถ้าตรงกัน
     */
    bool match(char expected);
    
    /**
     * @brief แยก token ถัดไป
     */
    void scanToken();
    
    /**
     * @brief แยกข้อความ (string)
     */
    void scanString();
    
    /**
     * @brief แยกตัวเลข (number)
     */
    void scanNumber();
    
    /**
     * @brief แยกตัวแปรหรือคำสำคัญ
     */
    void scanIdentifier();
    
    /**
     * @brief แยกความคิดเห็น (comment)
     */
    void scanComment();
    
    /**
     * @brief บันทึกข้อผิดพลาด
     * @param message ข้อความผิดพลาด
     */
    void error(const std::string& message);
    
    /**
     * @brief ตรวจสอบว่าตัวอักษรเป็นตัวเลขหรือไม่
     * @param c ตัวอักษรที่ต้องการตรวจสอบ
     * @return true ถ้าเป็นตัวเลข
     */
    static bool isDigit(char c);
    
    /**
     * @brief ตรวจสอบว่าตัวอักษรเป็นตัวอักษรหรือไม่
     * @param c ตัวอักษรที่ต้องการตรวจสอบ
     * @return true ถ้าเป็นตัวอักษร
     */
    static bool isAlpha(char c);
    
    /**
     * @brief ตรวจสอบว่าตัวอักษรเป็นตัวอักษรหรือตัวเลขหรือไม่
     * @param c ตัวอักษรที่ต้องการตรวจสอบ
     * @return true ถ้าเป็นตัวอักษรหรือตัวเลข
     */
    static bool isAlphaNumeric(char c);
};

} // namespace ai_language

#endif // AI_LANGUAGE_LEXER_H
