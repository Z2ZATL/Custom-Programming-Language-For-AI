
# การเชื่อมต่อ AI Language กับ scikit-learn

เอกสารนี้อธิบายวิธีการเชื่อมต่อ AI Language กับไลบรารี scikit-learn ของ Python เพื่อให้สามารถใช้งานโมเดล Machine Learning ที่มีประสิทธิภาพสูงได้

## ข้อกำหนดเบื้องต้น

1. ติดตั้ง Python 3.6 หรือสูงกว่า
2. ติดตั้ง scikit-learn และ dependencies ที่จำเป็น:

```
pip install scikit-learn numpy pandas matplotlib
```

## การใช้งาน

### 1. เปิดใช้งาน scikit-learn ในโค้ด AI Language

ให้เพิ่มคำสั่ง `set use_scikit true` หลังจากสร้างโมเดลในโค้ด AI Language ของคุณ:

```
# ตัวอย่างการใช้ scikit-learn
start create ML
load dataset "datasets/classification_data.csv" type "csv"
create model RandomForest
set trees 100
set max_depth 5
set use_scikit true
train model
show accuracy
save model to "scikit_model.pkl"
```

### 2. โมเดลที่รองรับ

โมเดลที่รองรับในการเชื่อมต่อกับ scikit-learn:

- RandomForest
- LinearRegression
- LogisticRegression
- SVM

### 3. การดูผลลัพธ์

หลังจากเทรนโมเดลด้วย scikit-learn คุณจะได้ผลลัพธ์ดังนี้:

- ความแม่นยำของโมเดล (Accuracy)
- ความแม่นยำแบบละเอียด (Precision)
- ความไว (Recall)
- F1 Score
- กราฟแสดงข้อมูลที่บันทึกไว้ในไฟล์ PNG

### 4. การบันทึกและโหลดโมเดล

โมเดลที่เทรนด้วย scikit-learn จะถูกบันทึกในรูปแบบไฟล์ pickle (.pkl) ซึ่งคุณสามารถโหลดกลับมาใช้งานได้ในภายหลัง

## ตัวอย่าง

ดูตัวอย่างการใช้งานได้ที่:

- `examples/ml_examples/scikit_integration.ai` - ตัวอย่างการใช้งานผ่านภาษา AI Language
- `examples/ml_examples/scikit_learn_demo.py` - ตัวอย่างการใช้งาน scikit-learn โดยตรงผ่าน Python

## การแก้ไขปัญหา

หากพบปัญหาในการใช้งาน scikit-learn กับ AI Language:

1. ตรวจสอบให้แน่ใจว่าคุณได้ติดตั้ง Python และ scikit-learn อย่างถูกต้อง
2. ทดสอบการทำงานของ scikit-learn โดยตรงด้วยการรันไฟล์ `examples/ml_examples/scikit_learn_demo.py`
3. ตรวจสอบโครงสร้างข้อมูลที่ใช้ว่าเข้ากันได้กับโมเดลที่เลือก
