
# คู่มือการบันทึกและจัดการโมเดล AI

## นามสกุลไฟล์สำหรับโมเดล AI แต่ละประเภท

โมเดล AI ที่เทรนแล้วแต่ละประเภทควรมีนามสกุลไฟล์ที่สะท้อนถึงลักษณะของโมเดลและช่วยให้จัดการได้ง่าย ดังนี้:

| ประเภท | นามสกุลไฟล์ | คำอธิบาย |
|--------|------------|----------|
| **Machine Learning (ML)** | `.mlmodel` | เหมาะสำหรับโมเดลอย่าง Linear Regression หรือ Decision Trees ที่เก็บพารามิเตอร์หรือโครงสร้างพื้นฐานของโมเดล |
| **Deep Learning (DL)** | `.dlmodel` | ใช้กับโมเดล Neural Networks ที่มีโครงสร้างซับซ้อนและน้ำหนัก (weights) จำนวนมาก |
| **Reinforcement Learning (RL)** | `.rlmodel` | เหมาะสำหรับโมเดลเช่น Q-Learning หรือ DQN ที่เก็บสถานะของ agent หรือตาราง Q-values |
| **Natural Language Processing (NLP)** | `.nlpmodel` | ใช้กับโมเดลอย่าง Transformers หรือ BERT ที่มีขนาดใหญ่และพารามิเตอร์มาก |
| **Computer Vision (CV)** | `.cvmodel` | เหมาะสำหรับโมเดลเช่น CNN ที่ใช้ในการรู้จำภาพและมีโครงสร้างซับซ้อน |

## การใช้งานกับ AI Language

เมื่อบันทึกโมเดลในโปรแกรม AI Language สามารถระบุนามสกุลไฟล์ที่เหมาะสมได้ผ่านคำสั่ง `save model`:

```
# สำหรับ Machine Learning
save model "my_linear_regression.mlmodel"

# สำหรับ Deep Learning
save model "my_neural_network.dlmodel"

# สำหรับ Reinforcement Learning
save model "my_qlearning.rlmodel"
```

## การบันทึก Output การโต้ตอบ

สคริปต์ `save_interaction.sh` ใช้สำหรับบันทึก output ของการโต้ตอบกับโปรแกรม AI Language
เพื่อเก็บประวัติการทำงานและผลลัพธ์
