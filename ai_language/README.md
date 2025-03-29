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
start create ML

# โหลดและเตรียมข้อมูล
load dataset "data.csv" type "csv"
clean data with drop_na true
split data into train, test with ratio 0.8, 0.2

# สร้างโมเดล
create model LinearRegression with normalize true
train model on train_data with epochs 100

# ประเมินโมเดล
evaluate model on test_data
show r2_score
```

### Deep Learning

```
# เริ่มต้นโปรเจกต์ Deep Learning
start create DL

# โหลดข้อมูลรูปภาพ
load dataset "images/" type "image"

# สร้างโครงข่ายประสาทเทียม
create neural_network CNN
add layer convolutional filters 32 kernel_size 3x3 activation "relu"
add layer max_pooling size 2x2
add layer flatten
add layer dense nodes 128 activation "relu"
add layer dense nodes 10 activation "softmax"

# คอมไพล์และฝึกโมเดล
compile model with optimizer "adam" loss "categorical_crossentropy"
train model with epochs 10 batch_size 32
```

### Reinforcement Learning

```
# เริ่มต้นโปรเจกต์ Reinforcement Learning
start create RL

# สร้างสภาพแวดล้อมและ agent
create environment "CartPole-v1"
create agent DQN

# ตั้งค่าและฝึก agent
configure agent with gamma 0.99 epsilon 1.0
train agent for episodes 1000
```

## เอกสารอ้างอิง

โปรดดูเอกสารเพิ่มเติมในไฟล์ [SYNTAX_REFERENCE.md](SYNTAX_REFERENCE.md) สำหรับรายละเอียดของไวยากรณ์และคำสั่งทั้งหมด และ [DEVELOPMENT_NOTES.md](DEVELOPMENT_NOTES.md) สำหรับนักพัฒนาที่ต้องการเข้าร่วมโปรเจกต์