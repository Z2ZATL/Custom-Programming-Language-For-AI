
# ตัวอย่างการใช้งานภาษา AI

โฟลเดอร์นี้ประกอบด้วยตัวอย่างสคริปต์ภาษา AI สำหรับการใช้งานในด้านต่างๆ

## คู่มือการทำงาน

- `syntax_guide.ai` - คู่มือแสดงลำดับการทำงานของไวยากรณ์ทั้งหมด
- `auto_parameter_guide.ai` - ตัวอย่างการใช้งานแบบให้โปรแกรมตั้งค่าอัตโนมัติ

## โครงสร้างโฟลเดอร์

- `ml_examples/` - ตัวอย่างสำหรับ Machine Learning
  - `linear_regression.ai` - การสร้างโมเดล Linear Regression
  - `classification.ai` - การสร้างโมเดลสำหรับ Classification
  
- `dl_examples/` - ตัวอย่างสำหรับ Deep Learning
  - `neural_network.ai` - การสร้างโมเดล Neural Network พื้นฐาน
  - `cnn.ai` - การสร้างโมเดล Convolutional Neural Network
  
- `rl_examples/` - ตัวอย่างสำหรับ Reinforcement Learning
  - `q_learning.ai` - การสร้างโมเดล Q-Learning
  - `dqn.ai` - การสร้างโมเดล Deep Q-Network

## ลำดับการทำงานของคำสั่ง

ทุกไฟล์ตัวอย่างจะปฏิบัติตามลำดับการทำงานเดียวกัน:

1. คำสั่งเริ่มต้น: `start`
2. คำสั่งสร้างโปรเจกต์: `create <ประเภท>`
3. คำสั่งโหลดข้อมูล: `load dataset "<ที่อยู่ไฟล์>"`
4. คำสั่งสร้างโมเดล: `create model <ชื่อโมเดล>`
5. คำสั่งปรับแต่งการเทรน (ไม่บังคับ): `set <พารามิเตอร์> <ค่า>`
6. คำสั่งเทรนโมเดล: `train model`
7. คำสั่งแสดงผลลัพธ์: `show <ประเภทผลลัพธ์>`
8. คำสั่งบันทึกโมเดล: `save model "<ที่อยู่ไฟล์>"`
