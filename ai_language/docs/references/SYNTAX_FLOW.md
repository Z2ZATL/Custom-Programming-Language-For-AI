
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
      +---------- (start) ----------+
      |                             |
      v                             |
+------------+                      |
| create AI  |                      |
+------------+                      |
      |                             |
      v                             |
+------------+       +------------+ |
| load data  | <---> | load model | |
+------------+       +------------+ |
      |                             |
      v                             |
+------------+                      |
| preprocess |                      |
+------------+                      |
      |                             |
      v                             |
+------------+                      |
| split data |                      |
+------------+                      |
      |                             |
      v                             |
+------------+                      |
|create model|                      |
+------------+                      |
      |                             |
      v                             |
+------------+     +------------+   |
| set params | --> | add layers |   |
+------------+     +------------+   |
      |                             |
      v                             |
+------------+                      |
|train model |                      |
+------------+                      |
      |                             |
      v                             |
+------------+     +------------+   |
| evaluate   | --> | show/plot  |   |
+------------+     +------------+   |
      |                             |
      v                             |
+------------+     +------------+   |
| save model | --> | predict    |   |
+------------+     +------------+   |
      |                             |
      v                             |
+---------- (end) ----------------+
```

## คำสั่งที่สามารถใช้ได้ในแต่ละขั้นตอน

### ก่อน create AI type
- `start` - เริ่มโปรแกรม

### หลัง create AI type
- `load dataset` - โหลดข้อมูล
- `load model` - โหลดโมเดลที่บันทึกไว้ก่อนหน้า
- `set timezone` - กำหนดเขตเวลา
- `set seed` - กำหนดค่าเริ่มต้นสำหรับการสุ่ม

### หลัง load dataset
- `preprocess data` - เตรียมข้อมูล
- `split dataset` - แบ่งข้อมูล
- `visualize data` - แสดงข้อมูลในรูปแบบภาพ
- `create model` - สร้างโมเดล
- `export results` - ส่งออกผลลัพธ์

### หลัง create model
- `set` - กำหนดค่าพารามิเตอร์
- `add layer` - เพิ่ม layer (สำหรับ DL)
- `inspect model` - ตรวจสอบโครงสร้างโมเดล
- `train model` - ฝึกโมเดล
- `schedule training` - กำหนดเวลาการฝึกโมเดล

### หลัง train model
- `evaluate model` - ประเมินประสิทธิภาพโมเดล
- `show` - แสดงค่าต่างๆ
- `plot` - สร้างกราฟ
- `predict` - ทำนายผลลัพธ์
- `save model` - บันทึกโมเดล
- `cross_validate` - ทำ cross-validation
- `validate model` - ตรวจสอบโมเดลด้วยข้อมูลชุดใหม่
- `compare models` - เปรียบเทียบประสิทธิภาพโมเดลหลายตัว
- `export results` - ส่งออกผลลัพธ์

### คำสั่งที่ใช้ได้ตลอดเวลา
- `check status` - ตรวจสอบสถานะ
- `debug on/off` - เปิด/ปิดโหมดดีบัก
- `help` - แสดงคำแนะนำการใช้คำสั่ง
- `list models` - แสดงรายการโมเดลที่มีอยู่
- `delete model` - ลบโมเดล
- `end` - จบโปรแกรม

## การตรวจสอบสถานะและข้อผิดพลาด

เมื่อใช้คำสั่ง `check status` โปรแกรมจะแสดงข้อมูลดังนี้:

1. **ประเภท AI ที่ใช้งานอยู่**: ML, DL, หรือ RL
2. **ข้อมูลที่โหลด**: ชื่อไฟล์, จำนวนตัวอย่าง, จำนวนคุณลักษณะ
3. **การแบ่งข้อมูล**: สัดส่วนการแบ่งข้อมูล (ถ้ามี)
4. **โมเดลที่กำลังใช้งาน**: ประเภทโมเดล, สถานะ (สร้างแล้ว, ฝึกแล้ว)
5. **พารามิเตอร์ที่ตั้งค่า**: พารามิเตอร์ที่กำหนดด้วยคำสั่ง `set`
6. **เขตเวลาที่ใช้**: ค่า timezone ที่กำหนด
7. **จำนวนโมเดลในระบบ**: จำนวนโมเดลที่สร้างหรือโหลด
8. **สถานะโหมดดีบัก**: เปิดหรือปิด

## ข้อควรระวังและข้อจำกัด

1. คำสั่ง `create ML`, `create DL`, หรือ `create RL` ต้องระบุก่อนใช้คำสั่งอื่นๆ ที่เกี่ยวข้องกับการสร้างหรือฝึกโมเดล
2. คำสั่ง `train model` ต้องมีการสร้างโมเดลก่อนด้วยคำสั่ง `create model`
3. คำสั่ง `predict` ต้องมีการฝึกโมเดลก่อนด้วยคำสั่ง `train model` หรือโหลดโมเดลที่ฝึกแล้วด้วย `load model`
4. คำสั่ง `add layer` ใช้ได้เฉพาะกับ `create DL` เท่านั้น
5. คำสั่ง `save model` จะเพิ่มนามสกุลไฟล์ที่เหมาะสมให้อัตโนมัติตามประเภทของโมเดล (.mlmodel, .dlmodel, .rlmodel)
6. คำสั่ง `set timezone` ควรใช้ก่อนการบันทึกโมเดลด้วย `save model`
7. คำสั่งหลายบรรทัดต้องจบด้วย `;;` ยกเว้นบรรทัดสุดท้าย

## การจัดการข้อผิดพลาด

โปรแกรมจะตรวจสอบลำดับขั้นตอนการทำงานและแสดงข้อผิดพลาดในกรณีต่อไปนี้:

- ไม่มีคำสั่ง `start` ในตอนเริ่มต้น
```
Error: Missing 'start' command at the beginning of the script.
```

- ใช้คำสั่งที่ต้องมีโมเดลก่อนที่จะมีการสร้างโมเดล
```
Error: No model created or loaded. Use 'create model' or 'load model' first.
```

- ลำดับคำสั่งไม่ถูกต้อง (เช่น `train model` ก่อน `create model`)
```
Error: Cannot train model before creating one. Use 'create model' first.
```

- พารามิเตอร์ไม่ถูกต้องหรือไม่ครบถ้วน
```
Error: Invalid parameter for 'split dataset'. Format should be 'split dataset <train_ratio> <test_ratio>'.
```

- ใช้คำสั่งไม่ตรงกับประเภท AI
```
Error: 'add layer' command is only available for Deep Learning. Use 'create DL' first.
```

## ตัวอย่างการใช้งานที่ถูกต้อง

### ตัวอย่าง Machine Learning
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

### ตัวอย่าง Deep Learning
```
start
create DL
load dataset "datasets/mnist.csv"
preprocess data normalize
split dataset 0.7 0.3
create model CNN
add layer input 784
add layer conv 32 kernel_size 3 activation "relu"
add layer pool 2 type "max"
add layer flatten
add layer hidden 128 activation "relu"
add layer output 10 activation "softmax"
set learning_rate 0.001
set epochs 20
train model
evaluate model
save model "mnist_model"
end
```

โปรแกรมจะบันทึกโมเดลเป็นไฟล์ `mnist_model.dlmodel` โดยอัตโนมัติ

### ตัวอย่าง Reinforcement Learning
```
start
create RL
load environment "datasets/environment.json"
create model QLearning
set discount_factor 0.9
set episodes 1000
train model
evaluate model
save model "q_learning_model"
end
```

โปรแกรมจะบันทึกโมเดลเป็นไฟล์ `q_learning_model.rlmodel` โดยอัตโนมัติ

### ตัวอย่างการใช้หลายโมเดลพร้อมกัน
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
save model "logistic_model"

create model RandomForest
set trees 100
train model
evaluate model
save model "forest_model"

# เปรียบเทียบโมเดล
compare models

end
```

### ตัวอย่างการใช้งานคำสั่งหลายบรรทัด
```
start
create DL ;;
load dataset "datasets/mnist.csv" ;;
preprocess data normalize ;;
split dataset 0.7 0.3 ;;
create model NeuralNetwork ;;
add layer input 784 ;;
add layer hidden 128 activation "relu" ;;
add layer output 10 activation "softmax" ;;
set learning_rate 0.001 ;;
set epochs 20 ;;
train model ;;
evaluate model ;;
save model "mnist_model"
end
```
