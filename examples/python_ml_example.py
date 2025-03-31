
import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LogisticRegression
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score, classification_report
import pickle

# 1. โหลดข้อมูล
print("กำลังโหลดข้อมูล...")
data = pd.read_csv('data.csv')
print(f"โหลดข้อมูลสำเร็จ: {data.shape[0]} แถว, {data.shape[1]} คอลัมน์")

# 2. แยกคุณลักษณะและเป้าหมาย
X = data.iloc[:, :-1]  # ทุกคอลัมน์ยกเว้นคอลัมน์สุดท้าย
y = data.iloc[:, -1]   # คอลัมน์สุดท้าย (label)

# 3. แบ่งข้อมูลเป็นชุดฝึกและชุดทดสอบ
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)
print(f"แบ่งข้อมูล: ชุดฝึก {X_train.shape[0]} แถว, ชุดทดสอบ {X_test.shape[0]} แถว")

# 4. ปรับสเกลข้อมูล
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)
print("ปรับสเกลข้อมูลเรียบร้อย")

# 5. สร้างและฝึกโมเดล
print("\n=== โมเดล Logistic Regression ===")
lr_model = LogisticRegression(max_iter=1000)
lr_model.fit(X_train_scaled, y_train)
print("ฝึกโมเดลเสร็จสิ้น")

# 6. ทำนายผลและประเมินโมเดล
y_pred = lr_model.predict(X_test_scaled)
accuracy = accuracy_score(y_test, y_pred)
print(f"ความแม่นยำ (Accuracy): {accuracy:.4f}")
print("\nรายละเอียดการประเมิน:")
print(classification_report(y_test, y_pred))

# 7. ฝึกโมเดลที่ซับซ้อนมากขึ้น
print("\n=== โมเดล Random Forest ===")
rf_model = RandomForestClassifier(n_estimators=100, random_state=42)
rf_model.fit(X_train_scaled, y_train)
print("ฝึกโมเดลเสร็จสิ้น")

# 8. ประเมินโมเดล Random Forest
rf_pred = rf_model.predict(X_test_scaled)
rf_accuracy = accuracy_score(y_test, rf_pred)
print(f"ความแม่นยำ (Accuracy): {rf_accuracy:.4f}")
print("\nรายละเอียดการประเมิน:")
print(classification_report(y_test, rf_pred))

# 9. บันทึกโมเดลที่ดีที่สุด
if rf_accuracy > accuracy:
    best_model = rf_model
    print("\nบันทึกโมเดล Random Forest...")
else:
    best_model = lr_model
    print("\nบันทึกโมเดล Logistic Regression...")

with open('ml_model.pkl', 'wb') as f:
    pickle.dump(best_model, f)
print("บันทึกโมเดลเรียบร้อย")
