
#!/usr/bin/env python3
import matplotlib.pyplot as plt
import pandas as pd
import os
import sys

def generate_learning_curves(csv_path, output_dir):
    """
    สร้างกราฟการเรียนรู้จากไฟล์ CSV และบันทึกเป็น PNG
    
    Args:
        csv_path: พาธของไฟล์ CSV ที่มีข้อมูล
        output_dir: โฟลเดอร์ที่จะบันทึกไฟล์ PNG
    """
    try:
        # ตรวจสอบว่าไฟล์ CSV มีอยู่จริง
        if not os.path.exists(csv_path):
            print(f"Error: CSV file not found: {csv_path}")
            return False
            
        # อ่านข้อมูลจากไฟล์ CSV
        data = pd.read_csv(csv_path)
        
        # ตรวจสอบว่ามีคอลัมน์ที่จำเป็น
        required_columns = ['Epoch', 'Loss', 'Accuracy']
        for col in required_columns:
            if col not in data.columns:
                print(f"Error: Required column '{col}' not found in CSV")
                return False
        
        # สร้างกราฟ
        plt.figure(figsize=(10, 6))
        
        # กราฟแสดง Loss
        plt.subplot(2, 1, 1)
        plt.plot(data['Epoch'], data['Loss'], 'r-', label='Loss')
        plt.title('Learning Curves')
        plt.ylabel('Loss')
        plt.grid(True)
        plt.legend()
        
        # กราฟแสดง Accuracy
        plt.subplot(2, 1, 2)
        plt.plot(data['Epoch'], data['Accuracy'], 'b-', label='Accuracy')
        plt.xlabel('Epoch')
        plt.ylabel('Accuracy')
        plt.grid(True)
        plt.legend()
        
        # บันทึกกราฟเป็นไฟล์ PNG
        plt.tight_layout()
        output_path = os.path.join(output_dir, 'learning_curves.png')
        plt.savefig(output_path)
        print(f"Plot saved as {output_path}")
        
        return True
    except Exception as e:
        print(f"Error generating plot: {e}")
        return False

if __name__ == "__main__":
    # เมื่อเรียกใช้งานโดยตรง สามารถส่งพารามิเตอร์ได้
    if len(sys.argv) != 3:
        print("Usage: python plot_generator.py <csv_file_path> <output_directory>")
        sys.exit(1)
        
    csv_path = sys.argv[1]
    output_dir = sys.argv[2]
    
    success = generate_learning_curves(csv_path, output_dir)
    sys.exit(0 if success else 1)
