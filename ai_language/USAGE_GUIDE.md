
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
load dataset "datasets/linear_data.csv" type "csv"
load dataset "datasets/iris.csv" type "csv"
load dataset "datasets/images/" type "image"
load environment "datasets/environment.json"
load config "datasets/config.json"
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
```

### คำสั่งเกี่ยวกับ Neural Network
```
add layer input 4
add layer hidden 8 activation "relu"
add layer output 3 activation "softmax"
```

### คำสั่งฝึกโมเดล
```
train model
```

### คำสั่งวิเคราะห์และแสดงผล
```
evaluate model
show accuracy
show performance
visualize data
visualize environment
plot model
```

### คำสั่งบันทึกโมเดล
```
save model "model_name.dat"
```

## ตัวอย่างลำดับคำสั่งสำหรับแต่ละประเภท

### Machine Learning - Linear Regression
```
start
create ML
load dataset "datasets/linear_data.csv" type "csv"
create model LinearRegression
set learning_rate 0.01
set epochs 100
train model
evaluate model
show accuracy
save model "linear_model.dat"
```

### Deep Learning - Neural Network
```
start
create DL
load dataset "datasets/iris.csv" type "csv"
create model NeuralNetwork
add layer input 4
add layer hidden 8 activation "relu"
add layer output 3 activation "softmax"
set learning_rate 0.001
set epochs 200
train model
evaluate model
show accuracy
save model "neural_network_model.dat"
```

### Reinforcement Learning - Q-Learning
```
start
create RL
load environment "datasets/environment.json"
load config "datasets/config.json"
create model QLearning
set episodes 1000
train model
evaluate model
show performance
save model "q_learning_model.dat"
```
