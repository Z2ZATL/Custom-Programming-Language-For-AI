
# บันทึก Output จากโปรแกรม AI Language

ไฟล์ในโฟลเดอร์นี้จะเก็บผลลัพธ์จากการใช้งานโปรแกรม AI Language ในโหมดโต้ตอบ

## วิธีใช้งาน

1. รันสคริปต์ `save_interaction.sh` เพื่อบันทึก output:
   ```
   bash "Program test/save_interaction.sh"
   ```

2. ไฟล์ output จะถูกสร้างในรูปแบบ `interaction_output_YYYYMMDD_HHMMSS.txt`

3. เมื่อใช้งานโปรแกรมเสร็จสิ้น สามารถพิมพ์ `exit` เพื่อออกจากโหมดโต้ตอบ

## ตัวอย่างคำสั่ง

```
start
create RL
load dataset "datasets/environment.json" type "json"
create model QLearning
train model
save model "my_model.ml"
```
