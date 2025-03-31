
# คู่มือการใช้งานภาษา AI ในโหมดโต้ตอบ

## การเริ่มต้นใช้งาน

เมื่อเริ่มต้นโปรแกรมในโหมดโต้ตอบด้วยคำสั่ง `./ai_lang -i` คุณจะเข้าสู่โหมดการโต้ตอบกับภาษา AI โดยคุณสามารถพิมพ์คำสั่งต่างๆ ได้ทั้งในรูปแบบบรรทัดเดียวหรือหลายบรรทัด

### การใช้คำสั่งหลายบรรทัด

คุณสามารถป้อนคำสั่งหลายบรรทัดได้โดย:
- พิมพ์เครื่องหมาย `\` (backslash) ท้ายบรรทัดแล้วกด Enter เพื่อพิมพ์ต่อในบรรทัดถัดไป
- พิมพ์เครื่องหมาย `;;` เมื่อจบคำสั่งชุดสุดท้ายเพื่อประมวลผลทั้งชุดคำสั่ง

ตัวอย่าง:
```
start \
create ML \
load dataset "datasets/linear_data.csv" \
create model LinearRegression ;;
```

คำสั่งชุดนี้จะทำงานเสมือนคุณได้ป้อนคำสั่งทีละบรรทัด 4 คำสั่งติดต่อกัน

## ลำดับการทำงานของคำสั่ง

ลำดับการทำงานของคำสั่งต้องเป็นไปตามขั้นตอนต่อไปนี้:

1. คำสั่งเริ่มต้น: `start`
2. คำสั่งสร้างโปรเจกต์: `create <ประเภท>`
3. คำสั่งโหลดข้อมูล: `load dataset "<ที่อยู่ไฟล์>"`
4. คำสั่งสร้างโมเดล: `create model <ชื่อโมเดล>`
5. คำสั่งปรับแต่งการเทรน (ไม่บังคับ): `set <พารามิเตอร์> <ค่า>`
6. คำสั่งเทรนโมเดล: `train model`
7. คำสั่งแสดงผลลัพธ์: `show <ประเภทผลลัพธ์>`
8. คำสั่งบันทึกโมเดล: `save model "<ที่อยู่ไฟล์>"`
9. คำสั่งสิ้นสุด: `end` (ไม่บังคับในโหมดโต้ตอบ)

## คำสั่งพื้นฐาน

### คำสั่งเริ่มต้น
```
start
```

### คำสั่งสร้างโปรเจค
```
create ML   # สำหรับ Machine Learning
create DL   # สำหรับ Deep Learning
create RL   # สำหรับ Reinforcement Learning
```

### คำสั่งโหลดข้อมูล
```
load dataset "datasets/linear_data.csv"
load dataset "datasets/iris.csv" type "csv"
load dataset "datasets/images/" type "image"
load dataset "datasets/environment.json"
load model "models/trained_model.mlmodel"
```

### คำสั่งสร้างโมเดล
```
create model LinearRegression
create model RandomForest
create model NeuralNetwork
create model CNN
create model QLearning
```

### คำสั่งกำหนดพารามิเตอร์
```
set learning_rate 0.01
set epochs 100
set batch_size 32
set trees 100
set max_depth 5
set episodes 1000
set discount_factor 0.9
set exploration_rate 0.3
set timezone 7     # ตั้งค่าเขตเวลา UTC+7 (ประเทศไทย)
```

### คำสั่งเกี่ยวกับ Neural Network
```
add layer input 4
add layer hidden 8 activation "relu"
add layer output 3 activation "softmax"
add layer dropout 0.2
```

### คำสั่งฝึกโมเดล
```
train model
```

### คำสั่งวิเคราะห์และแสดงผล
```
evaluate model
show accuracy
show loss
show graph
show performance
visualize data
plot model
```

### คำสั่งบันทึกโมเดล
```
save model "model_name.ml"
```

### คำสั่งโหลดโมเดล
```
load model "model_name.ml"
```

## ตัวอย่างลำดับคำสั่งสำหรับแต่ละประเภท

### Machine Learning - Linear Regression
```
start
create ML
load dataset "datasets/linear_data.csv"
create model LinearRegression
set learning_rate 0.01
set epochs 100
train model
evaluate model
show accuracy
save model "linear_model.ml"
```

### Deep Learning - Neural Network
```
start
create DL
load dataset "datasets/iris.csv"
create model NeuralNetwork
add layer input 4
add layer hidden 8 activation "relu"
add layer output 3 activation "softmax"
set learning_rate 0.001
set epochs 200
train model
evaluate model
show accuracy
save model "neural_network_model.ml"
```

### Reinforcement Learning - Q-Learning
```
start
create RL
load dataset "datasets/environment.json"
create model QLearning
set episodes 1000
set discount_factor 0.9
set exploration_rate 0.3
train model
evaluate model
show accuracy
save model "q_learning_model.rlmodel"
```

## การใช้ค่าพารามิเตอร์อัตโนมัติ

หากไม่มีการตั้งค่าพารามิเตอร์ด้วยคำสั่ง `set` โปรแกรมจะใช้ค่าพื้นฐานตามประเภทโมเดล:

### ค่าอัตโนมัติสำหรับ Machine Learning (ML)
- `learning_rate`: 0.01
- `epochs`: 50
- `batch_size`: 32

### ค่าอัตโนมัติสำหรับ Deep Learning (DL)
- `learning_rate`: 0.001
- `epochs`: 100
- `batch_size`: 32

### ค่าอัตโนมัติสำหรับ Reinforcement Learning (RL)
- `learning_rate`: 0.1
- `episodes`: 1000
- `discount_factor`: 0.9
