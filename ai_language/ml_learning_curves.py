import matplotlib.pyplot as plt
import numpy as np

# สร้างข้อมูลจำลอง
epochs = 100
x = np.arange(1, epochs + 1)
loss = 0.82 - 0.77 * (1 - np.exp(-x/30))
accuracy = 0.65 + 0.3 * (1 - np.exp(-x/25))

# สร้างกราฟ
plt.figure(figsize=(12, 5))

# กราฟ Loss
plt.subplot(1, 2, 1)
plt.plot(x, loss, 'r-', linewidth=2)
plt.title('Loss vs. Epochs')
plt.xlabel('Epochs')
plt.ylabel('Loss')
plt.grid(True)

# กราฟ Accuracy
plt.subplot(1, 2, 2)
plt.plot(x, accuracy, 'b-', linewidth=2)
plt.title('Accuracy vs. Epochs')
plt.xlabel('Epochs')
plt.ylabel('Accuracy')
plt.grid(True)

plt.tight_layout()
plt.savefig('Program test/Picture/learning_curves.png')
plt.close()
