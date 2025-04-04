
# นามสกุลไฟล์โมเดล AI Language

เอกสารนี้อธิบายนามสกุลไฟล์ที่ใช้สำหรับบันทึกโมเดลประเภทต่างๆ ใน AI Language

## นามสกุลไฟล์มาตรฐาน

AI Language ใช้นามสกุลไฟล์เฉพาะสำหรับโมเดลแต่ละประเภทเพื่อให้สามารถระบุได้อย่างชัดเจนว่าเป็นโมเดลประเภทใด:

| ประเภท AI | นามสกุลไฟล์ | ตัวอย่าง |
|-----------|-------------|---------|
| Machine Learning (ML) | `.mlmodel` | `linear_regression.mlmodel` |
| Deep Learning (DL) | `.dlmodel` | `neural_network.dlmodel` |
| Reinforcement Learning (RL) | `.rlmodel` | `q_learning.rlmodel` |

## การบันทึกโมเดล

เมื่อใช้คำสั่ง `save model` โปรแกรมจะเพิ่มนามสกุลไฟล์ที่เหมาะสมให้โดยอัตโนมัติตามประเภทของโมเดล:

```
save model "model_name"
```

ตัวอย่าง:
- ถ้าใช้ `create ML` และ `save model "logistic_model"` ไฟล์ที่ได้จะเป็น `logistic_model.mlmodel`
- ถ้าใช้ `create DL` และ `save model "cnn_model"` ไฟล์ที่ได้จะเป็น `cnn_model.dlmodel`
- ถ้าใช้ `create RL` และ `save model "rl_model"` ไฟล์ที่ได้จะเป็น `rl_model.rlmodel`

## การโหลดโมเดล

เมื่อใช้คำสั่ง `load model` สามารถระบุชื่อไฟล์โดยไม่ต้องมีนามสกุล โปรแกรมจะเติมนามสกุลที่เหมาะสมให้อัตโนมัติตามประเภท AI ที่เลือกด้วยคำสั่ง `create`:

```
load model "model_name"
```

ถ้าระบุนามสกุลไฟล์อยู่แล้ว โปรแกรมจะตรวจสอบว่านามสกุลไฟล์ตรงกับประเภท AI ที่เลือกหรือไม่:

```
load model "model_name.mlmodel"  # เหมาะสำหรับ create ML
load model "model_name.dlmodel"  # เหมาะสำหรับ create DL
load model "model_name.rlmodel"  # เหมาะสำหรับ create RL
```

## ข้อมูลที่บันทึกในไฟล์โมเดล

แต่ละไฟล์โมเดลบันทึกข้อมูลเฉพาะตามประเภทของ AI:

### ML (.mlmodel)
- ประเภทของโมเดล ML (เช่น LinearRegression, RandomForest)
- พารามิเตอร์ของโมเดล (เช่น learning_rate, epochs)
- ค่าสัมประสิทธิ์ (coefficients) หรือน้ำหนัก
- ค่าความแม่นยำและประสิทธิภาพอื่นๆ
- คุณลักษณะ (features) และความสำคัญของแต่ละคุณลักษณะ
- ข้อมูลการปรับแต่ง (preprocessing) ที่ใช้
- เวลาที่บันทึกโมเดลตามเขตเวลาที่กำหนด

ตัวอย่างเนื้อหาไฟล์ .mlmodel:
```
# ML Model saved from AI Language
model_type: RandomForest
learning_rate: 0.01
epochs: 100
trees: 100
max_depth: 10
accuracy: 0.95
create_time: 2023-08-15 14:30:45 (UTC+7)
preprocessing_applied: normalize
features: ["feature1", "feature2", "feature3"]
feature_importance: [0.4, 0.35, 0.25]
```

### DL (.dlmodel)
- ประเภทของโมเดล DL (เช่น NeuralNetwork, CNN)
- โครงสร้าง layer (จำนวน layer, ชนิด, จำนวนโหนด)
- น้ำหนัก (weights) และค่า bias
- ฟังก์ชันกระตุ้น (activation functions) ที่ใช้
- พารามิเตอร์การฝึก (learning_rate, batch_size)
- ประวัติการฝึก (loss และ accuracy)
- เวลาที่บันทึกโมเดลตามเขตเวลาที่กำหนด

ตัวอย่างเนื้อหาไฟล์ .dlmodel:
```
# DL Model saved from AI Language
model_type: NeuralNetwork
learning_rate: 0.001
epochs: 200
batch_size: 32
layers: 4
layer_1: input, 10
layer_2: hidden, 20, activation: "relu"
layer_3: hidden, 10, activation: "relu"
layer_4: output, 1, activation: "sigmoid"
loss: 0.05
accuracy: 0.97
create_time: 2023-08-15 14:30:45 (UTC+7)
```

### RL (.rlmodel)
- ประเภทของโมเดล RL (เช่น QLearning, DQN)
- พารามิเตอร์ (discount_factor, exploration_rate)
- Q-table หรือนโยบาย (policy)
- ประวัติผลตอบแทน (rewards)
- จำนวนเกมส์หรือเอพิโซดที่ฝึก
- สถานะสภาพแวดล้อม
- เวลาที่บันทึกโมเดลตามเขตเวลาที่กำหนด

ตัวอย่างเนื้อหาไฟล์ .rlmodel:
```
# RL Model saved from AI Language
model_type: QLearning
discount_factor: 0.9
episodes: 1000
exploration_rate: 0.3
average_reward: 850.5
environment: "CartPole-v1"
create_time: 2023-08-15 14:30:45 (UTC+7)
```

## ประโยชน์ของการใช้นามสกุลไฟล์เฉพาะ

1. **การระบุประเภทโมเดล**: ทำให้ทราบประเภทของโมเดลได้ทันทีจากนามสกุลไฟล์
2. **การโหลดอัตโนมัติ**: โปรแกรมสามารถเลือกวิธีการโหลดที่เหมาะสมตามนามสกุลไฟล์
3. **การจัดระเบียบ**: ช่วยในการจัดระเบียบไฟล์โมเดลในโครงการใหญ่
4. **ป้องกันข้อผิดพลาด**: ป้องกันการโหลดโมเดลผิดประเภท
5. **สะดวกในการแชร์**: ทำให้ผู้ใช้รายอื่นทราบว่าต้องใช้คำสั่งประเภทใดในการโหลดไฟล์

## การจัดการเมื่อมีนามสกุลไฟล์ไม่ถูกต้อง

หากพยายามโหลดไฟล์ที่มีนามสกุลไม่ตรงกับประเภท AI ที่เลือก โปรแกรมจะแสดงข้อความแจ้งเตือนและให้คำแนะนำ:

1. ถ้าโหลด `.mlmodel` ใน DL หรือ RL:
   ```
   Error: Attempting to load ML model in DL/RL project.
   Suggestion: Use 'create ML' first or use appropriate model file.
   ```

2. ถ้าโหลด `.dlmodel` ใน ML หรือ RL:
   ```
   Error: Attempting to load DL model in ML/RL project.
   Suggestion: Use 'create DL' first or use appropriate model file.
   ```

3. ถ้าโหลด `.rlmodel` ใน ML หรือ DL:
   ```
   Error: Attempting to load RL model in ML/DL project.
   Suggestion: Use 'create RL' first or use appropriate model file.
   ```

## การแปลงนามสกุลไฟล์

AI Language ไม่สนับสนุนการแปลงโมเดลระหว่างประเภท AI ที่แตกต่างกัน เนื่องจากโครงสร้างข้อมูลที่แตกต่างกันมาก อย่างไรก็ตาม ในกรณีที่ต้องการใช้งานโมเดลร่วมกัน ให้ใช้คำสั่ง `export` และ `import` ที่กำหนดรูปแบบเฉพาะ:

```
export model "model_name" format "common"
import model "model_name.common"
```
