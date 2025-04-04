
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

### เตรียมข้อมูล

```
preprocess data normalize
preprocess data standardize
```

### แบ่งข้อมูล

```
split dataset 0.8 0.2  # training 80%, testing 20%
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
```

สำหรับ Deep Learning คุณสามารถเพิ่ม layer ได้:

```
add layer input 4
add layer hidden 8 activation "relu"
add layer output 1 activation "sigmoid"
```

### ฝึกโมเดล

```
train model
```

## การประเมินและแสดงผลลัพธ์

### ประเมินโมเดล

```
evaluate model
```

### แสดงผลลัพธ์

```
show accuracy
show loss
```

### สร้างกราฟ

```
plot model learning_curve "graphs/learning_curve.png"
plot model structure "graphs/model_structure.png"
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
list models
delete model <model_name>
compare models
```

## การตั้งค่าเขตเวลา

```
set timezone 7  # ประเทศไทย
```

## ตัวอย่างสคริปต์ที่สมบูรณ์

### Machine Learning

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
plot model learning_curve "graphs/learning_curve.png"
save model "iris_model"
predict "5.1,3.5,1.4,0.2"
end
```

### Deep Learning

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

### Reinforcement Learning

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
show performance
save model "q_learning_model"
end
```

## ความช่วยเหลือเพิ่มเติม

หากต้องการข้อมูลเพิ่มเติม สามารถศึกษาได้จากเอกสารอ้างอิงในโฟลเดอร์ `docs/references/`:
- `SYNTAX_REFERENCE.md` - ไวยากรณ์ทั้งหมดของภาษา
- `MODEL_FILE_EXTENSIONS.md` - นามสกุลไฟล์ที่ใช้ในการบันทึกโมเดล
- `SYNTAX_FLOW.md` - ลำดับการทำงานของคำสั่ง
