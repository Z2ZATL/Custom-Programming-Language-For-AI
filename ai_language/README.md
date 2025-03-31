# AI Language

ภาษาโปรแกรมสำหรับการพัฒนาโมเดล AI ที่ใช้ไวยากรณ์ที่เข้าใจง่ายและเป็นธรรมชาติ ออกแบบมาให้ผู้ใช้สามารถสร้างและฝึกโมเดล AI ได้อย่างรวดเร็วและมีประสิทธิภาพ

## ลักษณะเด่น

- **ไวยากรณ์ที่เป็นธรรมชาติ**: ใช้คำสั่งแบบอังกฤษที่อ่านเข้าใจได้อย่างรวดเร็ว
- **รองรับทุกงาน AI**: Machine Learning, Deep Learning, และ Reinforcement Learning
- **ลดความซับซ้อน**: ไม่จำเป็นต้องเขียนโค้ดเชิงเทคนิคมากมาย
- **โค้ดที่กระชับ**: ทำงานที่ซับซ้อนได้ด้วยคำสั่งน้อย
- **คำแนะนำโมเดล**: ช่วยแนะนำโมเดลและพารามิเตอร์ที่เหมาะสม

## วิธีการใช้งาน

1. เขียนคำสั่งในไฟล์ .ai ตามไวยากรณ์ของภาษา
2. รันผ่าน interpreter: `./ai_lang your_file.ai`
3. รับผลลัพธ์และการแสดงผลภาพที่สร้างจากข้อมูลของคุณ

## ตัวอย่างโค้ด

### Machine Learning

```
# เริ่มต้นด้วยการระบุประเภทของโปรเจกต์
start
create ML

# โหลดและเตรียมข้อมูล
load dataset "datasets/linear_data.csv" type "csv"

# สร้างโมเดล
create model LinearRegression
set learning_rate 0.01
set epochs 100

# ฝึกโมเดลและประเมินผล
train model
evaluate model
show accuracy

# บันทึกโมเดล
save model "models/linear_model.mlmodel"
```

### Deep Learning

```
# เริ่มต้นโปรเจกต์ Deep Learning
start
create DL

# โหลดข้อมูลรูปภาพ
load dataset "datasets/images/" type "image"

# สร้างโครงข่ายประสาทเทียม
create model CNN
add layer input 28 28 3
add layer convolutional 32 3 3 activation "relu"
add layer max_pooling 2 2
add layer flatten
add layer hidden 128 activation "relu" 
add layer output 10 activation "softmax"

# ตั้งค่าและฝึกโมเดล
set learning_rate 0.001
set epochs 10
set batch_size 32
train model
show accuracy

# บันทึกโมเดล
save model "models/cnn_model.dlmodel"
```

### Reinforcement Learning

```
# เริ่มต้นโปรเจกต์ Reinforcement Learning
start
create RL

# โหลดสภาพแวดล้อมและตั้งค่า
load dataset "datasets/environment.json"

# สร้างโมเดล Q-Learning
create model QLearning
set discount_factor 0.9
set episodes 1000
set exploration_rate 0.3

# ฝึกและประเมินโมเดล
train model
show performance

# บันทึกโมเดล
save model "models/q_learning_model.rlmodel"
```

## เอกสารอ้างอิง

โปรดดูเอกสารเพิ่มเติมในไฟล์ [SYNTAX_REFERENCE.md](SYNTAX_REFERENCE.md) สำหรับรายละเอียดของไวยากรณ์และคำสั่งทั้งหมด และ [DEVELOPMENT_NOTES.md](DEVELOPMENT_NOTES.md) สำหรับนักพัฒนาที่ต้องการเข้าร่วมโปรเจกต์