
# คู่มือการใช้งาน AI Language

## บทนำ

AI Language เป็นภาษาที่ออกแบบมาเพื่อทำให้การสร้างและฝึกโมเดล AI เป็นเรื่องง่าย โดยไม่จำเป็นต้องรู้รายละเอียดการเขียนโค้ดแบบดั้งเดิม คู่มือนี้จะแนะนำการใช้งานเบื้องต้นจนถึงการใช้งานขั้นสูง

## การติดตั้ง

คุณสามารถเรียกใช้ AI Language ได้ 2 วิธี:

1. รัน binary ที่ build ไว้แล้ว:
```
./ai_lang <ชื่อไฟล์สคริปต์.ai>
```

2. รันในโหมด interactive:
```
./ai_lang -i
```

### การติดตั้งจากซอร์สโค้ด

หากต้องการติดตั้งจากซอร์สโค้ด ให้ทำตามขั้นตอนดังนี้:

```bash
mkdir -p build
cd build
cmake ..
make
```

หลังจากติดตั้งเสร็จแล้ว จะได้ไฟล์ binary ชื่อ `ai_lang` ที่สามารถรันได้ตามคำแนะนำด้านบน

## โครงสร้างพื้นฐานของภาษา

ไฟล์สคริปต์ AI Language มีนามสกุล `.ai` และประกอบด้วยชุดคำสั่งที่ทำงานไปตามลำดับ โดยโครงสร้างพื้นฐานจะเริ่มจาก `start` และจบด้วย `end`

```
start
# คำสั่งต่างๆ
end
```

## การเลือกประเภท AI

AI Language รองรับการทำงานกับ AI 3 ประเภท:

```
create ML  # Machine Learning
create DL  # Deep Learning
create RL  # Reinforcement Learning
```

คุณต้องเลือกประเภท AI ที่จะใช้งานก่อนที่จะใช้คำสั่งอื่นๆ

## การทำงานกับข้อมูล

### โหลดข้อมูล

```
load dataset "datasets/iris.csv"
load dataset "datasets/images/" type "image"
```

สำหรับ Reinforcement Learning คุณสามารถโหลดสภาพแวดล้อมได้:

```
load environment "datasets/environment.json"
load config "datasets/config.json"
```

### เตรียมข้อมูล

```
preprocess data normalize           # ปรับสเกลข้อมูลให้อยู่ในช่วง 0-1
preprocess data standardize         # ปรับให้ข้อมูลมีค่าเฉลี่ย 0 และความแปรปรวน 1
preprocess data fill_missing "mean" # เติมค่าที่หายไปด้วยค่าเฉลี่ย
preprocess data one_hot_encode      # แปลงตัวแปรจัดกลุ่มเป็น one-hot encoding
preprocess data remove_outliers     # กำจัดค่าผิดปกติ
```

### แบ่งข้อมูล

```
split dataset 0.8 0.2  # training 80%, testing 20%
split dataset 0.7 0.2 0.1  # training 70%, testing 20%, validation 10%
```

## การสร้างและฝึกโมเดล

### สร้างโมเดล

```
create model LinearRegression  # ML
create model NeuralNetwork     # DL
create model QLearning         # RL
```

### กำหนดค่าพารามิเตอร์

```
set learning_rate 0.01
set epochs 100
set batch_size 32
set timezone 7      # กำหนดเขตเวลาเป็น UTC+7 (ประเทศไทย)
set seed 42         # กำหนดค่าเริ่มต้นสำหรับการสุ่ม
```

สำหรับ Reinforcement Learning:

```
set discount_factor 0.9  # หรือ set gamma 0.9
set episodes 1000
set exploration_rate 0.3  # หรือ set epsilon 0.3
```

สำหรับ Deep Learning คุณสามารถเพิ่ม layer ได้:

```
add layer input 4
add layer hidden 8 activation "relu"
add layer dropout 0.2
add layer output 1 activation "sigmoid"
```

สำหรับ CNN:

```
add layer input 28 28 3  # width, height, channels
add layer conv 32 kernel_size 3 activation "relu"
add layer pool 2 type "max"
add layer flatten
add layer hidden 128 activation "relu"
add layer output 10 activation "softmax"
```

### ฝึกโมเดล

```
train model
```

หรือกำหนดเวลาในการฝึก:

```
schedule training "2023-08-15 22:00:00"
```

## การประเมินและแสดงผลลัพธ์

### ประเมินโมเดล

```
evaluate model
cross_validate 5  # 5-fold cross validation
validate model "datasets/validation_data.csv"
```

### แสดงผลลัพธ์

```
show accuracy
show loss
show precision
show recall
show f1_score
show confusion_matrix
show performance  # สำหรับ RL
```

### ตรวจสอบโมเดล

```
inspect model structure
inspect model parameters
inspect model performance
inspect model weights  # สำหรับ ML และ DL
inspect model q_table  # สำหรับ RL
```

### สร้างกราฟ

```
plot model learning_curve "graphs/learning_curve.png"
plot model structure "graphs/model_structure.png"
plot model prediction "graphs/predictions.png"
plot model feature_importance "graphs/feature_importance.png"
plot model confusion_matrix "graphs/confusion_matrix.png"
plot model reward_history "graphs/reward_history.png"  # สำหรับ RL
```

สร้างกราฟที่กำหนดเอง:

```
plot custom "epoch" "accuracy" "graphs/custom_plot.png"
```

## การบันทึกและโหลดโมเดล

### บันทึกโมเดล

```
save model "my_model"
```

โปรแกรมจะเพิ่มนามสกุลไฟล์ที่เหมาะสมให้อัตโนมัติตามประเภทของโมเดล:
- ML: `.mlmodel`
- DL: `.dlmodel`
- RL: `.rlmodel`

### โหลดโมเดล

```
load model "my_model"
```

## การทำนายด้วยโมเดล

```
predict "5.1,3.5,1.4,0.2"
predict dataset "datasets/new_data.csv"
```

## คำสั่งจัดการโมเดล

```
list models               # แสดงรายการโมเดลทั้งหมด
delete model model_name   # ลบโมเดล
compare models            # เปรียบเทียบประสิทธิภาพของโมเดลหลายตัว
check status              # ตรวจสอบสถานะปัจจุบัน
debug on                  # เปิดโหมดดีบัก
debug off                 # ปิดโหมดดีบัก
```

## คำสั่งหลายบรรทัด

คุณสามารถเขียนคำสั่งหลายบรรทัดโดยใช้ `;;` เป็นตัวจบคำสั่ง:

```
create model NeuralNetwork ;;
add layer input 4 ;;
add layer hidden 8 activation "relu" ;;
add layer output 1 activation "sigmoid"
```

## การตั้งค่าเขตเวลา

```
set timezone 7  # ประเทศไทย
set timezone 9  # ญี่ปุ่น
set timezone 0  # สหราชอาณาจักร (UTC)
set timezone -5 # นิวยอร์ก
```

## ตัวอย่างสคริปต์ที่สมบูรณ์

### Machine Learning - Linear Regression

```
start
create ML
load dataset "datasets/linear_data.csv"
preprocess data normalize
split dataset 0.8 0.2
create model LinearRegression
set learning_rate 0.01
set epochs 100
train model
evaluate model
show rmse
show r2
plot model learning_curve "graphs/lr_learning_curve.png"
plot model prediction "graphs/lr_prediction.png"
save model "linear_model"
predict "5.1,3.5,1.4,0.2"
end
```

### Machine Learning - Classification

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
show accuracy
show precision
show recall
show f1_score
show confusion_matrix
plot model learning_curve "graphs/log_learning_curve.png"
plot model confusion_matrix "graphs/confusion_matrix.png"
save model "iris_model"
predict "5.1,3.5,1.4,0.2"
end
```

### Deep Learning - Neural Network

```
start
create DL
load dataset "datasets/mnist.csv"
preprocess data normalize
split dataset 0.7 0.3
create model NeuralNetwork
add layer input 784
add layer hidden 128 activation "relu"
add layer dropout 0.2
add layer hidden 64 activation "relu"
add layer output 10 activation "softmax"
set learning_rate 0.001
set epochs 20
set batch_size 64
train model
evaluate model
show accuracy
show loss
plot model learning_curve "graphs/nn_learning_curve.png"
plot model structure "graphs/nn_structure.png"
save model "mnist_model"
end
```

### Deep Learning - CNN

```
start
create DL
load dataset "datasets/images/" type "image"
preprocess data normalize
split dataset 0.7 0.3
create model CNN
add layer input 28 28 3
add layer conv 32 kernel_size 3 activation "relu"
add layer pool 2 type "max"
add layer conv 64 kernel_size 3 activation "relu"
add layer pool 2 type "max"
add layer flatten
add layer hidden 128 activation "relu"
add layer dropout 0.5
add layer output 10 activation "softmax"
set learning_rate 0.001
set epochs 10
set batch_size 32
train model
evaluate model
show accuracy
save model "cnn_model"
end
```

### Reinforcement Learning - Q-Learning

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
show performance
plot model reward_history "graphs/rl_learning_curve.png"
save model "q_learning_model"
end
```

### Reinforcement Learning - DQN

```
start
create RL
load environment "datasets/environment.json"
create model DQN
add layer input 4
add layer hidden 24 activation "relu"
add layer hidden 24 activation "relu"
add layer output 2 activation "linear"
set learning_rate 0.001
set discount_factor 0.99
set episodes 500
set exploration_rate 1.0
set batch_size 64
train model
evaluate model
show performance
plot model reward_history "graphs/dqn_learning_curve.png"
save model "dqn_model"
end
```

### การใช้งานคำสั่งหลายบรรทัด

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

### การใช้งานกับ Timezone

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

## การแก้ไขปัญหาที่พบบ่อย

### ไม่สามารถโหลดข้อมูลได้

ตรวจสอบว่าไฟล์ข้อมูลอยู่ในตำแหน่งที่ถูกต้อง และมีรูปแบบที่รองรับ (csv, json, excel)

```
load dataset "datasets/iris.csv"  # ตรวจสอบว่าเส้นทางถูกต้อง
```

### การฝึกโมเดลไม่สำเร็จ

ตรวจสอบว่าคุณได้สร้างโมเดลและโหลดข้อมูลก่อนการฝึก:

```
create model LogisticRegression
set learning_rate 0.01  # ลองปรับลดค่า learning rate
set epochs 50           # ลองลดจำนวน epochs
train model
```

### ผลลัพธ์ไม่เป็นไปตามที่คาดหวัง

ลองปรับแต่งพารามิเตอร์หรือเลือกโมเดลที่เหมาะสมกับข้อมูลมากกว่าเดิม:

```
# ลองใช้โมเดลอื่น
create model RandomForest
set trees 100
train model
evaluate model
```

### การทำนายไม่ถูกต้อง

ตรวจสอบว่ารูปแบบข้อมูลที่ใช้ในการทำนายตรงกับข้อมูลที่ใช้ในการฝึก:

```
predict "5.1,3.5,1.4,0.2"  # ตรวจสอบจำนวนและลำดับของค่า
```

## ความช่วยเหลือเพิ่มเติม

หากต้องการข้อมูลเพิ่มเติม สามารถศึกษาได้จากเอกสารอ้างอิงในโฟลเดอร์ `docs/references/`:
- `SYNTAX_REFERENCE.md` - ไวยากรณ์ทั้งหมดของภาษา
- `MODEL_FILE_EXTENSIONS.md` - นามสกุลไฟล์ที่ใช้ในการบันทึกโมเดล
- `SYNTAX_FLOW.md` - ลำดับการทำงานของคำสั่ง

สามารถดูตัวอย่างเพิ่มเติมได้ในโฟลเดอร์ `examples/`:
- `examples/ml_examples/` - ตัวอย่างสำหรับ Machine Learning
- `examples/dl_examples/` - ตัวอย่างสำหรับ Deep Learning
- `examples/rl_examples/` - ตัวอย่างสำหรับ Reinforcement Learning
- `examples/syntax_guide.ai` - คู่มือไวยากรณ์
- `examples/timezone_example.ai` - ตัวอย่างการใช้งานเขตเวลา
