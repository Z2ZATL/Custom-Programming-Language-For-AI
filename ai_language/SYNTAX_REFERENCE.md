# AI Language - คู่มือไวยากรณ์

เอกสารนี้อธิบายไวยากรณ์และคำสั่งทั้งหมดที่สามารถใช้ในภาษา AI Language

## โครงสร้างพื้นฐาน

- คำสั่งแต่ละคำสั่งอยู่บนบรรทัดของตัวเอง
- ข้อความที่ขึ้นต้นด้วย `#` ถือเป็นคอมเมนต์ (จะไม่ถูกประมวลผล)
- คำสั่งส่วนใหญ่มีรูปแบบ `<คำสั่ง> <สิ่งที่จะทำ> with <พารามิเตอร์>`
- ข้อความใน `"` (เครื่องหมายคำพูด) จะถูกตีความเป็น string

## คำสั่งหลัก

### 1. เริ่มต้นโปรเจกต์

```
start create <ประเภท>
```
ประเภทที่รองรับ: `ML` (Machine Learning), `DL` (Deep Learning), `RL` (Reinforcement Learning)

### 2. โหลดข้อมูล

```
load dataset "<ที่อยู่ไฟล์>" type "<ประเภทไฟล์>"
```
ประเภทไฟล์ที่รองรับ: `csv`, `json`, `excel`, `image`, `audio`, `text`

### 3. ทำความสะอาดข้อมูล

```
clean data with <พารามิเตอร์>
```
พารามิเตอร์ที่ใช้ได้:
- `drop_na <true/false>` - ลบแถวที่มีค่า NA
- `fill_mean "<ประเภทคอลัมน์>"` - แทนที่ค่าที่หายไปด้วยค่าเฉลี่ย
- `handle_outliers <true/false>` - จัดการกับค่าผิดปกติ

### 4. แบ่งข้อมูล

```
split data into <ชื่อชุดข้อมูล> with ratio <สัดส่วน>
```
ตัวอย่าง:
```
split data into train, test with ratio 0.8, 0.2
split data into train, validation, test with ratio 0.7, 0.15, 0.15
```

### 5. สร้างโมเดล

#### Machine Learning
```
create model <ชื่อโมเดล> with <พารามิเตอร์>
```
โมเดลที่รองรับ: `LinearRegression`, `LogisticRegression`, `RandomForest`, `SVM`, `KNN`, `DecisionTree`, `GradientBoosting`, `XGBoost`

#### Deep Learning
```
create neural_network <ประเภท>
add layer <ประเภท> <พารามิเตอร์>
compile model with <พารามิเตอร์>
```
ประเภทโครงข่าย: `CNN`, `RNN`, `LSTM`, `GRU`, `Transformer`
ประเภท layer: `convolutional`, `max_pooling`, `flatten`, `dense`, `dropout`, `batch_norm`, `lstm`, `gru`, `embedding`, `attention`

#### Reinforcement Learning
```
create environment "<ชื่อสภาพแวดล้อม>"
create agent <ประเภท>
configure agent with <พารามิเตอร์>
```
ประเภท agent: `DQN`, `DDQN`, `A2C`, `PPO`, `TRPO`, `SAC`

### 6. ฝึกโมเดล

#### Machine Learning & Deep Learning
```
train model with <พารามิเตอร์>
train model on <ชุดข้อมูล> with <พารามิเตอร์>
```
พารามิเตอร์ทั่วไป: `epochs`, `batch_size`, `learning_rate`, `validation_data`, `early_stopping`

#### Reinforcement Learning
```
train agent for episodes <จำนวน> <พารามิเตอร์>
```
พารามิเตอร์: `max_steps`, `batch_size`, `target_update`

### 7. ประเมินโมเดล

```
evaluate model on <ชุดข้อมูล>
```

### 8. แสดงผลประเมิน

```
show <เมทริก>
```
เมทริกที่รองรับ:
- Machine Learning: `accuracy`, `precision`, `recall`, `f1_score`, `mae`, `mse`, `rmse`, `r2_score`
- Deep Learning: `confusion_matrix`, `classification_report`
- Reinforcement Learning: `rewards`, `steps`, `success_rate`

### 9. การทำนายและใช้โมเดล

```
predict using model on "<ข้อมูลทดสอบ>"
show prediction
```

### 10. บันทึกและโหลดโมเดล

```
save model to "<ที่อยู่ไฟล์>"
load model from "<ที่อยู่ไฟล์>"
```

### 11. การแสดงผลภาพ

```
visualize <ข้อมูล>
visualize data samples <จำนวน>
plot <เมทริก>
```

## การใช้คำสั่งขั้นสูง

### การเปรียบเทียบโมเดล

```
compare models <ชื่อโมเดลที่จะเปรียบเทียบ> with metric "<เมทริก>"
```

### การปรับแต่งโมเดลอัตโนมัติ

```
optimize model hyperparameters with <พารามิเตอร์>
```

## ตัวอย่างแบบสมบูรณ์

ตัวอย่างเพิ่มเติมอยู่ในโฟลเดอร์ `examples/`