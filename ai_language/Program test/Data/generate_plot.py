import matplotlib.pyplot as plt
import pandas as pd
import os

# อ่านข้อมูลจากไฟล์ CSV
data = pd.read_csv('Program test/Data/learning_curves_data.csv')

# สร้างกราฟ
plt.figure(figsize=(10, 6))
plt.subplot(2, 1, 1)
plt.plot(data['Epoch'], data['Loss'], 'r-', label='Loss')
plt.title('Learning Curves')
plt.ylabel('Loss')
plt.grid(True)
plt.legend()

plt.subplot(2, 1, 2)
plt.plot(data['Epoch'], data['Accuracy'], 'b-', label='Accuracy')
plt.xlabel('Epoch')
plt.ylabel('Accuracy')
plt.grid(True)
plt.legend()

# บันทึกกราฟเป็นไฟล์ PNG
plt.tight_layout()
plt.savefig('Program test/Data/learning_curves.png')
print('Plot saved as Program test/Data/learning_curves.png')
