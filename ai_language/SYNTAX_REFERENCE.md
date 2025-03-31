
# AI Language - คู่มือไวยากรณ์

เอกสารนี้อธิบายไวยากรณ์และคำสั่งทั้งหมดที่สามารถใช้ในภาษา AI Language

## โครงสร้างพื้นฐาน

- คำสั่งแต่ละคำสั่งอยู่บนบรรทัดของตัวเอง
- ข้อความที่ขึ้นต้นด้วย `#` ถือเป็นคอมเมนต์ (จะไม่ถูกประมวลผล)
- คำสั่งส่วนใหญ่มีรูปแบบ `<คำสั่ง> <สิ่งที่จะทำ> <พารามิเตอร์>`
- ข้อความใน `"` (เครื่องหมายคำพูด) จะถูกตีความเป็น string

## คำสั่งหลัก

### 1. เริ่มต้นโปรเจกต์

```
start
```

### 2. สร้างโปรเจกต์
```
create <ประเภท>
```
ประเภทที่รองรับ: `ML` (Machine Learning), `DL` (Deep Learning), `RL` (Reinforcement Learning)

### 3. โหลดข้อมูล

```
load dataset "<ที่อยู่ไฟล์>"
```
สามารถระบุประเภทไฟล์เพิ่มเติมได้:
```
load dataset "<ที่อยู่ไฟล์>" type "<ประเภทไฟล์>"
```
ประเภทไฟล์ที่รองรับ: `csv`, `json`, `excel`, `image`, `audio`, `text`

สำหรับ Reinforcement Learning:
```
load environment "<ที่อยู่ไฟล์>"
load config "<ที่อยู่ไฟล์>"
```

### 4. สร้างโมเดล

```
create model <ชื่อโมเดล>
```

โมเดลที่รองรับตามประเภท:
- **ML**: `LinearRegression`, `LogisticRegression`, `RandomForest`, `SVM`, `KNN`, `DecisionTree`, `GradientBoosting`
- **DL**: `NeuralNetwork`, `CNN`, `RNN`, `LSTM`, `GRU`, `Transformer`
- **RL**: `QLearning`, `DQN`, `PPO`, `A2C`, `DDQN`

### 5. ปรับแต่งพารามิเตอร์
```
set <ชื่อพารามิเตอร์> <ค่า>
```

พารามิเตอร์หลักที่รองรับ:
- `learning_rate` - อัตราการเรียนรู้
- `epochs` - จำนวนรอบการเทรน
- `batch_size` - ขนาดแบทช์
- `trees` - จำนวนต้นไม้ (สำหรับ RandomForest)
- `max_depth` - ความลึกสูงสุด (สำหรับโมเดลต้นไม้)
- `episodes` - จำนวนเกมส์ (สำหรับ RL)
- `discount_factor` - ค่าส่วนลดในอนาคต (สำหรับ RL)
- `exploration_rate` - อัตราการสำรวจ (สำหรับ RL)

สำหรับ Deep Learning เพิ่มเติม:
```
add layer <ประเภท> <พารามิเตอร์>
```
ประเภท layer: `input`, `hidden`, `output`, `dropout`, `conv`, `pool`, `flatten`

### 6. ฝึกโมเดล
```
train model
```

### 7. แสดงผลลัพธ์
```
show <ประเภทผลลัพธ์>
```
ประเภทผลลัพธ์ที่รองรับ:
- `accuracy` - ความแม่นยำ
- `loss` - ค่าความสูญเสีย
- `graph` - กราฟแสดงผลการเทรน
- `performance` - ประสิทธิภาพ (สำหรับ RL)

### 8. บันทึกและโหลดโมเดล
```
save model "<ที่อยู่ไฟล์>"
load model "<ที่อยู่ไฟล์>"
```

### 9. แสดงผลข้อมูลและโมเดล
```
visualize data
plot model
```

### 10. ประเมินโมเดล
```
evaluate model
```

### 11. สิ้นสุดการทำงาน
```
end
```

## การตั้งค่าอัตโนมัติ

หากไม่มีการตั้งค่าพารามิเตอร์ด้วยคำสั่ง `set` โปรแกรมจะใช้ค่าพื้นฐานตามประเภทโมเดล:

| AI Type | Learning Rate | Epochs | Batch Size | อื่นๆ |
|---------|--------------|--------|------------|-----|
| ML | 0.01 | 50 | 32 | - |
| DL | 0.001 | 100 | 32 | - |
| RL | 0.1 | - | - | episodes: 1000, discount_factor: 0.9 |

## ตัวอย่างแบบสมบูรณ์

ตัวอย่างเพิ่มเติมอยู่ในโฟลเดอร์ `examples/`
- `examples/syntax_guide.ai`
- `examples/auto_parameter_guide.ai`
- `examples/ml_examples/linear_regression.ai`
- `examples/dl_examples/neural_network.ai`
- `examples/rl_examples/q_learning.ai`

## การตั้งค่าเขตเวลา (Timezone)

สามารถตั้งค่าเขตเวลาที่จะใช้ในการบันทึกโมเดลได้ดังนี้:
```
set timezone <ค่า>
```

ค่าที่ใส่คือตัวเลขบวกหรือลบที่แสดงค่า offset จาก UTC 
- ประเทศไทย: `set timezone 7`
- ญี่ปุ่น: `set timezone 9`
- สหราชอาณาจักร: `set timezone 0`
- นิวยอร์ก: `set timezone -5`

ค่านี้จะมีผลในการบันทึกเวลาลงในไฟล์โมเดลเมื่อใช้คำสั่ง `save model`
