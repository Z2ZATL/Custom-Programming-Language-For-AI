
#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
ไฟล์ตัวอย่างการใช้งาน scikit-learn โดยตรงจาก Python
สำหรับทดสอบการทำงานของ scikit-learn โดยไม่ผ่าน AI Language
"""

import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, classification_report
from sklearn.decomposition import PCA

# โหลดข้อมูล
data = pd.read_csv('../datasets/classification_data.csv')
print('ข้อมูล:')
print(data.head())
print(f'รูปร่างข้อมูล: {data.shape}')

# แยก features และ target
X = data.iloc[:, :-1]
y = data.iloc[:, -1]

# แบ่งข้อมูลสำหรับการเทรนและทดสอบ
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# สร้างและเทรนโมเดล Random Forest
model = RandomForestClassifier(n_estimators=100, max_depth=5, random_state=42)
model.fit(X_train, y_train)

# ทำนายและประเมินโมเดล
y_pred = model.predict(X_test)
accuracy = accuracy_score(y_test, y_pred)
print(f'\nความแม่นยำ (Accuracy): {accuracy:.4f}')
print('\nรายงานการจำแนกประเภท:')
print(classification_report(y_test, y_pred))

# แสดงความสำคัญของ features
feature_importances = model.feature_importances_
features = X.columns
print('\nความสำคัญของ features:')
for feature, importance in zip(features, feature_importances):
    print(f'{feature}: {importance:.4f}')

# แสดงกราฟความสำคัญของ features
plt.figure(figsize=(10, 6))
sorted_idx = feature_importances.argsort()
plt.barh(range(len(sorted_idx)), feature_importances[sorted_idx])
plt.yticks(range(len(sorted_idx)), [features[i] for i in sorted_idx])
plt.xlabel('Feature Importance')
plt.title('Feature Importance in Random Forest Model')
plt.tight_layout()
plt.savefig('feature_importance.png')
print('บันทึกกราฟความสำคัญของ features ที่ feature_importance.png')

# แสดงกราฟ PCA สำหรับการมองเห็นข้อมูล
pca = PCA(n_components=2)
X_pca = pca.fit_transform(X)
plt.figure(figsize=(10, 8))
for i in np.unique(y):
    plt.scatter(X_pca[y == i, 0], X_pca[y == i, 1], label=f'Class {i}')
plt.legend()
plt.title('PCA 2D Visualization')
plt.tight_layout()
plt.savefig('pca_visualization.png')
print('บันทึกกราฟ PCA ที่ pca_visualization.png')

print('\nเสร็จสิ้นการทดสอบ scikit-learn')
