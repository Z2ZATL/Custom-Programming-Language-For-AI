# บันทึกการพัฒนา AI Language

เอกสารนี้รวบรวมบันทึกการพัฒนาและโน้ตสำหรับนักพัฒนาที่ทำงานกับ AI Language

## โครงสร้างโปรเจกต์

```
ai_language/
├── include/                # C++ header files
│   ├── interpreters/       # Interpreters for different AI types
│   ├── connectors/         # Connectors to ML libraries
│   ├── utils/              # Utility functions
│   ├── lexer.h             # Lexer for tokenizing
│   ├── parser.h            # Parser for syntax analysis
│   └── token_types.h       # Token type definitions
├── src/                    # C++ source files
│   ├── interpreters/       # Interpreter implementations
│   ├── connectors/         # Connector implementations
│   ├── utils/              # Utility implementations
│   ├── lexer.cpp           # Lexer implementation
│   ├── parser.cpp          # Parser implementation
│   └── ai_lang.cpp         # Main entry point
├── datasets/               # Sample datasets
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

## ระบบไวยากรณ์

AI Language ใช้โครงสร้างไวยากรณ์แบบ command-based ที่เน้นความเรียบง่ายและอ่านง่าย โดยมีขั้นตอนการประมวลผลดังนี้:

1. **Lexer** - แปลงข้อความเข้าเป็น token
2. **Parser** - วิเคราะห์ token และตรวจสอบไวยากรณ์
3. **Interpreter** - ตีความและดำเนินการตามคำสั่ง

## เพิ่มคำสั่งใหม่

การเพิ่มคำสั่งใหม่ทำได้โดยปฏิบัติตามขั้นตอนดังนี้:

1. ปรับปรุง Lexer ให้รองรับ token ใหม่ (ถ้าจำเป็น)
2. ปรับปรุง Parser ให้รองรับไวยากรณ์ใหม่
3. เพิ่มการตีความในไฟล์ Interpreter ที่เหมาะสม
4. อัพเดตเอกสารใน `docs/references/SYNTAX_REFERENCE.md`
5. เพิ่มตัวอย่างการใช้งานใน `examples/`

## การจัดการโมเดลและการบันทึก

โมเดลทั้งหมดถูกบันทึกด้วยนามสกุลที่กำหนดไว้ตามประเภท:
- `.mlmodel` - Machine Learning
- `.dlmodel` - Deep Learning
- `.rlmodel` - Reinforcement Learning

การบันทึกโมเดลจะเพิ่มนามสกุลที่เหมาะสมตามประเภทโดยอัตโนมัติ และบันทึกเมตาดาต้าเกี่ยวกับเวลาที่บันทึกตามเขตเวลาที่กำหนด

## การเชื่อมต่อกับไลบรารีภายนอก

AI Language สามารถเชื่อมต่อกับไลบรารีภายนอกได้ผ่านระบบ Connector:

- **MLInterpreter** - เชื่อมต่อกับ scikit-learn
- **DLInterpreter** - เชื่อมต่อกับ TensorFlow/Keras
- **RLInterpreter** - เชื่อมต่อกับ OpenAI Gym

## แผนการพัฒนาในอนาคต

- [ ] เพิ่มการรองรับ multi-line commands
- [ ] เพิ่มความสามารถในการ import สคริปต์จากไฟล์อื่น
- [ ] พัฒนา GUI สำหรับ Interactive Mode
- [ ] เพิ่มการรองรับการทำ Hyperparameter Tuning
- [ ] เพิ่มการรองรับ Transfer Learning
- [ ] พัฒนาการเชื่อมต่อกับฐานข้อมูล
- [ ] ขยายความสามารถในการทำงานกับข้อมูลหลายประเภท (image, audio, text)

## การทดสอบ

ใช้คำสั่งต่อไปนี้ในการทดสอบ:

```bash
cd build
make
ctest
```

## การรายงานปัญหา

หากพบปัญหาในการพัฒนา โปรดรายงานในระบบ issue tracking โดยระบุข้อมูลต่อไปนี้:
- สภาพแวดล้อมที่ใช้ (OS, compiler)
- ขั้นตอนที่ทำให้เกิดปัญหา
- ข้อความแสดงข้อผิดพลาด
- สคริปต์ตัวอย่างที่ทำให้เกิดปัญหา

## การตั้งค่า Timezone

ระบบการตั้งค่าเขตเวลาช่วยให้ผู้ใช้สามารถกำหนดเขตเวลาสำหรับการบันทึกเมตาดาต้าได้ โดยเก็บค่า offset จาก UTC ไว้ในไฟล์โมเดล