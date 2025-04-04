
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

- **ML (.mlmodel)**: พารามิเตอร์ของโมเดล, ประเภทข้อมูล, ผลลัพธ์การฝึกโมเดล
- **DL (.dlmodel)**: โครงสร้าง layer, น้ำหนัก, ค่า bias, ประเภทการกระตุ้น (activation)
- **RL (.rlmodel)**: Q-table, นโยบาย (policy), พารามิเตอร์การสำรวจ

นอกจากนี้ ไฟล์โมเดลทุกประเภทจะบันทึกข้อมูลเมตาดาต้าเหล่านี้:
- วันที่และเวลาที่บันทึกโมเดล (ตามเขตเวลาที่กำหนดด้วย `set timezone`)
- ประเภทของโมเดล (เช่น LinearRegression, NeuralNetwork, QLearning)
- ค่าพารามิเตอร์ที่ใช้ในการฝึกโมเดล
