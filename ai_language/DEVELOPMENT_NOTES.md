
# บันทึกการพัฒนา AI Language

## โครงสร้างของโปรเจกต์

```
ai_language/
├── include/              # ไฟล์ header สำหรับ library
│   ├── BaseInterpreter.h # คลาสพื้นฐานสำหรับ Interpreter
│   ├── DLInterpreter.h   # Interpreter สำหรับ Deep Learning
│   ├── InterpreterFactory.h # Factory สำหรับสร้าง Interpreter ตามประเภท
│   ├── MLInterpreter.h   # Interpreter สำหรับ Machine Learning
│   ├── RLInterpreter.h   # Interpreter สำหรับ Reinforcement Learning
│   ├── interpreter.h     # Interpreter หลัก
│   ├── lexer.h           # Lexer สำหรับแยกโทเค็น
│   ├── parser.h          # Parser สำหรับแปลง tokens เป็นคำสั่ง
│   └── token_types.h     # นิยามประเภทโทเค็นต่างๆ
├── src/                  # ไฟล์ source code
│   ├── BaseInterpreter.cpp # การทำงานของ Interpreter พื้นฐาน
│   ├── DLInterpreter.cpp # การทำงานของ DL Interpreter
│   ├── InterpreterFactory.cpp # การสร้าง Interpreter ตามประเภท
│   ├── MLInterpreter.cpp # การทำงานของ ML Interpreter
│   ├── RLInterpreter.cpp # การทำงานของ RL Interpreter
│   ├── interpreter.cpp   # การทำงานของ Interpreter หลัก
│   ├── lexer.cpp         # การทำงานของ Lexer
│   └── parser.cpp        # การทำงานของ Parser
├── examples/             # ตัวอย่างโค้ด
│   ├── dl_examples/      # ตัวอย่างสำหรับ Deep Learning
│   │   ├── cnn.ai        # ตัวอย่าง CNN
│   │   └── neural_network.ai # ตัวอย่าง Neural Network
│   ├── ml_examples/      # ตัวอย่างสำหรับ Machine Learning
│   │   ├── classification.ai # ตัวอย่างการจำแนกประเภท
│   │   └── linear_regression.ai # ตัวอย่าง Linear Regression
│   ├── rl_examples/      # ตัวอย่างสำหรับ Reinforcement Learning
│   │   ├── basic_rl_example.ai # ตัวอย่าง RL พื้นฐาน
│   │   ├── dqn.ai        # ตัวอย่าง Deep Q-Network
│   │   └── q_learning.ai # ตัวอย่าง Q-Learning
│   ├── README.md         # คำอธิบายตัวอย่าง
│   ├── auto_parameter_guide.ai # คู่มือการตั้งค่าพารามิเตอร์อัตโนมัติ
│   ├── multiline_syntax_test.ai # ตัวอย่างการใช้ไวยากรณ์หลายบรรทัด
│   ├── syntax_guide.ai   # คู่มือไวยากรณ์
│   └── timezone_example.ai # ตัวอย่างการใช้ timezone
├── tests/                # ไฟล์ทดสอบ
│   ├── CMakeLists.txt    # ไฟล์ build สำหรับทดสอบ
│   ├── advanced_error_handling_test.ai # ทดสอบการจัดการข้อผิดพลาดขั้นสูง
│   ├── error_handling_test.ai # ทดสอบการจัดการข้อผิดพลาดพื้นฐาน
│   ├── interpreter_test.cpp # ทดสอบ Interpreter
│   ├── lexer_test.cpp    # ทดสอบ Lexer
│   └── parser_test.cpp   # ทดสอบ Parser
├── datasets/             # ข้อมูลตัวอย่างสำหรับทดสอบ
│   ├── images/           # ข้อมูลรูปภาพสำหรับ CNN
│   ├── classification_data.csv # ข้อมูลสำหรับการจำแนกประเภท
│   ├── config.json       # ไฟล์การตั้งค่า
│   ├── environment.json  # สภาพแวดล้อมสำหรับ RL
│   ├── iris.csv          # ชุดข้อมูล Iris สำหรับ Neural Network
│   └── linear_data.csv   # ข้อมูลสำหรับ Linear Regression
├── build/                # โฟลเดอร์สำหรับไฟล์ที่คอมไพล์
│   ├── ai_lang           # ไฟล์ executable ที่คอมไพล์แล้ว
│   └── libai_language_lib.a # ไลบรารีที่คอมไพล์แล้ว
├── ai_lang               # ไฟล์ executable ในระดับโปรเจกต์
├── ai_lang.cpp           # โปรแกรมหลัก
├── CMakeLists.txt        # ไฟล์ build
├── README.md             # คำอธิบายโปรเจกต์
├── DEVELOPMENT_NOTES.md  # บันทึกการพัฒนา
├── SYNTAX_FLOW.md        # เอกสารแสดงการไหลของไวยากรณ์
├── SYNTAX_REFERENCE.md   # เอกสารอ้างอิงไวยากรณ์
├── USAGE_GUIDE.md        # คู่มือการใช้งาน
├── organize_examples.sh  # สคริปต์จัดระเบียบตัวอย่าง
├── run_example.sh        # สคริปต์รันตัวอย่าง
├── show_time             # โปรแกรมแสดงเวลา
└── show_time.cpp         # ซอร์สโค้ดโปรแกรมแสดงเวลา
```

## ส่วนประกอบหลักของภาษา

1. **Lexer** - แปลงข้อความให้เป็น tokens
2. **Parser** - แปลง tokens เป็นโครงสร้างแบบ AST (Abstract Syntax Tree)
3. **Interpreter** - ประมวลผล AST เพื่อดำเนินการตามคำสั่ง

## ฟีเจอร์สำคัญ

1. **การสร้างโมเดล AI** - สามารถสร้างโมเดล ML, DL และ RL ได้ง่าย
2. **การโหลดและประมวลผลข้อมูล** - รองรับหลายรูปแบบไฟล์และแหล่งข้อมูล
3. **การฝึกและประเมินโมเดล** - คำสั่งง่ายๆ สำหรับการฝึกและทดสอบโมเดล
4. **การบันทึกและโหลดโมเดล** - เก็บและนำกลับมาใช้โมเดลที่ฝึกแล้ว
5. **การตั้งค่า timezone** - บันทึกเวลาตามเขตเวลาของผู้ใช้

## การพัฒนาในอนาคต

### เฟส 1: พื้นฐาน (เสร็จสมบูรณ์)
- [x] อ่านไฟล์ .ai และแสดงผลคำสั่งพื้นฐาน
- [x] สร้างตัวอย่างสำหรับ ML, DL, RL
- [x] สร้าง lexer เบื้องต้น
- [x] สร้าง parser เบื้องต้น
- [x] พัฒนาโมดูลที่ใช้ร่วมกันสำหรับทุกประเภทของ AI
- [x] รองรับการตั้งค่า timezone ตามภูมิภาคของผู้ใช้
- [x] เพิ่มระบบการจัดการข้อผิดพลาดขั้นสูง
- [x] สนับสนุนการประมวลผลคำสั่ง predict
- [x] รองรับนามสกุลไฟล์โมเดลมาตรฐาน (.pkl, .h5, .pt, .onnx)

### เฟส 2: การเชื่อมต่อกับไลบรารี AI (กำลังพัฒนา)
- [ ] เชื่อมต่อกับ scikit-learn สำหรับ Machine Learning
- [ ] เชื่อมต่อกับ TensorFlow/PyTorch สำหรับ Deep Learning
- [ ] เชื่อมต่อกับ OpenAI Gym สำหรับ Reinforcement Learning
- [ ] สร้างระบบแสดงผลข้อมูลและกราฟ
- [x] ระบบรองรับการใช้งานในโหมดโต้ตอบ (Interactive Mode)
- [x] การทดสอบการจัดการข้อผิดพลาดขั้นสูง
- [x] พัฒนาระบบการตั้งค่าพารามิเตอร์อัตโนมัติ
- [ ] พัฒนาระบบตรวจสอบความเข้ากันได้ของนามสกุลไฟล์โมเดลกับเฟรมเวิร์ก

### เฟส 3: คุณสมบัติขั้นสูง (กำลังพัฒนา)
- [x] เพิ่มความสามารถในการปรับแต่งโมเดลอัตโนมัติ
- [ ] เพิ่มการวิเคราะห์ประสิทธิภาพโมเดล
- [ ] เพิ่มการแนะนำโมเดลและพารามิเตอร์ที่เหมาะสม
- [x] พัฒนาระบบปรับแต่งพารามิเตอร์อัตโนมัติ
- [ ] รองรับการทำงานบนคลาวด์และการทำงานร่วมกัน
- [ ] เพิ่มระบบแปลงรูปแบบไฟล์โมเดลระหว่างเฟรมเวิร์กต่างๆ (เช่น .pkl เป็น .onnx)

### เฟส 4: UI และการใช้งานทั่วไป (วางแผน)
- [ ] พัฒนา UI แบบกราฟิกสำหรับการใช้งาน
- [ ] สร้างเวอร์ชันเว็บแอปพลิเคชัน
- [ ] รองรับการทำงานบนอุปกรณ์มือถือ
- [ ] ระบบจัดการและแบ่งปันโมเดล
- [ ] สร้างแคตตาล็อกโมเดลสำเร็จรูปพร้อมใช้งาน

## การมีส่วนร่วมในการพัฒนา

หากคุณสนใจมีส่วนร่วมในการพัฒนา AI Language โปรดปฏิบัติตามขั้นตอนต่อไปนี้:

1. Fork repository
2. สร้าง branch ใหม่สำหรับคุณสมบัติที่ต้องการเพิ่ม
3. พัฒนาและทดสอบการเปลี่ยนแปลงของคุณ
4. ส่ง Pull Request พร้อมคำอธิบายที่ชัดเจน

## แนวทางการเขียนโค้ด

- ใช้ C++ 17 
- ใช้ Google C++ Style Guide
- ทุกฟังก์ชันต้องมี unit tests
- ใช้ snake_case สำหรับตัวแปรและฟังก์ชัน
- ใช้ CamelCase สำหรับคลาสและโครงสร้าง

## คำแนะนำสำหรับการพัฒนา

### วิธีการสร้าง
```bash
mkdir build && cd build
cmake ..
make
```

### วิธีการรันโปรแกรมในโหมดไฟล์
```bash
./ai_lang ../examples/ml_examples/linear_regression.ai
```

### วิธีการรันโปรแกรมในโหมดโต้ตอบ
```bash
./ai_lang -i
```

### วิธีการรันทดสอบ
```bash
mkdir build && cd build
cmake -DBUILD_TESTS=ON ..
make
make test
```

## ปัญหาที่ทราบแล้ว

- การใช้ timezone บนระบบปฏิบัติการบางตัวอาจมีปัญหา หากพบปัญหาให้ใช้คำสั่ง `set timezone` เพื่อกำหนดเขตเวลาที่ถูกต้อง
- บางคำสั่งอาจยังไม่ทำงานในสภาพแวดล้อมการใช้งานจริง เนื่องจากการพัฒนายังไม่เสร็จสมบูรณ์

## การอัปเดตล่าสุด

- เพิ่มระบบการจัดการข้อผิดพลาดขั้นสูงและชุดทดสอบขั้นสูง
- เพิ่มคำสั่ง predict สำหรับการทำนายผลลัพธ์จากโมเดลที่ฝึกแล้ว
- พัฒนาระบบการตั้งค่าพารามิเตอร์อัตโนมัติและเอกสารประกอบ
- เพิ่มการรองรับการทำงานกับหลายบรรทัดและไวยากรณ์ที่ซับซ้อนขึ้น
- เพิ่มการรองรับ timezone สำหรับการบันทึกเวลาในไฟล์โมเดล
- ปรับปรุงการจัดระเบียบตัวอย่างเป็นหมวดหมู่ (ML, DL, RL)
- แก้ไขบั๊กในการแสดงผลข้อความในโหมดโต้ตอบ
- ปรับปรุงเอกสารอ้างอิงและคู่มือการใช้งาน
