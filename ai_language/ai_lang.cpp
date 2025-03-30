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

// ฟังก์ชันสำหรับโหมดโต้ตอบ
void runInteractiveMode() {
    bool running = true;
    bool hasProject = false;
    bool hasLoadedData = false;
    bool hasCleanedData = false;
    bool hasSplitData = false;
    bool hasCreatedModel = false;
    bool hasTrainedModel = false;
    bool hasEvaluatedModel = false;
    std::string projectType;

    std::cout << "=== โหมดโต้ตอบ AI Language ===\n" << std::endl;
    std::cout << "พิมพ์ 'help' เพื่อดูคำสั่งที่รองรับ หรือ 'exit' เพื่อออกจากโปรแกรม\n" << std::endl;

    while (running) {
        std::string command;

        std::cout << "AI> ";
        std::getline(std::cin, command);

        // แปลงเป็นตัวพิมพ์เล็กเพื่อง่ายต่อการตรวจสอบ
        std::string lowerCmd = command;
        std::transform(lowerCmd.begin(), lowerCmd.end(), lowerCmd.begin(), ::tolower);

        if (lowerCmd == "exit" || lowerCmd == "quit") {
            running = false;
        } else if (lowerCmd == "help") {
            std::cout << "\nคำสั่งที่รองรับ:" << std::endl;
            std::cout << "  start create [ML/DL/RL] - เริ่มโปรเจกต์ใหม่" << std::endl;
            std::cout << "  load dataset \"ชื่อไฟล์\" - โหลดข้อมูลจากไฟล์" << std::endl;
            std::cout << "  clean data - ทำความสะอาดข้อมูล" << std::endl;
            std::cout << "  split data into train and test with ratio 0.8 - แบ่งข้อมูลเพื่อฝึกและทดสอบ" << std::endl;
            std::cout << "  create model [ชื่อโมเดล] - สร้างโมเดล ML" << std::endl;
            std::cout << "  train model for epochs 100 - ฝึกโมเดล" << std::endl;
            std::cout << "  evaluate model on test_data - ประเมินโมเดล" << std::endl;
            std::cout << "  show accuracy - แสดงความแม่นยำ" << std::endl;
            std::cout << "  save model to \"ชื่อไฟล์\" - บันทึกโมเดล" << std::endl;
            std::cout << "  visualize data - แสดงภาพข้อมูล" << std::endl;
            std::cout << "  plot learning_curve - แสดงกราฟการเรียนรู้" << std::endl;
            std::cout << "  exit/quit - ออกจากโปรแกรม" << std::endl;
        } else {
            // ตรวจสอบลำดับขั้นตอนที่ถูกต้อง
            if (lowerCmd.find("start create") != std::string::npos) {
                hasProject = true;
                if (lowerCmd.find("ml") != std::string::npos) {
                    projectType = "ML";
                } else if (lowerCmd.find("dl") != std::string::npos) {
                    projectType = "DL";
                } else if (lowerCmd.find("rl") != std::string::npos) {
                    projectType = "RL";
                }
            } else if (!hasProject) {
                std::cout << "โปรดเริ่มต้นโปรเจกต์ก่อนด้วยคำสั่ง 'start create [ML/DL/RL]'" << std::endl;
                continue;
            }

            if (hasProject) {
                bool valid = true;

                if (lowerCmd.find("load dataset") != std::string::npos) {
                    hasLoadedData = true;
                } else if (lowerCmd.find("clean") != std::string::npos) {
                    if (!hasLoadedData) {
                        std::cout << "โปรดโหลดข้อมูลก่อนด้วยคำสั่ง 'load dataset \"ชื่อไฟล์\"'" << std::endl;
                        valid = false;
                    }
                    hasCleanedData = true;
                } else if (lowerCmd.find("split") != std::string::npos) {
                    if (!hasCleanedData) {
                        std::cout << "โปรดทำความสะอาดข้อมูลก่อนด้วยคำสั่ง 'clean data'" << std::endl;
                        valid = false;
                    }
                    hasSplitData = true;
                } else if (lowerCmd.find("create model") != std::string::npos) {
                    if (!hasSplitData) {
                        std::cout << "โปรดแบ่งข้อมูลก่อนด้วยคำสั่ง 'split data into train and test with ratio 0.8'" << std::endl;
                        valid = false;
                    }
                    hasCreatedModel = true;
                } else if (lowerCmd.find("train model") != std::string::npos) {
                    if (!hasCreatedModel) {
                        std::cout << "โปรดสร้างโมเดลก่อนด้วยคำสั่ง 'create model [ชื่อโมเดล]'" << std::endl;
                        valid = false;
                    }
                    hasTrainedModel = true;
                } else if (lowerCmd.find("evaluate") != std::string::npos) {
                    if (!hasTrainedModel) {
                        std::cout << "โปรดฝึกโมเดลก่อนด้วยคำสั่ง 'train model for epochs 100'" << std::endl;
                        valid = false;
                    }
                    hasEvaluatedModel = true;
                } else if (lowerCmd.find("save model") != std::string::npos) {
                    if (!hasEvaluatedModel) {
                        std::cout << "โปรดประเมินโมเดลก่อนด้วยคำสั่ง 'evaluate model on test_data'" << std::endl;
                        valid = false;
                    }
                }

                if (valid) {
                    try {
                        // ประมวลผลคำสั่ง
                        Interpreter interpreter;
                        interpreter.interpret(command);
                    } catch (const std::exception& e) {
                        std::cout << "เกิดข้อผิดพลาด: " << e.what() << std::endl;
                    }
                }
            }
        }
    }

    std::cout << "ออกจากโปรแกรม" << std::endl;
}

// ฟังก์ชันรันโค้ดจากไฟล์
void runFromFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "ไม่สามารถเปิดไฟล์: " << filename << std::endl;
        return;
    }

    std::string line;
    std::string content;

    while (std::getline(file, line)) {
        // ข้ามบรรทัดว่างและความคิดเห็น
        if (line.empty() || line[0] == '#') {
            continue;
        }

        content += line + "\n";
    }

    std::cout << "=== ประมวลผลจากไฟล์: " << filename << " ===\n" << std::endl;
    Interpreter interpreter;
    interpreter.interpret(content);
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        // รันจากไฟล์
        runFromFile(argv[1]);
    } else {
        // รันในโหมดโต้ตอบ
        runInteractiveMode();
    }

    return 0;
}