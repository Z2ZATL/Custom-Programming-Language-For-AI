
# AI Language - คู่มือไวยากรณ์

เอกสารนี้อธิบายไวยากรณ์และคำสั่งทั้งหมดที่สามารถใช้ในภาษา AI Language

## โครงสร้างพื้นฐาน

- คำสั่งแต่ละคำสั่งอยู่บนบรรทัดของตัวเอง
- ข้อความที่ขึ้นต้นด้วย `#` ถือเป็นคอมเมนต์ (จะไม่ถูกประมวลผล)
- คำสั่งส่วนใหญ่มีรูปแบบ `<คำสั่ง> <สิ่งที่จะทำ> <พารามิเตอร์>`
- ข้อความใน `"` (เครื่องหมายคำพูด) จะถูกตีความเป็น string
- สามารถใช้คำสั่งหลายบรรทัดได้โดยใช้ `;;` เป็นตัวจบคำสั่ง

## ลำดับการทำงานหลัก

1. **เริ่มต้น** - ใช้คำสั่ง `start`
2. **กำหนดประเภท AI** - ใช้คำสั่ง `create ML/DL/RL`
3. **โหลดข้อมูล** - ใช้คำสั่ง `load dataset`
4. **จัดการข้อมูล** - ใช้คำสั่ง `preprocess` และ `split dataset`
5. **สร้างโมเดล** - ใช้คำสั่ง `create model`
6. **กำหนดพารามิเตอร์** - ใช้คำสั่ง `set`
7. **ฝึกโมเดล** - ใช้คำสั่ง `train model`
8. **ประเมินผล** - ใช้คำสั่ง `evaluate model`
9. **บันทึกโมเดล** - ใช้คำสั่ง `save model`
10. **จบการทำงาน** - ใช้คำสั่ง `end`

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

การโหลดโมเดลที่บันทึกไว้แล้ว:
```
load model "<ชื่อไฟล์>"
```
โปรแกรมจะเพิ่มนามสกุลที่เหมาะสมตามประเภท AI อัตโนมัติถ้าไม่ได้ระบุ

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
- `discount_factor` - ค่าส่วนลดในอนาคต (สำหรับ RL) หรือ `gamma`
- `exploration_rate` - อัตราการสำรวจ (สำหรับ RL) หรือ `epsilon`
- `timezone` - กำหนดเขตเวลา (เช่น 7 สำหรับไทย, 9 สำหรับญี่ปุ่น)

สำหรับ Deep Learning เพิ่มเติม:
```
add layer <ประเภท> <พารามิเตอร์>
```
ประเภท layer ที่รองรับ:
- `input <จำนวนโหนด>` - Input layer
- `hidden <จำนวนโหนด> activation "<ฟังก์ชันกระตุ้น>"` - Hidden layer
  - ฟังก์ชันกระตุ้น: "relu", "sigmoid", "tanh", "leaky_relu"
- `output <จำนวนโหนด> activation "<ฟังก์ชันกระตุ้น>"` - Output layer
  - ฟังก์ชันกระตุ้น: "softmax", "sigmoid", "linear"
- `dropout <อัตรา>` - Dropout layer (อัตรา 0.0-1.0)
- `conv <จำนวนฟิลเตอร์> kernel_size <ขนาด> activation "<ฟังก์ชันกระตุ้น>"` - Convolutional layer
- `pool <ขนาด> type "<ประเภท>"` - Pooling layer (ประเภท: "max", "avg")
- `flatten` - Flatten layer

### 6. จัดการข้อมูล (Data Preprocessing)
```
preprocess data <operation>
```
การดำเนินการที่รองรับ:
- `normalize` - ปรับสเกลข้อมูลให้อยู่ในช่วง 0-1
- `standardize` - ปรับให้ข้อมูลมีค่าเฉลี่ย 0 และความแปรปรวน 1
- `fill_missing "mean"` - เติมข้อมูลที่ขาดหายด้วยค่าเฉลี่ย
- `fill_missing "median"` - เติมข้อมูลที่ขาดหายด้วยค่ามัธยฐาน
- `fill_missing "mode"` - เติมข้อมูลที่ขาดหายด้วยค่าฐานนิยม
- `fill_missing "value" <ค่า>` - เติมข้อมูลที่ขาดหายด้วยค่าที่กำหนด
- `one_hot_encode` - แปลงข้อมูลตัวแปรจัดกลุ่มเป็น one-hot encoding
- `remove_outliers` - กำจัดค่าผิดปกติ

```
split dataset <train_ratio> <test_ratio> [<validation_ratio>]
```
แบ่งข้อมูลเป็นชุดฝึก ชุดทดสอบ และชุดตรวจสอบ (ถ้ากำหนด)

ตัวอย่าง:
```
split dataset 0.7 0.2 0.1  # train 70%, test 20%, validation 10%
```

### 7. ฝึกโมเดล
```
train model
```

```
cross_validate <folds>
```
ทำ cross-validation เพื่อประเมินโมเดล โดย folds คือจำนวนส่วนที่จะแบ่งข้อมูล (เช่น 5, 10)

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
- `weights` - ดูค่าน้ำหนักของโมเดล (สำหรับ ML และ DL)
- `gradients` - ดูค่า gradients ในการเทรนล่าสุด (สำหรับ DL)
- `layers` - ดูรายละเอียดของแต่ละ layer (สำหรับ DL)
- `q_table` - ดูค่า Q-table (สำหรับ RL)
- `policy` - ดูนโยบายการตัดสินใจ (สำหรับ RL)

### 9. แสดงผลลัพธ์
```
show <ประเภทผลลัพธ์>
```
ประเภทผลลัพธ์ที่รองรับ:
- `accuracy` - ความแม่นยำ
- `loss` - ค่าความสูญเสีย
- `precision` - ความแม่นยำเชิงบวก
- `recall` - ความไวในการตรวจจับ
- `f1_score` - ค่า F1 (ค่าเฉลี่ยฮาร์โมนิกของค่า precision และ recall)
- `confusion_matrix` - ตารางความสับสน
- `rmse` - ค่า Root Mean Squared Error (สำหรับ Regression)
- `r2` - ค่า R-squared (สำหรับ Regression)
- `performance` - ประสิทธิภาพ (สำหรับ RL)
- `rewards` - ผลตอบแทน (สำหรับ RL)

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
- `feature_importance` - กราฟแสดงความสำคัญของคุณลักษณะ (สำหรับโมเดลบางประเภท)
- `confusion_matrix` - กราฟแสดงตารางความสับสน
- `roc_curve` - กราฟแสดง ROC curve
- `reward_history` - กราฟแสดงประวัติผลตอบแทน (สำหรับ RL)
- `q_values` - กราฟแสดงค่า Q-values (สำหรับ RL)

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

ตัวอย่าง:
```
save model "my_model"  # จะบันทึกเป็น my_model.mlmodel (สำหรับ ML)
```

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

```
reset environment
```
รีเซ็ตสภาพแวดล้อมเป็นค่าเริ่มต้น

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

```
set seed <value>
```
กำหนดค่าเริ่มต้นสำหรับการสุ่ม เพื่อให้ผลลัพธ์สามารถทำซ้ำได้

### 17. การใช้งานคำสั่งหลายบรรทัด

คุณสามารถเขียนคำสั่งหลายบรรทัดได้โดยใช้ `;;` เป็นตัวจบคำสั่ง:

```
create model NeuralNetwork ;;
add layer input 4 ;;
add layer hidden 8 activation "relu" ;;
add layer output 1 activation "sigmoid"
```

### 18. สิ้นสุดการทำงาน
```
end
```

## การตั้งค่าอัตโนมัติ

หากไม่มีการตั้งค่าพารามิเตอร์ด้วยคำสั่ง `set` โปรแกรมจะใช้ค่าพื้นฐานตามประเภทโมเดล:

| AI Type | Learning Rate | Epochs | Batch Size | อื่นๆ |
|---------|--------------|--------|------------|-----|
| ML | 0.01 | 50 | 32 | - |
| DL | 0.001 | 100 | 32 | - |
| RL | 0.1 | - | - | episodes: 1000, discount_factor: 0.9, exploration_rate: 0.3 |

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

## ประโยชน์ของการตั้งค่าเขตเวลา

- ช่วยให้ผู้ใช้จากหลายภูมิภาคสามารถบันทึกเวลาในรูปแบบที่พวกเขาคุ้นเคย
- ทำให้การเปรียบเทียบเวลาการบันทึกโมเดลถูกต้องตามเขตเวลาท้องถิ่น
- เป็นประโยชน์เมื่อต้องทำงานร่วมกันในทีมที่อยู่คนละเขตเวลา

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
set batch_size 64
train model
evaluate model
show accuracy
save model "mnist_model"
end
```

### ตัวอย่างการใช้งาน RL
```
start
create RL
load environment "datasets/environment.json"
create model QLearning
set discount_factor 0.9
set episodes 1000
set exploration_rate 0.3
train model
evaluate model
plot model reward_history "graphs/rl_learning_curve.png"
save model "rl_model"
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
add layer hidden 64 activation "relu" ;;
add layer output 10 activation "softmax" ;;
set learning_rate 0.001 ;;
set epochs 20 ;;
set batch_size 64 ;;
train model ;;
evaluate model ;;
save model "mnist_model"
end
```

### ตัวอย่างการใช้งานกับ Timezone
```
start
create ML
set timezone 7  # ประเทศไทย
load dataset "datasets/iris.csv"
create model LogisticRegression
train model
# เมื่อบันทึกโมเดล เวลาจะถูกบันทึกในรูปแบบเขตเวลาของประเทศไทย (UTC+7)
save model "thai_timezone_model"
end
```
