#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <memory>
#include <regex>
#include <cctype>
#include <ctime>
#include <random>

// ประกาศคลาสหรือโครงสร้างหลัก
class Token {
public:
    enum Type {
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
        UNKNOWN,        // คำสั่งที่ไม่รู้จัก
        END             // จบคำสั่ง
    };

    Token(Type type, const std::string& value = "") : type(type), value(value) {}

    Type type;
    std::string value;
};

class Lexer {
public:
    Lexer(const std::string& input) : input(input), pos(0) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        
        while (pos < input.size()) {
            char current = input[pos];
            
            // ข้ามช่องว่าง
            if (std::isspace(current)) {
                pos++;
                continue;
            }
            
            // ข้อความในเครื่องหมายคำพูด
            if (current == '"') {
                tokens.push_back(parseString());
                continue;
            }
            
            // ตัวเลข
            if (std::isdigit(current)) {
                tokens.push_back(parseNumber());
                continue;
            }
            
            // คำ (keyword หรือ identifier)
            if (std::isalpha(current) || current == '_') {
                Token token = parseWord();
                tokens.push_back(token);
                continue;
            }
            
            // หากไม่ตรงกับกรณีใดเลย
            pos++;
        }
        
        tokens.push_back(Token(Token::END));
        return tokens;
    }

private:
    std::string input;
    size_t pos;
    
    Token parseString() {
        pos++; // ข้ามเครื่องหมาย " แรก
        std::string value;
        
        while (pos < input.size() && input[pos] != '"') {
            value += input[pos];
            pos++;
        }
        
        if (pos < input.size()) pos++; // ข้ามเครื่องหมาย " ปิด
        return Token(Token::STRING, value);
    }
    
    Token parseNumber() {
        std::string value;
        
        while (pos < input.size() && (std::isdigit(input[pos]) || input[pos] == '.')) {
            value += input[pos];
            pos++;
        }
        
        return Token(Token::NUMBER, value);
    }
    
    Token parseWord() {
        std::string value;
        
        while (pos < input.size() && (std::isalnum(input[pos]) || input[pos] == '_')) {
            value += input[pos];
            pos++;
        }
        
        // ตรวจสอบว่าเป็น keyword หรือไม่
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);
        
        if (value == "start") return Token(Token::START, value);
        if (value == "load") return Token(Token::LOAD, value);
        if (value == "clean") return Token(Token::CLEAN, value);
        if (value == "split") return Token(Token::SPLIT, value);
        if (value == "create") return Token(Token::CREATE, value);
        if (value == "train") return Token(Token::TRAIN, value);
        if (value == "evaluate") return Token(Token::EVALUATE, value);
        if (value == "show") return Token(Token::SHOW, value);
        if (value == "save") return Token(Token::SAVE, value);
        if (value == "visualize") return Token(Token::VISUALIZE, value);
        if (value == "plot") return Token(Token::PLOT, value);
        if (value == "with") return Token(Token::WITH, value);
        if (value == "for") return Token(Token::FOR, value);
        if (value == "into") return Token(Token::INTO, value);
        if (value == "and") return Token(Token::AND, value);
        if (value == "to") return Token(Token::TO, value);
        if (value == "on") return Token(Token::ON, value);
        if (value == "ratio") return Token(Token::RATIO, value);
        
        return Token(Token::IDENTIFIER, value);
    }
};

class Parser {
public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}
    
    void parse() {
        while (!isAtEnd()) {
            parseStatement();
        }
    }
    
private:
    std::vector<Token> tokens;
    size_t current;
    
    bool isAtEnd() {
        return tokens[current].type == Token::END;
    }
    
    Token advance() {
        if (!isAtEnd()) current++;
        return tokens[current - 1];
    }
    
    Token peek() {
        return tokens[current];
    }
    
    Token previous() {
        return tokens[current - 1];
    }
    
    bool match(Token::Type type) {
        if (isAtEnd()) return false;
        if (tokens[current].type != type) return false;
        
        advance();
        return true;
    }
    
    void parseStatement() {
        if (match(Token::START)) {
            parseStartStatement();
        } else if (match(Token::LOAD)) {
            parseLoadStatement();
        } else if (match(Token::CLEAN)) {
            parseCleanStatement();
        } else if (match(Token::SPLIT)) {
            parseSplitStatement();
        } else if (match(Token::CREATE)) {
            parseCreateStatement();
        } else if (match(Token::TRAIN)) {
            parseTrainStatement();
        } else if (match(Token::EVALUATE)) {
            parseEvaluateStatement();
        } else if (match(Token::SHOW)) {
            parseShowStatement();
        } else if (match(Token::SAVE)) {
            parseSaveStatement();
        } else if (match(Token::VISUALIZE)) {
            parseVisualizeStatement();
        } else if (match(Token::PLOT)) {
            parsePlotStatement();
        } else {
            // ข้ามคำสั่งที่ไม่รู้จัก
            advance();
        }
    }
    
    void parseStartStatement() {
        if (match(Token::CREATE)) {
            if (match(Token::IDENTIFIER)) {
                std::string learningType = previous().value;
                std::cout << "เริ่มต้นการสร้างโปรเจกต์ประเภท: " << learningType << std::endl;
                
                if (learningType == "ml" || learningType == "machine_learning") {
                    std::cout << "เริ่มต้นโปรเจกต์ Machine Learning" << std::endl;
                } else if (learningType == "dl" || learningType == "deep_learning") {
                    std::cout << "เริ่มต้นโปรเจกต์ Deep Learning" << std::endl;
                } else if (learningType == "rl" || learningType == "reinforcement_learning") {
                    std::cout << "เริ่มต้นโปรเจกต์ Reinforcement Learning" << std::endl;
                } else {
                    std::cout << "ไม่รู้จักประเภทการเรียนรู้: " << learningType << std::endl;
                }
            }
        }
    }
    
    void parseLoadStatement() {
        if (match(Token::IDENTIFIER)) {
            std::string dataType = previous().value;
            
            if (dataType == "dataset") {
                if (match(Token::STRING)) {
                    std::string filename = previous().value;
                    std::cout << "กำลังโหลดข้อมูลจากไฟล์: " << filename << std::endl;
                    // โค้ดจริงสำหรับการโหลดข้อมูล
                }
            }
        }
    }
    
    void parseCleanStatement() {
        if (match(Token::IDENTIFIER)) {
            std::string dataType = previous().value;
            std::cout << "กำลังทำความสะอาดข้อมูล: " << dataType << std::endl;
            // โค้ดจริงสำหรับการทำความสะอาดข้อมูล
        }
    }
    
    void parseSplitStatement() {
        if (match(Token::IDENTIFIER)) {
            std::string dataType = previous().value;
            
            if (match(Token::INTO)) {
                if (match(Token::IDENTIFIER) && previous().value == "train") {
                    if (match(Token::AND)) {
                        if (match(Token::IDENTIFIER) && previous().value == "test") {
                            if (match(Token::WITH)) {
                                if (match(Token::IDENTIFIER) && previous().value == "ratio") {
                                    if (match(Token::NUMBER)) {
                                        double ratio = std::stod(previous().value);
                                        std::cout << "กำลังแบ่งข้อมูล " << dataType << " เป็นชุดฝึกและชุดทดสอบด้วยอัตราส่วน: " << ratio << std::endl;
                                        // โค้ดจริงสำหรับการแบ่งข้อมูล
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    void parseCreateStatement() {
        if (match(Token::IDENTIFIER)) {
            std::string modelType = previous().value;
            
            if (modelType == "model") {
                if (match(Token::IDENTIFIER)) {
                    std::string algorithm = previous().value;
                    std::cout << "กำลังสร้างโมเดล: " << algorithm << std::endl;
                    
                    if (match(Token::WITH)) {
                        parseParameters();
                    }
                }
            } else if (modelType == "neural_network") {
                std::cout << "กำลังสร้างโครงข่ายประสาทเทียม" << std::endl;
                
                if (match(Token::WITH)) {
                    parseParameters();
                }
            } else if (modelType == "agent") {
                std::cout << "กำลังสร้าง agent สำหรับ Reinforcement Learning" << std::endl;
                
                if (match(Token::WITH)) {
                    parseParameters();
                }
            }
        }
    }
    
    void parseTrainStatement() {
        if (match(Token::IDENTIFIER)) {
            std::string target = previous().value;
            
            if (target == "model") {
                std::cout << "กำลังฝึกโมเดล" << std::endl;
                
                if (match(Token::WITH)) {
                    parseParameters();
                } else if (match(Token::FOR)) {
                    if (match(Token::IDENTIFIER)) {
                        std::string param = previous().value;
                        
                        if (param == "epochs") {
                            if (match(Token::NUMBER)) {
                                int epochs = std::stoi(previous().value);
                                std::cout << "จำนวน epochs: " << epochs << std::endl;
                            }
                        }
                    }
                }
            } else if (target == "agent") {
                std::cout << "กำลังฝึก agent" << std::endl;
                
                if (match(Token::FOR)) {
                    if (match(Token::IDENTIFIER)) {
                        std::string param = previous().value;
                        
                        if (param == "episodes") {
                            if (match(Token::NUMBER)) {
                                int episodes = std::stoi(previous().value);
                                std::cout << "จำนวน episodes: " << episodes << std::endl;
                            }
                        }
                    }
                }
            }
        }
    }
    
    void parseEvaluateStatement() {
        if (match(Token::IDENTIFIER)) {
            std::string target = previous().value;
            
            if (target == "model") {
                if (match(Token::ON)) {
                    if (match(Token::IDENTIFIER)) {
                        std::string dataType = previous().value;
                        std::cout << "กำลังประเมินโมเดลกับข้อมูล: " << dataType << std::endl;
                    }
                }
            }
        }
    }
    
    void parseShowStatement() {
        if (match(Token::IDENTIFIER)) {
            std::string metric = previous().value;
            std::cout << "กำลังแสดงเมตริก: " << metric << std::endl;
        }
    }
    
    void parseSaveStatement() {
        if (match(Token::IDENTIFIER)) {
            std::string target = previous().value;
            
            if (target == "model") {
                if (match(Token::TO)) {
                    if (match(Token::STRING)) {
                        std::string filename = previous().value;
                        std::cout << "กำลังบันทึกโมเดลไปยังไฟล์: " << filename << std::endl;
                    }
                }
            }
        }
    }
    
    void parseVisualizeStatement() {
        if (match(Token::IDENTIFIER)) {
            std::string target = previous().value;
            std::cout << "กำลังแสดงภาพข้อมูล: " << target << std::endl;
        }
    }
    
    void parsePlotStatement() {
        if (match(Token::IDENTIFIER)) {
            std::string target = previous().value;
            std::cout << "กำลังพล็อตกราฟ: " << target << std::endl;
        }
    }
    
    void parseParameters() {
        while (match(Token::IDENTIFIER)) {
            std::string param = previous().value;
            
            if (match(Token::NUMBER)) {
                std::string value = previous().value;
                std::cout << "พารามิเตอร์: " << param << " = " << value << std::endl;
            } else if (match(Token::STRING)) {
                std::string value = previous().value;
                std::cout << "พารามิเตอร์: " << param << " = \"" << value << "\"" << std::endl;
            }
            
            if (!match(Token::IDENTIFIER) && previous().value != "with") {
                break;
            }
        }
    }
};

class Interpreter {
public:
    void interpret(const std::string& source) {
        Lexer lexer(source);
        std::vector<Token> tokens = lexer.tokenize();
        
        Parser parser(tokens);
        parser.parse();
    }
};

// ฟังก์ชันช่วยสำหรับ demo
void runDemo(const std::string& command) {
    std::cout << "\n> " << command << std::endl;
    Interpreter interpreter;
    interpreter.interpret(command);
}

int main() {
    std::cout << "=== ทดสอบภาษาโปรแกรมสำหรับ AI ===\n" << std::endl;
    
    // ทดสอบคำสั่งพื้นฐาน
    runDemo("start create ML");
    runDemo("load dataset \"data.csv\"");
    runDemo("clean data");
    runDemo("split data into train and test with ratio 0.8");
    runDemo("create model LinearRegression");
    runDemo("create model DecisionTree with max_depth 5");
    runDemo("train model with epochs 100 batch_size 32 learning_rate 0.001");
    runDemo("evaluate model on test_data");
    runDemo("show accuracy");
    runDemo("save model to \"trained_model.h5\"");
    
    // ทดสอบคำสั่งสำหรับ Deep Learning
    std::cout << "\n=== Deep Learning ===\n";
    runDemo("start create DL");
    runDemo("create neural_network with layers 3 nodes 128 activation \"relu\"");
    runDemo("train model for epochs 50");
    
    // ทดสอบคำสั่งสำหรับ Reinforcement Learning
    std::cout << "\n=== Reinforcement Learning ===\n";
    runDemo("start create RL");
    runDemo("create agent with policy \"DQN\"");
    runDemo("train agent for episodes 1000");
    
    std::cout << "\n=== การแสดงผลข้อมูล ===\n";
    runDemo("visualize data");
    runDemo("plot learning_curve");
    
    return 0;
}