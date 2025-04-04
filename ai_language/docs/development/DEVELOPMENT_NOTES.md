
# บันทึกการพัฒนา AI Language

เอกสารนี้รวบรวมบันทึกการพัฒนาและโน้ตสำหรับนักพัฒนาที่ทำงานกับ AI Language

## โครงสร้างโปรเจกต์

```
ai_language/
├── include/                # C++ header files
│   ├── interpreters/       # Interpreters for different AI types
│   │   ├── BaseInterpreter.h      # พื้นฐานสำหรับ Interpreter ทุกประเภท
│   │   ├── MLInterpreter.h        # สำหรับ Machine Learning
│   │   ├── DLInterpreter.h        # สำหรับ Deep Learning
│   │   ├── RLInterpreter.h        # สำหรับ Reinforcement Learning
│   │   └── InterpreterFactory.h   # Factory pattern สำหรับสร้าง Interpreter
│   ├── connectors/         # Connectors to ML libraries
│   │   ├── BaseConnector.h        # พื้นฐานสำหรับ Connector ทุกประเภท
│   │   ├── Connector.h            # อินเตอร์เฟซหลักสำหรับเชื่อมต่อกับไลบรารี
│   │   └── ScikitLearnConnector.h # เชื่อมต่อกับ scikit-learn
│   ├── utils/              # Utility functions
│   │   └── plotting.h              # ฟังก์ชันสำหรับการสร้างกราฟ
│   ├── lexer.h             # Lexer for tokenizing
│   ├── parser.h            # Parser for syntax analysis
│   └── token_types.h       # Token type definitions
├── src/                    # C++ source files
│   ├── interpreters/       # Interpreter implementations
│   │   ├── BaseInterpreter.cpp     # การตีความคำสั่งพื้นฐาน
│   │   ├── MLInterpreter.cpp       # การตีความคำสั่งสำหรับ ML
│   │   ├── DLInterpreter.cpp       # การตีความคำสั่งสำหรับ DL
│   │   ├── RLInterpreter.cpp       # การตีความคำสั่งสำหรับ RL
│   │   └── InterpreterFactory.cpp  # การสร้าง Interpreter ตามประเภท
│   ├── connectors/         # Connector implementations
│   │   ├── Connector.cpp           # การเชื่อมต่อกับไลบรารีภายนอก
│   │   └── ScikitLearnConnector.cpp # การเชื่อมต่อกับ scikit-learn
│   ├── utils/              # Utility implementations
│   │   ├── plotting.cpp             # การสร้างกราฟและการแสดงผล
│   │   ├── show_time.cpp            # ตัวอย่างการแสดงเวลาและเขตเวลา
│   │   └── plot_generator.py        # สคริปต์ Python สำหรับสร้างกราฟขั้นสูง
│   ├── lexer.cpp           # Lexer implementation
│   ├── parser.cpp          # Parser implementation
│   └── ai_lang.cpp         # Main entry point
├── datasets/               # Sample datasets
│   ├── images/             # ตัวอย่างข้อมูลรูปภาพ
│   ├── iris.csv            # ตัวอย่างข้อมูล iris
│   ├── linear_data.csv     # ตัวอย่างข้อมูลสำหรับ linear regression
│   ├── classification_data.csv # ตัวอย่างข้อมูลสำหรับ classification
│   ├── environment.json    # ตัวอย่างข้อมูลสภาพแวดล้อมสำหรับ RL
│   └── config.json         # ตัวอย่างการตั้งค่าสำหรับ RL
├── docs/                   # Documentation
│   ├── development/        # Development notes
│   ├── guides/             # User guides
│   └── references/         # Reference documentation
├── examples/               # Example scripts
│   ├── ml_examples/        # Machine Learning examples
│   ├── dl_examples/        # Deep Learning examples
│   ├── rl_examples/        # Reinforcement Learning examples
│   ├── syntax_guide.ai     # คู่มือไวยากรณ์
│   └── timezone_example.ai # ตัวอย่างการใช้ timezone
├── tests/                  # Test files
│   ├── CMakeLists.txt      # CMake for tests
│   ├── lexer_test.cpp      # Lexer tests
│   ├── parser_test.cpp     # Parser tests
│   └── interpreter_test.cpp # Interpreter tests
└── CMakeLists.txt          # Main CMake file
```

## การ Build โปรเจกต์

```bash
mkdir -p build
cd build
cmake ..
make
```

ทดสอบการทำงาน:

```bash
./ai_lang ../examples/ml_examples/linear_regression.ai
```

รันในโหมด Interactive:

```bash
./ai_lang -i
```

## ระบบไวยากรณ์

AI Language ใช้โครงสร้างไวยากรณ์แบบ command-based ที่เน้นความเรียบง่ายและอ่านง่าย โดยมีขั้นตอนการประมวลผลดังนี้:

1. **Lexer** - แปลงข้อความเข้าเป็น token
   - แยกประเภทของแต่ละส่วนในคำสั่ง เช่น คีย์เวิร์ด, ค่าตัวเลข, สตริง
   - จัดการกับคอมเมนต์และช่องว่าง

2. **Parser** - วิเคราะห์ token และตรวจสอบไวยากรณ์
   - ตรวจสอบความถูกต้องของคำสั่ง
   - ตรวจสอบจำนวนและประเภทของอาร์กิวเมนต์
   - แปลงเป็นโครงสร้างข้อมูลภายในสำหรับ Interpreter

3. **Interpreter** - ตีความและดำเนินการตามคำสั่ง
   - ตีความคำสั่งต่างๆ เช่น การสร้างโมเดล, การฝึกโมเดล
   - ประมวลผลและแสดงผลลัพธ์
   - จัดการกับการบันทึกและโหลดโมเดล

## เพิ่มคำสั่งใหม่

การเพิ่มคำสั่งใหม่ทำได้โดยปฏิบัติตามขั้นตอนดังนี้:

1. ปรับปรุง Lexer ให้รองรับ token ใหม่ (ถ้าจำเป็น)
   - เพิ่มคีย์เวิร์ดใหม่ใน `token_types.h`
   - ปรับปรุงฟังก์ชัน tokenize ใน `lexer.cpp`

2. ปรับปรุง Parser ให้รองรับไวยากรณ์ใหม่
   - เพิ่มการตรวจสอบรูปแบบคำสั่งใหม่ใน `parser.cpp`
   - ตรวจสอบความถูกต้องของอาร์กิวเมนต์

3. เพิ่มการตีความในไฟล์ Interpreter ที่เหมาะสม
   - สร้างฟังก์ชันใหม่ใน Interpreter ที่เกี่ยวข้อง
   - ปรับปรุงฟังก์ชัน `interpretLine` ให้รองรับคำสั่งใหม่

4. อัพเดตเอกสารใน `docs/references/SYNTAX_REFERENCE.md`

5. เพิ่มตัวอย่างการใช้งานใน `examples/`

ตัวอย่างการเพิ่มคำสั่ง `export_model` ใน MLInterpreter:

```cpp
// เพิ่มใน include/interpreters/MLInterpreter.h
void handleExportModelCommand(const std::vector<std::string>& args);

// เพิ่มใน src/interpreters/MLInterpreter.cpp
void MLInterpreter::handleExportModelCommand(const std::vector<std::string>& args) {
    if (args.size() < 1) {
        std::cout << "Error: export_model requires a filename argument" << std::endl;
        return;
    }
    
    std::string filename = args[0];
    // ดำเนินการส่งออกโมเดล
    std::cout << "Exporting model to: " << filename << std::endl;
    // [implementation here]
}

// ปรับปรุง interpretLine ใน BaseInterpreter.cpp
void BaseInterpreter::interpretLine(const std::string& line) {
    // ...
    else if (command == "export_model") {
        handleExportModelCommand(args);
    }
    // ...
}
```

## การจัดการโมเดลและการบันทึก

โมเดลทั้งหมดถูกบันทึกด้วยนามสกุลที่กำหนดไว้ตามประเภท:
- `.mlmodel` - Machine Learning
- `.dlmodel` - Deep Learning
- `.rlmodel` - Reinforcement Learning

การบันทึกโมเดลจะเพิ่มนามสกุลที่เหมาะสมตามประเภทโดยอัตโนมัติ และบันทึกเมตาดาต้าเกี่ยวกับเวลาที่บันทึกตามเขตเวลาที่กำหนด

ตัวอย่างเนื้อหาไฟล์โมเดล:

```
# ML Model saved from AI Language
model_type: LinearRegression
learning_rate: 0.01
epochs: 100
accuracy: 0.95
create_time: 2023-08-15 14:30:45 (UTC+7)
```

## การเชื่อมต่อกับไลบรารีภายนอก

AI Language สามารถเชื่อมต่อกับไลบรารีภายนอกได้ผ่านระบบ Connector:

- **MLInterpreter** - เชื่อมต่อกับ scikit-learn
- **DLInterpreter** - เชื่อมต่อกับ TensorFlow/Keras
- **RLInterpreter** - เชื่อมต่อกับ OpenAI Gym

การเพิ่ม Connector ใหม่:

1. สร้างไฟล์ใหม่ใน `include/connectors/` และ `src/connectors/`
2. สืบทอดจากคลาส `BaseConnector`
3. ลงทะเบียน Connector ใหม่ใน `InterpreterFactory`

## การรองรับคำสั่งหลายบรรทัด

AI Language รองรับการเขียนคำสั่งหลายบรรทัดโดยใช้ `;;` เป็นตัวจบคำสั่ง ซึ่งการทำงานจะดำเนินการดังนี้:

1. สะสมคำสั่งในตัวแปร `multiline` จนกว่าจะพบ `;;`
2. เมื่อพบ `;;` หรือสิ้นสุดการรับข้อมูล จะส่งคำสั่งไปประมวลผล
3. ตรวจสอบคำสั่ง `create ML/DL/RL` เพื่อเลือก Interpreter ที่เหมาะสม

```cpp
// ตัวอย่างจาก ai_lang.cpp
std::string multiline = "";
// ...
if (multiline.find(";;") != std::string::npos || line.find(";;") != std::string::npos) {
    // ลบเครื่องหมาย ;; ออก
    size_t pos = multiline.find(";;");
    if (pos != std::string::npos) {
        multiline.erase(pos, 2);
    }
    
    // แยกคำสั่งและประมวลผลทีละบรรทัด
    std::istringstream iss(multiline);
    std::string command_line;
    while (std::getline(iss, command_line)) {
        if (!command_line.empty()) {
            interpreter->interpretLine(command_line);
        }
    }
    
    multiline = "";
} else {
    multiline += line + "\n";
}
```

## การตั้งค่า Timezone

ระบบการตั้งค่าเขตเวลาช่วยให้ผู้ใช้สามารถกำหนดเขตเวลาสำหรับการบันทึกเมตาดาต้าได้ โดยเก็บค่า offset จาก UTC ไว้ในไฟล์โมเดล

```cpp
// การตั้งค่า timezone ใน BaseInterpreter
if (paramName == "timezone") {
    try {
        int tz = std::stoi(paramValue);
        timezone = tz;
        std::cout << "Set timezone to UTC" << (tz >= 0 ? "+" : "") << tz << std::endl;
    } catch (std::invalid_argument& e) {
        std::cout << "Error: Invalid timezone value. Please provide an integer." << std::endl;
    }
}

// การใช้ timezone ในการบันทึกเวลา
std::string BaseInterpreter::getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    
    // ปรับเวลาตาม timezone
    in_time_t += timezone * 3600;
    
    std::stringstream ss;
    ss << std::put_time(std::gmtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    ss << " (UTC" << (timezone >= 0 ? "+" : "") << timezone << ")";
    
    return ss.str();
}
```

## แผนการพัฒนาในอนาคต

- [ ] **เพิ่มการรองรับ multi-line commands** - ปรับปรุงระบบการรองรับคำสั่งหลายบรรทัด
- [ ] **เพิ่มความสามารถในการ import สคริปต์จากไฟล์อื่น** - เพิ่มคำสั่ง `import` สำหรับการนำเข้าสคริปต์อื่น
- [ ] **พัฒนา GUI สำหรับ Interactive Mode** - สร้างส่วนติดต่อผู้ใช้แบบกราฟิก
- [ ] **เพิ่มการรองรับการทำ Hyperparameter Tuning** - เพิ่มคำสั่งสำหรับการปรับค่าพารามิเตอร์อัตโนมัติ
- [ ] **เพิ่มการรองรับ Transfer Learning** - เพิ่มความสามารถในการถ่ายโอนการเรียนรู้ระหว่างโมเดล
- [ ] **พัฒนาการเชื่อมต่อกับฐานข้อมูล** - เพิ่มความสามารถในการโหลดข้อมูลจากฐานข้อมูล
- [ ] **ขยายความสามารถในการทำงานกับข้อมูลหลายประเภท** - เพิ่มการรองรับข้อมูลประเภท image, audio, text
- [ ] **พัฒนาระบบการจัดการข้อผิดพลาดที่ดีขึ้น** - ปรับปรุงระบบการรายงานและการจัดการข้อผิดพลาด
- [ ] **เพิ่มการรองรับภาษาต่างๆ** - เพิ่มการรองรับภาษาอื่นๆ นอกเหนือจากภาษาอังกฤษ
- [ ] **การบูรณาการกับเครื่องมือ CI/CD** - เพิ่มการทดสอบอัตโนมัติและการวิเคราะห์โค้ด

## การทดสอบ

ใช้คำสั่งต่อไปนี้ในการทดสอบ:

```bash
cd build
make
ctest
```

การเพิ่มการทดสอบใหม่:

1. สร้างไฟล์ทดสอบใหม่ใน `tests/`
2. ปรับปรุง `tests/CMakeLists.txt` เพื่อเพิ่มการทดสอบ
3. รัน `ctest` เพื่อทดสอบ

## การรายงานปัญหา

หากพบปัญหาในการพัฒนา โปรดรายงานในระบบ issue tracking โดยระบุข้อมูลต่อไปนี้:
- สภาพแวดล้อมที่ใช้ (OS, compiler)
- ขั้นตอนที่ทำให้เกิดปัญหา
- ข้อความแสดงข้อผิดพลาด
- สคริปต์ตัวอย่างที่ทำให้เกิดปัญหา

## คำแนะนำในการเขียนโค้ด

1. **การตั้งชื่อฟังก์ชัน**:
   - ใช้รูปแบบ `camelCase` สำหรับชื่อฟังก์ชัน
   - ใช้ชื่อที่อธิบายการทำงานได้ชัดเจน เช่น `handleTrainCommand`, `evaluateModel`

2. **การตั้งชื่อตัวแปร**:
   - ใช้รูปแบบ `camelCase` สำหรับชื่อตัวแปร
   - ใช้ชื่อที่สื่อความหมาย เช่น `modelType`, `learningRate`

3. **การจัดรูปแบบโค้ด**:
   - ใช้ 4 spaces สำหรับการย่อหน้า
   - ใช้ `{}` สำหรับทุกบล็อกโค้ด แม้จะมีคำสั่งเดียว

4. **การจัดการข้อผิดพลาด**:
   - ตรวจสอบอาร์กิวเมนต์ก่อนใช้งานเสมอ
   - แสดงข้อความแจ้งเตือนที่ชัดเจนเมื่อเกิดข้อผิดพลาด

5. **การเขียนคอมเมนต์**:
   - เขียนคอมเมนต์อธิบายการทำงานของฟังก์ชันและอัลกอริทึมที่ซับซ้อน
   - ใช้ Doxygen-style comments สำหรับ API

## ข้อมูลเพิ่มเติม

- ศึกษารายละเอียดเพิ่มเติมได้จากเอกสารใน `docs/`
- ดูตัวอย่างการใช้งานได้จาก `examples/`
- ศึกษาการทดสอบได้จาก `tests/`
