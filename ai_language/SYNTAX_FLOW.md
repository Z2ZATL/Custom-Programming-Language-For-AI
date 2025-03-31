
# ลำดับการทำงานของ Syntax ในภาษา AI

## ขั้นตอนการใช้งาน

1. **คำสั่งเริ่มต้น: `start`**
   - เริ่มต้นการทำงานของโปรแกรม
   - ต้องเป็นคำสั่งแรกเสมอ

2. **คำสั่งสร้างโปรเจกต์: `create [type]`**
   - กำหนดประเภทของ AI: `ML`, `DL`, `RL`
   - ต้องเป็นคำสั่งที่สองหลังจาก `start`

3. **คำสั่งโหลดข้อมูล: `load dataset "[filename]" type "[type]"`**
   - โหลดข้อมูลจากไฟล์
   - ต้องใช้หลังจาก `create [type]`

4. **คำสั่งสร้างโมเดล: `create model [model_name]`**
   - สร้างโมเดล AI ตามประเภทที่กำหนด
   - ต้องใช้หลังจาก `load dataset`

5. **คำสั่งปรับแต่งการเทรน (ไม่บังคับ): `set [parameter] [value]`**
   - ปรับแต่งพารามิเตอร์การเทรน
   - ใช้ได้หลังจาก `create model` และก่อน `train model`
   - หากไม่ใช้ โปรแกรมจะตั้งค่าอัตโนมัติตามประเภท AI

6. **คำสั่งเทรนโมเดล: `train model`**
   - เริ่มกระบวนการฝึกโมเดล
   - ต้องใช้หลังจาก `create model`

7. **คำสั่งแสดงผลลัพธ์: `show [result_type]`**
   - แสดงผลลัพธ์ เช่น `show accuracy`, `show loss`, `show graph`
   - ต้องใช้หลังจาก `train model`

8. **คำสั่งบันทึกโมเดล: `save model "[filename]"`**
   - บันทึกโมเดลที่ฝึกแล้วลงไฟล์
   - ต้องใช้หลังจาก `train model`

9. **คำสั่งโหลดโมเดล: `load model "[filename]"`**
   - โหลดโมเดลที่บันทึกไว้
   - ใช้แทน `create model` ได้หลังจาก `load dataset`

10. **คำสั่งสิ้นสุด: `end`** (ไม่บังคับในโหมดโต้ตอบ)
    - สิ้นสุดการทำงานของโปรแกรม
    - เป็นคำสั่งสุดท้าย

## การตั้งค่าอัตโนมัติ

หากไม่ใช้คำสั่ง `set [parameter]` โปรแกรมจะตั้งค่าอัตโนมัติดังนี้:

| AI Type | Learning Rate | Epochs | Batch Size | อื่นๆ |
|---------|--------------|--------|------------|-----|
| ML | 0.01 | 50 | 32 | - |
| DL | 0.001 | 100 | 32 | - |
| RL | 0.1 | - | - | episodes: 1000, discount_factor: 0.9 |

## ตัวอย่างการใช้งาน

ดูตัวอย่างเพิ่มเติมได้ในไฟล์:
- `examples/syntax_guide.ai`
- `examples/auto_parameter_guide.ai`
- `examples/ml_examples/linear_regression.ai`
- `examples/dl_examples/neural_network.ai`
- `examples/rl_examples/q_learning.ai`
