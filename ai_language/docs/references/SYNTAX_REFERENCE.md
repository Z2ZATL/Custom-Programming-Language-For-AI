
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
- `timezone` - กำหนดเขตเวลา (เช่น 7 สำหรับไทย, 9 สำหรับญี่ปุ่น)

สำหรับ Deep Learning เพิ่มเติม:
```
add layer <ประเภท> <พารามิเตอร์>
```
ประเภท layer: `input`, `hidden`, `output`, `dropout`, `conv`, `pool`, `flatten`

### 6. จัดการข้อมูล (Data Preprocessing)
```
preprocess data <operation>
```
การดำเนินการที่รองรับ:
- `normalize` - ปรับสเกลข้อมูลให้อยู่ในช่วง 0-1
- `standardize` - ปรับให้ข้อมูลมีค่าเฉลี่ย 0 และความแปรปรวน 1
- `fill_missing "mean"` - เติมข้อมูลที่ขาดหายด้วยค่าเฉลี่ย

```
split dataset <train_ratio> <test_ratio>
```
แบ่งข้อมูลเป็นชุดฝึกและชุดทดสอบ

### 7. ฝึกโมเดล
```
train model
```

```
cross_validate <folds>
```
ทำ cross-validation เพื่อประเมินโมเดล

### 8. ประเมินและตรวจสอบโมเดล
```
evaluate model
```

```
validate model "<ที่อยู่ไฟล์>"
```
ตรวจสอบโมเดลด้วยชุดข้อมูลใหม่ (validation set)

```
inspect model <model_name> <option>
```
ตรวจสอบรายละเอียดของโมเดล โดย option ที่รองรับ:
- `structure` - ดูโครงสร้างของโมเดล
- `parameters` - ดูพารามิเตอร์ที่ตั้งค่าไว้
- `performance` - ดูผลลัพธ์ เช่น accuracy, loss

### 9. แสดงผลลัพธ์
```
show <ประเภทผลลัพธ์>
```
ประเภทผลลัพธ์ที่รองรับ:
- `accuracy` - ความแม่นยำ
- `loss` - ค่าความสูญเสีย
- `performance` - ประสิทธิภาพ (สำหรับ RL)

### 10. ทำนายด้วยโมเดล
```
predict "<input_data>"
```
ใช้โมเดลที่ฝึกแล้วเพื่อทำนายผลลัพธ์จากข้อมูลใหม่

```
predict dataset "<filename>"
```
ทำนายผลลัพธ์จากชุดข้อมูลใหม่

### 11. สร้างกราฟและการแสดงผล
```
plot model <plot_type> "<output_path>"
```
สร้างกราฟจากโมเดลและบันทึกเป็นไฟล์

ตัวเลือก plot_type:
- `learning_curve` - กราฟแสดงการเปลี่ยนแปลงของ accuracy/loss
- `structure` - กราฟแสดงโครงสร้างของโมเดล
- `prediction` - กราฟเปรียบเทียบค่าจริงและค่าที่ทำนาย

```
plot custom <x_axis> <y_axis> "<output_path>"
```
สร้างกราฟที่กำหนดเองโดยระบุแกน x และ y

```
visualize data
```
แสดงข้อมูลในรูปแบบที่เข้าใจง่าย

### 12. บันทึกและโหลดโมเดล
```
save model "<ชื่อไฟล์>"
```
บันทึกโมเดลลงไฟล์ โปรแกรมจะเพิ่มนามสกุลที่เหมาะสมตามประเภท AI อัตโนมัติ (ML: .mlmodel, DL: .dlmodel, RL: .rlmodel)

```
load model "<ชื่อไฟล์>"
```
โหลดโมเดลจากไฟล์ โปรแกรมจะเพิ่มนามสกุลที่เหมาะสมตามประเภท AI อัตโนมัติถ้าไม่ได้ระบุ

### 13. จัดการโมเดล
```
list models
```
แสดงรายการโมเดลที่สร้างหรือโหลด

```
delete model <model_name>
```
ลบโมเดลออกจากโปรเจกต์

```
compare models
```
เปรียบเทียบประสิทธิภาพของโมเดลหลายตัว

### 14. การจัดการสภาพแวดล้อม (สำหรับ RL)
```
create environment "<env_name>"
```
สร้างสภาพแวดล้อมใหม่สำหรับ RL

```
set environment parameter <param> <value>
```
ปรับแต่งพารามิเตอร์ของสภาพแวดล้อม

### 15. การจัดการข้อผิดพลาดและการดีบัก
```
check status
```
ตรวจสอบสถานะปัจจุบันของโปรเจกต์

```
debug on
debug off
```
เปิด/ปิดโหมดดีบัก

### 16. การทำงานขั้นสูง
```
export results "<filename>"
```
ส่งออกผลลัพธ์เป็นไฟล์

```
schedule training "<time>"
```
กำหนดเวลาการฝึกโมเดล (รูปแบบ: "YYYY-MM-DD HH:MM:SS")

### 17. สิ้นสุดการทำงาน
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

## ตัวอย่างแบบสมบูรณ์

ตัวอย่างเพิ่มเติมอยู่ในโฟลเดอร์ `examples/`
- `examples/syntax_guide.ai`
- `examples/auto_parameter_guide.ai`
- `examples/ml_examples/linear_regression.ai`
- `examples/dl_examples/neural_network.ai`
- `examples/rl_examples/q_learning.ai`

### ตัวอย่างการใช้งาน ML
```
start
create ML
load dataset "datasets/iris.csv"
preprocess data normalize
split dataset 0.8 0.2
create model LogisticRegression
set learning_rate 0.01
set epochs 100
train model
evaluate model
plot model learning_curve "graphs/learning_curve.png"
save model "logistic_model"
predict "5.1,3.5,1.4,0.2"
end
```

### ตัวอย่างการใช้งาน DL
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
plot model structure "graphs/network_structure.png"
save model "nn_model"
end
```

### ตัวอย่างการใช้งาน RL
```
start
create RL
load dataset "datasets/environment.json"
create model QLearning
set discount_factor 0.9
set episodes 1000
set exploration_rate 0.3
train model
evaluate model
plot model learning_curve "graphs/rl_learning_curve.png"
save model "rl_model"
end
```
