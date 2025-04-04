# ขั้นตอนการทำงานของไวยากรณ์ AI Language

เอกสารนี้อธิบายลำดับการทำงานและการไหลของคำสั่งในภาษา AI Language

## ขั้นตอนพื้นฐาน

ขั้นตอนการทำงานของโปรแกรม AI Language มีดังนี้:

1. **เริ่มต้น** - ทุกสคริปต์ต้องเริ่มต้นด้วยคำสั่ง `start`
2. **กำหนดประเภท AI** - ระบุประเภทของโปรเจกต์ด้วยคำสั่ง `create ML`, `create DL`, หรือ `create RL`
3. **โหลดข้อมูล** - โหลดข้อมูลสำหรับฝึกโมเดลด้วยคำสั่ง `load dataset`
4. **จัดการข้อมูล** - เตรียมข้อมูลด้วยคำสั่ง `preprocess` และ `split dataset`
5. **สร้างโมเดล** - สร้างโมเดลด้วยคำสั่ง `create model` ตามด้วยประเภทโมเดล
6. **กำหนดค่าพารามิเตอร์** - ตั้งค่าพารามิเตอร์ต่างๆ ด้วยคำสั่ง `set`
7. **ฝึกโมเดล** - ฝึกโมเดลด้วยคำสั่ง `train model`
8. **ประเมินผล** - ตรวจสอบผลลัพธ์ด้วยคำสั่ง `evaluate`, `show` หรือ `plot`
9. **บันทึกโมเดล** - บันทึกโมเดลด้วยคำสั่ง `save model`
10. **จบการทำงาน** - จบสคริปต์ด้วยคำสั่ง `end`

## แผนภาพการไหลของคำสั่ง

```
[start] --> [create AI type] --> [load dataset] --> [preprocessing] --> [create model] 
--> [set parameters] --> [train model] --> [evaluate/show/plot] --> [save model] --> [end]
```

## คำสั่งที่สามารถใช้ได้ในแต่ละขั้นตอน

### ก่อน create AI type
- `start` - เริ่มโปรแกรม

### หลัง create AI type
- `load dataset` - โหลดข้อมูล
- `load model` - โหลดโมเดลที่บันทึกไว้ก่อนหน้า
- `set timezone` - กำหนดเขตเวลา

### หลัง load dataset
- `preprocess data` - เตรียมข้อมูล
- `split dataset` - แบ่งข้อมูล
- `visualize data` - แสดงข้อมูลในรูปแบบภาพ
- `create model` - สร้างโมเดล

### หลัง create model
- `set` - กำหนดค่าพารามิเตอร์
- `add layer` - เพิ่ม layer (สำหรับ DL)
- `inspect model` - ตรวจสอบโครงสร้างโมเดล
- `train model` - ฝึกโมเดล

### หลัง train model
- `evaluate model` - ประเมินประสิทธิภาพโมเดล
- `show` - แสดงค่าต่างๆ
- `plot` - สร้างกราฟ
- `predict` - ทำนายผลลัพธ์
- `save model` - บันทึกโมเดล
- `cross_validate` - ทำ cross-validation
- `validate model` - ตรวจสอบโมเดลด้วยข้อมูลชุดใหม่

### คำสั่งที่ใช้ได้ตลอดเวลา
- `check status` - ตรวจสอบสถานะ
- `debug on/off` - เปิด/ปิดโหมดดีบัก
- `end` - จบโปรแกรม

## ข้อควรระวังและข้อจำกัด

1. คำสั่ง `create ML`, `create DL`, หรือ `create RL` ต้องระบุก่อนใช้คำสั่งอื่นๆ ที่เกี่ยวข้องกับการสร้างหรือฝึกโมเดล
2. คำสั่ง `train model` ต้องมีการสร้างโมเดลก่อนด้วยคำสั่ง `create model`
3. คำสั่ง `predict` ต้องมีการฝึกโมเดลก่อนด้วยคำสั่ง `train model` หรือโหลดโมเดลที่ฝึกแล้วด้วย `load model`
4. คำสั่ง `add layer` ใช้ได้เฉพาะกับ `create DL` เท่านั้น
5. คำสั่ง `save model` จะเพิ่มนามสกุลไฟล์ที่เหมาะสมให้อัตโนมัติตามประเภทของโมเดล (.mlmodel, .dlmodel, .rlmodel)

## การจัดการข้อผิดพลาด

โปรแกรมจะตรวจสอบลำดับขั้นตอนการทำงานและแสดงข้อผิดพลาดในกรณีต่อไปนี้:

- ไม่มีคำสั่ง `start` ในตอนเริ่มต้น
- ใช้คำสั่งที่ต้องมีโมเดลก่อนที่จะมีการสร้างโมเดล
- ลำดับคำสั่งไม่ถูกต้อง (เช่น `train model` ก่อน `create model`)
- พารามิเตอร์ไม่ถูกต้องหรือไม่ครบถ้วน

## ตัวอย่างการใช้งานที่ถูกต้อง

```
start
create ML
load dataset "datasets/iris.csv"
preprocess data normalize
split dataset 0.8 0.2
create model LogisticRegression
set learning_rate 0.01
train model
evaluate model
save model "iris_model"
end
```

โปรแกรมจะบันทึกโมเดลเป็นไฟล์ `iris_model.mlmodel` โดยอัตโนมัติ